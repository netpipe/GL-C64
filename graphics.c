/**
 * Some simple graphics routines for the commodore 64.
 *
 * Author(s): Andreas Rueckert <arueckert67@t-online.de>
 */

#include "graphics.h"
#include <string.h>


/**
 * A pointer to the screen buffer.
 */
unsigned char *screenbuffer;


/**
 * Enter the graphics mode.
 */
void grafmode( void) {

  unsigned char *vic = (unsigned char *)0xd000;

  vic[17] |= 0x20;  // Set bit 5 of $d011 to activate hires mode.
  vic[24] |= 0x08;  // Set screenbuf to 8192;

  vic[32] = 0;  // Set border color to black.
  
  // Set a pointer to the screen buffer.
  screenbuffer = (unsigned char *)8192;

  // Clear the screen.
  memset( screenbuffer, 0, 8000);

  // Clear the text ram (used as color ram now).
  memset( (void *)1024, 5*16+0, 1000);
}

/* signum function */
int sgn(int x){
  return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}

/**
 * Draw a line.
 * 
 * @param x1 The horizontal start coordinate.
 * @param y1 The vertical start coordinate.
 * @param x2 The horizontal end coordinate.
 * @param y2 The vertical end coordinate.
 * @param color 1 = foreground color, 0 = background color.
 */
void line( unsigned int x1
	   , unsigned char y1
	   , unsigned int x2
	   , unsigned char y2
	   , unsigned char color) {

  // Implementation taken from
  // https://de.wikipedia.org/wiki/Bresenham-Algorithmus#C-Implementierung  
  int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

  /* Entfernung in beiden Dimensionen berechnen */
   dx = x2 - x1;
   dy = y2 - y1;

/* Vorzeichen des Inkrements bestimmen */
   incx = sgn(dx);
   incy = sgn(dy);
   if(dx<0) dx = -dx;
   if(dy<0) dy = -dy;

/* feststellen, welche Entfernung größer ist */
   if( dx > dy) {
      /* x ist schnelle Richtung */
      pdx=incx; pdy=0;    /* pd. ist Parallelschritt */
      ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
      es =dy;   el =dx;   /* Fehlerschritte schnell, langsam */
   } else {
      /* y ist schnelle Richtung */
      pdx=0;    pdy=incy; /* pd. ist Parallelschritt */
      ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
      es =dx;   el =dy;   /* Fehlerschritte schnell, langsam */
   }

   /* Initialisierungen vor Schleifenbeginn */
   x = x1;
   y = y1;
   err = el/2;
   plot(x,y,color);

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
      plot(x,y,color);
   }
}

/**
 * Plot a pixel.
 * 
 * @param x The horizontal coordinate.
 * @param y The vertical coordinate.
 * @param color 1 = foreground color, 0 = background color.
 */
void plot( unsigned int x, unsigned char y, unsigned char color) {

  // Compute the address of the byte to modify.
  unsigned int ytextline = y >> 3;
  unsigned int addy = (ytextline << 8)
    + (ytextline << 6)
    + (x & 0xfff8)
    + (y & 7);

  // Compute the bitmask to use.
  unsigned char bitmask = (0x80 >> ( x & 7));

  if( color == 1) {
    screenbuffer[addy] |= bitmask;
  } else {
    screenbuffer[addy] &= ~bitmask;
  }
}

/**
 * Return to the text mode.
 */
void textmode( void) {

  unsigned char *vic = (unsigned char *)0xd000;
 
  vic[17] &= 0xdf;  // Clear hires bit.
}
