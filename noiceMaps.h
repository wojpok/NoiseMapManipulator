/*
 * 			C Course Project 20/21
 * 
 * 			Noise Map Manipulator
 * 
 * 			Author: 324526
 */

typedef signed long long sll;
typedef unsigned char byte;

//Choose wisely - You may want to sacriface performence in order to get higher quality

//typedef byte param_t;
typedef float param_t;
//typedef double param_t;
//typedef long double param_t;

typedef struct { byte b, g, r; param_t t; } color_t;

//simple wrappers
sll nextRandom();
sll getCurrentSeed();
void setRandomSeed();
void setSeed(sll s);

//Upscaling is done automatically, but prerendered larger can save some time and few headaches
void upscaleHashMap(int width, int height);

#define __lerp(y1, y2, x1x2dist, x1xdist) (((y1)*((x1x2dist)-(x1xdist))+((y2)*(x1xdist)))/(x1x2dist))
#define __max(a, b) (((a) > (b))?(a):(b))
#define __unused_variable(x) (void)x // _mixer_Overlay needs to be compatible with pointer but it doesnt uses all parameters

typedef struct {
	int width, height;//height = 1 means 1D
	color_t *colorMap;
} NoiseMap;

color_t color(byte r, byte g, byte b, param_t t);
NoiseMap *newEmptyMap();
byte isNoiseMapValid(NoiseMap *noiseMap);
byte clearNoiseMap(NoiseMap *noiseMap);
void clear(NoiseMap *nm);
//-----------------------------------------[noiceGenerators.c]-----------------------------------
// freq scaling - is the ration determining how impactful are higher frequencies
// 0.7 - Extreamly high; 3.0 - barely noticeable
void generatePerlinNoise1D(NoiseMap *target, int width, int hashOffsetX, int hashOffsetY, float freqScaling, int detailsLevel);
void generatePerlinNoise2D(NoiseMap *target, int width, int height, int hashOffsetX, int hashOffsetY, float freqScaling, int detailsLevel);
void generateWorleyNoise2D(NoiseMap *target, int width, int height, int pointsCount, int distScaling);
void generateCosWaveform(NoiseMap *target, int width, int height, float xScale, float yScale);
void generateEmptyNoiseMap(NoiseMap *target, int width, int height);
void copyNoiseMap(NoiseMap *target, NoiseMap *source);

//-----------------------------------------[noiceFilters.c]--------------------------------------
void castPerlinNoise1Dto2D(NoiseMap *noiseMap, int totalHeight, int level0height, int level255height);
void colorizeNoiseMap(NoiseMap *noiseMap, param_t lowerParameter, param_t higherParameter, color_t col1, color_t col2);
void mapParamt(NoiseMap *noiseMap, param_t lowerParameter, param_t higherParameter, param_t fromParameter, param_t toParameter);
//inspired by Game of Life
void conwaysMapSmoothing(NoiseMap *noiseMap, int radius, int threshold);
//-----------------------------------------[noiceMixers.c]---------------------------------------
void mixerOverlay(	NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerMask(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerAdd(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerMultiply(	NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerInverseMask(NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerLerp(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY, float t);
void wobbly(NoiseMap *target, NoiseMap *sourceX, NoiseMap *sourceY);
void perlinNoises1Dto2DPath(int width, int height, NoiseMap *noiseMap1, NoiseMap *noiseMap2);


typedef signed char    si1B;
typedef signed short   si2B;
typedef signed int     si4B;

void saveNoiseToBmp(const char* 	fileName, NoiseMap *source);
void loadBMPasNoiseMap(const char* bmpFileIn, NoiseMap *target);

#pragma pack(push, 1)
typedef                 // ## BMPv3 HEADER ###################### //
  struct BmpHeaderTAG   // sizeof(BmpHeader) must be 54 [Byte]    //
{
//// FILE HEADER      //// the very basic (main) file header    ////
  si1B id[2];           /* 0x4D42 must be here, meaning 'B'and'M' */
  si4B fileSize;        /* (xDim * yDim * 3) + 54L + padding      */
  si2B reserved[2];     /* ((ignored))                            */
  si4B dataOffset;      /* 54 [Byte] ((expected))                 */
//// DIB HEADER       //// 'device-independent bitmap' header   ////
  si4B DIBHeaderSize;   /* should be 40 for us *//* 14 + 40 = 54  */
                        /* version-wise values                    */
                        /*   12: (BMPv2) Windows 2.x + OS/2 1.x   */
                        /*   40: (BMPv3) Windows 3.x + Windows NT */
                        /*  108: (BMPv4) Windows 4.x (Windows 95) */
  si4B pixWidth;        /* xDim, horizontal size ((variable))     */
  si4B pixHeight;       /* yDim, vertical ((variable))            */
  si2B biPlanes;        /*  1 assume((fixed))                     */
  si2B bitsPerPixel;    /* 24 assume((fixed))                     */
  si4B biCompression;   /*  0 assume((fixed))                     */
  si4B biImageByteSize; /* ((ignored)) after memset(_,0x0,_)      */
  si4B biXPixPerMeter;  /* ((ignored)) after memset(_,0x0,_)      */
  si4B biYPixPerMeter;  /* ((ignored)) after memset(_,0x0,_)      */
  si4B biClrUsed;       /* ((ignored)) after memset(_,0x0,_)      */
  si4B biClrImportant;  /* ((ignored)) after memset(_,0x0,_)      */
}
#if defined(__linux__)
  __attribute__((__packed__, aligned(1)))
#endif
  BmpHeader;
#pragma pack(pop)
