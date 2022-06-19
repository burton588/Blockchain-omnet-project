//
// Copyright (C) 2009 Institut fuer Telematik, Universitaet Karlsruhe (TH)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

/**
 * @author Antonio Zea
 */

#include <string>
#include <map>
#include <iostream>
#include "common/UnderlayConfigurator.h"
#include "common/GlobalStatistics.h"
#include "common/GlobalNodeList.h"
#include "overlay/kademlia/KademliaBucket.h"
#include "applications/myapplication/TransactionsMsg_m.h"
#include "applications/myapplication/BlockMsg_m.h"
#include "common/GlobalNodeList.h"
#include "common/NodeVector.h"
#include "applications/myapplication/MyApplication.h"
#include "common/InitStages.h"
#include "common/CryptoModule.h"
#include "Block.h"
#include "sha256.h"

Define_Module(MyApplication);

// initializeApp() is called when the module is being created.
// Use this function instead of the constructor for initializing variables.
using namespace std;
void MyApplication::initializeApp(int stage)
{
    // initializeApp will be called twice, each with a different stage.
    // stage can be either MIN_STAGE_APP (this module is being created),
    // or MAX_STAGE_APP (all modules were created).
    // We only care about MIN_STAGE_APP here.

    if (stage != MIN_STAGE_APP) return;

    // copy the module parameter values to our own variables
    sendPeriod = par("sendPeriod");
    numToSend = par("numToSend");
    largestKey = par("largestKey");

    // initialize our statistics variables
    numSent = 0;
    numReceived = 0;

    // tell the GUI to display our variables
    WATCH(numSent);
    WATCH(numReceived);

    // start our timer!
    timerMsg = new cMessage("MyApplication Timer");
    scheduleAt(simTime() + SimTime::parse("10s"), timerMsg);

    blockChainBuildingTimerMsg = new cMessage("Block Chain Buiding Timer");


    bindToPort(8333);

}


// finish is called when the module is being destroyed
void MyApplication::finishApp()
{
    // finish() is usually used to save the module's statistics.
    // We'll use globalStatistics->addStdDev(), which will calculate min, max, mean and deviation values.
    // The first parameter is a name for the value, you can use any name you like (use a name you can find quickly!).
    // In the end, the simulator will mix together all values, from all nodes, with the same name.

    globalStatistics->addStdDev("MyApplication: Sent packets", numSent);
    globalStatistics->addStdDev("MyApplication: Received packets", numReceived);
}


