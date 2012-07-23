#include <iostream>
#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <limits>

#define DEBUG_LINE std::cout << __LINE__ << std::endl << std::flush

int same = 0;
int removed_a = 0;
int removed_v = 0;

   template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
   std::vector<T> result;
   result.reserve(a.size());

   std::transform(a.begin(), a.end(), b.begin(),
         std::back_inserter(result), std::plus<T>());
   return result;
}


typedef std::vector<float> R; // reward vector

/**
 * "int state" is *after* action. i.e.
 * old_state:	0b 111
 * action:		0b 100
 * state:		0b 011
 */
	float
get_trans_p(int trans, int state, std::vector<float> pv){
	float p_total = 1.0;
	int mask = 1;
	// Iterate in reverse because the mask starts at 0b0*001
	std::vector<float>::reverse_iterator p;
	for (p = pv.rbegin(); p != pv.rend(); p++) {
		if (!(mask & state)) {
			if (mask & trans) {
				p_total *= 1.0 - *p;
			} else {
				p_total *= *p;
			}
		}
		mask <<= 1;
	}
	return p_total;
}

/**
 * Returns all states that can follow given a certain state and action
 *
 * Example:
 * state:			0b 011101 (agents 0, 2, 3, 4 have messages)
 * action:			0b 000100 (agent 2 sends)
 * least_state:	0b 011001
 *
 */
	std::set<std::pair<int, float> >
get_transitions(int action, int state, int n_states, std::vector<float> pv)
{
	static std::vector<std::set<std::pair<int, float> > > cache = \
		std::vector<std::set<std::pair<int, float> > >(n_states);

	std::set<std::pair<int, float> > states;
	int least_state = state ^ action;
	int s;
	if (!cache[least_state].empty()) {
		return cache[least_state];
	}
	for (s = 0; s < n_states; s++) {
		int new_state = least_state | s;
		states.insert(std::pair<int, float>(new_state,\
					get_trans_p(new_state, least_state, pv)));
	}
	cache[least_state] = states;
	return states;
}

	std::set<int>
get_actions(int state, int n_agents)
{
	std::set<int> actions;
	actions.insert(0); // Doing nothing is always possible
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
	if (!action)
		return 0;
	return (int) log2(action)+1;
}

	inline int
