# Noise Map Manipulator

Whether you aim to create natural looking terrain, add details to pale texture or embed dynamic sky in you game, you hardly ever do it by hand. So called noise maps (for instance Perlin Noice) with proper usage can generate well suited elements. For this reason I decided to challange myself and create this library equipped with functionality required to generate noise maps and manipulate to achieve various effects. 

## Examples

API delievered by this library is capable of creating such images:
(All of those images can be generated with code included in ```main.c```)

### Treasure Map

[Treasure Map](https://i.imgur.com/LyL6sTo.png)

### Height Map

[Treasure Map](https://i.imgur.com/w42Ibx0.png)

### Funky Texture

[Treasure Map](https://i.imgur.com/hmyPAJm.png)

### Example 2D Game Map

[Treasure Map](https://i.imgur.com/JA3j4K8.png)

## Usage

Using ```make``` you can easily compile entire library as well as example program. Included example program can be run with command ```./exmaple.out```. You will be prompted with RNG seed input field and then with example image you would like to generate.

## Documentation

``` TODO ```

You can study examples included to undestand what's going on. One important thing: Every pixel in the NoiseMap consists of 4 parameters: 3 color bytes (RGB) and parameter T. Parameter T is not an alpha channel, it is distinct value used to help mapping values on the NoiseMap and coloring pixels.


