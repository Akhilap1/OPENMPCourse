# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <cmath>
# include <ctime>
# include <cstring>
#include <sstream>
#include <complex>
# include <omp.h>

//compile using  g++ -std=c++11  mandelbrot_serial1.cpp -o mandelbrot_serial1.o
//
using namespace std;

int main ( );
void timestamp ( void );
int value(int x, int y);
void initialPrints();
int processFloatPPMS();
int processDoublePPMS();
int valueFloat (int x, int y, float XMAX, float XMIN, float YMAX, float YMIN, float width, float height)  ;

float width = 600;
float height = 600; 
float XMIN=-2;
float XMAX=-0.75;
float YMIN=-1;
float YMAX=1;
int MAXITERPER_X_Y = 200;
int MAX_PPM_FILES = 100;
double wtime;

double Dwidth = 600;
double Dheight = 600; 
double DXMIN=-2;
double DXMAX=-0.75;
double DYMIN=-1;
double DYMAX=1;
double DXLEN = 0;
double DYLEN = 0;

void initialPrints()
{

  timestamp ( );
  cout << "\n";
  cout << "MANDELBROT_OPENMP\n";
  cout << "  C++/OpenMP version\n";
  cout << "\n";
  cout << "  Create an ASCII PPM image of the Mandelbrot set.\n";
  cout << "\n";
  cout << "  For each point C = X + i*Y\n";
  cout << "  with X range [" << XMIN << "," << XMAX << "]\n";
  cout << "  and  Y range [" << YMIN << "," << YMAX << "]\n";
  cout << "  carry out " << MAXITERPER_X_Y << " iterations of the map\n";
  cout << "  Z(n+1) = Z(n)^2 + C.\n";
  cout << "  If the iterates stay bounded (norm less than 2)\n";
  cout << "  then C is taken to be a member of the set.\n";
  cout << "\n";
  cout << "  An ASCII PPM image of the set is created using\n";
  cout << "    M = " << width << " pixels in the X direction and\n";
  cout << "    N = " << height << " pixels in the Y direction.\n";

  wtime = omp_get_wtime ( );


}
void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}

int valueFloat (int x, int y, float XMAX, float XMIN, float YMAX, float YMIN, float width, float height)  {
    complex<float> point((float)x*(XMAX-XMIN)/width+ XMIN, (float)y*(YMAX-YMIN)/height+YMIN);
    complex<float> z(0, 0);
    int nb_iter = 0;
    while (abs (z) < 2 && nb_iter <= MAXITERPER_X_Y) {
        z = z * z + point;
        nb_iter++;
    }
    if (nb_iter < MAXITERPER_X_Y)
       return nb_iter;
       //return (255*nb_iter)/200;
    else 
       return 0;
}

int value (int x, int y)  {
    complex<float> point((float)x*(XMAX-XMIN)/width+ XMIN, (float)y*(YMAX-YMIN)/height+YMIN);
    complex<float> z(0, 0);
    int nb_iter = 0;
    while (abs (z) < 2 && nb_iter <= MAXITERPER_X_Y) {
        z = z * z + point;
        nb_iter++;
    }
    if (nb_iter < MAXITERPER_X_Y)
       return nb_iter;
       //return (255*nb_iter)/200;
    else 
       return 0;
}


//****************************************************************************80
//
//  Purpose:
//  Zooms in to the point (-0.743643887037158,  0.1318259042053119)
//  adjusting the boundary values of XMAX, XMIN and YMAX and YMIN for every 
//  zoom iteration. Each iteration generates a new PPM files named as 
//  "mandelbrot<ITER_number>.PPM"
//****************************************************************************
int processFloatPPMS()
{

	float xmid = -0.743643887037158;
	//704752191506114774
	float ymid =  0.1318259042053119;
	//70493132056385139
    float XLEN = XMAX - XMIN;
	float YLEN = YMAX - YMIN;

   # pragma omp parallel 
	{
		width = 600;
		height = 600; 
		XMIN=-2;
		XMAX=-0.75;
		YMIN=-1;
		YMAX=1;
		MAXITERPER_X_Y = 200;
		XLEN = XMAX - XMIN;
		YLEN = YMAX - YMIN;
		#pragma omp for \
		private (XMAX, XMIN, YMAX, YMIN) 
		for (int iters =0; iters <MAX_PPM_FILES; iters++)
		{
		    ofstream my_Image1;
			float pXLEN = XLEN;
            float pYLEN = YLEN;
			pXLEN = XLEN*pow(0.9, iters); //zoom in
			pYLEN = YLEN*pow(0.9, iters);

	   		XMAX= xmid + pXLEN/2;
			XMIN= xmid - pXLEN/2;
			YMAX = ymid + pYLEN/2;
			YMIN = ymid - pYLEN/2;

			std::ostringstream o;
			o<< "mandelbrot_float"<< iters <<".ppm";
			string filename = o.str();
			cout << o.str() << omp_get_thread_num() << iters <<"\n";

		    std::ostringstream contents;
			contents << "P3\n" << width << " " << height << " 255\n";		
		    	for (int i = 0; i < width; i++) {
		        		for (int j = 0; j < height; j++)  {
							int val = valueFloat(i, j, XMAX, XMIN, YMAX, YMIN,width, height);
								contents << (255*val)/255 <<' ' << (255*200*val)/255 <<' '<< (255*100*val)/255 <<"\n";
		        			}
		    	}

			my_Image1.open(filename);
			if (my_Image1.is_open ()) {
		    	my_Image1 << contents.str();
			}
			else
		 	 cout << "Could not open the file";

		    my_Image1.close();
			contents.clear();
		}
	}


	return 0;
}

