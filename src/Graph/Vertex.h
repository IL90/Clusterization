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

namespace nmsgraph {

template<typename Content>
class Edge;

template<typename Content>
class Vertex {
	public:
		typedef Edge<Content> edge;
		typedef set<edge*> CEdge;
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
		void addEdge(edge *pe) {_edges.insert(pe);}

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
