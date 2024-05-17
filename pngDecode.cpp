#include <fstream>

unsigned int CRC32_function(unsigned char* buf, unsigned long len) {
	unsigned long crc_table[256];
	unsigned long crc;
	for (int i = 0; i < 256; i++) {
		crc = i;
		for (int j = 0; j < 8; j++)
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
		crc_table[i] = crc;
	};
	crc = 0xFFFFFFFFUL;
	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	return crc ^ 0xFFFFFFFFUL;
}
void changeHeight(unsigned char* buffer_IHDR, char* buffer, unsigned int height) {
	buffer_IHDR[8] = (height >> 24);
	buffer_IHDR[9] = ((height << 8) >> 24);
	buffer_IHDR[10] = ((height << 16) >> 24);
	buffer_IHDR[11] = ((height << 24) >> 24);
	buffer[20] = (height >> 24);
	buffer[21] = ((height << 8) >> 24);
	buffer[22] = ((height << 16) >> 24);
	buffer[23] = ((height << 24) >> 24);
	unsigned int newCRC = CRC32_function(buffer_IHDR, 17);
	buffer[29] = (newCRC >> 24);
	buffer[30] = ((newCRC << 8) >> 24);
	buffer[31] = ((newCRC << 16) >> 24);
	buffer[32] = ((newCRC << 24) >> 24);
}
extern "C" __declspec(dllexport) void pngDecode(std::string fileName, std::string fileOutName, unsigned int h) {
	std::ifstream in(fileName, std::ios::binary);
	std::ifstream inLength(fileName, std::ios::binary);
	std::ofstream out(fileOutName, std::ios::binary);
	if (in.is_open()) {
		inLength.seekg(0, inLength.end);
		const unsigned int n = inLength.tellg();
		char* buffer = new char[n];
		in.read(buffer, n);
		unsigned char* buffer_IHDR = new unsigned char[17];
		for (int k = 0; k < 17; k++) {
			buffer_IHDR[k] = buffer[12 + k];
		}
		changeHeight(buffer_IHDR, buffer, h);
		out.write(buffer, n);
		delete[] buffer_IHDR;
		delete[] buffer;
	}
	in.close();
	inLength.close();
	out.close();
	return;
}