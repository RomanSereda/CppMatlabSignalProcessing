#pragma once
#include <vector>
#include <memory>
#include <complex>
#include <functional>

using UDPtr = std::unique_ptr<double[]>;

struct CorrelativityResult {
	double value = std::nan(0);
	double ñorrelation = std::nan(0);
};

struct Data {
	std::shared_ptr<double[]> abscissa;
	std::shared_ptr<double[]> ordinate;
};

class Solver {
public:
	Solver(const UDPtr& t, const UDPtr& imag_sig, 
		const UDPtr& real_sig, const int pulse_len, int pulse_count);
	
	Data compute();

private:
	struct Pulse {
		std::unique_ptr<double[]> t;
		std::unique_ptr<std::complex<double>[]> sig;
		std::unique_ptr<double[]> angle;
	};
	std::vector<Pulse> mPulses;
	int mPulseSize = -1;

	std::shared_ptr<std::vector<std::shared_ptr<double[]>>> prepareCorrelativity(double* source, const int source_size,
		const std::vector<std::pair<double*, int>>& convolution_kernels, const int acceptable_window = 5);

	std::shared_ptr<CorrelativityResult[]> computeCorrelativity(const std::vector<std::shared_ptr<double[]>>& mul_results, const int mul_result_len,
		const int min_value, const int max_value, const std::vector<std::pair<double*, int>>& convolution_kernels);
};
