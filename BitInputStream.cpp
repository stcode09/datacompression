#include "BitInputStream.hpp"

/** Implementation of readBit
 */
int BitInputStream::readBit()
{
    //if buffer is 0, flush the buffer before writing the next bit
    if (this->bufi == 0)
        this->fillBuf();

    //set the correct bit in the mask to 1
    unsigned char mask = pow(2,(this->bufi -  1));

    //and the mask with the buffer to get the bit value
    int bit = this->buf & mask;

    //shift the bit value to the least significant bit position
    bit >>= (this->bufi - 1);

    //decrement the bit buffer index
    this->bufi--;

    //return the result
    return bit;
}

/** Implementation of readByte
 */
int BitInputStream::readByte()
{
    //if EOF, return -1, else return the next
    //byte from the stream
    if (this->in.eof())
        return -1;
    else
    {
        int c = this->in.get();
        return c;
    }

}

/** Implementation of readLong
 */
long BitInputStream::readLong()
{
    //if EOF, return -1, else return the next
    //4 bytes from the stream
    if (this->in.eof())
        return -1;
    else
    {
        long l;
        this->in.read(reinterpret_cast<char*>(&l),sizeof(long));
        return l;
    }
}

/** If the bit buffer contains any bits, flush the bit buffer to the ostream,
 *  clear the bit buffer, and set the bit buffer index to 0.
 */
void BitInputStream::fillBuf()
{
    //if the buffer is totally full, write the byte
    //to the output file
    if (this->bufi == 0)
    {
        //write the buffer to the output file
        this->in.get(buf);

        //reinitialize the buffer and buffer index
        this->bufi = 8;
    }
}
