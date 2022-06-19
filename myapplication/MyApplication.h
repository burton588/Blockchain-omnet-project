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

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "common/OverSimDefs.h"

#include "common/BaseApp.h"

#include "common/CryptoModule.h"
#include "inet/networklayer/flood/Flood.h"
#include <list>
#include <string>
#include <map>
#include "Block.h"

using namespace std;
class MyApplication : public BaseApp
{
    // module parameters
    simtime_t sendPeriod;     // we'll store the "sendPeriod" parameter here
    int numToSend;            // we'll store the "numToSend" parameter here
    int largestKey;           // we'll store the "largestKey" parameter here

    // statistics
    int numSent;              //number of packets sent
    int numReceived;          //number of packets received

    // our timer
    cMessage *timerMsg;
    cMessage *blockChainBuildingTimerMsg;

    //all transactions we recieve by flooding protocol
    map<int, list<int>> vseTransakcije;

    //blockChain [chain if Block structures]


    //class that holds the structure of a bitcoin block



    //our blockChain [chain of blocks of transactions]
    list<Block *> blockChain;

    // application routines
    void initializeApp(int stage);                 // called when the module is being created
    void finishApp();                              // called when the module is about to be destroyed
    void handleTimerEvent(cMessage* msg);          // called when we received a timer message
    void deliver(OverlayKey& key, cMessage* msg);  // called when we receive a message from the overlay
    void handleUDPMessage(cMessage* msg);          // called when we receive a UDP message
    void handleTCPMessage(cMessage* msg);
    list<int> allTransactionsTogether();
    string previousHashValue();
    string toString(Block* block);
    bool proofOfWork(Block* block);

public:
    MyApplication() { timerMsg = NULL; };
    ~MyApplication() { cancelAndDelete(timerMsg); };
};


#endif
