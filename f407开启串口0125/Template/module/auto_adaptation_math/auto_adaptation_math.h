#ifndef _AUTO_ADAPTATION_MATH_H
#define _AUTO_ADAPTATION_MATH_H
typedef struct
{
		float x;
		float y;
}PointF;

PointF calculateCentroid(unsigned char * grayImage, unsigned short width, unsigned short height);
PointF calculateCentre(unsigned char* grayImage, unsigned short width, unsigned short height);
unsigned int calculateTotal(unsigned char * grayImage, unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short width, unsigned short height, unsigned short* pressure_number);



#endif