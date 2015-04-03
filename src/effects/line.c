#pragma GCC optimize ("O0")
#include "main.h"
#include "simple.h"


/* signum function */
int sgn(int x){
  return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}

/* abs function */
static inline uint16_t iabs(int16_t x){
  return (x > 0) ? x : (x < 0) ? -x : 0;
}

/*--------------------------------------------------------------
 * Bresenham-Algorithmus: Linien auf Rastergeräten zeichnen
 *
 * Eingabeparameter:
 *    int xstart, ystart        = Koordinaten des Startpunkts
 *    int xend, yend            = Koordinaten des Endpunkts
 *
 * Ausgabe:
 *    void SetPixel(int x, int y) setze ein Pixel in der Grafik
 *         (wird in dieser oder aehnlicher Form vorausgesetzt)
 *---------------------------------------------------------------
 */
void drawLine(uint8_t *target, uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend, uint8_t color) {

    int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

    /* Entfernung in beiden Dimensionen berechnen */
   dx = xend - xstart;
   dy = yend - ystart;

/* Vorzeichen des Inkrements bestimmen */
   incx = sgn(dx);
   incy = sgn(dy);
   if(dx<0) dx = -dx;
   if(dy<0) dy = -dy;

/* feststellen, welche Entfernung größer ist */
   if (dx>dy)
   {
      /* x ist schnelle Richtung */
      pdx=incx; pdy=0;    /* pd. ist Parallelschritt */
      ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
      es =dy;   el =dx;   /* Fehlerschritte schnell, langsam */
   } else
   {
      /* y ist schnelle Richtung */
      pdx=0;    pdy=incy; /* pd. ist Parallelschritt */
      ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
      es =dx;   el =dy;   /* Fehlerschritte schnell, langsam */
   }

/* Initialisierungen vor Schleifenbeginn */
   x = xstart;
   y = ystart;
   err = el/2;
   drawPixel(target, x, y, color);

   /* Pixel berechnen */
   for(t=0; t<el; ++t) { /* t zaehlt die Pixel, el ist auch Anzahl */

      /* Aktualisierung Fehlerterm */
      err -= es;
      if(err<0) {
          /* Fehlerterm wieder positiv (>=0) machen */
          err += el;
          /* Schritt in langsame Richtung, Diagonalschritt */
          x += ddx;
          y += ddy;
      } else {
          /* Schritt in schnelle Richtung, Parallelschritt */
          x += pdx;
          y += pdy;
      }
      drawPixel(target, x, y, color);
   }
}

static inline void DrawPixelD(uint8_t *target, float distance) {
	float foo = 1 - (distance * 0.6667f);
	float bar = foo * foo;

	//uint8_t three = 5 * bar;
	uint8_t seven = 7 * bar;
	uint8_t color = COLOR(seven,0,0);
	*target = color;
}


/**
 * gupta-STROULL anti-aliased lines
 * https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
 */
