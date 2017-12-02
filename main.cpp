#include <iostream>
#include <vector>
#include "vlc.hpp"
using namespace std;


int main() {
	VLC vlc;
	vector<unsigned char> v(10,42);
	for (unsigned char a : v) {
		cout << int(a) << " ";
	}
}