#ifndef AGENT_H
#define AGENT_H


// Returns a random boolean. True with probability p.
inline bool random_b(float p=0.5);

class Agent {
	public:
		Agent(int agent_id, int total, float p);
		int action;	// Last action performed by agent
		int get_id();
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
		int n;		// Total number of agents
		int id;
		float p;		// probability of getting a new message
		bool msg;
};

#endif
