//
// Generated file, do not edit! Created by nedtool 5.5 from applications/myapplication/BlockMsg.msg.
//

#ifndef __BLOCKMSG_M_H
#define __BLOCKMSG_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "common/TransportAddress.h"
#include <list>
typedef std::list<int> Transactions;
// }}

/**
 * Enum generated from <tt>applications/myapplication/BlockMsg.msg:35</tt> by nedtool.
 * <pre>
 * // first, we declare an enum to tell the message types apart:
 * enum MessageType
 * {
 *     BLOCKMSG_REQ = 1;           // outgoing message
 *     BLOCKMSG_FLOOD = 2;           // returning message
 * }
 * </pre>
 */
enum MessageType {
    BLOCKMSG_REQ = 1,
    BLOCKMSG_FLOOD = 2
};

/**
 * Class generated from <tt>applications/myapplication/BlockMsg.msg:42</tt> by nedtool.
 * <pre>
 * // now we declare the message
 * packet BlockMsg
 * {
 *     int type \@enum(MessageType);      // message type
 *     TransportAddress senderAddress;  // address of the node that sent the message
 *     int nonce;
 *     Transactions transactions;
 *     string previousBlockHash;
 *     string currentBlockHash;
 * }
 * </pre>
 */
class BlockMsg : public ::omnetpp::cPacket
{
  protected:
    int type;
    TransportAddress senderAddress;
    int nonce;
    Transactions transactions;
    ::omnetpp::opp_string previousBlockHash;
    ::omnetpp::opp_string currentBlockHash;

  private:
    void copy(const BlockMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BlockMsg&);

  public:
    BlockMsg(const char *name=nullptr, short kind=0);
    BlockMsg(const BlockMsg& other);
    virtual ~BlockMsg();
    BlockMsg& operator=(const BlockMsg& other);
    virtual BlockMsg *dup() const override {return new BlockMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getType() const;
    virtual void setType(int type);
    virtual TransportAddress& getSenderAddress();
    virtual const TransportAddress& getSenderAddress() const {return const_cast<BlockMsg*>(this)->getSenderAddress();}
    virtual void setSenderAddress(const TransportAddress& senderAddress);
    virtual int getNonce() const;
    virtual void setNonce(int nonce);
    virtual Transactions& getTransactions();
    virtual const Transactions& getTransactions() const {return const_cast<BlockMsg*>(this)->getTransactions();}
    virtual void setTransactions(const Transactions& transactions);
    virtual const char * getPreviousBlockHash() const;
    virtual void setPreviousBlockHash(const char * previousBlockHash);
    virtual const char * getCurrentBlockHash() const;
    virtual void setCurrentBlockHash(const char * currentBlockHash);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BlockMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BlockMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef __BLOCKMSG_M_H

