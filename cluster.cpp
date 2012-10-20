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

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>

#include <SDL/SDL.h>

//#define DEBUG 1

#ifdef DEBUG
    #include <sstream>
#endif

#define WINW 720
#define WINH 480

#define ALPHA 0xFF000000

#define WHITE 0xFFFFFF
#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF
#define BLACK 0x000000
#define PINK  0xFF00FF

#define CLUSTERS 4
#define POINTS 500
//#define WANT_CENTERS_DRAWN 1

struct point
{
    int x, y, cluster;
};

int uabs(int n)
{
    if(n < 0)
        return n * -1;
    else
        return n;
}

void debug(std::string s)
{
    #ifdef DEBUG
        std::cout << s << std::endl;     
    #endif
}

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
       //debug("Caclulating dx..");
       double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
       //debug("Calculating x..");       
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

void visualize_cluster(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters)
{
    int maxw = 0, lw = WINW;    
    for(int i = 0; i < numpoints; i++)
    {
        if(points[i].x > maxw)
            maxw = points[i].x;
        if(points[i].x < lw)
            lw = points[i].x;
    }
    int maxh = 0, lh = WINH;   
    for(int i = 0; i < numpoints; i++)
    {
        if(points[i].y > maxh)
            maxh = points[i].y;
        if(points[i].y < lh)
            lh = points[i].y;
    }

    #ifdef DEBUG
        std::ostringstream convert1, convert2;
        convert1 << maxw;
        convert2 << maxh;
        debug("Maxw: " + convert1.str() + " Maxh: " + convert2.str());
    #endif
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WM_SetCaption("Cluster Visualization", "SDL Test");
    SDL_Surface* screen = SDL_SetVideoMode(WINW, WINH, 0, 0);

    SDL_Rect rect = {0, 0, WINW, WINH};
    SDL_FillRect(screen, &rect, WHITE);

    debug("Drawing axis...");
    //Draw axis
    SDL_Rect xaxis = {0, (maxh+lh)/2, WINW, 1};
    SDL_Rect yaxis = {(maxw+lw)/2, 0, 1, WINH};
    SDL_FillRect(screen, &xaxis, BLACK);
    SDL_FillRect(screen, &yaxis, BLACK);

    //TODO : Generate a random color for each cluster

    debug("Plotting points...");
    //Draw points
    for(int i = 0; i < numpoints; i++)
    {
        SDL_Rect pixel = {points[i].x+2, points[i].y+2, 5, 5};
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
            if(cluster_centersy[i] - 10  >= 0 && cluster_centersy[i] + 10  <= WINH && cluster_centersx[i] - 10  >= 0 && cluster_centersx[i] + 10  <= WINW)
            {//Causes a crash if a circle is drawn off the screen partially. I need a better circle function..
                if(i == 0)
                    fill_circle(screen, cluster_centersx[i], cluster_centersy[i], 10, (ALPHA | RED));
                if(i == 1)
                    fill_circle(screen, cluster_centersx[i], cluster_centersy[i], 10, (ALPHA | GREEN));
                if(i == 2)
                    fill_circle(screen, cluster_centersx[i], cluster_centersy[i], 10, (ALPHA | BLUE));
                if(i >= 3)
                    fill_circle(screen, cluster_centersx[i], cluster_centersy[i], 10, (ALPHA | PINK));
            }
        }
    #endif

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
                      case SDLK_ESCAPE:
                      case SDLK_q:
                        exit = 1;
                        break;
                  }
                  break;
            }
        }
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
    SDL_Quit();
}