// handleTimerEvent is called when a timer event triggers
void MyApplication::handleTimerEvent(cMessage* msg)
{
    // timer for generating transactions and flooding them to other nodes
    if (msg == timerMsg) {
        // reschedule our message
        cancelEvent(blockChainBuildingTimerMsg);
        scheduleAt(simTime() + SimTime::parse("10s"), blockChainBuildingTimerMsg);

        // if the simulator is still busy creating the network,
        // let's wait a bit longer
        if (underlayConfigurator->isInInitPhase()) return;


        //generate transactions, by generating 10 random numbers that simulate transactions
        list<int> transactions = {};

        for (int i = 0; i < 10; i++) {
            int data = rand() % 1000;
            transactions.push_back(data);
        }
        int id = rand ();
        int ttl =  2;

        //we save the transactions we didn't yet recieve, we know that by the id od the transaction
        vseTransakcije.insert({id, transactions});



        //here we write all transactions to output
        ///////////////////////////////////////////
        map<int,std::list<int>>::iterator it1;
        for(it1 = vseTransakcije.begin(); it1 != vseTransakcije.end(); it1++){
            list<int> transactions1 = it1->second;
            list<int>::iterator it2;
            for(it2 = transactions1.begin(); it2 != transactions1.end(); it2++){
                EV << to_string(*it2) << endl;
            }
        }
        ////////////////////////////////////////////



        EV << thisNode.getIp() << " + " << thisNode.getPort()  << std::endl;

        //send the transactions to all neighbor nodes
        NodeVector* nodes = callNeighborSet(5);
        for (auto i = nodes->begin(); i != nodes->end(); ++i){

            NodeHandle node = *i;
            if(thisNode.getIp() == node.getIp()){
                continue;
            }

            node.setPort(8333);
            EV << node.getIp() << " + " << node.getPort()  << std::endl;

            //generate transaction message
            TransactionsMsg *transactionsMsg; // the message we'll send
            transactionsMsg = new TransactionsMsg();
            transactionsMsg->setSenderAddress(thisNode); // set the sender address to our own
            transactionsMsg->setTransactions(transactions);
            transactionsMsg->setId(id);
            transactionsMsg->setTtl(ttl);
            transactionsMsg->setByteLength(100); // set the message length to 100 bytes

            //callRoute(node.getKey(), myMessage); // send it to the overlay
            sendMessageToUDP(node, transactionsMsg);
        }


    } //timer to generate a block and send it to other nodes
    else if (msg == blockChainBuildingTimerMsg){
        cancelEvent(timerMsg);
        scheduleAt(simTime() + SimTime::parse("10s"), timerMsg);

        //here we start to build blocks, e.c. mining for the correct hash value of the block and distributing to others
        Block *block;
        block = new Block();
        block->nonce = 0;
        block->transactions = allTransactionsTogether();
        block->previousBlockHash = previousHashValue();

        //"mining" for for 3 zero characters of sha256 string
        string currentBlockHash = "";
        while(currentBlockHash.substr(0,3) != "000"){
            block->nonce = rand();
            currentBlockHash = sha256(toString(block));
        }
        block->currentBlockHash = currentBlockHash;
        EV << "Blok: " << endl;
        string block_string = toString(block) + "\n" + "current block hash: " +  currentBlockHash;
        EV << block_string << endl;
        EV << sha256(toString(block)) << endl;


        //create and send message to neighbour nodes

        NodeVector* nodes = callNeighborSet(5);
        for (auto i = nodes->begin(); i != nodes->end(); ++i){

            NodeHandle node = *i;
            if(thisNode.getIp() == node.getIp()){
                continue;
            }

            node.setPort(8333);
            BlockMsg *blockMsg;
            blockMsg = new BlockMsg();
            blockMsg->setType(1);
            blockMsg->setSenderAddress(thisNode);
            blockMsg->setNonce(block->nonce);
            blockMsg->setTransactions(block->transactions);
            blockMsg->setPreviousBlockHash((block->previousBlockHash).c_str());
            blockMsg->setCurrentBlockHash(currentBlockHash.c_str());

            sendMessageToUDP(node, blockMsg);

        }

    }
    else {
        // unknown message types are discarded
        delete msg;
    }
}

// deliver() is called when we receive a message from the overlay.
// Unknown packets can be safely deleted here.
void MyApplication::deliver(OverlayKey& key, cMessage* msg)
{
    // we are only expecting messages of type MyMessage, throw away any other
    if(TransactionsMsg *transactionsMsg = dynamic_cast<TransactionsMsg*>(msg)){
            if (transactionsMsg == NULL) {
                delete msg; // type unknown!
                return;
            }

            auto transactions = transactionsMsg->getTransactions();

            list<int>::iterator it;
            for(it = transactions.begin(); it != transactions.end(); ++it){
                EV << *it << endl;
            }

            EV << thisNode.getIp() << ": Got packet from "
               << transactionsMsg->getSenderAddress() << std::endl;

                // send it back to its owner
             //   sendMessageToUDP(myMsg->getSenderAddress(), myMsg);

    }
    delete msg;

}


