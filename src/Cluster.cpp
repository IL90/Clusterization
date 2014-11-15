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
		(*it)->psrc()->cmpValue();
	}
}




/*static*/ 
void Filter::filterNight(Point* &ptr) {
	if(ptr->psrc()->isNight()) 
		HelpFunctions::delPtr(ptr);
}

/*static*/ 
void HelpFunctions::clearVectorAndSrc(vector<Point*> &pnts) {
	for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it) {
		delete (*it)->psrc();
		delete (*it);
	}
}

void degreesToX(vector<Point*> pnts,double lngAver,double latAver) {
	Point tmp;
	for(vector<Point*>::iterator it = pnts.begin(); it != pnts.end(); ++it) {
		(*it)->psrc()->convertDegrees(tmp.coords().at(0),tmp.coords().at(1),lngAver,latAver);
		(*it)->coords() = tmp.coords();
	}
}
