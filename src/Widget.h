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

#ifndef WIDGET_H
#define WIDGET_H


#include "Event.h"
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>

#include "Cluster.h"
#include "ForEl.h"
#include "Graph/Graph.h"

extern double RForEl;//m
extern double RSingle;//m

typedef Event2 Ev;

template< typename Point >
class Widget: public QWidget {
	typedef nmsgraph::Graph<Point> GraphP;
	typedef vector< std::tr1::shared_ptr<Point> > CPoint;
	
public:
	Widget(CPoint *pdat, int ax1, int ax2, const Ev &min_, const Ev &max_, QWidget *parent = NULL) : QWidget(parent) {
		setGeometry(100,100,700,700);
		showflag = true;
		for(int i = 0; i < 10; ++i) flagnumber[i] = false;
		fields[0] = 0.1;//x
		fields[1] = 0.1;//y
		xzoom=1.0;
		yzoom=1.0;
		xzs = yzs = 0.0;

		setData(pdat,min_,max_);
		
		setAxises(ax1, ax2);
	}
	~Widget() {}
	int xsh(double x) {
		return xscale*x + xshift;
	}
	int ysh(double y) {
		return h - (yscale*y + yshift);
	}
	double axsh(int ax) {
		return (ax-xshift)/xscale;
	}
	double aysh(int ay) {
		return (h - yshift - ay)/yscale;
	}
	
	void paintEvent(QPaintEvent *) {
	 	w = width();
		h = height();

		xscale = ((-1+2*fields[0])*w*xzoom)/(xrange[0]-xrange[1]);
		xshift = w*(-2*xzs*(-1+2*fields[0])*xzoom+xrange[1]*(-1+xzoom-2*fields[0]*xzoom)+xrange[0]*(1+xzoom-2*fields[0]*xzoom))/(2*(xrange[0]-xrange[1])); 
		yscale = ((-1+2*fields[1])*h*yzoom)/(yrange[0]-yrange[1]);
		yshift = h*(-2*yzs*(-1+2*fields[1])*yzoom+yrange[1]*(-1+yzoom-2*fields[1]*yzoom)+yrange[0]*(1+yzoom-2*fields[1]*yzoom))/(2*(yrange[0]-yrange[1])); 

		QPainter painter(this);
		painter.setPen(Qt::black);
		painter.setBrush(QBrush(Qt::SolidPattern));
		painter.setBrush(QBrush(QColor(128,128,128)));
		painter.drawRect(0,0,w,h);
		painter.setFont(QFont("Arial", 10));

		double num = 50;
		painter.drawLine(num, h - num, w - num, h - num);//X
		int n = 5;
		for(int k = 0; k < n; ++k) {
			painter.drawText(num + (w-2*num)*(double)k/(n-1)-10, h-num+10, QString::number(axsh(num + (w-2*num)*(double)k/(n-1))));
			painter.drawLine(num + (w-2*num)*(double)k/(n-1), h - num, num + (w-2*num)*(double)k/(n-1), h - num-5);//X
		}

		painter.drawLine(num, h - num, num, num);//Y
		for(int k = 0; k < n; ++k) {
			painter.drawText(num - 40, h - num - (h-2*num)*(double)k/(n-1) + 5, QString::number(aysh(h - num - (h-2*num)*(double)k/(n-1))));
			painter.drawLine(num, h - num - (h-2*num)*(double)k/(n-1), num + 5, h - num - (h-2*num)*(double)k/(n-1));//X
		}
		if(showflag) paintData(&painter);
	}
	
	void computeGraph() {
		assert(psources);
		Filter::hideSinglePoints(*psources,RSingle);
		clusterizationFOREL<Point>(clusters, *psources, RForEl);
		graph.setVertices(clusters.begin(),clusters.end());
		clusterizationMST(graph);
	}

	void keyPressEvent(QKeyEvent* pe);

	void setData(CPoint *ev,const Ev &min_, const Ev &max_) {
		assert(ev);
		psources = ev;
		min = min_;
		max = max_;
	}
	void paintData(QPainter* p);
	void setAxises(int a1, int a2) {
		axises[0] = a1;
		axises[1] = a2;
		setRanges( min.axis(axises[0]), min.axis(axises[1]),  max.axis(axises[0]), max.axis(axises[1]));
	}
	void setRanges(double x0, double y0, double x1, double y1) {
		xrange[0] = x0;
		xrange[1] = x1;
		yrange[0] = y0;
		yrange[1] = y1;
	}
private:
	double fields[2];

