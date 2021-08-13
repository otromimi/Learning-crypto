#pragma once

// My dependencies
#include "Structures.h"
#include "DB_operations.h"
#include "Wallet.h"

// Local dependencies
#include "eccrypto.h"
#include "sha.h"
#include "osrng.h"




namespace Node{

	const std::string hash_digest(const std::string message);
	
	const Transaction create_tx();

	const void create_block(Block& block);

	const void validate_inputs(std::string input);

	const void validate_tx(const Transaction tx);

	const void validate_block(const Block block);

	/// <summary>
	/// Elliptic Curve Digital Signature Algorithm verification.
	/// </summary>
	/// <param name="pt">Public member as a concatenated string of X and Y in hex.</param>
	/// <param name="signature">Signature over SHA256 hash.</param>
	/// <param name="message">Data to be verified.</param>
	/// <returns>True if verification went well, false otherwise.</returns>
	bool sign_verifier(std::string public_key, std::string signature, std::string message);

	

}
