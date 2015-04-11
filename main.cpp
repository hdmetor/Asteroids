#include <stdio.h>
#include <allegro5/allegro.h>
#include "init.h"
#include <allegro5/allegro_primitives.h>
#include "spaceship.h"
#include <iostream>

using namespace std;

float FPS = 60;
int SCREEN_W = 800;
int SCREEN_H = 600;

const int BOUNCER_SIZE = 32;
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
 
int main(int argc, char **argv)
{
   if (!init(60, 800, 600)){
      return -1;
   }
   
   ALLEGRO_DISPLAY *display = NULL;
   //ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *bouncer = NULL;
   float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
   float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
   bool key[4] = { false, false, false, false };
   bool redraw = true;
   bool doexit = false;
 
   // init code
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   // init display
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }

   al_clear_to_color(al_map_rgb(0,0,0));
   
   Spaceship* spaceship = new Spaceship(100,100);
   spaceship->DrawSpaceship();

   ALLEGRO_EVENT_QUEUE *event_queue; 
   event_queue = al_create_event_queue(); 
      if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_bitmap(bouncer);
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }

   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_clear_to_color(al_map_rgb(0,0,0));
 
   al_flip_display();
 
   al_start_timer(timer);
   while(!doexit) {

      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
 
      if(event.type == ALLEGRO_EVENT_TIMER) {
         spaceship->passTime();
         redraw = true;
      }

      if (event.type == ALLEGRO_EVENT_KEY_DOWN) { 
         switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
               redraw = true;
               spaceship->moveLeft();
               break;
            case ALLEGRO_KEY_RIGHT:
               redraw = true;
               spaceship->moveRight();
               break;
            case ALLEGRO_KEY_SPACE:
               spaceship->Fire();
               redraw = true;
               break; 
            case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }

      } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         al_clear_to_color(al_map_rgb(0,0,0));
         spaceship->DrawSpaceship();
         al_flip_display();
      }
   } 
   
   al_destroy_display(display);
 
   delete spaceship;
   return 0;
}