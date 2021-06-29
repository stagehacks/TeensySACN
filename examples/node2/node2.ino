#include <NativeEthernet.h>
#include <TeensyDMX.h>
namespace teensydmx = ::qindesign::teensydmx;
#include <TeensysACN.h>

byte MAC_ADDRESS[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int IP_ADDRESS[] = {192, 168, 1, 90};


elapsedMillis timeSincePacket = 0;

IPAddress SELF_IP(IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);

teensydmx::Sender dmxTx1{Serial2};
teensydmx::Sender dmxTx2{Serial1};


teensysacn::Receiver port1(1);
teensysacn::Receiver port2(2);


void setup() {

  Ethernet.begin(MAC_ADDRESS, SELF_IP);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWriteFast(2, HIGH);
  digitalWriteFast(6, HIGH);

  dmxTx1.begin();
  dmxTx2.begin();

  port1.begin();
  port2.begin();
}



void loop() {

  port1.update();
  if(port1.hasData()){
    resetIndicatorTimer();
    for(int i=1; i<=512; i++){
      dmxTx1.set(i, port1.getSlot(i));
    }
  }

  port2.update();
  if(port2.hasData()){
    resetIndicatorTimer();
    for(int i=1; i<=512; i++){
      dmxTx2.set(i, port2.getSlot(i));
    }
  }


  if (timeSincePacket < 20) {
    digitalWriteFast(LED_BUILTIN, HIGH);
  } else {
    digitalWriteFast(LED_BUILTIN, LOW);
  }

}



void resetIndicatorTimer() {
  if (timeSincePacket > 50) {
    timeSincePacket = 0;
  }
}
