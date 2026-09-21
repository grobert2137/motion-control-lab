#include <stdio.h>
#include <stdint.h>
// Helper function to print binary representation
void print_binary(unsigned int value, int bits) {
	for (int i = bits - 1; i >= 0; i--) {
		printf("%d", (value >> i) & 1);
	}
}
// Helper function to convert a number stored in a limited bit width into a properly signed 32 - bit int.
// Useful for handling packed signed values extracted from data.
int adjust_bit_length(int value, int bitWidth)
{
	int signbit = 1U << (bitWidth - 1);
	if (value & signbit)
	{
		int bitextension = 0;
		bitextension = (~bitextension) << bitWidth;
		printf("bitextension: %x\n", bitextension);
		return (int)(value | bitextension);
	}
	else
	{
		return value;
	}
}
int main(void) {
	/* GRANT DATA */
	unsigned char data[] = { 0xe4, 0x87, 0x18, 0x0f, 0x09, 0x8f, 0x0c, 0xcb };
	/* */

	// ================= Fx (11 bits) =================
	unsigned int fx_part1 = data[0] << 3; // all of data[0], shifted up
	unsigned int fx_part2 = data[1] >> 5; // top 3 bits of data[1]
	unsigned int fx_temp = fx_part1 | fx_part2;

	// ================= Fy (11 bits) =================
	unsigned int fy_part1 = data[1] & 0x1F; // lower 5 bits of data[1]
	fy_part1 = fy_part1 << 6; // shift into position
	unsigned int fy_part2 = data[2] >> 2; // top 6 bits of data[2]
	unsigned int fy_temp = fy_part1 | fy_part2;
	// ================= Fz (12 bits) =================

	/* FILL IN THIS PART */
	unsigned int fz_part1 = data[2] & 0x03; // lowest 2 bits of data[2]
	fz_part1 = fz_part1 << 10; // move into top position
	unsigned int fz_part2 = data[3] << 2; // all 8 bits of data[3], shifted
	unsigned int fz_part3 = data[4] >> 6; // top 2 bits of data[4]
	unsigned int fz_temp = fz_part1 | fz_part2 | fz_part3; // merge 3 components together
	/* UNTIL HERE */

	// ================= Example code to print binary results if needed for debugging =================
	print_binary(fz_part1, 12);
	printf("\n");
	print_binary(fy_temp, 11);
	printf("\n");

	/* CHECK THIS PRINT OUT TO SEE IF YOU GOT THE RIGHT ANSWER */
	print_binary(fz_temp, 12);
	printf("\n");

	// ================= Convert to Forces =================
	float fx = ((float)adjust_bit_length(fx_temp, 11)) / 40;
	float fy = ((float)adjust_bit_length(fy_temp, 11)) / 40;
	float fz = ((float)adjust_bit_length(fz_temp, 12)) / 40;
	printf("Fx: %f\n", fx);
	printf("Fy: %f\n", fy);
	printf("Fz: %f\n", fz);
	return 0;
}
