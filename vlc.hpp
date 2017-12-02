#ifndef MY_VLC_INCLUDED
#define MY_VLC_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <fstream>


namespace vlc {

typedef unsigned char byte;

/// array of bits, buf may be longer than sufficient
struct chunk {
private:
    std::vector<vlc::byte> buf= {0};
    size_t size_bits=0;
public:
    size_t get_size_bits() const {return size_bits; }
    void put_bit(bool newbit) {
        buf[size_bits/8] |= newbit << (7-size_bits%8);
        size_bits++;
        if (size_bits%8==0) {
            buf.push_back(0);
        }
    }
    bool get_bit(size_t index) const {
        vlc::byte a = buf[index/8];
        a <<= index%8;
        return a&128;
    }

    void put_chunk(const chunk& c) {
        if (c.size_bits==0) return;
        for (size_t i=0;i<c.size_bits;i++) {
            put_bit(c.get_bit(i));
        }
    }

    void numericprint(std::ostream& out) const {
        for (int i=0; i<=size_bits/8; i++) {
            out << int(buf[i]) << " ";
        }
    }

    void debugprint(std::ostream &out, bool perbyte=false) const {
        if (size_bits==0) return;
        for (int i=0; i<=(size_bits-1)/8; i++) {
            vlc::byte a = buf.at(i);
            for (int j=0;j<std::min(8,signed(size_bits)-i*8);j++) {

//                out << "(" <<i << " " << j << " " << int(a) << ")";
                out << !!int(a & 128);
                a <<=1;
            }
            if (perbyte) out<<" ";
        }

    }
    void debugput(std::string s) {
        for (char a : s) {
            put_bit(a=='1');
        }
    }
    void save(std::ofstream& out) {
        if (size_bits==0) return;
        out.write((char *)&(buf[0]),size_bits/8+1);
    }

    void save_file(std::string fname) {
        std::ofstream f(fname, std::ios::binary);
        save(f);
    }
};

std::ostream& operator<<(std::ostream &out, chunk c) {
    c.debugprint(out);
    return out;
}



class VLC {
    public:
    VLC() {}

};

}

#endif
