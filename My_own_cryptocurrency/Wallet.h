
#pragma once


class Wallet {
private:
	
	/// Generates a new key pair.
	void new_keys();

	/// Save keys to a.ec.der file
	void save_keys();

	/// Loads file from .ec.der
	void load_keys();

public:

	Wallet();

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

