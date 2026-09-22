#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h> // for pow()
#include "../matlab_export.h"

int main() {
	double r = 5000.0; // ohms
	double c = 0.0001; // farad
	double VDC = 0.5; // Volts
	double Vc = 0.0; // initialize to 0
	double t = 0.0; // starting time step
	double dt = 0.5; // time step

	MatlabExport out("RCCCircut_ouput.csv"); // create MatlabExport object

	while (t <= 5.0) {
		printf("\n%lf", Vc);
		out.write({ t, Vc }); // needed for Matlab export 
		Vc = (dt / (r * c)) * (VDC - Vc) + Vc;
		t += dt;
	}
	return 0;
}

/* Wrong stuff from before. Ty Sibinski helped to use while loop which is a lot easier here. 
double sysOut(double t, double r, double c, double VDC, double* k_prev) {
	double ans = (t / (r * c)) * VDC - (1 - (t / (r * c))) * (*k_prev);
	return ans;
}

int main() {

		for (int i = 0; i <= Tf; i + T) {
			prevOutput = sysOut(i, R, C, V, &prevOutput);
			printf("\n%lf", prevOutput);
		}
		return 0;
}
*/