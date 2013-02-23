#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::cout;
using std::cin;
using std::map;

int countAverages(const vector<long> &seq) {
	map<int, int> numbers;
	for (int i=0; i<seq.size(); ++i) {
		numbers[seq[i]] = i;
	}
	int count = 0;
	for (int k=0; k<seq.size(); ++k) {
	  for (int i=0; i<seq.size(); ++i) {
	    int required = seq[k] * 2 - seq[i];
	    map<int, int>::iterator it = numbers.find(required);
	    if (it != numbers.end() && it->second != i) {
		++count;
		goto donek;
	    }
/*	    for (int j=0; j<seq.size(); ++j) {
		if (i == j) continue;
		if (seq[j] == avg) {
			++count;
			goto donek;
		}
	    }
*/
	  }
	  donek:
	  continue;
	}
	return count;
}

int main() {
	int size;
	cin >> size;
	vector<long> seq(size);
	for (int i=0; i<size; ++i) {
		long l;
		cin >> l;
		seq[i] = l;
	}
	cout << countAverages(seq) << '\n';
}
