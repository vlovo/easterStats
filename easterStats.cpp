// easter.cpp : Defines the entry point for the console application.
//

 
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iosfwd>

#include <numeric>
#include <functional>
#include <algorithm>

#include "gnuplot-cpp/gnuplot_i.hpp"

namespace est
{
	template<typename T>
	double median(const std::vector<T> input) 
	{
		double ret =0;
		std::vector<T> tmp(input.size(),0);
		std::copy(input.begin(),input.end(),tmp.begin());
		std::nth_element(tmp.begin(),tmp.begin()+tmp.size()/2,tmp.end());
		ret = tmp[tmp.size()/2];

		if(tmp.size() % 2 == 0 )
		{
			std::nth_element(tmp.begin(),	tmp.begin()+1+tmp.size()/2,tmp.end());
			ret =( ret + tmp[1+tmp.size()/2])/2.0;
		}
		return ret;
	}

	template<class T,class Iter>
	double median(const Iter begin, const Iter  end)
	{
		T tmp(begin,end);

		std::nth_element(tmp.begin(),tmp.begin()+tmp.size()/2,tmp.end());

		double ret = tmp[tmp.size()/2];

		if(tmp.size() % 2 == 0 ) // second pass and average
		{
			std::nth_element(tmp.begin(),	tmp.begin()+1+tmp.size()/2,tmp.end());
			ret =( ret + tmp[1+tmp.size()/2])/2.0;
		}
		return ret;

	}

	template <class T, class Iter>
	int accumulate_upTo(const Iter begin, const Iter end, T threshold,T start)
	{
		T s;
		Iter it = begin;
		do 
		{ 
			s = std::accumulate(begin,++it,start);

		} while ( s < threshold && it!=end);

		return std::distance(begin,it);
	}


///calculates the number of days after februrary which is easter sunday 
int getEasterDay( int YEAR ) 
{
	int  k = YEAR / 100;    // K(X) = X div 100
	int s = 2 - (3*k + 3) / 4;  // S(K) = 2 - (3K + 3) div 4
	int m = 15 + (3*k + 3) / 4 - (8*k + 13) / 25;  // M(K) = 15 + (3K + 3) div 4 - (K + 13) div 25
	int a = YEAR % 19;  // A(X) = X mod 19
	int d = ( 19*a + m ) % 30;  // D(A,M) = (19A + M) mod 30
	int r = ( d + a / 11 ) / 29;  //  R(D,A) = (D + A div 11) div 29
	int og = 21 + d - r;  // OG(D,R) = 21 + D - R
	int sz = 7 - ( YEAR + YEAR / 4 + s ) % 7;  // SZ(X,S) = 7 - (X + X div 4 + S) mod 7
	int oe = 7 - ( og - sz ) % 7;  // OE(OG,SZ) = 7 - (OG - SZ) mod 7

	return og + oe;
}


}
const int min_y = 1582;
const int max_y = 2582;

std::vector<int> getListOfYears(int easter_day,int start_year = min_y, int end_year = max_y)
{
	std::vector<int> listOfYears; 

	for(int year= start_year ; year < end_year; ++year)
	{
		if( easter_day == est::getEasterDay(year))
		{
			listOfYears.push_back(year);
		}
	}
	
	return (listOfYears);
}

struct result
{
public:
	result()
	{
		index_year=0;
		occurence =0;
	}
	int index_year;
	int occurence;
	int year;
};

//void maxReach(int &abs_min, int &abs_max, std::map<int,std::vector<int> > &eDayDiffDistribution, size_t e, std::map<int,result> &minMap, std::map<int,std::vector<int> > eDayDistribution)
//{
//	abs_min = std::min(abs_min,*min);
//	abs_max=  std::max(*max,abs_max);
//
//
//	int max_ = 0;
//	for(int i = 1; i < eDayDiffDistribution[e].size(); ++i)
//	{
//		int it = 	est::accumulate_upTo(ebegin,eDayDiffDistribution[e].end(),80.0,0.0);
//		ebegin++;
//
//		if(it > max_)
//		{       
//			max_ = it;
//			minMap[e].index_year = i-1;
//			minMap[e].occurence =it-1;
//			minMap[e].year = eDayDistribution[e][i-1];
//		}
//
//	}
//}

