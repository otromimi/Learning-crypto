#include <iostream>

namespace My_own_crypto {

	namespace Tools {


		/// <summary>
		/// Encodes binary strings to hexadecimal.
		/// </summary>
		/// <param name="decoded">Binary string</param>
		/// <returns>Hexadeciaml</returns>
		std::string encode(const std::string decoded);

		/// <summary>
		/// Decode from hexadecimal to binary.
		/// </summary>
		/// <param name="encoded">Hexadecimal string</param>
		/// <returns>Binary string</returns>
		std::string decode(const std::string encoded);


		const std::string hash_sha256(const std::string message);

		/// <summary>
		/// Elliptic Curve Digital Signature Algorithm verification.
		/// </summary>
		/// <param name="pt">Public member as a concatenated string of X and Y in hex.</param>
		/// <param name="signature">Signature over SHA256 hash.</param>
		/// <param name="message">Data to be verified.</param>
		/// <returns>True if verification went well, false otherwise.</returns>
		bool sign_verifier(std::string public_key, std::string signature, std::string message);


		const std::string time_now();

		const long long time_to_epoch(std::string utc_time);

	}
}