//****************************************************************************80
//
//  Purpose: Using double precision
//  Zooms in to the point (-0.743643887037158704752191506114774,  0.131825904205311970493132056385139)
//  adjusting the boundary values of XMAX, XMIN and YMAX and YMIN for every 
//  zoom iteration. Each iteration generates a new PPM files named as 
//  "mandelbrot<ITER_number>.PPM"
//****************************************************************************
int processDoublePPMS()
{

	double xmid = -0.743643887037158704752191506114774;
	double ymid =  0.131825904205311970493132056385139;
    
    ofstream my_Image1;
    double DXLEN = DXMAX - DXMIN;
    double DYLEN = DYMAX - DYMIN;

    for (int iters =1; iters <MAX_PPM_FILES; iters++)
    {
    
   		DXMAX= xmid + DXLEN/2;
    	DXMIN= xmid - DXLEN/2;
    	DYMAX = ymid + DYLEN/2;
    	DYMIN = ymid - DYLEN/2;
 
		std::ostringstream o;
		o<< "mandelbrot_double"<< iters <<".ppm";
		string filename = o.str();
    	my_Image1.open(filename);
    	if (my_Image1.is_open ()) {
        	my_Image1 << "P3\n" << width << " " << height << " 255\n";
        	for (int i = 0; i < width; i++) {
            		for (int j = 0; j < height; j++)  {
                		int val = value(i, j);
                		//my_Image1<< val << ' ' << 0 << ' ' << 0 << "\n";
				my_Image1 << (255*val)/255 <<' ' << (255*200*val)/255 <<' '<< (255*100*val)/255 <<"\n";
            			}
        	}
        	my_Image1.close();
    	}
    	else
     	 cout << "Could not open the file";

		DXLEN = DXLEN*0.9; //zoom in
		DYLEN = DYLEN*0.9;
    }



	return 0;
}


//****************************************************************************80
//
//  Purpose:
//  generates a single PPM as perliminary visual of the mandelbrot set
//  then zooms in to the point (-0.743643887037158,  0.1318259042053119)
//  adjusting the boundary values of XMAX, XMIN and YMAX and YMIN for every 
//  zoom iteration. Each iteration generates a new PPM files named as 
//  "mandelbrot<ITER_number>.PPM"
//****************************************************************************
int main ()  {

    initialPrints();
    ofstream my_Image ;
    my_Image.open("mandelbrot.ppm");


    if (my_Image.is_open ()) {
        my_Image << "P3\n" << width << " " << height << " 255\n";
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++)  {
                int val = value(i, j);
		my_Image << (255*val)/255 <<' ' << (255*200*val)/255 <<' '<< (255*100*val)/255 <<"\n";
//                my_Image << val << ' ' << 0 << ' ' << 0 << "\n";
            }
        }
        my_Image.close();
        cout << "Written PPM: mandelbrot.ppm \n";
    }
    else
      cout << "Could not open the file";


	processFloatPPMS();
	cout << "Number of PPM files generated:" << MAX_PPM_FILES <<"\n";
	wtime = omp_get_wtime ( ) - wtime;
	cout << "\n";
	cout << "  Time = " << wtime << " seconds.\n";

	wtime = omp_get_wtime();
	//reset initial values
	width = 600;
	height = 600; 
	XMIN=-2;
	XMAX=-0.75;
	YMIN=-1;
	YMAX=1;
	MAXITERPER_X_Y = 200;
	DXLEN = XMAX - XMIN;
	DYLEN = YMAX - YMIN;
	 // processDoublePPMS();
	  cout << "Number of PPM files generated:" << MAX_PPM_FILES <<"\n";
	  wtime = omp_get_wtime ( ) - wtime;
	  cout << "\n";
	  cout << "  Time = " << wtime << " seconds.\n";

  return 0;
}
