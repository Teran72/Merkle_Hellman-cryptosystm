#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include "..\Long\lint.hpp"
#include "..\Fourier\fourier.hpp"
#include "merkle_hellman.hpp"

using namespace std;
using namespace lint;
using namespace fourier;
using namespace merkle_hellman;

merkle_hellman::SecretKey::SecretKey() {}

merkle_hellman::SecretKey::SecretKey(const SecretKey& sk)
	: size(sk.size)
	, m(sk.m)
	, w(sk.w)
	, wi(sk.wi) {
	we = vector<Lint>(size);
	perm = vector<int>(size);
    for (int i = 0; i < size; i++) {
    	we[i] = sk.we[i];
    	perm[i] = sk.perm[i];
    }
}

SecretKey& merkle_hellman::SecretKey::operator=(const SecretKey& sk) {
	if (&sk != this) {
		size = sk.size;
		m = sk.m;
		w = sk.w;
		wi = sk.wi;
		we = vector<Lint>(size);
		perm = vector<int>(size);
	    for (int i = 0; i < size; i++) {
	    	we[i] = sk.we[i];
	    	perm[i] = sk.perm[i];
	    }
	}
	return *this;
}

merkle_hellman::SecretKey::~SecretKey() {}

void merkle_hellman::SecretKey::build(int sz) {
	size = sz;
	wi = 0;
	we = vector<Lint>(size);
	perm = vector<int>(size);

	Lint nzeros = 1;
	for (int i = 0; i < sz; i++) {
		nzeros *= 2;
	}
	Lint lst = nzeros;
	for (int i = 0; i < size; i++) {
		perm[i] = i;
		Lint r;
		r.rand(nzeros);
		we[i] = lst - r;
		lst *= 2;
	}
	lst *= 4;
	Lint r;
	r.rand(2 * nzeros - 1);
	m = lst - r - 1;
	w = 0;


//	Lint sum = 0;
//	for (int i = 0; i < size; i++) {
//		perm[i] = i;
//		we[i] = 1 + sum + rand() % 20;
//		sum += we[i];
//	}
//	m = sum + rand() % 100;
//	w = 0;
	int cnt = 0;
	while (1) {
		cnt++;
		w.rand(m - 3);
		w += 2;
		if (gcd(w, m) == 1) {
			wi = inverse(w, m);
			break;
		}
	}
	cerr << "cnt = " << cnt << "\n";
	random_shuffle(perm.begin(), perm.end());
}

PublicKey merkle_hellman::SecretKey::getPublic() {
	vector<Lint> temp(size);
	for (int i = 0; i < size; i++) {
		temp[perm[i]] = (we[i] * w) % m;
	}
	return PublicKey(temp);
}

void merkle_hellman::SecretKey::write(std::ostream* outStream) const {
	(*outStream) << size << " "  << m << " " << w << " " << wi << "\n";
	for (int i = 0; i < size; i++) {
		(*outStream) << we[i] << " ";
	}
	(*outStream) << "\n";
	for (int i = 0; i < size; i++) {
		(*outStream) << perm[i] << " ";
	}
	(*outStream) << "\n";
}

void merkle_hellman::SecretKey::read(std::istream* inStream) {
	(*inStream) >> size >> m >> w >> wi;
	we.resize(size);
	perm.resize(size);
	for (int i = 0; i < size; i++) {
		(*inStream) >> we[i];
	}
	for (int i = 0; i < size; i++) {
		(*inStream) >> perm[i];
	}
}


merkle_hellman::PublicKey::PublicKey() {}

merkle_hellman::PublicKey::PublicKey(const PublicKey& pk)
	: size(pk.size) {
	we = vector<Lint>(size);
    for (int i = 0; i < size; i++) {
    	we[i] = pk.we[i];
    }
}

merkle_hellman::PublicKey::PublicKey(const vector<Lint>& ws) {
	size = ws.size();
	we = vector<Lint>(size);
    for (int i = 0; i < size; i++) {
    	we[i] = ws[i];
    }
}

PublicKey& merkle_hellman::PublicKey::operator=(const PublicKey& pk) {
	if (&pk != this) {
		size = pk.size;
		we = vector<Lint>(size);
	    for (int i = 0; i < size; i++) {
	    	we[i] = pk.we[i];
	    }
	}
	return *this;
}

merkle_hellman::PublicKey::~PublicKey() {}

void merkle_hellman::PublicKey::write(std::ostream* outStream) const {
	(*outStream) << size << "\n";
	for (int i = 0; i < size; i++) {
		(*outStream) << we[i] << " ";
	}
	(*outStream) << "\n";
}

void merkle_hellman::PublicKey::read(std::istream* inStream) {
	(*inStream) >> size;
	we.resize(size);
	for (int i = 0; i < size; i++) {
		(*inStream) >> we[i];
	}
}

vector<Lint> merkle_hellman::MerkleHellman::encrypt(const string& message, const PublicKey& pk) {
	int chunkSize = pk.size;
	int len = message.size();

	vector<Lint> res(0);
	Lint tempSum = 0;
	int pos = 0;

	for (int i = 0; i < len; i++) {
		int c = message[i];
		for (int j = 0; j < 8; j++) {
			if (c & 1) {
				tempSum += pk.we[pos];
			}
			pos++;
			c >>= 1;
			if (pos == chunkSize) {
				res.push_back(tempSum);
				pos = 0;
				tempSum = 0;
			}
		}
	}
	if (pos) {
		res.push_back(tempSum);
	}
	return res;
}

string merkle_hellman::MerkleHellman::decrypt(const vector<Lint>& crypt, const SecretKey& sk) {
	string res;
	int chunkSize = sk.size;
	int len = crypt.size();
	vector<int> buffer(chunkSize);

	int pos = 0;
	int c = 0;
	int mask = 1;

	for (int i = 0; i < len; i++) {
		Lint sum = crypt[i];
		sum *= sk.wi;
		sum %= sk.m;
		for (int j = chunkSize - 1; j >= 0; j--) {
			Lint next = sum - sk.we[j];
			if (next >= 0) {
				sum = next;
				buffer[sk.perm[j]] = 1;
			} else {
				buffer[sk.perm[j]] = 0;
			}
		}
		for (int j = 0; j < chunkSize; j++) {
			if (buffer[j]) {
				c |= mask;
			}
			pos++;
			if (pos == 8) {
				if (i == len - 1 && c == 0) {
					break;
				}
//				cerr << "Here c = " << c << "\n";
				res.push_back((char) (c));
				c = 0;
				mask = 1;
				pos = 0;
			} else {
				mask <<= 1;
			}
		}
	}
	return res;
}
