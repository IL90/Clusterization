#ifndef CLASTER_H
#define CLASTER_H

#include <algorithm>
#include <iterator>
#include <cassert>
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

class Event;

class Point {
	public:
		typedef vector<Point*> Container;
		Point() {
			_x.resize(_dim, 0);
			setSrc(0);
		}
		Point(Event *pev) {
			_x.resize(_dim, 0);
			setSrc(pev);
		}
		Point(Point* ptr) {
			_x.resize(_dim, 0);
			setSrc(0);
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
		
		/*Event & src() {
			if(_src == 0) {
				cout<<"Error: Point::_src = 0"<<endl;
				assert(0);
			}
			return *_src;
		}*/
		Event *psrc() {
			return _src;
		}
		void setSrc(Event *ptr) {_src = ptr;}
		static double ro2(const Point &lhs, const Point &rhs) {
			double sum = 0;
			for(int i = 0; i < lhs.dim(); ++i) 
				sum += (lhs._x[i]-rhs._x[i])*(lhs._x[i]-rhs._x[i]);
			return sum;
		}
		static double ro(const Point &lhs, const Point &rhs) {
			return sqrt(ro2(lhs,rhs));
		}
	private:
		static const int _dim;//dimension
		Event *_src;//content
		vector<double> _x;//coords
		Container _pnts;
};


void compute(vector<Point*>& pnts);

class HelpFunctions {
public:
	static void delPtr(Point* &ptr) {
		delete ptr;
		ptr = NULL;
	}
	static void clearVector(vector<Point*> &pnts) {
		for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it)
			delete (*it);
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
	static void deleteSinglePoints(vector<Point*> &pnts, double Rmax2) {
		cout<<"deleteSinglePoints("<<pnts.size()<<"-->";
		if(pnts.empty()) return;
		bool flag;
		vector<Point*>::iterator it1, it2;
		for(it1 = pnts.begin(); it1 != pnts.end(); ++it1) {
			flag = false;
			for(it2 = it1; it2 != pnts.end(); ++it2) {
				if(it1 == it2) continue;
				if(Point::ro2(*(*it1),*(*it2)) < Rmax2) {
					flag = true; 
					break;
				}
			}
			if(!flag) 
				HelpFunctions::delPtr(*it1);
		}
		pnts.erase(std::remove( pnts.begin(), pnts.end(), static_cast<Point*>(0) ), pnts.end()); 
		cout<<pnts.size()<<")"<<endl;
	}
private:
	static void filterNight(Point* &ptr);

	static void filterWeight(Point* &ptr) {
		if(ptr->weight() <= 1) 
			HelpFunctions::delPtr(ptr);
	}
};


#endif