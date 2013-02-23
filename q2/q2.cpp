#include <iostream>
#include <vector>
#include <math.h>

using std::vector;
using std::cin;
using std::cout;

bool isWhole(double d) {
	return d == (long) d;
}

int main() {
	long n;
	cin >> n;
	vector<long> cs;
	for (long c=3, i=0; i<n; ++c) {
		int occurrences = 0;
		long c2 = c * c;
		for (long a=1; a<c; ++a) {
			long a2 = a * a;
			if (isWhole(sqrt(c2 - a2))) {
				++occurrences;
			}
		}
		if (occurrences > 0) {
			++i;
//			i += occurrences / 2;
			if (occurrences > 2) {
				cs.push_back(c);
			}
		}
	}
	cout << '\n';
	for (int i=0; i<cs.size(); ++i) {
		cout << cs[i] << '\n';
	}
}
