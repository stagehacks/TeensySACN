#include "Arduino.h"
#include "TeensysACN.h"
#include <NativeEthernetUdp.h>

teensysacn::Receiver::Receiver(){
  //
}
teensysacn::Receiver::Receiver(byte universe){
  _universe = universe;
}

void teensysacn::Receiver::begin(){
  setUniverse(_universe);
}
void teensysacn::Receiver::begin(byte universe){
  setUniverse(universe);
}

void teensysacn::Receiver::update(){
  int packetSize = interface.parsePacket();
  if (packetSize) {
    interface.read(packetBuffer, 636);
    _slotCount = _checkHeader(packetBuffer, packetSize);
    
    if (_slotCount) {
      _packetReceived(packetBuffer);
    }
  }
}

void teensysacn::Receiver::setUniverse(byte universe){
  _universe = universe;
  interface.beginMulticast(IPAddress(239, 255, 0, _universe), 5568);
}

boolean teensysacn::Receiver::hasData(){
  return _hasData;
}
byte teensysacn::Receiver::getSlot(int slot){
  return _slots[slot-1];
}






void teensysacn::Receiver::_packetReceived(unsigned char * buf){
  //byte subnet = buf[113];
  byte universe = buf[114];
  byte priority = buf[108];
  byte ddpacket = buf[125];//added by andrew webberley
  if ( ddpacket != 0xdd ) {//added by andrew webberley
    if (universe == _universe && priority >= _priority) {
      _hasData = true;
      _priority = priority;
      for (int slot = 0; slot < _slotCount; slot++) {
        _slots[slot] = buf[126 + slot];
      }
    }
  }

}

int teensysacn::Receiver::_checkHeader(unsigned char * msg, int len) {

  for (byte i = 0; i < 10; i++) {
    if (msg[i + 4] != VALID_HEADER[i]) {
      return 0;
    }
  }

  if (msg[1] != 0x10) {
    return 0;
  }

  int addresscount = (byte) msg[123] * 256 + (byte) msg[124];
  return addresscount - 1;

}
