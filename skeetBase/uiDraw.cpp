//
//  uiDraw.cpp
//  Lab03
//
//  Created by Journey Curtis on 3/29/22.
//

#include <cassert>
#include "uiDraw.h"

/***************************************************************/
/***************************************************************/
/*                           DRAW BIRD                         */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * DRAW Disk
 * Draw a filled circule at [center] with size [radius]
 *************************************************************************/
void uiDraw::drawDisk(const Point& center, double radius,
              double red, double green, double blue) {
   assert(radius > 1.0);
   const double increment = M_PI / radius;  // bigger the circle, the more increments

   // begin drawing
   glBegin(GL_TRIANGLES);
   glColor3f((GLfloat)red /* red % */, (GLfloat)green /* green % */, (GLfloat)blue /* blue % */);

   // three points: center, pt1, pt2
   Point pt1;
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   Point pt2(pt1);

   // go around the circle
   for (double radians = increment;
      radians <= M_PI * 2.0 + .5;
      radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));

      glVertex2f((GLfloat)center.getX(), (GLfloat)center.getY());
      glVertex2f((GLfloat)pt1.getX(), (GLfloat)pt1.getY());
      glVertex2f((GLfloat)pt2.getX(), (GLfloat)pt2.getY());

      pt1 = pt2;
   }

   // complete drawing
   glEnd();
}

/*********************************************
 * STANDARD DRAW
 * Draw a standard bird: blue center and white outline
 *********************************************/
void uiDraw::drawStandard(Point pt, double radius) {
   drawDisk(pt, radius - 0.0, 1.0, 1.0, 1.0); // white outline
   drawDisk(pt, radius - 3.0, 0.0, 0.0, 1.0); // blue center
}

/*********************************************
 * SINKER DRAW
 * Draw a sinker bird: black center and dark blue outline
 *********************************************/
void uiDraw::drawSinker(Point pt, double radius) {
   drawDisk(pt, radius - 0.0, 0.0, 0.0, 0.8);
   drawDisk(pt, radius - 4.0, 0.0, 0.0, 0.0);
}

/*********************************************
 * FLOATER DRAW
 * Draw a floating bird: white center and blue outline
 *********************************************/
void uiDraw::drawFloater(Point pt, double radius) {
   drawDisk(pt, radius - 0.0, 0.0, 0.0, 1.0); // blue outline
   drawDisk(pt, radius - 4.0, 1.0, 1.0, 1.0); // white center
}

/*********************************************
 * CRAZY DRAW
 * Draw a crazy bird: concentric circles in a course gradient
 *********************************************/
void uiDraw::drawCrazy(Point pt, double radius) {
      drawDisk(pt, radius * 1.0, 0.0, 0.0, 1.0); // bright blue outside
      drawDisk(pt, radius * 0.8, 0.2, 0.2, 1.0);
      drawDisk(pt, radius * 0.6, 0.4, 0.4, 1.0);
      drawDisk(pt, radius * 0.4, 0.6, 0.6, 1.0);
      drawDisk(pt, radius * 0.2, 0.8, 0.8, 1.0); // almost white inside
}

/***************************************************************/
/***************************************************************/
/*                         DRAW BULLETS                        */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *************************************************************************/
void uiDraw::drawLine(const Point& begin, const Point& end,
                      double red, double green, double blue) const {
   // Get ready...
   glBegin(GL_LINES);
   glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);

   // Draw the actual line
   glVertexPoint(begin);
   glVertexPoint(end);

   // Complete drawing
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point (square actually on the screen, r pixels by r pixels
 *************************************************************************/
void uiDraw::drawDot(const Point& point, double radius,
                     double red, double green, double blue) const {
   // Get ready, get set...
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);
   double r = radius / 2.0;

   // Go...
   glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() - r));
   glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() - r));
   glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() + r));
   glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() + r));

   // Done!  OK, that was a bit too dramatic
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/*********************************************
 * PELLET OUTPUT
 * Draw a pellet - just a 3-pixel dot
 *********************************************/
void uiDraw::drawPellet(Point pt) {
   drawDot(pt, 3.0, 1.0, 1.0, 0.0);
}

/*********************************************
 * BOMB OUTPUT
 * Draw a bomb - many dots to make it have a soft edge
 *********************************************/
void uiDraw::drawBomb(Point pt, double radius) {
   // Bomb actually has a gradient to cut out the harsh edges
   drawDot(pt, radius + 2.0, 0.50, 0.50, 0.00);
   drawDot(pt, radius + 1.0, 0.75, 0.75, 0.00);
   drawDot(pt, radius + 0.0, 0.87, 0.87, 0.00);
   drawDot(pt, radius - 1.0, 1.00, 1.00, 0.00);
}

/*********************************************
 * SHRAPNEL OUTPUT
 * Draw a fragment - a bright yellow dot
 *********************************************/
void uiDraw::drawShrapnel(Point pt, double radius) {
   drawDot(pt, radius, 1.0, 1.0, 0.0);
}

/*********************************************
 * MISSILE OUTPUT
 * Draw a missile - a line and a dot for the fins
 *********************************************/
void uiDraw::drawMissile(Point pt, Velocity v) {
   // missile is a line with a dot at the end so it looks like fins.
   Point ptNext(pt);
   ptNext.add(v);
   drawLine(pt, ptNext, 1.0, 1.0, 0.0);
   drawDot(pt, 3.0, 1.0, 1.0, 1.0);
}