void MyApplication::handleUDPMessage(cMessage* msg)
{


    //if we receive message of type TransactionMsg
    if(TransactionsMsg *transactionsMsg = dynamic_cast<TransactionsMsg*>(msg)){

        auto transactions = transactionsMsg->getTransactions();

        int id = transactionsMsg->getId();
        int ttl = transactionsMsg->getTtl();
        ttl -= 1;
        TransportAddress senderAddress = transactionsMsg->getSenderAddress();


        //check if the transaction is already amids the received ones, if not, add the transaction and its id
        map<int,std::list<int>>::iterator it1;
        it1 = vseTransakcije.find(id);
        if (it1 == vseTransakcije.end()){
            vseTransakcije.insert({id, transactions});
        }


        //if ttl is greater than 0, than send the packet to neighbor nodes
        if (ttl > 0){
            NodeVector* nodes = callNeighborSet(5);
            for (auto i = nodes->begin(); i != nodes->end(); ++i){

                NodeHandle node = *i;

                if(thisNode.getIp() == node.getIp()){
                    continue;
                }
                if(node.getIp() == senderAddress.getIp()){
                    EV << "deluje pravilno" << endl;
                    continue;
                }

                node.setPort(8333);
                EV << node.getIp() << " + " << node.getPort()  << std::endl;

                //generate transactionMsg
                TransactionsMsg *transactionsMsg; // the message we'll send
                transactionsMsg = new TransactionsMsg();
                transactionsMsg->setSenderAddress(thisNode); // set the sender address to our own
                transactionsMsg->setTransactions(transactions);
                transactionsMsg->setId(id);
                transactionsMsg->setTtl(ttl);
                transactionsMsg->setByteLength(100); // set the message length to 100 bytes

                sendMessageToUDP(node, transactionsMsg);

            }
        }
    } //if message is of type BlockMsg
    else if(BlockMsg *blockMsg = dynamic_cast<BlockMsg*>(msg)){
        int msgType = blockMsg->getType();
        Block *block;
        block = new Block();
        block->nonce = blockMsg->getNonce();
        block->transactions = blockMsg->getTransactions();
        block->previousBlockHash = (string)blockMsg->getPreviousBlockHash();
        block->currentBlockHash = (string)blockMsg->getCurrentBlockHash();

        switch(msgType){

        case 1:
            //proof of work, the node check if the calculated hash value matches the hash value in block
            if(proofOfWork(block)){
                NodeVector* nodes = callNeighborSet(5);
                for (auto i = nodes->begin(); i != nodes->end(); ++i){

                    NodeHandle node = *i;
                    if(thisNode.getIp() == node.getIp()){
                        continue;
                    }


                    node.setPort(8333);
                    BlockMsg *blockMsg;
                    blockMsg = new BlockMsg();
                    blockMsg->setType(2);
                    blockMsg->setSenderAddress(thisNode);
                    blockMsg->setNonce(block->nonce);
                    blockMsg->setTransactions(block->transactions);
                    blockMsg->setPreviousBlockHash((block->previousBlockHash).c_str());
                    blockMsg->setCurrentBlockHash((block->currentBlockHash).c_str());


                    sendMessageToUDP(node, blockMsg);
                }

                //add the block to the chain
                blockChain.push_back(block);
                //clear the transactions map
                vseTransakcije.clear();
                cancelEvent(blockChainBuildingTimerMsg);
                cancelEvent(timerMsg);
                //plan for new transactions generation
                scheduleAt(simTime() + SimTime::parse("10s"), timerMsg);

            }
            break;

        case 2:
            list<Block *>::iterator it;
            for (it = blockChain.begin(); it != blockChain.end(); ++it){
                if((*it)->currentBlockHash == block->currentBlockHash){
                    return;

                }

            }
            cancelEvent(blockChainBuildingTimerMsg);
            cancelEvent(timerMsg);
            scheduleAt(simTime() + SimTime::parse("10s"), timerMsg);
            //clear transactions map
            vseTransakcije.clear();
            //add block to the chain
            blockChain.push_back(block);

            break;
        }
    }

    delete msg;
}

//function that puts all transactions from transactions map together, to get it ready for a block
std::list<int> MyApplication::allTransactionsTogether(){
    list<int> allTransactionsForBlock = {};
    map<int,std::list<int>>::iterator it;
    for(it = vseTransakcije.begin(); it != vseTransakcije.end(); it++){
        list<int> transactions1 = it->second;
        allTransactionsForBlock.splice(allTransactionsForBlock.end(), transactions1);

    }
    return allTransactionsForBlock;
}
//proof of work for the node
bool MyApplication::proofOfWork(Block *block){
    return sha256(toString(block)) == block->currentBlockHash;
}

//get hash value of the previous node, important only if the block is the first in the blockchain, therefore  previousBlocHash is all 0...
string MyApplication::previousHashValue(){
    string value;
    if(blockChain.size() == 0){
        value = "0000000000000000000000000000000000000000000000000000000000000000";
    }
    else{
        Block *block = blockChain.back();
        value = block->currentBlockHash;
    }
    return value;
}

//function that converts struct Block to string, so the sha256 value can be calculated
string MyApplication::toString(Block *block){
        list<int>::iterator it;
        string blockString = "";
        blockString += "nonce: " + to_string(block->nonce) + "\n";

        blockString += "transactions: \n";
        //iterate through all transactions in block
        for (it = block->transactions.begin(); it != block->transactions.end(); ++it){

            blockString += to_string(*it) + "\n";
        }

        blockString += "previous block hash: " + block->previousBlockHash + "\n";
        return blockString;
    }


