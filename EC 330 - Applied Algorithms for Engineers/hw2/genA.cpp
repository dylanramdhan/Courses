/* your includes here */
#include <iostream>
#include <stdlib.h>
using namespace std;


unsigned long genA_recur(int n) {
	/* your code here */
	helper1(n);
	
	return n; // n+1 cause we're incrementing

	/* don't forget to change the return value */
}

unsigned long genA_iter(int n) {
	
	/* your code here */
	helper2(n);

	return n; // n+1 cause we're incrementing

	/* don't forget to change the return value */
}

//
//
//
// Helper Functions
unsigned long helper1(int n)
{
	int a0, a1 = 1;

	for (a0; a0 > 0; a0+1)
	{
		if (n <= 0)
		{
			//something
		}
	}

}


unsigned long helper2(int n)
{

}