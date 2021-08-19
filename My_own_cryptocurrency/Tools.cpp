
#include "Tools.h"

#include "cryptlib.h"
#include "eccrypto.h"
#include "ecp.h"
#include "hex.h"
#include "oids.h"
#include "osrng.h"

using namespace My_own_crypto;



/// <summary>
/// Encodes binary strings to hexadecimal.
/// </summary>
/// <param name="decoded">Binary string</param>
/// <returns>Hexadeciaml</returns>
std::string Tools::encode(const std::string decoded) {
	std::string encoded;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
	CryptoPP::StringSource ss(decoded, true, new CryptoPP::Redirector(encoder));

	//std::cout << "binary: " << decoded.length() << std::endl;
	//std::cout << "hex: " << encoded.length() << std::endl;

	return encoded;
}

/// <summary>
/// Decode from hexadecimal to binary.
/// </summary>
/// <param name="encoded">Hexadecimal string</param>
/// <returns>Binary string</returns>
std::string Tools::decode(const std::string encoded) {
	std::string decoded;
	CryptoPP::HexDecoder decoder(new CryptoPP::StringSink(decoded));
	CryptoPP::StringSource ss(encoded, true, new CryptoPP::Redirector(decoder));

	//std::cout << encoded << std::endl;
	//std::cout << encoded.length() << std::endl;

	return decoded;
}


const std::string Tools::hash_digest(const std::string message) {

	std::string digest;
	CryptoPP::SHA256 hash;

	hash.Update((const CryptoPP::byte*)message.data(), message.size());
	digest.resize(hash.DigestSize());
	hash.Final((CryptoPP::byte*)&digest[0]);

	return digest;
}


/// <summary>
/// Elliptic Curve Digital Signature Algorithm verification.
/// </summary>
/// <param name="pt">Public member as a concatenated string of X and Y in hex.</param>
/// <param name="signature">Signature over SHA256 hash.</param>
/// <param name="message">Data to be verified.</param>
/// <returns>True if verification went well, false otherwise.</returns>
bool Tools::sign_verifier(std::string compressed_key, std::string signature, std::string message) {



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


