#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "noiceMaps.h"

byte *hashCodes = NULL;
int hashCodeWidth = 0; 
int hashCodeHeight = 0;

void upscaleHashMap(int width, int height) {
	if(width < 0) width = -width;
	if(height < 0) height = - height;
	
	//if empty generate
	if(hashCodes == NULL) {
		hashCodes = malloc(height*width*sizeof(byte));
		if(hashCodes == NULL) exit(1);
		
		for(int i = 0; i < height*width; i++)
			hashCodes[i] = nextRandom()%255;
		
		hashCodeHeight = height;
		hashCodeWidth = width;
		return;
	}
	//if not - upscale
	if(width <= hashCodeWidth && height <= hashCodeHeight) return;
	
	int newHeight = __max(hashCodeHeight, height);
	int newWidth = __max(hashCodeWidth, width);
	
	byte* newHash = malloc(newHeight*newWidth*sizeof(byte));
		if(newHash == NULL) exit(1);
		
	for(int y = 0; y < hashCodeHeight; y++) {
		memcpy(newHash+y*newWidth, hashCodes+y*hashCodeWidth, hashCodeWidth*sizeof(byte));
		for(int x = hashCodeWidth; x < newWidth; x++) {
			newHash[y*newWidth + x] = nextRandom()%255;
		}
	}
	
	for(int y = hashCodeHeight; y < newHeight; y++) {
		for(int x = 0; x < newWidth; x++) {
			newHash[y*newWidth + x] = nextRandom()%255;
		}
	}
	
	hashCodeHeight = newHeight;
	hashCodeWidth = newWidth;
	free(hashCodes);
	hashCodes = newHash;
}

//Some wrappers simply not to lose seed accidentally
static sll currentSeed;

void setRandomSeed() {
	srand(currentSeed = time(NULL));
}

void setSeed(sll s) {
	srand(currentSeed = s);
}

sll nextRandom() {
    return rand();
}

sll getCurrentSeed() {
	return currentSeed;
}

//memory managment  - each function checks if pointers are proper
//if needed it will clear the colorMap

byte isNoiseMapValid(NoiseMap *noiseMap) {
	if(noiseMap == NULL || noiseMap->colorMap == NULL
	|| noiseMap->width < 1 || noiseMap->height < 1) return 0;
	return 1;
}

byte clearNoiseMap(NoiseMap *noiseMap) {
	if(noiseMap != NULL)  {
		//if(noiseMap->colorMap != NULL)
			//free(noiseMap->colorMap);
		return 1;
	}
	return 0;
}

void clear(NoiseMap *nm) {
	if(nm == NULL) return;
	nm->width =
	nm->height = 0;
	nm->colorMap = NULL;
}

color_t color(byte r, byte g, byte b, param_t t) {
	color_t toReturn; 
	toReturn.r = r;
	toReturn.b = b;
	toReturn.g = g;
	toReturn.t = t;
	return toReturn;
}

void copyNoiseMap(NoiseMap *target, NoiseMap *source) {
	if(!clearNoiseMap(target) || !isNoiseMapValid(source)) return;
	target->width = source->width;
	target->height = source->height;
	target->colorMap = malloc(target->width*target->height*sizeof(color_t));
	if(target->colorMap == NULL) return;
	memcpy(target->colorMap, source->colorMap, target->width*target->height*sizeof(color_t));
}

void saveNoiseToBmp(const char* fileName, NoiseMap *source) {
	if(!isNoiseMapValid(source)) return;
	FILE * f = fopen(fileName,"wb+");
	if (!f) exit(0);
	BmpHeader hd;
	hd.id[0] = 0x42;
	hd.id[1] = 0x4D;
	hd.fileSize = 54 + source->width*source->height*3;
	hd.reserved[0] = 0;
	hd.reserved[1] = 0;
	hd.dataOffset = 54;
	hd.DIBHeaderSize = 40;
	hd.pixWidth = source->width;
	hd.pixHeight = source->height;
	hd.biPlanes = 1;
	hd.bitsPerPixel = 24;
	hd.biCompression = 0;
	hd.biImageByteSize = source->width*source->height*3;
	hd.biXPixPerMeter = 0;
	hd.biYPixPerMeter = 0;
	hd.biClrUsed = 0;
	hd.biClrImportant = 0;

	int emptyBytes = 0x0;
	int offset = 4-(source->width*3)%4;
	if(offset == 4) offset = 0;
	
	fwrite(&hd, 54, 1, f);
	for(int y = source->height-1; y >= 0; y--) {
		for(int x = 0; x < source->width; x++) {
			fwrite(source->colorMap+source->width*y+x, 3, 1, f);	
		}
		fwrite(&emptyBytes, offset, 1, f);	
	}
	fclose(f);
}

void loadBMPasNoiseMap(const char* bmpFileIn, NoiseMap *target) {
	if(!clearNoiseMap(target)) return;
	BmpHeader newHD;
	FILE * f = fopen(bmpFileIn, "rb");
	if(!f) exit(0);
	fread(&newHD, 1, 54, f);
	
	target->height = newHD.pixHeight;
	target->width = newHD.pixWidth;
	target->colorMap = malloc(sizeof(color_t) * target->width * target->height);
	if(target->colorMap == NULL) return;
	
	int padding = (((target->width*3)&(~3)) - (target->width*3));
	if(padding < 0) padding += 4;
	
	int offset = 4-(target->width*3)%4;
	if(offset == 4) offset = 0;
	
	fseek( f, 54, SEEK_SET );
	for(int y = target->height-1; y >= 0; y--){
			for(int x = 0; x < target->width; x++) {
				int index = y*target->width+x;
			fread(target->colorMap+index, 3, 1, f);
			target->colorMap[index].t = (target->colorMap[index].r +
			target->colorMap[index].g + target->colorMap[index].b)/3;
		}
		fseek( f, offset, 1);	
	}
	fclose(f);
}

void generateEmptyNoiseMap(NoiseMap *target, int width, int height) {
	if(!clearNoiseMap(target)) return;
	target->width = width;
	target->height = height;
	target->colorMap = malloc(width*height*sizeof(color_t));
}





















