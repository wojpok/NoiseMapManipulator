# Noise Map Manipulator

Whether you aim to create natural looking terrain, add details to a pale texture or embed dynamic sky in you game, you hardly ever do it by hand. Noise maps (for instance Perlin Noice) with proper usage can automate the generation of chosen elements. For this reason I decided to challenge myself and create this library equipped with functionality required to generate noise maps and manipulate them to achieve various effects. 

## Examples

This library can be used in e. g. situations given below:
(All of those images can be generated with code included in ```main.c```)

### Treasure Map

![Treasure Map](https://i.imgur.com/LyL6sTo.png)

### Height Map

![Treasure Map](https://i.imgur.com/w42Ibx0.png)

### Funky Texture

![Treasure Map](https://i.imgur.com/hmyPAJm.png)

### Example 2D Game Map

![Treasure Map](https://i.imgur.com/JA3j4K8.png)

## Usage

Using ```make``` you can easily compile entire library as well as an example program. The example program included in the repository can be run with the command ```./exmaple.out```. You will be prompted with RNG seed input field and with an example image you would generate.

## Documentation

``` TODO ```

You can study examples included to undestand what's going on. One important thing: Every pixel in the NoiseMap consists of 4 parameters: 3 color bytes (RGB) and parameter T. Parameter T is not an alpha channel. It is a distinct value used to help with mapping values on the NoiseMap and coloring pixels.