_index2action(int index)
{
	if (!index)
		return 0;
	return 1 << (index-1);
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
		//if (std::fabs(*ia - *ib) < std::numeric_limits<double>::epsilon()) {
		if (std::fabs(*ia - *ib) < 0.0000001) {
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

	if  (a_count)
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
		void add_all(int action, std::set<R> value, float p);
		void print(int t, int s);
		std::set<R> get_V();

	private:
		void add_V(R value);
		std::vector<std::set<R> > actions;
		std::set<R> V;
};

Q::Q(R default_value, int n_agents)
{
	for (int i = 0; i < n_agents+1; i++) {
		std::set<R> rewards;
		rewards.insert(default_value);
		actions.push_back(rewards);
	}
	V.insert(default_value);
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
Q::print(int t, int s)
{
	std::cout << "==================" << "(" << t << ", " << s << ")";
	std::cout << "==================" << std::endl;
	std::set<R>::iterator r;
	R::const_iterator i_it;
	int action_n = 0;
	std::cout << "Value: " << std::endl;
	print_set(V);
	std::cout << std::endl;

	std::vector<std::set<R> >::iterator a;
	for (a = actions.begin(); a != actions.end(); a++) {
		std::cout << "action " << _index2action(action_n++) << std::endl; // which action
		std::cout << "\t{" << std::endl;
		print_set(*a, true);
		std::cout << "\t}" << std::endl;
	}
}

/**
 * Adds sets of vectors
 * {a} + {b} + {c, d} = {a+b+c, a+b+d}
 * {a, b} + {c, d} = {a+c, a+d, b+c, b+d}
 */
	std::set<R>
add_sets(std::vector<std::set<R> > all) {
	std::set<R> result;
	std::vector<std::set<R> >::iterator at = all.begin();
	std::set<R>::iterator bt;
	std::set<R>::iterator vt;
	std::set<R> tmp;
	result = std::set<R>(*at);
	for (++at; at != all.end(); at++) {
		tmp.clear();
		// deep copy
		for (bt=result.begin(); bt != result.end(); bt++) {
			tmp.insert(R(*bt));
		}
		result.clear();
		for (vt=at->begin(); vt!= at->end(); vt++) {
			for (bt=tmp.begin(); bt != tmp.end(); bt++) {
				result.insert(*vt+*bt);
			}
		}
	}
	return result;
}

	void
Q::add(int action, R value)
{
	R cmp = R(value);
	cmp[1] = 99999999999;
	bool null_vec = (value[0] < 0.0000001 && value[1] < 0.0000001);
	int i = _action2index(action);
	static std::set<R>::iterator it;
	for (it = actions[i].begin(); it != actions[i].end(); it++) {
		switch (vector_compare(value, *it)) {
			case 0: // same vector already in key
				//std::cout << (*it)[0] << ", " << (*it)[1] << "\tsame" << std::endl;
				same++;
				return;
			case 1: // new vector is better; try to remove more before adding
				//std::cout << (*it)[0] << ", " << (*it)[1] << std::endl;
				actions[i].erase(it);
				break;
			case -1: // better vector already in key
				//std::cout << value[0] << ", " << value[1] << std::endl;
				if (!null_vec);
					removed_a++;
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	actions[i].insert(value); // no vectors were better; add
	add_V(value);
	return;
}

	void
Q::add_V(R value)
{
	bool null_vec = (value[0] < 0.0000001 && value[1] < 0.0000001);
	static std::set<R>::iterator it;
	for (it = V.begin(); it != V.end(); it++) {
		switch (vector_compare(value, *it)) {
			case 0: // same vector already in key
				same++;
				return;
			case 1: // new vector is better; try to remove more before adding
				if (!null_vec)
					removed_v++;
				V.erase(it);
				break;
			case -1: // better vector already in key
				if (!null_vec)
					removed_v++;
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	V.insert(value); // no vectors were better; add
	return;
}

	std::set<R>
multiply_all(std::set<R> values, float p)
{
	std::set<R> result;
	for (std::set<R>::iterator it = values.begin(); it != values.end(); it++) {
		R r = R(*it);
		for (int i = 0; i < r.size(); i++) {
			r[i] *= p;
		}
		result.insert(r);
	}
	return result;
}


using namespace std;

	int
main(int argc, char **argv)
{
	//====================================
	bool quiet=false;
	bool seedflg = false;
	int seed = 0;
	int time = 5;
	int n_agents = 2;
	vector<float> p_msg;

	// Get command-line options
	char c;
	int errflg = 0;
	while ((c = getopt(argc, argv, ":qs:r:n:")) != -1) {
		switch (c) {
			case 'n':
				n_agents = atoi(optarg);
				break;
			case 'r':
				time = atoi(optarg);
				break;
			case 's':
				seed = atoi(optarg);
				seedflg = true;
				break;
			case 'q':
				quiet = true;
				break;
			case ':':       /* -s, -n, -p or -r without operand */
				cerr << "Option -" << char(optopt) << " requires an operand" << endl;
				errflg++;
				break;
			case '?':
				cerr << "Unrecognised option: -" << char(optopt) << endl;
				errflg++;
		}
	}
	int index;
	for (index = optind; index < argc; index++) {
		float p = atof(argv[index]);
		if (p >= 0.0 && p <= 1.0) {
			p_msg.push_back(p);
		} else {
			cerr << "Probabilities should be between 0 and 1." << endl;
			exit(2);
		}
	}
	if (errflg) {
		cout << "Usage: " << argv[0] << \
			" [-q] [-s <seed>] [-r <repeat>] [-n <agents>] [<P> <P> ...]" << endl;
		exit(2);
	}
	//====================================

	int n_states = 2 << (n_agents - 1);

	R defval = R(n_agents, 0);

	cout << "creating states... ";
	int t, s;
	vector<vector<Q> > q(time, vector<Q>(n_states, Q(defval, n_agents)));
	cout << "done" << endl;

	for (t = 0; t < time; t++) {
		//cout << "\r\033[K" << flush;
		//cout << (100*t)/time << "%" << flush;
		cout << endl;
		for (s = 0; s < n_states; s++) {
			set<int> actions = get_actions(s, n_agents);
			set<int>::iterator a_it;
			for (a_it = actions.begin(); a_it != actions.end(); a_it++) {
				vector<set<R> > rewards;
				set<pair<int, float> > transitions = get_transitions(*a_it, s, n_states, p_msg);
				set<pair<int, float> >::iterator tr_it;
				same = 0;
				removed_a = 0;
				removed_v = 0;
				if (t > 0) {

					// calculate vector of sets p(s')*V(s')
					for (tr_it = transitions.begin(); tr_it != transitions.end(); tr_it++) {
						rewards.push_back(multiply_all(q[t-1][tr_it->first].get_V(), tr_it->second));
					}
					// each possible sum of rewards
					set<R> new_rewards = add_sets(rewards);
					set<R>::iterator rr;
					for (rr = new_rewards.begin(); rr != new_rewards.end(); rr++) {
						R r = R(*rr);
						// direct reward
						if (*a_it) {
							r[_action2index(*a_it)-1] += 1.0;
						}
						q[t][s].add(*a_it, r);
					}
				} else {
					if (*a_it) {
						R r = R(n_agents, 0.0);
						r[_action2index(*a_it)-1] = 1.0;
						q[t][s].add(*a_it, r);
					}
				}
				cout << "s,a=" << s << "," << *a_it << "\tsame: " << same << "\tremoved (a,v,t): " << removed_a << ", " << removed_v << ", " << removed_a+removed_v << endl;
			}
		}
	}
	cout << "\r\033[K" << flush;

	if (!quiet) {
		for (t = 0; t < time; t++) {
			for (s = 0; s < n_states; s++) {
				q[t][s].print(t, s);
			}
		}
	}
	cout << "Pareto front size: ";
	for (s = 0; s < n_states-1; s++) {
		cout << q[t-1][s].get_V().size() << ", ";
	}
	cout << q[t-1][s].get_V().size() << endl;
	return 0;
}
