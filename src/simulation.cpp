//#include "config.h"
#include "agent.h"
#include "channel.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


void print_throughput(const vector<int> throughput)
{
	int total_throughput = 0;
	cout << "Throughput: [";
	vector<int>::const_iterator thru = throughput.begin();
	for (; thru < throughput.end()-1; thru++) {
		cout << *thru << ", ";
		total_throughput += *thru;
	}
	total_throughput += *thru;
	cout << *thru << "]\t(" << total_throughput << ")" << endl;
}


int main(int argc, char **argv)
{
	bool quiet=false;
	bool seedflg = false;
	int seed = 0;
	int repeat = 100;
	int n_agents = 2;
	float p_msg = 0.5;

	// Get command-line options
	char c;
	int errflg = 0;
	while ((c = getopt(argc, argv, ":qs:r:n:p:")) != -1) {
		switch (c) {
			case 'p':
				p_msg = atof(optarg);
				if (p_msg > 1.0 || p_msg < 0.0) {
					cerr << "Error: p should be between 0 and 1" << endl;
					errflg++;
				}
				break;
			case 'n':
				n_agents = atoi(optarg);
				break;
			case 'r':
				repeat = atoi(optarg);
				break;
			case 's':
				seed = atoi(optarg);
				seedflg = true;
				break;
			case 'q':
				quiet = true;
				break;
			case ':':       /* -s, -n, -p or -r without operand */
				cerr << "Option -" << char(optopt) << " requires an operand" << endl;
				errflg++;
				break;
			case '?':
				cerr << "Unrecognised option: -" << char(optopt) << endl;
				errflg++;
		}
	}
	if (errflg) {
		cout << "Usage: " << argv[0] << \
			" [-q] [-s <seed>] [-r <repeat>] [-n <agents>] [-p <P>]" << endl;
		exit(2);
	}

	// Initialise random generator
	if (!seedflg) {
		seed = time(0);
		if (!quiet)
			cout << "Seed:\t" << seed << endl;
	}
	srand(seed);

	// Start simulator
	Channel channel = Channel(n_agents, p_msg);

	// Run simulation
	channel.run(repeat);

#if 0
	for (int i = repeat; i > 0; i--) {
		channel.run_turn();
	}
#endif

	print_throughput(channel.get_throughput());
	return 0;
}
