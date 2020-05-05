/*
 * DataPackets.cpp
 *
 * Created: 02.02.2020 12:30:52
 *  Author: timot
 */ 

#include "DataPackets.hpp"
#include "HAL/UART/ParseUart.hpp"
#include "Control/Servos/Servos.hpp"
#include "HAL/EEPROM/EEPROM.hpp"

bool DataPackets :: saveData = false;

void DataPackets :: SendStatus() {
	/*ExtUart :: SendString("[DATA]");
	ExtUart :: SendString(" AX ");
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
	
	ExtUart :: SendString(" DPr ");
	ExtUart :: SendInt(DiffBaro :: GetPressure());
	ExtUart :: SendString(" RM ");
	ExtUart :: SendUInt(Servos :: GetRightMotorPosition());
	ExtUart :: SendString(" LM ");
	ExtUart :: SendUInt(Servos :: GetLeftMotorPosition());
	*/
	ExtUart :: SendString(" APr ");
	ExtUart :: SendFloat(AbsoluteBaro :: GetPressure());
	ExtUart :: SendString("\n");
	DebugUart :: SendString("[DATA]");
	DebugUart :: SendString(" MX ");
	//DebugUart :: SendInt(Imu :: GetMagXYZ()[0]);
	//DebugUart :: SendString(" No ");
	DebugUart :: SendString(" APr ");
	DebugUart :: SendFloat(AbsoluteBaro :: GetPressure());
	DebugUart :: SendUInt(ParseGPSUart :: GetGPSCount());
	DebugUart :: SendString(" D ");
	DebugUart :: SendUInt(Sonar :: GetDistance());
	DebugUart :: SendString(" RM ");
	DebugUart :: SendUInt(Servos :: GetRightMotorPosition()); 
	DebugUart :: SendString(" LM ");
	DebugUart :: SendUInt(Servos :: GetLeftMotorPosition()); 
	DebugUart :: SendString("\n");
	
}

bool DataPackets :: SendOrSaveData(bool retry) {
	if (saveData) {
		return SaveDataFromSensors(retry);
	} else {
		SendStatus();
		return true;
	}
}

bool DataPackets :: SaveDataFromSensors(bool retry) {
	if (ExternalEeprom :: IsMemoryFull()) {
		DebugUart :: SendString("Eeprom memory full\n");
		CancelSavingData();
		return true;
	} else {
		return ExternalEeprom :: SaveData(Imu :: GetAccXYZ()[0], Imu :: GetAccXYZ()[1], Imu :: GetAccXYZ()[2],
					Imu :: GetGyroXYZ()[0], Imu :: GetGyroXYZ()[1],  Imu :: GetGyroXYZ()[2],
					Imu :: GetMagXYZ()[0], Imu :: GetMagXYZ()[1], Imu :: GetMagXYZ()[2],
					ParseGPSUart :: GetLatitude(), ParseGPSUart :: GetLongitude(), ParseGPSUart :: GetAltitude(),
					ParseGPSUart :: GetGPSCount(), Sonar :: GetDistance(), AbsoluteBaro :: GetPressure(), 
					DiffBaro :: GetPressure(), Servos :: GetRightMotorPosition(), Servos :: GetLeftMotorPosition(), retry);
	}
}

void DataPackets :: SendDataFromArray() {
	GenTimerD0 :: StopSendStatusPackets(); // Stopping status packets while sending saved data to UART
	ExtUart :: SendString("[DATA FROM MEMORY ARRAY START]\n");
	ExternalEeprom :: SendData();
	ExtUart :: SendString("[DATA FROM MEMORY ARRAY END]\n");
	GenTimerD0 :: StartSendStatusPackets();
}

void DataPackets :: StartSavingData() { // Initializing data saving to external EEPROM
	if (ExternalEeprom :: InitSaving()) {
		saveData = true;
		GenTimerD0 :: ChangeStatusPacketToFaster(true); // Using different speed for saving data than sending data
		led3 :: SetHigh();
	} else {
		DebugUart :: SendString("EEPROM not Ready for saving\n");
	} 
}

void DataPackets :: CancelSavingData() {
	saveData = false;
	GenTimerD0 :: ChangeStatusPacketToFaster(false);
	led3 :: SetLow();
	uint16_t lastPacketNr = ExternalEeprom :: GetLastPacketNr();
	InternalEeprom :: WriteUint16((uint16_t)InternalEeprom :: EepromAddress :: LastSavedPacket, lastPacketNr); // saving last EEPROM packet value to internal EEPROM to recover data after power cycle
}