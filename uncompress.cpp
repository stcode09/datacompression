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

        // create a file buffer to the input file
        std::filebuf rBuf;

        //create a huffman tree
        HCTree codeTree;

        //if we can open the input file with the file buffer
        if (rBuf.open(rFile, std::ios::in | std::ios::binary))
        {
            //connect to the input file
            std::istream rStream(&rBuf);

            //create a vectors of ints to store the frequency of
            //bytes that occurred in the original uncompressed file
            //(bytes and frequencies will be obtained from the input file
            //header section)
            std::vector<long> freqs(256);

            //build the Huffman code tree
            codeTree.build2(freqs, rStream);

            // create a 2nd file buffer for the output file
            std::filebuf wBuf;

            //now try and open the output file
            if (wBuf.open(wFile, std::ios::out | std::ios::binary))
            {
                //connect to the output file
                std::ostream wStream(&wBuf);

                //uncompress the input file into the output file
                codeTree.decompress(wStream, rStream);

                //close the file buffer for the output file
                wBuf.close();
            }
            else
                //notify user that the file couldn't be opened and thus uncompression failed
                std::cerr << "Error. " << wFile << " couldn't be opened.\nUncompression of " << rFile << " failed." << std::endl;

            //close the input file buffer
            rBuf.close();

        }
        else
            // notify user that the file couldn't be opened
            std::cerr << "Error. " << rFile << " couldn't be opened. Uncompression failed." << std::endl;

    }

    return 0;
}
