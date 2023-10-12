#include "solver.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include "kalman.h"

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

		Kalman kalman;

		auto sig = pulse.sig.get();
		for (size_t i = 0; i < mPulseSize; i++){
			auto result = std::atan2(mPulses[0].sig[i].imag(), mPulses[0].sig[i].real());
			result = result * 180.0 / M_PI;
			result = kalman.getAngle(result, 0.02, 0.05 * i);

			std::cout << result << " ";
		}
	}

}
