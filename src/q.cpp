#include <iostream>
#include <set>
#include <vector>


typedef std::vector<int> V;

/**
 * Returns all states that can follow given a certain state and action
 *
 * Example:
 * state:			0b 011101 (agents 0, 2, 3, 4 have messages)
 * action:			0b 000100 (agent 2 sends)
 * least_state:	0b 011001
 *
 */
	std::set<int>
transitions(int action, int state, int max_state)
{
	std::set<int> states;
	int least_state = state ^ action;
	int s;
	for (s = 0; s <= max_state; s++) {
		states.insert(least_state | s);
	}
	return states;
}

	std::set<int>
actions(int state, int n_agents)
{
	std::set<int> actions;
	int a = 1;
	for (int n = 1; n <= n_agents; n++) {
		if (a & state)
			actions.insert(a);
		a <<= 1;
	}
	return actions;
}

/**
 * vectors should be of the same length.
 *
 *	Return values:
 * Identical:	 0
 * a > b:		 1
 * b > a:		-1
 * Pareto:		 2
 */

	int
vector_compare(V a, V b)
{
	int a_count = 0;
	int b_count = 0;
	V::iterator ia;
	V::iterator ib;

	for (ia=a.begin(), ib=b.begin(); ia!=a.end() && ib!=b.end(); ia++, ib++) {
		if (*ia == *ib) {
			continue;
		}
		if (*ia > *ib) {
			a_count++;
			if (b_count)
				return 2;	// 1 - 1
		} else {
			b_count++;
			if (a_count)
				return 2;	// 1 - 1
		}
	}

	if (a_count)
		 return 1;			// x - 0
	if (b_count)
		return -1;			// 0 - x
	return 0;				// 0 - 0
}


class Q
{
	public:
		Q(V default_value);
		void add(std::vector<int> value);
		void print_vs();

	private:
		void insert(V value);
		std::set<V> Vs;
};

Q::Q(V default_value)
{
	Vs.insert(default_value);
}

	void
Q::print_vs()
{
	std::set<V>::iterator it;
	V::const_iterator i_it;
	for (it = Vs.begin(); it != Vs.end(); it++) {
		std::cout << "[";
		for (i_it = it->begin(); i_it != it->end(); i_it++) {
			std::cout << *i_it << ",";
		}
		std::cout << "]\n";
	}
}

	inline void
Q::insert(V value)
{
	Vs.insert(value);
}

	void
Q::add(V value)
{
	std::set<V>::iterator it;
	for (it = Vs.begin(); it != Vs.end(); ++it) {
		switch (vector_compare(value, *it)) {
			case 0: // same vector already in key
				return;
			case 1: // new vector is better; try to remove more before adding
				Vs.erase(it);
				break;
			case -1: // better vector already in key
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	insert(value); // no vectors were better; add
	return;
}


using namespace std;
int main(int argc, char const *argv[])
{
	int time = 2;
	int n_agents = 2;
	int n_states = 2 << (n_agents - 1);

	V defval;
	for (int i = 0; i < n_agents; i++)
		defval.push_back(0);

	vector<vector<Q> > q(time, vector<Q>(n_states, Q(defval)));

	int t, s;
	for (t = 0; t < time; t++) {
		for (s = 0; t < n_states; s++) {
			q[t][s] = 
		}
	}

	return 0;
}
