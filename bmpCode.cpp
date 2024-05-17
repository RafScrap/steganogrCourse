#include <fstream>
#include "bmp.h"

extern "C" __declspec(dllexport) std::string bmpCode(std::string fileName, std::string fileNameOut, std::string code) {
	const unsigned short length = code.length();
	for (int i = 0; i < 8; i++) {
		unsigned short byte = length << (15 - i);
		byte = byte >> 15;
		if (byte == 0)
			code = "0" + code;
		else code = "1" + code;
	};
	// ��������� ����
	std::ifstream fileStream(fileName, std::ifstream::binary);
	std::ofstream fileStreamOut(fileNameOut, std::ofstream::binary);
	if (!fileStream) {
		return "Error opening file " + fileName + ".";
	}
	// ��������� �����������
	BITMAPFILEHEADER fileHeader;
	readANDwrite(fileStream, fileStreamOut, fileHeader.bfType, sizeof(fileHeader.bfType));
	readANDwrite(fileStream, fileStreamOut, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	readANDwrite(fileStream, fileStreamOut, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	readANDwrite(fileStream, fileStreamOut, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	readANDwrite(fileStream, fileStreamOut, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));
	if (fileHeader.bfType != 0x4D42) {
		return "Error: " + fileName + " is not BMP file.";
	}
	// ���������� �����������
	BITMAPINFOHEADER fileInfoHeader;
	readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));
	// bmp core
	if (fileInfoHeader.biSize >= 12) {
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}
	// �������� ���������� � ��������
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3) {
		colorsCount = 3;
	}
	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;
	// bmp v1
	if (fileInfoHeader.biSize >= 40) {
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}
	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;
	if (fileInfoHeader.biSize >= 52) {
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}
	// ���� ����� �� ������, �� ������ ����� �� ���������
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}
	// bmp v3
	if (fileInfoHeader.biSize >= 56) {
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else {
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}
	// bmp v4
	if (fileInfoHeader.biSize >= 108) {
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}
	// bmp v5
	if (fileInfoHeader.biSize >= 124) {
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		readANDwrite(fileStream, fileStreamOut, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
	}
	// �������� �� �������� ���� ������ �������
	if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
		fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
		return "Error: Unsupported BMP format.";
	}
	if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
		return "Error: Unsupported BMP bit count.";
	}
	if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
		return "Error: Unsupported BMP compression.";
	}
	// rgb info
	RGBQUAD** rgbInfo = new RGBQUAD * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}
	// ����������� ������� ������� � ����� ������ ������
	unsigned int linePadding = (4 - (fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) & 3) & 3;
	// ������
	unsigned int bufer;
	unsigned int index = 0;

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			read(fileStream, bufer, fileInfoHeader.biBitCount / 8);
			rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
			rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
			rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
			rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
			if (index < code.length()) {
				if (code[index] == '0' && (bufer & 1) == 0) {
					bufer += 1;
				}
				else if (code[index] == '1' && (bufer & 1) == 1) {
					bufer -= 1;
				}
				index++;
			}
			write(fileStreamOut, bufer, fileInfoHeader.biBitCount / 8);
		}
		fileStream.seekg(linePadding, std::ios_base::cur);
		bufer = 0;
		write(fileStreamOut, bufer, linePadding);
	}
	fileStream.close();
	fileStreamOut.close();
	return "Success!";
}