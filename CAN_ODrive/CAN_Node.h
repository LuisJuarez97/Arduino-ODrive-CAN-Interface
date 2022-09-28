#ifndef CAN_NODE_H_
#define CAN_NODE_H_

#include <CAN.h>
// Number of axes
#define NUM_AXES  2
// Baudrates
#define CAN_BAUDRATE  500000
#define UART_BAUDRATE 115200
// ODrive Nodes
#define AXIS0_NODE_ID 0X001
#define AXIS1_NODE_ID 0X002
// Cammand IDs
#define ODRIVE_HEARTBEAT_MESSAGE  0x001
#define ODRIVE_ESTOP_MESSAGE      0x002
#define GET_MOTOR_ERROR           0x003
#define GET_ENCODER_ERROR         0x004
#define GET_SENSORLESS_ERROR      0x005
#define SET_AXIS_NODE_ID          0x006
#define SET_AXIS_REQUESTED_STATE  0x007
#define SET_AXIS_STARTUP_CONFIG   0x008
#define GET_ENCODER_ESTIMATES     0x009
#define GET_ENCODER_COUNT         0x00A
#define SET_CONTROLLER_MODES      0x00B
#define SET_INPUT_POS             0x00C
#define SET_INPUT_VEL             0x00D
#define SET_INPUT_TORQUE          0x00E
#define SET_LIMITS                0x00F
#define START_ANTICOGGING         0x010
#define SET_TRAJ_VEL_LIMIT        0x011
#define SET_TRAJ_ACCEL_LIMIT      0x012
#define SET_TRAJ_INERTIA          0x013
#define GET_IQ                    0x014
#define GET_SENSORLESS_ESTIMATES  0x015
#define REBOOT_ODRIVE             0x016
#define GET_VBUS_VOLTAGE          0x017
#define CLEAR_ERRORS              0x018
#define SET_LINEAR_COUNT          0x019
#define SET_POSITION_GAIN         0x01A
#define SET_VEL_GAINS             0x01B
// Axis state resquest values
#define AXIS_STATE_UNDEFINED                          0
#define AXIS_STATE_IDLE                               1
#define AXIS_STATE_STARTUP_SEQUENCE                   2
#define AXIS_STATE_FULL_CALIBRATION_SEQUENCE          3
#define AXIS_STATE_MOTOR_CALIBRATION                  4
#define AXIS_STATE_ENCODER_INDEX_SEARCH               6
#define AXIS_STATE_ENCODER_OFFSET_CALIBRATION         7
#define AXIS_STATE_CLOSED_LOOP_CONTROL                8
#define AXIS_STATE_LOCKIN_SPIN                        9
#define AXIS_STATE_ENCODER_DIR_FIND                   10
#define AXIS_STATE_HOMING                             11
#define AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION  12
#define AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION     13

typedef float FLOAT;

typedef union {  
    FLOAT f;
    char fBuff[sizeof(FLOAT)];
} dataUnion;

class CAN_Node {
  private:
    int node_id;
  public:
    CAN_Node(byte _node_id);
    void sendCommand(byte cmd_id, byte value);
    void setAxisRequestedState(byte state);
    void setInputPos(float pos);
    int getNodeId(void);
};



#endif  /* CAN_NODE_H_ */