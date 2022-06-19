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

//
// @author Antonio Zea
//

cplusplus {{
#include "common/TransportAddress.h"
#include <list>
typedef std::list<int> Transactions;
}}


// import the IP Address class
class noncobject TransportAddress;
class noncobject Transactions;
// first, we declare an enum to tell the message types apart:
enum MessageType
{
    BLOCKMSG_REQ = 1;           // message that has to be checked for proof of work
    BLOCKMSG_FLOOD = 2;           // message that is flooded between the nodes
}

// now we declare the message
packet BlockMsg
{
    int type enum(MessageType);      // message type
    TransportAddress senderAddress;  // address of the node that sent the message
    int nonce;						 
    Transactions transactions;
    string previousBlockHash;
    string currentBlockHash;
}

