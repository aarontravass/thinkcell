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
            m_map.insert(std::make_pair(keyBegin, val));
            m_map.insert(std::make_pair(keyEnd, m_valBegin));
        }
        else {
            // let's keep track of the previous iterator
            auto previousPointer = m_map.find(keyBegin) != m_map.end() ? lowerBound : std::next(lowerBound, -1);
            // now our `previous` points to either the same pointer or the previous pointer

            // our last pointer is the last seen pointer
            auto lastPointer = previousPointer;
            // we copy the value since we will be using it later
            V lastPointerValue = lastPointer->second;



            // since map is a binary search tree, we just insert it and let the stl handle it
            auto insertionResult = m_map.insert(std::make_pair(keyBegin, val));

            // we handle an edge case of the key already existing.
            // for this, we already have the pointer in `previousPointer` and we delete the old key
            if(!insertionResult.second){
                m_map.erase(keyBegin);
                insertionResult = m_map.insert(std::make_pair(keyBegin, val));
            }
            auto itr = insertionResult.first;
            // we start processing the next pointer
            itr++;
            auto nextPointer = itr;

            // we start from next pointer, keeping track of the last iterator we visited and the next pointer
            // loop till keyEnd and end of map
            while((itr->first < keyEnd) && itr != m_map.end()){

                nextPointer = std::next(itr, 1);
                // erase the key, val pair since it's been overwritten
                // which is the same as merging overlapping intervals
                m_map.erase(itr->first);
                // assign current pointer to lastPointer
                lastPointer = itr;
                lastPointerValue = lastPointer->second;
                // assign the old iterator to the new iterator
                itr = nextPointer;
            }

            // finally, insert the last key
            m_map.insert(
                        std::make_pair(
                            keyEnd,
                            nextPointer == m_map.end() ? m_valBegin : lastPointerValue
                            // we use the lastPointerValue if keyEnd is within some range
                            // other we insert m_ValBegin
                        )
                        );

        }

	}

	void print(){
	    cout<<"==============="<<endl;
        for(auto itr = m_map.begin();itr != m_map.end();itr++){
            cout<<(itr->first)<<" "<<itr->second<<endl;
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

void print(interval_map<int, int> &inmap){
    cout<<"========================"<<endl;
    for(int i = -5;i<12;i++){
        cout<<i<<" "<<inmap[i]<<endl;
    }
    cout<<"========================"<<endl;
}

int main()
{

//first test case
interval_map<int, char> fooh { 'z' };
fooh.assign(2,5,'a');
fooh.print();
std::cout << fooh[6] << std::endl << std::endl;

//second test case
// expected : z  b  z
fooh = interval_map<int, char>{'z'};
fooh.assign(1,4,'b');
cout << fooh[0] << " " << fooh[1] << " " << fooh[5] << endl;

//third test case
// expected: A
fooh = interval_map<int, char>{'z'};
fooh.assign(1,6,'A');
fooh.assign(2,4,'B');
cout << fooh[5] << endl;
fooh.print();


//forth test case
fooh = interval_map<int, char>{'z'};
//expected [0,'a'],[1,'z']
fooh.assign(0,1,'a');
fooh.print();


//fifth test case
// expected [0,'f']
fooh = interval_map<int, char>{'z'};
fooh.assign(1,2,'c');
fooh.assign(2,3,'d');
fooh.assign(3,4,'e');
fooh.assign(4,15,'g');
fooh.assign(0,10,'f');
fooh.print();
cout << endl;


//sixth test case
// expected: 0,'d'  2,'c'
fooh = interval_map<int, char>{'z'};
fooh.assign(1,4,'c');
fooh.assign(0,2,'d');
fooh.print();
cout << endl;



    return 0;
}
