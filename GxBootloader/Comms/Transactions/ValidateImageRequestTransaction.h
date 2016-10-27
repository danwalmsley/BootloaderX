/******************************************************************************
*       Description: 
*
*       Author: 
*         Date: 21 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef VALIDATEIMAGEREQUESTTRANSACTION_H_
#define VALIDATEIMAGEREQUESTTRANSACTION_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "Transaction.h"

/**
 *  ValidateImageRequestTransaction
 */
class ValidateImageRequestTransaction : public Transaction
{
#pragma mark Public Members
public:
    bool Encode (IDPPacket& packet)
    {
        return true;
    }

    bool Decode (Buffer& payloadData)
    {
        return true;
    }

    uint16_t GetId ()
    {
        return 0xB008;
    }

#pragma mark Protected Members
protected:

#pragma mark Private Members
private:

};

#endif
