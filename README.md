kmcv : K-Means Clustering Visualizer
------------------------------------------------------------------------------------

This project uses the K-Means Clustering algorithm to cluster points and then draws the resulting clusters with SDL, HTML5, and/or plaintext.

### Build Instructions:

Relies on SDL along with the C and C++ Standard Libraries. Just run make on Linux systems. It should be trivial to build on a Windows system or various embedded Linux based  platforms. If you don't want to use the SDL visualizer, you can build without SDL.

### Usage:

Edit the #define at the top of global.h to set various usage options.

For clustering points from a file, format your points as such:

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
