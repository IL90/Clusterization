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

#ifndef FOREL_H
#define FOREL_H

#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <ctime>
#include <cstdlib>

template<typename Content>
void clusterizationFOREL(vector<Content> &clusters, const vector< std::tr1::shared_ptr<Content> > &sources2, double R) {//formal element
	typedef vector<Content> Vval;
	double R2 = R*R;
	int size;
	srand(time(NULL));
	vector< std::tr1::shared_ptr<Content> > sources;
	cerr<<"clusterizationFOREL("<<sources2.size()<<"-->";
	//int kap=0;
	for(typename vector< std::tr1::shared_ptr<Content> >::const_iterator it = sources2.begin(); it != sources2.end(); ++it) {
		if(!(*it)->isHide()) sources.push_back((*it));
	}
	cerr<<sources.size()<<"-->";
	
	if(sources.empty()) {cerr<<"0)"<<endl; return;}
	Content tmp, tmp2;

	vector< int > ptrs;
	int dim = tmp.dim();

	clusters.clear();

	while(!sources.empty()) {
		//cerr<<"kap = "<<kap++<<endl;
		size = sources.size();
		clusters.push_back(Content());//new empty cluster
		Content &cluster = clusters.back();
		tmp.coords() = sources[rand()%size]->coords();//choose first point
		
		do {
			tmp2.coords() = tmp.coords();
			ptrs.clear();
			for(int i = 0; i < size; ++i) //find points into area
				if(Content::ro2(tmp, *sources[i]) <= R2)
					ptrs.push_back(i);
			//calc average
			for(int i = 0; i < dim; ++i) 
				tmp.coords()[i] = 0;
			for(typename vector< int >::iterator it = ptrs.begin(); it != ptrs.end(); ++it) 
				for(int i = 0; i < dim; ++i) 
					tmp.coords()[i] += sources[*it]->coords()[i];
			for(int i = 0; i < dim; ++i) 
				tmp.coords()[i] /= ptrs.size();

		} while(tmp.coords() != tmp2.coords());	
		
		//fill cluster
		cluster.coords() = tmp.coords();
		int size2 = ptrs.size();
		for(int i = 0; i < size2; ++i) {
			cluster.pnts().push_back(sources[ptrs[i]]);
			sources[ptrs[i]].reset();
		}
		//remove used points
		std::tr1::shared_ptr<Content> emptyptr;
		sources.erase(std::remove( sources.begin(), sources.end(), emptyptr ), sources.end()); 
	}
	cerr<<clusters.size()<<")"<<endl;
}

#endif
