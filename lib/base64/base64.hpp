#ifndef __BASE64_H
#define __BASE64_H

#include <string>
#include <vector>

typedef unsigned char byte;

std::vector<byte> base64_decode(const std::string& encoded_string);
std::string base64_encode(const byte* buff, unsigned int len);

#endif
