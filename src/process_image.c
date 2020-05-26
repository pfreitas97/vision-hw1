#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"


int coordinateCheck(int proposed, int actual)
{



    if ( proposed < 0)
    {

        return 0;
    }

    if (proposed >= actual)
    {
        return actual - 1;
    }

    return proposed;


}


float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in


    x =  coordinateCheck(x,im.w);

    y = coordinateCheck(y,im.h);

    c = coordinateCheck(c,im.c);

    return im.data[x + y*im.w + c*im.w*im.h];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in

    x =  coordinateCheck(x,im.w);

    y = coordinateCheck(y,im.h);

    c = coordinateCheck(c,im.c);

    im.data[x + y*im.w + c*im.w*im.h] = v;

}

image copy_image(image im)
{
/*    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in

    memcpy(&copy,&im,sizeof(copy));

    return copy;*/
    
    image copy = make_image(im.w, im.h, im.c);
    for(int i = 0; i < im.h*im.w*im.c; i++){

        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int w = 0; w < im.w; ++w)
    {
        for (int h = 0; h < im.h; ++h)
        {
            for (int c = 0; c < im.c; ++c)
            {

                float R = im.data[w + h*im.w + 0*im.w*im.h];

                float G = im.data[w + h*im.w + 1*im.w*im.h];

                float B = im.data[w + h*im.w + 2*im.w*im.h];

                gray.data[w + h*im.w] =  
                0.299*R + 0.587*G + 0.114*B;


            }
        }


    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int w = 0; w < im.w; ++w)
    {
        for (int h = 0; h < im.h; ++h)
            {
                set_pixel(im,w,h,c, get_pixel(im,w,h,c) + v);
            }   

     }
/*    for(int i = 0; i < im.h*im.w; i++){
        im.data[(c*im.h*im.w)+i] += v;
    }*/
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.w*im.h*im.c; ++i)
    {
        if (im.data[i] < 0)
        {
            im.data[i] = 0;   
        }
        if (im.data[i] > 1)
        {
            im.data[i] = 1;
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int w = 0; w < im.w; ++w)
    {
        for (int h = 0; h < im.h; ++h)
        {
            float V = three_way_max(get_pixel(im,w,h,0),
                get_pixel(im,w,h,1),get_pixel(im,w,h,2));

            float m = three_way_min(get_pixel(im,w,h,0),
                get_pixel(im,w,h,1),get_pixel(im,w,h,2));

            float C = V - m;

            float S = 0.0;

            if(V != 0) S = C / V;

            float Hp = 0.0;

            float R = get_pixel(im,w,h,0);

            float G = get_pixel(im,w,h,1);

            float B = get_pixel(im,w,h,2);

            if ((V == R) && C != 0)
            {
                Hp = (G - B) / C;
            } else if(V == G && C != 0) {
                Hp = ((B - R) / C) + 2;
            } else if (V == B && C != 0) 
            {
                Hp = ((R - G) / C) + 4;   
            }

            float H = 0.0;

            if (Hp < 0)
            {
                H = (Hp/6) + 1;
            } else {

                H = (Hp/6);

            }
            set_pixel(im, w, h, 0, H);
            set_pixel(im, w, h, 1, S);
            set_pixel(im, w, h, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
   for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float h = get_pixel(im, x, y, 0);
            float s = get_pixel(im, x, y, 1);
            float v = get_pixel(im, x, y, 2);

            float r = 0;
            float g = 0;
            float b = 0;

            if (s == 0)
            {
                r = 0;
                b = 0;
                g = 0;
            }

            float i = trunc(h * 6);
            float f = (h * 6) - i;
            float p = v * (1 - s);
            float q = v * (1 - s * f);
            float t = v * (1 - s * (1 - f));

            if (i == 0)
            {
                r = v;
                g = t;
                b = p;
            }

            if (i == 1)
            {
                r = q;
                g = v;
                b = p;
            }

            if (i == 2)
            {
                r = p;
                g = v;
                b = t;
            }

            if (i == 3)
            {
                r = p;
                g = q;
                b = v;
            }

            if (i == 4)
            {
                r = t;
                g = p;
                b = v;
            }

            if (i == 5)
            {
                r = v;
                g = p;
                b = q;
            }

            set_pixel(im, x, y, 0, r);
            set_pixel(im, x, y, 1, g);
            set_pixel(im, x, y, 2, b);
        }
    }
}
