#ifndef getEasterDay_h__
#define getEasterDay_h__

/// calculates the date of easter sunday   as date of  march
// calculation is valid  for the  Gregorian calender 
//  examples   :
// int e =  getEasterDay(2017) ; ->  e = 47   is   47 -31 = 16th of April
// int e =  getEasterDay(
//

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
#endif // getEasterDay_h__
