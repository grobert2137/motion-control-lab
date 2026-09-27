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

