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
	
	private:
		static bool saveData;
		
	public:
		static void SendStatus();
	
		static bool SaveDataFromSensors(bool retry);
	
		static void SendDataFromArray();
		
		static bool SendOrSaveData(bool retry);
		
		static void StartSavingData();
		
		static void CancelSavingData();
};


#endif /* DATAPACKETS_H_ */