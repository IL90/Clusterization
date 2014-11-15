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

#include "Event.h"
#include "Cluster.h"
#include <cassert>

std::istream &operator>>(std::istream& in, Event& event) {
	event.input(in);
	return in;
}
std::istream &operator>>(std::istream& in, Event9& event) {
	event.input(in);
	return in;
}
std::istream &operator>>(std::istream& in, Event2& event) {
	event.input(in);
	return in;
}
std::istream &operator>>(std::istream& in, Event6& event) {
	event.input(in);
	return in;
}

template<typename T>
bool less(T l, T r) {return l < r;}
template<typename T>
bool greater(T l, T r) {return l > r;}

void Event::setMinMax(vector<Point*>& pnts, bool isMin) {
	bool (*funci)(int,int);
	bool (*funcd)(double,double);
	if(isMin) {
		funci = less;
		funcd = less;
	}
	else {
		funci = greater;
		funcd = greater;
	}
	vector<Point*>::iterator it;
	Event *tmp;
	(*this) = *pnts.front()->psrc();
	for(it = pnts.begin(); it != pnts.end(); ++it) {
		tmp = ((*it)->psrc());
		if(funci(tmp->day , day)) day = tmp->day;
		if(funci(tmp->datetime.toTime_t() , datetime.toTime_t())) datetime = tmp->datetime;
	}
}
void Event9::setMinMax(vector<Point*>& pnts, bool isMin) {
	Event::setMinMax(pnts, isMin);
	bool (*funci)(int,int);
	bool (*funcd)(double,double);
	if(isMin) {
		funci = less;
		funcd = less;
	}
	else {
		funci = greater;
		funcd = greater;
	}
	vector<Point*>::iterator it;
	EventThis *tmp;
	(*this) = *dynamic_cast<EventThis*>(pnts.front()->psrc());
	for(it = pnts.begin(); it != pnts.end(); ++it) {
		tmp = dynamic_cast<EventThis*>((*it)->psrc());
		assert(tmp);
		for(int j = 0; j < 3; ++j) {
			if(funci(tmp->data1[j] , data1[j])) data1[j] = tmp->data1[j];
			if(funcd(tmp->data2[j] , data2[j])) data2[j] = tmp->data2[j];
		}
		for(int j = 0; j < 6; ++j)
			if(funci(tmp->data3[j] , data3[j])) data3[j] = tmp->data3[j];
	}
}

void Event2::setMinMax(vector<Point*> &pnts, bool isMin) {
	Event::setMinMax(pnts, isMin);
	bool (*funci)(int,int);
	bool (*funcd)(double,double);
	if(isMin) {
		funci = less;
		funcd = less;
	}
	else {
		funci = greater;
		funcd = greater;
	}
	vector<Point*>::iterator it;
	EventThis *tmp;

	(*this) = *dynamic_cast<EventThis*>(pnts.front()->psrc());
	for(it = pnts.begin(); it != pnts.end(); ++it) {
		tmp = dynamic_cast<EventThis*>((*it)->psrc());
		assert(tmp);
		if(funcd(tmp->phi , phi)) phi = tmp->phi;
		if(funcd(tmp->tet , tet)) tet = tmp->tet;
		if(funcd(tmp->_height , _height)) _height = tmp->_height;
		if(funci(tmp->data2 , data2)) data2 = tmp->data2;
		for(int j = 0; j < 3; ++j) 
			if(funci(tmp->data3[j] , data3[j])) data3[j] = tmp->data3[j];
		if(funci(tmp->datetime2.toTime_t() , datetime2.toTime_t())) datetime2 = tmp->datetime2;
		for(int j = 0; j < 2; ++j) 
			if(funci(tmp->data4[j] , data4[j])) data4[j] = tmp->data4[j];
		if(funcd(tmp->vel , vel)) vel = tmp->vel;
		if(funcd(tmp->azimuth , azimuth)) azimuth = tmp->azimuth;
		if(funcd(tmp->vel2 , vel2)) vel2 = tmp->vel2;
		if(funcd(tmp->data5 , data5)) data5 = tmp->data5;
	}
}

