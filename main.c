#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noiceMaps.h"
#include <math.h>

int main() {
	//sll seed = 1610808559;
	setRandomSeed();
	//pre-renderowana duża mapa hashy oszczędza dużo czasu
	upscaleHashMap(1000, 1000);
	
// -------------------------------------- Terraria
	NoiseMap Slice1, Slice2, Slice3, Slice4;
	
	generatePerlinNoise1D(&Slice1, 128, 0, 0, 1.3, 20);
	castPerlinNoise1Dto2D(&Slice1, 255, 128, 255);
	colorizeNoiseMap(&Slice1, 120, 255, 	color(108,92,129,255),	color(108,92,129,255));
	
	generatePerlinNoise1D(&Slice2, 128, 128, 0, 1.7, 4);
	castPerlinNoise1Dto2D(&Slice2, 255, 128, 255);
	colorizeNoiseMap(&Slice2, 120, 255, 	color(46,144,69,255),	color(46,144,69,255));
	
	generatePerlinNoise1D(&Slice3, 128, 256, 0, 2.2, 20);
	castPerlinNoise1Dto2D(&Slice3, 255, 128, 255);
	colorizeNoiseMap(&Slice3, 120, 255, 	color(188,168,86,255),	color(186,168,86,255));
	
	generatePerlinNoise1D(&Slice4, 128, 384, 0, 1.8, 20);
	castPerlinNoise1Dto2D(&Slice4, 255, 128, 255);
	colorizeNoiseMap(&Slice4, 120, 255, color(211,236,243,255),	color(211,236,243,255));
	

	NoiseMap empty;
	generateEmptyNoiseMap(&empty, 512, 255);
	
	mixerOverlay(&empty, &Slice1, 0, 0);
	mixerOverlay(&empty, &Slice2, 128, 0);
	mixerOverlay(&empty, &Slice3, 256, 0);
	mixerOverlay(&empty, &Slice4, 384, 0);

	NoiseMap copy;
	copyNoiseMap(&copy, &empty);
	mapParamt(&empty, 200, 255, 240, 240);
	
	NoiseMap wobbly1, wobbly2;
	generatePerlinNoise2D(&wobbly1, 512, 256, 0, 0, 3.0, 10); 
	mapParamt(&wobbly1, 0, 255, 120, 140);
	generatePerlinNoise2D(&wobbly2, 512, 256, 256, 256, 3.0, 10); 
	mapParamt(&wobbly2, 0, 255, 120, 140);
	
	wobbly(&empty, &wobbly1, &wobbly2);
	
	colorizeNoiseMap(&copy, 0, 255, 	color(0,0,0,255),	color(0,0,0,255));
	mixerInverseMask(&empty, &copy, 0, 30);
	
	NoiseMap undegroundbiomes;
	generatePerlinNoise2D(&undegroundbiomes, 512, 512, 0, 0, 1.7, 10);
	colorizeNoiseMap(&undegroundbiomes, 0, 90, 		color(120,130,100,255),	color(110,120,110,255));
	colorizeNoiseMap(&undegroundbiomes, 90, 110, 	color(45,130,87,255),	color(67,120,90,255));
	colorizeNoiseMap(&undegroundbiomes, 110, 130, 	color(90,60,40,255),	color(100,50,50,255));
	colorizeNoiseMap(&undegroundbiomes, 130, 140, 	color(200,70,100,255),	color(190,60,110,255));
	colorizeNoiseMap(&undegroundbiomes, 140, 255, 	color(120,130,100,255),	color(110,120,110,255));
	// żelazo
	NoiseMap perlin ;
	generateWorleyNoise2D(&perlin, 512, 512, 50, 10);
	colorizeNoiseMap(&perlin, 60, 255, 	color(0,0,0,255),	color(0,0,0,255));
	mapParamt(&perlin, 120, 255, 240, 240);
	colorizeNoiseMap(&perlin, 0, 120, 	color(210,200,220,255),	color(220,190,230,255));
	mapParamt(&perlin, 0, 120, 0, 0);
	
	// miedź
	NoiseMap perlin1;
	generateWorleyNoise2D(&perlin1, 512, 512, 50, 10);
	colorizeNoiseMap(&perlin1, 60, 255, 	color(0,0,0,255),	color(0,0,0,255));
	mapParamt(&perlin1, 120, 255, 240, 240);
	colorizeNoiseMap(&perlin1, 0, 120, 	color(139,81,44,255),	color(120,90,40,255));
	mapParamt(&perlin1, 0, 120, 0, 0);
	
	// złoto
	NoiseMap perlin2;
	generateWorleyNoise2D(&perlin2, 512, 512, 50, 10);
	colorizeNoiseMap(&perlin2, 60, 255, 	color(0,0,0,255),	color(0,0,0,255));
	mapParamt(&perlin2, 120, 255, 240, 240);
	colorizeNoiseMap(&perlin2, 0, 120, 	color(186,171,76,255),	color(186,171,76,255));
	mapParamt(&perlin2, 0, 120, 0, 0);
	
	mixerMask(&undegroundbiomes, &perlin, 0, 0);
	mixerMask(&undegroundbiomes, &perlin1, 0, 0);
	mixerMask(&undegroundbiomes, &perlin2, 0, 0);
	
	mapParamt(&empty, 0, 245, 0, 0);

	NoiseMap WobblyMap1;
	generatePerlinNoise2D(&WobblyMap1, 512,512, 0, 0, 1.5, 10);
	NoiseMap WobblyMap2;
	generatePerlinNoise2D(&WobblyMap2, 512,512, 512, 0, 1.5, 10);
	
	wobbly(&undegroundbiomes, &WobblyMap1, &WobblyMap2);
	
	mixerMask(&undegroundbiomes, &empty, 0, 0);
				
	NoiseMap Hell1; generatePerlinNoise1D(&Hell1, 512, 0, 1, 1.2, 10);
	NoiseMap Hell2; generatePerlinNoise1D(&Hell2, 512, 0, 2, 0.7, 10);
	NoiseMap Hell3; generatePerlinNoise1D(&Hell3, 512, 0, 3, 0.7, 10);
	
	castPerlinNoise1Dto2D(&Hell1, 20, 0, 20);
	castPerlinNoise1Dto2D(&Hell2, 30, 0, 30);
	mapParamt(&Hell2, 0, 255, 255, 0);
	castPerlinNoise1Dto2D(&Hell3, 20, 0, 20);
	
	colorizeNoiseMap(&Hell1, 0, 255, color(200,30,60,255), color(60,70,80,255));
	colorizeNoiseMap(&Hell2, 0, 255, color(200,30,60,255), color(60,70,80,255));
	colorizeNoiseMap(&Hell3, 0, 255, color(200,30,60,255), color(60,70,80,255));
	
	mapParamt(&Hell2, 0, 255, 255, 255);
	mapParamt(&Hell3, 0, 255, 255, 255);
	
	mixerInverseMask(&undegroundbiomes, &Hell1, 0, 452);
	mixerInverseMask(&undegroundbiomes, &Hell2, 0, 468);
	mixerInverseMask(&undegroundbiomes, &Hell3, 0, 492);
	
	saveNoiseToBmp("TerrariaLike.bmp", &undegroundbiomes);
	
	
	//conwaysMapSmoothing(&empty, 2, 10);
	
	// --------------------------------------------- Ręczne pismo -----------------------------------------------
	/*NoiseMap ashes;
	loadBMPasNoiseMap("AshesToAshes.bmp", &ashes);
	NoiseMap perlin1;
	generatePerlinNoise2D(&perlin1, 1024, 2048, 0, 0,  1.2, 10);
	
	mapParamt(&perlin1, 0, 255, 100, 150);
	
	NoiseMap perlin2;
	generatePerlinNoise2D(&perlin2, 1024, 2048, 1024, 0, 1.2, 1);
	mapParamt(&perlin2, 0, 255, 100, 150);
	
	wobbly(&ashes, &perlin1, &perlin2);
	mapParamt(&ashes, 0, 255, 255, 0);
	conwaysMapSmoothing(&ashes, 1, 2);
	conwaysMapSmoothing(&ashes, 1, 2);
	colorizeNoiseMap(&ashes, 0, 255, color(255,255,255,255),  color(0,0,0,0));
	saveNoiseToBmp("Ashes.bmp", &ashes);*/

// ------------------ stara mapa morska --------------------------------------------
	/*NoiseMap Perlin1;
	generatePerlinNoise2D(&Perlin1, 2048, 1024, 0, 0, 1.6,  20);
	//NoiseMap wobbly1 = generatePerlinNoise2D(2048, 1024, 0, 1024, 1.6,  20);
	NoiseMap perc;
	
	copyNoiseMap(&perc, &Perlin1);
	
	mapParamt(&Perlin1, 140, 141, 255, 255);
	mapParamt(&Perlin1, 100, 101, 255, 255);
	conwaysMapSmoothing(&Perlin1, 1, 3);
	
	colorizeNoiseMap(&perc, 100, 255, 	color(184,164,60,255),	color(220,100,30,255));
	colorizeNoiseMap(&perc, 0, 100, 	color(184,164,60,255),	color(155,162,171,255));
	
	colorizeNoiseMap(&Perlin1,  0, 255, 		color(42,15,3,255),	color(42,15,3,255));
	mixerInverseMask(&perc, &Perlin1, 0, 0);
	//wobbly(&perc, wobbly1, wobbly1);
	
	saveNoiseToBmp("s.bmp", &perc);*/
	
	
 //------------------ Mapa wysokości --------------------------------------------
	/*NoiseMap Perlin1;
	generatePerlinNoise2D(&Perlin1, 512, 512, 0, 0, 1.8,  20);
	//NoiseMap wobbly1 = generatePerlinNoise2D(2048, 1024, 0, 1024, 1.6,  20);
	
	colorizeNoiseMap(&Perlin1,  0, 90,   color(206,233,250,255),		color(206,233,250,255));
	colorizeNoiseMap(&Perlin1,  90, 130,  color(231,240,250,255),		color(231,240,250,255));
	colorizeNoiseMap(&Perlin1,  130, 150, color(220,210,110,255),	color(220,210,110,255));
	colorizeNoiseMap(&Perlin1,  150, 180, color(254,245,150,255),	color(254,245,150,255));
	colorizeNoiseMap(&Perlin1,  180, 200, color(254,201,135,255),	color(254,201,135,255));
	colorizeNoiseMap(&Perlin1,  200, 255, color(242,150,61,255),	color(242,150,61,255));
	saveNoiseToBmp("mapa.bmp", &Perlin1);*/


	//------------------------ Tekstury ---------------------------------
	/*NoiseMap cos1 = generateCosWaveform(512, 512, 0.19, 0.09);
	NoiseMap cos2 = generateCosWaveform(512, 512, 0.2, 0.1);
	colorizeNoiseMap(&cos1, 0, 255, color(255, 230, 220, 255), color(150, 200, 170, 255));
	colorizeNoiseMap(&cos2, 0, 255, color(200, 230, 250, 255), color(130, 140, 210, 255));
	NoiseMap Perlin1 = generatePerlinNoise2D(512, 512, 0, 0, 1.8,  20);
	NoiseMap Perlin2 = generatePerlinNoise2D(512, 512, 0, 0, 1.8,  20);
	wobbly(&cos1, Perlin1, Perlin2);
	wobbly(&cos2, Perlin2, Perlin1);
	mixNoiseMaps(cos1, cos2, 0, 0, &_mixer_Add);
	saveNoiseToBmp("cos2.bmp",cos1);*/
	
	
	//----------------------- Tekstury ------------------------------------------------------
	/*NoiseMap cos1 = generateCosWaveform(2048, 2048, 0.0, 0.2);
	NoiseMap cos2 = generateCosWaveform(2048, 2048, 0.3, 0.2);
	NoiseMap cos3 = generateCosWaveform(2048, 2048, 0.4, 0.1);
	colorizeNoiseMap(&cos1, 0, 255, color(0, 0, 0, 255), color(0, 0, 255, 255));
	colorizeNoiseMap(&cos2, 0, 255, color(0, 0, 0, 255), color(0, 255, 0, 255));
	colorizeNoiseMap(&cos3, 0, 255, color(0, 0, 0, 255), color(255, 0, 0, 255));
	NoiseMap Perlin1 = generatePerlinNoise2D(2048, 2048, 0, 0, 1.3,  20);
	NoiseMap Perlin2 = generatePerlinNoise2D(2048, 2048, 0, 0, 1.3,  20);
	wobbly(&cos1, Perlin1, Perlin2);
	wobbly(&cos2, Perlin2, Perlin1);
	mapParamt(&Perlin1, 0, 255, 255, 0);
	wobbly(&cos3, Perlin2, Perlin1);
	NoiseMap cos1c = copyNoiseMap(cos1);
	mixNoiseMaps(cos1, cos2, 0, 0, &_mixer_Add);
	mixNoiseMaps(cos1, cos3, 0, 0, &_mixer_Add);
	saveNoiseToBmp("cosAdd.bmp",cos1);
	
	mixNoiseMaps(cos1c, cos2, 0, 0, &_mixer_Sum);
	mixNoiseMaps(cos1c, cos3, 0, 0, &_mixer_Sum);
	saveNoiseToBmp("cosMult.bmp",cos1c);*/
	//-------------------------- Test nadpisania kluczy
	/*extern byte* hashCodes;
	extern int hashCodeHeight;
	extern int hashCodeWidth;
	
	NoiseMap image;
	loadBMPasNoiseMap("Nowa_mapa3.bmp", &image);
	
	for(int y = 0; y < image.width; y++) {
		for(int x = 0; x < image.height; x++) {
			if(image.colorMap[y*image.width+x].t < 245)
				hashCodes[x+y*hashCodeWidth] = 0;
		}
	}
	mapParamt(&image, 240, 255, 100, 100);
	mapParamt(&image, 0, 100, 255, 255);
	conwaysMapSmoothing(&image, 1, 3);
	colorizeNoiseMap(&image, 240, 255, color(42,15,3,255),	color(42,15,3,255));
	
	NoiseMap map;
	generatePerlinNoise2D(&map, 4096, 4096, 0, 0, 1.2, 9);
	
	colorizeNoiseMap(&map, 20, 255, 	color(184,164,60,255),	color(190,100,30,255));
	colorizeNoiseMap(&map, 0, 20, 	color(100,110,120,255),	color(155,162,171,255));
	
	
	mixerInverseMask(&image, &map, 0, 0);
	saveNoiseToBmp("mapaout.bmp", &image);*/
	
	/*NoiseMap nm;
	clearNoiseMap(&nm);
	generatePerlinNoise2D(&nm, 200, 128, 50, 60, 1.7, 5030);
	
	saveNoiseToBmp("debug.bmp", &nm);
	*/
	printf("%lld\n", getCurrentSeed());
	
	//saveNoiseToBmp("worleyTest2.bmp", newMap1);
	//
	
}
