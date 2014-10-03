#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>
#include <bitset>
#include <cmath>

using namespace std;

/** A class for reading bits (and ints) from an istream
 */
class BitInputStream {
private:
    istream& in;  // the istream to delegate to
    char buf;     // the buffer of bits
    int bufi;     // the bit buffer index

public:
    /** Initialize a BitInputStream object, given an istream.
    */
    BitInputStream(istream& s) : in(s), buf(0), bufi(0) { }

    /** Read the next bit from the bit buffer.
     *  If the bit buffer is currently empty,
     *  fill the bit buffer by reading a char from the istream first.
     *  Return the bit read as the least signficant bit of an int.
     *  Return -1 on EOF.
     *  This must be consistent with BitOutputStream::writeBit(), in terms
     *  of ordering of bits in the stream.
     */
    int readBit();

    /** Read a byte from the istream.
     *  Return -1 on EOF.
     *  This function doesn't touch the bit buffer.
     *  The client has to manage interaction between reading bits
     *  and reading bytes.
     */
    int readByte();

    /** Read a non-negative long from the istream.
     *  Return -1 on EOF.
     *  This function doesn't touch the bit buffer.
     *  The client has to manage interaction between reading bits
     *  and reading ints.
     */
    long readLong();

    /** If the bit buffer is empty, fill it with the next 8 bits
     */
    void fillBuf();
};

#endif // BITINPUTSTREAM_HPP