void Event6::setMinMax(vector<Point*> &pnts, bool isMin) {
	Event::setMinMax(pnts, isMin);
	bool (*funci)(int,int);
	bool (*funcd)(double,double);
	if(isMin) {
		funci = less;
		funcd = less;
	}
	else {
		funci = greater;
		funcd = greater;
	}
	vector<Point*>::iterator it;
	EventThis *tmp;
	(*this) = *dynamic_cast<EventThis*>(pnts.front()->psrc());
	for(it = pnts.begin(); it != pnts.end(); ++it) {
		tmp = dynamic_cast<EventThis*>((*it)->psrc());
		assert(tmp);
		for(int j = 0; j < 3; ++j) 
			if(funci(tmp->data1[j] , data1[j])) data1[j] = tmp->data1[j];
		if(funcd(tmp->norm , norm)) norm = tmp->norm;
	}
}

void Event::input(std::istream& in) {
	int tmp[6];
	char ch;
	//////////////////////////////////////////////////
	in>>day;//day of record
	in>>ch;//,
	//////////////////////////////////////////////////
	for(int i = 0; i < 3; ++i) {//date
		in>>tmp[i];
		if(i != 2) in>>ch;//.
	}
	datetime.setDate(QDate(tmp[2],tmp[1],tmp[0]));
	in>>ch;//,
	//////////////////////////////////////////////////
	in>>ch;//w //day of week
	in>>ch;//e 
	in>>ch;//,
	//////////////////////////////////////////////////
	for(int i = 3; i < 6; ++i) {//time
		in>>tmp[i];
		if(i != 5) in>>ch;//:
	}	
	datetime.setTime(QTime(tmp[3],tmp[4],tmp[5]));
	in>>ch;//,
}

void Event9::input(std::istream& in) {
	Event::input(in);
	char ch;
	for(int i = 0; i < 3; ++i) {//data1
		in>>data1[i];
		in>>ch;//,
	}
	//////////////////////////////////////////////////
	for(int i = 0; i < 3; ++i) {//data2
		in>>data2[i];
		in>>ch;//,
	}
	//////////////////////////////////////////////////
	for(int i = 0; i < 6; ++i) {//data3
		in>>data3[i];
		if(i != 5) in>>ch;//,
	}
	//////////////////////////////////////////////////
	//in>>ch;//\n
}

void Event2::input(std::istream& in) {
	char ch;
	int tmp[6];
		in>>ch;//G
		in>>ch;//P
		in>>ch;//S
		in>>ch;//,
	//////////////////////////////////////////////////	
		Event::input(in);
	//////////////////////////////////////////////////
		in>>phi;//coords
		in>>ch;//,
		in>>tet;
		in>>ch;//,
	//////////////////////////////////////////////////
		in>>_height;//_height
		in>>ch;//,
	//////////////////////////////////////////////////
		in>>data2;//data2
		in>>ch;//,
	//////////////////////////////////////////////////
		in>>ch;//A
		in>>ch;//,
	//////////////////////////////////////////////////
		for(int i = 0; i < 3; ++i) {//data3
			in>>data3[i];
			in>>ch;//,
		}
	//////////////////////////////////////////////////
		for(int i = 0; i < 3; ++i) {//date
			in>>tmp[i];
			if(i != 2) in>>ch;//.
		}
		datetime2.setDate(QDate(tmp[2],tmp[1],tmp[0]));
		in>>ch;//,
	//////////////////////////////////////////////////
		in>>ch;//w //day of week
		in>>ch;//e 
		in>>ch;//,
	//////////////////////////////////////////////////
		for(int i = 3; i < 6; ++i) {//time
			in>>tmp[i];
			if(i != 5) in>>ch;//:
		}	
		datetime2.setTime(QTime(tmp[3],tmp[4],tmp[5]));
		in>>ch;//,
	//////////////////////////////////////////////////
		for(int i = 0; i < 2; ++i) {//data4
			in>>data4[i];
			in>>ch;//,
		}	
	//////////////////////////////////////////////////
		in>>vel;
		in>>ch;//,
		in>>azimuth;
		in>>ch;//,
		in>>vel2;
		in>>ch;//,
		in>>data5;
	//////////////////////////////////////////////////
	//in>>ch;//\n
}

void Event6::input(std::istream& in) {
	Event::input(in);
	char ch;
	//////////////////////////////////////////////////
	for(int i = 0; i < 3; ++i) {//data1
		in>>data1[i];
		if(i != 2) in>>ch;//,
	}
	//////////////////////////////////////////////////
	//in>>ch;//\n
}
