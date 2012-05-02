#ifndef CHANNEL_H
#define CHANNEL_H

#include "agent.h"
#include <vector>

static const int WAIT = 0;
static const int SEND = 1;

static const int SUCCESS	= 0;
static const int EMPTY		= 1;
static const int COLLISION = 2;

using namespace std;

class Channel {
	public:
		Channel(int num_agents, float p);
		int run_turn();
		int get_throughput();
	private:
		vector<Agent> agents;
		int throughput;
};

#endif
