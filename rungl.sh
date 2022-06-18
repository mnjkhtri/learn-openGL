#!/usr/bin/bash
g++ -I Includes $1 Sources/glad.c Sources/stbimage.c -o glout -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm -lassimp
./glout
rm glout
