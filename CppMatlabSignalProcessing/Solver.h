#pragma once
#include <vector>
#include <memory>
#include <complex>

class Solver {
public:
	Solver(
		const std::unique_ptr<double[]>& t,
		const std::unique_ptr<double[]>& imag_sig,
		const std::unique_ptr<double[]>& real_sig,
		const int pulse_len, int pulse_count);
private:
	struct Pulse {
		std::unique_ptr<double[]> t;
		std::unique_ptr<std::complex<double>[]> sig;
	};
	std::vector<Pulse> mPulses;
};
