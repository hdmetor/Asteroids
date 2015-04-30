#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "init.h"
 
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *bouncer = NULL;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_FONT *font = NULL;

using namespace std;

bool InitAllegro( float FPS,  int SCREEN_W,  int SCREEN_H) {

if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return false;
   }
 
   if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return false;
   }

   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return false;
   }

   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return false;
   }

   al_init_font_addon(); 
   al_init_ttf_addon();
   font = al_load_ttf_font("src/comicsans.ttf", 16, 0);

   if (!font){
      fprintf(stderr, "Could not load font.\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      al_destroy_font(font);
      return false;
   }
 
   event_queue = al_create_event_queue(); 
      if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      al_destroy_font(font);
      return false;
   }

   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_clear_to_color(al_map_rgb(0,0,0));
   return true;
}