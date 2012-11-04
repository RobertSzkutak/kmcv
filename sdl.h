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

#ifndef KMCV_SDL
#define KMCV_SDL

#include "global.h"

#ifdef USE_SDL

#include <SDL/SDL.h>
#include <math.h>

void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel);

void sdl_visualize_clusters(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters);

#endif
#endif
