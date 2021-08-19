
#pragma once

// Standard dependencies
#include <iostream>
#include <iomanip>
#include <ostream>


// My dependencies



// Local dependencies
#include "cryptlib.h"
#include "eccrypto.h"
#include "files.h"
#include "ecp.h"
#include "hex.h"
#include "oids.h"
#include "osrng.h"

namespace My_own_crypto {

	class Wallet {
	private:

		const std::string name;

		CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
		CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;


		/// Generates a new key pair.
		void new_keys();

		/// Save keys to a.ec.der file
		void save_keys();

		/// Loads file from .ec.der
		void load_keys();

	public:

		Wallet(const char* name);

		~Wallet();

		/// <summary>
		/// Sign a transaction
		/// </summary>
		/// <param name="message">json string of data</param>
		/// <returns>binary sign</returns>
		std::string sign_tx(std::string message);

		/// <summary>
		/// Veryfies the signature of a message with the wallet private key.
		/// </summary>
		/// <param name="signature">string in binary</param>
		/// <param name="message">data to be verified in jason</param>
		/// <returns>true if verification was okay, false otherwise</returns>
		bool verify_tx_sig(std::string signature, std::string message);


		std::string get_compressedPublic();

	};

}