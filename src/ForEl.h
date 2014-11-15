#ifndef FOREL_H
#define FOREL_H

#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <ctime>
#include <cstdlib>

template<typename Content>
void clusterizationFOREL(vector<Content*> &clusters, vector<Content*> sources/*duplicate*/, double R) {//formal element
	cout<<"clusterizationFOREL("<<sources.size()<<"-->";
	double R2 = R*R;
	int size;
	srand(time(NULL));
	Content tmp, tmp2;
	vector<Content**> ptrs;
	int dim = tmp.dim();

	while(!sources.empty()) {
		size = sources.size();
		clusters.push_back(new Content);//new empty cluster
		Content &claster = *clusters.back();
		tmp.coords() = sources[rand()%size]->coords();//choose first point
		
		do {
			
			tmp2.coords() = tmp.coords();
			ptrs.clear();
			for(int i = 0; i < size; ++i) //find points into area
				if(Content::ro2(tmp, *sources[i]) <= R2)
					ptrs.push_back(&sources[i]);
			//calc average
			for(int i = 0; i < dim; ++i) 
				tmp.coords()[i] = 0;
			for(typename vector<Content**>::iterator it = ptrs.begin(); it != ptrs.end(); ++it) 
				for(int i = 0; i < dim; ++i) 
					tmp.coords()[i] += (**it)->coords()[i];
			for(int i = 0; i < dim; ++i) 
				tmp.coords()[i] /= ptrs.size();

		} while(tmp.coords() != tmp2.coords());	
		
		//fill cluster
		claster.coords() = tmp.coords();
		int size2 = ptrs.size();
		for(int i = 0; i < size2; ++i) {
			claster.pnts().push_back(*(ptrs[i]));
			*(ptrs[i]) = 0;
		}
		//remove used points
		sources.erase(std::remove( sources.begin(), sources.end(), static_cast<Content*>(0) ), sources.end()); 
	}
	cout<<clusters.size()<<")"<<endl;
}

#endif
