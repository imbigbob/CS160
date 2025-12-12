#include <fstream>

#include "WriteString.h"

// String writing
void writeString(std::ofstream &out, const std::string &s) {
    int len = s.size();
    out.write(reinterpret_cast<char*>(&len), sizeof(len));
    out.write(s.data(), len);
}

// String reading
bool readString(std::ifstream &in, std::string &s) {
    int len;
    if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
    s.resize(len);
    if (!in.read(&s[0], len)) return false;
    return true;
}
