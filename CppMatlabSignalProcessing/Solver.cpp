#include "Solver.h"
#include <iostream>

Solver::Solver(
	const std::unique_ptr<double[]>& t,
	const std::unique_ptr<double[]>& imag_sig,
	const std::unique_ptr<double[]>& real_sig,
	const int pulse_len, int pulse_count)
{
	for (size_t i = 0; i < pulse_count; i++) 
	{
		auto t_values = std::make_unique<double[]>(pulse_len);
		memcpy(t_values.get(), &t.get()[i * pulse_len], pulse_len * sizeof(double));

		auto sig_values = std::make_unique<std::complex<double>[]>(pulse_len);
		for (size_t j = 0; j < pulse_len; j++)
		{
			auto realval = real_sig.get()[i * pulse_len + j];
			auto imagval = imag_sig.get()[i * pulse_len + j];
			sig_values[j] = std::complex<double>(realval, imagval);
		}
		mPulses.push_back({ std::move(t_values), std::move(sig_values) });
	}

	for (size_t j = 0; j < pulse_len; j++) 
	{
		std::cout << std::atan2(mPulses[0].sig[j].imag(), mPulses[0].sig[j].real()) << " ";
	}
}
