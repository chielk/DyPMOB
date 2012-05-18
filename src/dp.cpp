#include <iostream>
#include <map>
#include <list>
#include <algorithm>

#define EE (0)
#define ME (1)
#define EM (2)
#define MM (3)


/**
 * lists should be sorted and of the same length.
 *
 *	Return values:
 * Identical:	 0
 * a > b:		 1
 * b > a:		-1
 * Pareto:		 2
 */

	int
list_compare(std::list<int> a, std::list<int> b)
{
	int a_count = 0;
	int b_count = 0;
	std::list<int>::iterator ia;
	std::list<int>::iterator ib;

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
		Q(std::list<int> v);
		void set_default(std::list<int> v);
		std::multimap<int, std::list<int> >::iterator operator[](const int key);
		void insert(const int key, const std::list<int> value);
		void add(const int key, std::list<int> value);
	private:
		std::multimap<int, std::list<int> > m;
		std::list<int> default_v;

};

Q::Q(std::list<int> v=NULL)
{
	default_v = v;
}

	void
Q::set_default(std::list<int> v)
{
	default_v = v;
}

	inline void
Q::insert(const int key, const std::list<int> value)
{
	m.insert(std::pair<int, std::list<int> >(key, value));
}

	inline std::multimap<int, std::list<int> >::iterator
Q::operator[](const int key)
{
	return m.find(key);
}

	void
Q::add(const int key, std::list<int> value)
{
	std::multimap<int, std::list<int> >::iterator it = m.find(key);
	value.sort();
	bool done = false;
	for (; it != m.end() && !done; it++) {
		switch (list_compare(value, it->second)) {
			case 0: // same list already in key
				return;
			case 1: // new list is better; try to remove more before adding
				m.erase(it);
				break;
			case -1: // better list already in key
				return;
			case 2: // pareto-equal: add later if not case 0 or 2
				break;
		}
	}
	insert(key, value); // no lists were better; add
	return;
}


using namespace std;
int main(int argc, char const *argv[])
{
	list<int> defval;
	list<int> r1;
	list<int> r2;
	list<int> r3;
	list<int> r4;
	list<int> r5;

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

	Q q = Q(defval);
	q.add(EE, r1);
	q.add(EE, r2);
	q.add(EE, r3);
	q.add(EE, r4);

	for (list<int>::iterator it=q[EE]->second.begin(); it!=q[EE]->second.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;

	return 0;
}
