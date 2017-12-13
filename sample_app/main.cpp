#include <cmath>
#include <cstdio>
#include <cinttypes>

#include "../wave_analyze/Oscilator.h"

using namespace yr;
using namespace yr::waveanalyze;

#define PI 3.1415926535897932384626433
#define resonance_freq 10000.0
#define Q 1000.0
#define max_count 100

double search_max_freq(
	const double freq
	)
{
	double range = resonance_freq / Q / 5.0;
	double max_freq = freq;
	const double zeta = 1.0 / (2.0 * Q);

	while (0.00000000001 < range) {
		double max_amp = 0.0;
		double max_amp_freq = 0.0;

		printf("\tsearch %3.16f - %3.16f\n",
			max_freq - range / 2.0,
			max_freq - range / 2.0 + (range / max_count) * max_count
			);

		for (int count = 0; count <= max_count; count++) {
			Oscilator oscilator(44100, freq * 2.0 * PI, zeta);

			double trial_freq = max_freq - range / 2.0 + range / max_count * count;

			for (uint64_t i = 0; i < 44100 * 10; i++) {
				double force = cos(i / 44100.0 * trial_freq * 2.0 * PI);
				oscilator.InputExternalForce(force);
			}

			double amplitude = oscilator.GetAmplitude();
			if (max_amp < amplitude) {
				max_amp = amplitude;
				max_amp_freq = trial_freq;
			}
		}

		printf("\tfreq %3.16f max_freq %3.16f max_amp_freq %3.16f max_amp %e range %e\n",
			freq, max_freq, max_amp_freq, max_amp, range
			);

		if (max_freq - range / 2.0 == max_amp_freq) {
			max_freq -= range / 4.0;
		}
		else if (max_freq - range / 2.0 + (range / max_count) * max_count == max_amp_freq) {
			max_freq += range / 4.0;
		}
		else {
			max_freq = max_amp_freq;
			range /= 2.0;
		}
	}

	return max_freq;
}

extern int count;

int main(int argc, char** argv)
{
	double zeta = 1.0 / (2.0 * Q);
	double freq = resonance_freq;
	
#if 1
	Oscilator oscilator(44100, freq * 2.0 * PI, 0.0);

	oscilator.InputExternalForce(1.0);
	for (unsigned long long i = 0; i < 44100LL * 10LL; i++) {
		oscilator.InputExternalForce(0.0);
	}

	printf("count : %d\n", count);
	printf("freq : %f\n", count / 10000000.0);

#endif

#if 0
	double diff = resonance_freq / Q / 5.0;
	double freq_diff = diff;

	double freq_theory_inv = 440.0 * sqrt(1.0 - 2.0 * pow(zeta, 2.0));
	double freq_theory = 440.0 / sqrt(1.0 - 2.0 * pow(zeta, 2.0));
	printf("result : %3.16f %3.16f\n", freq_theory_inv, freq_theory);

	bool is_ascending = false;

	while (0.00000000001 < diff || 0.00000000001 < freq_diff) {
		double max_freq = search_max_freq(freq);
		freq_diff = fabs(resonance_freq - max_freq);
		printf("freq %3.16f max_freq %3.16f\n", freq, max_freq);

		if (max_freq < resonance_freq) {
			if (!is_ascending) {
				diff /= 2.0;
				freq += diff;
				is_ascending = true;
			}
			else {
				freq += diff;
			}
		}
		else if (resonance_freq < max_freq) {
			if (is_ascending) {
				diff /= 2.0;
				freq -= diff;
				is_ascending = false;
			}
			else {
				freq -= diff;
			}
		}
	}

#endif

	return 0;
}

