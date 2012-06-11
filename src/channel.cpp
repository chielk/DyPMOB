#include "channel.h"
#include <vector>
#include <iostream>

Channel::Channel(int num_agents, float p)
{
	for (int i = 0; i < num_agents; ++i) {
		agents.push_back(Agent(i, num_agents,  p));
		throughput.push_back(0);
	}
}

/**
 * Uses an int like an array of booleans to represent if agents have a message
 * to send.
 */
	int
Channel::_state_to_int()
{
	vector<Agent>::iterator agent;
	int state = 0;
	for (agent = agents.begin(); agent < agents.end(); agent++) {
		state |= agent->has_msg();
	}
	return state;
}

	void
Channel::run(int turns)
{
	int sender_id;
	int total_sent;
	int feedback = 0;

	vector<Agent>::iterator agent;
	vector<int>::iterator thru;
	for(int i = turns; i > 0; i--) {
		sender_id = 0;
		total_sent = 0;
		agent = agents.begin();
		thru = throughput.begin();
		// count sending agents
		for (; agent < agents.end(); agent++) {
			thru++;
			if (agent->act() == SEND) {
				sender_id = agent->get_id();
				total_sent++;
				if (total_sent > 1) {
					feedback = COLLISION;
					break;
				}
			}
		}
		// set feedback
		if (feedback != COLLISION) {
			if (total_sent == 1) {
				feedback = SUCCESS;
				throughput[sender_id]++;
			} else {
				feedback = EMPTY;
			}
		}
		// send feedback to agents
		for (agent=agents.begin(); agent < agents.end(); agent++) {
			agent->receive_feedback(feedback);
		}
	}
}

	vector<int>
Channel::get_throughput()
{
	return throughput;
}
