#include "solver.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include "../Kalman/kalman.h"
#include "nojitter.h"

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

void Solver::compute() 
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

		const int running_average_window = 3;
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

}
