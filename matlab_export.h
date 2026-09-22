/*
matlab_export.h

Discription:
	Lightweight helper file used to extract C++ simulation data from terminal and export it as a csv.

Features:
	Header-only; No need for seperate .cpp file, just use #include 

MATLAB Side Usage:
	data = readmatrix("output.csv")
	t = data(:, 1);
	Vc = data(:, 2);
	plot(t, Vc);

*/

#pragma once // modern version of #ifndef #define. Supported by MSVC but not all compilers. 
#include <fstream> // File stream I/O gives classes for reading/writing files. 
#include <string> // C++ string type with built in functions like length() and + for concatenation
#include <initializer_list> // allows us to use out.write({ t, Vc}); which is a lightweight read-only container. whenever you pass a brace-enclosed list of vlaues to a function expecting one value, the loop inside the called function will just walk through each value in the braces. 

class MatlabExport {
public:
	// Constructor - this runs automtically when a MatlabExport object is created
	// "explicit" is a bug preventing thing. It means we have to explicitly call MatlabExport("data.csv") for it to construct the object.  
	// filename: output CSV file. header: optional first-row column names
	explicit MatlabExport(const std::string& filename, const std::string& header = "") {
		file.open(filename);
		if (!header.empty()) {
			file << header << "\n";
		}
	}


	// If MatlabExport is not called, and file is open, close the file. 
	~MatlabExport() {
		if (file.is_open()) {
			file.close();
		}
	}

	// Writes one comma-seperated row. Works for any number of values.
	// out.write({ t, Vc} }); or out.write({ t, Vc, current });
	void write(std::initializer_list<double> values) {
		bool first = true;
		for (double v : values) {
			if (!first) file << ",";
			file << v;
			first = false;
		}
		file << "\n";
	}

private:
	std::ofstream file;
};