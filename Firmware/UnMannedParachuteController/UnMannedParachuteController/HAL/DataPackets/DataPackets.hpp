/*
 * DataPackets.h
 *
 * Created: 24.01.2020 16:25:21
 *  Author: timot
 */ 


#ifndef DATAPACKETS_HPP_
#define DATAPACKETS_HPP_

#include "Control/Imu/Imu.hpp"
#include "HAL/UART/ParseUart.hpp"
#include "HAL/UART/UART.hpp"
#include "HAL/ADC/ADC.hpp"
#include "Control/AbsBaro/AbsBaro.hpp"

class DataPackets {
	public:
		static void sendStatus() {
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
			ExtUart :: SendString(" Prs ");
			ExtUart :: SendFloat(AbsoluteBaro :: GetPressure());
			ExtUart :: SendString("\n");
		}
};


#endif /* DATAPACKETS_H_ */