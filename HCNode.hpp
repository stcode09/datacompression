#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

/** A class, instances of which are nodes in an HCTree.
 */
class HCNode {
    //friend comparison function to allow comparisons from outside the class
    friend bool comp(HCNode* one, HCNode* other){return *one < *other;};

private:
    long count;
    byte symbol; // byte in the file we're keeping track of
    HCNode* c0;  // pointer to '0' child
    HCNode* c1;  // pointer to '1' child
    HCNode* p;   // pointer to parent

public:
    // default constructor
    HCNode(long count, byte symbol, HCNode* c0 = 0, HCNode* c1 = 0, HCNode* p = 0):
        count(count), symbol(symbol), c0(c0), c1(c1), p(p) { };

    // constructor for non-leaf nodes
    HCNode(long count): count(count), symbol(0), c0(0), c1(0), p(0) { };

    //destructor
    virtual ~HCNode();

    /** Less-than comparison, so HCNodes will work in std::priority_queue
     **  We want small counts to have high priority.
     **  And we want to break ties deterministically.
     **/
    bool operator<(const HCNode& other);

    /** function to return value of byte
     */
    const byte getValue();

    /** function to set value of byte
     */
    void setValue(byte& sym );

    /** function to return value of count
     */
    const long getCount();

    /** function to set value of count
     */
    void setCount(long c );

    /** function to set value of 0 child
     */
    void setC0(HCNode * const c0);

    /** function to set value of 1 child
     */
    void setC1(HCNode * const c1);

    /** function to set value of parent node
     */
    void setP(HCNode * const p);

    /** function to return pointer to 0 child
     */
    HCNode* getC0() const;

    /** function to return pointer to 0 child
     */
    HCNode* getC1() const;

    /** function to return pointer to 0 child
     */
    HCNode* getP() const;

    /** implementation of public function to print out HCNode contents
     */
    virtual void printNode(std::ostream& stm) const
    {
         stm << "[" << this->count << "," << this->symbol << "]";
    };
};

/** For printing an HCNode to an ostream
 *  Possibly useful for debugging.
 */
std::ostream& operator<<(std::ostream&, const HCNode&) __attribute__((weak)); // shut the linker up
std::ostream& operator<<(std::ostream& stm, const HCNode& n)
{
    // call printNode function to prepare stream
    n.printNode(stm);
    // return stream
    return stm;
}

#endif // HCNODE_HPP
