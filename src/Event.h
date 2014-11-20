/*
Copyright (C) 2014 Ilya Arkhipovsky
This file is part of Clusterization.
( Implementation of long arithmetic. The base of the system of numeration is any integer from 2 to 1 << ((sizeof(unsigned long long)<<2)-1) )

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contact e-mail <arkhipovsky.ilya@yandex.ru>
*/

#ifndef EVENT_H
#define EVENT_H

#define REARTH 6371000
#define PI 3.14159265359

#include <QDebug>

#include <QDateTime>
#include <iostream>
#include <cmath>
#include <vector>
using std::vector;
#include <list>
using std::list;

#include <tr1/memory>

using std::cout;
using std::endl;
#include "Sun.h"



class Point;

class Event {
public:
	friend long difftime1(const Event& ev1, const Event& ev2) {
		return abs(static_cast<long>(ev2.datetime.toTime_t()) - static_cast<long>(ev1.datetime.toTime_t()));
	}
	friend std::ostream &operator<<(std::ostream& out, const Event& event);
	friend std::istream &operator>>(std::istream& in, Event& event);
	virtual void input(std::istream& in);
	virtual ~Event() {}
	virtual void setMinMax(const vector< std::tr1::shared_ptr<Point> > &pnts, bool isMin);
	virtual double axis(int) = 0;
	virtual bool isNight() const {return true;}
	virtual void cmpValue() {cout<<"Event::cmpValue"<<endl;}
	virtual void convertDegrees(double &, double &, double, double) {}
	virtual double height() const {return 0;}
	virtual double velocity() const {return 0;}
	int day;
	QDateTime datetime;
};

class Event9: public Event {
typedef Event9 EventThis;
public:
	friend std::ostream &operator<<(std::ostream& out, const Event9& event);
	friend std::istream &operator>>(std::istream& in, Event9& event);
	void input(std::istream& in);
	void setMinMax(const vector< std::tr1::shared_ptr<Point> > &pnts, bool isMin);
	double axis(int i) {
		switch(i) {
			case 0:
				return day;
			case 1:
				return datetime.toTime_t();
			case 2: return data1[0];
			case 3: return data1[1];
			case 4: return data1[2];

			case 5: return data2[0];
			case 6: return data2[1];
			case 7: return data2[2];

			case 8:  return data3[0];
			case 9:  return data3[1];
			case 10: return data3[2];
			case 11: return data3[3];
			case 12: return data3[4];
			case 13: return data3[5];
			default: return -1;
		}
	}
	static QString filename() {
		return QString("_acc.txt");
	}
	void cmpValue() {}
	int data1[3];
	double data2[3];
	int data3[6];
};

class Event2: public Event {
typedef Event2 EventThis;
public:
	friend std::ostream &operator<<(std::ostream& out, const EventThis& event);
	friend std::istream &operator>>(std::istream& in, EventThis& event);
	void input(std::istream& in);
	void setMinMax(const vector< std::tr1::shared_ptr<Point> > &pnts, bool isMin);
	double axis(int i) {
		switch(i) {
			case 0:
				return day;
			case 1:
				return datetime.toTime_t();

			case 2: return phi;
			case 3: return tet;
			case 4: return _height;
			case 5: return data2;
			
			case 6: return data3[0];
			case 7: return data3[1];
			case 8: return data3[2];
			
			case 9:  return datetime2.toTime_t();
			
			case 10: return data4[0];
			case 11: return data4[1];

			case 12: return vel;
			case 13: return azimuth;
			case 14: return vel2;
			case 15: return data5;			
			default: return -1;
		}
	}
	void convertDegrees(double &x, double &y, double lngAver, double latAver = 0) {
		x = REARTH*(phi-lngAver)*PI/180*sin(0.5*PI-tet*PI/180);//l
		y = REARTH*(tet-latAver)*PI/180;//h
	}
	void cmpValue() {
		calcNight();
	}
	static QString filename() {
		return QString("_gps.txt");
	}
	void calcNight() {
		Sun sun;
		sun.setDate(datetime.date().year(),datetime.date().month(),datetime.date().day());
		sun.setCoords(tet,phi);
		double cur = datetime.time().hour() + datetime.time().minute()/60.0 + datetime.time().second()/3600.0;
		double rise = sun.UT(true);
		double set = sun.UT(false);
		if(rise == -1) {
			_night = true;
		}
		else if(rise == -2) _night = false;
		else {
			if(rise < set) {
				if(rise < cur && cur < set) _night = false;
				else _night = true;
			}
			else {
				if(rise < cur || cur < set) _night = false;
				else _night = true;
			}
		}
	}
	double height() const {return _height;}
	double velocity() const {return vel;}
	bool isNight() const {return _night;}
	double phi,tet;//degrees
	double _height;//m
	int data2;
	int data3[3];
	QDateTime datetime2;
	int data4[2];
	double vel;//m/s
	double azimuth;//degrees
	double vel2;//m/s
	double data5;

