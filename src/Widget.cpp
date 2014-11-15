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

#include "Widget.h"
#include <QDebug>
#include <QPoint>
#include <map>
using std::map;
#include <utility>
using std::pair;

class ColorFunctions {
public:
	static QColor colorHour(double h, int localZone = 3) {
		int r,g,b;
		double hour = h+localZone;
		if(hour >= 24) hour -= 24;
		else if(hour < 0 ) hour += 24;

		if(0 <= hour && hour < 8) {//r = 0
			r = 255*(1-hour/8);
			g = 255*(hour/8);
			b = 0;
		}
		else if(8 <= hour && hour < 16) {//g = 8
			r = 0;
			g = 255*(1-(hour-8)/8);
			b = 255*((hour-8)/8);
		}
		else if(16 <= hour && hour < 24) {//b = 16
			r = 255*((hour-16)/8);
			g = 0;
			b = 255*(1-(hour-16)/8);
		}
		return QColor(r,g,b);
	}
	static QColor colorInterval(Point*lhs, Point*rhs) {
		double d = fabs(rhs->psrc()->datetime.toTime_t() - lhs->psrc()->datetime.toTime_t())/60 ;//0 - 300
		
		int r = 0, g = 0, b = 0, alpha = 255;
		if(d < 70) r = g = b = 255;
		else if(d < 130) r = 255;
		else if(d < 190) g = 255;
		else if(d < 300) b = 255;
		else if(d < 720) {b = g = 128;}
		else if(d < 1440) {r = g = b = 64;}
		else alpha = 0;

		return QColor(r, g, b, alpha);
	}
	static QColor colorHeight(Point *ptr) {
		double d = ptr->psrc()->height();//0 - 300
		int r = 0, g = 0, b = 0;
		if(d < 50) r = g = b = 0;
		else if(d < 100) r = g = b = 92;
		else if(d < 150) b = 255;
		else if(d < 300) {b = g = 192;}
		else {r = g = b = 255;}
		return QColor(r, g, b);
	}
	static QColor colorVelocity(Point *ptr) {
		double d = ptr->psrc()->velocity();//0 - 100
		if(d < 0.8) return QColor(Qt::black);//3km/h
		else if(d < 1.3) return QColor(Qt::blue);//5km/h
		else if(d < 2.8) return QColor(Qt::green);//10km/h
		else if(d < 4.2) return QColor(Qt::red);//15km/h
		else if(d < 5.6) return QColor(255,255,0);//20km/h
		else return QColor(Qt::white);
	}
	static QColor colorWeight(Point *ptr) {
		int r, g, b;
		r = ptr->weight();
		r *= 10;
		g = 0;
		if(r < 0) r = 0;
		else if(r > 255) {
			r = r - 255;
			r = 255;
			if(g > 255) g = 255;
		}
		b = r;
		return QColor(r,g,b);
	}
};

bool compareTime(Point *lhs, Point *rhs) {
	return (lhs)->psrc()->datetime.toTime_t() < (rhs)->psrc()->datetime.toTime_t();
}






