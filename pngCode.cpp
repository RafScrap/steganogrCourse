#include <fstream>

extern "C" __declspec(dllexport) unsigned int pngCode(std::string fileOldName, std::string fileNewName, std::string fileOutName) {
	std::ifstream inOld(fileOldName, std::ios::binary);
	std::ifstream inNew(fileNewName, std::ios::binary);
	std::ifstream inLength(fileNewName, std::ios::binary);
	std::ofstream out(fileOutName, std::ios::binary);
	unsigned int height = 0;
	if (inOld.is_open() && inNew.is_open()) {
		char* bufferOld = new char[33];
		inOld.read(bufferOld, 33);
		inLength.seekg(0, inLength.end);
		const unsigned int n = inLength.tellg();
		char* bufferNew = new char[n];
		inNew.read(bufferNew, n);
		height = (bufferNew[20] << 24) + (bufferNew[21] << 16) + (bufferNew[22] << 8) + bufferNew[23];
		bufferNew[20] = bufferOld[20];
		bufferNew[21] = bufferOld[21];
		bufferNew[22] = bufferOld[22];
		bufferNew[23] = bufferOld[23];
		bufferNew[29] = bufferOld[29];
		bufferNew[30] = bufferOld[30];
		bufferNew[31] = bufferOld[31];
		bufferNew[32] = bufferOld[32];
		out.write(bufferNew, n);
		delete[] bufferNew;
		delete[] bufferOld;
	}
	inOld.close();
	inNew.close();
	inLength.close();
	out.close();
	return height;
}