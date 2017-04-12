#include <iostream>
#include <iterator>
#include "base64.hpp"

int main(int argc, char *argv[])
{
    std::string text = "Lorem ipsum";
    std::cout << text << std::endl;

    std::string encoded;
    base64::encode(text.begin(), text.end(), std::back_inserter(encoded));
    std::cout << encoded << std::endl;

    std::string decoded;
    base64::decode(encoded.begin(), encoded.end(), std::back_inserter(decoded));
    std::cout << decoded << std::endl;

    std::string part1 = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQ";
    std::string part2 = "sIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdA==";
    base64::context context;
    base64::decode_more(context, part1.begin(), part1.end(), std::ostream_iterator<char>(std::cout));
    base64::decode_more(context, part2.begin(), part2.end(), std::ostream_iterator<char>(std::cout));

    std::cout << std::endl;

    return 0;
}
