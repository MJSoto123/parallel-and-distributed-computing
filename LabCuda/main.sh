#!/bin/bash

if [ -z $1 ]; then
    echo "para ayuda"
    echo "./main.sh -h"
    echo "para compilar gray"
    echo "./main.sh -g [imagen]"
    echo "para compilar blur"
    echo "./main.sh -b [imagen]"
elif [ "-h" = "$1" ]; then
    echo "para compilar opencv"
    echo "g++ [archivo].cpp -o [nombre].out -lopencv_core -lopencv_imgcodecs"
elif [ "-b" = "$1" ]; then
    rm blur.out
    nvcc blur.cu -o blur.out $(pkg-config --cflags --libs opencv4)
    # nvcc -o blur.out blur.cu -I/home/alejandro/opencv4.9-custom/include/opencv4 -L/home/alejandro/opencv4.9-custom/lib -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_core
    sudo ./blur.out $2
elif [ "-g" = "$1" ]; then
    rm gray.out
    nvcc gray.cu -o gray.out $(pkg-config --cflags --libs opencv4)
    # nvcc -o gray.out gray.cu -I/home/alejandro/opencv4.9-custom/include/opencv4 -L/home/alejandro/opencv4.9-custom/lib -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_core
    sudo ./gray.out $2
fi