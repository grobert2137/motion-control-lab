#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES // for C++  
#include <cmath>
#include <time.h>
#include <fstream>
#include <iostream>
#include "IIRdf1filt.h"
#include "../myWin826.h"
#include "../826api.h"
#include "../RealTime.h"

#define SAMPLE_RATE  1000    // Hz
#define MAX_LOOP_TIME  10    //seconds
#define RECORD_LENGTH (MAX_LOOP_TIME*SAMPLE_RATE)
#define IO_BOARD_NUM  0						  
#define ADC_SLOT      0						  
#define DAC_ZERO_OUTPUT 0x8000
#define DAC_CONFIG_GAIN S826_DAC_SPAN_10_10   
#define DAC_VRANGE    20.0                    
#define DAC_CNT_RANGE 0xFFFF				  
#define DAC_OFFSET_COUNTS 0x8000            
#define DAC_CHANNEL  0					  
#define ADC_CHANNEL  2 
#define ADC_CNT_RANGE 0xFFFF				   
#define ADC_GAIN     S826_ADC_GAIN_1         
#define ADC_VRANGE   20                    
#define ADC_ENABLE 1

const double a[] = { 1.0000000000000000, -1.991114292201654, 0.991153595868935 };
const double b[] = { 0.995566972017647, -1.991133944035295, 0.995566972017647 };

const double freq[16] = { 2.5, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 75.0 };
const double amp[2] = { 0.5, 1.0 };
const size_t freq_length = sizeof(freq) / sizeof(freq[0]);
const size_t amp_length = sizeof(amp) / sizeof(amp[0]);

int main() {
	int  errcode = S826_ERR_OK;
	int  boardflags = S826_SystemOpen(); // open 826 driver and find all 826 boards
	int  ncount = 0;
	uint dacout;
	//char localPath[20] = "output/";

	int i, j, n;
	char ch;
	char fileName[30];

	double voltageout = 0;
	double Amp = 0;
	double Frequency = 0;

	uint   start_count;
	uint   counts;
	int    rawcounts[RECORD_LENGTH];
	double filtcounts[RECORD_LENGTH];

	// real-time filter
	int  N = sizeof(a) / sizeof(double); // number of a's and b's. filter order plus 1.
	IIRdf1filt filter(N, a, b);          // instantiate and initialize the filter

	// instantiate our "real time" object that will pace our loop
	RealTime realTime(SAMPLE_RATE);

	// Configure data acquisition interfaces and start them running.
	X826(S826_AdcSlotConfigWrite(IO_BOARD_NUM, ADC_SLOT, ADC_CHANNEL, 0, ADC_GAIN));  // program adc timeslot attributes: slot, chan, 0us settling time. For -5V-5V use: "S826_ADC_GAIN_2"
	X826(S826_AdcSlotlistWrite(IO_BOARD_NUM, 1 << ADC_SLOT, S826_BITWRITE));  // enable adc timeslot; disable all other slots
	X826(S826_AdcEnableWrite(IO_BOARD_NUM, ADC_ENABLE));  // enable adc conversions
	X826(S826_DacRangeWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_CONFIG_GAIN, 0));  // program dac output range: -10V to +10V

	// Initialize Motor Shaft Encoder Interface
	S826_CounterModeWrite(0, 0, S826_CM_K_QUADX1); // Configure counter0 as incremental encoder interface.
	S826_CounterStateWrite(0, 0, 1);               // Start tracking encoder position.
	S826_CounterRead(0, 0, &start_count);          // Read initial encoder counts 

	printf("\nHello! I'm the automated version of the motor System ID application\n");

	// commence pseudo-real-time loop.
	
	for (j = 0; j < amp_length; j++) {
		for (i = 0; i < freq_length; i++) {

			realTime.Start();
			printf("\nRUNNING TEST - %lf V amplitude, %lf Hz frequency", amp[j], freq[i]);
			int ampName = 10 * amp[j];
			int freqName = 10 * freq[i];
			snprintf(fileName, sizeof(fileName), "%d_%d", ampName, freqName);

			while (!_kbhit() && ncount < RECORD_LENGTH) {
				voltageout = amp[j] * sin(2 * M_PI * freq[i] * ncount / SAMPLE_RATE);
				// scale the desired voltage to the DAC integer output.
				dacout = (uint)(voltageout * (DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);
				// output to the DAC
				X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));
				S826_CounterRead(0, 0, &counts);          // Read current encoder counts.
				counts = counts - start_count;             // Subtracting the initial counts so we begin from 0 
				rawcounts[ncount] = counts;
				filtcounts[ncount] = filter.FilterSample((float)rawcounts[ncount]);
				realTime.Sleep();
				ncount++;
			}
			realTime.Stop(ncount); // stop real-time loop

			X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_ZERO_OUTPUT, 0));   // put to zero at the end
			

			// Write Data to Output File
			std::string fullPath = std::string(fileName) + ".txt";
			std::ofstream outfile(fullPath); 

			for (n = 0; n < ncount; n++) {
				outfile << rawcounts[n] << "," << filtcounts[n] << "\n";
			}

			ncount = 0;
			counts = 0;


			outfile.close();
			printf("\nFilename is: %s", fileName);

			// Wait until ENTER 
			//while ((ch = getchar()) != '\n' && ch != EOF);

			//Sleep 2 seconds
			Sleep(2000);
		}
		printf("\n\n---------------- Moving on to next amplitude ----------------\n");
	}
	S826_SystemClose();
	return 0;
}

