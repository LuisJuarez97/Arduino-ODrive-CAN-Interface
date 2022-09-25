// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

int count = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  if (Serial.available()){
    char cmd = Serial.read();
    Serial.write(cmd);
    if (count == 1) {
      CAN.beginPacket(0x12);
    }
    CAN.write(cmd);
    if (count == 8 || cmd == '\n') {
      CAN.endPacket();
      count = 0;
    }
    count++;
  }
}
