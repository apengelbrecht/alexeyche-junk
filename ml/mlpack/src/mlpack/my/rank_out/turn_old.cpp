#include "turn.hpp"

Clusters
calculate_clusters(const arma::mat& dataset, double r, size_t d1, size_t d2) 
{
	Log::Info << "Starting clustering with parameters - " << "dimension1: " << d1 << ", dimension2: " << d2 << ", resolution: " << r << endl;
	size_t m = dataset.n_rows;
	size_t dim = 2; // 2 axis
	
	//std::vector<arma::uvec*> col_sort_ind;
	//for(size_t ax=0; ax<dim; ax++) {
	//	arma::colvec col_cur = dataset.unsafe_col(ax);			
	//	col_sort_ind.push_back(new arma::uvec(arma::sort_index(col_cur, 0)));
	//}
	arma::colvec col1 = dataset.unsafe_col(0);
	arma::colvec col2 = dataset.unsafe_col(1);
	arma::uvec c1_s = arma::sort_index(col1, 0);
	arma::uvec c2_s = arma::sort_index(col2, 0);

	arma::umat SNN(m,dim); //SNN for dim independent axis 
	arma::vec dens(m); // density for each point
	dens.zeros();
	SNN.zeros();	
	
	size_t clust_axis = 0;
	size_t max_clust = 0;
	//for(size_t ax=0; ax<dim; ax++) {		
		size_t ax = 0;
		size_t clust_id=0;	// start clusters count from 1, 0 means no cluster
		bool in_clust=false;
		for(size_t i=1; i<(m-1); i++) {
			size_t cur = c1_s(i);
			size_t prev = c1_s(i-1);
			size_t next = c1_s(i+1);

			double dist_l = fabs(dataset(cur,ax) - dataset(prev,ax));
			double dist_r = fabs(dataset(cur,ax) - dataset(next,ax));
			//double dist_l2 = fabs(dataset(cur,1) - dataset(prev,1));
			//double dist_r2 = fabs(dataset(cur,1) - dataset(next,1));
			if((dist_l <= r) && (dist_r <= r)) { //&& (dist_l2 <= r) && (dist_r2 <= r)) { 	
				if(!in_clust) { clust_id++; in_clust = true; }
				SNN(cur,ax) = 1;
				//SNN(prev,ax) = clust_id;
				//SNN(next,ax) = clust_id;					
			} else {
				in_clust = false;
			}		
		}		
		ax=1;
		for(size_t i=1; i<(m-1); i++) {
			size_t cur = c2_s(i);
			size_t prev = c2_s(i-1);
			size_t next = c2_s(i+1);

			double dist_l = fabs(dataset(cur,ax) - dataset(prev,ax));
			double dist_r = fabs(dataset(cur,ax) - dataset(next,ax));
			//double dist_l2 = fabs(dataset(cur,1) - dataset(prev,1));
			//double dist_r2 = fabs(dataset(cur,1) - dataset(next,1));
			if((dist_l <= r) && (dist_r <= r)) { //&& (dist_l2 <= r) && (dist_r2 <= r)) { 	
				if(!in_clust) { clust_id++; in_clust = true; }
				SNN(cur,ax) = 1;
				//SNN(prev,ax) = clust_id;
				//SNN(next,ax) = clust_id;					
			} else {
				in_clust = false;
			}		
		}
		//if(clust_id > max_clust) {
		//	max_clust = clust_id;   //choosing axis with max clusters
		//	clust_axis = ax; 
		//}
	//}
	Log::Info << "Clusters for each axis perfomed. Axis with max clusters choosing for clustering (";
	Log::Info << clust_axis << ": " << max_clust << " clusters)";
	Log::Info << std::endl;

	Clusters clusts(m);
	
	

	in_clust=false;
	for(size_t i=0; i<m; i++) {
		size_t cur = c1_s(i);				
		if ( (SNN(cur,0)>0) || (SNN(cur,1)>0) )  {					
			//Log::Debug << "clust" << std::endl;
			if(!in_clust) {
				in_clust=true;
				clusts.clust_num++;
			}
			clusts.clust_ind(cur) = SNN(cur,0) + SNN(cur,1); //clusts.clust_num;
		} else {
			//Log::Debug << "clust not found" << std::endl;
			in_clust = false;
		}
	}
	return clusts;	
}