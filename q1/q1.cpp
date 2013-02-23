#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <stdlib.h>

using std::cin;
using std::cout;
using std::vector;
using std::string;

bool isIsbn(string isbn) {
	if (isbn[0] != '9' || isbn[1] != '7' ||
		(isbn[2] != '9' && isbn[2] != '8')) {
//		cout << "first 3 invalid!";
		return false;
	}
	int digits[13];
	int d=0;
	int g=0;
	for (int i=0; d<13; ++i) {
		if (isbn[i] == '-' || isbn[i] == ' ') {
			++g;
			continue;
		}
		if (!isdigit(isbn[i])) {
//			cout << isbn[i] << " not a digit";
			return false;
		}
		char dig[2] = { isbn[i], 0};
		digits[d] = atoi(dig);
//		cout << "digits[" << d << " = " << digits[d] << '\n';
		++d;
	}
	if (d != 13) {
//		cout << "d != 12";
		return false;
	}
	if (g != 4) {
		return false;
	}
	int sum = digits[0] + 3*digits[1] + digits[2] + 3*digits[3] +
		digits[4] + 3*digits[5] + digits[6] + 3*digits[7] +
		digits[8] + 3*digits[9] + digits[10] + 3*digits[11];
//	cout << "sum = " << sum << '\n';
//	cout << "checksum = " << (10 - sum % 10) % 10 << '\n';
	if (digits[12] != ((10 - sum % 10)) % 10) {
//		cout << "checksum = " << (10 - sum % 10) % 10 << '\n';
		return false;
	}
	return true;
}

int main() {
	string isbn;
	while(getline(cin, isbn)) {
		cout << (isIsbn(isbn) ? 1 : 0) << '\n';
	}
}
