

// My dependencies
#include "Wallet.h"


using namespace CryptoPP;
using namespace My_own_crypto;

AutoSeededRandomPool prng;

Wallet::Wallet(const char* name): name(name) {
	try {
		this->load_keys();
		//this->new_keys();
	}
	catch (Exception e) {
		this->new_keys();
	}
}

Wallet::~Wallet() {
	this->save_keys();
}

void Wallet::new_keys(){

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
	privateKey.Initialize(prng, CryptoPP::ASN1::secp256k1());

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
	CryptoPP::StringSource ss(signature, true, new CryptoPP::Redirector(encoder));

	//Integer sign_num(encoded.c_str());

	std::ostringstream num_str;
	//num_str << std::hex << sign_num;


	return  encoded;// num_str.str();

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
	CryptoPP::StringSource ss(signature, true, new CryptoPP::Redirector(decoder));


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

std::string Wallet::get_compressedPublic() {

	
	publicKey.AccessGroupParameters().SetPointCompression(true);

	// Save the public keys
	std::string public_k;
	publicKey.Save(StringSink(public_k).Ref());
	
	std::string code = publicKey.GetPublicElement().y % 2 ? "03" : "02";

	std::stringstream x_public;
	x_public << std::hex << publicKey.GetPublicElement().x;

	Integer carlos = publicKey.GetPublicElement().x;

	std::cout << carlos.ByteCount() << std::endl;
	
	return code + x_public.str();
}


void Wallet::save_keys() {
	// Save private key in PKCS #8 format
	std::string file = this->name + ".ec.der";
	FileSink fs(file.c_str(), true /*binary*/);
	privateKey.Save(fs);
}

void Wallet::load_keys() {

	// Load private key in PKCS #8 format
	std::string file = this->name + ".ec.der";
	FileSource fs(file.c_str(), true /*pump all*/);
	privateKey.Load(fs);

	
	if (privateKey.Validate(prng, 3)) {
		privateKey.MakePublicKey(publicKey);
	}
	else {
		std::cout << "Problem loading keys." << std::endl;
	}
}