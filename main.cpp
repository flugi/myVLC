#include <iostream>
#include <vector>
#include <sstream>
#include "vlc.hpp"
using namespace std;

string randomchunk() {
    string s;
    int h = rand()%40;
    for (int i=0;i<h;i++) {
        s+=rand()%2?'0':'1';

    }
    return s;
}

void chunk_basic_test() {
    for (int t=0;t<30;t++) {
        string s = randomchunk();
        vlc::chunk c;
        c.debugput(s);
        stringstream ss;
        ss << c;
        string s2;
        for (int i=0;i<c.get_size_bits();i++) {
            s2 += c.get_bit(i)?'1':'0';
        }
        if (ss.str() == s && s == s2) {
            cout << "OK ";
        } else {
            cout << s << " -> ";
            cout << "ERROR" << endl;
            c.debugprint(cout, 1);
            cout << endl;
            c.numericprint(cout);
            cout << endl << s2 << endl;

        }
    }
    cout << endl;
}

void chunk_concat_test() {
    for (int t=0;t<30;t++) {
        string s1=randomchunk(), s2=randomchunk();
        vlc::chunk c1, c2;
        c1.debugput(s1);
        c2.debugput(s2);
        c1.put_chunk(c2);
        stringstream ss;
        c1.debugprint(ss);
        if (ss.str() == s1+s2) {
            cout << "OK ";
        } else {
            cout << "ERROR: " << endl;
            cout << s1 << " " << s2 << endl ;
            cout << ss.str() << endl;

        }
    }
    cout << endl;
}

void chunk_file_test() {
    string s = "010000010100001001000011010001000111"; //ABCD..
    vlc::chunk c;
    c.debugput(s);
    c.save_file("_test");
    ifstream f("_test");
    string sb="    ";
    f.read((char *)&(sb[0]),4);
    if (sb == "ABCD") {
        cout << "OK" << endl;
    } else {
        cout << "ERROR" << endl;
    }
}

int main() {
    chunk_basic_test();
    chunk_concat_test();
    chunk_file_test();
}
