#include "main.h"
#include "simple.h"


// Taken and adapted from http://en.wikipedia.org/wiki/Midpoint_circle_algorithm

/**
 * The '(x != 0 && y != 0)' test in the last line of this function
 * may be omitted for a performance benefit if the radius of the
 * circle is known to be non-zero.
 */
void plot4points(uint8_t *target, int cx, int cy, int x, int y) {
  drawPixel(target, cx + x, cy + y, 0xff);
  if (x != 0) drawPixel(target, cx - x, cy + y, 0xff);
  if (y != 0) drawPixel(target, cx + x, cy - y, 0xff);
  if (x != 0 && y != 0) drawPixel(target, cx - x, cy - y, 0xff);
}

/**
 *
 */
void plot8points(uint8_t *target, int cx, int cy, int x, int y)
{
  plot4points(target, cx, cy, x, y);
  if (x != y) plot4points(target, cx, cy, y, x);
}



/**
 *
 */
void drawCircle(uint8_t *target, uint16_t cx, uint16_t cy, uint16_t radius) {

	  int error = -radius;
	  int x = radius;
	  int y = 0;

	  // The following while loop may altered to 'while (x > y)' for a
	  // performance benefit, as long as a call to 'plot4points' follows
	  // the body of the loop. This allows for the elimination of the
	  // '(x != y)' test in 'plot8points', providing a further benefit.
	  //
	  // For the sake of clarity, this is not shown here.
	  while (x >= y) {
	    plot8points(target, cx, cy, x, y);

	    error += y;
	    ++y;
	    error += y;

	    // The following test may be implemented in assembly language in
	    // most machines by testing the carry flag after adding 'y' to
	    // the value of 'error' in the previous step, since 'error'
	    // nominally has a negative value.
	    if (error >= 0) {
	      error -= x;
	      --x;
	      error -= x;
	    }
	  }


}
