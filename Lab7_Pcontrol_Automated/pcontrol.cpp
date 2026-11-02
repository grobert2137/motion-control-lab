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
	uint counts_error; 

	int  enc_samples[ITERATIONS];
	unsigned int ref_position = ENC_REFERENCE_STEP_COUNTS;
	float        Kp = 0;
	char key = '\0';

	//---*****WARNING!*****---DO NOT CHANGE THE CODE BETWEEN WARNINGS-----------------
	sample_period = PERFORMANCE_COUNTS / SAMPLE_RATE; // sample period in NT performance counts
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); // set this program to run at a high priority under NT to ensure real-time behavior
	//--***** END WARNING! *****-------------------------------------------------------

	// Configure interfaces and start them running
	X826(S826_DacRangeWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_CONFIG_GAIN, 0));  // program dac output range: -10V to +10V
	S826_CounterModeWrite(0, 0, S826_CM_K_QUADX1);  // Configure counter0 as incremental encoder interface.
	S826_CounterStateWrite(0, 0, 1);                // Start tracking encoder position.
	S826_CounterRead(0, 0, &start_count);           // Read initial encoder counts 


	// user input...
	printf("\nPlease enter the gain Kp:\n");
	scanf_s("%f", &Kp);

	// Get reference time just as we  enter the loop for post analysis of timing.
	// We compare windows timer with total time divided by number of iterations.
	start = clock();

	// get system performance counts NOW
	QueryPerformanceCounter(&hr0);

	// Control Loop...
	while (key != 'q') {

		/*** Your Motor Control Code logic goes here ***/

		/* read encoder */
		S826_CounterRead(0, 0, &counts);          // Read current encoder counts.
		counts = counts - start_count;             // Subtracting the initial counts so we begin from 0 


		/* generate input signal for voltage to command */
		counts_error = ref_position - counts;
		command = kp * (float)counts_error;


		// Clamp control input as a safety rails so we don't crash
		if (abs(command) > DAC_VRANGE / 2.0) {
			command = command / abs(command) * DAC_VRANGE / 2.0;
			printf("command saturated!!!");
		}
		printf("command %lf\n", command);
		printf("counts %10d\n", counts);

		// Map command voltage to command counts, and offset the signed command to unsigned DAC write.
		// See above where DAC is initialized -VRANGE to +VRANGE. Its input range is unsigned 16-bit.
		// We need to shift our bipolar command up into this unsigned DAC input range.
		// The DAC will map it back to negative or positive based on the setup above.

		dacout = (uint)(command * (uint)(DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);
		X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));

		if (ncount < ITERATIONS) {
			enc_samples[ncount] = counts;
			ncount++;
		}

		if (_kbhit()) {
			key = _getch();
			if (77 == key) { //if right arrow key is hit
				ref_position += 500;
			}
			else if (75 == key) { //if left arrow key is hit
				ref_position -= 500;
			}
		}

		//enc_samples[ncount] = counts;
		sleepfor(hr0, sample_period);
		QueryPerformanceCounter(&hr0); // counts from right NOW
		//ncount++;
	}

	// Zero the DAC output immediately upon loop exit
	X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, 0x8000, 0));   // put to zero at the end

	//---*****WARNING!*****---DO NOT CHANGE THE CODE BETWEEN WARNINGS-----------------
	stop = clock();
	elapsed_time = (float)(stop - start) / CLOCKS_PER_SEC; // calculate how fast the loop ran
	ffr = ((float)ncount) / elapsed_time;
	printf("\n%d cycles in %f sec (%f Hz, %f sec))\n", ncount, elapsed_time, ffr, 1.0 / ffr);
	S826_SystemClose();
	//--***** END WARNING! *****-------------------------------------------------------
 
	save_enc_count_data(enc_samples, ncount); // save data
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

