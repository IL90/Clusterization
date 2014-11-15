#ifndef MST_H
#define MST_H
//minimum spanning tree
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
using std::cout;
using std::endl;
#include <functional>
#include <list>
using std::list;
#include <vector>
using std::vector;

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
	typedef Graph<Content> graph_t;
	typedef typename graph_t::vertex vertex_t;
	typedef typename graph_t::edge   edge_t;

	int FullSize = graph.vertices().size();
	
	Ed tmped;
	int index = 0;
	const vector<vertex_t*> &vertices = graph.vertices();
	typename vector< Ed >::iterator eit;
	vector<Ed> edges(FullSize - 1);
	vector<bool> used(FullSize, false);

	while(!edges.empty()) {//O(N^2)
		used[index] = true;

		for(int i = 0; i < FullSize; ++i) {//O(N)
			if(used[i]) continue;
			tmped.set( index, i, Content::ro2(*vertices[index]->ptr(), *vertices[i]->ptr()) );
			edges[i] = std::min(edges[i], tmped);
		}
		
		eit = std::min_element(edges.begin(), edges.end());//O(N)
		if((*eit).cost == 1.0/0.0) break;
		graph.addEdge(vertices[(*eit).i[0]], vertices[(*eit).i[1]], sqrt((*eit).cost));
		(*eit).cost = 1.0/0.0;
		index = (*eit).i[1];
	}
}

template<typename Content>
void setIntensity(Graph<Content> &graph) {
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
}

template<typename Content>
void clusterizationMST(Graph<Content> &graph, int countClusters = 1) {
	cout<<"clusterizationMST("<<graph.vertices().size()<<"-->";
	createMST_N2(graph);
	//double r = graph.averageCost();
	setIntensity(graph);
	int j;
	for(j = 1; j < countClusters; ++j)
		graph.ruptMostIntensityEdge();
	cout<<j<<")"<<endl;
}

}

#endif
