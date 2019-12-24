#include <RCSwitch.h>

#include <timer.h>
auto timer = timer_create_default();

RCSwitch mySwitch = RCSwitch();

#define COUNTER_MAX 30*60  //30 минут
int counterShutDown = 0;
byte needSendShutDown = 0;
byte needShutDown = 0;

bool timerLoop(void *) {
  if (needShutDown){
    if (counterShutDown > COUNTER_MAX){
      counterShutDown = 0;
      needShutDown = 0;
      needSendShutDown = 1;
    } else
      counterShutDown++;
  }
}

void setup() {
    mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
    mySwitch.enableTransmit(10);
    mySwitch.setPulseLength(315);
    mySwitch.setRepeatTransmit(2);

    timer.every(1000, timerLoop);
}


void loop() {
  timer.tick();

  if (mySwitch.available()) {
    if (mySwitch.getReceivedValue() == 13254305L){
        mySwitch.send(mySwitch.getReceivedValue(), 24); 
        needShutDown = 1;
        counterShutDown = 0;
    } else if (mySwitch.getReceivedValue() == 13254306L){
        mySwitch.send(mySwitch.getReceivedValue(), 24); 
    }
      mySwitch.resetAvailable();
  }
  if (needSendShutDown) {
    needSendShutDown = 0;
    mySwitch.send(13254306, 24); 
  } 
}
