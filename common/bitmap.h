#ifndef __BITMAP_H_
#define __BITMAP_H_

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
struct RGB {
	GLbyte blue;
	GLbyte green;
	GLbyte red;
	GLbyte alpha;
};

struct BMPInfoHeader {
  GLuint	size;
  GLuint	width;
  GLuint	height;
  GLushort  planes;
  GLushort  bits;
  GLuint	compression;
  GLuint	imageSize;
  GLuint	xScale;
  GLuint	yScale;
  GLuint	colors;
  GLuint	importantColors;
};

struct BMPHeader {
  GLushort	type; 
  GLuint	size; 
  GLushort	unused; 
  GLushort	unused2; 
  GLuint	offset; 
}; 

struct BMPInfo {
  BMPInfoHeader		header;
  RGB				colors[1];
};
#pragma pack(8)


GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

#endif // __BITMAP_H_
