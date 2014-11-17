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

#ifndef VERTEX_H
#define VERTEX_H

#include <cassert>
#include <memory>
#include <set>
using std::set;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

#include <tr1/memory>

namespace nmsgraph {

template<typename Content>
class Edge;

template<typename Content>
class Vertex {
	public:
		typedef Edge<Content> edge;
		typedef vector< std::tr1::weak_ptr<edge> > CEdge;
		typedef typename CEdge::iterator CEiter;

		Vertex(Content *ptr) : _counter(1),_flag(false), _ptr(ptr) {
			++countObj;
		}
		
		void setPtr(Content *ptr) {_ptr=ptr;}
		const Content *ptr() const {return _ptr;}
		
		void printName() const {
			cout<<"Vertex("<<ptr()<<") counter = "<<_counter<<endl;
		}
		void printEdges() const {
			cout<<"edges:"<<endl;
			for(CEiter it = _edges.begin(); it != _edges.end(); ++it) {
				cout<<"\t"; (*it)->printName();
			}
		}

		const CEdge& edges() const {return _edges;}
		void addEdge(const std::tr1::shared_ptr<edge> &pe) {_edges.push_back(pe);}

		int count() const {return _counter;}
		void addCount(int c) {_counter += c;} 
		bool flag() const {return _flag;}
		void setFlag(bool f) {_flag = f;}
	private:
		Vertex(const Vertex &);
		Vertex &operator=(const Vertex &);
	private:
		static int countObj;
		CEdge _edges;
		int _counter;
		bool _flag;
		const Content * _ptr;
};
template<typename Content>
int Vertex<Content>::countObj = 0;

}

#endif
