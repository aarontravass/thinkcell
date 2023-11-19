#include <iostream>
#include <map>

using namespace std;



template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
// INSERT YOUR SOLUTION HERE
        // If the range is negative, then we don't assign
        if(!(keyBegin < keyEnd)) return;
        // find the first pointer to keyBegin
        auto lowerBound = m_map.lower_bound(keyBegin);
        // base case
        // if keyBegin is absent, then we insert (keyBegin, V) and (keyEnd, mValBegin)
        // visual representation
        // -inf------(keyBegin)--------keyEnd------- +inf
        //    A      |   V         V   |   A      A
        if(lowerBound == m_map.end()) {
            m_map.insert(std::pair<K,V>(keyBegin, val));
            m_map.insert(std::pair<K,V>(keyEnd, m_valBegin));
        }
        else {
            // let's keep track of the previous iterator
            auto previousPointer = lowerBound->first == keyBegin ? lowerBound : std::next(lowerBound, -1);
            // now our `previous` points to either the same pointer or the previous pointer

            // our last pointer is the last seen pointer
            auto lastPointer = previousPointer;
            // `it` represents a key which satisfies this condition    key1<key<=key2
            // consider the case of
            // -inf------(1)--------(6)------- +inf
            //    A      |   B      |   A      A
            // if we want to insert (5) - (6) - C
            // we check if the value is same then we insert from 5 onwards and keep merging
            // consecutive intervals untill we reach keyEnd
            // since map is a binary search tree, we just insert it and let the stl handle it
            auto insertionResult = m_map.insert(std::pair<K,V>(keyBegin, val));
            auto itr = insertionResult.first;
            itr++;
            auto nextPointer = itr;
            // we start from lower bound, keeping track of the last iterator we visited
            // loop till keyEnd and end of map
            while((itr->first < keyEnd) && itr != m_map.end()){

                nextPointer = std::next(itr, 1);
                // erase the key, val pair since it's been overwritten
                // which is the same as merging overlapping intervals
                m_map.erase(itr->first);
                // assign current pointer to lastPointer
                lastPointer = itr;
                // assign the old iterator to the new iterator
                itr = nextPointer;
            }
            // finally, insert the last key
            if(nextPointer == m_map.end()) m_map.insert(std::pair<K,V>(keyEnd, m_valBegin));
            else m_map.insert(std::pair<K,V>(keyEnd, lastPointer->second));
        }

	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

void print(interval_map<char, int> &inmap){
    cout<<"========================"<<endl;
    for(int i = -5;i<12;i++){
        cout<<i<<" "<<inmap[i]<<endl;
    }
    cout<<"========================"<<endl;
}

int main()
{
    interval_map<char, int> inmap(1);
    /*
    inmap.assign(2,5,2);
    print(inmap);
    inmap.assign(3,6,3);
    print(inmap);
    */
    inmap.assign(1, 10, 4);
    print(inmap);
    inmap.assign(2, 5,5);
    print(inmap);
    //inmap.assign(8, 11, 7);
    //print(inmap);

    return 0;
}
