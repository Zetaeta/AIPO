#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int flatSize = 0;
vector<string> moves;

bool flat(const vector<int> &piles) {
	for (int i=0; i<piles.size(); ++i) {
		if (piles[i] != flatSize) {
			return false;
		}
	}
	return true;
}

int getNeighbourMinimum(vector<int> piles, int i, int *loc = 0) {
	if (i == 0) {
		if (loc) *loc = 1;
		return piles[1];
	}
	else if (i == piles.size() - 1) {
		if (loc) *loc = i - 1;
		return piles[i-1];
	}
	else {
		if (loc) {
			*loc = piles[i-1] < piles[i+1] ? i-1 : i+1;
			return piles[*loc];
		}
		return std::min(piles[i-1], piles[i+1]);
	}
}

int getNeighbourMaximum(vector<int> piles, int i, int *loc = 0) {
	if (i == 0) {
		if (loc) *loc = 1;
		return piles[1];
	}
	else if (i == piles.size() - 1) {
		if (loc) *loc = i-1;
		return piles[i-1];
	}
	else {
		if (loc) {
			*loc = piles[i-1] > piles[i+1] ? i-1 : i+1;
			return piles[*loc];
		}
		return std::max(piles[i-1], piles[i+1]);
	}
}

void move(vector<int> &piles, int i, int m) {
	ostringstream oss;
	oss << i << " " << m << '\n';
#ifdef DEBUG
	cout << i << " " << m << '\n';
#endif
	moves.push_back(oss.str());
	if (i == 0 || i == piles.size() - 1) {
		piles[i] -= m;
		piles[i == 0 ? 1 : i-1] += m;
	}
	else {
		piles[i] -= m * 2;
		piles[i + 1] += m;
		piles[i - 1] += m;
	}
#ifdef DEBUG
	for (int i=0; i<piles.size(); ++i) {
		cout << piles[i] << " ";
	}
	cout << '\n';
#endif
}

#define TO_FLAT
//#define GIVE_ALL

int canSpare(vector<int> &piles, int i) {
#ifdef GIVE_ALL
	if (i == 0 || i == piles.size()-1) return piles[i];
	return piles[i] / 2;
#else
#ifdef TO_FLAT
	if (i == 0 || i == piles.size()-1) return piles[i] - flatSize;
	return (piles[i] - flatSize) / 2 ;
#else
#error no canSpare function
#endif
#endif
}

int maxNeighbourCanGive(vector<int> piles, int i, int *loc = 0) {
	if (i == 0) {
		if (loc) *loc = 1;
		return canSpare(piles, 1);
	}
	else if (i == piles.size() - 1) {
		if (loc) *loc = i-1;
		return canSpare(piles, i-1);
	}
	else {
		if (loc) {
			*loc = canSpare(piles, i-1) > canSpare(piles, i+1) ? i-1 : i+1;
			return canSpare(piles, *loc);
		}
		return std::max(canSpare(piles, i-1), canSpare(piles, i+1));
	}
} 

void makeMoveFromMax(vector<int> &piles) {
	int max = 0;
	for (int i=1; i<piles.size(); ++i) {
		if (piles[i] > piles[max] &&
			getNeighbourMinimum(piles, i) < flatSize) {
			max = i;
		}
		else if (piles[i] == piles[max]) {
			int inm = getNeighbourMinimum(piles, i);
			int maxnm = getNeighbourMinimum(piles, max);
			if (inm < maxnm) {
				max = i;
			}
			else if (maxnm < inm) {
				continue;
			}
/*			else {
				int imm = getNeighbourMaximum(piles, i);
				int maxmm = getNeighbourMaximum(piles, max);
				if (imm < maxmm) {
					max = i;
				}
			}
*/
		}
	}
	int toMove = min(flatSize - getNeighbourMinimum(piles, max),
		piles[max] / 2);
	if (toMove <= 0) {
		return;
	}
	move(piles, max, toMove);
}

void makeMoveToMin(vector<int> &piles) {
	int min = 0;
	int from = -1;
	for (int i=1; i<piles.size(); ++i) {
		if (piles[i] < piles[min] &&
			piles[i] < flatSize &&
			maxNeighbourCanGive(piles, i) > 0) {
			min = i;
		}
		else if (piles[i] == piles[min] || maxNeighbourCanGive(piles, min) <= 0) {
			int ifrom;
			int minfrom;
			int imm = getNeighbourMaximum(piles, i, &ifrom);
			int minmm = getNeighbourMaximum(piles, min, &minfrom);
			if (imm > minmm && canSpare(piles, ifrom) > 0) {
				min = i;
				from = ifrom;
			}
			else if (minmm < imm) {
				from = minfrom;
				continue;
			}
			else {
				int inm = getNeighbourMinimum(piles, ifrom);
				int minnm = getNeighbourMinimum(piles, minfrom);
				if (inm < minnm && canSpare(piles, ifrom) > 0) {
					min = i;
					from = ifrom;
				}
				else {
					from = minfrom;
				}
			}
		}
	}
	if (from == -1) {
		maxNeighbourCanGive(piles, min, &from);
	}
	int toMove = std::min(flatSize - piles[min],
		canSpare(piles, from));
	if (toMove <= 0) {
		makeMoveFromMax(piles);
		return;
	}
	move(piles, from, toMove);
}

void makeMoveMostCanGive(vector<int> &piles) {
	int min = 0;
	int from = -1;
	for (int i=1; i<piles.size(); ++i) {
		if (maxNeighbourCanGive(piles, i) - piles[i] <
			maxNeighbourCanGive(piles, min) - piles[min] &&
			piles[i] < flatSize) {
			min = i;
		}
		else if (maxNeighbourCanGive(piles, i) - piles[i] == maxNeighbourCanGive(piles, min) -
				piles[min] || maxNeighbourCanGive(piles, min) <= 0) {
			int ifrom;
			int minfrom;
			getNeighbourMaximum(piles, i, &ifrom);
			getNeighbourMaximum(piles, min, &minfrom);

			int inm = getNeighbourMinimum(piles, ifrom);
			int minnm = getNeighbourMinimum(piles, minfrom);
			if (inm < minnm && canSpare(piles, ifrom) > 0) {
				min = i;
				from = ifrom;
			}
			else {
				from = minfrom;
			}
		}
	}
	if (from == -1) {
		maxNeighbourCanGive(piles, min, &from);
	}
	int toMove = std::min(flatSize - piles[min],
		canSpare(piles, from));
	if (toMove <= 0) {
		makeMoveFromMax(piles);
		return;
	}
	move(piles, from, toMove);
}

//#define TO_MIN
#define FROM_MAX
//#define GIVE_MOST

void makeMove(vector<int> &piles) {
#ifdef FROM_MAX
	makeMoveFromMax(piles);
#endif
#ifdef TO_MIN
	makeMoveToMin(piles);
#endif
#ifdef GIVE_MOST
	makeMoveMostCanGive(piles);
#endif
}

int main() {
	int n;
	ifstream in("flat.inp");
	in >> n;
	vector<int> piles(n);
	for (int i=0; i<n; ++i) {
		in >> piles[i];
		flatSize += piles[i];
	}
	in.close();
	flatSize /= n;
	while (!flat(piles)) {
		makeMove(piles);
	}
	ofstream of("flat.out");
	of << moves.size() << '\n';
	for (int i=0; i<moves.size(); ++i) {
		of << moves[i];
	}
	of.close();
}
