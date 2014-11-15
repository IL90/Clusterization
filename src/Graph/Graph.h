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

#ifndef GRAPH_H
#define GRAPH_H
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>

#include <set>
using std::set;
#include <vector>
using std::vector;
#include <list>
using std::list; 
#include <iostream>
using std::cout;
using std::endl;

#include "Vertex.h"
#include "Edge.h"

namespace nmsgraph {

template<typename Content>
class Graph {
	public:
		typedef Edge<Content> edge;
		typedef Vertex<Content> vertex;
		typedef vector<vertex*> CVertex;
		typedef vector<edge*> CEdge;
		
		Graph() {}
		~Graph() {
			for(typename CVertex::iterator it = _vertices.begin(); it != _vertices.end(); ++it)
				delete *it;
			for(typename CEdge::iterator it = _edges.begin(); it != _edges.end(); ++it)
				delete *it;
		}
		template<class InputIterator>
		void setVertices(InputIterator first, InputIterator last) {
			while (first!=last) {
				_vertices.push_back(new vertex(&(*first)));
				++first;
			}
		}
		template<class InputIterator>
		void setVertices_ptr(InputIterator first, InputIterator last) {
			while (first!=last) {
				_vertices.push_back(new vertex(*first));
				++first;
			}
		}
		double averageCost() const;
		void averageIntensity(double &full, double &rupt, double &unrupt) const;
		void ruptMostIntensityEdge();
		void computeIntensities() {
			for_each(_edges.begin(),_edges.end(),std::mem_fun(&edge::computeIntensity));
		}
		double quality() {
			double full, rupt, unrupt;
			averageIntensity(full,rupt,unrupt);
			return rupt/(unrupt + full);

		}
		void printVertices() const {
			for_each(_vertices.begin(),_vertices.end(),std::mem_fun(&vertex::printName));
		}
		void printEdges() const {
			for_each(_edges.begin(),_edges.end(),std::mem_fun(&edge::printName));
		}
		const CVertex &vertices()  const {return _vertices;}
		const CEdge &edges() const {return _edges;}
		void addEdge(vertex *lptr, vertex *rptr, double cost) {
			_edges.push_back(new edge(lptr, rptr, cost));
			_edges.back()->lvertex()->addEdge(_edges.back());
			_edges.back()->rvertex()->addEdge(_edges.back());
		}
	private:
		Graph(const Graph &);
		Graph &operator=(const Graph &);
	private:
		CVertex _vertices;
		CEdge _edges;
};

#include "Graph_impl.hpp"

}

#endif
