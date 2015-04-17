// Core
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
//Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
// Self
//#include "init.h"
#include "objects.h"
#include <cstdlib>

using namespace std;

//Maybe move this to a config file
const int asteoridNumber = 1;
bool debug = false;
const float FPS = 20;
const int SCREEN_W = 800;
const int SCREEN_H = 600;
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_SPACE
};
enum NAMES {
   OBJECT, SPACESHIP, ASTEROID, SHOOT
};


int main(int argc, char **argv)
{

   srand(static_cast<unsigned int>(time(0)));
   
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *bouncer = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue;
   bool key[] = { false, false, false, false, false, false };
   bool redraw = true;
   bool doexit = false;


   //Init code
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return -1;
   }

   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }

   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }
   
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

   float spaceshipStartSpeed  = 0;
   Spaceship* spaceship = new Spaceship(300,300, spaceshipStartSpeed);

   //vector<Object*> objects;
   //objects.push_back(spaceship);
   vector<Asteroid*> asteroids;
   vector<Shoot*> shoots;

   for (int i = 0; i < asteoridNumber; i++) {
      int x = rand() % 760  + 20;
      int y = rand() % 560 + 20;
      float dir = 1.5;
      float speed = 2;
      float spin = .009;
      Asteroid* asteroid = new Asteroid(x, y, speed, dir, spin);
      //objects.push_back(asteroid);
      asteroids.push_back(asteroid);
   }

   al_flip_display();
   al_start_timer(timer);

   while(!doexit) {

      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
 

      if(event.type == ALLEGRO_EVENT_TIMER) {
         //Updating everything
         spaceship->Update();
         for (int i = 0; i < asteroids.size(); i++) {
            asteroids[i]->Update();
         }
         for (int i = 0; i < shoots.size(); i++) {
            shoots[i]->Update();
         }
         // TODO: make this an iterator
         /*

         for (int i = 0; i < objects.size(); i++) {
            Object* currentObject = objects[i];
            currentObject->Update();
            if (debug) {
               if (i !=0)
               currentObject->DebugPrint();
            }
         };
         */
//THIS IS THE SEG FAULT
         for (int i = 0; i < shoots.size(); i++){
            Shoot* shoot = shoots[i];
               // if the shoot it ouside the boundaries
               if(
                  (shoot->x * shoot->y) < 0 ||
                  shoot->x > SCREEN_W || 
                  shoot->y > SCREEN_H ) {
                  cout << "ouside" << endl;
                  delete shoot;
                  shoots.erase(shoots.begin() + i);

               } else {

               for (int j = 0; j < asteroids.size(); j++) {
                  Asteroid * asteroid = asteroids[j];
                  if (
                      shoot->x > asteroid->x - 25 &&
                      shoot->x < asteroid->x + 20 &&
                      shoot->y > asteroid->y - 20 &&
                      shoot->y < asteroid->y + 20
                      ) {
                     cout << "bam" << endl;
                     delete shoot;
                     delete asteroids[j];
                     shoots.erase(shoots.begin() + i);
                     asteroids.erase(asteroids.begin() + j);
                  }
               }
            }
               // if it shoot an asteroid:
               
            }


            // check if the object has to be destroied
         
         redraw = true;

         if(key[KEY_UP]) {
            //cout <<"UPPING" << endl;
            spaceship->accelerate();
         }
 
         if(key[KEY_DOWN]) {
            //cout <<"DOWNING" << endl;
            spaceship->decelerate();
         }
 
         if(key[KEY_LEFT]) {
            //cout <<"LEFTING" << endl;
            spaceship->moveLeft();
         }
 
         if(key[KEY_RIGHT]) {
            //cout <<"RIGHTING" << endl;
            spaceship->moveRight();
         }
 
         if(key[KEY_SPACE]) {
            //cout <<"FIRING" << endl;
            Shoot* blast = spaceship->Fire();
            //objects.push_back(blast);
            shoots.push_back(blast);
         }

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
               key[KEY_SPACE] = false;
               break;
 
            case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }
      }
      // close display
      if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }

      // redraw
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         al_clear_to_color(al_map_rgb(0,0,0));

         spaceship->Draw();
         for (int i = 0; i < asteroids.size(); i++) {
            asteroids[i]->Draw();
         }

         for (int i = 0; i < shoots.size(); i++) {
            shoots[i]->Draw();
         }
         al_flip_display();
      }
   } 

   // game was quitted
   al_destroy_display(display);
 /*
   for (int i = 0; i < objects.size(); i++) {
      delete objects[i];
   }
*/
   
   return 0;
}