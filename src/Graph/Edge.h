#ifndef EDGE_H
#define EDGE_H

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
class Vertex;

class PropertyEdge {
	public:
		PropertyEdge(double cst = 0): _lcount(0), _rcount(0), _ruptured(false) {
			setCost(cst);
		}
		void setCost(double cst) {_cost = cst;}
		double cost() const {return _cost;}
		int lcount() const {return _lcount;}
		int rcount() const {return _rcount;}
		double intensity() const {return _intensity;}
		bool ruptured() const {return _ruptured;}
		void setLcount(int c) {_lcount = c;}
		void setRcount(int c) {_rcount = c;}
		void setRuptured(bool f) {_ruptured = f;}
		void computeIntensity() {
			_intensity = _cost*4*_lcount*_rcount/((_lcount+_rcount)*(_lcount+_rcount));
		}
		int name;
	private:
		double _cost;
		int _lcount;
		int _rcount;
		double _intensity;
		bool _ruptured;
};

template<typename Content>
class Edge {
	public:
		typedef Vertex<Content> vertex;

		Edge(vertex *lptr, vertex *rptr, double cost)
		 : _lvertex(lptr), _rvertex(rptr), _property( std::auto_ptr<PropertyEdge>(new PropertyEdge(cost)) ) {
		 	getProperty().name = countObj;
		 	++countObj;
		 }

		/*friend void compareCost(const Edge &lhs,const Edge &rhs) {
			return lhs.getProperty().cost() < rhs.getProperty().cost();
		}*/

		vertex *lvertex() const {return _lvertex;}
		vertex *rvertex() const {return _rvertex;}
		const PropertyEdge &getProperty() const {return *_property.get();}
		PropertyEdge &getProperty() {return *_property.get();}
		void printName() const {
			cout<<"Edge("<<getProperty().name<<")["<<getProperty().lcount()<<"; "<<getProperty().rcount()<<"]";
			if(getProperty().ruptured()) cout<<" ruptured";
			cout<<endl;
			printVertices();
		}
		void printVertices() const {
			cout<<"vertices:"<<endl;
			cout<<"\t"; lvertex()->printName();
			cout<<"\t"; rvertex()->printName();
		}
		void computeIntensity() {
			getProperty().computeIntensity();
		}
		
	private:
		Edge(const Edge&);
		Edge &operator=(const Edge&);
	private:
		static int countObj;
		vertex *_lvertex;
		vertex *_rvertex;
		std::auto_ptr< PropertyEdge > _property;
};
template<typename Content>
int Edge<Content>::countObj = 0;

}

#endif
