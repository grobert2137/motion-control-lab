#include <time.h>
#include "../myWin826.h"
#include "../826api.h"
#include "../RealTime.h"

// Sensoray Config
#define IO_BOARD_NUM  0	

// ADC Config (INPUT)
#define ADC_SLOT 0 
#define ADC_ENABLE 1
#define ADC_CNT_RANGE 0xFFFF				  // 2^16= 0xFFFF (16 bit converter) 
#define ADC_GAIN     S826_ADC_GAIN_1          // -10 to 10 option
#define ADC_VRANGE   20                       // this must correspond to gain setting

// DAC Config (OUTPUT)
#define DAC_CNT_RANGE 0xFFFF				  // 16-bit DAC
#define DAC_CONFIG_GAIN S826_DAC_SPAN_10_10   // -10 to 10 spans 20 volts so...
#define DAC_VRANGE    20.0                    // This MUST correspond to the DAC_SPAN just above
#define DAC_ZERO_OUTPUT 0x8000
#define DAC_OFFSET_COUNTS 0x8000              // 32768 offset for a signed desired output mapped to unsigned DAC write.

// Channels
#define DAC_CHANNEL   7						  
#define ADC_CHANNEL  1 

// Sample Rate
#define SAMPLE_RATE  1000  

int main()
{
	int  errcode = S826_ERR_OK;
	int  boardflags = S826_SystemOpen();  // open 826 driver and find all 826 boards
	int  slotdata;              
	int  ncount = 0;
	uint dacout;

	double voltagein = 0.0;
	double voltageout = 0;

	double y_voltagein_filt = 0.0;
	double z_voltagein_filt = 0.0;
	double y_voltagein_filt_prev = 0.0;
	double z_voltagein_filt_prev = 0.0;

	double A = 0.846;
	double B = 0.154;

	// Instantiate RealTime with defined sample rate
	RealTime realTime(SAMPLE_RATE);

	// Configure data achqisition interfaces and start them running.
	X826(S826_AdcSlotConfigWrite(IO_BOARD_NUM, ADC_SLOT, ADC_CHANNEL, 0, ADC_GAIN));  // program adc timeslot attributes: slot, chan, 0us settling time. For -5V-5V use: "S826_ADC_GAIN_2"
	X826(S826_AdcSlotlistWrite(IO_BOARD_NUM, 1 << ADC_SLOT, S826_BITWRITE));  // enable adc timeslot; disable all other slots
	X826(S826_AdcEnableWrite(IO_BOARD_NUM, ADC_ENABLE));  // enable adc conversions

	X826(S826_DacRangeWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_CONFIG_GAIN, 0));  // program dac output range: -10V to +10V

	// commence pseudo-real-time loop.
	realTime.Start();

	while (!_kbhit())
	{
		// ADC Input
		X826(AdcReadSlot(IO_BOARD_NUM, ADC_SLOT, &slotdata));
		signed short int adcin = slotdata;
		voltagein = (double)(adcin * ADC_VRANGE) / ADC_CNT_RANGE;

		//------------FILTERING----------------
		// SECOND order IIR filter
		y_voltagein_filt = (double)(A * y_voltagein_filt_prev) + (B * voltagein);
		z_voltagein_filt = (double)(A * z_voltagein_filt_prev) + (B * y_voltagein_filt);

		// Save for next loop
		y_voltagein_filt_prev = y_voltagein_filt;
		z_voltagein_filt_prev = z_voltagein_filt;

		// DAC Output
		voltageout = z_voltagein_filt;
		dacout = (uint)(voltageout * (DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);
		X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));

		realTime.Sleep();
		ncount++;
	}

	realTime.Stop(ncount);
	X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_ZERO_OUTPUT, 0));   // put to zero at the end
	S826_SystemClose();
	return 0;
}