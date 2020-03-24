/*
 * ExpMovingFilter.h
 *
 * Created: 01/01/2020 15:22:11
 *  Author: Timo Tiirats
 */ 

#ifndef EXPMOVINGFILTER_HPP_
#define EXPMOVINGFILTER_HPP_


template<typename conf, typename T>
class ExpMovingFilter {
	private:
		T average;
		bool firstvalue = true;
		static constexpr float alpha = conf :: alpha;
	
	public:
		T UpdateAndReturn(T value) {
			if (firstvalue) {
				average = value;
				firstvalue = false;
			} else {
				average = (alpha * value) + (1.0 - alpha) * average;
			}
			return average;
		}
};

struct AccConf {
	static constexpr float alpha = 0.1f;
}; 

struct GyroConf {
	static constexpr float alpha = 0.1f;
};

struct MagConf {
	static constexpr float alpha = 0.1f;
};



#endif /* EXPMOVINGFILTER_H_ */