void cluster(int clusters, int * pointsx, int * pointsy, int numpoints)
{
    //Create points
    debug("Creating points...");
    point points[numpoints];
    for(int i = 0; i < numpoints; i++)
    {
        points[i].x = pointsx[i];
        points[i].y = pointsy[i];
        points[i].cluster = 0;        
    }

    //Find max width of graph
/*
    debug("Getting with of points...");
    int maxw = 0;    
    for(int i = 0; i < numpoints; i++)
    {
        if(pointsx[i] > maxw)
            maxw = pointsx[i];
    }

    //Find max height of graph
    debug("Getting height of points...");
    int maxh = 0;
    for(int i = 0; i < numpoints; i++)
    {
        if(pointsy[i] > maxh)
            maxh = pointsy[i];
    }
    */
    //Randomly generate cluster centers
    //TODO : Some kind of error handling so cluster centers can't be the same point
    //TODO : Maybe better initial cluster center generation -->
    /*
        Idea : Calculate origin and then choose centers by moving in the four diagonals from the origin.
               This wouldn't work with truly random datasets but since this is where we predict the clusters will be ...
    */
    debug("Picking initial cluster centers...");
    int cluster_centersx[clusters], cluster_centersy[clusters], cluster_count[clusters];
    for(int i = 0; i < clusters; i++)
    {
        cluster_centersx[i] = rand() % WINW + 1;
        cluster_centersy[i] = rand() % WINH + 1;
        cluster_count[i] = 0;
    }

    bool exit = false;

    debug("Starting clustering algorithm...");
    while(exit == false)
    {
        exit = true;

        //Assign points to a cluster
        for(int i = 0; i < numpoints; i++)
        {
            int cluster_score = WINW * WINH;//Something so high it could never happen

            for(int j = 0; j < clusters; j++)
            {
                debug("Computing new cluster score...");
                int new_cluster_score = uabs(points[i].x - cluster_centersx[j]) + uabs(points[i].y - cluster_centersy[j]);
                if(new_cluster_score < cluster_score)
                {
                    cluster_score = new_cluster_score;
                    points[i].cluster = j;
                }   
            }
        }

        //Recalculate cluster centers. If they don't change, exit the loop
        for(int i = 0; i < clusters; i++)
        {
            int all_x = 0, all_y = 0;
            cluster_count[i] = 0;
            for(int j = 0; j < numpoints; j++)
            {
                if(points[j].cluster == i)
                {
                    all_x += points[j].x;
                    all_y += points[j].y;
                    cluster_count[i]++;
                }
            }

            debug("Computing new centers for clusters...");
            if(cluster_count[i] > 0)
            {
                int new_center_x = (int) all_x / cluster_count[i];
                int new_center_y = (int) all_y / cluster_count[i];

                if(cluster_centersx[i] != new_center_x || cluster_centersy[i] != new_center_y)
                {
                    cluster_centersx[i] = new_center_x;
                    cluster_centersy[i] = new_center_y;
                    exit = false;//The loop will repeat
                }
            }
            else//Make cluster center be the point closest to the cluster center if no points are contained in the cluster
            {
                int score = WINW * WINH;
                int place = 0;
                for(int j = 0; j < numpoints; j++)
                {
                    int newscore = uabs(cluster_centersx[i] - points[j].x) + uabs(cluster_centersy[i] - points[j].y);
                    if(newscore < score)
                    {
                        bool hack = false;
                        for(int x = 0; x < clusters; x++)
                            if(cluster_count[x] == 1 && points[j].cluster == x)
                                hack = true;
                        if(hack = false)//if no other clusters contain only this point
                        {
                            score = newscore;
                            place = j;
                            cluster_count[points[j].cluster]--;
                        }
                    }
                }
                cluster_centersx[i] = points[place].x;
                cluster_centersy[i] = points[place].y;
                points[place].cluster = i;
            }
        }
    }
    visualize_cluster(points, numpoints, cluster_centersx, cluster_centersy, clusters);
}

void randomPoints()
{
    int clusters = CLUSTERS;
    int numpoints = POINTS;
    int pointsx[numpoints], pointsy[numpoints];

    for(int i = 0; i < numpoints; i++)
    {
        pointsx[i] = rand() % WINW + 1;
        pointsy[i] = rand() % WINH + 1;
    }

    debug("Moving into clustering ...");

    cluster(clusters, pointsx, pointsy, numpoints);
}

void readInPoints()
{
    //TODO: Figure out how to not have to use two ifstreams
    int pif = 0;//points in file

    std::string line;
    std::ifstream file1 ("points.txt");

    if(file1.is_open())
    {
        while(file1.good())
        {
            getline(file1, line);
            pif++;
        }
        file1.close();
    }
    else
    {
        randomPoints();
        return;
    }

    int pointsx[pif], pointsy[pif];

    std::ifstream file ("points.txt");

    int count = 0;

    debug("All good at here...");

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            size_t comma = line.find(",");
            int pos = int(comma);
            std::string x = line.substr(0, pos);
            std::string y = line.substr(pos+1);
            pointsx[count] = atoi(x.c_str());
            pointsy[count] = atoi(y.c_str());        
            count++;
        }
        file.close();
    }

    int clusters = CLUSTERS;
    cluster(clusters, pointsx, pointsy, pif);
}

int main (int argc, char *argv[])
{
    srand(time(0));
    
    //randomPoints();

    readInPoints();

    return 0;
}
