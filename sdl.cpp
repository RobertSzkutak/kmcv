/* 
   kmcv - K-Means Clustering program and visualizer.
   Copyright (C) 2012-2013 Robert L Szkutak II - robert@robertszkutak.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or(at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "global.h"

#ifdef USE_SDL

#include "sdl.h"
#include "util.h"

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

//Without this defined the origin of the graph will be (0,0)
//#define WANT_MEAN_AXIS 1 //Creates an axis where the origin of the axis is the mean of all X and Y points

//Support for a custom SDL coordinate pair (X,Y) where the origin for your generated graph lies
#define ORIGIN 1

#ifdef ORIGIN
    #define ORIGIN_X WINW / 2
    #define ORIGIN_Y WINH / 2
#endif

SDL_Surface* screen;

void draw_points(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters);
void adjustOffset(int dir);

bool up = false, down = false, right = false, left = false;

void sdl_visualize_clusters(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WM_SetCaption("Cluster Visualization", "SDL Test");
    screen = SDL_SetVideoMode(WINW, WINH, 0, 0);

    SDL_Rect rect = {0, 0, WINW, WINH};
    SDL_FillRect(screen, &rect, WHITE);

    draw_points(points, numpoints, cluster_centersx, cluster_centersy, clusters);	

    debug("Entering input loop...");
    SDL_Event event;
    bool exit = false;

    while (!exit)
    {
        if (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:
                  exit = 1;
                  break;
                case SDL_KEYDOWN:
                  switch (event.key.keysym.sym) 
                  {
		                case SDLK_UP:
			                up = true;
			            break;
		                case SDLK_DOWN:
			                down = true;
			            break;
		                case SDLK_RIGHT:
			                right = true;
			            break;
		                case SDLK_LEFT:
			                left = true;
			            break;
                      case SDLK_ESCAPE:
                      case SDLK_q:
                        exit = 1;
                        break;
                  }
                  break;
                case SDL_KEYUP:
                  switch (event.key.keysym.sym) 
                  {
		                case SDLK_UP:
			                up = false;
			            break;
		                case SDLK_DOWN:
			                down = false;
			            break;
		                case SDLK_RIGHT:
			                right = false;
			            break;
		                case SDLK_LEFT:
			                left = false;
			            break;
                      case SDLK_ESCAPE:
                      case SDLK_q:
                        exit = 1;
                        break;
                  }
                  break;
            }
        }
        if(up == true)
            adjustOffset(UP);
        if(down == true)
            adjustOffset(DOWN);
        if(left == true)
            adjustOffset(LEFT);
        if(right == true)
            adjustOffset(RIGHT);
        if(up == true || down == true || left == true || right == true)
        {
            draw_points(points, numpoints, cluster_centersx, cluster_centersy, clusters);
        }
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
    SDL_Quit();
}

#ifdef ORIGIN
int offset_x = ORIGIN_X, offset_y = ORIGIN_Y;
#else
int offset_x = 0, offset_y = 0;
#endif
int offsetSpeed = 2;
void adjustOffset(int dir)
{
    if(dir == LEFT)
	    offset_x -= offsetSpeed;
	if(dir == RIGHT)
	    offset_x += offsetSpeed;
	if(dir == DOWN)
	    offset_y += offsetSpeed;
    if(dir == UP)
	    offset_y -= offsetSpeed;
}

void draw_points(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters)
{
  	SDL_Rect rect = {0, 0, WINW, WINH};
    SDL_FillRect(screen, &rect, WHITE);

    //Draw axis
    #ifdef WANT_AXIS_DRAWN
        #ifdef WANT_MEAN_AXIS
            int totx = 0, toty = 0;    
            for(int i = 0; i < numpoints; i++)
            {
                totx += points[i].x;
                toty += points[i].y;
            }
            debug("Drawing axis...");
            SDL_Rect xaxis = {0, toty/numpoints+offset_y, WINW, 1};
            SDL_Rect yaxis = {totx/numpoints+offset_x, 0, 1, WINH};
            SDL_FillRect(screen, &xaxis, BLACK);
            SDL_FillRect(screen, &yaxis, BLACK);
        #else
            SDL_Rect xaxis = {0, 0+offset_y, WINW, 1};
            SDL_Rect yaxis = {0+offset_x, 0, 1, WINH};
            SDL_FillRect(screen, &xaxis, BLACK);
            SDL_FillRect(screen, &yaxis, BLACK);
        #endif
    #endif

    //TODO : Generate a random color for each cluster

    debug("Plotting points...");
    //Draw points
    for(int i = 0; i < numpoints; i++)
    {
        SDL_Rect pixel = {points[i].x-2+offset_x, points[i].y-2+offset_y, 5, 5};//Offset of -3 makes center pixel of 5x5 appear at x,y 
        switch(points[i].cluster)
        {
            case 0:
              SDL_FillRect(screen, &pixel, RED);
              break;
            case 1:
              SDL_FillRect(screen, &pixel, GREEN);
              break;
            case 2:
              SDL_FillRect(screen, &pixel, BLUE);
              break;
            case 3:
              SDL_FillRect(screen, &pixel, PINK);
              break;
            default:
              SDL_FillRect(screen, &pixel, BLACK);
              break;
        }
    }

    //Draw Cluster Centers
    #ifdef WANT_CENTERS_DRAWN
        debug("Drawing cluster centers...");
        for(int i = 0; i < clusters; i++)
        {
            if(cluster_centersy[i] + offset_y - 10  >= 0 && cluster_centersy[i] + offset_y + 10  <= WINH && cluster_centersx[i] + offset_x - 10  >= 0 && cluster_centersx[i] + offset_x + 10  <= WINW)
            {//Causes a crash if a circle is drawn off the screen partially. I need a better circle function..
                if(i == 0)
                    fill_circle(screen, cluster_centersx[i]+offset_x, cluster_centersy[i]+offset_y, 10, (ALPHA | RED));
                if(i == 1)
                    fill_circle(screen, cluster_centersx[i]+offset_x, cluster_centersy[i]+offset_y, 10, (ALPHA | GREEN));
                if(i == 2)
                    fill_circle(screen, cluster_centersx[i]+offset_x, cluster_centersy[i]+offset_y, 10, (ALPHA | BLUE));
                if(i >= 3)
                    fill_circle(screen, cluster_centersx[i]+offset_x, cluster_centersy[i]+offset_y, 10, (ALPHA | PINK));
            }
        }
    #endif			
}

/*
* SDL_Surface 32-bit circle-fill algorithm without using trig
*
* While I humbly call this "Celdecea's Method", odds are that the 
* procedure has already been documented somewhere long ago.  All of
* the circle-fill examples I came across utilized trig functions or
* scanning neighbor pixels.  This algorithm identifies the width of
* a semi-circle at each pixel height and draws a scan-line covering
* that width.  
*
* The code is not optimized but very fast, owing to the fact that it
* alters pixels in the provided surface directly rather than through
* function calls.
*
* WARNING:  This function does not lock surfaces before altering, so
* use SDL_LockSurface in any release situation.
*/
void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel) {
   // Note that there is more to altering the bitrate of this 
   // method than just changing this value.  See how pixels are
   // altered at the following web page for tips:
   //   http://www.libsdl.org/intro.en/usingvideo.html
   static const int BPP = 4;

   double r = (double)radius;

   for (double dy = 1; dy <= r; dy += 1.0)
   {
       // This loop is unrolled a bit, only iterating through half of the
       // height of the circle.  The result is used to draw a scan line and
       // its mirror image below it.
       // The following formula has been simplified from our original.  We
       // are using half of the width of the circle because we are provided
       // with a center and we need left/right coordinates.
       double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));       
       int x = cx - dx;
       // Grab a pointer to the left-most pixel for each half of the circle
       Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
       Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;
               
       for (; x <= cx + dx; x++)
       {
           *(Uint32 *)target_pixel_a = pixel;
           *(Uint32 *)target_pixel_b = pixel;
           target_pixel_a += BPP;
           target_pixel_b += BPP;
       }
   }
}

#endif
