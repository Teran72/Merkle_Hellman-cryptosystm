#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>
#include "..\Merkle Hellman\merkle_hellman.hpp"

using namespace std;
using namespace lint;
using namespace merkle_hellman;

int main(int argc, char *argv[]) {
//	srand(239);
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);	srand((int) ms.count());
	SecretKey sk;

	int n = atoi(argv[1]);

	sk.build(n);
	freopen((string("key_s_") + string(argv[1]) + string("_") + string(argv[2])).c_str(), "w", stdout);
	sk.write();
	freopen((string("key_p_") + string(argv[1]) + string("_") + string(argv[2])).c_str(), "w", stdout);	
	sk.getPublic().write();
	return 0;
/*
	
//	freopen("key_s_16_01", "r", stdin);
//	sk.read();
	
	PublicKey pk;
	freopen("key_p_16_01", "r", stdin);
	pk.read();

	vector<Lint> crypt;
	crypt = MerkleHellman::encrypt("Hi there!", pk);
//	for (int i = 0; i < (int) (crypt.size()); i++) {
//		cout << crypt[i] << " ";
//	}
//	cout << "\n";

	SecretKey sk;
	freopen("key_s_16_01", "r", stdin);
	sk.read();
	string res = MerkleHellman::decrypt(crypt, sk);
	cout << res;


	return 0;*/
}
