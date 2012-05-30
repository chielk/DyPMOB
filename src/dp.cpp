#include <iostream>
#include <map>
#include <set>
#include <algorithm>

#define EE (0)
#define ME (1)
#define EM (2)
#define MM (3)


/**
 * multisets should be of the same length.
 *
 *	Return values:
 * Identical:	 0
 * a > b:		 1
 * b > a:		-1
 * Pareto:		 2
 */

	int
multiset_compare(std::multiset<int> a, std::multiset<int> b)
{
	int a_count = 0;
	int b_count = 0;
	std::multiset<int>::iterator ia;
	std::multiset<int>::iterator ib;

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
		Q(std::multiset<int> v);
		void multiset_default(std::multiset<int> v);
		std::multimap<int, std::multiset<int> >::iterator operator[](const int key);
		void insert(const int key, const std::multiset<int> value);
		void add(const int key, std::multiset<int> value);
	private:
		std::multimap<int, std::multiset<int> > m;
		std::multiset<int> default_v;

};

Q::Q(std::multiset<int> v)
{
	default_v = v;
}

	void
Q::multiset_default(std::multiset<int> v)
{
	default_v = v;
}

	inline void
Q::insert(const int key, const std::multiset<int> value)
{
	m.insert(std::pair<int, std::multiset<int> >(key, value));
}

	inline std::multimap<int, std::multiset<int> >::iterator
Q::operator[](const int key)
{
	return m.find(key);
}

	void
Q::add(const int key, std::multiset<int> value)
{
	std::multimap<int, std::multiset<int> >::iterator it = m.find(key);
	bool done = false;
	for (; it != m.end() && !done; it++) {
		switch (multiset_compare(value, it->second)) {
			case 0: // same multiset already in key
				return;
			case 1: // new multiset is better; try to remove more before adding
				m.erase(it);
				break;
			case -1: // better multiset already in key
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	insert(key, value); // no multisets were better; add
	return;
}


using namespace std;
int main(int argc, char const *argv[])
{
	multiset<int> defval;
	multiset<int> r1;
	multiset<int> r2;
	multiset<int> r3;
	multiset<int> r4;
	multiset<int> r5;

	defval.insert(0);
	defval.insert(0);

	r1.insert(1);
	r1.insert(1);

	r2.insert(10);
	r2.insert(2);

	r3.insert(3);
	r3.insert(3);
	r3.insert(4);
	r3.insert(1);

	for (multiset<int>::iterator it = r3.begin(); it!=r3.end(); ++it) {
		cout << *it << " ";
	}

	r4.insert(10);
	r4.insert(10);

	Q q = Q(defval);
	q.add(EE, r1);
	q.add(EE, r2);
	q.add(EE, r3);
	q.add(EE, r4);

	for (multiset<int>::iterator it=q[EE]->second.begin(); it!=q[EE]->second.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;

	return 0;
}
