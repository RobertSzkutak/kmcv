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

#ifdef USE_HTML5

#include "html.h"

void html5_visualize_clusters(point *points, int numpoints, int *cluster_centersx, int *cluster_centersy, int clusters)
{
    std::ofstream out("output.html");
    
    std::string top = "<html>\
	<head>\
		<title>Generated by kmcv - http:/"+std::string("/github.com/RobertSzkutak/kmcv </title>");
	std::string bottom = "</script>\
	</head>\
	<body>\
		<canvas id=\"myCanvas\">\
			Your Browser does not support HTML5		</canvas>\
	</body>\
</html>";
	std::string script = "<script>";
	
	script += "var canvas = null;\
               var context = null;\
               window.onload = init;\
               var offset_x = 0;\
               var offset_y = 0;\
               var speed = 2;\n\
               window.addEventListener(\'keydown\', function(event)\n\
               {\n\
                switch (event.keyCode)\n\
                {\n\
                    case 32: //Spacebar\n\
                     break;\
                    case 37: //Left\n\
                     offset_x += speed;\
                    break;\
                    case 38: // Up\n\
                     offset_y += speed;\
                    break;\
                    case 39: // Right\n\
                     offset_x -= speed;\
                    break;\
                    case 40: // Down\n\
                     offset_y -= speed;\
                    break;\
                }\
               }, false);\
               function init()\
               {\
	               canvas = document.getElementById('myCanvas');\
                   context = canvas.getContext('2d');\
	               canvas.width =";
    std::ostringstream convert, convert2;
    convert << WINW;
    convert2 << WINH;
    script += convert.str() + ";\
	canvas.height = " + convert2.str() +";\
	setInterval(draw, 1000 / 30);\
                  }\
    function draw()\
    {\
    context.clearRect(0, 0, canvas.width, canvas.height);";

    //Draw axis
    #ifdef WANT_AXIS_DRAWN
        int totx = 0, toty = 0;    
        for(int i = 0; i < numpoints; i++)
        {
            totx += points[i].x;
            toty += points[i].y;
        }
        std::ostringstream avgx, avgy;
        avgx << totx / numpoints;
        #ifdef MIRROR_Y
            avgy << toty / numpoints * -1;
        #else
            avgy << toty / numpoints;
        #endif
        #ifdef WANT_MEAN_AXIS
            //Draw Y Axis
            script += "context.fillStyle = \"rgb(0, 0, 0)\";";
            script += "context.beginPath();";
            script += "context.moveTo(" + avgx.str() + "+offset_x,0);\
                       context.lineTo(" + avgx.str() + "+offset_x," + convert2.str() + ");\
                       context.closePath();\
                       context.stroke();";
            //Draw X axis
            script += "context.beginPath();";
            script += "context.moveTo(0," + avgy.str() + "+offset_y);\
                       context.lineTo(" + convert.str() + "," + avgy.str() + "+offset_y);\
                       context.closePath();\
                       context.stroke();";
        #else
            //Draw Y Axis
            script += "context.fillStyle = \"rgb(0, 0, 0)\";";
            script += "context.beginPath();";
            script += "context.moveTo(0+offset_x,0);\
                       context.lineTo(0+offset_x," + convert2.str() + ");\
                       context.closePath();\
                       context.stroke();";
            //Draw X axis
            script += "context.beginPath();";
            script += "context.moveTo(0,0+offset_y);\
                       context.lineTo(" + convert.str() + ",0+offset_y);\
                       context.closePath();\
                       context.stroke();";
        #endif
    #endif

    //Draw Cluster Centers
    #ifdef WANT_CENTERS_DRAWN
        for(int i = 0; i < clusters; i++)
        {
            std::ostringstream x, y;
            x << cluster_centersx[i];
            #ifdef MIRROR_Y
                y << cluster_centersy[i]*-1;
            #else
                y << cluster_centersy[i];
            #endif
            
            script += "context.beginPath();";
            script += "context.arc(" + x.str() + "+offset_x, " + y.str() + "+offset_y, 5, 0, 2 * Math.PI, false);";
            switch(i)
            {
                case 0:
                  script += "context.fillStyle = \"rgb(255,0,0)\";";//Red
                  break;
                case 1:
                  script += "context.fillStyle = \"rgb(0,255,0)\";";//Green
                  break;
                case 2:
                  script += "context.fillStyle = \"rgb(0,0,255)\";";//Blue
                  break;
                case 3:
                  script += "context.fillStyle = \"rgb(255,0,255)\";";//Pink
                  break;
                default:
                  script += "context.fillStyle = \"rgb(0,0,0)\";";//Black
                  break;
            }
            script += "context.fill();";
            script += "context.closePath();";
        }
    #endif
    
    //Plot points
    for(int i = 0; i < numpoints; i++)
    {
        std::ostringstream x, y;
        x << points[i].x-2;//Offset of -2 makes center pixel of 5x5 appear at x,y
        #ifdef MIRROR_Y
            y << points[i].y*-1-2;
        #else
            y << points[i].y-2;
        #endif
        //Pick color
        switch(points[i].cluster)
        {
            case 0:
              script += "context.fillStyle = \"rgb(255,0,0)\";";//Red
              break;
            case 1:
              script += "context.fillStyle = \"rgb(0,255,0)\";";//Green
              break;
            case 2:
              script += "context.fillStyle = \"rgb(0,0,255)\";";//Blue
              break;
            case 3:
              script += "context.fillStyle = \"rgb(255,0,255)\";";//Pink
              break;
            default:
              script += "context.fillStyle = \"rgb(0,0,0)\";";//Black
              break;
        }
        //Draw point
        script += "context.fillRect(" + x.str() + "+offset_x, " + y.str() + "+offset_y, 5, 5);";
    }
    
    script += "}";
	
	out << top << script << bottom;
	out.close();
}

#endif
