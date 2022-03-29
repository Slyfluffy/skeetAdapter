/***********************************************************************
 * Source File:
 *    Bullet
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that kills other things
 ************************************************************************/

#include "bullet.h"

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

/*********************************************
 * BULLET constructor
 *********************************************/
Bullet::Bullet(double angle, double speed, double radius, int value) :
   dead(false), radius(radius), value(value)
{
   // set the initial position
   pt.setX(dimensions.getX() - 1.0);
   pt.setY(1.0);
   assert(pt.getX() > 100.0);

   // set the initial velocity
   v.setDx(-speed * cos(angle));
   v.setDy(speed * sin(angle));
   assert(v.getDx() <= 0.0);
   assert(v.getDy() >= 0.0);
}

/*********************************************
 * BOMB DEATH
 * Bombs have a tendency to explode!
 *********************************************/
void Bomb::death(std::list<Bullet*>& bullets)
{
   for (int i = 0; i < 20; i++)
      bullets.push_back(new Shrapnel(*this));
}

 /***************************************************************/
 /***************************************************************/
 /*                             MOVE                            */
 /***************************************************************/
 /***************************************************************/

/*********************************************
 * BULLET MOVE
 * Move the bullet along by one time period
 *********************************************/
void Bullet::move(std::list<Effect*> & effects)
{
   // inertia
   pt.add(v);

   // out of bounds checker
   if (isOutOfBounds())
      kill();
}

/*********************************************
 * BOMB MOVE
 * Move the bomb along by one time period
 *********************************************/
void Bomb::move(std::list<Effect*> & effects)
{
    // kill if it has been around too long
    timeToDie--;
    if (!timeToDie)
        kill();

    // do the inertia thing
    Bullet::move(effects);
}

/*********************************************
 * MISSILE MOVE
 * Move the missile along by one time period
 *********************************************/
void Missile::move(std::list<Effect*> & effects)
{
    // kill if it has been around too long
    effects.push_back(new Exhaust(pt, v));

    // do the inertia thing
    Bullet::move(effects);
}

/*********************************************
 * SHRAPNEL MOVE
 * Move the shrapnel along by one time period
 *********************************************/
void Shrapnel::move(std::list<Effect*> & effects)
{
    // kill if it has been around too long
    timeToDie--;
    if (!timeToDie)
        kill();

    // add a streek
    effects.push_back(new Streek(pt, v));
    
    // do the usual bullet stuff (like inertia)
    Bullet::move(effects);
}

/***************************************************************/
/***************************************************************/
/*                             MISC.                           */
/***************************************************************/
/***************************************************************/

/******************************************************************
 * RANDOM
 * This function generates a random number.
 ****************************************************************/
int Bullet::random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);
   return num;
}
double Bullet::random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   assert(min <= num && num <= max);
   return num;
}
