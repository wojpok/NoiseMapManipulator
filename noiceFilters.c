#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "noiceMaps.h"

void mapParamt(NoiseMap *noiseMap, param_t lowerParameter, param_t higherParameter, param_t fromParameter, param_t toParameter) {
	if(!isNoiseMapValid(noiseMap)) return;
	int paramt = 0;
	int paramDist = higherParameter - lowerParameter;
	if(paramDist == 0) return;
	for(int i = 0; i < noiseMap->width*noiseMap->height; i++) {
		paramt = noiseMap->colorMap[i].t;
		if(paramt >= lowerParameter && paramt <= higherParameter) {
			noiseMap->colorMap[i].t = (param_t) __lerp(fromParameter, toParameter, paramDist, paramt-lowerParameter);
		}
	}
}

void castPerlinNoise1Dto2D(NoiseMap* noiseMap, int totalHeight, int level0height, int level255height) {
	if(!isNoiseMapValid(noiseMap)) return;
	
	if(noiseMap->height != 1) return;
	color_t *newColorMap = malloc(totalHeight*noiseMap->width*sizeof(color_t));
	if(newColorMap == NULL) return;
	
	noiseMap->height = totalHeight;
	
	color_t white, black;
	white.r = white.b = white.g = white.t = 255;
	black.r = black.b = black.g = black.t = 0;
	
	for(int x = 0; x < noiseMap->width; x++) {
		color_t curr = noiseMap->colorMap[x];
		//lerp
		int lineHeight = (level0height*(255-curr.t)+(level255height*(curr.t)))/255;
		
		for(int y = 0; y <= lineHeight; y++) {
			newColorMap[y*noiseMap->width + x] = black;
		}
		for(int y = lineHeight+1; y < totalHeight; y++) {
			newColorMap[y*noiseMap->width + x] = white;
		}
	}
	
	free(noiseMap->colorMap);
	noiseMap->colorMap = newColorMap;
}

//funkcja sprawdza po kolei wszystkie piksele,
//jeżeli parametr t piskela zanjduje się między zadanymi widełkami to jest kolorowany 
void colorizeNoiseMap(NoiseMap *noiseMap, param_t lowerParameter, param_t higherParameter, color_t col1, color_t col2) {
	if(!isNoiseMapValid(noiseMap)) return;
	int paramt = 0;
	int paramDist = higherParameter - lowerParameter;
	if(paramDist == 0) return;
	for(int i = 0; i < noiseMap->width*noiseMap->height; i++) {
		paramt = noiseMap->colorMap[i].t;
		if(paramt >= lowerParameter && paramt <= higherParameter) {
			int dist = paramt-lowerParameter;
			noiseMap->colorMap[i].r = __lerp(col1.r, col2.r, paramDist, dist);
			noiseMap->colorMap[i].g = __lerp(col1.g, col2.g, paramDist, dist);
			noiseMap->colorMap[i].b = __lerp(col1.b, col2.b, paramDist, dist);
		}
	}
}

//algorytm działa podobnie do Game of Life, zlicza wszytskich żywych sąsiadów w zadynym promieniu
// Może nastapić śmierć z osamotnienia i ożywienie gdy zostaje przekroczony ustalony próg
//nie występuje za to śmierć z przeludnienia, więc główny kształt nie zostaje naruszony
void conwaysMapSmoothing(NoiseMap *noiseMap, int radius, int threshold) {
	if(!isNoiseMapValid(noiseMap)) return;
	
	color_t black = color(0,0,0,0);
	color_t white = color(255,255,255,255);
	color_t *newMap = malloc(sizeof(color_t)*noiseMap->width*noiseMap->height);
	if(newMap == NULL) return;
	
	for(int y = 0; y < noiseMap->height; y++) {
		for(int x = 0; x < noiseMap->width; x++) {
			int count = 0;
			for(int yoff = -radius; yoff <= radius; yoff++) {
				if(y + yoff < 0 || y + yoff >= noiseMap->height) continue;
				for(int xoff = -radius; xoff <= radius; xoff++) {
					if(x + xoff < 0 || x + xoff >= noiseMap->width) continue;
						if(noiseMap->colorMap[(y+yoff)*noiseMap->width +(x+xoff)].t ==255)
							count++;
				}
			}
			if(count >= threshold)
				newMap[y*noiseMap->width+x] = white;
			else
				newMap[y*noiseMap->width+x] = black;
		}
	}
	
	free(noiseMap->colorMap);
	noiseMap->colorMap = newMap;	
}















