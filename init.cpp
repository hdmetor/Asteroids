#include <allegro5/allegro.h>
 
int init( float FPS = 60,  int SCREEN_W = 800,  int SCREEN_H = 600) {

	if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

    if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return -1;
   }
   return 0;
}