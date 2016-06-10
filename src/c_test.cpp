#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>	/* cout */
#include <signal.h>	/* raise */
int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc < 3) {
		// Tell the user how to run the program
		std::cerr << "Usage: " << argv[0] << " num1 num2" << std::endl;
		/* "Usage messages" are a conventional way of telling the user
		 * how to run a program if they enter the command incorrectly.
		 */
		return 1;
	}

	int i = atoi(argv[1]);
	int j = atoi(argv[2]);

	int iSecret, iGuess;

	/* initialize random seed: */
	srand (time(NULL));

	/* generate secret number between 1 and 10: */
	iSecret = rand() % 10 + 1;

	if (iSecret < 3){
		raise(SIGSEGV);
	}
	// Print the user's name:
	std::cout << iSecret + i*j << "\n";
	
	return iSecret + i*j ;
}
