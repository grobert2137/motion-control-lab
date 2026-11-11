#include "../myWin826.h"
#include "../826api.h"
#include <time.h>
#include <fstream>
#include <math.h>

// Time parameters
#define SAMPLE_RATE         1000.0
#define PERFORMANCE_COUNTS  10000000 
#define PI                  3.141592653

// Define max size of iterations array
#define MAX_ITERATIONS 2500

// IO card configuration constants
#define IO_BOARD_NUM  0
#define DAC_CHANNEL   0
#define DAC_CONFIG_GAIN S826_DAC_SPAN_10_10   // -10 to 10 spans 20 volts so...
#define DAC_VRANGE    20.0                    // This MUST match the DAC_SPAN just above
#define DAC_CNT_RANGE 0xFFFF				  // 16-bit DAC
#define DAC_OFFSET_COUNTS 0x8000              // 32768 offset for a signed desired output mapped to unsigned DAC write.

// Encoder Constants
#define ENC_COUNTS_PER_REV         1000   // counts per revolution
#define ENC_REFERENCE_STEP_COUNTS  250  // Amplitude of Sin input   

#define SINE_STOP_TIME  2.0
#define SINE_KP         0.004f



// local function prototypes
static void sleepfor(LARGE_INTEGER from, long performance_clicks);
static int save_enc_count_data(int* filtered_data, int element_num);
static int runMotor(int plant, long sample_period);
static int runSineMotor(long sample_period);
static void setGainVals();
static void getGainVals();

// PID variables
float               Kp[6] = { 0.005, 0.0045,   0.006, 0.0045, 0.00405, 0.0054 };
float               Ki[6] = { 0.000, 0.0674,    0.15,  0.000,  0.0261, 0.058  };
float               Kd[6] = { 0.000,  0.000, 0.00006,  0.000,   0.000, 0.0001255 };
double        integral = 0;
double      derivative = 0;
double          output = 0;
double   previousError = 0;
double    counts_error = 0;
double		   command = 0;
double              Ts = 1 / SAMPLE_RATE; // Get sample period in seconds

// Loop Constants
const double freq[7] = { 4.0, 8.0, 10.0, 12.0, 14.0, 16.0, 20.0 };
const double kp_values[2] = {};
const size_t freq_length = sizeof(freq) / sizeof(freq[0]);

// ------------------------------------- MAIN LOOP --------------------------------------- //

int main() {
	
	int errcode = S826_ERR_OK;
	int boardflags = S826_SystemOpen();        // open 826 driver and find all 826 boards

	long sample_period;

	char key = '\0';

	//---*****WARNING!*****---DO NOT CHANGE THE CODE BETWEEN WARNINGS-----------------
	sample_period = PERFORMANCE_COUNTS / SAMPLE_RATE; // sample period in NT performance counts
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); // set this program to run at a high priority under NT to ensure real-time behavior
	//--***** END WARNING! *****-------------------------------------------------------

	// Configure interfaces and start them running
	X826(S826_DacRangeWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_CONFIG_GAIN, 0));  // program dac output range: -10V to +10V
	S826_CounterModeWrite(0, 0, S826_CM_K_QUADX1);  // Configure counter0 as incremental encoder interface.
	S826_CounterStateWrite(0, 0, 1);                // Start tracking encoder position.

	// Application Loop
	while (key != 'q') { // Shut down program with Q
		system("cls");
		printf("-------------HOME SCREEN--------------- \n [s] -> EXERCISE_1\n [r] -> RUN MODE (EXCERCISE 2-5)\n [w] -> RUN PLANT 2 ONLY\n\n");
		getGainVals();
		printf("\n\nPRESS Q TO EXIT\n");

		key = _getch();

		switch (key) {
			case 's':
				runSineMotor(sample_period);
				continue;
			case 'r':
				system("cls");
				printf("\n\n---------------- Starting Plant 1 (Unloaded) ----------------\n");
				runMotor(0, sample_period);

				// Wait until pully is attached
				 printf("\nAttach the pulley, then press any key...\n");
   				_getch();

				printf("\n\n---------------- Moving on to next plant ----------------\n");
				runMotor(1, sample_period);
				system("cls");
				continue;
			case 'w':
				system("cls");
				printf("\n\n---------------- Running ONLY Plant 2 (loaded) ----------------\n");
				runMotor(1, sample_period);
			case 'q':
			// exit main()
				break;
		}
	}

	S826_SystemClose();
	return 0;
}