	double xrange[2];
	double yrange[2];

	double xscale, yscale;
	double xshift, yshift;
	int w,h;

	double xzoom,yzoom,xzs,yzs;

	int axises[2];

	CPoint *psources;
	vector<Point> clusters;
	GraphP graph;
	
	bool showflag;
	bool flagnumber[10];
	Ev min, max;
};

template< typename Point >
void Widget<Point>::paintData(QPainter* p) {
	if(!psources) return;
	double rx,ry;
	rx = ry = RForEl;
	rx *= xscale;
	ry *= yscale;
	if(rx < 1) rx = 1;
	if(ry < 1) ry = 1;
	typedef typename GraphP::CEdge CEdge;
	typedef typename GraphP::CVertex CVertex;


	p->setBrush(QBrush(Qt::NoBrush));

	//draw init Points
	cerr<<"draw init Points"<<endl;
	p->setPen(QColor(192,0,0));
	for(typename CPoint::const_iterator it = (*psources).begin(); it != (*psources).end(); ++it) {
		assert(*it);
		if((*it)->isHide()) continue;
		p->drawRect(QRectF(xsh((*it)->x(0)), ysh((*it)->x(1)),1,1));
	}
	cerr<<"draw Edges MST"<<endl;
	//draw Edges MST
	for(typename CEdge::const_iterator eit = graph.edges().begin(); eit != graph.edges().end(); ++eit) {
		p->setPen(Qt::white);
		assert(*eit);
		if((*eit)->getProperty().ruptured()) p->setPen(QColor(125,125,125));
		assert((*eit)->lvertex().lock());
		assert((*eit)->rvertex().lock());
		assert((*eit)->lvertex().lock()->ptr());
		assert((*eit)->rvertex().lock()->ptr());
		p->drawLine(
			xsh( (*eit)->lvertex().lock()->ptr()->x(0) ), 
			ysh( (*eit)->lvertex().lock()->ptr()->x(1) ), 
			xsh( (*eit)->rvertex().lock()->ptr()->x(0) ), 
			ysh( (*eit)->rvertex().lock()->ptr()->x(1) )
		);
	}
	//draw Vertices MST
	cerr<<"draw Vertices MST"<<endl;
	for(typename CVertex::const_iterator it = graph.vertices().begin(); it != graph.vertices().end(); ++it) {
		p->setPen(Qt::black);
		assert(*it);
		assert((*it)->ptr());
		p->drawEllipse(QRectF(xsh((*it)->ptr()->x(0))-rx, ysh((*it)->ptr()->x(1))-ry, 2*rx, 2*ry));
		p->setPen(Qt::black);
		p->drawRect(QRectF(xsh((*it)->ptr()->x(0)), ysh((*it)->ptr()->x(1)),1,1));
	}

}


template< typename Point >
void Widget<Point>::keyPressEvent(QKeyEvent* pe) {
	switch(pe->key()) {
		case Qt::Key_A:
			xzs -= 20/xscale;
		break;
		case Qt::Key_D:
			xzs += 20/xscale;
		break;
		case Qt::Key_W:
			yzs += 20/yscale;
		break;
		case Qt::Key_S:
			yzs -= 20/yscale;
		break;
		
		case Qt::Key_J:
			xzoom /= 1.5;
		break;
		case Qt::Key_L:
			xzoom *= 1.5;
		break;
		case Qt::Key_I:
			yzoom *= 1.5;
		break;
		case Qt::Key_K:
			yzoom /= 1.5;
		break;
		case Qt::Key_Return:
			showflag = !showflag;
			qDebug()<<"Key_Return";
		break;
		case Qt::Key_Minus:
			RSingle /= 1.25;
			computeGraph();
			qDebug()<<"Key_Minus RSingle = "<<RSingle;
		break;
		case Qt::Key_Equal:
			RSingle *= 1.25;
			computeGraph();
			qDebug()<<"Key_Equal RSingle = "<<RSingle;
		break;
			case Qt::Key_Less:
			RForEl /= 1.25;
			computeGraph();
			qDebug()<<"Key_Less RForEl = "<<RForEl;
		break;
		case Qt::Key_Greater:
			RForEl *= 1.25;
			computeGraph();
			qDebug()<<"Key_Greater RForEl = "<<RForEl;
		break;
		case Qt::Key_Escape:
			exit(0);
		break;
		default:
			QWidget::keyPressEvent(pe);
	}
	update();
}




#endif