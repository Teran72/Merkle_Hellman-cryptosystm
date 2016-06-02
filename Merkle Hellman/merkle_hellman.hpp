#ifndef MERKLE_HELLMAN
#define MERKLE_HELLMAN

#include <iostream>
#include <vector>
#include <string>
#include "..\Long\lint.hpp"

namespace merkle_hellman {

	struct PublicKey {
	public:
		PublicKey();

		PublicKey(const PublicKey& pk);

		PublicKey(const std::vector<lint::Lint>& ws);

		PublicKey& operator=(const PublicKey& pk);

		~PublicKey();

		void write(std::ostream* outStream = &std::cout) const;

		void read(std::istream* inStream = &std::cin);

		int size;
		std::vector<lint::Lint> we;
	};

	struct SecretKey {
	public:
		SecretKey();

		SecretKey(const SecretKey& sk);

		SecretKey& operator=(const SecretKey& sk);

		~SecretKey();

		void build(int size);

		PublicKey getPublic();

		void write(std::ostream* outStream = &std::cout) const;

		void read(std::istream* inStream = &std::cin);

		int size;
		lint::Lint m, w, wi;
		std::vector<lint::Lint> we;
		std::vector<int> perm;
	};

	struct MerkleHellman {
	public:
		static std::vector<lint::Lint> encrypt(const std::string& message, const PublicKey& pk);

		static std::string decrypt(const std::vector<lint::Lint>& crypt, const SecretKey& sk);
	};

} // merkle_hellman

#endif // !MERKLE_HELLMAN
