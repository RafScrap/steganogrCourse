#include <iostream>

extern "C" __declspec(dllexport) std::string bmpCode(std::string, std::string, std::string);
extern "C" __declspec(dllexport) std::string bmpDecode(std::string);
extern "C" __declspec(dllexport) unsigned int pngCode(std::string, std::string, std::string);
extern "C" __declspec(dllexport) void pngDecode(std::string, std::string, unsigned int);