// ------------------------------------ HELPER FUNCTIONS --------------------------------------- //

// Windows infrastructure for pacing loop time to achieve our SAMPLE_RATE
static void sleepfor(LARGE_INTEGER from, long performance_clicks)
{
	LARGE_INTEGER pc0;
	long pc_diff;

	QueryPerformanceCounter(&pc0);
	pc_diff = (long)pc0.QuadPart - (long)from.QuadPart;

	if (pc_diff > performance_clicks)
		printf(".");

	while (pc_diff < performance_clicks) {
		QueryPerformanceCounter(&pc0);
		pc_diff = (long)pc0.QuadPart - (long)from.QuadPart;
	}
	return;
}

// save encoder count array to text file
static int save_enc_count_data(int* filtered_data, int element_num)
{
	FILE* datafile = NULL;
	int i;

	char fileName[30] = "enc_data2.txt";
	printf("\nType in a file name:\n");
	scanf_s("%s", fileName, sizeof(fileName));
	strcat_s(fileName, ".txt");

	fopen_s(&datafile, fileName, "w");
	if (datafile == NULL)
	{
		printf("\nError opening file\n");
		return(-1);
	}

	for (i = 0; i < element_num; i++)
	{
		fprintf(datafile, "%d\n", filtered_data[i]);
	}

	fclose(datafile);

	return(0);
}

int runMotor(int plant, long sample_period) {
	int start_idx;
	int end_idx;
	float stop_time;


	// loop timing control
	float elapsed_time, ffr;
	int    ncount = 0;
	clock_t start, stop;
	LARGE_INTEGER hr0;
	unsigned int iterations;
	unsigned int ref_position = ENC_REFERENCE_STEP_COUNTS;
	int enc_samples[MAX_ITERATIONS];
	int errcode = S826_ERR_OK;

	// dac count variables
	uint counts;
	uint start_count;
	uint dacout;
	double counts_error;

	switch (plant) {
		case 0:
			start_idx = 0;
			end_idx = 2;
			stop_time = 2;
			break;
		case 1:
			start_idx = 3;
			end_idx = 5;
			stop_time = 2.5;
			break;
		}
	
	// Plant specific run value defines
	iterations = (unsigned int)(stop_time*SAMPLE_RATE);
	
	for (int i = start_idx; i <= end_idx; i++) {

		// Reset PI and PID values
		integral = 0;
		counts_error = 0;
		previousError = 0;

		// reset ncount
		ncount = 0;

		printf("\nValues Reset. Running test %d\n", i);
		S826_CounterRead(0, 0, &start_count);  // Read initial encoder counts
		start = clock();

		// get system performance counts NOW
		QueryPerformanceCounter(&hr0);

		// 2 or 2.5 second control loop
		while (ncount < iterations) {
			// Read current encoder counts
			S826_CounterRead(0, 0, &counts);
			counts = counts - start_count;

			counts_error = (double)((int)ref_position - (int)counts);
		
			derivative = (counts_error - previousError) / Ts;
  			integral += counts_error * Ts;

			previousError = counts_error;

			command = (Kp[i] * counts_error) + (Ki[i] * integral) + (Kd[i] * derivative);

			// Clamp control input as a safety rails so we don't crash
			if (fabs(command) > DAC_VRANGE / 2.0) {
				command = command / fabs(command) * DAC_VRANGE / 2.0;
				printf("command saturated!!!");
			}
			printf("command %lf\n", command);
			printf("counts %10d\n", counts);

			dacout = (uint)(command * (uint)(DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);
			X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));

			if (ncount < iterations) {
				enc_samples[ncount] = counts;
				ncount++;
			}

			sleepfor(hr0, sample_period);
			QueryPerformanceCounter(&hr0); // counts from right NOW
		}

		//Zero the DAC output immediately upon loop exit
		X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, 0x8000, 0));   // put to zero at the end

		stop = clock();
		elapsed_time = (float)(stop - start) / CLOCKS_PER_SEC; // calculate how fast the loop ran
		ffr = ((float)ncount) / elapsed_time;
		printf("\n%d cycles in %f sec (%f Hz, %f sec))\n", ncount, elapsed_time, ffr, 1.0 / ffr);

		FILE* datafile = NULL;
		int i;

		/*
		P Control (Plant 1)   = 0
		PI Control (Plant 1)  = 1
		PID Control (Plant 1) = 2

		P Control (Plant 2)   = 3
		PI Control (Plant 2)  = 4
		PID Control (Plant 2) = 5
		*/
		char fileName[30];
		snprintf(fileName, sizeof(fileName), "ControllerTest_%d.txt", (int)i);

		fopen_s(&datafile, fileName, "w");
		if (datafile == NULL) {
			printf("\nError opening file\n");
			return(-1);
		}

		for (i = 0; i < element_num; i++) {
			fprintf(datafile, "%d\n", filtered_data[i]);
		}

		fclose(datafile);
		Sleep(2000);
	}
	return 0;
}

