#include "HCNode.hpp"

//destructor
HCNode::~HCNode()
{
    //if we aren't at the root
    if (this->getP() != nullptr)
    {
        //remove the parent's corrent child pointer
        if (this->getP()->getC0() == this)
            this->getP()->setC0(nullptr);
        else
            this->getP()->setC1(nullptr);
    }
}

/** implementation of operator <
 */
bool HCNode::operator<(const HCNode& other)
{
    // if counts are different, just compare counts
    if (this->count != other.count)
        return this->count < other.count;

    // counts are equal. use symbol value to break tie.
    // (for this to work, internal HCNodes must have symb set.)
    return this->symbol < other.symbol;
}

/** implementation of function to return value of byte
 */
const byte HCNode::getValue()
{
    //return the byte value
    return this->symbol;
}

/** implementation of function to set value of byte
 */
void HCNode::setValue(byte& sym )
{
    //set the byte value
    this->symbol = sym;
}

/** implementation of function to return value of count
 */
const long HCNode::getCount()
{
    //return the count of this node
    return this->count;
}

/** implementation of function to set value of count
 */
void HCNode::setCount(long c )
{
    //set the count of this node
    this->count = c;
}

/** function to set value of 0 child
 */
void HCNode::setC0(HCNode * const c0)
{
    //set the 0 child pointer
    this->c0 = c0;
}

/** function to set value of 1 child
 */
void HCNode::setC1(HCNode * const c1)
{
    //set the 1 child pointer
    this->c1 = c1;
}

/** function to set value of parent node
 */
void HCNode::setP(HCNode * const p)
{
    //set the parent point
    this->p = p;
}

/** function to return pointer to 0 child
 */
HCNode* HCNode::getC0() const
{
    //return the 0 child pointer
    return this->c0;
}

/** function to return pointer to 0 child
 */
HCNode* HCNode::getC1() const
{
    //return the 1 child pointer
    return this->c1;
}

/** function to return pointer to 0 child
 */
HCNode* HCNode::getP() const
{
    //return the parent pointer
    return this->p;
}
