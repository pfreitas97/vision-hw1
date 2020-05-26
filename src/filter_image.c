#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{ 
    float total = 0;
    for (int i = 0; i < im.w*im.h*im.c; ++i)
    {
        if (im.data[i])
        {
            total += im.data[i];
        } else {
            total += 1;
        }
    }



    for (int i = 0; i < im.w*im.h*im.c; ++i)
    {
        if (im.data[i])
        {
            im.data[i] = (im.data[i])/total;
        } else {

            im.data[i] = (1)/total;
        }
    }


}

image make_box_filter(int w)
{
    // TODO
    image box = make_image(w,w,1);

    l1_normalize(box);

    return box;
}


float convolve_pixel(image im, image filter,int x,int y,int c){
    // Runs the operation to determine final pixel value 
    //also handles padding strategy

    float effW = ((float)filter.w) /2;    // effective height up or down
    float effH = ((float) filter.h)/2; 

    float total = 0;

    //printf("%f \n", effW);


    for (int i = -1*effW; i < effW; ++i)
    {
        for (int j = -1*effH; j < effH; ++j)
        {
            if (filter.c == 1)
            { 
               // printf("filter.c == 1, %f, %f,tot: %f \n", get_pixel(im,x + i,y + j,c), 
               //     get_pixel(filter,i + effW, j+effW,1) ,total);

                total += get_pixel(im,x + i,y + j,c)
            * get_pixel(filter,i + effW, j+effW,0);

       // printf("FILLER == 1, %f, %f,tot: %f \n", get_pixel(im,x + i,y + j,c), 
       // get_pixel(filter,i + effW, j+effW,1) ,total);


            } else {

           //     printf("Within other\n");

            total += get_pixel(im,x + i,y + j,c)
            * get_pixel(filter,i + effW, j + effW,c);

            }


        }
    }

   
  // printf("THOT NOW: %f \n" ,total);

    return total;

}


image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert( (filter.c == im.c) || (filter.c ==1) );

    image neuimage;

    if (preserve == 1)
    {
        neuimage = make_image(im.w,im.h,im.c);

        for (int x = 0; x < im.w; ++x)
        {
            for (int y = 0; y < im.h; ++y)
            {
                for (int c = 0; c < im.c; ++c)
                {

                    set_pixel(neuimage,x,y,c,convolve_pixel(im,filter,x,y,c));

                }


            }
        }






    } else {

        neuimage = make_image(im.w,im.h,1);

        for (int x = 0; x < im.w; ++x)
        {
            for (int y = 0; y < im.h; ++y)
            {

                float total = 0;

                for (int c = 0; c < im.c; ++c)
                {
                    total += convolve_pixel(im, filter, x,y,c);
                }

                set_pixel(neuimage,x,y,0,total);


            }
        }        




    }


    return neuimage;
}

image make_highpass_filter()
{
    image high = make_image(3,3,1);

    for (int x = 0; x < high.w; ++x)
    {
        for (int y = 0; y < high.h; ++y)
        {
            if (x == 1 || y == 1)
            {
                if (x == 1 && y == 1)
                {
                    set_pixel(high,x,y,0,4.0);
                } else {

                    set_pixel(high,x,y,0,-1.0);

                }
            } else {
                set_pixel(high,x,y,0,0.0);
            }
        }
    }

    return high;
}

image make_sharpen_filter()
{
    // TODO

    image sharp = make_image(3,3,1);

    for (int x = 0; x < sharp.w; ++x)
    {
        for (int y = 0; y < sharp.h; ++y)
        {
            if (x == 1 || y == 1)
            {
                if (x == 1 && y == 1)
                {
                    set_pixel(sharp,x,y,0,5.0);
                } else {

                    set_pixel(sharp,x,y,0,-1.0);

                }
            } else {
                set_pixel(sharp,x,y,0,0.0);
            }
        }
    }


    return sharp;
}

image make_emboss_filter()
{
    image emboss = make_image(3,3,1);

    set_pixel(emboss,0,0,0,-2.0);
    set_pixel(emboss,2,2,0,2.0);

    set_pixel(emboss,1,1,0,1.0);



    set_pixel(emboss,0,1,0,-1.0);
    set_pixel(emboss,1,0,0,-1.0);


    set_pixel(emboss,1,2,0,1.0);
    set_pixel(emboss,2,1,0,1.0);

    set_pixel(emboss,0,2,0,0.0);
    set_pixel(emboss,2,0,0,0.0);


    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO

    int size = ceil(6*sigma);
    if (size % 2 == 0)
    {
        size += 1;
    }

    image gauss = make_image(size,size,1);



    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            float val = (exp(-1* ((pow((float)(x-(size/2)),2) + 
                pow((float)(y-(size/2)),2))/(2*pow(sigma,2)))) )
                            / (TWOPI*pow(sigma,2));

            set_pixel(gauss,x,y,0,val);
        }
    }



    float total = 0;

    for (int i = 0; i < gauss.w*gauss.h*gauss.c; ++i)
    {
        total += gauss.data[i];
        
    }


    for (int i = 0; i < gauss.w*gauss.h*gauss.c; ++i)
    {
            gauss.data[i] = (gauss.data[i])/total;
    }


    // for (int x = 0; x < size; ++x)
    // {
    //     for (int y = 0; y < size; ++y)
    //     {
    //         printf("%f \n", get_pixel(gauss,x,y,0));
    //     }
    // }

    return gauss;
}

