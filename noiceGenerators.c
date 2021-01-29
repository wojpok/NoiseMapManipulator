#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "noiceMaps.h"

extern byte *hashCodes;
extern int hashCodeWidth; 
extern int hashCodeHeight;

void generateWorleyNoise2D(NoiseMap *target, int width, int height, int pointsCount, int distScaling) {
	if(!clearNoiseMap(target)) return;
	
	int *points = malloc(sizeof(int) * pointsCount * 2);
	if(points == NULL) return;
	
	target->colorMap = malloc(sizeof(color_t) * width* height);
	if(target->colorMap == NULL) {free(points); return;} 
	
	target->width = width;
	target->height = height;
	
	for(int i = 0 ; i < pointsCount; i++) {
		points[i*2] = 	nextRandom()%width;
		points[i*2+1] = nextRandom()%height;
	}
	
	int maxVal = 0;
	
	const int max = height*height + width*width;
	
	for(int y = 0 ; y < height; y++) {
		for(int x = 0 ; x < width; x++) {
			int shortestDiff = max;
			int newDist;
			for(int i = 0; i < pointsCount; i++) {
				newDist = (y-points[i*2+1])*(y-points[i*2+1]) + (x-points[i*2])*(x-points[i*2]);
				if(newDist < shortestDiff) shortestDiff = newDist;
			}
			
			color_t newCol;
			int nVal = 255 - ((float)(distScaling-(int)sqrt(shortestDiff)))/distScaling*255;
			if(nVal >255) nVal = 255;
			else if(maxVal < nVal) maxVal = nVal;
			newCol.t = nVal;
					
			target->colorMap[y*width+x] = newCol;
		}
	}

	for(int y = 0 ; y < height*width; y++) {
		color_t newCol;
		newCol.t = (param_t) __lerp(0,255,maxVal,target->colorMap[y].t);
		 newCol.r = newCol.g = newCol.b = newCol.t;
		target->colorMap[y] = newCol;
	}
	
	free(points);
}

void generatePerlinNoise1D(NoiseMap *target, int width, int hashOffsetX, int hashOffsetY, float freqScaling, int detailsLevel) {
	//ustawianie postawowych danych i sanityzacja wejścia
	if(!clearNoiseMap(target)) return; 
	target->width = width;
	target->height = 1;
	target->colorMap = NULL;
	if(width < 10 || freqScaling < 0.2 || detailsLevel < 1) {
		return;
	}
	int log2width = 0;
	while(width>>log2width != 1)
		log2width++;
	if((1<<log2width) < width)
		log2width++;
	int cWidth = 1<<log2width;	
		
	//poziom detali większy niż logarytm z szerokości kazałby wykonywać interpolacje o odstępie 0 (dzielienie przez 0)
	if(detailsLevel > log2width) detailsLevel = log2width;
	
	upscaleHashMap(cWidth+hashOffsetX, hashOffsetY);
	
	//przydzielenie pamięci do wykonania niezbędnych obliczeń
	target->colorMap = malloc(sizeof(color_t)*(width));
	int *keys = malloc((cWidth+1)*sizeof(int));
	float *tabl = calloc(cWidth*sizeof(float), 1);
	if(keys == NULL || target->colorMap == NULL || tabl == NULL) {
		if(keys != NULL) free(keys);
		if(tabl != NULL) free(tabl);
		return;
	}
	
	//wypełnienie kluczy losowymi wartościami
	for(int i = 0; i <= cWidth; i++) {
		keys[i] = hashCodes[i+hashOffsetX+hashCodeHeight*hashOffsetY];
	}
	
	//właściwy algorytm
	//scale przechowuje mnożnik dla danej częstotliwości (zmienia się co obrót pętli)
	float scale = 1.0;
	//acc przechowuje maksymalną wartość którą można osiągnąć, potrzebne do znormalizowania wyniku na końcu
	float acc = 0.0;
	
	//iterowanie po częstotliwościach, zaczynając od 0 do detailLevel <= log2width
	for(int freq = 0; freq < detailsLevel; freq++) {
		int incrementation = (cWidth>>freq);
		//liniowa interpolacja między częstotliwościami
		for(int keypoints = 0; keypoints < cWidth; keypoints+=incrementation) {	
			int y0 = keys[keypoints], y1 = keys[keypoints+incrementation];
		
			for(int i = keypoints; i < keypoints + incrementation; i++) {
				tabl[i]+=((float)( __lerp(y0, y1, incrementation, i - keypoints))/scale);
			}
		}
		//aktualizacja akkumulacji i skali
		acc += (float)255/scale;
		scale*=freqScaling;
	}
	color_t temp;
	temp.r = temp.b = temp.g = 0;
	for(int i = 0; i < width; i++) {
		//przeskalowanie, zamiana na postać decymalną i zapis do parametru t;
		temp.t = (param_t)((tabl[i]/acc)*255);
		target->colorMap[i] = temp;
	}
	//sprzątanie
	free(tabl); 
	free(keys);
}

