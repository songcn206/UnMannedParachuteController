/*
 * DataPackets.cpp
 *
 * Created: 02.02.2020 12:30:52
 *  Author: timot
 */ 

#include "DataPackets.hpp"
#include "HAL/UART/ParseUart.hpp"
#include "Control/Servos/Servos.hpp"

uint16_t DataPackets :: datapointer = 0;
DataPackets :: SavedData DataPackets :: savedData[300];
bool DataPackets :: saveData = false;
bool DataPackets :: arrayFull = false;

void DataPackets :: SendStatus() {
	ExtUart :: SendString("[DATA] AX ");
	ExtUart :: SendInt(Imu :: GetAccXYZ()[0]);
	ExtUart :: SendString(" AY ");
	ExtUart :: SendInt(Imu :: GetAccXYZ()[1]);
	ExtUart :: SendString(" AZ ");
	ExtUart :: SendInt(Imu :: GetAccXYZ()[2]);
	ExtUart :: SendString(" GX ");
	ExtUart :: SendInt(Imu :: GetGyroXYZ()[0]);
	ExtUart :: SendString(" GY ");
	ExtUart :: SendInt(Imu :: GetGyroXYZ()[1]);
	ExtUart :: SendString(" GZ ");
	ExtUart :: SendInt(Imu :: GetGyroXYZ()[2]);
	ExtUart :: SendString(" MX ");
	ExtUart :: SendInt(Imu :: GetMagXYZ()[0]);
	ExtUart :: SendString(" MY ");
	ExtUart :: SendInt(Imu :: GetMagXYZ()[1]);
	ExtUart :: SendString(" MZ ");
	ExtUart :: SendInt(Imu :: GetMagXYZ()[2]);
	ExtUart :: SendString(" La ");
	ExtUart :: SendFloat(ParseGPSUart :: GetLatitude());
	ExtUart :: SendString(" Lo ");
	ExtUart :: SendFloat(ParseGPSUart :: GetLongitude());
	ExtUart :: SendString(" Alt ");
	ExtUart :: SendFloat(ParseGPSUart :: GetAltitude());
	ExtUart :: SendString(" No ");
	ExtUart :: SendUInt(ParseGPSUart :: GetGPSCount());
	ExtUart :: SendString(" D ");
	ExtUart :: SendUInt(Sonar :: GetDistance());
	ExtUart :: SendString(" APr ");
	ExtUart :: SendFloat(AbsoluteBaro :: GetPressure());
	ExtUart :: SendString(" DPr ");
	ExtUart :: SendInt(DiffBaro :: GetPressure());
	ExtUart :: SendString(" RM ");
	ExtUart :: SendUInt(Servos :: GetRightMotorPosition());
	ExtUart :: SendString(" LM ");
	ExtUart :: SendUInt(Servos :: GetLeftMotorPosition());
	ExtUart :: SendString("\n");
	
	DebugUart :: SendString("Data Sent");
	DebugUart :: SendUInt(Sonar :: GetDistance());
	DebugUart :: SendString("\n");
}

void DataPackets :: SendOrSaveData() {
	if (saveData) {
		SaveDataFromSensors();
	} else {
		SendStatus();
	}
}

void DataPackets :: SaveDataFromSensors() {
	//savedData[datapointer] = {.sonar = Sonar :: GetDistance(), .absbaro = AbsoluteBaro :: GetPressure(), .alt = ParseGPSUart :: GetAltitude()};
	
	savedData[datapointer] = {.ax = Imu :: GetAccXYZ()[0], .ay = Imu :: GetAccXYZ()[1], .az = Imu :: GetAccXYZ()[2],
								.gx = Imu :: GetGyroXYZ()[0], .gy = Imu :: GetGyroXYZ()[1], .gz = Imu :: GetGyroXYZ()[2],
								.mx = Imu :: GetMagXYZ()[0], .my = Imu :: GetMagXYZ()[1], .mz = Imu :: GetMagXYZ()[2]};
	
	
	if (datapointer == sizeof(savedData) / sizeof (SavedData) - 1) {
		saveData = false;
	}
	datapointer++;
}

void DataPackets :: SendDataFromArray() {
	GenTimerD0 :: StopSendStatusPackets();
	ExtUart :: SendString("[DATA FROM ARRAY]\n");
	/*for (uint16_t i = 0; i < datapointer; i++) {
		ExtUart :: SendUInt(savedData[i].sonar);
		ExtUart :: SendString(" ");
		ExtUart :: SendFloat(savedData[i].absbaro);
		ExtUart :: SendString(" ");
		ExtUart :: SendFloat(savedData[i].alt);
		ExtUart :: SendString("\n");
	}*/
	
	for (uint16_t i = 0; i < datapointer; i++) {
		ExtUart :: SendInt(savedData[i].ax);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].ay);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].az);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].gx);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].gy);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].gz);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].mx);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].my);
		ExtUart :: SendString(" ");
		ExtUart :: SendInt(savedData[i].mz);
		ExtUart :: SendString("\n");

	}
	
	
	
	GenTimerD0 :: StartSendStatusPackets();
}

void DataPackets :: StartSavingData() {
	saveData = true;
	datapointer = 0;
}

void DataPackets :: CancelSavingData() {
	saveData = false;
}