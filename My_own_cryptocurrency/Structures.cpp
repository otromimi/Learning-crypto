

#include "Structures.h"



/// <summary>
/// Encodes binary strings to hexadecimal.
/// </summary>
/// <param name="decoded">Binary string</param>
/// <returns>Hexadeciaml</returns>
std::string encode(const std::string decoded) {
    std::string encoded;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
    CryptoPP::StringSource(decoded, true, new CryptoPP::Redirector(encoder));

    //std::cout << "binary: " << decoded.length() << std::endl;
    //std::cout << "hex: " << encoded.length() << std::endl;

    return encoded;
}

/// <summary>
/// Decode from hexadecimal to binary.
/// </summary>
/// <param name="encoded">Hexadecimal string</param>
/// <returns>Binary string</returns>
std::string decode(const std::string encoded) {
    std::string decoded;
    CryptoPP::HexDecoder decoder(new CryptoPP::StringSink(decoded));
    CryptoPP::StringSource(encoded, true, new CryptoPP::Redirector(decoder));

    //std::cout << encoded << std::endl;
    //std::cout << encoded.length() << std::endl;

    return decoded;
}




Entity::Entity(std::string account, unsigned int value) :account(account), value(value) {}

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Entity& data) {
    outstream << "\t" << data.account << "\t" << data.value << "\n";

    return outstream;
}



   



/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Transaction& data) {

    /*
    // Passing from epoch to UTC
    std::tm* utc_struct = std::gmtime((const time_t*)data.time);
    char* time_UTC_string = std::asctime(utc_struct);
    *(time_UTC_string + 24) = 0x00;
    */
    outstream << "-----Transaction: " << "-----\n" <<
        " UTC " << data.time << "\n" <<
        "Inputs: { ";
    for (std::string i : data.inputs) {
        outstream << i << ", ";
    }
    outstream << "\b\b }\n" <<
        "Outputs: \n";
    for (Entity i : data.outputs) {
        outstream << i;
    }
    outstream << "\n" <<
        "Origin: " << data.origin << "\n" <<
        "Value: " << data.value << "\n" <<
        "Fee: " << data.fee << "\n" <<
        "\n --Sign-- \n" << data.signature << "\n" <<
        "--------------------------------------\n";

    return outstream;
}


Transaction::Transaction() {}

Transaction::Transaction(std::string origin, unsigned int value, unsigned int fee) :
    origin(origin), value(value), fee(fee) {
    /*
    std::time_t now = std::time(0);
    long peter = now;
    char* dt = ctime(&now);


    std::tm* utc_struct = std::gmtime(&now);
    char* time_UTC_string = std::asctime(utc_struct);
    *(time_UTC_string+24) = 0x00;*/

    this->time = std::time(0);

}

std::string Transaction::tx_to_json() {

    struct_mapping::reg(&Transaction::time, "Time");
    struct_mapping::reg(&Transaction::inputs, "Inputs");
    struct_mapping::reg(&Transaction::outputs, "Outputs");
    struct_mapping::reg(&Transaction::origin, "Origin");
    struct_mapping::reg(&Transaction::value, "Value");
    struct_mapping::reg(&Transaction::fee, "Fee");


    std::ostringstream tx_json;
    struct_mapping::map_struct_to_json(*this, tx_json);


    return tx_json.str();
}

void Transaction::json_to_tx(std::string tx_json) {

    struct_mapping::reg(&Transaction::time, "Time");
    struct_mapping::reg(&Transaction::inputs, "Inputs");
    struct_mapping::reg(&Transaction::outputs, "Outputs");
    struct_mapping::reg(&Transaction::origin, "Origin");
    struct_mapping::reg(&Transaction::value, "Value");
    struct_mapping::reg(&Transaction::fee, "Fee");

    //  std::ostringstream tx_json;

    std::istringstream jason_data(tx_json);

    struct_mapping::map_json_to_struct(*this, jason_data);

    //std::cout << tx_json.str() << std::endl;

}

std::string Transaction::to_db_string() {
    std::ostringstream ordered_data;
    ordered_data << "\"" << this->signature << "\"" << ", " << 123 << ", " << this->time << ", \"" << this->origin << "\"," << this->fee;
    return ordered_data.str();
}





