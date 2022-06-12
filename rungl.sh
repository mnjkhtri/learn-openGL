#!/usr/bin/bash
gcc -c glad.c
gcc $1 -o glout glad.o -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
./glout
rm glout