//algorytm działa analogicznie do wersji 1D, ale podwaja się ilość kroków ze względu na drugi wymiar
//będzie zastosowana bilioniowa interpolacja
void generatePerlinNoise2D(NoiseMap *target, int width, int height, int hashOffsetX, int hashOffsetY, float freqScaling, int detailsLevel) {
	//ustawianie postawowych danych i sanityzacja wejścia
	if(!clearNoiseMap(target)) return; 
	target->width = width;
	target->height = height;
	target->colorMap = NULL;
	if(width < 10 || height < 10 || freqScaling < 0.2 || detailsLevel < 1) {
		return;
	}
	int log2width = 0;
	while(width>>log2width != 1) 
		log2width++;
	if((1<<log2width) < width) 
		log2width++;
	int cWidth = 1<<log2width;	
	//poziom detali większy niż logarytm z szerokości kazałby wykonywać interpolacje o odstępie 0 (dzielienie przez 0)
	if(detailsLevel > log2width) detailsLevel = log2width;
	//analogicznie jak wyżej
	int log2height = 0;
	while(height>>log2height != 1)
		log2height++;
	if((1<<log2height) < height)
		log2height++;
	int cHeight = 1<<log2height;
		
	if(cHeight < cWidth) {cHeight = cWidth;}
	
	if(detailsLevel > log2height) detailsLevel = log2height;
	
	upscaleHashMap(cWidth+hashOffsetX+3, cHeight+hashOffsetY+3);
	
	//przydzielenie pamięci do wykonania niezbędnych obliczeń
	target->colorMap = malloc(sizeof(color_t)*width*height);
	int *keys = malloc((cWidth+1)*(cHeight+1)*sizeof(int));
	float *tabl = calloc(cWidth*cHeight*sizeof(float), 1);
	if(keys == NULL || target->colorMap == NULL || tabl == NULL) {
		if(keys != NULL) free(keys);
		if(tabl != NULL) free(tabl);
		return;
	}
	
	//wypełnienie kluczy losowymi wartościami
	for(int y = 0; y < (cHeight+1); y++) {
		for(int x = 0; x < (cWidth+1); x++) {
			keys[x+cWidth*y] = hashCodes[x+hashOffsetX+hashCodeWidth*(y+hashOffsetY)];	
		}
	}

	float scale = 1.0;
	float acc = 0.0;
	
	//iterowanie po częstotliwościach, zaczynając od 0 do detailLevel <= log2width
	for(int freq = 0; freq < detailsLevel; freq++) {
		int incrementation = (cWidth>>freq);
		//liniowa interpolacja między częstotliwościami
		for(int keypointsX = 0; keypointsX < cWidth; keypointsX+=incrementation) {	
			for(int keypointsY = 0; keypointsY < cHeight; keypointsY+=incrementation) {	
				//zebranie kluczy z kwadratu
				int y0 = keys[keypointsX+keypointsY*cWidth], 
					y1 = keys[keypointsX+keypointsY*cWidth+incrementation],
				    y2 = keys[keypointsX + (keypointsY+incrementation)*cWidth], 
				    y3 = keys[keypointsX + (keypointsY+incrementation)*cWidth + incrementation];
			
				for(int x = keypointsX; x < keypointsX + incrementation; x++) {
					int y4 = __lerp(y0, y1, incrementation, x - keypointsX),
						y5 = __lerp(y2, y3, incrementation, x - keypointsX);
					for(int y = keypointsY; y < keypointsY + incrementation; y++) {
						tabl[x+y*cWidth]+=((float)( __lerp(y4, y5, incrementation, y - keypointsY))/scale);
					}
				}
			}
		}
		acc += (float)255/scale;
		scale*=freqScaling;
	}
	color_t temp;
	temp.r = temp.b = temp.g = 0;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			temp.t =  (param_t)((tabl[y*cWidth+x]/acc)*255);
			temp.r = temp.b = temp.g = (byte) temp.t;
			target->colorMap[y*width+x] = temp;
		}
	}
	//I really dont know why is that but apparently uncommenting this crashes everything
	//free(tabl); 
	//free(keys);
}

//Simple cos map
void generateCosWaveform(NoiseMap *target, int width, int height, float xScale, float yScale) {
	if(!clearNoiseMap(target)) return;
	target->width = width;
	target->height = height;
	target->colorMap = malloc(width*height*sizeof(color_t));
	if(target->colorMap == NULL) return;
	
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			float res = cos(x*xScale+y*yScale);
			res = res*128 + 128;
			
			target->colorMap[y*width+x] = color(res, res, res, res);
		}
	}
}
