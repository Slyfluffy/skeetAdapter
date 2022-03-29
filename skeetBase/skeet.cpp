/**********************************************************************
 * Skeet
 * No actual birds were killed in the making of this game.
 **********************************************************************/

#include <string>
#include <sstream>
#include "skeet.h"
using namespace std;


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_18
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied 
#define _USE_MATH_DEFINES
#include <math.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // _WIN32

/************************
 * SKEET ANIMATE
 * move the gameplay by one unit of time
 ************************/
void Skeet::animate()
{
   time++;
   
   // if status, then do not move the game
   if (time.isStatus())
   {
      // get rid of the bullets and the birds without changing the score
      birds.clear();
      bullets.clear();
      effects.clear();
      return;
   }
   
   // spawn
   spawn();
   
   // move the birds and the bullets
   for (auto element : birds)
   {
      element->advance();
      hitRatio.adjust(element->isDead() ? -1 : 0);
   }
   for (auto bullet : bullets)
      bullet->move(effects);
   for (auto effect : effects)
      effect->fly();
      
   // hit detection
   for (auto element : birds)
      for (auto bullet : bullets)
         if (!element->isDead() && !bullet->isDead() &&
             element->getRadius() + bullet->getRadius() >
             minimumDistance(element->getPosition(), element->getVelocity(),
                             bullet->getPosition(),  bullet->getVelocity()))
         {
            for (int i = 0; i < 25; i++)
               effects.push_back(new Fragment(bullet->getPosition(), bullet->getVelocity()));
            element->kill();
            bullet->kill();
            hitRatio.adjust(1);
         }
   
   // remove the zombie birds
   for (auto it = birds.begin(); it != birds.end();)
      if ((*it)->isDead())
      {
         score.adjust((*it)->getPoints());
         it = birds.erase(it);
      }
      else
         ++it;
       
   // remove zombie bullets
   for (auto it = bullets.begin(); it != bullets.end(); )
      if ((*it)->isDead())
      {
         (*it)->death(bullets);
         it = bullets.erase(it);
      }
      else
         ++it;
   
   // remove zombie fragments
   for (auto it = effects.begin(); it != effects.end();)
      if ((*it)->isDead())
         it = effects.erase(it);
      else
         ++it;
}

/************************
 * SKEET DRAW LEVEL
 * output everything that will be on the screen
 ************************/
void Skeet::drawLevel() const
{
   // output the background
   drawTool.drawBackground(dimensions, time.level() * .1, 0.0, 0.0);
   
   // output the gun
   gun.display();
         
   // output the birds, bullets, and fragments
   for (auto effect : effects)
      effect->render();
   for (auto bullet : bullets)
      bullet->output();
   for (auto element : birds)
      element->draw();
   
   // status
   drawTool.drawText(Point(10, dimensions.getY() - 30), score.getText());
   drawTool.drawText(Point(dimensions.getX() / 2 - 30,  dimensions.getY() - 30), time.getText());
   drawTool.drawText(Point(dimensions.getX() - 110, dimensions.getY() - 30), hitRatio.getText());
}

/************************
 * SKEET DRAW STATUS
 * place the status message on the center of the screen
 ************************/
void Skeet::drawStatus() const
{
   // output the text information
   ostringstream sout;
   if (time.isGameOver())
   {
      // draw the end of game message
      drawTool.drawText(Point(dimensions.getX() / 2 - 30, dimensions.getY() / 2 + 10),
               "Game Over");

      // draw end of game status
      drawTool.drawText(Point(dimensions.getX() / 2 - 30, dimensions.getY() / 2 - 10),
               score.getText());
   }
   else
   {
      // output the status timer
      drawTool.drawTimer(dimensions, 1.0 - time.percentLeft(),
                     (time.level() - 0.0) * .1, 0.0, 0.0,
                     (time.level() - 1.0) * .1, 0.0, 0.0);

      // draw the message giving a countdown
      sout << "Level " << time.level()
           << " begins in " << time.secondsLeft() << " seconds";
      drawTool.drawText(Point(dimensions.getX() / 2 - 110, dimensions.getY() / 2 - 10),
         sout.str());
   }
}

/************************
 * SKEET INTERACT
 * handle all user input
 ************************/
void Skeet::interact(const UserInput & ui)
{
   // reset the game
   if (time.isGameOver() && ui.isSpace())
   { 
      time.reset();
      score.reset();
      hitRatio.reset();
      return;
   }

   // gather input from the interface
   gun.interact(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft());
   Bullet *p = nullptr;

   // a pellet can be shot at any time
   if (ui.isSpace())
      p = new Pellet(gun.getAngle());
   // missiles can be shot at level 2 and higher
   else if (ui.isM() && time.level() > 1)
      p = new Missile(gun.getAngle());
   // bombs can be shot at level 3 and higher
   else if (ui.isB() && time.level() > 2)
      p = new Bomb(gun.getAngle());
   
   // add something if something has been added
   if (nullptr != p)
   {
      bullets.push_back(p);
      score.adjust(0 - p->getValue());
   }
   
   // send movement information to all the bullets. Only the missile cares.
   for (auto bullet : bullets)
      bullet->input(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft(), ui.isB()); 
}

/******************************************************************
 * RANDOM
 * This function generates a random number.
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/************************
 * SKEET SPAWN
 * lanuch new birds
 ************************/
void Skeet::spawn()
{
   double size;
   switch (time.level())
   {
      // in level 1 spawn big birds occasionally
      case 1:
         size = 30.0;
         // spawns when there is nothing on the screen
         if (birds.size() == 0 && random(0, 15) == 1)
            birds.push_back(new Standard(size, 7.0));
         
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Standard(size, 7.0));
         break;
         
      // two kinds of birds in level 2
      case 2:
         size = 25.0;
         // spawns when there is nothing on the screen
         if (birds.size() == 0 && random(0, 15) == 1)
            birds.push_back(new Standard(size, 7.0, 12));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Standard(size, 5.0, 12));
         // spawn every 3 seconds
         if (random(0, 3 * 30) == 1)
            birds.push_back(new Sinker(size));
         break;
      
      // three kinds of birds in level 3
      case 3:
         size = 20.0;
         // spawns when there is nothing on the screen
         if (birds.size() == 0 && random(0, 15) == 1)
            birds.push_back(new Standard(size, 5.0, 15));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Standard(size, 5.0, 15));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Sinker(size, 4.0, 22));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Floater(size));
         break;
         
      // three kinds of birds in level 4
      case 4:
         size = 15.0;
         // spawns when there is nothing on the screen
         if (birds.size() == 0 && random(0, 15) == 1)
            birds.push_back(new Standard(size, 4.0, 18));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Standard(size, 4.0, 18));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Sinker(size, 3.5, 25));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Floater(size, 4.0, 25));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Crazy(size));
         break;
         
      default:
         break;
   }
}
