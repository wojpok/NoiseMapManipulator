#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "noiceMaps.h"

void wobbly(NoiseMap *target, NoiseMap *sourceX, NoiseMap *sourceY) {
	if(!isNoiseMapValid(target) || !isNoiseMapValid(sourceX) || !isNoiseMapValid(sourceY) ||
	sourceX->width < target->width || sourceX->height < target->height ||
	sourceY->width < target->width || sourceY->height < target->height) return;
	
	color_t *newMap = malloc(target->width *target->height*sizeof(color_t));
	if(newMap == NULL) return;
	
	for(int y = 0; y < target->height; y++) {
		for(int x = 0; x < target->width; x++) {
			
			int xind = sourceX->colorMap[x+sourceX->width*y].t + x-128;
			int yind = sourceY->colorMap[x+sourceY->width*y].t + y-128;
			
			if(xind >= target->width) xind = target->width-1;
			else if(xind < 0) xind = 0;
			if(yind >= target->height) yind = target->width-1;
			else if(yind < 0) yind = 0;

			newMap[x+target->width*y] = target->colorMap[yind*target->width+xind];
		}
	}
	free(target->colorMap);
	target->colorMap = newMap;
}


static void mixNoiseMaps(NoiseMap *target, NoiseMap *source, int offsetX, int offsetY, color_t (*colorMixer)(color_t col1, color_t col2)) {
	if(!isNoiseMapValid(target) || !isNoiseMapValid(source)) return;
	int fromX = 0, toX = source->width, fromY = 0, toY=source->height;
	if(offsetX < 0) fromX = -offsetX;
	if(offsetY < 0) fromY = -offsetY;
	if(offsetX + source->width < toX) toX = offsetX + source->width;
	if(offsetY + source->height < toY) toY = offsetY + source->height;

	for(int y = fromY; y < toY; y++) {
		for(int x = fromX; x < toX; x++) {
			int ind = (y + offsetY) * target->width+ x + offsetX;
			target->colorMap[ind] = 
				colorMixer(target->colorMap[ind],source->colorMap[(y)*source->width+x]);
		}
	}
}

static color_t _mixer_Overlay( color_t col1, color_t col2) {
	__unused_variable(col1);
	return col2;
}

static color_t _mixer_Mask(color_t col1, color_t col2) {
	if(col2.t > col1.t) return col1;
	return col2;
}

static color_t _mixer_InverseMask(color_t col1, color_t col2) {
	if(col2.t > col1.t) return col2;
	return col1;
}

static color_t _mixer_Add(color_t col1, color_t col2) {
	color_t newCol;
	newCol.r = __max(col1.r, col2.r);
	newCol.g = __max(col1.g, col2.g);
	newCol.b = __max(col1.b, col2.b);
	newCol.t = __max(col1.t, col2.t);
	return newCol;
}

static float lerpParameter = 0.5;

static color_t _mixer_Lerp(color_t col1, color_t col2) {
	color_t newCol;
	newCol.r = __lerp(col1.r, col2.r, 1, lerpParameter);
	newCol.g = __lerp(col1.g, col2.g, 1, lerpParameter);
	newCol.b = __lerp(col1.b, col2.b, 1, lerpParameter);
	newCol.t = __lerp(col1.t, col2.t, 1, lerpParameter);
	return newCol;
}

static color_t _mixer_Mult(color_t col1, color_t col2) {
	color_t newCol;
	newCol.r = col1.r* col2.r/255;
	newCol.g = col1.g* col2.g/255;
	newCol.b = col1.b* col2.b/255;
	newCol.t = col1.t* col2.t/255;
	return newCol;
}

void mixerOverlay(	NoiseMap *target, NoiseMap *source, int offsetX, int offsetY) {
	mixNoiseMaps(target, source, offsetX, offsetY, &_mixer_Overlay);
}
void mixerMask(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY){
	mixNoiseMaps(target, source, offsetX, offsetY, &_mixer_Mask);
}
void mixerAdd(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY){
	mixNoiseMaps(target, source, offsetX, offsetY, &_mixer_Add);
}
void mixerMultiply(	NoiseMap *target, NoiseMap *source, int offsetX, int offsetY){
	mixNoiseMaps(target, source, offsetX, offsetY, &_mixer_Mult);
}
void mixerInverseMask(NoiseMap *target, NoiseMap *source, int offsetX, int offsetY){
	mixNoiseMaps(target, source, offsetX, offsetY, &_mixer_InverseMask);
}
void mixerLerp(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY, float t){
	lerpParameter = t;
	if(t > 1) lerpParameter = 1;
	if(t < 0) lerpParameter = 0;
	mixNoiseMaps(target, source, offsetX, offsetY, &_mixer_Lerp);
}


void perlinNoises1Dto2DPath(int width, int height, NoiseMap *noiseMap1, NoiseMap *noiseMap2) {
	if(!isNoiseMapValid(noiseMap1) || !isNoiseMapValid(noiseMap2)) return;
	color_t *newMap = malloc(sizeof(color_t)*width*height);
	if(newMap ==NULL) return;
	
	color_t black = color(0,0,0,0);
	color_t white = color(255,255,255,255);
	for(int i = 0; i < width*height; i++)
		newMap[i] = black;
	for(int i = 0; i < noiseMap1->width; i++) {
		int x = noiseMap1->colorMap[i].t /255 * width;
		int y = noiseMap2->colorMap[i].t /255 * height;
		
		for(int yoff = -2; yoff <= 2; yoff++) {
			if(y + yoff < 0 || y + yoff >= height) continue;
			for(int xoff = -2; xoff <= 2; xoff++) {
				if(x + xoff < 0 || x + xoff >= width) continue;
				newMap[(y+yoff)*width +(x+xoff)] = white;
			}
		}
		
		
	}
	free(noiseMap1->colorMap);
	noiseMap1->colorMap = newMap;
	noiseMap1->width = width;
	noiseMap1->height = height;	
}







