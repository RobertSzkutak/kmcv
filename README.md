kmcv : K-Means Clustering Visualizer
------------------------------------------------------------------------------------

This project uses the K-Means Clustering algorithm to cluster points and then draws the resulting clusters.

### Build Instructions:

Relies on SDL along with the C and C++ Standard Libraries. Just run make on Linux systems. It should be trivial to build on a Windows system or various embedded Linux based  platforms.

### Usage:

For clustering random points: Just modify the #define statements at the top of global.h. Then in main() run randomPoints(); 

For clustering points from a file use readInPoints(); Format points as such:

    x,y\n
    x,y\n
    x,y\n
    etc.

An example points.txt file is included in this repository.

### About the author:
* Name: Robert L Szkutak II
* Website: http://robertszkutak.com
* E-mail: robert@robertszkutak.com
* Twitter: @robertszkutak