static int runSineMotor(long sample_period)
{
	int errcode = S826_ERR_OK;   // needed by X826()
	unsigned int iterations = (unsigned int)(SINE_STOP_TIME * SAMPLE_RATE);

	static int    enc_samples[MAX_ITERATIONS];
	static double ref_samples[MAX_ITERATIONS];

	for (size_t j = 0; j < freq_length; j++) {
		uint start_count, counts, dacout;
		unsigned int ncount = 0;
		LARGE_INTEGER hr0;
		clock_t start, stop;

		printf("\nRUNNING TEST - %.1f Hz\n", freq[j]);

		// Re-zero the encoder for this test
		S826_CounterRead(IO_BOARD_NUM, 0, &start_count);

		start = clock();
		QueryPerformanceCounter(&hr0);

		while (ncount < iterations) {
			// Position relative to the start of this test (signed)
			S826_CounterRead(IO_BOARD_NUM, 0, &counts);
			int position = (int)(counts - start_count);

			// Sine reference in encoder counts
			double t   = ncount / SAMPLE_RATE;
			double ref = ENC_REFERENCE_STEP_COUNTS * sin(2.0 * PI * freq[j] * t);

			// P control
			double cmd = SINE_KP * (ref - position);

			// Clamp to DAC range
			if (fabs(cmd) > DAC_VRANGE / 2.0)
				cmd = (cmd > 0 ? 1.0 : -1.0) * DAC_VRANGE / 2.0;

			dacout = (uint)(cmd * (DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);
			X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));

			enc_samples[ncount] = position;
			ref_samples[ncount] = ref;
			ncount++;

			sleepfor(hr0, sample_period);
			QueryPerformanceCounter(&hr0);
		}

		// Zero the motor immediately
		X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_OFFSET_COUNTS, 0));

		stop = clock();
		float elapsed = (float)(stop - start) / CLOCKS_PER_SEC;
		printf("%u cycles in %f sec (%f Hz)\n", ncount, elapsed, ncount / elapsed);

		// Save: time, reference, position
		char fileName[40];
		snprintf(fileName, sizeof(fileName), "Exercise1_%dHz.txt", (int)freq[j]);

		FILE* datafile = NULL;
		fopen_s(&datafile, fileName, "w");
		if (datafile == NULL) {
			printf("Error opening %s\n", fileName);
			continue;
		}
		for (unsigned int n = 0; n < ncount; n++) {
			fprintf(datafile, "%f,%f,%d\n", n / SAMPLE_RATE, ref_samples[n], enc_samples[n]);
		}
		fclose(datafile);
		printf("Saved %s\n", fileName);

		// Let the motor settle before the next frequency
		Sleep(2000);
	}

	printf("\nAll sine tests done. Press any key to return...\n");
	_getch();
	return 0;
}

void getGainVals() {
	for (int i = 0; i < 6; i++) {
		printf("\nKp = %f , Ki = %f, Kd = %f", Kp[i], Ki[i], Kd[i]);
	}
}