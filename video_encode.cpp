#include "vlc.hpp"

#include <iostream>
#include <fstream>
#include <map>
using namespace std;
using namespace vlc;


vector<unsigned short> load16bitraw(string fname, int w, int h) {
    vector<unsigned short> res;
    vector<unsigned short> frame(w*h);
    ifstream f(fname, ios::binary);
    f.read((char*)(&frame[0]),8); //header

    f.read((char*)(&frame[0]),w*h*2);
    while (f.good()) {
        for (unsigned short a : frame) {
            res.push_back(a);
        }
        f.read((char*)(&frame[0]),w*h*2);
    }
    return res;
}


int main(int argc, char*argv[]) {
    string fname;
    if (argc == 2) {
        fname = argv[1];
    } else {
        cerr << "batch mode not implemented yet";
    }
    map<int, int> hist;
    vector<unsigned short> v = load16bitraw(fname, 320, 100);
    vector<int> raw;
    for (int i=320*100;i<v.size();i++) {
        int sample = (int(v[i])-int(v[i-320*100]));
        raw.push_back(sample);
        hist[sample]++;
    }
    cout << raw.size() << " samples read, " << raw.size()*2 << " bytes input"<< endl;
    vlc::NumericSemiBlockCode<8> nbc;
    vlc::chunk c;
    nbc.encode(raw, c);
    cout <<"Semiblockcode: "<< c.get_size_bits()/8+1<< endl;
    ofstream f("hist.txt");
    for (pair<int, int> a : hist) {
        f <<a.first << " "<< a.second << endl;
    }

    HuffmanCode hc;
    hc.build(raw);
    chunk c2;
    hc.encode(raw,c2);
    cout <<"Huffman: " << c2.get_size_bits()/8+1<< endl;


    c.save_file("semiblock.data");
    c2.save_file("huffman.data");

    return 0;
}
