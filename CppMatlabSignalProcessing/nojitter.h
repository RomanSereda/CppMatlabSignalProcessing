#pragma once
#include <vector>

class NoJitter {
public:
	NoJitter(const double jitter_value, const double max_value, const int jitter_len);
	void compute(double* values, const int values_len, const int post_running_average_window = 0);

private:
	const double mJitterValue;
	const double mMaxValue;
	const int mJitterLen;
	
	std::vector<double> mValues;
	bool mIsJittContinued = false;
	bool mIsFirstJitter = true;

	void toNaN(double* values, const int values_len);
	void tofix(double* values, const int values_len);
};