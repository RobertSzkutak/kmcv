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
#include "util.h"
#include "main.h"

#ifdef USE_SDL
    #include "sdl.h"
#endif

#ifdef USE_HTML5
    #include "html.h"
#endif

void print_clusters(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters)
{
    std::ofstream out("output.txt");
    out << "Clusters:\n\n";
    for(int i = 0; i < clusters; i++)
    {
        out << "Cluster " << i << ":" << std::endl;
        out << "Center: (" << cluster_centersx[i] << "," << cluster_centersy[i] << ")" << std::endl;
        out << "Cluster Points:" << std::endl;
        for(int j = 0; j < numpoints; j++)
        {
            if(points[j].cluster == i)
            {
                out << "(" << points[j].x << "," << points[j].y << ")" << std::endl;
            }
        }
        out << std::endl << std::endl << std::endl;
    }
    out.close();
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
                int new_cluster_score = abs(points[i].x - cluster_centersx[j]) + abs(points[i].y - cluster_centersy[j]);
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
                    int newscore = abs(cluster_centersx[i] - points[j].x) + abs(cluster_centersy[i] - points[j].y);
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
    #ifdef WANT_CLUSTERS_PRINTED
        print_clusters(points, numpoints, cluster_centersx, cluster_centersy, clusters);
    #endif

    #ifdef USE_HTML5
        html5_visualize_clusters(points, numpoints, cluster_centersx, cluster_centersy, clusters);
    #endif

    #ifdef USE_SDL
        sdl_visualize_clusters(points, numpoints, cluster_centersx, cluster_centersy, clusters);
    #endif
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
    
    #ifdef WANT_RANDOM
        randomPoints();
    #endif

    #ifdef WANT_READIN
        readInPoints();
    #endif

    return 0;
}
