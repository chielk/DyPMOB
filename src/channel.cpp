#include "channel.h"
#include <vector>

Channel::Channel(int num_agents, float p)
{
	for (int i = 0; i < num_agents; i++) {
		agents.push_back(Agent(i, p));
	}
}
