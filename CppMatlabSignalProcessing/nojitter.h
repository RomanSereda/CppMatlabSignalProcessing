#pragma once
#include <vector>
#include <cmath>

#pragma warning( disable : 6387 )

class NoJitter {
public:
	NoJitter(const double jitter_value, const double max_value, const int jitter_len)
		: mJitterLen(jitter_len), mMaxValue(max_value), mJitterValue(jitter_value){

		mValues.reserve(jitter_len);
	}

	void compute(double* values, const int values_len, const int post_running_average_window = 0) {
		srand(time(NULL));

		toNaN(values, values_len);
		tofix(values, values_len);

		if (post_running_average_window) {
			for (size_t j = 0; j < values_len; j++) {
				double sum = 0;
				for (size_t i = 0; i < post_running_average_window; i++) {
					if (i + j == values_len) return;
					sum += values[i + j];
				}
				sum /= post_running_average_window;
				values[j] = sum;
			}
		}
	}

private:
	const double mJitterValue;
	const double mMaxValue;
	const int mJitterLen;
	
	std::vector<double> mValues;
	bool mIsJittContinued = false;
	bool mIsFirstJitter = true;

	void toNaN(double* values, const int values_len) {
		for (size_t j = 0; j < values_len; j++) {
			mValues.push_back(abs(values[j]));
			if (mValues.size() > mJitterLen)
				mValues.erase(mValues.begin());

			mIsJittContinued = false;
			for (size_t i = 0; i < mValues.size(); i++) {
				if (mValues[i] >= mJitterValue)
					mIsJittContinued = true;
			}

			if (mIsJittContinued)
				values[j] = mIsFirstJitter ? mMaxValue : std::nan(0);
			else mValues.clear();

			mIsFirstJitter = mIsJittContinued ? false : true;
		}
	}

	void tofix(double* values, const int values_len) {
		int jitter_begin = -1;

		for (size_t j = 0; j < values_len; j++) {
			if (std::isnan(values[j])) {
				if (jitter_begin == -1) 
					jitter_begin = j;

				if (j + 1 == values_len) {
					for (size_t i = jitter_begin; i < values_len; i++) {
						values[i] = mMaxValue;
					}
					break;
				}

				if (!std::isnan(values[j + 1])) {
					const int running_average_window = 16;

					double mix_value = values[j + 1];
					if (j + running_average_window + 1 < values_len) {
						mix_value = 0;
						for (size_t k = 0; k < running_average_window; k++) {
							mix_value += values[j + 1 + k];
						}
						mix_value /= running_average_window;
					}

					const int len = j - jitter_begin;
					const int val = mMaxValue - mix_value;
					const double val_per_tick = val / len;

					for (size_t i = 0; i < len + 1; i++) {
						auto value = double(mMaxValue) - val_per_tick * i;
						values[i + jitter_begin] = value + value * (0.025 * (rand()%10 - 5));
					}

					jitter_begin = -1;
				}
			}
		}
	}
};