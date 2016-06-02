#include <vector>
#include <tuple>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "lint.hpp"
#include "..\Fourier\fourier.hpp"

using namespace std;
using namespace lint;
using namespace fourier;

lint::Lint::Lint(long long num) {
	digits.resize(0);
	negative = num < 0;
	if (negative) {
		num *= -1ll;
	}		
	do {
		digits.push_back(num % base);
		num /= base;
	} while (num);
}

lint::Lint::Lint(const Lint& lint)
	: negative(lint.negative) {
	int n = lint.digits.size();
	digits.resize(n);
	for (int i = 0; i < n; i++) {
		digits[i] = lint.digits[i];
	}
}

Lint& lint::Lint::operator=(const Lint& lint) {
	if (&lint != this) {
		negative = lint.negative;
		int n = lint.digits.size();
		digits.resize(n);
		for (int i = 0; i < n; i++) {
			digits[i] = lint.digits[i];
		}
	}
	return *this;
}

lint::Lint::~Lint() {}

Lint& lint::Lint::operator+=(const Lint& lint) {
	if (negative ^ lint.negative) {
		subtract(lint);
	} else {
		add(lint);
	}
	return *this;
}

Lint& lint::Lint::operator-=(const Lint& lint) {
	if (negative ^ lint.negative) {
		add(lint);
	} else {
		subtract(lint);
	}
	return *this;
}

Lint& lint::Lint::operator*=(const Lint& lint) {
	if (isZero() || lint.isZero()) {
		negative = false;
		digits.resize(0);
		return *this;
	}
	negative ^= lint.negative;
	int len1 = digits.size();
	int len2 = lint.digits.size();
	if (true) {
		int n = 1;
		while (n < len1 + len2) {
			n <<= 1;
		}
		vector<complex> first(n);
		vector<complex> second(n);
		for (int i = 0; i < len1; i++) {
			first[i] = digits[i];
		}
		for (int i = 0; i < len2; i++) {
			second[i] = lint.digits[i];
		}
		fft(first, first, false);
		fft(second, second, false);
		for (int i = 0; i < n; i++) {
			first[i] = first[i] * second[i];
		}
		fft(first, first, true);
		digits.resize(n);
		int carry = 0;
        for (int i = 0; i < n; i++) {
        	int temp = carry + first[i].round();
        	carry = 0;
			if (temp >= base) {
				carry = temp / base;
				temp %= base;
			}
			digits[i] = temp;
        }
        if (carry) {
        	while (carry >= base) {
        		digits.push_back(carry % base);
        		carry /= base;
        	}
        }
        while (!digits.back()) {
        	digits.pop_back();
        }
	}
	return *this;
}

Lint& lint::Lint::operator/=(const Lint& lint) {
	if (lint.isZero()) {
		throw overflow_error("Divide by zero exception");
	}
	if (isZero()) {
		return *this;
	}
	int len1 = digits.size();
	int len2 = lint.digits.size();
	int n = len1 - len2 + 1;
	int precision = 1;
	while (precision < n) {
		precision <<= 1;
	}
//	cout << "prec = " << precision << ", n = " << n << "\n";
//	cout << "inv = " << lint.inverse(n) << "\n";
	Lint res = (*this * abs(lint).inverse(n)) >> (2 * precision - 1 - precision + len2);
//	cout << "res = " << res << "\n";
	res.negative = negative ^ lint.negative;
int cnt = 0;
	Lint prod = res * lint;
//	cout << "res = " << res << ", lint = " << lint << "\n";
//	cout << "prod = " << prod << "\n";
	while ((lint.negative && prod < *this) || (!lint.negative && prod > *this)) {
//		if (cnt > 100) {
//			printf("looped1\n");
//		}
		prod -= lint;
		res -= 1;
		cnt++;
	}
	prod += lint;
	while ((lint.negative && prod >= *this) || (!lint.negative && prod <= *this)) {
//		if (cnt > 100) {
			//printf("looped2\n");
//		}
		prod += lint;
		res += 1;
		cnt++;
	}
	*this = res;
//	cout << "cnt = " << cnt << "\n";
	return *this;
}

Lint& lint::Lint::operator%=(const Lint& lint) {
	*this -= (*this / lint) * lint;
	return *this;
}

Lint& lint::Lint::operator<<=(int n) {
	shift(n);
	return *this;
}

Lint& lint::Lint::operator>>=(int n) {
	shift(-n);
	return *this;
}

Lint lint::Lint::operator+() const {
	return *this;
}