	bool _night;
};

class Event6: public Event {
typedef Event6 EventThis;
public:
	friend std::ostream &operator<<(std::ostream& out, const Event6& event);
	friend std::istream &operator>>(std::istream& in, Event6& event);
	void input(std::istream& in);
	void setMinMax(const vector< std::tr1::shared_ptr<Point> > &pnts, bool isMin);
	double axis(int i) {
		switch(i) {
			case 0:
				return day;
			case 1:
				return datetime.toTime_t() + datetime.time().msec()*0.001;
			case 2: return data1[0];
			case 3: return data1[1];
			case 4: return data1[2];
			case 5: return norm;
			default: return -1;
		}
	}
	static QString filename() {
		return QString("_acc.txt");
	}
	void cmpValue() {
		calcNorm();
	}
	void calcNorm() {
		norm = sqrt(data1[0]*data1[0] + data1[1]*data1[1] + data1[2]*data1[2]);
	}
	int data1[3];
	double norm;
};

/*
class Gps {
public:
	Gps(vector<Event9>* pdata) {
		int size = (*pdata).size();
		for(int i = 0; i < 2; ++i) {
			coords[i].resize(size);
			vels[i].resize(size);
			accels[i].resize(size);
		}
		accn.resize(size);
		acct.resize(size);
		double x[3][2];
		double T[3],X[3],A[3];

		(*pdata)[0].convertDegrees(x[0]);
		(*pdata)[1].convertDegrees(x[1]);
		T[0] = (*pdata)[0].datetime.toTime_t();
		T[1] = (*pdata)[1].datetime.toTime_t();

		for(int i = 1; i < size-1; ++i) {
			(*pdata)[i+1].convertDegrees(x[2]);
			T[2] = (*pdata)[i+1].datetime.toTime_t();
			for(int k = 0; k < 2; ++k) {
				for(int j = 0; j < 3; ++j) {
					X[j] = x[j][k];
				}
				curve(A,T,X);

				coords[k][i] = X[1];
				vels[k][i] = A[1] + 2*A[2]*T[1];
				accels[k][i] = 2*A[2];
			}

			double normv = sqrt(vels[0][i]*vels[0][i] + vels[1][i]*vels[1][i]);
			double norma2 = accels[0][i]*accels[0][i] + accels[1][i]*accels[1][i];
			acct[i] = (vels[0][i]*accels[0][i] + vels[1][i]*accels[1][i])/normv;
			accn[i] = sqrt(norma2 - acct[i]*acct[i]);

			for(int k = 0; k < 2; ++k) {
				x[0][k] = x[1][k];
				x[1][k] = x[2][k];
			}
		}
	}
	void curve(double A[3], double T[3], double X[3]) {
		double dT01 = T[0]-T[1];
		double dT02 = T[0]-T[2];
		double dT12 = T[1]-T[2];

		double dX01 = X[0]-X[1];
		double dX02 = X[0]-X[2];
		double dX12 = X[1]-X[2];

		double D = dT01*dT02*dT12;

		A[0] = ( T[2]*(T[1]*X[0]*dT12 - T[0]*X[1]*dT02) + T[0]*T[1]*X[2]*dT01 )/D;
		A[1] = ( T[2]*T[2]*dX01 + T[0]*T[0]*dX12 - T[1]*T[1]*dX02 )/D;
		A[2] = ( -T[2]*dX01 + T[1]*dX02 - T[0]*dX12 )/D;
	}

private:
	vector<double> coords[2];
	vector<double> vels[2];
	vector<double> accels[2];
	vector<double> accn;
	vector<double> acct;
};
*/



#endif