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

#include <algorithm>
#include <iterator>

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;
#include <list>
using std::list;
#include <set>
using std::set;

#include "Cluster.h"
#include "Event.h"
 
const int Point::_dim = 2;

void compute(vector<Point*>& pnts) {
	for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it) {
		(*it)->src().cmpValue();
	}
}




/*static*/ 
void Filter::filterNight(Point* &ptr) {
	if(ptr->src().isNight()) 
		HelpFunctions::delPtr(ptr);
}

/*static*/ 
void HelpFunctions::clearVectorAndSrc(vector<Point*> &pnts) {
	for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it) {
		delete (*it);
		*it = NULL;
	}
}

void degreesToX(vector<Point*> pnts,double lngAver,double latAver) {
	Point tmp;
	for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it) {
		(*it)->src().convertDegrees(tmp.coords().at(0),tmp.coords().at(1),lngAver,latAver);
		(*it)->coords() = tmp.coords();
	}
}
