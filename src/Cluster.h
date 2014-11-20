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

#ifndef CLASTER_H
#define CLASTER_H

#include <algorithm>
#include <iterator>
#include <cassert>
#include <vector>
using std::vector;

#include <tr1/memory>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

class Event;

class Point {
	public:
		typedef vector<Point*> Container;
		Point() : _isHide(false) {
			_x.resize(_dim, 0);
			//setSrc(0);
		}
		Point(const std::tr1::shared_ptr<Event> &pev) : _isHide(false) {
			_x.resize(_dim, 0);
			setSrc(pev);
		}
		Point(Point* ptr) : _isHide(false) {
			_x.resize(_dim, 0);
			//setSrc(0);
			_pnts.push_back(ptr);
			_x = ptr->_x;
		}
		double stddeviation() const {//only after calcAverage
			double sum = 0;
			for(Container::const_iterator it = _pnts.begin(); it != _pnts.end(); ++it)
				sum += ro(**it, *this);
			return sqrt(sum/_pnts.size());
		}
		int weight() const {return _pnts.size();}

		double x(int i) const {return _x[i];}

		vector<double> &coords() {return _x;}
		const vector<double> &coords() const {return _x;}
		Container &pnts() {return _pnts;}
		const Container &pnts() const {return _pnts;}

		int dim() const {return _dim;}
		const Event &src() const {
			if(_src.get() == 0) assert(0);
			return *_src;
		}
		Event &src() {
			if(_src.get() == 0) assert(0);
			return *_src;
		}
		void setSrc(const std::tr1::shared_ptr<Event> &ptr) {_src = ptr;}
		static double ro2(const Point &lhs, const Point &rhs) {
			double sum = 0;
			for(int i = 0; i < lhs.dim(); ++i) 
				sum += (lhs._x[i]-rhs._x[i])*(lhs._x[i]-rhs._x[i]);
			return sum;
		}
		static double ro(const Point &lhs, const Point &rhs) {
			return sqrt(ro2(lhs,rhs));
		}
		void setHide(bool b) {_isHide = b;}
		bool isHide() {return _isHide;}
	private:
		static const int _dim;//dimension
		std::tr1::shared_ptr<Event> _src;//content
		vector<double> _x;//coords
		Container _pnts;
		bool _isHide;
};


void compute(vector<Point*>& pnts);

template<typename T>
struct Cleaner {
	Cleaner(T *ptr, void (*func)(T&)) : obj(ptr), cleanFunc(func) {}
	~Cleaner() {cleanFunc(*obj);}
	T *obj;
	void (*cleanFunc)(T&);
};


class HelpFunctions {
public:
	static void delPtr(Point* &ptr) {
		delete ptr;
		ptr = NULL;
	}
	static void clearVector(vector<Point*> &pnts) {
		for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it) {
			delete (*it);
			*it = NULL;
		}
	}
	static void clearVectorAndSrc(vector<Point*> &pnts);
};

class Filter {
public:
	static void filtration(vector<Point*> &pnts, int typeFilter) {
		if(pnts.empty()) return;
		void (*vfunc)(Point* &);
		switch(typeFilter) {
			case 0:
				vfunc = filterNight;	
			break;
			default:
				vfunc = filterWeight;	
		} 
		for_each(pnts.begin(), pnts.end(), vfunc);
		pnts.erase( std::remove(pnts.begin(), pnts.end(), static_cast<Point*>(0)), pnts.end()); 
	}
	static void hideSinglePoints(vector<Point*> &pnts, double Rmax) {
		cerr<<"hideSinglePoints("<<pnts.size()<<"-->";
		if(pnts.empty()) return;
		double Rmax2 = Rmax*Rmax;
		bool flag;
		vector<Point*>::iterator it1, it2;
		for(it1 = pnts.begin(); it1 != pnts.end(); ++it1) {
			(*it1)->setHide(false);
		}
		int visible = pnts.size();
		for(it1 = pnts.begin(); it1 != pnts.end(); ++it1) {
			flag = false;
			for(it2 = pnts.begin(); it2 != pnts.end(); ++it2) {
				if(it1 == it2) continue;
				if((*it2)->isHide()) continue;
				if(Point::ro2(*(*it1),*(*it2)) < Rmax2) {
					flag = true;
					break;
				}
			}
			if(!flag) {
				(*it1)->setHide(true);
				--visible;
			}
		}

		cerr<<visible<<")"<<endl;
	}
private:
	static void filterNight(Point* &ptr);

	static void filterWeight(Point* &ptr) {
		if(ptr->weight() <= 1) 
			HelpFunctions::delPtr(ptr);
	}
};


#endif