#include <iostream>
#include "lint.hpp"

using namespace std;
using namespace lint;

int main() {
	freopen("data.in", "r", stdin);
//	int ia, ib;
//	cin >> ia >> ib;                 
//	Lint a(ia);
	Lint a;
	Lint b;
//	b = ib;
	cin >> a >> b;

	cout << "a = " << a << ", b = " << b << "\n";
//	cout << "a / b = " << (a / b) << "\n";
//	cout << "a % b = " << (a % b) << "\n";

//	cout << "gcd = " << gcd(a, b) << "\n";
//	auto ans = diofant(make_tuple(a, 1, 0), make_tuple(b, 0, 1));
//	cout << get<0>(ans) << "\n";
//	cout << get<1>(ans) << "\n";
//	cout << get<2>(ans) << "\n";
	cout << inverse(b, a) << "\n";

//	(a * b).write();
//	cout << " ";
//	(a - b).write();
//	cout << "\n";

	return 0;
}
