#include "../header/gauss_dist.h"

#define PI 3.14159265

//realdist = the rela life ditance : we will take it 1 meter (written as 1,2 meters on paper)
//pfl = Pixel Face Length for real distance (for 1 meter, we need to measure it ourselves on average)
//found pfl = the length of the face we measure ourselves
double r1_dist(double realdist, double pfl, double foundpfl)
{
    return (realdist* foundpfl) / pfl;
}

//pfl = Pixel Face Length for real distance (for 1 meter, we need to measure it ourselves on average)
//found pfl = the length of the face in pixels that we measure ourselves 
//(here it is for the second frame face (the one after movement that is))
double r7_dist(double realdist2, double pfl2, double foundpfl2)
{
    return (realdist2* foundpfl2) / pfl2;
}

//25 = length of face in cm on average
// pfl = perspective length of the face in pixels on our measured distances //the first face 1#
// => example : at 1 meter we have 25 cm = 100 pixels
// on 1.2 meters we have 25 cm = 90 pixels (NEED TO MEASURE IT OUT OURSELVES)
double r4_dist(int x1, int y1, int x2, int y2,double pfl)
{
    double xres = 0;
    if(x1 >x2)
        xres = x1-x2;
    else
        xres = x2-x1;

    double yres = 0;
    if(y1 >y2)
        yres = y1-y2;
    else
        yres = y2-y1;

    double pixdist = sqrt((yres*yres) + (xres*xres));
    //25 cm is average face (check value!!!)
    //converted to real life
    //pfl/25.0 == for every 1 pixel we have X amount of Centimeters in real life
    //pixdist = disntace in pixels between the two points
    return pixdist * (pfl/25.0);
}

//abs return an int, cant use it since we need double
//in case point 2# is closer, r8 will become negative in case of r7-r1
//so abs value to resolve it mathematically
double r8_dist(double r7, double r1)
{
    if (r7>r1)
        return r7-r1;
    return r1-r7;
}

double r2_dist(double r8, double r4)
{
    return sqrt((r8*r8) + (r4*r4));
}

double r3_dist(double r7, double r4)
{
    return sqrt((r7*r7) + (r4*r4));
}

double biggear_angle(double realdist, double pfl, double foundpfl, double foundpfl2, int x1, int y1, int x2, int y2)
{
    double r1 = r1_dist(realdist, pfl, foundpfl);
    double r7 = r7_dist(realdist, pfl, foundpfl2);
    double r4 = r4_dist(x1, y1, x2, y2, pfl);
    double r8 = r8_dist(r7, r1);
    double r2 = r2_dist(r8,r4);
    double r3 = r3_dist(r7, r4);
    double cosi = (((r1*r1) - (r2*r2) + (r3*r3))/ (2*r1*r3));
    double val = 180.0 / PI;

    return acos(cosi) * val;
}

//pfl_2 = pixel face length for point 2#
double smallgear_angle(double r2, int y1, int y2, double pfl_2)
{
    double yres = (double)(abs(y2 - y1));
    double gaussdist = pfl_2 *25.0 /yres;
    double sinu = sin(gaussdist/r2);

    double val = 180.0 / PI;
    return asin(sinu) * val;
}

//fpl = Face Pixel length ; ammout of pixels length wise of a face
//coord of the two centers of 2 points of interest
double gauss_dist(double fpl, int x1, int y1, int x2, int y2)
{
    //int list[] = {(radius of detection, length in pixel of a face) * number of how many circle radii};
    double xres = 0;
    if(x1 >x2)
        xres = x1-x2;
    else
        xres = x2-x1;

    double yres = 0;
    if(y1 >y2)
        yres = y1-y2;
    else
        yres = y2-y1;

    double pixdist = 0;
    if(yres > xres)
        pixdist = yres;
    else
        pixdist = xres;
    //25 cm is average face (check value!!!)
    //converted to real life
    return (pixdist * 25.0) / fpl; 
}

/*
---------------> X
|
|
Y*/
 //x1 y 1 center coord of image
//x2 y 2 center coord of second frame
//dist = real life dist : deduced from pixel length in a set array
//fpl : you already know.....
//fuck arrays, feed it two var, biggear and small gear
void find_angle(int x1, int y1, int x2, int y2, double fpl, double dist\
,double* biggear, double *smallgear)
{
    //beta dist = distance betwen orthog projec of second frame and center updown, so (x2,Y1)
    double betadist = gauss_dist(fpl, x1, y1, x2, y1);
    double betaangle = tan(betadist/ dist);
    *biggear = atan(betaangle);

    //alphadist = dist between orthog projec of second frame and center sidwares
    double alphadist = gauss_dist(fpl, x1, y1, x1, y2);
    double alphaangle = tan(alphadist);
    *smallgear= atan(alphaangle);


}