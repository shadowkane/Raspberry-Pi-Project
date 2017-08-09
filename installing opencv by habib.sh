#!/bin/bash

###########################################################
#
# OpenCV 3.2.0 
# http://opencv.org/
#
###########################################################

#make sure you have to download and unzip opencv and then open the folder before you run this file

 
#----------------------------------------------------------
# Installing dependecies
#----------------------------------------------------------

 
sudo apt-get -y install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config

sudo apt-get -y install python-dev python-numpy python2.7-dev unzip python3-dev libqt4-dev

sudo apt-get -y install libjpeg-dev libpng12-dev libjasper-dev libtiff5-dev

sudo apt-get -y install libavcodec-dev libswscale-dev libavformat-dev libv4l-dev libxvidcore-dev libx264-dev libdc1394-22 libdc1394-22-dev

sudo apt-get -y install libatlas-base-dev gfortran 

sudo apt-get -y install libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev v4l-utils 
 
 
#----------------------------------------------------------
# Installing OpenCV
#----------------------------------------------------------


mkdir build
 
cd build
 
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..

#you can use make -j 4 
make 
 
sudo make install
 
sudo ldconfig
