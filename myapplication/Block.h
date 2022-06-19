
#include <list>
#include <string>
#include <map>

#ifndef BLOCK_H
#define BLOCK_H


//class block, which represents a single object in the chain (block-chain)
using namespace std;
class Block
{
public:
    int nonce;
    list<int> transactions;
    string previousBlockHash;
    string currentBlockHash;
};
#endif
