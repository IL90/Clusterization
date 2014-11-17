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

#ifndef MST_H
#define MST_H
//minimum spanning tree
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <functional>
#include <list>
using std::list;
#include <vector>
using std::vector;

#include <tr1/memory>

#include "Graph.h"

namespace nmsgraph {

class Ed {//edge for createMST_N2
public:
	Ed() : cost(1.0/0.0) {}
	friend bool operator<(const Ed &lhs,const Ed &rhs) {
		return lhs.cost < rhs.cost;
	}
	void set(int i1,int i2, double c) {
		i[0] = i1;
		i[1] = i2;
		cost = c;
	}
	int i[2];
	double cost;
};

template<typename Content>
void createMST_N2(Graph<Content> &graph) {
	cerr<<"createMST_N2"<<endl;
	typedef Graph<Content> graph_t;
	typedef typename graph_t::vertex vertex_t;
	typedef typename graph_t::edge   edge_t;

	int FullSize = graph.vertices().size();
	if(FullSize <= 1) {
		cerr<<"/createMST_N2(FullSize <= 1)"<<endl;
		return;
	}

	Ed tmped;
	int index = 0;
	const vector< std::tr1::shared_ptr<vertex_t> > &vertices = graph.vertices();
	typename vector< Ed >::iterator eit;
	vector<Ed> edges(FullSize - 1);
	vector<bool> used(FullSize, false);
	while(!edges.empty()) {//O(N^2)
		used[index] = true;

		for(int i = 0; i < FullSize; ++i) {//O(N)
			if(used[i]) 
				continue;
			tmped.set( index, i, Content::ro2(*vertices[index]->ptr(), *vertices[i]->ptr()) );
			edges[i] = std::min(edges[i], tmped);
		}
		eit = std::min_element(edges.begin(), edges.end());//O(N)
		if(eit == edges.end() || (*eit).cost == 1.0/0.0) 
			break;
		graph.addEdge(vertices[(*eit).i[0]], vertices[(*eit).i[1]], sqrt((*eit).cost));
		(*eit).cost = 1.0/0.0;
		index = (*eit).i[1];
	}
	cerr<<"/createMST_N2"<<endl;
}

template<typename Content>
void setIntensity(Graph<Content> &graph) {
	cerr<<"setIntensity"<<endl;
	typedef typename Graph<Content>::vertex vertex;
	typedef typename Graph<Content>::edge edge;
	typedef typename Graph<Content>::CVertex CVertex;
	
	typename list< vertex* >::iterator it;
	list< vertex* > lv;
	
	for(typename CVertex::const_iterator git = graph.vertices().begin(); git != graph.vertices().end(); ++git) {
		if((*git)->edges().size() == 1) 
			lv.push_back(*git);
	}

	bool b;
	int count;
	int N = graph.vertices().size();
	edge *pedge = 0;
	while(lv.size() > 1) {
		for(it = lv.begin(); it != lv.end(); ++it) {
			(*it)->setFlag(false);
			pedge = 0;
			for(typename vertex::CEdge::iterator eit = (*it)->edges().begin(); eit != (*it)->edges().end(); ++eit) {
				if((*eit)->getProperty().lcount() == 0) {
					pedge = *eit;
				}
			}
			if(pedge == 0) continue;
			count = (*it)->count();
			if(pedge->lvertex() == (*it)) {
				pedge->getProperty().setLcount(count);
				pedge->getProperty().setRcount(N-count);
				pedge->rvertex()->addCount(count);
				(*it) = pedge->rvertex();
			}
			else if(pedge->rvertex() == (*it)) {
				pedge->getProperty().setRcount(count);
				pedge->getProperty().setLcount(N-count);
				pedge->lvertex()->addCount(count);
				(*it) = pedge->lvertex();
			}
			else assert(0);
		}

		it = lv.begin();
		while(it != lv.end()) {
			if(!(*it)->flag()) {
				(*it)->setFlag(true);
				++it;
			}
			else {
				it = lv.erase(it);

			}
		}

		it = lv.begin();
		while(it != lv.end()) {
			b = false;
			for(typename vertex::CEdge::iterator eit = (*it)->edges().begin(); eit != (*it)->edges().end(); ++eit) {
				if((*eit)->getProperty().lcount() != 0) continue;
				if(!b) {
					b = true;
				}
				else {
					(*it)->setFlag(false);
					it = lv.erase(it);
					--it;
					break;
				}
			}
			++it;
		}
	}
	graph.computeIntensities();
	cerr<<"/setIntensity"<<endl;
}

template<typename Content>
void clusterizationMST(Graph<Content> &graph, int countClusters = 1) {
	cerr<<"clusterizationMST("<<graph.vertices().size()<<"-->)"<<endl;
	if(graph.vertices().size() <= 1) {cerr<<graph.vertices().size()<<")"<<endl; return;}
	createMST_N2(graph);
	//double r = graph.averageCost();
	//cerr<<"setIntensity(graph);"<<endl;
	//setIntensity(graph);
	/*for(int j = 1; j < countClusters; ++j)
		graph.ruptMostIntensityEdge();*/
	cerr<<"/clusterizationMST("<<"-->"<<countClusters<<")"<<endl;
}

}//end nmsgraph

#endif
