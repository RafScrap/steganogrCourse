#include <fstream>

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


namespace cv {
	// CIEXYZTRIPLE stuff
	typedef int FXPT2DOT30;

	typedef struct {
		FXPT2DOT30 ciexyzX;
		FXPT2DOT30 ciexyzY;
		FXPT2DOT30 ciexyzZ;
	} CIEXYZ;

	typedef struct {
		CIEXYZ  ciexyzRed;
		CIEXYZ  ciexyzGreen;
		CIEXYZ  ciexyzBlue;
	} CIEXYZTRIPLE;

	// bitmap file header
	typedef struct {
		unsigned short bfType; //������ �����
		unsigned int   bfSize; //������ ����� � ������
		unsigned short bfReserved1; //������ ��������� ����
		unsigned short bfReserved2; //������ ��������� ����
		unsigned int   bfOffBits; //��������� ���������� ������ ������������ ������ ������ ��������� (� ������)
	} BITMAPFILEHEADER;

	// bitmap info header
	typedef struct {
		unsigned int   biSize; //������ BITMAPINFOHEADER � ������
		unsigned int   biWidth; //������ � ��������
		unsigned int   biHeight; //������ ������ � �������� (*)
		unsigned short biPlanes; //��������� ������ �������� 1
		unsigned short biBitCount; //���������� ��� �� ������� (16, 24, 32)
		unsigned int   biCompression; //������ �������� ��������
		//(0 - ��������� ������, 3 - ��������� ������ � ������� �������� ������� ��� biBitCount = 16, 32)
		unsigned int   biSizeImage; //������ � ������ ��� ������ � ���� ������ (� ��� ��������� ������, ��. biCompression)
		unsigned int   biXPelsPerMeter; //���������� �������� �� ���� �� �����������
		unsigned int   biYPelsPerMeter; //���������� �������� �� ���� �� ���������
		unsigned int   biClrUsed;
		unsigned int   biClrImportant;

		unsigned int   biRedMask; //������� ����� ��� �������� ����� 
		unsigned int   biGreenMask; //������� ����� ��� �������� ����� 
		unsigned int   biBlueMask; /////������� ����� ��� �������� ����� 
		unsigned int   biAlphaMask; //������� ����� ��� �����-������

		unsigned int   biCSType;
		CIEXYZTRIPLE   biEndpoints;
		unsigned int   biGammaRed;
		unsigned int   biGammaGreen;
		unsigned int   biGammaBlue;

		unsigned int   biIntent;
		unsigned int   biProfileData;
		unsigned int   biProfileSize;
		unsigned int   biReserved; //������ ���� ��������
	} BITMAPINFOHEADER;

	// rgb quad
	typedef struct {
		unsigned char  rgbBlue;
		unsigned char  rgbGreen;
		unsigned char  rgbRed;
		unsigned char  rgbReserved;
	} RGBQUAD;

	// read bytes
	template <typename Type>
	void read(std::ifstream& in, Type& result, std::size_t size) {
		in.read(reinterpret_cast<char*>(&result), size);
	}

	// write bytes
	template <typename Type>
	void write(std::ofstream& out, Type& result, std::size_t size) {
		out.write(reinterpret_cast<char*>(&result), size);
	}

	template <typename Type>
	void readANDwrite(std::ifstream& in, std::ofstream& out, Type& result, std::size_t size) {
		in.read(reinterpret_cast<char*>(&result), size);
		out.write(reinterpret_cast<char*>(&result), size);
	}
	unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
		if (mask == 0) {
			return 0;
		}
		int
			maskBufer = mask,
			maskPadding = 0;

		while (!(maskBufer & 1)) {
			maskBufer >>= 1;
			maskPadding++;
		}
		return (byte & mask) >> maskPadding;
	}
}

#endif // MAIN_H_INCLUDEDs