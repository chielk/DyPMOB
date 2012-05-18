#include <limits>
#include <cstdlib>
#include "agent.h"
#include "channel.h"

inline bool random_b(float p)
{
	const static int MAX_INT = std::numeric_limits<int>::max();
	return p * MAX_INT > rand();
}

Agent::Agent(int agent_id, int total, float prob)
{
	n = total;
	p = prob;
	refresh();
	iteration = 1;
	id = agent_id;
}

	int
Agent::get_id()
{
	return id;
}

	bool
Agent::refresh()
{
	if (!msg)
		msg = random_b(p);
	return msg;
}

	int
Agent::send()
{
	if (!msg)
		return wait();
	msg = false;
	action = SEND;
	refresh();
	iteration++;
	return action;
}

	int
Agent::wait()
{
	action = WAIT;
	refresh();
	iteration++;
	return action;
}

	int
Agent::default_policy()
{
	if (iteration % n == id) {
		return send();
	} else {
		return wait();
	}
}

	void
Agent::receive_feedback(int feedback)
{
	if (action == SEND && feedback == COLLISION)
		msg = true;
}
