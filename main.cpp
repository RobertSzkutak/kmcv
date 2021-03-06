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
    int clustersn[clusters];
    
    //Print cluster data
    out << "Clusters:\n\n";
    for(int i = 0; i < clusters; i++)
    {
        clustersn[i] = 0;
        out << "Cluster " << i << ":" << std::endl;
        out << "Center: (" << cluster_centersx[i] << "," << cluster_centersy[i] << ")" << std::endl;
        out << "Cluster Points:" << std::endl;
        for(int j = 0; j < numpoints; j++)
        {
            if(points[j].cluster == i)
            {
                out << "(" << points[j].x << "," << points[j].y << ")" << std::endl;
                clustersn[i]++;
            }
        }
        out << std::endl << std::endl << std::endl;
    }
    
    //Print cluster statistics
    for(int i = 0; i < clusters; i++)
    {
        out << "Total points in cluster " << i << ": " << clustersn[i] << std::endl;
        out << "Chance of belonging to cluster " << i << ": " << (double) clustersn[i] / numpoints << std::endl;
    }

    //Print data statistics
    #ifdef WANT_SPLIT_AXIS//Yeah, this is kind of a hack of something different. Conference driven development is forcing me to be ugly right now.
        int avgx = 0, avgy = 0, greatx=points[0].x, lowx=points[0].x, greaty=points[0].y, lowy=points[0].y;
        for(int i = 0; i < numpoints; i++)
        {
            if(points[i].x > greatx)
                greatx = points[i].x;
            if(points[i].x < lowx)
                lowx = points[i].x;
            if(points[i].y > greaty)
                greaty = points[i].y;
            if(points[i].y < lowy)
                lowy = points[i].y;
        }
        avgx = (greatx + lowx) / 2;
        avgy = (greaty + lowy) / 2;
        out << std::endl << "GreatX: " << greatx << "LowX: " << lowx << "GreatY: " << greaty << "LowY: " << lowy;
        out << std::endl << "Split of data: (" << avgx << "," << avgy << ")" << std::endl << std::endl;
    #else
        int avgx = 0, avgy = 0;
        for(int i = 0; i < numpoints; i++)
        {
            avgx += points[i].x;
            avgy += points[i].y;
        }
        avgx /= numpoints;
        avgy /= numpoints;
        out << std::endl << "Mean of data: (" << avgx << "," << avgy << ")" << std::endl << std::endl;
    #endif
    
    int ul = 0, uls = 0, ur = 0, urs = 0, ll = 0, lls = 0, lr = 0, lrs = 0;
    for(int i = 0; i < numpoints; i++)
    {
        //Find the upper leftmost point
        if(points[i].x < avgx && points[i].y < avgy && uls < abs(points[i].x - avgx) + abs(points[i].y - avgy))
        {
            ul = i;
            uls = abs(points[i].x - avgx) + abs(points[i].y - avgy);
        }
        //Find the upper rightmost point
        if(points[i].x > avgx && points[i].y < avgy && urs < abs(points[i].x - avgx) + abs(points[i].y - avgy))
        {
            ur = i;
            urs = abs(points[i].x - avgx) + abs(points[i].y - avgy);
        }
        //Find the lower leftmost point
        if(points[i].x < avgx && points[i].y > avgy && lls < abs(points[i].x - avgx) + abs(points[i].y - avgy))
        {
            ll = i;
            lls = abs(points[i].x - avgx) + abs(points[i].y - avgy);
        }
        //Find the lower rightmost point
        if(points[i].x > avgx && points[i].y > avgy && lrs < abs(points[i].x - avgx) + abs(points[i].y - avgy))
        {
            lr = i;
            lrs = abs(points[i].x - avgx) + abs(points[i].y - avgy);
        }
    }

    #ifdef MIRROR_Y
        out << "The lower leftmost point is (" << points[ul].x << "," << points[ul].y << ")" << std::endl;
        out << "The lower rightmost point is (" << points[ur].x << "," << points[ur].y << ")" << std::endl;
        out << "The upper leftmost point is (" << points[ll].x << "," << points[ll].y << ")" << std::endl;
        out << "The upper rightmost point is (" << points[lr].x << "," << points[lr].y << ")" << std::endl << std::endl;
    #else
        out << "The upper leftmost point is (" << points[ul].x << "," << points[ul].y << ")" << std::endl;
        out << "The upper rightmost point is (" << points[ur].x << "," << points[ur].y << ")" << std::endl;
        out << "The lower leftmost point is (" << points[ll].x << "," << points[ll].y << ")" << std::endl;
        out << "The lower rightmost point is (" << points[lr].x << "," << points[lr].y << ")" << std::endl << std::endl;
    #endif

    //Print Quadrant Statistics
    int q1, q2, q3, q4;
    q1=q2=q3=q4=0;
    for(int i = 0; i < numpoints; i++)
    {
        //Upper left (quadrant 1)
        if(points[i].x < avgx && points[i].y < avgy)
        {
            //out << "(" << points[i].x << "," << points[i].y << ")" << " is in quadrant 1 (lower left)\n";
            q1++;
        }

        //Upper right (quadrant 2)
        if(points[i].x > avgx && points[i].y < avgy)
            q2++;
        //Lower right (quadrant 3)
        if(points[i].x > avgx && points[i].y > avgy)
            q3++;
        //Lower left (quadrant 4)
        if(points[i].x < avgx && points[i].y > avgy)
            q4++;
    }

    #ifdef MIRROR_Y
        out << "Number of points in quadrant 1 (lower left): " << q1 << std::endl;
        out << "Chance of landing in quadrant 1: " << (double) q1 / numpoints << std::endl;
	    out << "Number of points in quadrant 2 (lower right): " << q2 << std::endl;
        out << "Chance of landing in quadrant 2: " << (double) q2 / numpoints << std::endl;
	    out << "Number of points in quadrant 3 (upper right): " << q3 << std::endl;
        out << "Chance of landing in quadrant 3: " << (double) q3 / numpoints << std::endl;
	    out << "Number of points in quadrant 4 (upper left): " << q4 << std::endl;
        out << "Chance of landing in quadrant 4: " << (double) q4 / numpoints << std::endl;
    #else
        out << "Number of points in quadrant 1 (upper left): " << q4 << std::endl;
        out << "Chance of landing in quadrant 1: " << (double) q4 / numpoints << std::endl;
	    out << "Number of points in quadrant 2 (upper right): " << q3 << std::endl;
        out << "Chance of landing in quadrant 2: " << (double) q3 / numpoints << std::endl;
	    out << "Number of points in quadrant 3 (lower right): " << q2 << std::endl;
        out << "Chance of landing in quadrant 3: " << (double) q2 / numpoints << std::endl;
	    out << "Number of points in quadrant 4 (lower left): " << q1 << std::endl;
        out << "Chance of landing in quadrant 4: " << (double) q1 / numpoints << std::endl;
    #endif

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
        #ifdef WANT_RANDOM
            cluster_centersx[i] = rand() % WINW + 1;
            cluster_centersy[i] = rand() % WINH + 1;
        #else
            int max_x = 0, max_y = 0;
            for(int j = 0; j < numpoints; j++)
            {
                if(points[j].x > max_x)
                    max_x = points[j].x;
                if(points[j].y > max_y)
                    max_y = points[j].y;
            }
            cluster_centersx[i] = rand() % max_x + 1;
            cluster_centersy[i] = rand() % max_y + 1;
        #endif
        cluster_count[i] = 0;

        #ifdef DEBUG
            std::cout << "Initial cluster centers: \n";
            for(int i = 0; i < clusters; i++)
                std::cout << "(" << cluster_centersx[i] << "," << cluster_centersy[i] << ")\n";
        #endif
    }

    bool exit = false;

    debug("Starting clustering algorithm...");
    while(exit == false)
    {
        exit = true;

        //Assign points to a cluster
        for(int i = 0; i < numpoints; i++)
        {
            unsigned int cluster_score = WINW * WINH * 1000;//Something so high it could never happen

            for(int j = 0; j < clusters; j++)
            {
                //debug("Computing new cluster score...");
                unsigned int new_cluster_score = abs(points[i].x - cluster_centersx[j]) + abs(points[i].y - cluster_centersy[j]);
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
            if(cluster_count[i] > 0)//If there are actually points inside this cluster..
            {
                int new_center_x = (int) all_x / cluster_count[i];
                int new_center_y = (int) all_y / cluster_count[i];

                if(cluster_centersx[i] != new_center_x || cluster_centersy[i] != new_center_y)
                {
                    cluster_centersx[i] = new_center_x;
                    cluster_centersy[i] = new_center_y;
                    exit = false;//The loop will repeat
                }

                //If two cluster centers are the same, give the one with less points a different center
                for(int j = 0; j < clusters-1; j++)
                    for(int h = j + 1; h < clusters; h++)
                        if(cluster_centersx[j] == cluster_centersx[h] && cluster_centersy[j] == cluster_centersy[h])
                        {
                            debug("Problem identified! Changing cluster centers...");
                            int change = 0;
                            if(cluster_count[j] > cluster_count[h])
                            {
                                change = j;
                            }
                            else
                            {
                                change = h;
                            }
                            cluster_count[change] = 0;
                            while(true)
                            {
                                int k = rand() % numpoints;
                                if(points[k].x != cluster_centersx[change] && points[k].y != cluster_centersy[change])
                                {
                                    debug("Found a usable point...");
                                    cluster_count[points[k].cluster]--;
                                    points[k].cluster = change;
                                    cluster_centersx[change] = points[k].x;
                                    cluster_centersy[change] = points[k].y;
                                    break;
                                }
                            }
                            exit = false;
                        }

                #ifdef DEBUG
                    debug("Cluster centers are: ");
                    for(int j = 0; j < clusters; j++)
                    {
                        std::cout << "(" << cluster_centersx[j] << "," << cluster_centersy[j] << ")\n";
                    }
                #endif
            }
            else//Choose a new starting cluster center if no points are contained in the cluster
            {
                #ifdef WANT_RANDOM
                    cluster_centersx[i] = rand() % WINW + 1;
                    cluster_centersy[i] = rand() % WINH + 1;
                #else
                    int max_x = 0, max_y = 0;
                    for(int j = 0; j < numpoints; j++)
                    {
                    if(points[j].x > max_x)
                        max_x = points[j].x;
                    if(points[j].y > max_y)
                        max_y = points[j].y;
                    }
                        cluster_centersx[i] = rand() % max_x + 1;
                        cluster_centersy[i] = rand() % max_y + 1;
                #endif
                exit = false;
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

    pif--;

    int pointsx[pif], pointsy[pif];

    std::ifstream file ("points.txt");

    int count = 0;

    debug("All good at here...");

    if(file.is_open())
    {
        while(file.good() && count < pif)
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
