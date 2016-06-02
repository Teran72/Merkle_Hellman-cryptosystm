#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include "fourier.hpp"

using namespace std;
using namespace fourier;

complex fourier::operator+(const complex &a, const complex &b) {
	return complex(a.re + b.re, a.im + b.im);
}

complex fourier::operator-(const complex &a, const complex &b) {
	return complex(a.re - b.re, a.im - b.im);
}

complex fourier::operator*(const complex &a, const complex &b) {
	return complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

complex fourier::operator/(const complex &a, double l) {
	return complex(a.re / l, a.im / l);
}

int fourier::complex::round() {
	return (int (re + 0.5) - ((re + 0.5 < 0) ? 1 : 0));
}

void fourier::fft(const vector<complex> &as, vector<complex> &res, bool inverse) {
	int n = 1;
	int len = as.size();
	while (n < len) {
		n <<= 1;
	}

	vector<int> perm(n);
	perm[0] = 0;
	int str_m = 1;
	int inv_m = n >> 1;
	for (int i = 1; i < n; i++) {
		if (i > 1 && (i & (i - 1)) == 0) {
			str_m <<= 1;
			inv_m >>= 1;
		}
		perm[i] = inv_m | perm[i ^ str_m];
	}
	res.resize(n);
	vector<bool> was(n, false);
	for (int j = 0; j < n; j++) {
		if (!was[j]) {
			complex dump = (j < len) ? as[j] : complex(0);
			int i = j;
			int ni = perm[i];
			for (; !was[ni];) {
				res[i] = (ni < len) ? as[ni] : complex(0);
				was[i] = true;
				i = ni;
				ni = perm[i];
			}
			if (!was[i]) {
				res[i] = dump;
	            was[i] = true;
			}            
		}
	}
	
	vector<complex> ws(n);
	int half = n / 2;
	for (int i = 0; i < n; i++) {
		double x = 2 * M_PI * i / n * (inverse ? -1 : 1);
		if (i < half) {
			ws[i].re = cos(x);
			ws[i].im = sin(x);
		} else {
			ws[i].re = -ws[i - half].re;
			ws[i].im = -ws[i - half].im;
		}
	}
	ws.push_back(ws[0]);

	int step = half;
	for (int m = 1; m < n; m <<= 1) {
		int i = 0;
		for (int j = 0; j < n; j++) {
			complex y0 = res[j];
			complex y1 = res[j + m] * ws[i * step];
			res[j] = y0 + y1;
			res[j + m] = y0 - y1;
			if (++i == m) {
				j += m;
				i = 0;
			}
		}
		step >>= 1;
	}
	if (inverse) {
		for (int i = 0; i < n; i++) {
			res[i] = res[i] / n;
		}
	}
}
