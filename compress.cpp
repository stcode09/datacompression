#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char* argv[])
{
    //notify user if the right number of arguments weren't provided
    if (argc != 3)
        std::cout << "You need to provide 2 input arguments for this program." << std::endl;
    else
    {
        //set filenames to process from input argument
        string rFile = argv[1], wFile = argv[2];

        // create a file buffer for the input file
        std::filebuf rBuf;

        //create a huffman tree
        HCTree codeTree;

        // if we can open the input file with the file buffer
        if (rBuf.open(rFile, std::ios::in | std::ios::binary))
        {

            //connect to the input file
            std::istream rStream(&rBuf);

            //create a vectors of ints to store the frequencies of the bytes
            //in the input file
            std::vector<long> freqs(256);

            //build the Huffman tree from the input file
            codeTree.build(freqs, rStream);

            //close the input file buffer
            rBuf.close();
        }
        else
            // notify user that the input file couldn't be opened
            std::cerr << "Error. " << rFile << " couldn't be opened. Compression failed." << std::endl;

        // create a 2nd file buffer for the output file
        std::filebuf wBuf;

        //now open the output file to begin writing to it
        if (wBuf.open(wFile, std::ios::out | std::ios::binary))
        {
            //connect to the output file
            std::ostream wStream(&wBuf);

            //re-open the input file to begin compression
            if (rBuf.open(rFile, std::ios::in | std::ios::binary))
            {
                //connect to the input file
                std::istream rStream(&rBuf);

                //write the output file
                codeTree.compress(wStream, rStream);
            }

            //close the input file buffer
            rBuf.close();
        }
        else
            //notify user that the output file couldn't be opened
            std::cerr << "Error. " << wFile << " couldn't be opened. Compression failed." << std::endl;

        //close the output file buffer
        wBuf.close();
    }

    return 0;
}
