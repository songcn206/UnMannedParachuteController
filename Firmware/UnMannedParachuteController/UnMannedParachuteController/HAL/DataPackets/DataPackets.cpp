/*
 * DataPackets.cpp
 *
 * Created: 02.02.2020 12:30:52
 *  Author: timot
 */ 

#include "DataPackets.hpp"
#include "HAL/UART/ParseUart.hpp"

uint16_t DataPackets :: datapointer = 0;
DataPackets :: SavedData DataPackets :: savedData[1000];
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
	ExtUart :: SendString("\n");
}

void DataPackets :: SendOrSaveData() {
	if (saveData) {
		if (arrayFull) {
			SendStatus();
		} else {
			SaveDataFromSensors();
		}
	} else {
		SendStatus();
	}
}

void DataPackets :: SaveDataFromSensors() {
	savedData[datapointer] = {.lat = ParseGPSUart :: GetLatitude(), .lon = ParseGPSUart :: GetLongitude(),
		.alt = ParseGPSUart :: GetAltitude(), .no = ParseGPSUart :: GetGPSCount()};
	
	if (datapointer == sizeof(savedData) / sizeof (SavedData) - 1) {
		arrayFull = true;
	}
	datapointer++;
}

void DataPackets :: SendDataFromArray() {
	GenTimerD0 :: StopSendStatusPackets();
	ExtUart :: SendString("[DATA FROM ARRAY]\n");
	for (uint16_t i = 0; i < datapointer; i++) {
		ExtUart :: SendFloat(savedData[i].lat);
		ExtUart :: SendString(" ");
		ExtUart :: SendFloat(savedData[i].lon);
		ExtUart :: SendString(" ");
		ExtUart :: SendFloat(savedData[i].alt);
		ExtUart :: SendString(" ");
		ExtUart :: SendUInt(savedData[i].no);
		ExtUart :: SendString("\n");
	}
	saveData = false;
	arrayFull = false;
	datapointer = 0;
	GenTimerD0 :: StartSendStatusPackets();
}

void DataPackets :: StartSavingData() {
	saveData = true;
}