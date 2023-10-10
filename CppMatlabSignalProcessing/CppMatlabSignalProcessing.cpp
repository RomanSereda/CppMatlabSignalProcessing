#include <iostream>
#include "../PhaseSignalGen/PhaseSignalGen.h"
#include "../PlotView/PlotView.h"
#pragma comment(lib, "mclmcrrt.lib")
#pragma comment(lib, "../PhaseSignalGen/PhaseSignalGen.lib")
#pragma comment(lib, "../PhaseSignalPlot/PhaseSignalPlot.lib")
#pragma comment(lib, "../PlotView/PlotView.lib")

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

	if (!PlotViewInitialize()) {
		std::cerr << "could not initialize the library properly" << std::endl;
		std::cin.get();
		return -1;
	}

	mwArray t, sig;
	PhaseSignalGen(2, t, sig);

	auto t_size = t.NumberOfElements();
	std::unique_ptr<double[]> t_results = std::make_unique<double[]>(t_size);;
	t.GetData(t_results.get(), t_size);

	auto sig_size = sig.NumberOfElements();
	std::unique_ptr<double[]> sig_results = std::make_unique<double[]>(sig_size);;
	sig.GetData(sig_results.get(), sig_size);
		
	PlotView(t, sig);

	system("pause");
	PlotViewTerminate();
	PhaseSignalGenTerminate();
	if (!mclTerminateApplication()) {
		std::cerr << "unexpected error during termination" << std::endl;
		std::cin.get();
		return -1;
	}
}
