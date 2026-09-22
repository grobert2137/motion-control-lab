#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <math.h> // for pow()


unsigned int bit_merge(unsigned short s1, unsigned short s2) {
	unsigned int s2n = s2 <<16; // was wrong before because shifted output has to be "int" not "short"
	unsigned int ans = s1 | s2n;
	return ans;
}


int main() {

	unsigned short hn1 = 0x1234;
	unsigned short hn2 = 0xabcd;
	int result;

	result = bit_merge(hn1, hn2);
	printf("\nMerging 0x%x and 0x%x results 0x%x", hn1, hn2, result);

}