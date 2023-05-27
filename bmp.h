#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

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
	unsigned short bfType; //‘ормат файла
	unsigned int   bfSize; //–азмер файла в байтах
	unsigned short bfReserved1; //ƒолжен содержать ноль
	unsigned short bfReserved2; //ƒолжен содержать ноль
	unsigned int   bfOffBits; //ѕоложение пиксельных данных относительно начала данной структуры (в байтах)
} BITMAPFILEHEADER;

// bitmap info header
typedef struct {
	unsigned int   biSize; //–азмер BITMAPINFOHEADER в байтах
	unsigned int   biWidth; //Ўирина в пиксел€х
	unsigned int   biHeight; //¬ысота растра в пиксел€х (*)
	unsigned short biPlanes; //ƒопустимо только значение 1
	unsigned short biBitCount; // оличество бит на пиксель (16, 24, 32)
	unsigned int   biCompression; //способ хранени€ пикселей
	//(0 - двумерный массив, 3 - двумерный массив с масками цветовых каналов при biBitCount = 16, 32)
	unsigned int   biSizeImage; //размер в байтах дл€ данных в виде строки (у нас двумерный массив, см. biCompression)
	unsigned int   biXPelsPerMeter; // оличество пикселей на метр по горизонтали
	unsigned int   biYPelsPerMeter; // оличество пикселей на метр по вертикали
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;

	unsigned int   biRedMask; //битова€ маска дл€ красного цвета 
	unsigned int   biGreenMask; //битова€ маска дл€ зеленого цвета 
	unsigned int   biBlueMask; /////битова€ маска дл€ красного цвета 
	unsigned int   biAlphaMask; //битова€ маска дл€ альфа-канала

	unsigned int   biCSType;
	CIEXYZTRIPLE   biEndpoints;
	unsigned int   biGammaRed;
	unsigned int   biGammaGreen;
	unsigned int   biGammaBlue;

	unsigned int   biIntent;
	unsigned int   biProfileData;
	unsigned int   biProfileSize;
	unsigned int   biReserved; //ƒолжно быть обнулено
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
	// определение количества нулевых бит справа от маски
	int
		maskBufer = mask,
		maskPadding = 0;

	while (!(maskBufer & 1)) {
		maskBufer >>= 1;
		maskPadding++;
	}
	// применение маски и смещение
	return (byte & mask) >> maskPadding;
}

#endif // MAIN_H_INCLUDEDs