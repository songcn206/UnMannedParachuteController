/*
 * MovingAverageFilter.h
 *
 * Created: 01/01/2020 15:40:20
 *  Author: Timo Tiirats
 */ 


#ifndef MOVINGAVERAGEFILTER_HPP_
#define MOVINGAVERAGEFILTER_HPP_

template<typename conf>
class MovingAverageFilter {
	private:
		static constexpr uint16_t arrayLength = conf :: arrayLength;
		static uint16_t array[arrayLength];
		static uint8_t occupiedPlaces;
		static uint16_t sum;
		static uint16_t average;
	
	public:
		static void Init() {
			occupiedPlaces = 0;
		}
	
		static uint16_t UpdateAndReturn(uint16_t value) {
			if (occupiedPlaces < arrayLength) {
				array[occupiedPlaces] = value;
				occupiedPlaces++;
			} else {
				for (uint16_t i = 0; i < arrayLength - 1; i++) {
					array[i] = array[i+1];
				}
				array[arrayLength - 1] = value;
			}
			
			sum = 0;
			for (uint16_t i = 0; i < occupiedPlaces; i++) {
				sum += array[i];
			}
			average = sum / occupiedPlaces;
						
			return average;
		}
};

template <typename conf>
uint8_t MovingAverageFilter<conf> :: occupiedPlaces;

template <typename conf>
uint16_t MovingAverageFilter<conf> :: sum;

template <typename conf>
uint16_t MovingAverageFilter<conf> :: average;

template <typename conf>
uint16_t MovingAverageFilter<conf> :: array[arrayLength];

#endif /* MOVINGAVERAGEFILTER_H_ */