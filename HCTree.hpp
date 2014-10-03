#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <iomanip>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const
    {
        return !(*lhs < *rhs);
    }
};

/** A Huffman Code Tree class.
 *  Not very generic: Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    std::vector<HCNode*> leaves;

public:
    explicit HCTree() : root(0)
    {
        leaves = std::vector<HCNode*>(256, (HCNode*) 0);
    }

    /** default destructor
     */
    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
     *  the frequency of occurrence of byte i in the message and
     *  s points to an uncompressed file.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(std::vector<long>& freqs, std::istream& rStream);

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
     *  the frequency of occurrence of byte i in the message and
     *  s points to a compressed file
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     *  The number of bytes from the original uncompressed file is returned
     */
    void build2(std::vector<long>& freqs, std::istream& rStream);

    /** Use the Huffman tree to create the output file.
     *  PRECONDITION: build has been ran to create a Huffman tree.
     *  POSTCONDITION: the output file specified in the 2nd input
     *  argument contains the header and compressed huffman code
     *  representing the data from the file in the first input argument
     */
    void compress(std::ostream& wStream, std::istream& rStream);

    /** Use the Huffman tree to create the output file.
     *  PRECONDITION: build has been ran to create a Huffman tree.
     *  POSTCONDITION: the output file specified in the 2nd input
     *  argument contains the data of the original file whose
     *  compression information was contained in the input file
     */
    void decompress(std::ostream& wStream, std::istream& rStream);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leave, s vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Populate the freqs vector with the frequency of each
     *  byte value encountered in the file to be compressed
     *  PRECONDITION: in points to an uncompressed file and build
     *  has been called
     *  POSTCONDITION: freqs[i] contains the count of byte i
     */
    void charCount(std::vector<long>& freqs, BitInputStream& in);

    /** Populate the freqs vector with the frequency of each
     *  byte value encountered in the file to be compressed
     *  PRECONDITION: in points to a compressed file and build2
     *  has been called
     *  POSTCONDITION: freqs[i] contains the count of byte i and
     *  the number of unique bytes in the original uncompressed file
     *  is returned
     */
    void charCount2(std::vector<long>& freqs, BitInputStream& in);

    /** Function to set the root to point at an HCNode
     */
    void setRoot(HCNode* const root);

    /** Recursive function to cycle through the Huffman tree
     *  leaf to root and write huffman code to file in root to leaf order
     */
    void leafToRoot(HCNode* const ptr, BitOutputStream& out) const;

    /** Function to cycle through the Huffman tree from root to leaf
     *  and return the byte represented by the huffman code
     */
    int rootToLeaf(BitInputStream& in) const;

    /** Recursive function to cycle through the Huffman tree
     *  leaf to root and output huffman code in root to leaf order
     */
    void traverseToScreen(HCNode* const ptr) const;

    /** Function to count the number of non-zero leaves in the tree
     */
    int leafCount();

    /** Function to print byte value, it's count and it's Huffman code for debugging
     */
    void printHuffman(std::vector<long>& freqs);

    /** Function to delete the entire huffman tree
     *  PRECONDITION: build has been called and a huffman tree exists
     *  POSTCONDITION: all nodes of the tree have been delete and root
     *  points to nothing
     */
    void deleteTree(HCNode* root);
};

#endif // HCTREE_HPP
