#pragma once

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
#include "oids.h"
#include "osrng.h"





namespace Node{

	const std::string hash_digest(const std::string message) {

		std::string digest;
		CryptoPP::SHA256 hash;
		
		hash.Update((const CryptoPP::byte*)message.data(), message.size());
		digest.resize(hash.DigestSize());
		hash.Final((CryptoPP::byte*)&digest[0]);

		return digest;
	}

	const Transaction create_tx() {

		//std::cout << version << std::endl;

		std::string destination;
		unsigned int value;
		std::string origin("1330662819286567003101256740359821157367793328918");
		unsigned int fee;

		std::cout << "Beneficiary: ";
		getline(std::cin, origin);

		std::cout << "Value: ";
		std::string value_s;
		getline(std::cin, value_s);
		value = std::stoi(value_s);

		std::cout << "Fee: ";
		std::string fee_s;
		getline(std::cin, fee_s);
		fee = std::stoi(fee_s);


		Transaction tx( origin, value, fee);

		// Serialization of the transaction structure in a JSON string
		//const std::string serialize_tx = Transaction::tx_to_json(tx);
		//std::cout << tx << std::endl;


		//tx.tx_id = Node::hash_digest(serialize_tx);

		//std::cout << tx << std::endl;
		return tx;

	}

	const void create_block(Block& block) {}

	const void validate_inputs(std::string input) {}

	const void validate_tx(const Transaction tx) {}

	const void validate_block(const Block block) {}

	/// <summary>
	/// Elliptic Curve Digital Signature Algorithm verification.
	/// </summary>
	/// <param name="pt">Public member as a concatenated string of X and Y in hex.</param>
	/// <param name="signature">Signature over SHA256 hash.</param>
	/// <param name="message">Data to be verified.</param>
	/// <returns>True if verification went well, false otherwise.</returns>
	bool sign_verifier(std::string public_key, std::string signature, std::string message) {

		std::string decoded_publickey = decode(public_key);
		std::string decoded_signature = decode(signature);

		CryptoPP::AutoSeededRandomPool prng;


		CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
		publicKey.AccessGroupParameters().SetPointCompression(true);

		publicKey.Load(CryptoPP::StringSource(decoded_publickey, true).Ref());

		
		//bool key_okay = publicKey.Validate(prng, 3);
		
		

		CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(publicKey);
		

		bool result = verifier.VerifyMessage((const CryptoPP::byte*)&message[0], message.size(), (const CryptoPP::byte*)&decoded_signature[0], decoded_signature.size());

		// Verification failure?
		if (!result) {
			std::cerr << "Failed to verify signature on message" << std::endl;
		}
		else {
			std::cerr << "All good!" << std::endl;
		}



		return result;
	}

	
	/// <summary>
	/// Encodes binary strings to hexadecimal.
	/// </summary>
	/// <param name="decoded">Binary string</param>
	/// <returns>Hexadeciaml</returns>
	std::string encode(const std::string decoded) {
		std::string encoded;
		CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
		CryptoPP::StringSource(decoded, true, new CryptoPP::Redirector(encoder));

		//std::cout << encoded << std::endl;
		//std::cout << "binary: " << decoded.length() << std::endl;
		//std::cout << "hex: " << encoded.length() << std::endl;

		return encoded;
	}

	/// <summary>
	/// Decode from hexadecimal to binary.
	/// </summary>
	/// <param name="encoded">Hexadecimal string</param>
	/// <returns>Binary string</returns>
	std::string decode(const std::string encoded) {
		std::string decoded;
		CryptoPP::HexDecoder decoder(new CryptoPP::StringSink(decoded));
		CryptoPP::StringSource(encoded, true, new CryptoPP::Redirector(decoder));

		//std::cout << encoded << std::endl;
		//std::cout << encoded.length() << std::endl;

		return decoded;
	}

}
