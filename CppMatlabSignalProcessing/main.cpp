#include <iostream>
#include "../PhaseSignalGen/PhaseSignalGen.h"
#pragma comment(lib, "mclmcrrt.lib")
#pragma comment(lib, "../PhaseSignalGen/PhaseSignalGen.lib")

#include "solver.h"
#include "gui.h"

int main()
{
	if (!mclInitializeApplication(NULL, 0)) {
		std::cerr << "could not initialize the application properly" << std::endl;
		std::cin.get();
		return -1;
	}

	if (!PhaseSignalGenInitialize()) {
		std::cerr << "could not initialize the library properly" << std::endl;
		std::cin.get();
		return -1;
	}

	mwArray t, imag_sig, real_sig, pulse_len, pulse_count;
	PhaseSignalGen(5, t, imag_sig, real_sig, pulse_len, pulse_count);

	auto __memcpy = [](const mwArray& arr) -> auto {
		auto size = arr.NumberOfElements();
		auto results = std::make_unique<double[]>(size);
		arr.GetData(results.get(), size);
		return std::move(results);
	};

	auto mem_t           = __memcpy(t);
	auto mem_imag_sig    = __memcpy(imag_sig);
	auto mem_real_sig    = __memcpy(real_sig);
	auto mem_pulse_len   = __memcpy(pulse_len);
	auto mem_pulse_count = __memcpy(pulse_count);

	Solver solver(mem_t, mem_imag_sig, mem_real_sig, mem_pulse_len[0], mem_pulse_count[0]);

	solver.compute();

	Gui gui;
	gui.run();

	PhaseSignalGenTerminate();
	if (!mclTerminateApplication()) {
		std::cerr << "unexpected error during termination" << std::endl;
		std::cin.get();
		return -1;
	}
}
