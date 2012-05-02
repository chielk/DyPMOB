#include "channel.h"
#include <vector>
#include <iostream>

Channel::Channel(int num_agents, float p)
{
	for (int i = 0; i < num_agents; i++) {
		agents.push_back(Agent(i, p));
	}
	throughput = 0;
}

void Channel::run_turn()
{
	int feedback;
	int total_sent = 0;
	vector<Agent>::iterator it;
	for (it=agents.begin(); it < agents.end(); it++) {
		if (it->act() == SEND)
			total_sent++;
	}
	switch (total_sent) {
		case 0:
			feedback = EMPTY;
			cout << "EMPTY" << endl;
			break;
		case 1:
			feedback = SUCCESS;
			cout << "SUCCESS" << endl;
			throughput++;
			break;
		default:
			cout << "COLLISION" << endl;
			feedback = COLLISION;
	}
	for (it=agents.begin(); it < agents.end(); it++) {
		it->receive_feedback(feedback);
	}
}

int Channel::get_throughput()
{
	return throughput;
}
