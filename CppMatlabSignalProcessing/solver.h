#pragma once
#include <vector>
#include <memory>
#include <complex>
#include <functional>

using UDPtr = std::unique_ptr<double[]>;

class Solver {
public:
	Solver(const UDPtr& t, const UDPtr& imag_sig, 
		const UDPtr& real_sig, const int pulse_len, int pulse_count);
	
	void compute();

private:
	struct Pulse {
		std::unique_ptr<double[]> t;
		std::unique_ptr<std::complex<double>[]> sig;
		std::unique_ptr<double[]> angle;
	};
	std::vector<Pulse> mPulses;
	int mPulseSize = -1;
};
