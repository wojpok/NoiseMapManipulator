/*
 * 
 * 			Projekt z przedmiotu Kurs programowania w języku C 2020/21
 * 	
 * 			Temat: Biblioteka generująca i manipulująca szumami 1D i 2D
 * 
 */

// TO DO 
// 4) padding w BMP

typedef signed long long sll;
typedef unsigned char byte;

//każdy piksel przechowuje wartości rgb 0-255 i parametr t
//użycie dobrego kontenera dla parametru t jest kluczowe by wyniki były dobrej jakości i były odporne na skalowanie, aczkolwiek może to wpłynąć na czas wykonywania i użytą pamięć
//w kolejności od najgorszej jakości do najlepszej: (każda wersja działa poprawnie bo parametr t jest mapowany między 0 i 255 żeby kolory można było łatwo przeliczać)
//typedef byte param_t;
typedef float param_t;
//typedef double param_t;
//typedef long double param_t;

typedef struct { byte b, g, r; param_t t; } color_t;

//najważniejsza cecha pseudo losowych liczb w generowaniu szumów to możliwość
//otrzymania tego samego wyniku używając tego samego ziarna
sll nextRandom();
sll getCurrentSeed();
void setRandomSeed();
void setSeed(sll s);

void upscaleHashMap(int width, int height);
byte* getHashPtr();
int getHashWidth();
int getHashHeight();

#define __lerp(y1, y2, x1x2dist, x1xdist) (((y1)*((x1x2dist)-(x1xdist))+((y2)*(x1xdist)))/(x1x2dist))
#define __max(a, b) (((a) > (b))?(a):(b))
#define __unused_variable(x) (void)x //wyciszenie kompilatora bo pewna funkcja nie wykorzystuje wszystkich paramaetrów ale musi być zgodna ze wskaźnikiem

typedef struct {
	int width, height;//height = 1 oznacza szum 1D
	color_t *colorMap;
} NoiseMap;

color_t color(byte r, byte g, byte b, param_t t);
byte isNoiseMapValid(NoiseMap *noiseMap);
byte clearNoiseMap(NoiseMap *noiseMap);
void clear(NoiseMap *nm);
//freq scaling oznacza jak duży wpływ mają wysokie czętstotliwości
// ok 0.7 bardzo duży - 1.8 umiarkowany - 4.0 niezauważalnie mały
//detailsLevel inaczej ilość oktaw wykorzystanych w generowaniu - większa liczba - większa dokładność
void generatePerlinNoise1D(NoiseMap *target, int width, int hashOffsetX, int hashOffsetY, float freqScaling, int detailsLevel);
void generatePerlinNoise2D(NoiseMap *target, int width, int height, int hashOffsetX, int hashOffsetY, float freqScaling, int detailsLevel);
void generateWorleyNoise2D(NoiseMap *target, int width, int height, int pointsCount, int distScaling);
void generateCosWaveform(NoiseMap *target, int width, int height, float xScale, float yScale);
void generateEmptyNoiseMap(NoiseMap *target, int width, int height);
void copyNoiseMap(NoiseMap *target, NoiseMap *source);

//-----------------------------------------[noiceFilters.c]--------------------------------------
void castPerlinNoise1Dto2D(NoiseMap *noiseMap, int totalHeight, int level0height, int level255height);
//liniowo mapuje color między zadanymi parametrami
void colorizeNoiseMap(NoiseMap *noiseMap, param_t lowerParameter, param_t higherParameter, color_t col1, color_t col2);
//liniowo mapuje parametr t między zadanymi wartościami
void mapParamt(NoiseMap *noiseMap, param_t lowerParameter, param_t higherParameter, param_t fromParameter, param_t toParameter);
//Funkcja zawdzięcza swoją nazwe Johnowi Conwayowi, twórcy Game of Life, która jest inspiracją dla tego algorytmu
void conwaysMapSmoothing(NoiseMap *noiseMap, int radius, int threshold);
//-----------------------------------------[noiceMixers.c]---------------------------------------

void mixerOverlay(	NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerMask(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerAdd(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerMultiply(	NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerInverseMask(NoiseMap *target, NoiseMap *source, int offsetX, int offsetY);
void mixerLerp(		NoiseMap *target, NoiseMap *source, int offsetX, int offsetY, float t);

//ważne żeby obie mapy fromWobbly były tej samej wielkości lub większe od toWobbly
void wobbly(NoiseMap *target, NoiseMap *sourceX, NoiseMap *sourceY);
void perlinNoises1Dto2DPath(int width, int height, NoiseMap *noiseMap1, NoiseMap *noiseMap2);


typedef signed char    si1B;
typedef signed short   si2B;
typedef signed int     si4B;

//zapis i odczyt do pliku
void saveNoiseToBmp(const char* fileName, NoiseMap *source);
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
