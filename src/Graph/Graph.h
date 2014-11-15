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
