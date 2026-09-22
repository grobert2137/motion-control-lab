#include <iostream>
int main()
{
	int num1 = 5, num2 = 10;
	int* pnum = NULL;
	pnum = &num1;
	*pnum += 10;
	pnum = &num2;
	num1 = *pnum;
}