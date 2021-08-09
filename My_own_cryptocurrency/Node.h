// Standard dependencies
#include <iostream>

// My own dependencies
#include "DB_operations.h"
#include "Wallet.h"
#include "Structures.h"

// Local dependencies
#include "sha.h"
#include "eccrypto.h"
#include "eccrypto.h"
#include "hex.h"
#include "files.h"

/*
std::string destination,
unsigned int value,
std::string owner,
unsigned int cashback,
unsigned int fee*/

namespace Node{

	const std::string hash_digest(const std::string serialize_tx) {

		std::string encoded;
		std::string digest;
		CryptoPP::SHA256 hash;
		CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));

		hash.Update((const CryptoPP::byte*)serialize_tx.data(), serialize_tx.size());
		digest.resize(hash.DigestSize());
		hash.Final((CryptoPP::byte*)&digest[0]);

		CryptoPP::StringSource(digest, true, new CryptoPP::Redirector(encoder));

		//std::cout << "Message: " << serialize_tx  << "\nDigest: " << encoded << std::endl;

		return encoded;
	}

	const void create_tx(std::string version) {

		std::cout << version << std::endl;

		std::string destination;
		unsigned int value;
		std::string owner("1330662819286567003101256740359821157367793328918");
		unsigned int cashback;
		unsigned int fee;

		std::cout << "Beneficiary: ";
		getline(std::cin, destination);

		std::cout << "Value: ";
		std::string value_s;
		getline(std::cin, value_s);
		value = std::stoi(value_s);

		std::cout << "Fee: ";
		std::string fee_s;
		getline(std::cin, fee_s);
		fee = std::stoi(fee_s);

		cashback = fee + value;

		Transaction tx("carlos", destination, value, owner, cashback, fee);

		// Serialization of the transaction structure in a JSON string
		const std::string serialize_tx = Transaction::tx_to_json(tx);
		//std::cout << tx << std::endl;


		tx.tx_id = Node::hash_digest(serialize_tx);

		std::cout << tx << std::endl;

	}

	

}
