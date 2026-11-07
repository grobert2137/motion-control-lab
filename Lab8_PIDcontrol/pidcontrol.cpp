#include "../myWin826.h"
#include "../826api.h"
#include <time.h>
#include <fstream>

// Time parameters
#define SAMPLE_RATE         1000.0
#define PERFORMANCE_COUNTS  10000000 
#define PI                  3.141592653

#define STOP_TIME 2  // seconds
#define ITERATIONS (unsigned int)(STOP_TIME*SAMPLE_RATE)

// IO card configuration constants
#define IO_BOARD_NUM  0
#define DAC_CHANNEL   0
#define DAC_CONFIG_GAIN S826_DAC_SPAN_10_10   // -10 to 10 spans 20 volts so...
#define DAC_VRANGE    20.0                    // This MUST match the DAC_SPAN just above
#define DAC_CNT_RANGE 0xFFFF				  // 16-bit DAC
#define DAC_OFFSET_COUNTS 0x8000              // 32768 offset for a signed desired output mapped to unsigned DAC write.

// Encoder Constants
#define ENC_COUNTS_PER_REV         1000   // counts per revolution
#define ENC_REFERENCE_STEP_COUNTS  500    

// local function definitions
static void sleepfor(LARGE_INTEGER from, long performance_clicks);
static int save_enc_count_data(int* filtered_data, int element_num);


// ------------------------------------- MAIN LOOP --------------------------------------- //

int main() {
	// Sensoray Configuration items..could be defines but leave for now
	int errcode = S826_ERR_OK;
	int boardflags = S826_SystemOpen();        // open 826 driver and find all 826 boards

	// loop timing control
	float elapsed_time, ffr;
	int    ncount = 0;
	long   sample_period;
	clock_t start, stop;
	LARGE_INTEGER hr0;

	// dac count variables
	uint counts;
	uint start_count;
	uint dacout;
	double counts_error;

	int          enc_samples[ITERATIONS];
	unsigned int ref_position = ENC_REFERENCE_STEP_COUNTS;
	float        Kp = 0.008;
	float command;
	char         key = '\0';



	// Application Loop
	while (key != 'q') { // Shut down program with Q

		system("cls");
		printf("-------------HOME SCREEN--------------- \n [t] -> TUNING MODE\n [r] -> RUN MODE\n [k] -> KP ADJUST\n [s] -> SAVE PREVIOUS RUN\n\n");
		printf("\nKP = %.3f", Kp);
		printf("\nREF_POSITION = %u", ref_position);
		printf("\n\nPRESS Q TO EXIT\n");

		key = _getch();

		switch (key) {

		case 't':
			system("cls");
			printf("-------WELCOME TO TUNING MODE!--------\n\n Press RIGHT ARROW to +500\n Press LEFT ARROW to -500\n\n PRESS E TO EXIT\n");
			while (key != 'e') {
				key = _getch();
				if (77 == key) { ref_position += 500; printf("\033[2K\rref_position = %u", ref_position); }
				else if (75 == key) { ref_position -= 500; printf("\033[2K\rref_position = %u", ref_position); }
			}
			continue;

		case 'r':
			system("cls");
			printf("---------- WELCOME TO RUN MODE ---------------");


			//---*****WARNING!*****---DO NOT CHANGE THE CODE BETWEEN WARNINGS-----------------
			sample_period = PERFORMANCE_COUNTS / SAMPLE_RATE; // sample period in NT performance counts
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); // set this program to run at a high priority under NT to ensure real-time behavior
			//--***** END WARNING! *****-------------------------------------------------------

			// Configure interfaces and start them running
			X826(S826_DacRangeWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_CONFIG_GAIN, 0));  // program dac output range: -10V to +10V
			S826_CounterModeWrite(0, 0, S826_CM_K_QUADX1);  // Configure counter0 as incremental encoder interface.
			S826_CounterStateWrite(0, 0, 1);                // Start tracking encoder position.
			S826_CounterRead(0, 0, &start_count);           // Read initial encoder counts 

			// Get reference time just as we  enter the loop for post analysis of timing.
			// We compare windows timer with total time divided by number of iterations.
			start = clock();

			// get system performance counts NOW
			QueryPerformanceCounter(&hr0);

			while (key != 'e') {
				// Read current encoder counts
				S826_CounterRead(0, 0, &counts);
				counts = counts - start_count;

				counts_error = (double)((int)ref_position - (int)counts);
				command = Kp * counts_error;

				// Clamp control input as a safety rails so we don't crash
				if (abs(command) > DAC_VRANGE / 2.0) {
					command = command / abs(command) * DAC_VRANGE / 2.0;
					printf("command saturated!!!");
				}
				printf("command %lf\n", command);
				printf("counts %10d\n", counts);

				dacout = (uint)(command * (uint)(DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);
				X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));

				if (ncount < ITERATIONS) {
					enc_samples[ncount] = counts;
					ncount++;
				}

				if (_kbhit()) {
					key = _getch();
				}

				sleepfor(hr0, sample_period);
				QueryPerformanceCounter(&hr0); // counts from right NOW

				//--***** END WARNING! *****-------------------------------------------------------

			}
			//Zero the DAC output immediately upon loop exit
			X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, 0x8000, 0));   // put to zero at the end

			//---*****WARNING!*****---DO NOT CHANGE THE CODE BETWEEN WARNINGS-----------------
			stop = clock();
			elapsed_time = (float)(stop - start) / CLOCKS_PER_SEC; // calculate how fast the loop ran
			ffr = ((float)ncount) / elapsed_time;
			printf("\n%d cycles in %f sec (%f Hz, %f sec))\n", ncount, elapsed_time, ffr, 1.0 / ffr);

			save_enc_count_data(enc_samples, ncount);

			continue;

		case 'k':
			// user input...
			printf("\nPlease enter the gain Kp:\n");
			scanf_s("%f", &Kp);
			printf("\nNew Kp Value is: %f\n", Kp);
			Sleep(2000);
			continue;

		case 's':
			printf("Saving previous data run as");

			continue;

		case 'q':
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

