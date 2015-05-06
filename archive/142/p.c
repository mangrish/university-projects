/*****************************************************************************/
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*
**  Program : PROJECT A: Gold - plating a football, by calculating surface area
**  For     : 433 -142: Computing fundamentals B;
**            PROJECT A
**  Written : Mark Angrish, Student number: 74592
**            January 1999;
**  Input   : From stdin: diameter and width, num1, and num2;
**  Output  : To stdout: The total surace area of the football , and the width
**            of a football of 20cm diameter and TSA of 1600 sq.cm.
*/
/*---------------------------------------------------------------------------*/
 
/*****************************************************************************/
/*****************************************************************************/



#include <stdio.h>
#include <math.h>



#define VAL 10000
#define PI 3.141592654
#define DIAM 20.00
#define AREA 1600.00
#define EPS 0.0001



/*
** FUNCTION PROTOTYPES
*/

void getnum( double *, double * );
double total_area( double , double );
double seg_area( double , double, int  );
void print_area( double, double, double );
double search( double , double );
void print_partc(  double );


int main( void )
/**************/

{
    double num1, num2, surface_area, partc;
    double lo = 0.00;
    double hi = 50.00;

    getnum( &num1, &num2 );
    surface_area = total_area( 0.5*num1, 0.5*num2 ); 
    print_area( num1, num2, surface_area );
    partc = search( lo, hi );
    print_partc( partc );
    
    return 0;
}



/*
** getnum: returns the values for the specifications 
**         of the total surface area
*/

void getnum( double *num1, double *num2 )
/***************************************/

{  
    printf( "Enter the length of the football ( in cm ): \n");
    scanf( "%lf",num1);
    printf( "Enter the diameter of the football ( in cm ): \n");
    scanf( "%lf",num2);
    
    return;
}



/*
** total_area: returns the accumulated total of all the individual
**             segment total surface area
*/

double total_area( double n1, double n2 )
/***************************************/

{
    double tot_area = 0;
    int i ; 
    for ( i = 0; i < VAL; i++) 
    {
      tot_area += seg_area( n1, n2, i );
    }

    return tot_area;
}



/*
** seg_area: calculates the total surface area of an individual 
**           interval( which is a cylinder)
*/

double seg_area( double n2, double n1, int i )
/********************************************/

{

    double x_val1, x_val2, y_val1, y_val2, mid_y1, diag, seg_area1 ;


    x_val1 = ( (double) i / VAL ) * n1;
    x_val2 = ( (double) (i + 1) / VAL ) * n1;

    y_val1 = sqrt( pow( n2, 2 ) * ( 1 - ( pow( x_val1, 2 ) / pow( n1, 2 ))));
    y_val2 = sqrt( pow( n2, 2 ) * ( 1 - ( pow( x_val2, 2 ) / pow( n1, 2 )))); 
    
    mid_y1 = (y_val1 + y_val2) / 2;
    
    diag = sqrt( (pow( (n1 / VAL ), 2 ) + pow( (y_val2 - y_val1), 2 )));
   
    seg_area1 = (2 * PI * mid_y1 * diag);
    
    return seg_area1;

}



/*
** print_area: outputs the value of the total surface area of the given input
*/

void print_area( double num1, double num2, double surface_area )
/**************************************************************/

{
   {
   printf( "The total suface area is of a football with length %.2f cm and\n" , num1 );
   printf( "diameter %.2f is %.2f sq cm!!! \n\n" , num2, ( 2 *surface_area) ); 
   }
   return;
}



/*
** search: for part c, it calculates the required length for a football of 
**         diameter 20cm and total surface area 1600 sq cm
*/

double search( double lo, double hi )
/***********************************/

{
    double mid; 
    mid = (lo + hi) / 2;

	
	 if ( total_area( 0.5* DIAM, 0.5* mid ) < ((AREA/2) - EPS) )
         {
         return search( mid,hi );
         } 
	 else if ( total_area(  0.5 * DIAM ,0.5 * mid) > ((AREA/2) + EPS) )
         {
         return search( lo,mid );
         }

    return mid;
}



/*
** print_partc : outputs the value of search(part c)
*/

void print_partc( double partc )
/******************************/

{
    {
    printf( "For a football with diameter %.2f cm and total surface\n", DIAM); 
    printf( "area %.2f sq. cm, the length is %.2f cm.\n\n", AREA, partc);
    }

    return;
}








  
