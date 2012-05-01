#ifndef AGENT_H
#define AGENT_H

#include "config.h"

static const int ALREADY_DONE = 0;

// Returns a random boolean. True with probability p.
inline bool random_b(float p=0.5);

class Agent {
	public:
		Agent(int agent_id, float p);
		int action;	// Last action performed by agent
		void receive_feedback(int feedback);
		int default_policy();
		inline int act()
		{
#if POLICY == DEFAULT_POLICY
			return default_policy();
#endif
		}
	private:
		int send();
		int wait();
		bool refresh();
		int iteration;
		int id;
		float p;		// probability of getting a new message
		bool done;	// has perfomed action
		bool msg;
};

#endif
