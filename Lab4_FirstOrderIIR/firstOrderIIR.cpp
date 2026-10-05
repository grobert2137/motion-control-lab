#include <time.h>
#include "../myWin826.h"
#include "../826api.h"
#include "../RealTime.h"

#define SAMPLE_RATE  1000    // SET ME

// IO card configuration constants
#define IO_BOARD_NUM  0						  // no change
#define ADC_SLOT      0						  // no change

#define DAC_ZERO_OUTPUT 0x8000
#define DAC_CONFIG_GAIN S826_DAC_SPAN_10_10   // -10 to 10 spans 20 volts so...
#define DAC_VRANGE    20.0                    // This MUST correspond to the DAC_SPAN just above
#define DAC_CNT_RANGE 0xFFFF				  // 16-bit DAC
#define DAC_OFFSET_COUNTS 0x8000              // 32768 offset for a signed desired output mapped to unsigned DAC write.

#define DAC_CHANNEL   7						  // SET ME! CHECK YOUR SETUP: DAC channel output

#define ADC_CHANNEL  1                        // SET ME! CHECK YOUR SETUP: analog input channel to track


#define ADC_CNT_RANGE 0xFFFF				  // 2^16= 0xFFFF (16 bit converter) 
#define ADC_GAIN     S826_ADC_GAIN_1          // -10 to 10 option
#define ADC_VRANGE   20                       // this must correspond to gain setting


#define ADC_ENABLE 1

int main()
{
	int  errcode = S826_ERR_OK;
	int  boardflags = S826_SystemOpen();        // open 826 driver and find all 826 boards
	int  slotdata;               // adc data from the slot of interest
	int  ncount = 0;
	uint dacout;

	double voltagein = 0.0;
	double voltageout = 0;

	// instantiate our "real time" object that will pace our loop
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
		// read ADC sample
		X826(AdcReadSlot(IO_BOARD_NUM, ADC_SLOT, &slotdata));
		signed short int adcin = slotdata;

		// get the ADC sample as a voltage
		voltagein = (double)(adcin * ADC_VRANGE) / ADC_CNT_RANGE;
		voltageout = voltagein;

		// scale the desired voltage to the DAC integer output.
		dacout = (uint)(voltageout * (DAC_CNT_RANGE / DAC_VRANGE) + DAC_OFFSET_COUNTS);

		// output to the DAC
		X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, dacout, 0));

		realTime.Sleep();
		ncount++;
	}

	realTime.Stop(ncount); // stop real-time loop


	X826(S826_DacDataWrite(IO_BOARD_NUM, DAC_CHANNEL, DAC_ZERO_OUTPUT, 0));   // put to zero at the end
	S826_SystemClose();


	return 0;
}