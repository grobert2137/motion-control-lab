#include <stdio.h>
#include <fstream>

int main() {
	int i; 
	int j;
	int Sum = 0;
	int arr[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	char ch;
	char fileName[30];
	
	// Define frequencies and amplitudes as arrays of doubles
	double freq[16] = { 2.5, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 75.0 };
	double amp[2] = { 0.5, 1.0 };

	size_t freq_length = sizeof(freq) / sizeof(freq[0]);
	size_t amp_length = sizeof(amp) / sizeof(amp[0]);

	for (j = 0; j < amp_length; j++) {
		for (i = 0; i < freq_length; i++) {
			printf("\n%lf V amplitude, %lf Hz frequency", amp[j], freq[i]);

			int ampName = 10 * amp[j];
			int freqName = 10 * freq[i];
			snprintf(fileName, sizeof(fileName), "%d_%d", ampName, freqName);

			printf("\nFilename is: %s", fileName);

			// Wait until ENTER 
			while ((ch = getchar()) != '\n' && ch != EOF);
		}
		printf("\n\n---------------- Moving on to next amplitude ----------------\n");
	}

	printf("\n---------------- END ----------------\n");
	return 0;
}