#include <LibPrintf.h>
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
  } else {
    Serial.println("CAN Running!");
  }
}

unsigned long long prevMillis = 0;
unsigned long long offset = 0;
int Tm = 1;
int A = 50;
int S = 5;
bool wave_enable = false;

void loop() {
  waveRutine();


  if (Serial.available()){
    char cmd = Serial.read();
    modifyBehaviourWave(cmd);
    sendMessagesToODrive(cmd);
  }
  readMessagesFromODrive();
}




void waveRutine(void) {
  if (wave_enable) {
    if ((millis() - prevMillis) > Tm) {
      prevMillis = millis();
      unsigned long long x = prevMillis - offset;
      // Serial.println(A*sin(x*M_PI/S));
      Axis[0].setInputPos(A*sin(x*M_PI/(S*1000)));
      Axis[1].setInputPos(A*sin(x*M_PI/(S*1000)));
    }
  }
}

void modifyBehaviourWave(char cmd) {
  if (cmd == 'A') {
    A = Serial.parseInt();
  }
  if (cmd == 'S') {
    S = Serial.parseInt();
  }
  if (cmd == 'E') {
    wave_enable = Serial.parseInt();
  }
}

void sendMessagesToODrive(char cmd) {
  if (cmd == 'a') {
    current_axis = Serial.parseInt();
    printf("Axis : %d\n", current_axis);
  }
  if (cmd == 'f') {
    printf("Requesting AXIS_STATE_FULL_CALIBRATION_SEQUENCE in Axis %d\n", current_axis);
    Axis[current_axis].sendCommand(SET_AXIS_REQUESTED_STATE, AXIS_STATE_FULL_CALIBRATION_SEQUENCE);
  }
  if (cmd == 'c') {
    printf("Requesting AXIS_STATE_CLOSED_LOOP_CONTROL in Axis %d\n", current_axis);
    Axis[current_axis].setAxisRequestedState(AXIS_STATE_CLOSED_LOOP_CONTROL);
  }
  if (cmd == 'i') {
    printf("Requesting AXIS_STATE_IDLE in Axis %d\n", current_axis);
    Axis[current_axis].setAxisRequestedState(AXIS_STATE_IDLE);
  }
  if (cmd == 'p') {
    int pos = Serial.parseInt();
    printf("input position:\t%d\n", pos);
    Axis[current_axis].setInputPos(pos);
  }
  if (cmd == '?') {
    printf("Info:\n");
    printf("\tCurrent_axis:\t%d\n", current_axis);
    printf("\tNode_id:\t%d\n", Axis[current_axis].getNodeId());
  }
}

void readMessagesFromODrive(void) {
  int packetSize = CAN.parsePacket();
  if (packetSize) {
    Serial.print("Received ");
    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }
    short packetId = CAN.packetId();
    byte nodeId = packetId >> 5;
    byte cmdId = packetId & 0b11111;
    Serial.print("packet with id 0x");
    Serial.print(packetId, HEX);
    Serial.print(" Node ");
    Serial.print(nodeId, HEX);
    Serial.print(" CMD_ID 0x");
    Serial.print(cmdId, HEX);

    if (CAN.packetRtr()) {
      byte dataLength = CAN.packetDlc();
      Serial.print(" and requested length ");
      Serial.println(dataLength);
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);
      //only print packet data for non-RTR packets
      // while (CAN.available()) {
      //   Serial.print(CAN.read(), HEX);
      // }
      if (cmdId == GET_ENCODER_ESTIMATES) {
        byte data[8];
        byte k = 0;
        while (CAN.available()) {
          data[k++] = CAN.read();
        }
        Serial.println("\tEncoder Estimates:");
        dataUnion position, velocity;
        for (int i = 0; i < 8; i++) {
          if (i < 4) {
            position.fBuff[i] = data[i];
          } else {
            velocity.fBuff[i-4] = data[i];
          }
        }
        Serial.print("Position:\t");  Serial.println(position.f);
        Serial.print("Velocity:\t");  Serial.println(velocity.f);
      }
    }
    Serial.println();
  }
}
