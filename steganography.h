#include <iostream>
/**
 * \brief Функция шифрования бинарной строки в BMP-файл методом младших бит
 */
extern "C" __declspec(dllexport) std::string bmpCode(std::string, std::string, std::string);
/**
 * \brief Функция дешифрования бинарной строки из BMP-файла, зашифрованной методом младших бит
 */
extern "C" __declspec(dllexport) std::string bmpDecode(std::string);
/**
 * \brief Функция шифрования бинарной строки в PNG-файл методом наполнения за границами картинки
 */
extern "C" __declspec(dllexport) unsigned int pngCode(std::string, std::string, std::string);
/** 
* \brief Функция дешифрования бинарной строки из PNG-файла, зашифрованной наполнения за границами картинки
 */
extern "C" __declspec(dllexport) void pngDecode(std::string, std::string, unsigned int);