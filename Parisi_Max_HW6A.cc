// Max Parisi
// CPE 553-A hwShapes
// Cite: http://members.chello.at/~easyfilter/bresenham.html
	// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
// I pledge my honor that I have abided by the Stevens Honor System.

#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Bitmap {
private:
	constexpr static uint32_t w = 800, h = 600;
	uint32_t pixels[h][w];
	
public:
	Bitmap(uint32_t color)
	{ 
		for (uint32_t i = 0; i < h; i++)
			for (uint32_t j = 0; j < w; j++)
				pixels[i][j] = color;
	}
	
	void horizLine(uint32_t x1, uint32_t x2, uint32_t y, uint32_t color)
	{
		// use min
		if (y < h)
			for (uint32_t i = x1; i <= x2 && i < w; i++)
				pixels[y][i] = color;
	}
	
	void vertLine(uint32_t y1, uint32_t y2, uint32_t x, uint32_t color)
	{
		// use min
		if (x < w)
			for (uint32_t i = y1; i <= y2 && i < h; i++)
				pixels[i][x] = color;
	}
	
	// top-left (x, y), width, height
	void drawRect(uint32_t x, uint32_t y, uint32_t rw, uint32_t rh, uint32_t color)
	{
		if (x < w && y < h && x+rw < w && y+rh < h) {
			uint32_t i;
			for (i = x; i < x+rw; i++) {
				pixels[y][i] = color;
				pixels[y+rh][i] = color;
			}
			for (i = y; i < y+rh; i++) {
				pixels[i][x] = color;
				pixels[i][x+rw] = color;				
			}
		}
	}
	
	// top-left (x, y), width, height
	void fillRect(uint32_t x, uint32_t y, uint32_t rw, uint32_t rh, uint32_t color)
	{
		if (x < w && y < h && x+rw < w && y+rh < h) {
			for (uint32_t i = y; i < y+rh; i++)
				for (uint32_t j = x; j < x+rw; j++)
					pixels[i][j] = color;
		}
	}
	
	// drawn using https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	void line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
	{
		// only works for line going down and right where dx > dy
		if (x0 < x1 && y0 <= y1 && 
			x0 < w && x1 < w && 
			y0 < h && y1 < h) {
			uint32_t dx = x1 - x0;
			uint32_t dy = y1 - y0;
			double derr = abs(double(dy) / double(dx));
			double error = 0.0;
			int y = y0;
			for (uint32_t x = x0; x <= x1; x++) {
				pixels[y][x] = color;
				error += derr;
				if (error >= 0.5) {
					y += 1;
					error -= 1.0;
				}
			}
		}
	}
	
	// adapted from http://members.chello.at/~easyfilter/bresenham.html 
	void ellipse(uint32_t x, uint32_t y, uint32_t xdiam, uint32_t ydiam, uint32_t color)
	{
		int x0 = x - xdiam/2;
		int y0 = y - ydiam/2;
		int x1 = x + xdiam/2;
		int y1 = y + ydiam/2;

		int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
		long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
		long err = dx+dy+b1*a*a, e2; /* error of 1.step */

		if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
		if (y0 > y1) y0 = y1; /* .. exchange them */
		y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
		a *= 8*a; b1 = 8*b*b;

		do {
			pixels[y0][x1] = color; /*   I. Quadrant */
			pixels[y0][x0] = color; /*  II. Quadrant */
			pixels[y1][x0] = color; /* III. Quadrant */
			pixels[y1][x1] = color; /*  IV. Quadrant */
			e2 = 2*err;
			if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */ 
			if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
		} while (x0 <= x1);
	   
		while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
			pixels[y0][x0-1] = color; /* -> finish tip of ellipse */
			pixels[y0++][x1+1] = color;
			pixels[y1][x0-1] = color;
			pixels[y1--][x1+1] = color;
		}
	}
	
	void save(const char *name)
	{
		stbi_write_png(name, w, h, 4, pixels, w*4);
	}
};

int main() {
	// FIRST PART (ADD COLORS HERE)
	constexpr uint32_t BLACK = 0xFF000000; // black opaque
	constexpr uint32_t RED =   0xFF0000FF; // red opaque
	constexpr uint32_t BLUE =  0xFFFF0000; // blue opaque
	constexpr uint32_t WHITE = 0xFFFFFFFF; // white opaque
	constexpr uint32_t YELLOW = 0xFF00FFFF; // yellow opaque
	constexpr uint32_t GREEN = 0xFF00FF00; // green opaque



	// SECOND PART (DO NOT EDIT)
	int xcenter = 100;
	int ycenter = 100;
	int xdiameter = 200;
	int ydiameter = 100;

	Bitmap b(BLACK); // Hardcoded size (***800 x 600 pixels***)
	
        b.horizLine(0, 500, 200, RED); // Red horizontal line, from x=0 to x=500, at y = 200
	b.vertLine(0, 399, 300, RED); // Red vertical line, from y=0 to y=399, at x = 300
	
	b.drawRect(200,200, 100,50, BLUE); // Blue rectangle, TOP-LEFT at x=200, y=200. width=100, height=50
	b.fillRect(201,201, 98,48, WHITE); // White rectangle, same rules as above, but filled with color
	
	b.line(400,0, 550,300, YELLOW); // Line drawn using https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	
	b.ellipse(xcenter, ycenter, xdiameter, ydiameter, GREEN); //Ellipse using specs from above
	
	b.save("bitmap.png");
	
	
	// THIRD PART - OPTIONAL FUNCTION
	// 100pt bonus for properly implementing Wu's antialiasing
	//https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
	//b.antialiasedLine(400,50, 550,350, YELLOW);        
}
