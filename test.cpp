#include <iostream>
#include "steganography.h"

int main() {
	std::string path = "..\\images\\";
	std::cout << bmpCode(path + "000.bmp", path + "001.bmp", "01110111") << "\n";
	std::cout << bmpDecode(path + "001.bmp") << "\n";
	unsigned int height = pngCode(path + "pngOld.png",
		path + "pngNew.png",
		path + "pngOut.png");
	pngDecode(path + "pngOut.png",
		path + "png.png", height);

	system("pause");
	return 0;
}