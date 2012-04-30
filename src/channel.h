#ifndef CHANNEL_H
#define CHANNEL_H

#include "agent.h"

class Channel {
	public:
		Channel(int num_agents);
	private:
		Agent* agents;
};

#endif
