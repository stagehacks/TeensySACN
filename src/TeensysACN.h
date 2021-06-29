#include "Arduino.h"
#include <NativeEthernetUdp.h>

namespace teensysacn{

const char VALID_HEADER[10] = "ASC-E1.17";

class Receiver{
  public:
    Receiver();
    Receiver(byte universe);
    void begin();
    void begin(byte universe);
    void update();
    boolean hasData();
    byte getSlot(int slot);
    void setUniverse(byte universe);
  private:
    EthernetUDP interface;
    byte _universe = 1;
    byte _priority = 0;
    byte _hasData = false;
    byte _slots[512];
    int _slotCount;
    unsigned char packetBuffer[636];
    void _packetReceived(unsigned char * buf);
    int _checkHeader(unsigned char * msg, int len);
};

}
