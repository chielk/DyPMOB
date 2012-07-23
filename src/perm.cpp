#include <algorithm>
#include <functional>
#include <set>
#include <vector>
#include <iostream>

   template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
   std::vector<T> result;
   result.reserve(a.size());

   std::transform(a.begin(), a.end(), b.begin(),
         std::back_inserter(result), std::plus<T>());
   return result;
}

typedef std::vector<float> R;



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


	std::set<R>
add_sets(std::vector<std::set<R> > all) {
	std::vector<std::set<R> >::iterator at;
	std::vector<std::set<R> >::iterator bt;
	std::set<R>::iterator end;
	std::set<R>::iterator aat;
	std::set<R>::iterator bbt;
	std::set<R> result;
	for (at=all.begin(); at != all.end()-1; at++) { // first set
		for (bt=all.begin(),++bt; bt != all.end(); bt++) { // second set
			for (aat=at->begin(); aat != at->end(); aat++) { // first reward
				for (bbt=bt->begin(); bbt != bt->end(); bbt++) { // second reward
					result.insert(*aat+*bbt);
				}
			}
		}
	}
	return result;
}

using namespace std;
int main(int argc, char const *argv[])
{
#if 0
	R a;
	a.push_back(0);
	a.push_back(0.96);

	R b;
	b.push_back(0.64);
	b.push_back(0.8);

	R c;
	c.push_back(0);
	c.push_back(0.24);

	R d;
	d.push_back(.2);
	d.push_back(.2);

	R e;
	e.push_back(.36);
	e.push_back(0);

	set<R> s1;
	s1.insert(a);
	s1.insert(b);

	set<R> s2;
	s2.insert(c);
	s2.insert(d);
	s2.insert(e);

	vector<set<R> > s;
	s.push_back(s1);
	s.push_back(s2);
	set<R> r = add_sets(s);

#else
	float p = .5;
	R a;
	a.push_back(0);
	a.push_back(0.1);

	R b;
	b.push_back(1);
	b.push_back(0.8);

	set<R> s;
	s.insert(a);
	s.insert(b);

	set<R> r = multiply_all(s, p);
#endif

	std::set<R>::iterator it;
	R::const_iterator rit;
	for (it=r.begin(); it != r.end(); it++) {
		for (rit=it->begin(); rit != it->end(); rit++) {
			cout << *rit << "\t";
		}
		cout << endl;
	}
	return 0;
}
