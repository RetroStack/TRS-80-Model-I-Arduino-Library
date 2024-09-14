#include <Arduino.h>
#include "./Model1.h"
#include "./SerialLogger.h"

Model1 *model1;
SerialLogger *logger;

void setup()
{
  Serial.begin(500000); // baud rate
  Serial.println();

  logger = new SerialLogger();
  model1 = new Model1(logger, false);

  logger->info("Library setup complete.");

  model1->activateTestSignal();
  for(int i = 0; i < 4; i++) {
    Serial.println(i);
    model1->writeMemory(0x5C00+i, i+1);
  }
  Serial.println("Written...");
  // model1->deactivateTestSignal();
  // for(int i = 0; i < 4; i++) {
  //   Serial.println(model1->readMemory(0x5C00+i));
  // }
  // Serial.println("Read1");
  delay(5000);
  // model1->activateTestSignal();
  for(int i = 0; i < 4; i++) {
    Serial.println(model1->readMemory(0x5C00+i));
  }
  Serial.println("Read2");
  model1->deactivateTestSignal();
}

ISR(TIMER1_COMPA_vect)
{
  model1->refreshNextMemoryRow();
}

int n = 0;
int b = 0;
void loop()
{
  logger->info("Reading...");

  // model1->writeMemory(0x5a01, 16);
  // Serial.println(model1->readMemory(0x5a01));

  // model1->refreshNextMemoryRow();
  // model1->refreshNextMemoryRow();

  // model1->writeMemory(n, 2);
  // Serial.println(n, 16);
  // n++;
  // model1->writeMemory(1, 1);

  // Serial.println(model1->readMemory(0x3C00));
  // model1->readMemory(1);

  // model1->writeIO(0, 1);
  // model1->writeIO(1, 1);

  // model1->readIO(0);
  // model1->readIO(1);

  // Serial.println(n);
  // if (n == 20) {
  //   n = 0;
  //   if (b == 0) {
  //     b = 1;
  //     model1->activateTestSignal();
  //   } else {
  //     b = 0;
  //     model1->deactivateTestSignal();
  //   }
  // }

  model1->logState();

  delay(100000);
}
