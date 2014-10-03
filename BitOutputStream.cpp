#include "BitOutputStream.hpp"

/** Write the least significant bit of the argument into
 *  the bit buffer, and increment the bit buffer index.
 *  Flush to the ostream first if the bit buffer is full.
 *  This must be consistent with BitInputStream::readBit().
 */
void BitOutputStream::writeBit(int bit)
{
    //if buffer is 0, flush the buffer before writing the next bit
    if (this->bufi == 0)
        this->flush();

    //if bit is 0, just decrement the buffer index because it is initialized
    //to all zeros and thus the value is already implicitly there
    if (bit == 0)
        this->bufi--;
    else
    {
        //set the correct bit in the mask to 1
        char mask = pow(2,(this->bufi -  1));

        //or the mask with the buffer to add the value to the buffer
        this->buf = this->buf | mask;

        //decrement the buffer index
        this->bufi--;
    }
}

/** Write the least significant byte of the argument to the ostream.
 *  This function doesn't touch the bit buffer.
 *  The client has to manage interaction between writing bits
 *  and writing bytes.
 */
void BitOutputStream::writeByte(int b)
{
    //create a mask to obscure the last three bytes in the int
    const char MASK = 255;

    //convert that int to a char
    char myByte = b & MASK;

    //write the char (1 byte) to the ostream
    this->out.put(myByte);
}

/** Write the argument to the ostream.
 *  This function doesn't touch the bit buffer.
 *  The client has to manage interaction between writing bits
 *  and writing ints.
 */
void BitOutputStream::writeLong(long l)
{
    //write a long to the ostream
    this->out.write(reinterpret_cast<const char*>(&l), sizeof(long));
}

/** If the bit buffer contains any bits, flush the bit buffer to the ostream,
 *  clear the bit buffer, and set the bit buffer index to 8.
 *  Also flush the ostream itself.
 */
void BitOutputStream::flush()
{
    //write the buffer to the output file
    this->out.put(buf);

    //reset the bit buffer and it's index
    this->bufi = 8;
    this->buf = 0;

    //flush the ostream
    out.flush();
}
