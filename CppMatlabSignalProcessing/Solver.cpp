#include "solver.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include "../Kalman/kalman.h"
#include "nojitter.h"

#pragma warning( disable : 6387 )

Solver::Solver(const UDPtr& t, const UDPtr& imag_sig, 
	const UDPtr& real_sig, const int pulse_len, int pulse_count) : mPulseSize(pulse_len)
{
	for (size_t i = 0; i < pulse_count; i++) 
	{
		auto t_values = std::make_unique<double[]>(pulse_len);
		memcpy(t_values.get(), &t.get()[i * pulse_len], pulse_len * sizeof(double));

		auto sig_values = std::make_unique<std::complex<double>[]>(pulse_len);
		for (size_t j = 0; j < pulse_len; j++) {
			auto realval = real_sig.get()[i * pulse_len + j];
			auto imagval = imag_sig.get()[i * pulse_len + j];
			sig_values[j] = std::complex<double>(realval, imagval);
		}
		mPulses.push_back({ std::move(t_values), std::move(sig_values) });
	}
}

Data Solver::compute()
{
	for (auto& pulse : mPulses) {
		pulse.angle = std::make_unique<double[]>(mPulseSize);

		auto angle = [](const std::complex<double>& value) -> auto {
				auto result = std::atan2(value.imag(), value.real());
				return result * 180.0 / M_PI;
			};

		for (size_t i = 0; i < mPulseSize; i++){
			pulse.angle[i] = angle(pulse.sig[i]);
		}

		const int running_average_window = 2;
		const double maximum_angle_value = 180.0;
		const double jitter_angle_value = maximum_angle_value * 0.95;
		const int jitter_angle_window = 10;

		NoJitter noJitter(jitter_angle_value, maximum_angle_value, jitter_angle_window);
		noJitter.compute(pulse.angle.get(), mPulseSize, running_average_window);

		Kalman kalman(0.15, 0.25, 32, angle(pulse.sig[0]));
		for (size_t i = 0; i < mPulseSize; i++) {
			pulse.angle[i] = kalman.getFilteredValue(pulse.angle[i]);
		}
	}

	const int missing_part_begin = mPulses[0].t[mPulseSize - 1] + 1;
	const int missing_part_end = mPulses[1].t[0] - 1;

	const int abscissa_size = mPulseSize * 2 + missing_part_end - missing_part_begin;

	std::shared_ptr<double[]> abscissa(new double[abscissa_size]);
	const int abscissa_begin = mPulses[0].t[0];
	for (size_t i = 0; i < abscissa_size; i++) {
		abscissa[i] = abscissa_begin + i;
	}

	const int ordinate_size = abscissa_size;

	std::shared_ptr<double[]> ordinate(new double[ordinate_size]);
	for (size_t i = 0; i < ordinate_size; i++) {
		ordinate[i] = std::nan(0);
	}

	for (const auto& pulse : mPulses) {
		const int pulse_begin = pulse.t[0] - mPulses[0].t[0];
		for (size_t i = 0; i < mPulseSize; i++) {
			const int j = i + pulse_begin;
			if(j < ordinate_size)
				ordinate[j] = pulse.angle[i];
		}
	}

	std::vector<std::pair<double*, int>> convolution_kernels;
	for (const auto& pulse : mPulses) {
		convolution_kernels.push_back(std::pair<double*, int>(pulse.angle.get(), mPulseSize));
	}

	auto kernel_mul_results = prepareCorrelativity(ordinate.get(), ordinate_size, convolution_kernels);
	auto correlativityResult = computeCorrelativity(*kernel_mul_results.get(), ordinate_size, 180, -180, convolution_kernels);

	for (size_t i = 0; i < ordinate_size; i++) {
		if (std::isnan(ordinate[i])) 
			ordinate[i] = correlativityResult[i].value;
	}

	return { abscissa , ordinate };
}

std::shared_ptr<std::vector<std::shared_ptr<double[]>>> Solver::prepareCorrelativity(double* source, const int source_size,
	const std::vector<std::pair<double*, int>>& convolution_kernels, const int acceptable_window)
{
	auto comp_conv = [=]( double* y, double* kernel, const int kernel_len) -> double {
			double sum = 0.0;

			int size = 0;
			for (size_t i = 0; i < kernel_len; i++) {
				if(std::isnan(y[i])) continue;

				auto min = kernel[i] - acceptable_window;
				auto max = kernel[i] + acceptable_window;
				if (min < y[i] && max > y[i]) ++sum;

				++size;
			}
			return sum / size;
		};

	std::shared_ptr<std::vector<std::shared_ptr<double[]>>> kernel_mul_results(new std::vector<std::shared_ptr<double[]>>());
	auto featuring = [&, comp_conv](double* kernel, int kernel_size) -> auto {
		std::shared_ptr<double[]> mul_results(new double[source_size]);
		
		double max = 0.0;
		for (size_t i = 0; i < source_size; i++) {
			if (kernel_size > source_size - i)
				kernel_size = source_size - i;

			auto result = comp_conv(&source[i], kernel, kernel_size);
			if (max < result) max = result;

			mul_results[i] = result;
		}

		for (size_t i = 0; i < source_size; i++) {
			mul_results[i] /= max;
		}

		return mul_results;
	};

	for (const auto& kernel: convolution_kernels) {
		kernel_mul_results->push_back(featuring(kernel.first, kernel.second));
	}

	return kernel_mul_results;
}

std::shared_ptr<CorrelativityResult[]> Solver::computeCorrelativity(const std::vector<std::shared_ptr<double[]>>& mul_results, const int mul_result_len,
	const int min_value, const int max_value, const std::vector<std::pair<double*, int>>& convolution_kernels) 
{
	std::shared_ptr<CorrelativityResult[]> correlativityResult (new CorrelativityResult[mul_result_len]);

	auto correlativity_insert = [&](double* kernel, int kernel_len, double* kernel_mul_result) {
		for (size_t i = 0; i < mul_result_len; i++) {
			if (std::isnan(correlativityResult[i].ñorrelation) || correlativityResult[i].ñorrelation < kernel_mul_result[i]) {
				for (size_t j = 0; j < kernel_len; j++)
				{
					if (j + i < mul_result_len) {
						auto& insert = correlativityResult[j + i];
						insert.value = kernel[j];
						insert.ñorrelation = kernel_mul_result[i];
					}


				}
			}
		}
	};

	int kernel_id = 1;
	correlativity_insert(convolution_kernels[kernel_id].first, convolution_kernels[kernel_id].second, mul_results[kernel_id].get());

	return correlativityResult;
}
