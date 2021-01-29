#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noiceMaps.h"
#include <math.h>

int main() {
	printf("Welcome to the example usage of NoiseMapManipulator\n");
	sll seed;
	printf("Select RNG seed (0 = random): ");
	scanf("%lld", &seed);
	if( seed == 0 ) {
		setRandomSeed();
		printf("Current seed: %lld\n", getCurrentSeed());
	}
	else {
		setSeed(seed);
	}
	
	//pre-rendered larger hash is not necesery because perlin noise generators do this automatically when needed
	//but if you want to achieve smooth transition beetween maps with different parameters you may want to do this
	//You can remove this line and render terraria like terrain - difference is noticeable
	upscaleHashMap(5000, 5000);
	
	int effect;
	
	printf("Choose an example to render:\n");
	printf("1) Old Sea Map\n");
	printf("2) Height Map\n");
	printf("3) Hand Writing simulation (from source image)\n");
	printf("4) 2D Rock texture\n");
	printf("5) 2D Wood texture\n");
	printf("6) Terraria like terrain\n");
	printf("Choice: ");
	scanf("%d", &effect);
	
	switch(effect) {
		// -------------------------------------------  OLD SEA MAP ---------------------------------
		case 1 : ;
			NoiseMap *main = newEmptyMap();
			generatePerlinNoise2D(main, 2048, 1024, 0, 0, 1.6,  20);
			
			NoiseMap *colors = newEmptyMap();;
			
			copyNoiseMap(colors, main);
			
			//cutout certain pixels determined by param_t
			//use smoothing filter
			// colorize
			mapParamt(main, 140, 141, 255, 255);
			mapParamt(main, 100, 101, 255, 255);
			conwaysMapSmoothing(main, 1, 3);
			colorizeNoiseMap(main,  0, 255, 		color(42,15,3,255),	color(42,15,3,255));
			
			//colorize
			colorizeNoiseMap(colors, 100, 255, 	color(184,164,60,255),	color(220,100,30,255));
			colorizeNoiseMap(colors, 0, 100, 	color(184,164,60,255),	color(155,162,171,255));
			
			//join 2 maps
			mixerInverseMask(colors, main, 0, 0);
			
			saveNoiseToBmp("Result.bmp", colors);
		break;
		// -------------------------------------------  HEIGHT MAP ----------------------------------
		case 2 : ;
			NoiseMap* Perlin = newEmptyMap();
			generatePerlinNoise2D(Perlin, 512, 512, 0, 0, 1.8,  20);
			
			colorizeNoiseMap(Perlin,  0, 90,   color(206,233,250,255),		color(206,233,250,255));
			colorizeNoiseMap(Perlin,  90, 130,  color(231,240,250,255),		color(231,240,250,255));
			colorizeNoiseMap(Perlin,  130, 150, color(220,210,110,255),	color(220,210,110,255));
			colorizeNoiseMap(Perlin,  150, 180, color(254,245,150,255),	color(254,245,150,255));
			colorizeNoiseMap(Perlin,  180, 200, color(254,201,135,255),	color(254,201,135,255));
			colorizeNoiseMap(Perlin,  200, 255, color(242,150,61,255),	color(242,150,61,255));
			
			saveNoiseToBmp("Result.bmp", Perlin);
		break;
		// -------------------------------------------- HAND WRITING -------------------------------
		case 3 : ;
			NoiseMap *ashes = newEmptyMap();
			loadBMPasNoiseMap("sourceImages/AshesToAshes.bmp", ashes);
			
			//generate 2 noise maps, which will be used as wobbly source
			NoiseMap *perlin1 = newEmptyMap();
			generatePerlinNoise2D(perlin1, 1024, 2048, 0, 0,  1.2, 10);
			mapParamt(perlin1, 0, 255, 100, 150);
			
			NoiseMap *perlin2 = newEmptyMap();
			generatePerlinNoise2D(perlin2, 1024, 2048, 1024, 0, 1.2, 1);
			mapParamt(perlin2, 0, 255, 100, 150);
			
			wobbly(ashes, perlin1, perlin2);
			//smooth results
			mapParamt(ashes, 0, 255, 255, 0);
			conwaysMapSmoothing(ashes, 1, 2);
			conwaysMapSmoothing(ashes, 1, 2);
			//colorize
			colorizeNoiseMap(ashes, 0, 255, color(255,255,255,255),  color(0,0,0,0));
			
			saveNoiseToBmp("Result.bmp", ashes);
			
		break;
		// -------------------------------------------- ROCK TEXTURE --------------------------------
		case 4 : ;
		
			NoiseMap *cos1 = newEmptyMap(); generateCosWaveform(cos1, 512, 512, 0.19, 0.09);
			NoiseMap *cos2 = newEmptyMap(); generateCosWaveform(cos2, 512, 512, 0.2, 0.1);
			//colorizeNoiseMap(&cos1, 0, 255, color(255, 230, 220, 255), color(150, 200, 170, 255));
			//colorizeNoiseMap(&cos2, 0, 255, color(200, 230, 250, 255), color(130, 140, 210, 255));
			NoiseMap *Perlin1 = newEmptyMap(); generatePerlinNoise2D(Perlin1, 512, 512, 0, 0, 1.8,  20);
			NoiseMap *Perlin2 = newEmptyMap(); generatePerlinNoise2D(Perlin2, 512, 512, 512, 0, 1.8,  20);
			wobbly(cos1, Perlin1, Perlin2);
			wobbly(cos2, Perlin2, Perlin1);
			//replace mixer with different one to see some cool effects
			mixerMultiply(cos1, cos2, 0, 0);
			
			saveNoiseToBmp("Result.bmp",cos1);
		break;
		// -------------------------------------------- WOOD TEXTURE --------------------------------
		case 5 : ;
			NoiseMap *circles = newEmptyMap();
			
			loadBMPasNoiseMap("sourceImages/Circles.bmp", circles);
			NoiseMap *cWobbly1 = newEmptyMap(); NoiseMap *cWobbly2 = newEmptyMap();
			
			//cool effect with scaling 1.5
			generatePerlinNoise2D(cWobbly1, 1024, 1024, 0, 0, 1.6,  20);
			generatePerlinNoise2D(cWobbly2, 1024, 1024, 1024, 0, 1.6,  20);
			
			wobbly(circles, cWobbly1, cWobbly2);
			mixerMultiply(circles, cWobbly1, 0, 0);
			
			colorizeNoiseMap(circles, 0, 255, 	color(78,29,29,255),	color(216,134,83,255));
			
			saveNoiseToBmp("Result.bmp",circles);	
		break;
		// -------------------------------------------- TERRARIA LIKE -------------------------------
		case 6 : ;
			NoiseMap *Slice1 = newEmptyMap(), *Slice2 = newEmptyMap(), *Slice3 = newEmptyMap(), *Slice4 = newEmptyMap();
			
			//generate 4 1D slices - surface biomes
			
			generatePerlinNoise1D(Slice1, 128, 0, 0, 1.3, 20);
			castPerlinNoise1Dto2D(Slice1, 255, 128, 255);
			colorizeNoiseMap(Slice1, 120, 255, 	color(108,92,129,255),	color(108,92,129,255));
			
			generatePerlinNoise1D(Slice2, 128, 128, 0, 1.7, 4);
			castPerlinNoise1Dto2D(Slice2, 255, 128, 255);
			colorizeNoiseMap(Slice2, 120, 255, 	color(46,144,69,255),	color(46,144,69,255));
			
			generatePerlinNoise1D(Slice3, 128, 256, 0, 2.2, 20);
			castPerlinNoise1Dto2D(Slice3, 255, 128, 255);
			colorizeNoiseMap(Slice3, 120, 255, 	color(188,168,86,255),	color(186,168,86,255));
			
			generatePerlinNoise1D(Slice4, 128, 384, 0, 1.8, 20);
			castPerlinNoise1Dto2D(Slice4, 255, 128, 255);
			colorizeNoiseMap(Slice4, 120, 255, color(211,236,243,255),	color(211,236,243,255));
			
			//join all slices
			NoiseMap *empty= newEmptyMap();
			generateEmptyNoiseMap(empty, 512, 255);
			
			mixerOverlay(empty, Slice1, 0, 0);
			mixerOverlay(empty, Slice2, 128, 0);
			mixerOverlay(empty, Slice3, 256, 0);
			mixerOverlay(empty, Slice4, 384, 0);

			NoiseMap *copy= newEmptyMap();
			copyNoiseMap(copy, empty);
			
			//important later
			mapParamt(empty, 200, 255, 240, 240);
			
			//wobbly-fy surface
			NoiseMap *wobbly1= newEmptyMap(), *wobbly2= newEmptyMap();
			generatePerlinNoise2D(wobbly1, 512, 256, 0, 0, 3.0, 10); 
			mapParamt(wobbly1, 0, 255, 120, 140);
			generatePerlinNoise2D(wobbly2, 512, 256, 256, 256, 3.0, 10); 
			mapParamt(wobbly2, 0, 255, 120, 140);
			
			wobbly(empty, wobbly1, wobbly2);
			
			//trick - by propper mapping paramt this filters will make surface thinner
			colorizeNoiseMap(copy, 0, 255, 	color(0,0,0,255),	color(0,0,0,255));
			mixerInverseMask(empty, copy, 0, 30);
			
			NoiseMap *undegroundbiomes= newEmptyMap();
			generatePerlinNoise2D(undegroundbiomes, 512, 512, 0, 0, 1.7, 10);
			colorizeNoiseMap(undegroundbiomes, 0, 90, 		color(120,130,100,255),	color(110,120,110,255));
			colorizeNoiseMap(undegroundbiomes, 90, 110, 	color(45,130,87,255),	color(67,120,90,255));
			colorizeNoiseMap(undegroundbiomes, 110, 130, 	color(90,60,40,255),	color(100,50,50,255));
			colorizeNoiseMap(undegroundbiomes, 130, 140, 	color(200,70,100,255),	color(190,60,110,255));
			colorizeNoiseMap(undegroundbiomes, 140, 255, 	color(120,130,100,255),	color(110,120,110,255));
			// iron
			NoiseMap *iron = newEmptyMap();
			generateWorleyNoise2D(iron, 512, 512, 50, 10);
			colorizeNoiseMap(iron, 60, 255, 	color(0,0,0,255),	color(0,0,0,255));
			mapParamt(iron, 120, 255, 240, 240);
			colorizeNoiseMap(iron, 0, 120, 	color(210,200,220,255),	color(220,190,230,255));
			mapParamt(iron, 0, 120, 0, 0);
			
			// copper
			NoiseMap *copper= newEmptyMap();
			generateWorleyNoise2D(copper, 512, 512, 50, 10);
			colorizeNoiseMap(copper, 60, 255, 	color(0,0,0,255),	color(0,0,0,255));
			mapParamt(copper, 120, 255, 240, 240);
			colorizeNoiseMap(copper, 0, 120, 	color(139,81,44,255),	color(120,90,40,255));
			mapParamt(copper, 0, 120, 0, 0);
			
			// gold
			NoiseMap *gold= newEmptyMap();
			generateWorleyNoise2D(gold, 512, 512, 50, 10);
			colorizeNoiseMap(gold, 60, 255, 	color(0,0,0,255),	color(0,0,0,255));
			mapParamt(gold, 120, 255, 240, 240);
			colorizeNoiseMap(gold, 0, 120, 	color(186,171,76,255),	color(186,171,76,255));
			mapParamt(gold, 0, 120, 0, 0);
			
			//join all
			mixerMask(undegroundbiomes, iron, 0, 0);
			mixerMask(undegroundbiomes, copper, 0, 0);
			mixerMask(undegroundbiomes, gold, 0, 0);
			
			mapParamt(empty, 0, 245, 0, 0);

			//wobblyfy
			NoiseMap *WobblyMap1= newEmptyMap();
			generatePerlinNoise2D(WobblyMap1, 512,512, 0, 0, 1.5, 10);
			NoiseMap *WobblyMap2= newEmptyMap();
			generatePerlinNoise2D(WobblyMap2, 512,512, 512, 0, 1.5, 10);
			
			wobbly(undegroundbiomes, WobblyMap1, WobblyMap2);
			
			//join surface and underground
			mixerMask(undegroundbiomes, empty, 0, 0);
			
			//simple underworld			
			NoiseMap *Hell1= newEmptyMap(); generatePerlinNoise1D(Hell1, 512, 0, 1, 1.2, 10);
			NoiseMap *Hell2= newEmptyMap(); generatePerlinNoise1D(Hell2, 512, 0, 2, 0.7, 10);
			NoiseMap *Hell3= newEmptyMap(); generatePerlinNoise1D(Hell3, 512, 0, 3, 0.7, 10);
			
			castPerlinNoise1Dto2D(Hell1, 20, 0, 20);
			castPerlinNoise1Dto2D(Hell2, 30, 0, 30);
			mapParamt(Hell2, 0, 255, 255, 0);
			castPerlinNoise1Dto2D(Hell3, 20, 0, 20);
			
			colorizeNoiseMap(Hell1, 0, 255, color(200,30,60,255), color(60,70,80,255));
			colorizeNoiseMap(Hell2, 0, 255, color(200,30,60,255), color(60,70,80,255));
			colorizeNoiseMap(Hell3, 0, 255, color(200,30,60,255), color(60,70,80,255));
			
			mapParamt(Hell2, 0, 255, 255, 255);
			mapParamt(Hell3, 0, 255, 255, 255);
			
			//join
			mixerInverseMask(undegroundbiomes, Hell1, 0, 452);
			mixerInverseMask(undegroundbiomes, Hell2, 0, 468);
			mixerInverseMask(undegroundbiomes, Hell3, 0, 492);
			
			saveNoiseToBmp("Result.bmp", undegroundbiomes);
		break;
		default :
			printf("Invalid option, exiting...\n");
			exit(0);
	}
	
	printf("\nDone\n\n");
}
