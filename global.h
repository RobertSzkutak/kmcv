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

//kmcv options
//#define WANT_RANDOM 1 //If you want to use random points
#define WANT_READIN 1 //If you want to use points from a file

#ifdef WANT_RANDOM
    #ifdef WANT_READIN
        #error You cannot have both WANT_RANDOM and WANT_READIN defined
    #endif
#endif

//#define DEBUG 1 //Whether or not to output debug information to the terminal

//k-means options
#define CLUSTERS 4 //Number of clusters to arrange points into
#define POINTS 500 //Number of points to generate if random generation is used
#define WANT_CENTERS_DRAWN 1 //Whether or not to draw the cluster centers in the rendering of clusters
#define WANT_AXIS_DRAWN 1 //Whether or not to draw a mean axis in the rendering of clusters
#define WANT_CLUSTERS_PRINTED 1 //Whether or not to make an output file detailing the clusters, cluster centers, and clusters that each point belongs to

#define USE_SDL 1 //Whether or not to use the SDL visualizer
#define USE_HTML5 1 //Whether or not to use the HTML5 visualizer

//Visualizer options
#define WINW 2048 //Window Width
#define WINH 2048 //Window Height
#define WANT_MEAN_AXIS 1//Creates an axis where the origin of the axis is the mean of all X and Y points
#define MIRROR_Y 1 //Option to reflect points across the X axis (inverting Y value (eg. 603 == -603)) to make it look more like a traditional graph
//Support for a custom coordinate pair (X,Y) where the origin (0,0) for your generated graph lies
#define ORIGIN 1
#ifdef ORIGIN
    #define ORIGIN_X WINW / 2
    #define ORIGIN_Y WINH / 2
#endif

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
