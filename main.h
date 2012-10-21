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

#ifndef MAIN_H
#define MAIN_H

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>

#ifdef USE_SDL
    #include <SDL/SDL.h>
#endif

#ifdef DEBUG
    #include <sstream>
#endif

struct point
{
    int x, y, cluster;
};

#endif