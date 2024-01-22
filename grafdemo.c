/**
 * Very simple graphics demo.
 */

#include "graphics.h"

int main( ) {

  grafmode();  // Init hires mode.

 // for( int i=0, i < 320; ++i ) {

    line( 1, 0, 319-1, 199, 1);
 // }

  textmode();
  
  return 0;
}
