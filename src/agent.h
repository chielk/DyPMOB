#ifndef AGENT_H
#define AGENT_H

static const int ALREADY_DONE = 0;

static const int SEND = 0;
static const int WAIT = 1;

// Returns a random boolean. True with probability p.
inline bool random_b(float p);

class Agent {
	public:
		Agent(float p);
		int action;	// Last action performed by agent
		int send();
		int wait();
		bool refresh();
	private:
		int iteration;
		float p;		// probability of getting a new message
		bool done;	// has perfomed action
		bool msg;
};

#endif
