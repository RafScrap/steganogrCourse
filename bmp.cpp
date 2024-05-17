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