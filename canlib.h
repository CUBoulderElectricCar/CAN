/*
 *  CU Electric Car CAN Messaging Library
 *  canlib.c
 *  Author: Jack Danielski
 */

#ifndef CANLIB_H_
#define CANLIB_H_

/******************************
 * CAN Device ID Definitions
 ******************************/

#define GPSRASPPI           0x1111
#define ODROIDXU            0x2222
#define CENTRAL             0x3333
#define JETSON              0x4444
#define SPEEDCTL            0x5555
#define STEERING            0x6666


/******************************
 * Message ID Definitions
 ******************************/

/* Steering Wheel ID */
#define STEERINGWHEEL       0x10

/* Speed ID */
#define CHANGESPEED         0x20

/* Sensor Data ID */
#define GETLIDARDISTANCE    0x30 // request LIDAR data
#define SENDLIDARDISTANCE   0x31 // send LIDAR data
#define GETUSSDISTANCE      0X32 // request ultrasonic sensor data
#define SENDUSSDISTANCE     0X33 // send ultrasonic sensor data
#define GETGPSDATA          0x34 // request GPS data
#define SENDGPSDATA         0x35 // send GPS data


/* Battery Data ID */
#define BATTVOLTAGE         0x40 // current voltage reading
#define CURRENTREAD         0x41 // current current reading

/* Misc. ID */
#define DISREGMSG           0x90 // disregards previous MSG

#endif
