// Standard dependencies
#include <iostream>
#include <iomanip>
#include <hex.h>
#include <ostream>

// My dependencies
#include "Wallet.h"

// Local dependencies
#include "cryptlib.h"
#include "eccrypto.h"
#include "files.h"
#include "ecp.h"
#include "hex.h"
#include "oids.h"
#include "osrng.h"


using namespace CryptoPP;

ECDSA<ECP, SHA256>::PrivateKey privateKey;
ECDSA<ECP, SHA256>::PublicKey publicKey;
	
AutoSeededRandomPool prng;

Wallet::Wallet() {
	this->new_address();
}

Wallet::~Wallet() {
	this->save_keys();
}

void Wallet::new_address(){

	// Generating private key and storing it privately
	privateKey.Initialize(prng, ASN1::secp256k1());
	bool result_public = privateKey.Validate(prng, 3);
	if (!result_public) {
		std::cout << "Error while generating new private key." << std::endl;
	}

	// Generating public key and storing it privatly
	privateKey.MakePublicKey(publicKey);

	bool result_private = publicKey.Validate(prng, 3);
	if (!result_private) { 
		std::cout << "Error while generating new public key." << std::endl;
	}

	std::cout << privateKey.GetPrivateExponent() << std::endl;

};

// Signs transaction
std::string Wallet::sign_tx(std::string message) {

	
	ECDSA<ECP, SHA256>::Signer signer(privateKey);

	bool result = signer.AccessKey().Validate(prng, 3);
	if (!result) { 
		std::cout << "Error while initializing the signer." << std::endl;
	}

	size_t siglen = signer.MaxSignatureLength();
	std::string signature(siglen, 0x00);

	siglen = signer.SignMessage(prng, (const byte*)&message[0], message.size(), (byte*)&signature[0]);
	signature.resize(siglen);

	return signature;

	/*
	std::string encoded;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
	CryptoPP::StringSource(signature, true, new CryptoPP::Redirector(encoder));

	std::cout << encoded << std::endl;
	std::cout << encoded.length() << std::endl;
	*/
};


// Veryfy transaction signature
bool Wallet::verify_tx_sig(std::string signature, std::string message) {

	ECDSA<ECP, SHA256>::Verifier verifier(publicKey);

	bool result = verifier.VerifyMessage((const byte*)&message[0], message.size(), (const byte*)&signature[0], signature.size());

	// Verification failure?
	if (!result) {
		std::cout << "Failed to verify signature on message" << std::endl;
	}
	else {
		std::cout << "All good!" << std::endl;
	}

	return result;
}


void Wallet::save_keys() {
	// Save private key in PKCS #8 format
	FileSink fs("private.ec.der", true /*binary*/);
	privateKey.Save(fs);
}

void Wallet::load_keys() {

	// Load private key in PKCS #8 format
	FileSource fs("private.ec.der", true /*pump all*/);
	privateKey.Load(fs);

	
	if (privateKey.Validate(prng, 3)) {
		std::cout << "Keys loaded succesfully." << std::endl;
	}
}