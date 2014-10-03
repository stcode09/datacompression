#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

/** implementation of default destructor
 */
HCTree::~HCTree()
{
    //delete the entire tree
    deleteTree(this->root);
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message and
 *  s points to a uncompressed file.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(std::vector<long>& freqs, istream& rStream)
{
    //create a bit input stream from the passed in istream
    BitInputStream in(rStream);

    //calculate the frequency of characters in the stream
    this->charCount(freqs, in);

    //create a priority queue of HCNodes and create nodes for
    //all the bytes found in the file
    std::priority_queue<HCNode*,std::vector<HCNode*>,HCNodePtrComp> pq;
    for (int i = 0; i < 256; i++ )
    {
        //if the current byte has a positive frequency
        if (freqs[i] != 0)
        {
            //create a node for it and push the node onto the queue
            HCNode* h = new HCNode(freqs[i],i);
            this->leaves[i] = h;
            pq.push(this->leaves[i]);
            //release heap memory
            h = nullptr;
            delete h;
        }
    }

    //while there is more than one node/tree in the queue or
    //there is just one node in the queue and it isn't the root node
    while (pq.size() > 1 || pq.top()->getValue())
    {
        //create a new empty node
        HCNode* temp1 = new HCNode(0);

        //set the new node's 0 child
        temp1->setC0(pq.top());
        //remove the 0 child from the queue
        pq.pop();
        //set the 0 child's parent
        temp1->getC0()->setP(temp1);
        //set the parent node's count to be the child's count
        temp1->setCount(temp1->getC0()->getCount());

        //if there isn't just one node in the queue
        if (!pq.empty())
        {
            //set the new node's 1 child
            temp1->setC1(pq.top());
            //remove the 1 child from the queue
            pq.pop();
            //set the 1 child's parent
            temp1->getC1()->setP(temp1);

            //set the parent node's count to be the sum of the count of it's children
            temp1->setCount(temp1->getCount()+temp1->getC1()->getCount());
        }

        //push the new node onto the queue
        pq.push(temp1);

        //memory clean up
        temp1 = nullptr;
        delete temp1;
    }

    //if the queue isn't empty
    if (!pq.empty())
    {
        //now that the tree is constructed, set the HCTree object's root and
        //pop it from the queue
        this->setRoot(pq.top());
        pq.pop();
    }
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message and
 *  s points to a compressed file
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 *  The number of bytes from the original uncompressed file is returned
 */
void HCTree::build2(std::vector<long>& freqs, std::istream& rStream)
{
    //create a bit input stream from the passed in istream
    BitInputStream in(rStream);

    //determine the number of unique bytes, what they are and
    //their frequencies in the original uncompressed file from
    //the header of the compressed file
    this->charCount2(freqs, in);

    //create a priority queue of HCNodes and create nodes for
    //all the bytes found in the file
    std::priority_queue<HCNode*,std::vector<HCNode*>,HCNodePtrComp> pq;
    for (int i = 0; i < 256; i++ )
    {
        //if the current byte has a positive frequency
        if (freqs[i] != 0)
        {
            //create a node for it and push the node onto the queue
            HCNode* h = new HCNode(freqs[i],i);
            this->leaves[i] = h;
            pq.push(this->leaves[i]);
            //release heap memory
            h = nullptr;
            delete h;
        }
    }

    //if the queue isn't empty
    if (!pq.empty())
    {
        //while there is more than one node/tree in the queue or
        //there is just one node in the queue and it isn't the root node
        while (pq.size() > 1 || pq.top()->getValue())
        {
            //create a new empty node
            HCNode* temp1 = new HCNode(0);

            //set the new node's 0 child
            temp1->setC0(pq.top());
            //remove the 0 child from the queue
            pq.pop();
            //set the 0 child's parent
            temp1->getC0()->setP(temp1);
            //set the parent node's count to be the child's count
            temp1->setCount(temp1->getC0()->getCount());

            //if there isn't just one node in the queue
            if (!pq.empty())
            {
                //set the new node's 1 child
                temp1->setC1(pq.top());
                //remove the 1 child from the queue
                pq.pop();
                //set the 1 child's parent
                temp1->getC1()->setP(temp1);

                //set the new node's count to be the sum of the count of it's children
                temp1->setCount(temp1->getCount()+temp1->getC1()->getCount());
            }
            //push the new node onto the queue
            pq.push(temp1);

            //memory clean up
            temp1 = nullptr;
            delete temp1;
        }

        //now that the tree is constructed, set the HCTree object's root and
        //pop it from the queue
        this->setRoot(pq.top());
        pq.pop();
    }
}

/** Use the Huffman tree to create the output file.
 *  PRECONDITION: build has been ran to create a Huffman tree.
 *  POSTCONDITION: the output file specified in the 2nd input
 *  argument contains the header and compressed huffman code
 *  representing the data from the file in the first input argument
 */
void HCTree::compress(std::ostream& wStream, std::istream& rStream)
{
    //variable to hold the number of unique bytes in the input file
    //if all 256 bytes are used, we need to store it as 255 because 256
    //doesn't fit in a single byte
    int uniqueBytes = (this->leafCount() - 1);

    //create an output stream object
    BitOutputStream out(wStream);

    //if the input file wasn't empty, write compressed code to output file
    if (uniqueBytes >= 0)
    {
        //write the number of unique bytes to the output file
        out.writeByte(uniqueBytes);

        //cycle through leaves and write the non-null values to the output file
        for (int i = 0; i < 256; i++)
        {
            if (leaves[i] != nullptr)
                out.writeByte(leaves[i]->getValue());
        }

        //cycle through leaves and write the non-null frequencies to the output file
        for (int i = 0; i < 256; i++)
        {
            if (leaves[i] != nullptr)
                out.writeLong(leaves[i]->getCount());
        }

        //now need to write the huffman code translation of the input file to the output file
        BitInputStream in(rStream);

        //read in the first byte
        int i = in.readByte();

        //while not eof
        while (i != -1)
        {
            //convert the int to a byte
            byte b = i;

            //write the compressed verion of the current byte to the output file
            encode(b,out);

            //read in the next byte
            i = in.readByte();
        }

        //flush the output buffer one last time to write any remaining bits to the output file
        out.flush();
    }
}

/** Use the Huffman tree to create the output file.
 *  PRECONDITION: build has been ran to create a Huffman tree.
 *  POSTCONDITION: the output file specified in the 2nd input
 *  argument contains the data of the original file whose
 *  compression information was contained in the input file
 */
void HCTree::decompress(std::ostream& wStream, std::istream& rStream)
{
    //variable to hold total number of bytes in original uncompressed file
    long totalBytes = 0;

    //cycle through leaves to determine number of bytes in input file
    for (int i = 0; i < 256; i++)
    {
        if (leaves[i] != nullptr)
            totalBytes += leaves[i]->getCount();
    }

    //if the uncompressed input file wasn't empty, write uncompressed code to output file
    if (totalBytes != 0)
    {
        //create streams for the input file and the output file
        BitInputStream in(rStream);
        BitOutputStream out(wStream);

        //while there are still bytes to be decoded/written
        while (totalBytes > 0)
        {
            //decode the next byte from the input stream and
            //write it to the output file
            out.writeByte(this->decode(in));

            //decrement the totalBytes remaining
            totalBytes--;
        }
    }
}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const
{
    //create a temporary pointer to the current symbol
    HCNode* const ptr = leaves[symbol];

    //write the Huffman code to the output file
    leafToRoot(ptr, out);
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const
{
    //call helper function to find the symbol
    //encoded by the current place in the bit stream
    return rootToLeaf(in);
}

/** Recursive function to cycle through the Huffman tree
 *  leaf to root and write huffman code to file in root to leaf order
 */
void HCTree::leafToRoot(HCNode* const ptr, BitOutputStream& out) const
{
    //if we are at the root return to the caller
    if(ptr == this->root)
       return;

    //otherwise, continue to the parent node
    leafToRoot(ptr->getP(), out);

    //write the bit type of the current node
    //(1 is it is a 1 child otherwise 0)
    out.writeBit(ptr->getP()->getC1() == ptr);
}

/** Function to cycle through the Huffman tree from root to leaf
 *  and return the byte represented by the huffman code
 */
int HCTree::rootToLeaf(BitInputStream& in) const
{
    //create a temporary pointer pointing to the root of the huffman tree
    HCNode* ptr = this->root;

    //obtain the first bit to determine which child to go to
    int i = in.readBit();

    //travel to the first correct child node
    if (i == 1)
        ptr = ptr->getC1();
    else
        ptr = ptr->getC0();

    //while we aren't at a leaf node
    while (ptr->getC0() != nullptr && ptr->getC1() != nullptr)
    {
        //determine the next bit falue
        i = in.readBit();

         //travel to the next correct child node
        if (i == 1)
            ptr = ptr->getC1();
        else
            ptr = ptr->getC0();
    }
    int j = ptr->getValue();

    //return the value of the leaf node
    return j;
}

/** Recursive function to cycle through the Huffman tree
 *  leaf to root and output huffman code in root to leaf order
 */
void HCTree::traverseToScreen(HCNode* const ptr) const
{
    //if we are at the root return to the caller
    if(ptr == this->root)
       return;

    //otherwise, continue to the parent node
    traverseToScreen(ptr->getP());

    //write the bit type of the current node
    //(1 is it is a 1 child otherwise 0)
    std::cout << (ptr->getP()->getC1() == ptr);
}

/** Populate the freqs vector with the frequency of each
 *  byte value encountered in the file to be compressed
 *  PRECONDITION: in points to an uncompressed file and build
 *  has been called
 *  POSTCONDITION: freqs[i] contains the count of byte i
 */
void HCTree::charCount(std::vector<long>& freqs, BitInputStream& in)
{
    //read in the first byte
    int i = in.readByte();

    //while not eof
    while (i != -1)
    {
        //increment the count
        freqs[i]++;
        //read in the next byte
        i = in.readByte();
    }
}

/** Populate the freqs vector with the frequency of each
 *  byte value encountered in the file to be compressed
 *  PRECONDITION: in points to a compressed file and build2
 *  has been called
 *  POSTCONDITION: freqs[i] contains the count of byte i and
 *  the number of unique bytes in the original uncompressed file
 *  is returned
 */
void HCTree::charCount2(std::vector<long>& freqs, BitInputStream& in)
{
    //read in the first byte and add one to the value
    //value is stored as 1 less than actual so 256 fits
    //in a single byte
    int byteCount = (in.readByte() + 1);

    //if the file isn't empty, count the characters
    if (byteCount != -1)
    {
        //create a vector to hold the unique bytes in order
        std::vector<byte> chars(byteCount);

        //insert the unique bytes into chars
        for (int i = 0; i < byteCount; i++)
            chars[i] = in.readByte();

        //update freqs with the frequency of the chars from
        //the original uncompressed file
        for (int i = 0; i < byteCount; i++)
        {
            long c = in.readLong();
            freqs[chars[i]] = c;
        }
    }
}

/** Function to set the root to point at an HCNode
 */
void HCTree::setRoot(HCNode* const root)
{
    //set the root to the provided pointer value
    this->root = root;
}

/** Function to count the number of non-zero leaves in the tree
 */
int HCTree::leafCount()
{
    //variable to hold the number of unique bytes in the input file
    int uniqueBytes = 0;

    //cycle through leaves to determine number of bytes in input file
    for (int i = 0; i < 256; i++)
    {
        if (leaves[i] != nullptr)
            //totalBytes += leaves[i]->getCount();
            uniqueBytes += 1;
    }

    //return the byte count
    return uniqueBytes;
}

/** Function to print byte value, it's count and it's Huffman code for debugging
 */
void HCTree::printHuffman(std::vector<long>& freqs)
{
    //debug to output huffman tree
    for (int i = 0; i < 256; i++)
    {
        if (this->leaves[i] != nullptr)
        {
            std::cout << setw(10) << (long)this->leaves[i]->getValue();
            std::cout << " : " << setw(11) << freqs[i] << " : ";
            this->traverseToScreen(this->leaves[i]);
            std::cout << "\n";
        }
    }
}

/** Function to delete the entire huffman tree
 *  PRECONDITION: build has been called and a huffman tree exists
 *  POSTCONDITION: all nodes of the tree have been delete and root
 *  points to nothing
 */
void HCTree::deleteTree(HCNode* node)
{
    //if the node isn't already deleted
    if (node != nullptr)
    {
        //delete the 0 child
        deleteTree(node->getC0());
        //delete the 1 child
        deleteTree(node->getC1());
        //delete the node
        delete node;
    }

    //set the pointer to nothing
    node = nullptr;
}
