#include "CAN_Node.h"

CAN_Node Axis[] = { AXIS0_NODE_ID, AXIS1_NODE_ID };

int current_axis = 0;

void setup() {
  Serial.begin(UART_BAUDRATE);
  while (!Serial);  // Esperar a que se inicie comunicacion serial
  // Serial.println("CAN - ODrive");
  // Iniciar comunicacion CAN
  if (!CAN.begin(CAN_BAUDRATE)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

unsigned long long prevMillis = 0;
unsigned long long offset = 0;
int Tm = 1;
int A = 50;
int S = 5;
bool disable;

void loop() {
  if (!disable) {
    if ((millis() - prevMillis) > Tm) {
      prevMillis = millis();
      unsigned long long x = prevMillis - offset;
      // Serial.println(A*sin(x*M_PI/S));
      Axis[0].setInputPos(A*sin(x*M_PI/(S*1000)));
      Axis[1].setInputPos(A*sin(x*M_PI/(S*1000)));
    }
  }


  if (Serial.available()){
    char cmd = Serial.read();
    if (cmd == 'A') {
      A = Serial.parseInt();
    }
    if (cmd == 'S') {
      S = Serial.parseInt();
    }
    if (cmd == 'D') {
      disable = Serial.parseInt();
    }



    if (cmd == 'a') {
      current_axis = Serial.parseInt();
      Serial.write("Axis : ");
      Serial.println(current_axis);
    }
    if (cmd == 'f') {
      Serial.write("Requesting AXIS_STATE_FULL_CALIBRATION_SEQUENCE in Axis ");
      Serial.println(current_axis);
      Axis[current_axis].sendCommand(SET_AXIS_REQUESTED_STATE, AXIS_STATE_FULL_CALIBRATION_SEQUENCE);
    }
    if (cmd == 'c') {
      Serial.write("Requesting AXIS_STATE_CLOSED_LOOP_CONTROL in Axis ");
      Serial.println(current_axis);
      Axis[current_axis].setAxisRequestedState(AXIS_STATE_CLOSED_LOOP_CONTROL);
    }
    if (cmd == 'i') {
      Serial.write("Requesting AXIS_STATE_IDLE in Axis ");
      Serial.println(current_axis);
      Axis[current_axis].setAxisRequestedState(AXIS_STATE_IDLE);
    }
    if (cmd == 'p') {
      int pos = Serial.parseInt();
      Serial.write("input position:\t");
      Serial.println(pos);
      Axis[current_axis].setInputPos(pos);
    }
    if (cmd == '?') {
      Serial.write("Info    :\n");
      Serial.write("\tCurrent_axis:\t");  Serial.println(current_axis);
      Serial.write("\tNode_id:\t");  Serial.println(Axis[current_axis].getNodeId());
    }
  }

  /*int packetSize = CAN.parsePacket();
  Serial.println(packetSize);

  if (packetSize) {
    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);
    while (CAN.available()) {
      //Serial.print(CAN.read());
    }
  }*/
}
