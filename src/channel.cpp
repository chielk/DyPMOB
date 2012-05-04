#include "channel.h"
#include <vector>
#include <iostream>

Channel::Channel(int num_agents, float p)
{
	for (int i = 0; i < num_agents; i++) {
		agents.push_back(Agent(i, p));
		throughput.push_back(0);
	}
}

int Channel::run_turn()
{
	int feedback;
	int sender_id = 0;
	int total_sent = 0;

	vector<Agent>::iterator agent = agents.begin();
	vector<int>::iterator thru = throughput.begin();
	for (; agent < agents.end(); agent++) {
		thru++;
		if (agent->act() == SEND) {
			sender_id = agent->get_id();
			total_sent++;
		}
	}
	switch (total_sent) {
		case 0:
			feedback = EMPTY;
			break;
		case 1:
			feedback = SUCCESS;
			throughput[sender_id]++;
			break;
		default:
			feedback = COLLISION;
	}
	for (agent=agents.begin(); agent < agents.end(); agent++) {
		agent->receive_feedback(feedback);
	}
	return feedback;
}

vector<int> Channel::get_throughput()
{
	return throughput;
}
