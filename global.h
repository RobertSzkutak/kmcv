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

#ifndef GLOBAL_H
#define GLOBAL_H

#define WANT_RANDOM 1 //If you want to use random points
//#define WANT_READIN 1 //If you want to use points from a file

#ifdef WANT_RANDOM
    #ifdef WANT_READIN
        #error You cannot have both WANT_RANDOM and WANT_READIN defined
    #endif
#endif

#define CLUSTERS 4 //Number of clusters to arrange points into
#define POINTS 500 //Number of points to generate if random generation is used
#define WANT_CENTERS_DRAWN 1 //Whether or not to draw the cluster centers in the rendering of clusters
#define WANT_AXIS_DRAWN 1 //Whether or not to draw a mean axis in the rendering of clusters
#define WANT_CLUSTERS_PRINTED 1 //Whether or not to make an output file detailing the clusters, cluster centers, and clusters that each point belongs to

#define USE_SDL 1 //Whether or not to use the SDL visualizer
#define USE_HTML5 1 //Whether or not to use the HTML5 visualizer

//#define DEBUG 1 //Whether or not to output debug information to the terminal

#define WINW 720 //Window Width
#define WINH 480 //Window Height

//Color constants for SDL
#define ALPHA 0xFF000000

#define WHITE 0xFFFFFF
#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF
#define BLACK 0x000000
#define PINK  0xFF00FF

struct point
{
    int x, y, cluster;
};

#endif
