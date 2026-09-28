## How to Setup Sensoray Projects

### Part 1: Ensure .cpp files are compiled with Project
- In Solution Explorer, right-click the project (for example Lab3_SineIO) and choose Add → **Existing Item...**
- Go up one folder to `motion-control-lab` ctrl + click `myWin826.cpp` and `RealTime.cpp` and click **Add**.
	- This doesn't copy anything on the drive, it just links the .cpp files to the project, so the compiler knows to compile them with everything else. 

### Part 2: Link s826.lib
- Right-click the project and choose Properties.
- At the top, set Configuration: All Configurations and Platform: Win32.
- Go to **Linker → General → Additional Library Directories** and add `$(SolutionDir)`. That's the folder with the `.sln` and `s826.lib`.
- Go to **Linker → Input → Additional Dependencies**, click the dropdown, then **Edit...**, and add `s826.lib` on its own line. Leave the existing entries alone.
- Click **OK**.

## Lab 3 Notes
### Example 1 (ADC):
1. Will read in the following values:
	- `-9 V` 
	- `-1.5 V`
	- `2.5 V`
	- `6 V`
2. Start by making sure power supply and multimeter are set to `ADC0` on the Sensoray (Pin 4 = 	`+AD0`, Pin 3 = `-AD0`)
3. Set power supply to `-9 VDC`. Execute adc.exe
4. Write down the digital value. 
5. Change power supply to `-1.5 V` & hit `Spacebar` to read the value. 
6. Repeat for `2.5 V` and `6 V`
7. Modify the gain in the `S826_AdcSlotConfigWrite` function. 
	- Set the `ADC_GAIN` to `S826_ADC_GAIN_2 ` which `= 1`. That sets the Sensoray to +/- 5 volt range. 
	- Update the `ADC_VRANGE` to `10`
8. Rerun read in for `-9`, `-1.5`, `2.5`, and `6`

### Example 2 (DAC):
1. Go into code and ensure the setpoint prints out properly
	 - The "Setpoint" is the voltage out REQUESTED. Not necessarily the actual DAC out. 
	


