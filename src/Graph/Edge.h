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

#include <tr1/memory>

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

		Edge(const std::tr1::shared_ptr<vertex> &lptr, const std::tr1::shared_ptr<vertex> &rptr, double cost)
		 : _lvertex(lptr), _rvertex(rptr), _property(cost) {
		 	getProperty().name = countObj;
		 	++countObj;
		 }

		std::tr1::weak_ptr<vertex> lvertex() const {return _lvertex;}
		std::tr1::weak_ptr<vertex> rvertex() const {return _rvertex;}
		const PropertyEdge &getProperty() const {
			return _property;
		}
		PropertyEdge &getProperty() {
			return _property;
		}
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
		std::tr1::weak_ptr<vertex> _lvertex;
		std::tr1::weak_ptr<vertex> _rvertex;
		PropertyEdge _property;
};
template<typename Content>
int Edge<Content>::countObj = 0;

}

#endif
