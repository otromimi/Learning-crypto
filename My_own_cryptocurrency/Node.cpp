// Standard dependencies
#include <iostream>


// My own dependencies
#include "Node.h"



const std::string Node::hash_digest(const std::string message) {

	std::string digest;
	CryptoPP::SHA256 hash;

	hash.Update((const CryptoPP::byte*)message.data(), message.size());
	digest.resize(hash.DigestSize());
	hash.Final((CryptoPP::byte*)&digest[0]);

	return digest;
}

const Transaction Node::create_tx() {

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


	Transaction tx(origin, value, fee);

	// Serialization of the transaction structure in a JSON string
	//const std::string serialize_tx = Transaction::tx_to_json(tx);
	//std::cout << tx << std::endl;


	//tx.tx_id = Node::hash_digest(serialize_tx);

	//std::cout << tx << std::endl;
	return tx;

}

const void Node::create_block(Block& block) {}

const void Node::validate_inputs(std::string input) {}

const void Node::validate_tx(const Transaction tx) {}

const void Node::validate_block(const Block block) {}

/// <summary>
/// Elliptic Curve Digital Signature Algorithm verification.
/// </summary>
/// <param name="pt">Public member as a concatenated string of X and Y in hex.</param>
/// <param name="signature">Signature over SHA256 hash.</param>
/// <param name="message">Data to be verified.</param>
/// <returns>True if verification went well, false otherwise.</returns>
bool Node::sign_verifier(std::string compressed_key, std::string signature, std::string message) {
	

	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
	publicKey.AccessGroupParameters().Initialize(CryptoPP::ASN1::secp256k1());



	CryptoPP::StringSource ss(compressed_key, true, new CryptoPP::HexDecoder);
	CryptoPP::ECP::Point point;

	publicKey.GetGroupParameters().GetCurve().DecodePoint(point, ss, ss.MaxRetrievable());
	publicKey.SetPublicElement(point);


	//CryptoPP::AutoSeededRandomPool prng;
	//bool key_okay = publicKey.Validate(prng, 3);

	std::string decoded_signature = decode(signature);

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



