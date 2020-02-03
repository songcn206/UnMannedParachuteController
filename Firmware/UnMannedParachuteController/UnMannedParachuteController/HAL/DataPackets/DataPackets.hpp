/*
 * DataPackets.h
 *
 * Created: 24.01.2020 16:25:21
 *  Author: timot
 */ 


#ifndef DATAPACKETS_HPP_
#define DATAPACKETS_HPP_

#include "Control/Imu/Imu.hpp"

#include "HAL/UART/UART.hpp"
#include "HAL/ADC/ADC.hpp"
#include "Control/AbsBaro/AbsBaro.hpp"
#include "Control/DiffBaro/DiffBaro.hpp"
#include "HAL/Timers/GenericTimer/GenTimerD0.hpp"

#include <avr/pgmspace.h>

class DataPackets {
	public:
		struct SavedData {
			float lat;
			float lon;
			float alt;
			uint8_t no;
		};
	
	private:
		static SavedData savedData[1000]; // max ~14KB
		static uint16_t datapointer;
		static bool saveData;
		static bool arrayFull;
		
	public:
		static void SendStatus();
	
		static void SaveDataFromSensors();
	
		static void SendDataFromArray();
		
		static void SendOrSaveData();
		
		static void StartSavingData();
};


#endif /* DATAPACKETS_H_ */