/***************************************************************/
/***************************************************************/
/*                         DRAW EFFECTS                        */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * FRAGMENT RENDER
 * Draw the fragment on the screen
 *************************************************************************/
void uiDraw::drawFragment(Point pt, double age, double size) const {
    // Draw this sucker
    glBegin(GL_TRIANGLE_FAN);
    
    // the color is a function of age - fading to black
    glColor3f((GLfloat)age, (GLfloat)age, (GLfloat)age);
    
    // draw the fragment
    glVertex2f((GLfloat)(pt.getX() - size), (GLfloat)(pt.getY() - size));
    glVertex2f((GLfloat)(pt.getX() + size), (GLfloat)(pt.getY() - size));
    glVertex2f((GLfloat)(pt.getX() + size), (GLfloat)(pt.getY() + size));
    glVertex2f((GLfloat)(pt.getX() - size), (GLfloat)(pt.getY() + size));
    glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
    glEnd();
}

/************************************************************************
 * STREEK RENDER
 * Draw the shrapnel streek on the screen
 *************************************************************************/
void uiDraw::drawStreek(Point pt, Point ptEnd, double age) const {
    // Draw this sucker
    glBegin(GL_LINES);
    glColor3f((GLfloat)age, (GLfloat)age, (GLfloat)age);

    // Draw the actual line
    glVertex2f((GLfloat)pt.getX(), (GLfloat)pt.getY());
    glVertex2f((GLfloat)ptEnd.getX(), (GLfloat)ptEnd.getY());

    glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
    glEnd();
}

/************************************************************************
 * EXHAUST RENDER
 * Draw a missile exhaust on the screen
 *************************************************************************/
void uiDraw::drawExhaust(Point pt, Point ptEnd, double age) const {   
   // Draw this sucker
   glBegin(GL_LINES);
   glColor3f((GLfloat)age, (GLfloat)age, (GLfloat)age);

   // Draw the actual line
   glVertex2f((GLfloat)pt.getX(), (GLfloat)pt.getY());
   glVertex2f((GLfloat)ptEnd.getX(), (GLfloat)ptEnd.getY());

   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/***************************************************************/
/***************************************************************/
/*                          DRAW EXTRAS                        */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * DRAW Background
 * Fill in the background
 *  INPUT color   Background color
 *************************************************************************/
void uiDraw::drawBackground(Point dimensions, double redBack,
                            double greenBack, double blueBack) const {
   glBegin(GL_TRIANGLE_FAN);

   // two rectangles is the fastest way to fill the screen.
   glColor3f((GLfloat)redBack /* red % */, (GLfloat)greenBack /* green % */, (GLfloat)blueBack /* blue % */);
   glVertex2f((GLfloat)0.0, (GLfloat)0.0);
   glVertex2f((GLfloat)dimensions.getX(), (GLfloat)0.0);
   glVertex2f((GLfloat)dimensions.getX(), (GLfloat)dimensions.getY());
   glVertex2f((GLfloat)0.0, (GLfloat)dimensions.getY());

   glEnd();
}

/************************************************************************
 * DRAW Timer
 * Draw a large timer on the screen
 *  INPUT percent     Amount of time left
 *        Foreground  Foreground color
 *        Background  Background color
 *************************************************************************/
void uiDraw::drawTimer(Point dimensions, double percent,
                     double redFore, double greenFore, double blueFore,
                     double redBack, double greenBack, double blueBack) const {
   double radians;

   GLfloat length = (GLfloat)dimensions.getX();
   GLfloat half = length / (GLfloat)2.0;

   // do the background stuff
   drawBackground(dimensions, redBack, greenBack, blueBack);

   // foreground stuff
   radians = percent * M_PI * 2.0;
   GLfloat x_extent = half + length * (GLfloat)sin(radians);
   GLfloat y_extent = half + length * (GLfloat)cos(radians);

   // get read to draw the triangles
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)redFore /* red % */, (GLfloat)greenFore /* green % */, (GLfloat)blueFore /* blue % */);
   glVertex2f(half, half);

   // fill in the triangles, one eight at a time
   switch ((int)(percent * 8.0))
   {
   case 7: // 315 - 360
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, length);
      break;
   case 6: // 270 - 315
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, half);
      break;
   case 5: // 225 - 270
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      break;
   case 4: // 180 - 225
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(half, 0.0);
      break;
   case 3: // 135 - 180
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      glVertex2f(length, 0.0);
      break;
   case 2: // 90 - 135 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      break;
   case 1: // 45 - 90 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      break;
   case 0: // 0 - 45 degrees
      glVertex2f(half, length);
      break;
   }
   glVertex2f(x_extent, y_extent);

   // complete drawing
   glEnd();

   // draw the red line now
   glBegin(GL_LINES);
   glColor3f((GLfloat)0.6, (GLfloat)0.0, (GLfloat)0.0);
   glVertex2f(half, half);
   glVertex2f(x_extent, y_extent);
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void uiDraw::drawText(const Point& topLeft, const char* text) const
{
   void* pFont = GLUT_TEXT;
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);

   // prepare to output the text from the top-left corner
   glRasterPos2f((GLfloat)topLeft.getX(), (GLfloat)topLeft.getY());

   // loop through the text
   for (const char* p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}
