//
//  uiDraw.h
//  Skeet
//
//  Created by Journey Curtis on 3/29/22.
//

#ifndef uiDraw_h
#define uiDraw_h

#include "point.h"
#include <string>

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

/*****************************
 * SKEET :: UIDRAW CLASS
 * Adapter that handles all
 * the various drawing methods
 ****************************/
class uiDraw {
private:
   void glVertexPoint(const Point& point) const {
      glVertex2f((GLfloat)point.getX(), (GLfloat)point.getY());
   }
   
   void drawDisk(const Point& center, double radius, double red, double green, double blue);
   void drawLine(const Point& begin, const Point& end, double red, double green, double blue) const;
   void drawDot(const Point& point, double radius, double red, double green, double blue) const;
   
public:
   // Bird drawing methods
   void drawStandard(Point pt, double radius);
   void drawSinker(Point pt, double radius);
   void drawFloater(Point pt, double radius);
   void drawCrazy(Point pt, double radius);
   
   // Bullet drawing methods
   void drawPellet(Point pt);
   void drawMissile(Point pt, Velocity v);
   void drawBomb(Point pt, double radius);
   void drawShrapnel(Point pt, double radius);
   
   // Effects drawing methods
   void drawFragment(Point pt, double age, double size) const;
   void drawStreek(Point pt, Point ptEnd, double age) const;
   void drawExhaust(Point pt, Point ptEnd, double age) const;
   void drawBackground(Point dimensions, double redBack,
                       double greenBack, double blueBack) const;
   void drawTimer(Point dimensions, double percent,
                  double redFore, double greenFore, double blueFore,
                  double redBack, double greenBack, double blueBack) const;
   void drawText(const Point& topLeft, const char* text) const;
   void drawText(const Point& topLeft, const std::string& text) const {
      drawText(topLeft, text.c_str());
   }
};

#endif /* uiDraw_h */