void drawLineAA(uint8_t *target, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {

	uint8_t* addr = target + x0 + (y0 * videoInstance.resx);

	int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;

    int16_t du;
  	int16_t dv;
   	int16_t u;
   	int16_t v;
   	int16_t uincr;
	int16_t vincr;

	uint16_t dxabs = iabs(dx);
	uint16_t dyabs = iabs(dy);

    /* By switching to (u,v), we combine all eight octants */
    if (dxabs > dyabs) {
		du = dxabs;
		dv = dyabs;
		u = x1;
		v = y1;
		uincr = 1;
		vincr = videoInstance.resx;
		if (dx < 0) uincr = -uincr;
		if (dy < 0) vincr = -vincr;
    } else {
		du = dyabs;
		dv = dxabs;
		u = y1;
		v = x1;
		uincr = videoInstance.resx;
		vincr = 1;
		if (dy < 0) uincr = -uincr;
		if (dx < 0) vincr = -vincr;
    }

    int16_t uend = u + 2 * du;
    int16_t d = (2 * dv) - du;	    /* Initial value as in Bresenham's */
    int16_t incrS = 2 * dv;	/* delta d for straight increments */
    int16_t incrD = 2 * (dv - du);	/* delta d for diagonal increments */
    int16_t twovdu = 0;	/* Numerator of distance; starts at 0 */

    float sqrtdudv;
    arm_sqrt_f32(du * du + dv * dv, &sqrtdudv);

    float invD = 1.0 / (2.0 * sqrtdudv);   /* Precomputed inverse denominator */
    float invD2du = 2.0 * (du * invD);   /* Precomputed constant */

    do {
		DrawPixelD(addr, twovdu * invD);
		DrawPixelD(addr + vincr, invD2du - twovdu * invD);
		DrawPixelD(addr - vincr, invD2du + twovdu * invD);

		if (d < 0) {
			/* choose straight (u direction) */
			twovdu = d + du;
			d = d + incrS;
		} else {
			/* choose diagonal (u+v direction) */
			twovdu = d - du;
			d = d + incrD;
			v = v + 1;
			addr = addr + vincr;
		}

		u = u + 1;
		addr = addr + uincr;
    } while (u < uend);

}

#define INTENSITY_BITS 3
#define NUM_LEVELS 7
#define INTENTSITY_SHIFT (16 - INTENSITY_BITS)


// http://www.codeproject.com/Articles/13360/Antialiasing-Wu-Algorithm
void drawWuLine (uint8_t *target, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {

   uint16_t errorAdj, errorAcc;
   uint16_t errorAccTemp, weighting;
   int16_t dx, dy, temp, xDir;

   if (y0 > y1) {
      temp = y0; y0 = y1; y1 = temp;
      temp = x0; x0 = x1; x1 = temp;
   }

   *(target + x0 + y0 * videoInstance.resx) = COLOR(7,0,0);

   if ((dx = x1 - x0) >= 0) {
      xDir = 1;
   } else {
      xDir = -1;
      dx = -dx;
   }

   // Horizontal line
   if ((dy = y1 - y0) == 0) {
      while (dx-- != 0) {
         x0 += xDir;
         *(target + x0 + y0 * videoInstance.resx) = COLOR(7,0,0);
      }
      return;
   }

   // Vertical line
   if (dx == 0) {
      do {
         y0++;
         *(target + x0 + y0 * videoInstance.resx) = COLOR(7,0,0);
      } while (--dy != 0);
      return;
   }

   // Diagonal line
   if (dx == dy) {
      do {
         x0 += xDir;
         y0++;
         *(target + x0 + y0 * videoInstance.resx) = COLOR(7,0,0);
      } while (--dy != 0);
      return;
   }

   // Line is not horizontal, diagonal, or vertical
   errorAcc = 0;

   if (dy > dx) {
      errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;
      while (--dy) {
         errorAccTemp = errorAcc;
         errorAcc += errorAdj;
         if (errorAcc <= errorAccTemp) {
            x0 += xDir;
         }
         y0++;
         weighting = errorAcc >> INTENTSITY_SHIFT;

         *(target + x0 + y0 * videoInstance.resx) = COLOR(NUM_LEVELS - weighting,0,0);
         *(target + x0 + xDir + y0 * videoInstance.resx) = COLOR(weighting,0,0);

      }
      *(target + x1 + y1 * videoInstance.resx) = COLOR(7,0,0);
      return;
   }

   errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

   while (--dx) {
      errorAccTemp = errorAcc;
      errorAcc += errorAdj;
      if (errorAcc <= errorAccTemp) {
         y0++;
      }
      x0 += xDir;
      weighting = errorAcc >> INTENTSITY_SHIFT;

      *(target + x0 + y0 * videoInstance.resx) = COLOR(NUM_LEVELS - weighting,0,0);
      *(target + x0 + ((y0 + 1) * videoInstance.resx)) = COLOR(weighting,0,0);

   }

   *(target + x1 + y1 * videoInstance.resx) = COLOR(7,0,0);
}

uint8_t colorDim[8][8] = {
		{0,	0,	0,	0,	0,	0,	0,	0},
		{0,	0,	0,	0,	1,	1,	1,	1},
		{0,	0,	1,	1,	1,	1,	2,	2},
		{0,	0,	1,	1,	2,	2,	3,	3},
		{0,	1,	1,	2,	3,	3,	3,	4},
		{0,	1,	2,	2,	3,	3,	4,	5},
		{0,	1,	2,	3,	4,	4,	5,	6},
		{0,	1,	2,	3,	4,	5,	6,	7}
};

void drawWuLineColor (uint8_t *target, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t r, uint8_t g, uint8_t b) {

   uint8_t colorFull = COLOR(r,g,b);

   uint16_t errorAdj, errorAcc;
   uint16_t errorAccTemp, weighting;
   int16_t dx, dy, temp, xDir;

   if (y0 > y1) {
      temp = y0; y0 = y1; y1 = temp;
      temp = x0; x0 = x1; x1 = temp;
   }

   *(target + x0 + y0 * videoInstance.resx) = colorFull;

   if ((dx = x1 - x0) >= 0) {
      xDir = 1;
   } else {
      xDir = -1;
      dx = -dx;
   }

   // Horizontal line
   if ((dy = y1 - y0) == 0) {
      while (dx-- != 0) {
         x0 += xDir;
         *(target + x0 + y0 * videoInstance.resx) = colorFull;
      }
      return;
   }

   // Vertical line
   if (dx == 0) {
      do {
         y0++;
         *(target + x0 + y0 * videoInstance.resx) = colorFull;
      } while (--dy != 0);
      return;
   }

   // Diagonal line
   if (dx == dy) {
      do {
         x0 += xDir;
         y0++;
         *(target + x0 + y0 * videoInstance.resx) = colorFull;
      } while (--dy != 0);
      return;
   }

   // Line is not horizontal, diagonal, or vertical
   errorAcc = 0;

   if (dy > dx) {
      errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;
      while (--dy) {
         errorAccTemp = errorAcc;
         errorAcc += errorAdj;
         if (errorAcc <= errorAccTemp) {
            x0 += xDir;
         }
         y0++;
         weighting = errorAcc >> INTENTSITY_SHIFT;

         *(target + x0 + y0 * videoInstance.resx) = COLOR(
        		 colorDim[NUM_LEVELS - weighting][r],
        		 colorDim[NUM_LEVELS - weighting][g],
        		 colorDim[NUM_LEVELS - weighting][b]);
         *(target + x0 + xDir + y0 * videoInstance.resx) = COLOR(
        		 colorDim[weighting][r],
        		 colorDim[weighting][g],
        		 colorDim[weighting][b]);

      }
      *(target + x1 + y1 * videoInstance.resx) = colorFull;
      return;
   }

   errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

   while (--dx) {
      errorAccTemp = errorAcc;
      errorAcc += errorAdj;
      if (errorAcc <= errorAccTemp) {
         y0++;
      }
      x0 += xDir;
      weighting = errorAcc >> INTENTSITY_SHIFT;

      *(target + x0 + y0 * videoInstance.resx) = COLOR(
     		 colorDim[NUM_LEVELS - weighting][r],
     		 colorDim[NUM_LEVELS - weighting][g],
     		 colorDim[NUM_LEVELS - weighting][b]);
      *(target + x0 + ((y0 + 1) * videoInstance.resx)) = COLOR(
     		 colorDim[weighting][r],
     		 colorDim[weighting][g],
     		 colorDim[weighting][b]);

   }

   *(target + x1 + y1 * videoInstance.resx) = colorFull;
}


