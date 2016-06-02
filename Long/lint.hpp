#ifndef LINT
#define LINT

#include <vector>
#include <tuple>
#include <iostream>

namespace lint {

	struct Lint {
	public:
		Lint(long long num = 0);

		Lint(const Lint& lint);

		Lint& operator=(const Lint& lint);

		~Lint();

		Lint& operator+=(const Lint& lint);

		Lint& operator-=(const Lint& lint);

		Lint& operator*=(const Lint& lint);

		Lint& operator/=(const Lint& lint);

		Lint& operator%=(const Lint& lint);

		Lint& operator<<=(int n);

		Lint& operator>>=(int n);

		Lint operator+() const;

		Lint operator-() const;
	
		bool isZero() const;

		bool isNegative() const;

		void doAbs();

		int compareAbs(const Lint& lint) const;

		int compare(const Lint& lint) const;

		void write(std::ostream* outStream = &std::cout) const;

		void read(std::istream* inStream = &std::cin);

		void rand(const Lint& m);

	private:
		bool negative;
		std::vector<int> digits;
		static const int base = 1000;
	
		void add(const Lint& lint);

		void subtract(const Lint& lint);

		void shift(int n);

		Lint inverse(int precision = 0) const;

		Lint innerInverse() const;
	};

	Lint operator+(Lint a, Lint const& b);

	Lint operator-(Lint a, Lint const& b);

	Lint operator*(Lint a, Lint const& b);

	Lint operator/(Lint a, Lint const& b);

	Lint operator%(Lint a, Lint const& b);

	Lint operator<<(Lint a, int n);

	Lint operator>>(Lint a, int n);

	bool operator==(const Lint& a, const Lint& b);

	bool operator!=(const Lint& a, const Lint& b);

	bool operator<(const Lint& a, const Lint& b);

	bool operator>(const Lint& a, const Lint& b);

	bool operator<=(const Lint& a, const Lint& b);

	bool operator>=(const Lint& a, const Lint& b);

	std::ostream& operator<<(std::ostream& outStream, const Lint& lint);

	std::istream& operator>>(std::istream& inStream, Lint& lint);

	Lint abs(Lint a);

	Lint gcd(const Lint& a, const Lint& b);

	std::tuple<Lint, Lint, Lint> diofant(const std::tuple<Lint, Lint, Lint>& a, const std::tuple<Lint, Lint, Lint>& b);

	Lint inverse(const Lint& a, const Lint& m);

} // lint

#endif // !LINT
