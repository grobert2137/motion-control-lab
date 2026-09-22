// iostream library is C++ only. Its only needed if you do std::cout or std::cin
// BUT iostream also drags in cstdio as a its own header internally.
#include <iostream>

// Explicitly declare cstdio for good practice when using printf() and scanf_s() <------------- 
#include <cstdio>

//Declare vars
float temp_c;
float temp_f;

float convert_to_f(float c) {
    float f = (9.0f / 5.0f) * c + 32;
    return f;
}

int main() {
    printf("\nHi! Please enter a temperature in degrees Celcius:");
    scanf_s("%f", &temp_c);
    temp_f = convert_to_f(temp_c);
    printf("\nTemperature in F: %f", temp_f);
}