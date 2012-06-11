#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#define EE (0)
#define ME (1)
#define EM (2)
#define MM (3)


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
vector_compare(std::vector<int> a, std::vector<int> b)
{
	int a_count = 0;
	int b_count = 0;
	std::vector<int>::iterator ia;
	std::vector<int>::iterator ib;

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
		Q(int time, int num_agents, std::vector<int> v);
		void vector_default(std::vector<int> v);
		void insert(int time, int state, const std::vector<int> value);
		std::vector<std::set<std::vector<int> > > operator[](const int key);
		void add(int time, int state, std::vector<int> value);

	private:
		std::vector<std::vector<std::set<std::vector<int> > > > v;
		std::vector<int> default_v;
};

Q::Q(int time, int num_agents, std::vector<int> v ) :\
		v(time, std::vector<std::set<std::vector<int> > >(2 << num_agents))
{
	default_v = v;
}

	std::vector<std::set<std::vector<int> > >
Q::operator[](const int key)
{
	return v[key];
}

	void
Q::vector_default(std::vector<int> v)
{
	default_v = v;
}

	inline void
Q::insert(int time, int state, const std::vector<int> value)
{
	v[time][state].insert(value);
}

	void
Q::add(int time, int state, std::vector<int> value)
{
	std::set<std::vector<int> >::iterator it;
	std::set<std::vector<int> > current = v[time][state];

	bool done = false;
	for (it = current.begin(); it != current.end() && !done; it++) {
		switch (vector_compare(value, *it)) {
			case 0: // same vector already in key
				return;
			case 1: // new vector is better; try to remove more before adding
				current.erase(it);
				break;
			case -1: // better vector already in key
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	insert(time, state, value); // no vectors were better; add
	return;
}


using namespace std;
int main(int argc, char const *argv[])
{
	int time = 10;
	vector<int> defval;
	vector<int> r1;
	vector<int> r2;
	vector<int> r3;
	vector<int> r4;
	vector<int> r5;

	defval.push_back(0);
	defval.push_back(0);

	r1.push_back(1);
	r1.push_back(1);

	r2.push_back(10);
	r2.push_back(2);

	r3.push_back(3);
	r3.push_back(3);

	r4.push_back(10);
	r4.push_back(10);

	Q q = Q(10, 2, defval);
	q.add(0, EE, r1);
	q.add(0, EE, r2);
	q.add(1, EE, r3);
	q.add(1, EE, r4);

	set<vector<int> >::iterator it;
	for (int i = 0; i < time; i++) {
		for (int j = 0; j < 2 << 2; j++) {
			for (it=q[i][j].begin(); it != q[i][j].end(); it++) {
				cout << "[";
				cout << (*it)[0];
				cout << "]";
			}
			cout << "\t";
		}
		cout << endl;
	}
	return 0;
}
