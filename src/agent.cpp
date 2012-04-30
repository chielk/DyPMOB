#include <limits>
#include <cstdlib>
#include "agent.h"
#include "channel.h"

const int MAX_INT = std::numeric_limits<int>::max();

inline bool random_b(float p)
{
	return p * MAX_INT > rand();
}

Agent::Agent(int agent_id, float prob)
{
	p = prob;
	refresh();
	done = false;
	iteration = 0;
	id = agent_id;
}

bool Agent::refresh()
{
	if (done)
		throw ALREADY_DONE;
	if (!msg)
		msg = random_b(p);
	done = false;
	return msg;
}

int Agent::send()
{
	action = SEND;
	refresh();
	iteration++;
	return action;
}

int Agent::wait()
{
	action = WAIT;
	refresh();
	iteration++;
	return action;
}

int Agent::act()
{
	if (iteration % id == 0) {
		return send();
	} else {
		return wait();
	}
}
