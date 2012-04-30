#include "config.h"
#include "simulation.h"
#include "agent.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;


int main(int argc, char **argv)
{
	bool quiet = false;
	bool seedflg = false;
	int seed;

	// Get command-line options
	char c;
	int errflg = 0;
	while ((c = getopt(argc, argv, ":qs:")) != -1) {
        switch (c) {
        case 's':
            seed = atoi(optarg);
				seedflg = true;
            break;
        case 'q':
				quiet = true;
            break;
			case ':':       /* -s without operand */
				cerr << "Option -" << char(optopt) << " requires an operand" << endl;
				errflg++;
				break;
        case '?':
				cerr << "Unrecognised option: -" << char(optopt) << endl;
            errflg++;
        }
    }
    if (errflg) {
		  cout << "Usage: " << argv[0] << " [-q] [-s <seed>]" << endl;
        exit(2);
    }

	 // Initialise random generator
	 if (!seedflg) {
		seed = time(0);
		if (!quiet||seedflg)
			cout << "Seed:\t" << seed << endl;
	 }
	 srand(seed);

	 return 0;
}
