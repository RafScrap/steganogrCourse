#include <fstream>
#include "bmp.h"

extern "C" __declspec(dllexport) std::string bmpDecode(std::string fileName) {
	std::string res = "";
	std::ifstream fileStream(fileName, std::ifstream::binary);
	if (!fileStream) {
		return "Error opening file " + fileName + ".";
	}
	// 
	BITMAPFILEHEADER fileHeader;
	read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
	read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));
	if (fileHeader.bfType != 0x4D42) {
		return "Error: " + fileName + " is not BMP file.";
	}
	// 
	BITMAPINFOHEADER fileInfoHeader;
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));
	// bmp core
	if (fileInfoHeader.biSize >= 12) {
		read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}
	// 
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3) {
		colorsCount = 3;
	}
	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;
	// bmp v1
	if (fileInfoHeader.biSize >= 40) {
		read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}
	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;
	if (fileInfoHeader.biSize >= 52) {
		read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}
	// 
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}
	// bmp v3
	if (fileInfoHeader.biSize >= 56) {
		read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else {
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}
	// bmp v4
	if (fileInfoHeader.biSize >= 108) {
		read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}
	// bmp v5
	if (fileInfoHeader.biSize >= 124) {
		read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
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
	unsigned int linePadding = (4 - (fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) & 3) & 3;
	unsigned int bufer;
	unsigned int k = 0, n = 0;
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			read(fileStream, bufer, fileInfoHeader.biBitCount / 8);
			rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
			rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
			rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
			rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
			if (k < 8) {
				n = n << 1;
				if ((bufer & 1) == 0) n += 1;
			}
			else if (k - 8 < n) {
				if ((bufer & 1) == 0) res += "1";
				else  res += "0";
			}
			k++;
		}
		fileStream.seekg(linePadding, std::ios_base::cur);
	}
	fileStream.close();
	return res;
}