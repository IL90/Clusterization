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

#include <string>
using std::string;

#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
#include <utility>
using std::pair;
#include <memory>

#include "Event.h"

#include <QDir>
#include <QDebug>
#include <QApplication>
#include "Widget.h"

#include "Cluster.h"

#include "Graph/MST.h"

extern void clearVector(vector<Point*> &pnts);
extern void filterClustersWithOnePoint(vector<Point*> &pnts);
extern void degreesToX(vector<Point*> pnts,double lngAver,double latAver);

void statistics(const vector<Point*> &pnts) {
	if(pnts.size() <= 1) return;
	vector<Point*>::const_iterator it = pnts.begin();
	vector<Point*>::const_iterator it2 = it; ++it;
	
	map<int,int> stats;
	map<int,int>::iterator mit;
	while(it != pnts.end()) {
		if((mit=stats.find( difftime1((*it2)->src(), (*it)->src())/60 )) == stats.end())
			stats.insert(pair<int,int>(difftime1((*it2)->src(), (*it)->src())/60, 1));
		else 
			++(*mit).second;
		++it;
		++it2;
	}
	mit = stats.begin();
	while(mit != stats.end()) {
		cout<<(*mit).first<<",\t"<<(*mit).second<<endl;
		++mit;
	}
	cout<<"size = "<<pnts.size()<<endl;
}


void readFromFile(vector<Point*> &sources) {
	struct ifs {
		ifs() {
			QString path1 = QString(".") + QDir::separator();
			QString path2 = QString("..") + QDir::separator() + QString("..") + QDir::separator() + QString("..") + QDir::separator();
			QString tagName = "tag2672";
			QString tmp = path1+tagName+Ev::filename();
			i.open(tmp.toStdString().c_str(), std::ifstream::in);
			if(i.is_open()) return;
			tmp = path2+tagName+Ev::filename();
			i.open(tmp.toStdString().c_str(), std::ifstream::in);
			if(!i.is_open()) {
				std::cerr<<"file("<<Ev::filename().toStdString().c_str()<<") cant be opened"<<endl;
				throw "error";
			}
		}
		~ifs() {i.close();}
		std::ifstream i;
	} obj;
	std::ifstream &in = obj.i;
	
	std::auto_ptr<Event> sptr;
	Ev* tmp;
	while(!in.eof()) {
		cout<<"k"<<endl;
		tmp = new Ev;
		sptr = std::auto_ptr<Event>(tmp);
		in>>(*tmp);
		if(tmp->phi == 0 || tmp->tet == 0) {
			sptr.reset();
		}
		else {
			sources.push_back(new Point);
			sources.back()->setSrc(std::tr1::shared_ptr<Event>(sptr.release()));
		}
	}
	delete sources.back();
	sources.pop_back();
	qDebug()<<"loaded from file";
}

int main(int argc, char *argv[]) {
	typedef nmsgraph::Graph<Point> GraphP;
	typedef GraphP::vertex vertex;

	vector<Point*> sources;
	Cleaner< vector<Point*> > cleanerSources(&sources, HelpFunctions::clearVectorAndSrc);
	readFromFile(sources);
	compute(sources);

	Ev min,max;
	min.setMinMax(sources, true);
	max.setMinMax(sources, false);
	degreesToX(sources, (max.phi+min.phi)/2, (max.tet+min.tet)/2);

	//Filter::hideSinglePoints(sources,RMAX);
	//vector<Point*> clusters;
	//Cleaner< vector<Point*> > cleanerClusters(&clusters, HelpFunctions::clearVector);
	//clusterizationFOREL<Point>(clusters, sources, RForEl);
	//Filter::filtration(clusters,1);

	int ax[2];
	ax[0] = 2;
	ax[1] = 3;
	/*if(argc == 3) {
		ax[0] = atoi(argv[1]);
		ax[1] = atoi(argv[2]);
	}*/
	
	
	//cout<<"clusters"<<endl;
	//statistics(clusters);
	//cout<<"sources"<<endl;
	//statistics(sources);



	double x1,y1,x2,y2;	
	min.convertDegrees(x1, y1, (min.phi+max.phi)*0.5,(min.tet+max.tet)*0.5);
	max.convertDegrees(x2, y2, (min.phi+max.phi)*0.5,(min.tet+max.tet)*0.5);
	
	QApplication app(argc,argv);
	
	Widget< Point > widget(&sources, ax[0], ax[1], min, max);
	widget.setRanges(x1,y1,x2,y2);
	widget.show();
	app.exec();
	
	return 0;
}