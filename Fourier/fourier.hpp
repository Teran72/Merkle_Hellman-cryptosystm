#ifndef FOURIER
#define FOURIER

#include <vector>

namespace fourier {
	struct complex {
		complex(double re = 0, double im = 0)
			: re(re)
			, im(im) {}

		int round();

		double re, im;
	};

	complex operator+(const complex &a, const complex &b);
	complex operator-(const complex &a, const complex &b);
	complex operator*(const complex &a, const complex &b);
	complex operator/(const complex &a, double l);

	void fft(const std::vector<complex> &as, std::vector<complex> &res, bool inverse);

} // fourier

#endif // !FOURIER
