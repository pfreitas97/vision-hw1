#include <math.h>
#include <stdio.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in

    return get_pixel(im,round(x),round(y),c);

}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)

	image resize = make_image(w,h,im.c);

	// We want the equiv coordinates in the old image space (Old)
	// given that we have the coordinates in the new one. (New)
	// Consider the coordinates are in the center such that [-0.5,-0.5] is
	// leftmost pixel for every image space. And the right bottom most pixel is
	// [W_max - 0.5, H_max - 0.5] for every image, (because counting from 0)
	// using these two points the equations A*Nnew + B = Old.For X,Y become:

	// a = old/new, b = 0.5*a - 0.5

	float Ax = ((float)im.w)/((float)w);
	float Bx = 0.5*Ax - 0.5;

	float Ay = ((float)im.h)/((float)h);
	float By = 0.5*Ay - 0.5;	


	for (int x = 0; x < w; ++x)
	 {
	 	for (int y = 0; y < h; ++y)
	 	{
	 		for (int c = 0; c < im.c; ++c)
	 		{
	 			float oX = Ax*x + Bx;
	 			float oY = Ay*y + By;

	 			float equiV = nn_interpolate(im,oX,oY,c);

	 			set_pixel(resize,x,y,c, equiV);

	 		}

	 	}
	 } 

    return resize;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
	// Utilizing two separate linear interpolations:
	// First finding average along y axis (2 points), then x axis.
	// NOTE distaances are inversely proportiomal to pixel contribution

	// Averages distance of smaller x value points 
	float q_1 = (ceil(y) - y)*get_pixel(im,floor(x),floor(y),c)
				+ (y - floor(y))*get_pixel(im,floor(x),ceil(y),c);


	// Averages the pixel value of the two points with larger x values
	float q_2 = (ceil(y) - y)*get_pixel(im,ceil(x),floor(y),c)
				+ (y - floor(y))*get_pixel(im,ceil(x),ceil(y),c);

	float avg = q_1*(ceil(x) - x) + q_2*(x - floor(x));


   
    return avg;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
	image resize = make_image(w,h,im.c);

	// We want the equiv coordinates in the old image space (Old)
	// given that we have the coordinates in the new one. (New)
	// Consider the coordinates are in the center such that [-0.5,-0.5] is
	// leftmost pixel for every image space. And the right bottom most pixel is
	// [W_max - 0.5, H_max - 0.5] for every image, (because counting from 0)
	// using these two points the equations A*Nnew + B = Old.For X,Y become:

	// a = old/new, b = 0.5*a - 0.5

	float Ax = ((float)im.w)/((float)w);
	float Bx = 0.5*Ax - 0.5;

	float Ay = ((float)im.h)/((float)h);
	float By = 0.5*Ay - 0.5;	


	for (int x = 0; x < w; ++x)
	 {
	 	for (int y = 0; y < h; ++y)
	 	{
	 		for (int c = 0; c < im.c; ++c)
	 		{
	 			float oX = Ax*x + Bx;
	 			float oY = Ay*y + By;

	 			float equiV = bilinear_interpolate(im,oX,oY,c);

	 			set_pixel(resize,x,y,c, equiV);

	 		}

	 	}
	 } 

    return resize;

}