Lint lint::Lint::operator-() const {
	if (isZero()) {
		return *this;
	} else {
		Lint res(*this);
		res.negative ^= true;
		return res;
	}
}

	
bool lint::Lint::isZero() const {
	return (digits.size() == 0) || (digits.size() == 1 && digits[0] == 0);
}

bool lint::Lint::isNegative() const {
	return negative;
}

int lint::Lint::compareAbs(const Lint& lint) const {
	int len1 = digits.size();
	int len2 = lint.digits.size();
	for (int i = max(len1, len2) - 1; i >= 0; i--) {
		int dig1 = (i < len1 ? digits[i] : 0);
		int dig2 = (i < len2 ? lint.digits[i] : 0);
		if (dig1 != dig2) {
			return dig1 < dig2 ? -1 : 1;
		}
	}
	return 0;
}

void lint::Lint::doAbs() {
	negative = false;
}

int lint::Lint::compare(const Lint& lint) const {
	int coeff = negative ? -1 : 1;
	if (negative ^ lint.negative) {
		return coeff;
	}		
	return coeff * compareAbs(lint);
}

void lint::Lint::write(ostream* outStream) const {
	if (negative) {
		(*outStream) << "-";
	}
	if (!digits.size()) {
		(*outStream) << "0";
	} else {
		for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
			int dig = *it;
			if (it != digits.rbegin()) {
				int size = base / 10;
				while (size > 1 && dig < size) {
					(*outStream) << "0";
					size /= 10;
				}
			}				
			(*outStream) << dig;
		}
	}		
}

void lint::Lint::read(istream* inStream) {
	char c;
	do {
		(*inStream).get(c);
	} while (c == ' ' || c == '\t' || c == '\n');
	if (c == '-') {
		negative = true;
		(*inStream).get(c);
	} else {
		negative = false;
	}
	digits.resize(0);
	vector<int> reversed(0);
	int temp = 0;
	int pos = 1;
	while ('0' <= c && c <= '9') {
		if (pos >= base) {
			reversed.push_back(temp);
			temp = 0;
			pos = 1;
		}
		temp = temp * 10 + (c - '0');
		pos *= 10;
		(*inStream).get(c);
	}
	(*inStream).unget();
	int n = reversed.size();
	int npos = base / pos;
	digits.resize(n);
	for (int i = n - 1; i >= 0; i--) {
		int next = reversed[i];
		digits[n - i - 1] = temp + next % npos * pos;
		temp = next / npos;
	}
	digits.push_back(temp);
	while (!digits.back()) {
		digits.pop_back();
	}
	if (!digits.size()) {
		negative = false;
	}
}

void lint::Lint::rand(const Lint& m) {
	int size = m.digits.size() + 1;
	digits.resize(size);
	for (int i = 0; i < size; i++) {
		digits[i] = std::rand() % base;
		if (digits[i] == 0 && i + 1 == size) {
			digits[i]++;
		}
	}
	*this %= m;
}

void lint::Lint::add(const Lint& lint) {
	int lintLen = lint.digits.size();
	int len = max((int) (digits.size()), lintLen);
	digits.resize(len);
	int carry = 0;
	for (int i = 0; i < len; i++) {
		int lintDigit = 0;
		if (i < lintLen) {
			lintDigit = lint.digits[i];
		} else if (carry == 0) {
			break;
		}
		digits[i] += carry + lintDigit;
		if (digits[i] >= base) {
			digits[i] -= base;
			carry = 1;
		} else {
			carry = 0;
		}
	}
	if (carry) {
		digits.push_back(1);
	}
}

void lint::Lint::subtract(const Lint& lint) {
	bool direction = compareAbs(lint) >= 0;
	int lintLen = lint.digits.size();
	int len = max((int) (digits.size()), lintLen);
	digits.resize(len);
	int carry = 0;
	for (int i = 0; i < len; i++) {
		int lintDigit = 0;
		if (i < lintLen) {
			lintDigit = lint.digits[i];
		} else {
			if (carry == 0) {
				break;
			}
		}
		int temp = (digits[i] - lintDigit) * (direction ? 1 : -1) - carry;
		if (temp < 0) {
			carry = 1;
			temp += base;
		} else {
			carry = 0;
		}
		digits[i] = temp;
	}
	while (digits.size() && !digits.back()) {
		digits.pop_back();
	}
	if (isZero()) {
		negative = false;
	} else if (!direction) {
		negative ^= true;
	}
}