int main(int argc, char* argv[])
{
	const int min_easter_day =  22;
	const int max_easter_day  = min_easter_day+35; // 57 -31= 26
 
	std::vector<int> eDays;

	std::map<int,std::vector<int> > eDayDistribution;
	std::map<int,std::vector<int> > eDayDiffDistribution;
	std::map<int,result> minMap;
 


	int eday =  est::getEasterDay(2017);

	Gnuplot::set_GNUPlotPath("X:\\gnuplot\\bin");
    Gnuplot g("");
		 
	std::vector<int> h;
	
	for(int e = min_easter_day ; e < max_easter_day; ++e)
	{
		 eDays.push_back(e);
	 	 eDayDistribution[e] = getListOfYears(e,1582,2582);
		 h.push_back(eDayDistribution[e].size());
	}
	

	g.set_terminal_std("windows wsize 800,600");
	g.set_terminal_std("pngcairo size 800,600");
	g.showonscreen();
	g.cmd("set output 'easterFrequency01.png'");
	g.reset_plot();
	g.set_title("absolute frequency of easter day in the range from  the years 1582 to 2582");
	g.set_ylabel("absolute frequency");
	g.set_xlabel("easter day ");
	g.set_pointsize(1.5);
 	g.set_style("boxes fs solid 0.25 lc 3  lw 2.5").set_grid("lw 2 xtics   ytics ").plot_xy(eDays,h,"");
		

	/// answer question
	std::vector<int> list_16th_of_April = eDayDistribution[47];
	auto pos_of_2017 = std::find(list_16th_of_April.begin(),list_16th_of_April.end(),2017);
	int result =   (*(pos_of_2017+1) - *pos_of_2017) ;

	
	
	
	/// adjacent diff
	eDayDiffDistribution = eDayDistribution;
	
	for(size_t e = min_easter_day ; e < max_easter_day; ++e)
	{
		std::adjacent_difference(eDayDistribution[e].begin(),
			                     eDayDistribution[e].end(), 
								 eDayDiffDistribution[e].begin());
	}

	
	
	/// plot 16th April as example
	std::vector<int> e47(eDayDiffDistribution[47].size()-1,0);
	std::copy(eDayDiffDistribution[47].begin()+1,eDayDiffDistribution[47].end(),e47.begin());

	g.set_terminal_std("pngcairo size 800,600");
	g.showonscreen();
	g.cmd("set output 'repeatInterval16th.png'");
	g.reset_plot();
	g.set_title("repeat interval for  easter day April 16th  in the range from  1582 to 2582");
	g.set_ylabel("time / years");
	g.set_xlabel("list index");
	g.set_style("boxes fs solid 0.25 lc 3  lw 2.5").set_grid("lw 2 xtics   ytics ").plot_x(e47,"");




	//int abs_min =10000;
	//int abs_max = 0;
	//
	/// do statitics over repeat intervals

	std::vector<double> emin;
	std::vector<double> emax;
	std::vector<double> emean;
	std::vector<double> estd_dev;
	std::vector<double> emedian;


	for(size_t e = min_easter_day ; e < max_easter_day; ++e)
	{
		if(!eDayDiffDistribution[e].empty())
		{
		
			int numberOfData  = eDayDiffDistribution[e].size()-1;
		
			auto ebegin = eDayDiffDistribution[e].begin() +1;
		
			auto min = std::min_element(ebegin,eDayDiffDistribution[e].end());
			emin.push_back(*min);
				
			auto max = std::max_element(ebegin,eDayDiffDistribution[e].end());
			emax.push_back(*max);
			auto sum = std::accumulate(ebegin,eDayDiffDistribution[e].end(),0.0);
		
			double average = sum/ numberOfData;
			emean.push_back(average);
		
		


			double  sum_diff_squared = std::accumulate(ebegin,eDayDiffDistribution[e].end(),0.0,[&average](int a, int b)
															  {
																 return (a + pow((b-average),2.0) );
															  }
															  );

	   

			double std_dev =  sqrt( sum_diff_squared/(numberOfData -1));
		
			estd_dev.push_back(std_dev);
			auto diffCubicOp = [&average](int a,int b){return( a + pow((b-average),3.0) );};

			double sum_diff_cubic = std::accumulate(ebegin,eDayDiffDistribution[e].end(),0.0,diffCubicOp);
		
			double skewness =sum_diff_cubic/((numberOfData-1)*pow(std_dev,3));


			double med= est::median<std::vector<int> >(ebegin,eDayDiffDistribution[e].end());

			emedian.push_back(med);
		
			//maxReach(abs_min, abs_max, eDayDiffDistribution, e, minMap, eDayDistribution);

 
	
		
		
	}
	}


	Gnuplot g1("");
    g1.reset_all();
	
	/*g1.set_terminal_std("pngcairo size 800,600");
	g1.showonscreen();
	g1.cmd("set output 'statistics.png'");*/

	g1.reset_plot();
	g1.set_multiplot("layout 2,1");
	g1.set_title("");
	g1.set_ylabel("");
	g1.set_xlabel("");
	g1.plot_xy(eDays,emin,"min").plot_xy(eDays,emax,"min");
	
	
				 
 

	return 0;
 
}