image add_image(image a, image b)
{
    // TODO
    //assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image neuimage = make_image(a.w,a.h,a.c);

    for (int x = 0; x < a.w; ++x)
    {
        for (int y = 0; y < a.h; ++y)
        {
            for (int c = 0; c < a.c; ++c)
            {
                float av = get_pixel(a,x,y,c);
                float bv = get_pixel(b,x,y,c);

                set_pixel(neuimage,x,y,c,av + bv);
            }
        }
    }



    return neuimage;
}

image sub_image(image a, image b)
{
    // TODO

    //assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image neuimage = make_image(a.w,a.h,a.c);

    for (int x = 0; x < a.w; ++x)
    {
        for (int y = 0; y < a.h; ++y)
        {
            for (int c = 0; c < a.c; ++c)
            {
                float av = get_pixel(a,x,y,c);
                float bv = get_pixel(b,x,y,c);

                set_pixel(neuimage,x,y,c,av - bv);
            }
        }
    }



    return neuimage;

}

image make_gx_filter()
{
    // TODO
    image neuimage = make_image(3,3,1);

    set_pixel(neuimage,0,0,0,-1);
    set_pixel(neuimage,0,1,0,-2);
    set_pixel(neuimage,0,2,0,-1);


    set_pixel(neuimage,2,0,0,1);
    set_pixel(neuimage,2,1,0,2);
    set_pixel(neuimage,2,2,0,1);


    return neuimage;
}

image make_gy_filter()
{
    // TODO
    image neuimage = make_image(3,3,1);

    set_pixel(neuimage,0,0,0,-1);
    set_pixel(neuimage,1,0,0,-2);
    set_pixel(neuimage,2,0,0,-1);


    set_pixel(neuimage,0,2,0,1);
    set_pixel(neuimage,1,2,0,2);
    set_pixel(neuimage,2,2,0,1);


    return neuimage;
}

void feature_normalize(image im)
{
    // TODO
    float min = 1;
    float max = 0;   

    for (int i = 0; i < im.w*im.h*im.c; ++i)
    {
        if (im.data[i] < min)
        {
            min = im.data[i];
        }
        if (im.data[i] > max)
        {
            max = im.data[i];
        }
    }

    float range = max - min;

    if (range == 0)
    {

        for (int i = 0; i < im.w*im.h*im.c; ++i)
        {
            im.data[i] = 0;
        }

    } else {

        for (int i = 0; i < im.w*im.h*im.c; ++i)
        {
            im.data[i] = (im.data[i] - min)/range;
        }

    }


}

image *sobel_image(image im)
{
    // TODO

    image GX = make_gx_filter();
    image GY = make_gy_filter();


    image GXim = convolve_image(im,GX,0);

    image GYim = convolve_image(im, GY, 0);

    image mag = make_image(im.w,im.h,1);

    image theta = make_image(im.w,im.h,1);


    for (int i = 0; i < GXim.w*GXim.h*GXim.c; ++i)
    {
        mag.data[i] = sqrt(pow(GXim.data[i],2) + pow(GYim.data[i],2));

        theta.data[i] = atan(GYim.data[i]/ GXim.data[i]);
    }

    image *ptrsobel = calloc(2, sizeof(image));

    memcpy(ptrsobel, &mag, sizeof(mag));
    memcpy(ptrsobel + 1, &theta, sizeof(theta));


    return ptrsobel;
}

image colorize_sobel(image im)
{
    // TODO

    image GX = make_gx_filter();
    image GY = make_gy_filter();


    image GXim = convolve_image(im,GX,0);

    image GYim = convolve_image(im, GY, 0);

    image mag = make_image(im.w,im.h,1);

    image theta = make_image(im.w,im.h,1);


    image colorSOBEL = make_image(im.w,im.h,3);

                            // GXIM.C = 1;
    for (int i = 0; i < GXim.w*GXim.h*GXim.c; ++i)
    {
        mag.data[i] = sqrt(pow(GXim.data[i],2) + pow(GYim.data[i],2));

        theta.data[i] = atan(GYim.data[i]/ GXim.data[i]);

        // hue

        //colorSOBEL.data[i] = theta.data[i];

         colorSOBEL.data[i] =  theta.data[i] < 0 ? ((theta.data[i]/6) + 1) 
                                 : (theta.data[i]/6); 

        //sat

        colorSOBEL.data[i + im.w*im.h] = mag.data[i];                        

        //value

        colorSOBEL.data[i + 2*im.w*im.h] = mag.data[i];


    }

    hsv_to_rgb(colorSOBEL);

    //feature_normalize(colorSOBEL);
    clamp_image(colorSOBEL);


    return colorSOBEL;
}