void lint::Lint::shift(int s) {
	if (s == 0) {
		return;
	}
	if (isZero()) {
		return;
	}
	int n = digits.size();
	if (s > 0) {
		digits.resize(n + s);
		for (int i = n - 1; i >= 0; i--) {
			digits[i + s] = digits[i];
		}
		for (int i = 0; i < s; i++) {
			digits[i] = 0;
		}
	} else if (s < 0) {
		for (int i = 0; i - s < n; i++) {
			digits[i] = digits[i - s];
		}
		digits.resize(max(n + s, 0));
	}
}

Lint lint::Lint::inverse(int precision) const {
	if (isZero()) {
		throw overflow_error("Divide by zero exception");
	}
//	if (precision == 0) {
//		precision = digits.size();
//	}
	int temp = precision;
	precision = 1;
	while (precision < temp) {
		precision <<= 1;
	}
//	cout << "prec = " << precision << ", temp = " << temp << "\n";
//	cout << "here " << *this << "\n" << (abs((*this) << (precision - digits.size()))) << "\n";
	return (abs((*this) << (precision - digits.size()))).innerInverse();
}

Lint lint::Lint::innerInverse() const {
	int precision = digits.size();
//	cout << "in inverse: " << *this << ", prec = " << precision << "\n";
	if (precision == 1) {
//		cout << "final answer is " << base / digits[0] << "\n";
		return base / digits[0];
	} else {
		bool firstTime = true;
		int precisionNew = precision >> 1;
		Lint p2 = (*this) >> precisionNew;
		Lint a;
		Lint ap;
		while (true) {			
			if (firstTime) {
				a = p2.innerInverse();
				ap = a * *this;
			}
			a = ((2 * a) << (firstTime ? precisionNew : 0)) - ((a * ap) >> ((firstTime ? precision : 2 * precision) - 1));
			ap = a * *this;
			firstTime = false;
//			cout << "answer is " << a << "\n";
//			cout << "difference = " << abs((Lint(1) << (2 * precision - 1)) - a * p) << "\n";
			if (compareAbs((Lint(1) << (2 * precision - 1)) - ap) == 1) {
				break;
			}
		}		
//		cout << "final answer is " << a << "\n";
		return a;
	}
	return 0;
}

Lint lint::operator+(Lint a, Lint const& b) {
	return a += b;
}

Lint lint::operator-(Lint a, Lint const& b) {
	return a -= b;
}

Lint lint::operator*(Lint a, Lint const& b) {
	return a *= b;
}

Lint lint::operator/(Lint a, Lint const& b) {
	return a /= b;
}

Lint lint::operator%(Lint a, Lint const& b) {
	return a %= b;
}

Lint lint::operator<<(Lint a, int n) {
	return a <<= n;
}

Lint lint::operator>>(Lint a, int n) {
	return a >>= n;
}

bool lint::operator==(const Lint& a, const Lint& b) {
	return a.compare(b) == 0;
}

bool lint::operator!=(const Lint& a, const Lint& b) {
	return a.compare(b) != 0;
}

bool lint::operator<(const Lint& a, const Lint& b) {
	return a.compare(b) == -1;
}

bool lint::operator>(const Lint& a, const Lint& b) {
	return a.compare(b) == 1;
}

bool lint::operator<=(const Lint& a, const Lint& b) {
	return a.compare(b) != 1;
}

bool lint::operator>=(const Lint& a, const Lint& b) {
	return a.compare(b) != -1;
}

ostream& lint::operator<<(ostream& outStream, const Lint& lint) {
	lint.write(&outStream);
	return outStream;
}

istream& lint::operator>>(istream& inStream, Lint& lint) {
	lint.read(&inStream);
	return inStream;
}

Lint lint::abs(Lint a) {
	a.doAbs();
	return a;
}

Lint lint::gcd(const Lint& a, const Lint& b) {
//	cout << "in gcd: a = " << a << ", b = " << b << "\n";
	if (b.isZero()) {
		return a;
	} else {
		return gcd(b, a % b);
	}
}

std::tuple<Lint, Lint, Lint> lint::diofant(const std::tuple<Lint, Lint, Lint>& a, const std::tuple<Lint, Lint, Lint>& b) {
	if (get<0>(b).isZero()) {
		return a;
	} else {
		Lint val = get<0>(a) / get<0>(b);
		return diofant(b, make_tuple(get<0>(a) % get<0>(b), get<1>(a) - val * get<1>(b), get<2>(a) - val * get<2>(b)));
	}
}

Lint lint::inverse(const Lint& a, const Lint& m) {
	auto ans = diofant(make_tuple(a, 1, 0), make_tuple(m, 0, 1));
	if (m.isZero() || get<0>(ans) != 1) {
		throw overflow_error("First argument is uninvertable");
	}
	Lint w = get<1>(ans);
	return w % m;
}
