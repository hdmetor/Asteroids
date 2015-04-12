#include <stdio.h>
#include <allegro5/allegro.h>
#include "init.h"
#include <allegro5/allegro_primitives.h>
#include "objects.h"
#include <iostream>

using namespace std;

float FPS = 60;
int SCREEN_W = 800;
int SCREEN_H = 600;

const int BOUNCER_SIZE = 32;
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_SPACE
};
 
int main(int argc, char **argv)
{
   if (!init(60, 800, 600)){
      return -1;
   }
   
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *bouncer = NULL;
   bool key[] = { false, false, false, false, false, false };
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
   
   Spaceship* spaceship = new Spaceship(100,100, 0);
   spaceship->Draw();

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
 
      // allow keeping key pressed
      if(event.type == ALLEGRO_EVENT_TIMER) {

         if(key[KEY_UP]) {
            spaceship->accelerate();
            cout << "UPPING" << endl;
         }
 
         if(key[KEY_DOWN]) {
            spaceship->decelerate();
            cout << "DOWING" << endl;
         }
 
         if(key[KEY_LEFT]) {
            spaceship->moveLeft();
            cout << "LEFTING" << endl;
         }
 
         if(key[KEY_RIGHT]) {
            spaceship->moveRight();
            cout << "RIGHTING" << endl;
         }
 
         if(key[KEY_SPACE]) {
            spaceship->Fire();
            cout << "FIRING" << endl;
         }

         spaceship->passTime();
         redraw = true;
      }
      // press a key
      else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               break;
 
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = true;
               break;
 
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;
               break;

            case ALLEGRO_KEY_SPACE:
               key[KEY_SPACE] = true;
               break;
         }
      }
      // release a key
      else if(event.type == ALLEGRO_EVENT_KEY_UP) {
         switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;
 
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = false;
               break;
 
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = false;
               break;

            case ALLEGRO_KEY_SPACE:
               key[KEY_SPACE] = true;
               break;
 
            case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }
      }

      if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         al_clear_to_color(al_map_rgb(0,0,0));
         spaceship->Draw();
         al_flip_display();
      }
   } 
   
   al_destroy_display(display);
 
   delete spaceship;
   return 0;
}