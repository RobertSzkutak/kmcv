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

#define CLUSTERS 4
#define POINTS 500
//#define WANT_CENTERS_DRAWN 1
#define WANT_AXIS_DRAWN 1
#define WANT_CLUSTERS_PRINTED 1

#define USE_SDL 1

//#define DEBUG 1

#define WINW 720 //Window Width
#define WINH 480 //Window Height

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
