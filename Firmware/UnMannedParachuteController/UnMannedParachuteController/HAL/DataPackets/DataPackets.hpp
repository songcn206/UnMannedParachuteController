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
		/*struct SavedData {
			uint16_t sonar;
			float absbaro;
			float alt;
		};*/
		
		struct SavedData {
			int16_t ax;
			int16_t ay;
			int16_t az;
			int16_t gx;
			int16_t gy;
			int16_t gz;
			int16_t mx;
			int16_t my;
			int16_t mz;	
		};
	
	private:
		static SavedData savedData[1]; // max ~14KB
		static uint16_t datapointer;
		static bool saveData;
		static bool arrayFull;
		
	public:
		static void SendStatus();
	
		static void SaveDataFromSensors();
	
		static void SendDataFromArray();
		
		static void SendOrSaveData();
		
		static void StartSavingData();
		
		static void CancelSavingData();
};


#endif /* DATAPACKETS_H_ */