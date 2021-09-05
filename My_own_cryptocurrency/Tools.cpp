
#define _CRT_SECURE_NO_WARNINGS

// Standard
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
#include <fstream>

// My own
#include "Tools.h"

// Dependencies
#include "cryptlib.h"
#include "eccrypto.h"
#include "ecp.h"
#include "hex.h"
#include "oids.h"
#include "osrng.h"

using namespace My_own_crypto;



std::string Tools::encode(const std::string decoded) {
	std::string encoded;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
	CryptoPP::StringSource ss(decoded, true, new CryptoPP::Redirector(encoder));
	return encoded;
}


std::string Tools::decode(const std::string encoded) {
	std::string decoded;
	CryptoPP::HexDecoder decoder(new CryptoPP::StringSink(decoded));
	CryptoPP::StringSource ss(encoded, true, new CryptoPP::Redirector(decoder));
	return decoded;
}


const std::string Tools::hash_sha256(const std::string message) {
	std::string digest;
	CryptoPP::SHA256 hash;
	hash.Update((const CryptoPP::byte*)message.data(), message.size());
	digest.resize(hash.DigestSize());
	hash.Final((CryptoPP::byte*)&digest[0]);
	return encode(digest);
}



bool Tools::sign_verifier(std::string compressed_key, std::string signature, std::string message) {

	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
	publicKey.AccessGroupParameters().Initialize(CryptoPP::ASN1::secp256k1());

	CryptoPP::StringSource ss(compressed_key, true, new CryptoPP::HexDecoder);
	CryptoPP::ECP::Point point;

	publicKey.GetGroupParameters().GetCurve().DecodePoint(point, ss, ss.MaxRetrievable());
	publicKey.SetPublicElement(point);

	std::string decoded_signature = decode(signature);

	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(publicKey);
	bool result = verifier.VerifyMessage((const CryptoPP::byte*)&message[0], message.size(), (const CryptoPP::byte*)&decoded_signature[0], decoded_signature.size());

	// Verification failure?
	if (!result) {
		std::cerr << "Failed to verify signature on message" << std::endl;
	}
	else {
		//std::cerr << "All good!" << std::endl;
	}
	return result;
}


const std::string Tools::time_now() {
	std::time_t ttime = std::time(NULL);
	tm* gmt_time = gmtime(&ttime);

	//char* dt = ctime(&ttime);

	std::string  utc_time;

	utc_time = std::to_string(1900 + gmt_time->tm_year) + "-";

	if (gmt_time->tm_mon < 10)
		utc_time += "0";
	utc_time += std::to_string(gmt_time->tm_mon) + "-";
	if (gmt_time->tm_mday < 10)
		utc_time += "0";
	utc_time += std::to_string(gmt_time->tm_mday) + " ";
	if (gmt_time->tm_hour < 10)
		utc_time += "0";
	utc_time += std::to_string(gmt_time->tm_hour) + ":";
	if (gmt_time->tm_min < 10)
		utc_time += "0";
	utc_time += std::to_string(gmt_time->tm_min) + ":";
	if (gmt_time->tm_sec < 10)
		utc_time += "0";
	utc_time += std::to_string(gmt_time->tm_sec);
		
	//std::cout << utc_time << std::endl;

	//long long back_time = time_to_epoch(utc_time);

	//std::cout << ttime << std::endl;
	//std::cout << back_time << std::endl;
	
	//char* dt = asctime(gmt_time);
	//std::cout << "The current UTC date and time is:" << dt << std::endl;
	//tm* gmt_time = gmtime(dt);

	return utc_time;
}

const bool Tools::check_date(std::string date) {
	try {
		tm num_date = Tools::get_int_time(date);
		return true;
	}
	catch(std::string s){
		std::cout << s << std::endl;
		return false;
	}
}

