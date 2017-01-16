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

		if(tmp.size() % 2==0 )
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
}

///calculates the number of days after februrary which is easter sunday 
int getEasterSundayIndex( int YEAR ) 
{
	int  k = YEAR / 100;    // K(X) = X div 100
	int s = 2 - (3*k + 3) / 4;  // S(K) = 2 - (3K + 3) div 4
	int m = 15 + (3*k + 3) / 4 - (8*k + 13) / 25;  // M(K) = 15 + (3K + 3) div 4 - (K + 13) div 25
	int a = YEAR % 19;  // A(X) = X mod 19
	int d = ( 19*a + m ) % 30;  // D(A,M) = (19A + M) mod 30
	//int r = (int) d / 29 + (int) d / 28 - (int) d / 29 - (int) a / 11;  // R(D,A) = D div 29 + (D div 28 - D div 29) (A div 11)
	int r = ( d + a / 11 ) / 29;  // Shorter Roegel version: R(D,A) = (D + A div 11) div 29
	int og = 21 + d - r;  // OG(D,R) = 21 + D - R
	int sz = 7 - ( YEAR + YEAR / 4 + s ) % 7;  // SZ(X,S) = 7 - (X + X div 4 + S) mod 7
	int oe = 7 - ( og - sz ) % 7;  // OE(OG,SZ) = 7 - (OG - SZ) mod 7

	return og + oe;
}

const int min_y = 1582;
const int max_y = 3164;

std::vector<int> getListOfYears(int easter_day,int start_year = min_y, int end_year = max_y)
{
	std::vector<int> ret; 

	for(int y= start_year ; y < end_year; ++y)
	{
		if( easter_day == getEasterSundayIndex(y))
		{
			ret.push_back(y);
		}
	}
	
	return (ret);
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

int main(int argc, char* argv[])
{
	const int min_easter_day =  22;
	const int max_easter_day  = min_easter_day+35;

	std::ofstream file;

	file.open("out.txt");

	std::map<int,std::vector<int> > easter_day_map;
	std::map<int,std::vector<int> > easter_day_range;
	std::map<int,result> minMap;
	std::map<int,std::vector<int> > easter;

	for(int y= min_y ; y < max_y; ++y)
	{

		easter[y].push_back(getEasterSundayIndex(y));
		//file << y << "  " << getEasterSundayIndex(y) <<  "\n";
	}
	 

	
	for(int e = min_easter_day ; e < max_easter_day; ++e)
	{
	 	 easter_day_map[e] = getListOfYears(e,1582,3164); //(e,1582,3164);
	}

	easter_day_range = easter_day_map;
	
	for(size_t e = min_easter_day ; e < max_easter_day; ++e)
	{
		std::adjacent_difference(easter_day_range[e].begin(),
			                     easter_day_range[e].end(), 
								 easter_day_range[e].begin());
	}

	
	int abs_min =10000;
	int abs_max = 0;
	for(size_t e = min_easter_day ; e < max_easter_day; ++e)
	{
		if(!easter_day_range[e].empty())
		{
		
		int numberOfData  = easter_day_range[e].size()-1;
		
		auto begin = easter_day_range[e].begin() +1;
		auto min = std::min_element(begin,easter_day_range[e].end());
		auto max = std::max_element(begin,easter_day_range[e].end());

		auto sum = std::accumulate(begin,easter_day_range[e].end(),0.0);
		
		double average = sum/ numberOfData;

		
		


		double  sum_diff_squared = std::accumulate(begin,easter_day_range[e].end(),0.0,[&average](int a, int b)
														  {
			                                                 return (a + pow((b-average),2.0) );
														  }
														  );

	   

		double std_dev =  sqrt( sum_diff_squared/(numberOfData -1));
		
		auto diffCubicOp = [&average](int a,int b){return( a + pow((b-average),3.0) );};

		double sum_diff_cubic = std::accumulate(begin,easter_day_range[e].end(),0.0,diffCubicOp);
		
		double skewness =sum_diff_cubic/((numberOfData-1)*pow(std_dev,3));


		double med= est::median<std::vector<int> >(begin,easter_day_range[e].end());

		
		abs_min = std::min(abs_min,*min);
		abs_max=  std::max(*max,abs_max);
		
		
		int max_ = 0;
		for(int i = 1; i < easter_day_range[e].size(); ++i)
		{
			int it = 	est::accumulate_upTo(begin,easter_day_range[e].end(),80.0,0.0);
			begin++;
						 			
			if(it > max_)
			{       
					max_ = it;
					minMap[e].index_year = i-1;
					minMap[e].occurence =it-1;
					minMap[e].year = easter_day_map[e][i-1];
			}

		}

		file << e << " " << easter_day_map[e].size() << " "  << average << " " << std_dev << " " << *min   << "  " << *max << " "<< med << " " << minMap[e].year << "\n";

int maxIndex =0;		 
int absMax =0;
				
		if( maxIndex > 0) std::cout << " day " <<  e <<  " max " << absMax  << " at year " << easter_day_map[e][maxIndex-1] << "\n";
		}	
	
	
		
		
	}

	std::cout << abs_min << " \n";
	std::cout << abs_max << " \n";

	file.close();

	return 0;
}

