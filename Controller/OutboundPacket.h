#ifndef _OUTBOUNDPACKET_H
#define _OUTBOUNDPACKET_H

#include <string>

using namespace std;

class OutboundPacket
{
    public:
		OutboundPacket();
        virtual ~OutboundPacket();
        string getType();
        virtual string getLine() = 0;	//this is to be overriden
    protected:
        string _packetType;
        int progress;
};

#endif

