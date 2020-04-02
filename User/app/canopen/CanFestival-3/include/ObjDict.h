
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef OBJDICT_H
#define OBJDICT_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 _valueRangeTest (UNS8 typeValue, void * value);
const indextable * _scanIndexOD(CO_Data *d, UNS16 wIndex, UNS32 * errorCode);//(UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

/* Master node data struct */
extern CO_Data ObjDict_Data;
extern UNS32 PDO1_transmit_mask_PDO1_mask_LowerBytes;		/* Mapped at index 0x2014, subindex 0x01 */
extern UNS32 PDO1_transmit_mask_PDO1_mask_upperBytes;		/* Mapped at index 0x2014, subindex 0x02 */
extern UNS32 PDO2_transmit_mask_PDO2_mask_LowerBytes;		/* Mapped at index 0x2015, subindex 0x01 */
extern UNS32 PDO2_transmit_mask_PDO2_mask_upperBytes;		/* Mapped at index 0x2015, subindex 0x02 */
extern UNS32 PDO3_transmit_mask_PDO3_mask_LowerBytes;		/* Mapped at index 0x2016, subindex 0x01 */
extern UNS32 PDO3_transmit_mask_PDO3_mask_upperBytes;		/* Mapped at index 0x2016, subindex 0x02 */
extern UNS32 PDO4_transmit_mask_PDO4_mask_LowerBytes;		/* Mapped at index 0x2017, subindex 0x01 */
extern UNS32 PDO4_transmit_mask_PDO4_mask_upperBytes;		/* Mapped at index 0x2017, subindex 0x02 */
extern UNS8 Error_status_bits[10];		/* Mapped at index 0x2100, subindex 0x00*/
extern UNS8 CAN_node_ID;		/* Mapped at index 0x2101, subindex 0x00*/
extern UNS16 CAN_bit_rate;		/* Mapped at index 0x2102, subindex 0x00*/
extern UNS16 SYNC_counter;		/* Mapped at index 0x2103, subindex 0x00*/
extern UNS16 SYNC_time;		/* Mapped at index 0x2104, subindex 0x00*/
extern UNS8 FWName[10];		/* Mapped at index 0x2200, subindex 0x00*/
extern UNS16 Switch_value;		/* Mapped at index 0x3000, subindex 0x00*/
extern INTEGER32 UserRegisters_userRegister1;		/* Mapped at index 0x4000, subindex 0x01 */
extern INTEGER32 UserRegisters_userRegister2;		/* Mapped at index 0x4000, subindex 0x02 */
extern INTEGER32 UserRegisters_userRegister3;		/* Mapped at index 0x4000, subindex 0x03 */
extern INTEGER32 UserRegisters_userRegister4;		/* Mapped at index 0x4000, subindex 0x04 */
extern INTEGER32 UserRegisters_userRegister5;		/* Mapped at index 0x4000, subindex 0x05 */
extern INTEGER32 UserRegisters_userRegister6;		/* Mapped at index 0x4000, subindex 0x06 */
extern INTEGER32 UserRegisters_userRegister7;		/* Mapped at index 0x4000, subindex 0x07 */
extern INTEGER32 UserRegisters_userRegister8;		/* Mapped at index 0x4000, subindex 0x08 */
extern INTEGER32 UserRegisters_userRegister9;		/* Mapped at index 0x4000, subindex 0x09 */
extern INTEGER32 UserRegisters_userRegister10;		/* Mapped at index 0x4000, subindex 0x0A */
extern INTEGER32 UserRegisters_userRegister11;		/* Mapped at index 0x4000, subindex 0x0B */
extern INTEGER32 UserRegisters_userRegister12;		/* Mapped at index 0x4000, subindex 0x0C */
extern INTEGER32 UserRegisters_userRegister13;		/* Mapped at index 0x4000, subindex 0x0D */
extern INTEGER32 UserRegisters_userRegister14;		/* Mapped at index 0x4000, subindex 0x0E */
extern INTEGER32 UserRegisters_userRegister15;		/* Mapped at index 0x4000, subindex 0x0F */
extern INTEGER32 UserRegisters_userRegister16;		/* Mapped at index 0x4000, subindex 0x10 */
extern INTEGER32 UserRegisters_userRegister17;		/* Mapped at index 0x4000, subindex 0x11 */
extern INTEGER32 UserRegisters_userRegister18;		/* Mapped at index 0x4000, subindex 0x12 */
extern INTEGER32 UserRegisters_userRegister19;		/* Mapped at index 0x4000, subindex 0x13 */
extern INTEGER32 UserRegisters_userRegister20;		/* Mapped at index 0x4000, subindex 0x14 */
extern INTEGER32 UserRegisters_userRegister21;		/* Mapped at index 0x4000, subindex 0x15 */
extern INTEGER32 UserRegisters_userRegister22;		/* Mapped at index 0x4000, subindex 0x16 */
extern INTEGER32 UserRegisters_userRegister23;		/* Mapped at index 0x4000, subindex 0x17 */
extern UNS16 Global_Gain;		/* Mapped at index 0x5400, subindex 0x00*/
extern UNS16 Global_Gain_Snd;		/* Mapped at index 0x5401, subindex 0x00*/
extern UNS16 Proportion_Gain;		/* Mapped at index 0x5402, subindex 0x00*/
extern UNS16 Damping_Gain;		/* Mapped at index 0x5403, subindex 0x00*/
extern UNS16 Velocity_Gain;		/* Mapped at index 0x5404, subindex 0x00*/
extern UNS16 VelocityInteg_Gain;		/* Mapped at index 0x5405, subindex 0x00*/
extern UNS16 AccFeedforward_Gain;		/* Mapped at index 0x5406, subindex 0x00*/
extern UNS16 Jerk_Hz;		/* Mapped at index 0x5407, subindex 0x00*/
extern UNS16 ProGain_VelocityMode;		/* Mapped at index 0x5408, subindex 0x00*/
extern UNS16 IntegGain_VelocityMode;		/* Mapped at index 0x5409, subindex 0x00*/
extern UNS16 DampingFilter_Gain;		/* Mapped at index 0x540A, subindex 0x00*/
extern UNS16 CurrentFilter_Gain;		/* Mapped at index 0x540B, subindex 0x00*/
extern UNS16 MaxVelocity;		/* Mapped at index 0x540C, subindex 0x00*/
extern UNS16 MaxAccel;		/* Mapped at index 0x540D, subindex 0x00*/
extern UNS16 JogSpeed;		/* Mapped at index 0x540E, subindex 0x00*/
extern UNS16 Jog_Accel;		/* Mapped at index 0x540F, subindex 0x00*/
extern UNS16 Jog_Decel;		/* Mapped at index 0x5410, subindex 0x00*/
extern UNS16 P2P_Velocity;		/* Mapped at index 0x5411, subindex 0x00*/
extern UNS16 P2P_Accel;		/* Mapped at index 0x5412, subindex 0x00*/
extern UNS16 P2P_Decel;		/* Mapped at index 0x5413, subindex 0x00*/
extern UNS16 P2P_ChgVelocity;		/* Mapped at index 0x5414, subindex 0x00*/
extern UNS16 Jog_ChgVelocity_ChgSpeed0;		/* Mapped at index 0x5415, subindex 0x01 */
extern UNS16 Jog_ChgVelocity_ChgSpeed2;		/* Mapped at index 0x5415, subindex 0x02 */
extern UNS16 Jog_ChgVelocity_ChgSpeed3;		/* Mapped at index 0x5415, subindex 0x03 */
extern UNS16 Jog_ChgVelocity_ChgSpeed4;		/* Mapped at index 0x5415, subindex 0x04 */
extern UNS16 Jog_ChgVelocity_ChgSpeed5;		/* Mapped at index 0x5415, subindex 0x05 */
extern UNS16 Jog_ChgVelocity_ChgSpeed6;		/* Mapped at index 0x5415, subindex 0x06 */
extern UNS16 Jog_ChgVelocity_ChgSpeed7;		/* Mapped at index 0x5415, subindex 0x07 */
extern UNS16 Jog_ChgVelocity_ChgSpeed8;		/* Mapped at index 0x5415, subindex 0x08 */
extern UNS16 Encoder_Res;		/* Mapped at index 0x5416, subindex 0x00*/
extern UNS16 FullEncoder_Res;		/* Mapped at index 0x5417, subindex 0x00*/
extern UNS16 Step_Rev;		/* Mapped at index 0x5418, subindex 0x00*/
extern UNS16 Step_Rev_Snd;		/* Mapped at index 0x5419, subindex 0x00*/
extern UNS16 Elec_Ratio_N;		/* Mapped at index 0x541A, subindex 0x00*/
extern UNS16 Elec_Ratio_D;		/* Mapped at index 0x541B, subindex 0x00*/
extern UNS16 StepMode;		/* Mapped at index 0x541C, subindex 0x00*/
extern UNS16 PositionFault_Window;		/* Mapped at index 0x541D, subindex 0x00*/
extern UNS16 InPositionCounts;		/* Mapped at index 0x541E, subindex 0x00*/
extern UNS16 PosErr_Range;		/* Mapped at index 0x541F, subindex 0x00*/
extern UNS16 InPos_ContTime;		/* Mapped at index 0x5420, subindex 0x00*/
extern UNS16 AnalogOffset_AnalogOffset1;		/* Mapped at index 0x5421, subindex 0x01 */
extern UNS16 AnalogOffset_AnalogOffset2;		/* Mapped at index 0x5421, subindex 0x02 */
extern UNS16 AnalogOffset_AnalogOffset3;		/* Mapped at index 0x5421, subindex 0x03 */
extern UNS16 AnalogType;		/* Mapped at index 0x5422, subindex 0x00*/
extern UNS16 AnalogDeadband_AnalogDeadband1;		/* Mapped at index 0x5423, subindex 0x01 */
extern UNS16 AnalogDeadband_AnalogDeadband2;		/* Mapped at index 0x5423, subindex 0x02 */
extern UNS16 AnalogDeadband_AnalogDeadband3;		/* Mapped at index 0x5423, subindex 0x03 */
extern UNS16 AnalogFilterGain;		/* Mapped at index 0x5424, subindex 0x00*/
extern UNS16 ServoEnable;		/* Mapped at index 0x5425, subindex 0x00*/
extern UNS16 AlarmResetInput;		/* Mapped at index 0x5426, subindex 0x00*/
extern UNS16 DefineLimits;		/* Mapped at index 0x5427, subindex 0x00*/
extern UNS16 GlobalGain_Selection;		/* Mapped at index 0x5428, subindex 0x00*/
extern UNS16 AlarmOutput;		/* Mapped at index 0x5429, subindex 0x00*/
extern UNS16 BrakeOutput;		/* Mapped at index 0x542A, subindex 0x00*/
extern UNS16 MoveOutput_MoveOutput1;		/* Mapped at index 0x542B, subindex 0x01 */
extern UNS16 MoveOutput_MoveOutput2;		/* Mapped at index 0x542B, subindex 0x02 */
extern UNS16 MoveOutput_MoveOutput3;		/* Mapped at index 0x542B, subindex 0x03 */
extern UNS16 MoveOutput_MoveOutput4;		/* Mapped at index 0x542B, subindex 0x04 */
extern UNS16 BrakeDisengage_Delay;		/* Mapped at index 0x542C, subindex 0x00*/
extern UNS16 BrakeEngage_Delay;		/* Mapped at index 0x542D, subindex 0x00*/
extern UNS16 Inputs_Filter_FilterInput1;		/* Mapped at index 0x542E, subindex 0x01 */
extern UNS16 Inputs_Filter_FilterInput2;		/* Mapped at index 0x542E, subindex 0x02 */
extern UNS16 Inputs_Filter_FilterInput3;		/* Mapped at index 0x542E, subindex 0x03 */
extern UNS16 Inputs_Filter_FilterInput4;		/* Mapped at index 0x542E, subindex 0x04 */
extern UNS16 NodeID;		/* Mapped at index 0x542F, subindex 0x00*/
extern UNS16 BaudRate;		/* Mapped at index 0x5430, subindex 0x00*/
extern UNS16 Clamp_para_Clamp_res;		/* Mapped at index 0x5431, subindex 0x01 */
extern UNS16 Clamp_para_Clamp_cont;		/* Mapped at index 0x5431, subindex 0x02 */
extern UNS16 Clamp_para_Clamp_time;		/* Mapped at index 0x5431, subindex 0x03 */
extern UNS16 VelocityReach_window;		/* Mapped at index 0x5432, subindex 0x00*/
extern UNS16 TorqueReach_window;		/* Mapped at index 0x5433, subindex 0x00*/
extern UNS16 TachOutput;		/* Mapped at index 0x5434, subindex 0x00*/
extern UNS16 JogMode;		/* Mapped at index 0x5435, subindex 0x00*/
extern UNS16 AlarmMask;		/* Mapped at index 0x5436, subindex 0x00*/
extern UNS16 DSP_Error_Code;		/* Mapped at index 0x603F, subindex 0x00*/
extern UNS16 Controlword;		/* Mapped at index 0x6040, subindex 0x00*/
extern UNS16 Statusword;		/* Mapped at index 0x6041, subindex 0x00*/
extern INTEGER16 Quick_stop_option_code;		/* Mapped at index 0x605A, subindex 0x00*/
extern INTEGER8 Modes_of_operation;		/* Mapped at index 0x6060, subindex 0x00*/
extern INTEGER8 Modes_of_operation_display;		/* Mapped at index 0x6061, subindex 0x00*/
extern INTEGER32 Position_actual_value;		/* Mapped at index 0x6064, subindex 0x00*/
extern UNS32 Maximal_following_error;		/* Mapped at index 0x6065, subindex 0x00*/
extern INTEGER32 Velocity_actual_value;		/* Mapped at index 0x606C, subindex 0x00*/
extern INTEGER16 target_torque;		/* Mapped at index 0x6071, subindex 0x00*/
extern UNS16 max_current;		/* Mapped at index 0x6073, subindex 0x00*/
extern INTEGER16 torque_demand_value;		/* Mapped at index 0x6074, subindex 0x00*/
extern INTEGER16 Current_actual_value;		/* Mapped at index 0x6078, subindex 0x00*/
extern INTEGER32 Target_position;		/* Mapped at index 0x607A, subindex 0x00*/
extern INTEGER32 Home_offset;		/* Mapped at index 0x607C, subindex 0x00*/
extern UNS8 polarity;		/* Mapped at index 0x607E, subindex 0x00*/
extern UNS32 Maximal_profile_velocity;		/* Mapped at index 0x607F, subindex 0x00*/
extern UNS32 Profile_velocity;		/* Mapped at index 0x6081, subindex 0x00*/
extern UNS32 Profile_acceleration;		/* Mapped at index 0x6083, subindex 0x00*/
extern UNS32 Profile_deceleration;		/* Mapped at index 0x6084, subindex 0x00*/
extern UNS32 Quick_stop_deceleration;		/* Mapped at index 0x6085, subindex 0x00*/
extern UNS32 torque_slope;		/* Mapped at index 0x6087, subindex 0x00*/
extern INTEGER8 Homing_method;		/* Mapped at index 0x6098, subindex 0x00*/
extern UNS32 Homing_speeds_Speed_for_switch_search;		/* Mapped at index 0x6099, subindex 0x01 */
extern UNS32 Homing_speeds_Speed_for_zero_search;		/* Mapped at index 0x6099, subindex 0x02 */
extern UNS32 Homing_acceleration;		/* Mapped at index 0x609A, subindex 0x00*/
extern INTEGER32 pos_err_actual_value;		/* Mapped at index 0x60F4, subindex 0x00*/
extern UNS32 digital_outputs_physical_outputs;		/* Mapped at index 0x60FE, subindex 0x01 */
extern INTEGER32 Target_velocity;		/* Mapped at index 0x60FF, subindex 0x00*/
extern UNS32 Supported_drive_modes;		/* Mapped at index 0x6502, subindex 0x00*/
extern UNS8 home_switch;		/* Mapped at index 0x7001, subindex 0x00*/
extern UNS16 idle_current;		/* Mapped at index 0x7002, subindex 0x00*/
extern UNS16 drive_inputs;		/* Mapped at index 0x7003, subindex 0x00*/
extern UNS16 torque_Constant;		/* Mapped at index 0x7005, subindex 0x00*/
extern UNS8 DSP_clearAlarm;		/* Mapped at index 0x7006, subindex 0x00*/
extern UNS8 QSegment;		/* Mapped at index 0x7007, subindex 0x00*/
extern UNS16 velocity_actual_value;		/* Mapped at index 0x7009, subindex 0x00*/
extern INTEGER32 position_actual_value;		/* Mapped at index 0x700A, subindex 0x00*/
extern UNS16 DSP_statuscode;		/* Mapped at index 0x700B, subindex 0x00*/
extern UNS8 zero_Position;		/* Mapped at index 0x700C, subindex 0x00*/
extern UNS16 Acceleration_Current;		/* Mapped at index 0x700D, subindex 0x00*/
extern UNS16 Analog_Input1;		/* Mapped at index 0x700E, subindex 0x00*/
extern UNS16 AlarmCodeUpper;		/* Mapped at index 0x700F, subindex 0x00*/
extern UNS16 HardStopCurrent;		/* Mapped at index 0x7010, subindex 0x00*/
extern UNS16 Peak_Current;		/* Mapped at index 0x7011, subindex 0x00*/
extern UNS16 Driver_Temperature;		/* Mapped at index 0x7012, subindex 0x00*/

#endif // OBJDICT_H