const int Tools::is_older(std::string older, std::string newer) {
	tm old_time = Tools::get_int_time(older);
	tm new_time = Tools::get_int_time(newer);

	if (old_time.tm_year < new_time.tm_year)
		return 1;
	if(old_time.tm_year > new_time.tm_year)
		return -1;

	if (old_time.tm_mon < new_time.tm_mon)
		return 1;
	if (old_time.tm_mon > new_time.tm_mon)
		return -1;

	if (old_time.tm_mday < new_time.tm_mday)
		return 1;
	if (old_time.tm_mday > new_time.tm_mday)
		return -1;

	if (old_time.tm_hour < new_time.tm_hour)
		return 1;
	if (old_time.tm_hour > new_time.tm_hour)
		return -1;

	if (old_time.tm_min < new_time.tm_min)
		return 1;
	if (old_time.tm_min > new_time.tm_min)
		return -1;

	if (old_time.tm_sec < new_time.tm_sec)
		return 1;
	if (old_time.tm_sec > new_time.tm_sec)
		return -1;

	return 0;
}

int Tools::avg_tpu(std::string older, std::string newer, int count) {
	int delta = is_older(older, newer);
	if (delta != 1)
		return delta;

	tm first = get_int_time(older);
	tm second = get_int_time(newer);
	
	delta = (first.tm_hour - second.tm_hour) * 3600;
	delta += (first.tm_min - second.tm_min) * 24;
	delta += (first.tm_sec - second.tm_sec);

	return floor(delta/count);
}

const tm Tools::get_int_time(std::string utc_time) {
	
	std::time_t rawtime;
	tm timeinfo;
	std::string aux;
	int element = 0;

	time(&rawtime);
	timeinfo = *localtime(&rawtime);

	for (char i : utc_time) {
		if (i != '-' && i != ':' && i != ' ')
			aux += i;
		else {
			switch (element) {
			case 0:
				timeinfo.tm_year = std::stoi(aux);
				break;
			case 1:
				timeinfo.tm_mon = std::stoi(aux);
				if (timeinfo.tm_mon > 12)
					throw std::string("Invalid month.");
				break;
			case 2:
				timeinfo.tm_mday = std::stoi(aux);
				switch (timeinfo.tm_mon)
				{
				case 1:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				case 2:
					if ((timeinfo.tm_mday > 29 && !((timeinfo.tm_year % 4 == 0) && (timeinfo.tm_year % 100 != 0))) || (timeinfo.tm_mday > 28 && ((timeinfo.tm_year % 4 == 0) && (timeinfo.tm_year % 100 != 0))))
						throw std::string("Invalid day.");
					break;
				case 3:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				case 4:
					if (timeinfo.tm_mday > 30)
						throw std::string("Invalid day.");
					break;
				case 5:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				case 6:
					if (timeinfo.tm_mday > 30)
						throw std::string("Invalid day.");
					break;
				case 7:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				case 8:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				case 9:
					if (timeinfo.tm_mday > 30)
						throw std::string("Invalid day.");
					break;
				case 10:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				case 11:
					if (timeinfo.tm_mday > 30)
						throw std::string("Invalid day.");
					break;
				case 12:
					if (timeinfo.tm_mday > 31)
						throw std::string("Invalid day.");
					break;
				default:
					break;
				}
				break;
			case 3:
				timeinfo.tm_hour = std::stoi(aux);
				if(timeinfo.tm_hour > 23)
					throw std::string("Invalid hour.");
				break;
			case 4:
				timeinfo.tm_min = std::stoi(aux);
				if(timeinfo.tm_min > 59)
					throw std::string("Invalid minute.");
				break;
			case 5:
				timeinfo.tm_sec = std::stoi(aux);
				if(timeinfo.tm_sec > 59)
					throw std::string("Invalid second.");
				break;
			default:
				break;
			}
			element++;
			aux = "";
		}
	}

	//rawtime = mktime(timeinfo);
	return timeinfo;
}

bool Tools::cont_loop() {
	std::string answer;
	std::string aux;
	
	while (true) {
		std::cin.clear();
		std::getline(std::cin, aux);

		std::cout << "(Y\\n): ";
		answer = std::cin.get(); //>> answer;
		
		if (answer == "Y" || answer == "y" || answer == "\n")
			return true;
		if (answer == "N" || answer == "n")
			return false;
	}
}


void Tools::to_txt(std::string file, std::string data) {
	std::ofstream fw(file+".txt", std::ofstream::out);
	if (fw.is_open())
	{
		fw << data;
		fw.close();
	}
	else 
		std::cerr << "Problem with opening file" << file << ".txt" << std::endl;
}