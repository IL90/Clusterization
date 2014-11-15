


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
#include "ForEl.h"
#include "Graph/MST.h"

#define RMAX 200 //m

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
		if((mit=stats.find( difftime1(*(*it2)->psrc(), *(*it)->psrc())/60 )) == stats.end())
			stats.insert(pair<int,int>(difftime1(*(*it2)->psrc(), *(*it)->psrc())/60, 1));
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

template<typename T>
struct Cleaner {
	Cleaner(T *ptr, void (*func)(T&)) : obj(ptr), cleanFunc(func) {}
	~Cleaner() {cleanFunc(*obj);}
	T *obj;
	void (*cleanFunc)(T&);
};

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
		tmp = new Ev;
		sptr = std::auto_ptr<Event>(tmp);
		in>>(*tmp);
		if(tmp->phi == 0 || tmp->tet == 0) {
			sptr.reset();
		}
		else {
			sources.push_back(new Point);
			sources.back()->setSrc(sptr.release());
		}
	}
	delete sources.back()->psrc();
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

	Filter::deleteSinglePoints(sources,RMAX*RMAX);

	vector<Point*> clusters;
	Cleaner< vector<Point*> > cleanerClusters(&clusters, HelpFunctions::clearVector);
	clusterizationFOREL<Point>(clusters, sources, RFOREL);

	GraphP graph;
	graph.setVertices_ptr(clusters.begin(),clusters.end());
	clusterizationMST(graph);

	int ax[2];
	ax[0] = 2;
	ax[1] = 3;
	/*if(argc == 3) {
		ax[0] = atoi(argv[1]);
		ax[1] = atoi(argv[2]);
	}*/
	
	//Filter::filtration(clusters,0);
	//cout<<"clusters"<<endl;
	//statistics(clusters);
	//cout<<"sources"<<endl;
	//statistics(sources);



	double x1,y1,x2,y2;	
	min.convertDegrees(x1, y1, (min.phi+max.phi)*0.5,(min.tet+max.tet)*0.5);
	max.convertDegrees(x2, y2, (min.phi+max.phi)*0.5,(min.tet+max.tet)*0.5);
	
	QApplication app(argc,argv);
	
	Widget< Point > widget(const_cast<vector<vertex *> *>(&graph.vertices()), ax[0], ax[1], min, max);
	widget.setGraph(&graph);
	widget.setRanges(x1,y1,x2,y2);
	widget.show();
	app.exec();
	
	return 0;
}