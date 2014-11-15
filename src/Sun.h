#ifndef SUN_H
#define SUN_H

//Source:
//	Almanac for Computers, 1990
//	published by Nautical Almanac Office
//	United States Naval Observatory
//	Washington, DC 20392
//
//Inputs:
//	day, month, year:      date of sunrise/sunset
//	latitude, longitude:   location for sunrise/sunset
//	zenith:                Sun's zenith for sunrise/sunset
//	  offical      = 90 degrees 50'
//	  civil        = 96 degrees
//	  nautical     = 102 degrees
//	  astronomical = 108 degrees
//	
//	NOTE: longitude is positive for East and negative for West
//
//Worked example (from book):
//	June 25, 1990:	25, 6, 1990
//	Wayne, NJ:      40.9, -74.3
//	Office zenith:  90 50' cos(zenith) = -0.01454

#define PI 3.14159265359
#define zenith 96
#include <cmath>
#include <iostream>
//692309- 69200
class Sun {
public:
	void setDate(int Year,int Month,int Day) {
		year = Year;
		month = Month;
		day = Day;
	}
	void setCoords(double lat,double lng) {
		latitude = lat;
		longitude = lng;
	}
	double UT(bool isRisingTime) {
		//2. convert the longitude to hour value and calculate an approximate time
		double lngHour = longitude / 15;
		int N = dayOfYear();
		double t;
		if(isRisingTime)
			t = N + ((6 - lngHour) / 24);
		else
			t = N + ((18 - lngHour) / 24);

		double L = calcTrueLongitude(t);//3-4
		double RA = calcRightAscension(L);
		double H = calcLocHour(L,isRisingTime);//6-7

		if(H == -1) {//the sun never rises on this location (on the specified date)
			return -1;
		}
		else if(H == -2) {//the sun never sets on this location (on the specified date)
			return -2;
		}

		//8. calculate local mean time of rising/setting	
		double T = H + RA - (0.06571 * t) - 6.622;
		//9. adjust back to UTC
		double UT = T - lngHour;
		if(UT >= 24)   UT -= 24;
		else if(UT < 0) UT += 24;
		return UT;
	}
	double durationDay() {
		double rise = UT(true);
		double set = UT(false);
		if(rise == -1) 
			return 0;
		else if(rise == -2) 
			return 24;
		return set - rise;
	}
private:
	int dayOfYear() {//1. first calculate the day of the year
		int N1,N2,N3;
		N1 = floor(275 * month / 9);
		N2 = floor((month + 9) / 12);
		N3 = (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
		return N1 - (N2 * N3) + day - 30;
	}
	double calcTrueLongitude(double t) {//3-4. calculate the Sun's true longitude
		//[Note throughout the arguments of the trig functions
		//(sin, tan) are in degrees. It will likely be necessary to
		//convert to radians. eg sin(170.626 deg) =sin(170.626*pi/180
		//radians)=0.16287]
		double M = (0.9856 * t) - 3.289;//3. calculate the Sun's mean anomaly
		double L = M + (1.916 * sin(M*PI/180)) + (0.020 * sin(2 * M*PI/180)) + 282.634;
		if(L >= 360)   L -= 360;
		else if(L < 0) L += 360;
		return L;
	}
	double calcRightAscension(double L) {//5
		//5a. calculate the Sun's right ascension
		double RA = 180/PI*atan(0.91764 * tan(L*PI/180));
		if(RA >= 360)   RA -= 360;
		else if(RA < 0) RA += 360;
		//5b. right ascension value needs to be in the same quadrant as L
		double Lquadrant  = (floor( L/90)) * 90;
		double RAquadrant = (floor(RA/90)) * 90;
		RA = RA + (Lquadrant - RAquadrant);
		//5c. right ascension value needs to be converted into hours
		RA = RA / 15;
		return RA;
	}
	double calcLocHour(double L, bool isRisingTime) {//6-7
		//6. calculate the Sun's declination
		double sinDec = 0.39782 * sin(L*PI/180);
		double cosDec = cos(asin(sinDec));
		//7a. calculate the Sun's local hour angle
		double cosH = (cos(zenith*PI/180) - (sinDec * sin(latitude*PI/180))) / (cosDec * cos(latitude*PI/180));
		if(cosH > 1) 
			return -1;//the sun never rises on this location (on the specified date)
		if(cosH <-1)
			return -2;//the sun never sets on this location (on the specified date)
		//7b. finish calculating H and convert into hours
		double H;
		if(isRisingTime)
			H = 360 - 180/PI*acos(cosH);
		else
			H = 180/PI*acos(cosH);
		H = H / 15;
		return H;
	}
private:
	double latitude,longitude;
	int year,month,day;
};
/*
int main() {
	Sun sun;
	sun.setDate(2014,10,15);
	sun.setCoords(69,48);
	double hour = sun.UT(true);
	std::cout<<"rs = "<<(int)hour<<":"<<(int)((hour-(int)hour)*60)<<std::endl;
	hour = sun.UT(false);
	std::cout<<"st = "<<(int)hour<<":"<<(int)((hour-(int)hour)*60)<<std::endl;
	hour = sun.durationDay();
	std::cout<<"dr = "<<(int)hour<<":"<<(int)((hour-(int)hour)*60)<<std::endl;

	return 0;
}*/

#endif
