#include <iostream>
#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

#define DEBUG_LINE std::cout << __LINE__ << std::endl;

   template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
   std::vector<T> result;
   result.reserve(a.size());

   std::transform(a.begin(), a.end(), b.begin(),
         std::back_inserter(result), std::plus<T>());
   return result;
}


typedef std::vector<int> R; // reward vector

/**
 * Returns all states that can follow given a certain state and action
 *
 * Example:
 * state:			0b 011101 (agents 0, 2, 3, 4 have messages)
 * action:			0b 000100 (agent 2 sends)
 * least_state:	0b 011001
 *
 * FIXME: Could be more efficient
 */
	std::set<int>
get_transitions(int action, int state, int max_state)
{
	std::set<int> states;
	int least_state = state ^ action;
	int s;
	for (s = 0; s < max_state; s++) {
		states.insert(least_state | s);
	}
	return states;
}

	std::set<int>
get_actions(int state, int n_agents)
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

	inline int
_action2index(int action)
{
	return (int) log2(action) - 1;
}

	inline int
_index2action(int index)
{
	return (index + 1);
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
vector_compare(R a, R b)
{
	int a_count = 0;
	int b_count = 0;
	R::iterator ia;
	R::iterator ib;

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
		Q(R default_value, int n_agents);
		void add(int action, R value);
		void add_all(int action, std::set<R> value);
		void print();
		std::set<R> get_V();


	private:
		void insert(int action, R value);
		std::vector<std::set<R> > actions;
		std::set<R> V;
};

Q::Q(R default_value, int n_agents)
{
	for (int i = 0; i < n_agents; i++) {
		std::set<R> rewards;
		rewards.insert(default_value);
		actions.push_back(rewards);
	}
}

	std::set<R>
Q::get_V()
{
	return V;
}

	void
print_set(std::set<R> s, bool t=false)
{
	std::set<R>::iterator r;
	R::const_iterator i_it;
	for (r = s.begin(); r != s.end(); r++) {
		if (t)
			std::cout << "\t[";
		else
			std::cout << "[";
		for (i_it = r->begin(); i_it != r->end(); i_it++) {
			std::cout << *i_it;
			if (i_it != r->end()-1)
				std::cout << ",";
		}
		std::cout << "]\n";
	}
}
	void
Q::print()
{
	std::cout << "===========================" << std::endl;
	std::set<R>::iterator r;
	R::const_iterator i_it;
	int action_n = 0;
	std::cout << "Value: ";
	print_set(V);
	std::cout << std::endl;

	std::vector<std::set<R> >::iterator a;
	for (a = actions.begin(); a != actions.end(); a++) {
		std::cout << "action " << action_n++ << std::endl; // which action
		std::cout << "\t{" << std::endl;
#if 0
		for (r = a->begin(); r != a->end(); r++) {
			std::cout << "\t[";
			for (i_it = r->begin(); i_it != r->end(); i_it++) {
				std::cout << *i_it;
				if (i_it != r->end()-1)
					std::cout << ",";
			}
			std::cout << "]\n";
		}
#else
		print_set(*a, true);
#endif
		std::cout << "\t}" << std::endl;
	}
}

	inline void
Q::insert(int action, R value)
{
	actions[_action2index(action)].insert(value);
}

	void
Q::add(int action, R value)
{
	int a = _action2index(action);
	std::set<R>::iterator it;
	for (it = actions[a].begin(); it != actions[a].end(); ++it) {
		switch (vector_compare(value, *it)) {
			case 0: // same vector already in key
				return;
			case 1: // new vector is better; try to remove more before adding
				actions[a].erase(it);
				break;
			case -1: // better vector already in key
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	actions[a].insert(value); // no vectors were better; add
	return;
}

	void
Q::add_all(int action, std::set<R> values)
{
	for (std::set<R>::iterator it = values.begin(); it != values.end(); it++) {
		R r = *it;
		r[_action2index(action)] += 1;
		add(action, r);
	}
}

using namespace std;

	int
main(int argc, char const *argv[])
{
	int time = 2;
	int n_agents = 2;
	int n_states = 2 << (n_agents - 1);

	R defval = R(n_agents, 0);

	cout << "creating states... ";
	int t, s;
	vector<vector<Q> > q(time, vector<Q>(n_states, Q(defval, n_agents)));
	cout << "done" << endl;

	for (t = 0; t < time; t++) {
		for (s = 0; s < n_states; s++) {
			set<int> actions = get_actions(s, n_agents);
			set<int>::iterator a_it;
			for (a_it = actions.begin(); a_it != actions.end(); a_it++) {
				set<R> rewards;
				set<int> transitions = get_transitions(*a_it, s, n_states);
				set<int>::iterator tr_it;
				for (tr_it = transitions.begin(); tr_it != transitions.end(); tr_it++) {
					if (t > 0) {
						q[s][t].add_all(*a_it, q[t-1][*tr_it].get_V());
					} else {
						/*
						R r = R(n_agents, 0);
						DEBUG_LINE
						r[_action2index(*a_it)] = 1;
						DEBUG_LINE
						//q[s][t].add(*a_it, r);
						*/
					}
				}
				//q[t][s].add(a_it, transitions);
				//cout << endl;
			}
		}
	}

	for (t = 0; t < time; t++) {
		for (s = 0; s < n_states; s++) {
			q[s][t].print();
		}
	}
	return 0;
}
