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

#ifndef GRAPH_IMPL_HPP
#define GRAPH_IMPL_HPP


template<typename Content>
double Graph<Content>::averageCost() const {
	double aver = 0;
	for(typename CEdge::const_iterator it = _edges.begin(); it != _edges.end(); ++it) {
		aver += (*it).getProperty().cost();
	}
	return aver/_edges.size();
}

template<typename Content>
void Graph<Content>::averageIntensity(double &full, double &rupt, double &unrupt) const {
	double f = 0,r = 0, u = 0;
	int rsize = 0, usize = 0;
	double tmp;
	for(typename CEdge::const_iterator it = _edges.begin(); it != _edges.end(); ++it) {
		tmp = (*it)->getProperty().intensity();
		f += tmp;
		if((*it)->getProperty().ruptured()){
			r += tmp;
			++rsize;
		}
		else {
			u += tmp;
			++usize;
		}
	}
	full = f/_edges.size();
	if(rsize == 0) rupt = 0;
	else rupt = r/rsize;
	if(usize == 0) unrupt = 0;
	else unrupt = u/usize;
}

template<typename Content>
void Graph<Content>::ruptMostIntensityEdge() {
	double maxintensity = 0;
	edge *pe = 0;
	double tmp;
	for(typename CEdge::const_iterator it = _edges.begin(); it != _edges.end(); ++it) {
		if((*it)->getProperty().ruptured()) 
			continue;
		if((tmp=(*it)->getProperty().intensity()) > maxintensity) {
			maxintensity = tmp;
			pe = (*it);
		}
	}
	if(pe) 
		pe->getProperty().setRuptured(true);
}

#endif
