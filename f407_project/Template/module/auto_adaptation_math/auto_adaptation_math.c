#include "auto_adaptation_math.h"
#include <stdio.h>
#include <stdlib.h>


//计算重心
PointF calculateCentroid(unsigned char * grayImage, unsigned short width, unsigned short height) {
    unsigned int sumX = 0, sumY = 0, totalIntensity = 0;
    for (unsigned short y = 0; y < height; y++) {
        for (unsigned short x = 0; x < width; x++) {
            unsigned short index = y * width + x;
            unsigned int intensity = grayImage[index];
						if(intensity <= 2)
							intensity = 0;
            sumX += x * intensity;
            sumY += y * intensity;
            totalIntensity += intensity;
        }
    }
		
		
    if (totalIntensity == 0) {
        return (PointF){-1,-1};
			}
    
    float centroidX = (float)sumX / totalIntensity;
    float centroidY = (float)sumY / totalIntensity;
		
		return (PointF){centroidX, centroidY};
}


//计算体心
PointF calculateCentre(unsigned char* grayImage, unsigned short width, unsigned short height)
{
		unsigned int sumX = 0, sumY = 0, totalIntensity = 0;
		for (unsigned short y = 0; y < height; y++)
		{
			for (unsigned short x = 0; x < width; x++)
			{
					unsigned short index = y * width + x;
					unsigned short intensity = grayImage[index] > 1 ? 1 : 0;
					
					sumX += x * intensity;
					sumY += y * intensity;
					totalIntensity += intensity;
			}
		}
		
		if (totalIntensity == 0)
		{
				return (PointF){-1,-1};
		}
		
		float centreX = (float)sumX / totalIntensity;
		float centreY = (float)sumY / totalIntensity;
		
		return (PointF){centreX, centreY};
}


//计算压力图某个区域的压力总和和压力个数
unsigned int calculateTotal(unsigned char * grayImage, unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short width, unsigned short height, unsigned short* pressure_number)
{
		unsigned int total_value = 0;
		unsigned short total_number = 0;
	
		for(unsigned short y = y_start; y < y_end; y++)
			for(unsigned short x = x_start; x < x_end; x++)
			{
					unsigned short index = y*width + x;
					unsigned short intensity = grayImage[index];
					total_value	+= intensity;
					total_number += (intensity > 2 ? 1 : 0);
			}
			
			*pressure_number = total_number;
			return total_value;
}


