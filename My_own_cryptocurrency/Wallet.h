
#pragma once


class Wallet {
private:
	// Generates new wallet address
	void new_address();
	void save_keys();
	void load_keys();

public:

	Wallet();

	~Wallet();
	
	// Signs transaction
	std::string sign_tx(std::string message);

	// Veryfy transaction signature
	bool verify_tx_sig(std::string signature, std::string message);

};

