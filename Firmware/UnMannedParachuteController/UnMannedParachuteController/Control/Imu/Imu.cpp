/*
 * Imu.cpp
 *
 * Created: 11.01.2020 20:13:54
 *  Author: timot
 */ 

#include "Imu.hpp"

int16_t Imu :: acc[3];
int16_t Imu :: gyro[3];
int16_t Imu :: mag[3];

uint8_t Imu :: rawData[18];
uint8_t Imu :: dataPointer = 0;

uint8_t Imu :: discardedPackets = 0;

Imu :: AccFilter Imu :: AccXFilter;
Imu :: AccFilter Imu :: AccYFilter;
Imu :: AccFilter Imu :: AccZFilter;

Imu :: GyroFilter Imu :: GyroXFilter;
Imu :: GyroFilter Imu :: GyroYFilter;
Imu :: GyroFilter Imu :: GyroZFilter;

Imu :: MagFilter Imu :: MagXFilter;
Imu :: MagFilter Imu :: MagYFilter;
Imu :: MagFilter Imu :: MagZFilter;