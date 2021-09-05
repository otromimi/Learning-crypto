#pragma once

#include <iostream>

namespace My_own_crypto {

	namespace Tools {


		/// <summary>
		/// Encodes binary strings to hexadecimal.
		/// </summary>
		/// <param name="decoded">Binary string.</param>
		/// <returns>Hexadeciaml.</returns>
		std::string encode(const std::string decoded);

		/// <summary>
		/// Decode from hexadecimal to binary.
		/// </summary>
		/// <param name="encoded">Hexadecimal string.</param>
		/// <returns>Binary string.</returns>
		std::string decode(const std::string encoded);

		/// <summary>
		/// Computes the hash SHA256.
		/// </summary>
		/// <param name="message">String to be hashed.</param>
		/// <returns>Hash digest.</returns>
		const std::string hash_sha256(const std::string message);

		/// <summary>
		/// Elliptic Curve Digital Signature Algorithm verification.
		/// </summary>
		/// <param name="pt">Public member as a concatenated string of X and Y in hex.</param>
		/// <param name="signature">Signature over SHA256 hash.</param>
		/// <param name="message">Data to be verified.</param>
		/// <returns>True if verification went well, false otherwise.</returns>
		bool sign_verifier(std::string public_key, std::string signature, std::string message);

		/// <summary>
		/// Displays the user with the opstion (Y/N).
		/// </summary>
		/// <returns>True if the user inputed (y,Y) or False in case (n,N).</returns>
		bool cont_loop();

		/// <summary>
		/// Time from host in UTC, not really precisse but work in windows and linux.
		/// </summary>
		/// <returns>String with UTC time in the format: dd/mm/yyyy HH:MM:SS .</returns>
		const std::string time_now();

		const bool check_date(std::string date);


		/// <summary>
		/// Cheack if which of the dates is older.
		/// </summary>
		/// <param name="older">String with UTC datetime format: AAAA/MM/DD hh:mm:ss</param>
		/// <param name="newer">String with UTC datetime format: AAAA/MM/DD hh:mm:ss</param>
		/// <returns>1 if older is older, -1 if newer is older and 0 if the same.</returns>
		const int is_older(std::string older, std::string newer);

		int avg_tpu(std::string older, std::string newer, int count = 1);

		/// <summary>
		/// Parse the string time created in <time_now> into a int struc.
		/// </summary>
		/// <param name="utc_time">UTC time in format:  dd/mm/yyyy HH:MM:SS .</param>
		/// <returns>Structure <mt> containing numeric field for date values.</returns>
		const tm get_int_time(std::string utc_time);

		/// <summary>
		/// Outputst the constests to a .txt file.
		/// </summary>
		/// <param name="file">name or path to file (the .txt is added at the end).</param>
		/// <param name="data">string to put on the file.</param>
		void to_txt(std::string file, std::string data);
	}
}