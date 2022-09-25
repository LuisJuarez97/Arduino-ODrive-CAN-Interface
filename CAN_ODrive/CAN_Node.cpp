#include "CAN_Node.h"

#define FRAME_ID(node_id, cmd_id)  node_id << 5 | cmd_id

CAN_Node::CAN_Node(byte _node_id) : node_id(_node_id) {}

void CAN_Node::sendCommand(byte cmd_id, byte value) {
  CAN.beginPacket(FRAME_ID(node_id, cmd_id));
  CAN.write(value);
  CAN.endPacket();
}

void CAN_Node::setAxisRequestedState(byte state) {
  CAN.beginPacket(FRAME_ID(node_id, SET_AXIS_REQUESTED_STATE));
  CAN.write(state);
  CAN.endPacket();
}

void CAN_Node::setInputPos(float pos) {
  dataUnion position;
  position.f = pos;
  CAN.beginPacket(FRAME_ID(node_id, SET_INPUT_POS));
  for(int i = 0; i < sizeof(FLOAT); i++) {
    CAN.write(position.fBuff[i]);
  }
  CAN.endPacket();
}

int CAN_Node::getNodeId(void) {
  return node_id;
}