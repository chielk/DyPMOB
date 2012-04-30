#ifndef CHANNEL_H
#define CHANNEL_H

#include "agent.h"
#include <vector>

using namespace std;

class Channel {
	public:
		Channel(int num_agents, float p);
	private:
		vector<Agent> agents;
};

#endif
