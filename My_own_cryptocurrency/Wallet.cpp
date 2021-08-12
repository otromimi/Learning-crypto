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
	// Passing random seed and eliptic curve to be use
	privateKey.Initialize(prng, ASN1::secp256k1());
	bool result_private = privateKey.Validate(prng, 3);
	if (!result_private) {
		std::cerr << "Error while generating new private key." << std::endl;
	}

	// Generating public key and storing it privatly
	privateKey.MakePublicKey(publicKey);

	bool result_public = publicKey.Validate(prng, 3);
	if (!result_public) { 
		std::cerr << "Error while generating new public key." << std::endl;
	}



	AutoSeededRandomPool prng;
	ECDSA<ECP, SHA1>::PrivateKey privateKey;
	ECDSA<ECP, SHA1>::PublicKey publicKey;
	privateKey.Initialize(prng, CryptoPP::ASN1::secp256r1());

	const Integer& x1 = privateKey.GetPrivateExponent();
	
	privateKey.MakePublicKey(publicKey);
	const ECP::Point& q = publicKey.GetPublicElement();
	const Integer& qx = q.x;
	const Integer& qy = q.y;
	

	publicKey.AccessGroupParameters().SetPointCompression(false);
	ByteQueue q1;
	publicKey.Save(q1);
	

	publicKey.AccessGroupParameters().SetPointCompression(true);
	ByteQueue q2;
	publicKey.Save(q2);
	

};

// Signs transaction
std::string Wallet::sign_tx(std::string message) {

	
	ECDSA<ECP, SHA256>::Signer signer(privateKey);

	bool result = signer.AccessKey().Validate(prng, 3);
	if (!result) { 
		std::cerr << "Error while initializing the signer." << std::endl;
	}

	size_t siglen = signer.MaxSignatureLength();
	std::string signature(siglen, 0x00);

	siglen = signer.SignMessage(prng, (const byte*)&message[0], message.size(), (byte*)&signature[0]);
	signature.resize(siglen);

	std::string encoded;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
	CryptoPP::StringSource(signature, true, new CryptoPP::Redirector(encoder));

	return encoded;

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

	std::string decoded_signature;
	CryptoPP::HexDecoder decoder(new CryptoPP::StringSink(decoded_signature));
	CryptoPP::StringSource(signature, true, new CryptoPP::Redirector(decoder));


	ECDSA<ECP, SHA256>::Verifier verifier(publicKey);

	bool result = verifier.VerifyMessage((const byte*)&message[0], message.size(), (const byte*)&decoded_signature[0], decoded_signature.size());

	// Verification failure?
	if (!result) {
		std::cerr << "Failed to verify signature on message" << std::endl;
	}
	else {
		std::cerr << "All good!" << std::endl;
	}



	return result;
}

std::string Wallet::get_publicElement() {

	
	publicKey.AccessGroupParameters().SetPointCompression(true);

	// Save the public keys
	std::string public_k;
	publicKey.Save(StringSink(public_k).Ref());
	
	//////////////////////////////////////////////////////////////////////
	// Print some stuff about them
	std::string encoded_k;
	StringSource ss3(public_k, true, new HexEncoder(new StringSink(encoded_k)));

	
	return encoded_k;
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
		std::cerr << "Keys loaded succesfully." << std::endl;
	}
}