/*
 * Sarah Godine Final Project
 * CSCI 4229
 *
 * Resources: 
 * My Previous Homeworks
 * ex14.c, ex15.c, ex19.c
 * https://www.opengl.org/archives/resources/code/samples/redbook/torus.c
 * 
 */

#include "CSCIx229.h"

//ex8
int th=0;         //  Azimuth of view angle
int ph=40;         //  Elevation of view angle
int axes=1;       //  Display axes
int mode=0;       //  What to display
// projection
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=10;   //  Size of world
int fps = 0;       // first person
int view = 0;     // which projection
int angle = 0;

//first person view coordinates
double fx = 0;
double fy = 0;
double fz = 10;

//camera
double cx = 0;
double cz = 0;

// Light values ex13
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   7;  // Elevation of light
int move=1;       //  Move light
int light=1;      //  Lighting
float shinyvec[1];

unsigned int texture[24]; // Texture names

//timer intitalizer
int frameNumber = 0;


#define PI 3.1416

//ex8
/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}


//ex 9
/*
 *  Set projection
 */
static void myProject()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   // first person
   if (fps) {
       gluPerspective(fov,asp,dim/4,4*dim);
   }
   else if (view) {
    glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
      
   }
  else {
    gluPerspective(fov,asp,dim/6,6*dim);
  }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
   glutIdleFunc(move?idle:NULL);
   glutPostRedisplay();
}


//ex8
/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   // glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   // glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
    double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void skybox(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  const double width = 18.5;
  const double height = 25;
  const double length = 20;

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  
  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
  glNormal3f(0.0,-1.0,0.0);
  glTexCoord2d(1.0,0.0); glVertex3f( width, height, -length);  // Top Right Of The Quad (Top)
  glTexCoord2d(1.0,1.0); glVertex3f(-width, height, -length);  // Top Left Of The Quad (Top)
  glTexCoord2d(0.0,1.0);  glVertex3f(-width, height, length); // Bottom Left Of The Quad (Top)
  glTexCoord2d(0.0,0.0);  glVertex3f( width, height, length); // Bottom Right Of The Quad (Top)
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
  glNormal3f(0.0,0.0,-1.0);
  glTexCoord2d(0.0,0.0); glVertex3f( width, height, length); // Top Right Of The Quad (Front)
  glTexCoord2d(1.0,0.0); glVertex3f(-width, height, length); // Top Left Of The Quad (Front)
  glTexCoord2d(1.0,1.0); glVertex3f(-width, 0, length);  // Bottom Left Of The Quad (Front)
  glTexCoord2d(0.0,1.0); glVertex3f( width, 0, length);  // Bottom Right Of The Quad (Front)
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
  glNormal3f(0.0,0.0,1.0);
  glTexCoord2d(0.0,0.0); glVertex3f( width, height, -length);  // Top Right Of The Quad (Back)
  glTexCoord2d(1.0,0.0); glVertex3f(-width, height, -length);  // Top Left Of The Quad (Back)
  glTexCoord2d(1.0,1.0); glVertex3f(-width, 0, -length); // Bottom Left Of The Quad (Back)
  glTexCoord2d(0.0,1.0); glVertex3f( width, 0, -length); // Bottom Right Of The Quad (Back)
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
  glNormal3f(1.0,0.0,0.0);
  glTexCoord2d(0.0,0.0); glVertex3f(-width, height, length);  // Top right Of The Quad (Left)
  glTexCoord2d(1.0,0.0); glVertex3f(-width, height, -length);  // Top Left Of The Quad (Left)
  glTexCoord2d(1.0,1.0); glVertex3f(-width, 0, -length); // Bottom Left Of The Quad (Left)
  glTexCoord2d(0.0,1.0); glVertex3f(-width, 0, length); // Bottom Right Of The Quad (Left)
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
  glNormal3f(-1.0,0.0,0.0);
  glTexCoord2d(0.0,0.0); glVertex3f( width, height, -length);  // Top Right Of The Quad (Right)
  glTexCoord2d(1.0,0.0); glVertex3f( width, height, length); // Top Left Of The Quad (Right)
  glTexCoord2d(1.0,1.0); glVertex3f( width, 0, length);  // Bottom Left Of The Quad (Right)
  glTexCoord2d(0.0,1.0); glVertex3f( width, 0, -length); // Bottom Right Of The Quad (Right)
  glEnd();

  //gravel
  glBindTexture(GL_TEXTURE_2D,texture[10]);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-18.5,0.05,-20);
  glTexCoord2f(7,0); glVertex3f(-18.5,0.05,20);
  glTexCoord2f(7,7); glVertex3f(18.5,0.05,20);
  glTexCoord2f(0,7); glVertex3f(18.5,0.05,-20);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

static void puttingGreen(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  
  // putting green
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-1.5,0.1,-4.5);
  glTexCoord2f(1,0); glVertex3f(-1.5,0.1,4.5);
  glTexCoord2f(1,1); glVertex3f(1.5,0.1,4.5);
  glTexCoord2f(0,1); glVertex3f(1.5,0.1,-4.5);
  glEnd();

  //metal plate
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-.5,0.11,3.9);
  glTexCoord2f(1,0); glVertex3f(-.5,0.11,4.5);
  glTexCoord2f(1,1); glVertex3f(.5,0.11,4.5);
  glTexCoord2f(0,1); glVertex3f(.5,0.11,3.9);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

static void wall(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  
  // left
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-1.5,0.1,4.5);
  glTexCoord2f(1,0); glVertex3f(-1.3,0.1,4.5);
  glTexCoord2f(1,1); glVertex3f(-1.3,0.3,4.5);
  glTexCoord2f(0,1); glVertex3f(-1.5,0.3,4.5);
  //back
  glNormal3f(0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-1.3,0.1,-4.5);
  glTexCoord2f(1,0); glVertex3f(-1.5,0.1,-4.5);
  glTexCoord2f(1,1); glVertex3f(-1.5,0.3,-4.5);
  glTexCoord2f(0,1); glVertex3f(-1.3,0.3,-4.5);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-1.5,0.1,-4.5);
  glTexCoord2f(5,0); glVertex3f(-1.5,0.1,4.5);
  glTexCoord2f(5,1); glVertex3f(-1.5,0.3,4.5);
  glTexCoord2f(0,1); glVertex3f(-1.5,0.3,-4.5);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-1.3,0.1,4.5);
  glTexCoord2f(5,0); glVertex3f(-1.3,0.1,-4.5);
  glTexCoord2f(5,1); glVertex3f(-1.3,0.3,-4.5);
  glTexCoord2f(0,1); glVertex3f(-1.3,0.3,4.5);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-1.5,0.3,4.5);
  glTexCoord2f(5,0); glVertex3f(-1.3,0.3,4.5);
  glTexCoord2f(5,1); glVertex3f(-1.3,0.3,-4.5);
  glTexCoord2f(0,1); glVertex3f(-1.5,0.3,-4.5);
  glEnd();

  // middle
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-1.3,0.1,-4.3);
  glTexCoord2f(3,0); glVertex3f(1.3,0.1,-4.3);
  glTexCoord2f(3,3); glVertex3f(1.3,0.3,-4.3);
  glTexCoord2f(0,3); glVertex3f(-1.3,0.3,-4.3);
  //back
  glNormal3f(0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(1.3,0.1,-4.5);
  glTexCoord2f(3,0); glVertex3f(-1.3,0.1,-4.5);
  glTexCoord2f(3,3); glVertex3f(-1.3,0.3,-4.5);
  glTexCoord2f(0,3); glVertex3f(1.3,0.3,-4.5);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-1.3,0.3,-4.3);
  glTexCoord2f(3,0); glVertex3f(1.3,0.3,-4.3);
  glTexCoord2f(3,3); glVertex3f(1.3,0.3,-4.5);
  glTexCoord2f(0,3); glVertex3f(-1.3,0.3,-4.5);
  glEnd();

  // right
  // glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(1.3,0.1,4.5);
  glTexCoord2f(1,0); glVertex3f(1.5,0.1,4.5);
  glTexCoord2f(1,1); glVertex3f(1.5,0.3,4.5);
  glTexCoord2f(0,1); glVertex3f(1.3,0.3,4.5);
  //back
  glNormal3f(0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(1.5,0.1,-4.5);
  glTexCoord2f(1,0); glVertex3f(1.3,0.1,-4.5);
  glTexCoord2f(1,1); glVertex3f(1.3,0.3,-4.5);
  glTexCoord2f(0,1); glVertex3f(1.5,0.3,-4.5);
  //left
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(1.3,0.1,-4.5);
  glTexCoord2f(5,0); glVertex3f(1.3,0.1,4.5);
  glTexCoord2f(5,1); glVertex3f(1.3,0.3,4.5);
  glTexCoord2f(0,1); glVertex3f(1.3,0.3,-4.5);
  //right
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(1.5,0.1,4.5);
  glTexCoord2f(5,0); glVertex3f(1.5,0.1,-4.5);
  glTexCoord2f(5,1); glVertex3f(1.5,0.3,-4.5);
  glTexCoord2f(0,1); glVertex3f(1.5,0.3,4.5);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(1.3,0.3,4.5);
  glTexCoord2f(5,0); glVertex3f(1.5,0.3,4.5);
  glTexCoord2f(5,1); glVertex3f(1.5,0.3,-4.5);
  glTexCoord2f(0,1); glVertex3f(1.3,0.3,-4.5);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

//ex8
static void golfBall(double x,double y,double z,double r)
{
   // const int d=5;
   // int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   double az;
   double el;
   double difference = 3.0;
   double bigdifference = difference * 4.0;

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[1]);

   for(el = -90.0; el < 90; el+=difference)
    {
      glBegin(GL_QUAD_STRIP);
      for(az = 0.0; az <= 360.0; az+=bigdifference)
      {
        glTexCoord2f(az/360,(el+90.0)/180);
        glNormal3d(Sin(az)*Cos(el),Sin(el),Cos(az)*Cos(el));
        glVertex3d(Sin(az)*Cos(el),Sin(el),Cos(az)*Cos(el));
        glTexCoord2f((az+bigdifference)/360,(el+90.0)/180);
        glNormal3d(Sin(az)*Cos(el+difference),Sin(el+difference),Cos(az)*Cos(el+difference));
        glVertex3d(Sin(az)*Cos(el+difference),Sin(el+difference),Cos(az)*Cos(el+difference));
      }
      glEnd();
    }

    glDisable(GL_TEXTURE_2D);

   //  Undo transformations
   glPopMatrix();
}

void flagPole(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   double i;
   double radius = 0.3;
   double height = 0.5;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(1,1,1);

   int j;
   glBegin(GL_TRIANGLE_STRIP);
   for (j = 0; j <= 360; j++) {
      const float tc = (j / (float) 360);

      double x = radius * Cos(j);
      double y = height;
      double z = radius * Sin(j);

      glNormal3f(Cos(j), 1, Sin(j));

      glTexCoord2f(-tc, 0.0); glVertex3d(x, y, z);
      glTexCoord2f(-tc, 1.0); glVertex3d(x, -y, z);
   }
   glEnd();

   //glBindTexture(GL_TEXTURE_2D,texture[12]);

   glNormal3f(0,1,0);

   //top of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glTexCoord2f(0.5,0.5);  glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   //glBindTexture(GL_TEXTURE_2D,texture[12]);

   glNormal3f(0,-1,0);

   //bottom of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);  glVertex3d(0.0, -height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   //glDisable(GL_TEXTURE_2D); 

   glPopMatrix();
}

static void flag(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  
  // flag
  glBindTexture(GL_TEXTURE_2D,texture[4]);

  glBegin(GL_TRIANGLES);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0.0, 0.0); glVertex3f(0, 1.3, -3.5);
  glTexCoord2f(2.0, 0.0); glVertex3f(.5, 1.05, -3.5);
  glTexCoord2f(1.0, 2.0); glVertex3f(0, .8, -3.5);
  // back
  glNormal3f(0,0,-1);
  glTexCoord2f(0.0, 0.0); glVertex3f(0, 1.3, -3.51);
  glTexCoord2f(2.0, 0.0); glVertex3f(.5, 1.05, -3.51);
  glTexCoord2f(1.0, 2.0); glVertex3f(0, .8, -3.51);
  glEnd();
  

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

void hole(double x, double y, float radius, 
              double dx, double dy,double dz,
              double th)
{
  glTranslated(dx,dy,dz);
  glRotated(th,1,0,0);

  int i;
  int triangleAmount = 20; //# of triangles used to draw circle

  //radius
  float twicePi = 2.0f * M_PI;
  glColor3f(0,0,0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y); // center of circle
  for(i = 0; i <= triangleAmount;i++) 
  { 
    glVertex2f(
      x + (radius * cos(i *  twicePi / triangleAmount)), 
      y + (radius * sin(i * twicePi / triangleAmount)));      
  }
  glEnd();

}

void windmill(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  glPushMatrix();

  glTranslated(x,y,z);
  glRotated(th,1,0,0);
  glScaled(dx,dy,dz);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  //body of windmill
  //glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
   //  house
   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1.25,-1.25, 1);
   glTexCoord2f(1.0, 0.0); glVertex3f(+1.25,-1.25, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(+.75,+.775, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.75,+.775, 1);
   //  Back
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1.25,-1.25,-.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-1.25,-1.25,-.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.75,+.775,-.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(+.75,+.775,-.5);
   //  Right
   glNormal3f( 1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1.25,-1.25,+1);
   glTexCoord2f(1.0, 0.0); glVertex3f(+1.25,-1.25,-.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(+.75,+.775,-.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(+.75,+.775,+1);
   //  Left
   glNormal3f( -1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1.25,-1.25,-.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-1.25,-1.25,+1);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.75,+.775,+1);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.75,+.775,-.5);
   //  Top
   glVertex3f(-.75,+.75,+1);
   glVertex3f(+.75,+.75,+1);
   glVertex3f(+.75,+.75,-.5);
   glVertex3f(-.75,+.75,-.5);
   //  Bottom
   glVertex3f(-1.25,-1.25,-.5);
   glVertex3f(+1.25,-1.25,-.5);
   glVertex3f(+1.25,-1.25,+1);
   glVertex3f(-1.25,-1.25,+1);
   //end
   glEnd();
   //legs left

   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1.25,-1.25, 1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.55,-1.25, 1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.55,-1.85, 1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-1.4,-1.85, 1);
   //  Back
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1.25,-1.25,-.5);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.55,-1.25,-.5);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.55,-1.85,-.5);
   glTexCoord2f(0.0, 0.5); glVertex3f(-1.4,-1.85,-.5);
   //  Right
   glNormal3f( 1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.55,-1.25,+1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.55,-1.25,-.5);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.55,-1.85,-.5);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.55,-1.85,+1);
   // Left
   glNormal3f( -1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1.25,-1.25,-.5);
   glTexCoord2f(0.5, 0.0); glVertex3f(-1.25,-1.25,+1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-1.4,-1.85,+1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-1.4,-1.85,-.5);
   // //  Bottom
   glVertex3f(-1.4,-1.85,-.5);
   glVertex3f(-.55,-1.85,-.5);
   glVertex3f(-.55,-1.85,+1);
   glVertex3f(-1.4,-1.85,+1);
   glEnd();

   //legs right
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(1.25,-1.25, 1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.55,-1.25, 1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.55,-1.85, 1);
   glTexCoord2f(0.0, 0.5); glVertex3f(1.4,-1.85, 1);
   //  Back
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(1.25,-1.25,-.5);
   glTexCoord2f(0.5, 0.0); glVertex3f(.55,-1.25,-.5);
   glTexCoord2f(0.5, 0.5); glVertex3f(.55,-1.85,-.5);
   glTexCoord2f(0.0, 0.5); glVertex3f(1.4,-1.85,-.5);
   //  Right
   glNormal3f( -1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(.55,-1.25,+1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.55,-1.25,-.5);
   glTexCoord2f(0.5, 0.5); glVertex3f(.55,-1.85,-.5);
   glTexCoord2f(0.0, 0.5); glVertex3f(.55,-1.85,+1);
   // Left
   glNormal3f( 1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(1.25,-1.25,-.5);
   glTexCoord2f(0.5, 0.0); glVertex3f(1.25,-1.25,+1);
   glTexCoord2f(0.5, 0.5); glVertex3f(1.4,-1.85,+1);
   glTexCoord2f(0.0, 0.5); glVertex3f(1.4,-1.85,-.5);
   // //  Bottom
   glVertex3f(1.4,-1.85,-.5);
   glVertex3f(.55,-1.85,-.5);
   glVertex3f(.55,-1.85,+1);
   glVertex3f(1.4,-1.85,+1);
   glEnd();

   const double rooftop = 1.5;
   const double roofx = .85;
   const double roofxback = -.85;
   const double roofz = 1.1;
   const double roofbottom = .75;
   const double roofzpoint = .25;
   const double roofzback = -.6;

  glBindTexture(GL_TEXTURE_2D,texture[8]);
   // roof
  glBegin(GL_TRIANGLES);
  // front
  glNormal3f(0,0,1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofx,+roofbottom,+roofz);
  glTexCoord2f(1.0, 0.0); glVertex3f(roofxback,+roofbottom,+roofz);
  glTexCoord2f(1.0, 1.0); glVertex3f(0,+rooftop,roofzpoint);
  // back
  glNormal3f(0,0,-1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofx,+roofbottom,roofzback);
  glTexCoord2f(1.0, 0.0); glVertex3f(roofxback,+roofbottom,roofzback);
  glTexCoord2f(1.0, 1.0); glVertex3f(0,+rooftop,roofzpoint);
  // right
  glNormal3f(1,1,0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofx,+roofbottom,roofzback);
  glTexCoord2f(1.0, 0.0); glVertex3f(+roofx,+roofbottom,+roofz);
  glTexCoord2f(1.0, 1.0); glVertex3f(0,+rooftop,+roofzpoint);
  // left
  glNormal3f(-1,-1,0);
  glTexCoord2f(0.0, 0.0); glVertex3f(roofxback,+roofbottom,roofzback);
  glTexCoord2f(1.0, 0.0); glVertex3f(roofxback,+roofbottom,+roofz);
  glTexCoord2f(1.0, 1.0); glVertex3f(0,+rooftop,+roofzpoint); 
  glEnd();
  // bottom
  glBegin(GL_QUADS);
  glNormal3f(0,1,0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofx,+roofbottom,roofzback);
  glTexCoord2f(1.0, 0.0); glVertex3f(+roofx,+roofbottom,+roofz);
  glTexCoord2f(1.0, 1.0); glVertex3f(roofxback,+roofbottom,+roofz);
  glTexCoord2f(0.0, 1.0); glVertex3f(roofxback,+roofbottom,roofzback);
  glEnd();


  //wings on windmill

  //glColor3f(1,1,1);

  glBindTexture(GL_TEXTURE_2D,texture[6]);

  int i;

  //glTranslatef(4, -2, 2);
  glRotated(frameNumber * (180.0/46), 0, 0, 1);

  //glColor3f(0.4, 0.4, 0.8);
  for (i = 0; i < 4; i++) {
    glRotated(90, 0, 0, 1);  // Note: These rotations accumulate.

    //back
    glBegin(GL_POLYGON);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(0, 0.01, 1);
    glTexCoord2f(1,0); glVertex3f(1.75, 0.3, 1);
    glTexCoord2f(1,1); glVertex3f(1.75, -0.3, 1);
    glTexCoord2f(0,1); glVertex3f(0, -0.01, 1);
    //front
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(0, 0.01, 1.25);
    glTexCoord2f(1,0); glVertex3f(1.75, 0.3, 1.25);
    glTexCoord2f(1,1); glVertex3f(1.75, -0.3, 1.25);
    glTexCoord2f(0,1); glVertex3f(0, -0.01, 1.25);
    //top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(0, 0.01, 1.25);
    glTexCoord2f(1,0); glVertex3f(1.75, 0.3, 1.25);
    glTexCoord2f(1,1); glVertex3f(1.75, 0.3, 1);
    glTexCoord2f(0,1); glVertex3f(0, 0.01, 1);
    //bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(0, -0.01, 1.25);
    glTexCoord2f(1,0); glVertex3f(1.75, -0.3, 1.25);
    glTexCoord2f(1,1); glVertex3f(1.75, -0.3, 1);
    glTexCoord2f(0,1); glVertex3f(0, -0.01, 1);
    //right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(1.75, 0.3, 1);
    glTexCoord2f(1,0); glVertex3f(1.75, 0.3, 1.25);
    glTexCoord2f(1,1); glVertex3f(1.75, -0.3, 1.25);
    glTexCoord2f(0,1); glVertex3f(1.75, -0.3, 1);
    glEnd();
  }

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

void courseTwoHex(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,texture[0]);

  int i;

  for (i = 0; i < 6; i++) {
    glRotated(60, 0, 1, 0);
  
    glBegin(GL_TRIANGLES);
    glNormal3f(0,1,0);
    glTexCoord2f(0.0, 0.0); glVertex3f(.875,0.05,-1.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-.875,0.05,-1.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0,0.15,0);
    glEnd();
  }

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

void courseHexWall(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.875,0.1,-1.5);
  glTexCoord2f(3,0); glVertex3f(.875,0.1,-1.5);
  glTexCoord2f(3,3); glVertex3f(.875,0.3,-1.5);
  glTexCoord2f(0,3); glVertex3f(-.875,0.3,-1.5);
  //back
  glNormal3f(0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(.875,0.1,-1.7);
  glTexCoord2f(3,0); glVertex3f(-.875,0.1,-1.7);
  glTexCoord2f(3,3); glVertex3f(-.875,0.3,-1.7);
  glTexCoord2f(0,3); glVertex3f(.875,0.3,-1.7);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-.875,0.3,-1.5);
  glTexCoord2f(3,0); glVertex3f(.875,0.3,-1.5);
  glTexCoord2f(3,3); glVertex3f(.975,0.3,-1.7);
  glTexCoord2f(0,3); glVertex3f(-.975,0.3,-1.7);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-.875,0.3,-1.7);
  glTexCoord2f(1,0); glVertex3f(-.9795,0.3,-1.7);
  glTexCoord2f(1,1); glVertex3f(-.9795,0.1,-1.7);
  glTexCoord2f(0,1); glVertex3f(-.875,0.1,-1.7);

  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(.9795,0.3,-1.7);
  glTexCoord2f(1,0); glVertex3f(.875,0.3,-1.7);
  glTexCoord2f(1,1); glVertex3f(.875,0.1,-1.7);
  glTexCoord2f(0,1); glVertex3f(.9795,0.1,-1.7);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

void courseTwo(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-1.1,0.11,3.9);
  glTexCoord2f(1,0); glVertex3f(-1.1,0.11,4.5);
  glTexCoord2f(1,1); glVertex3f(.5,0.11,4.5);
  glTexCoord2f(0,1); glVertex3f(.5,0.11,3.9);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[0]);
  //beginning
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-2.1,0.1,-4.5);
  glTexCoord2f(1,0); glVertex3f(-2.1,0.1,4.5);
  glTexCoord2f(1,1); glVertex3f(1.5,0.1,4.5);
  glTexCoord2f(0,1); glVertex3f(1.5,0.1,-4.5);
  glEnd();
  //bend
  glBegin(GL_QUADS);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(7,1,-10);
  glTexCoord2f(1,0); glVertex3f(-2.1,1,-4.5);
  glTexCoord2f(1,1); glVertex3f(1.5,0.115,-2.5);
  glTexCoord2f(0,1); glVertex3f(7,0.115,-6);
  glEnd();
  //tringle smoother
  glBegin(GL_TRIANGLES);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.1,0.115,-1.5);
  glTexCoord2f(1,0); glVertex3f(-2.1,1,-4.5);
  glTexCoord2f(1,1); glVertex3f(1.5,0.115,-2.5);
  glEnd();
  //end
  glBegin(GL_QUADS);
  glNormal3f(0,1,0);
  glTexCoord2f(1,1); glVertex3f(15.25,0.115,-10);
  glTexCoord2f(0,0); glVertex3f(15.25,0.115,-2.25);
  glTexCoord2f(0,1); glVertex3f(10.95,0.115,-2.25);
  glTexCoord2f(1,0); glVertex3f(10.95,0.115,-6);
  glEnd();
  //middle
  glBegin(GL_QUADS);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(7,1,-10);
  glTexCoord2f(1,0); glVertex3f(15.25,0.115,-10);
  glTexCoord2f(1,1); glVertex3f(10.95,0.115,-6);
  glTexCoord2f(0,1); glVertex3f(7,0.115,-6);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

static void courseTwoWall(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  
  // first left
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-2.25,0.1,4.5);
  glTexCoord2f(1,0); glVertex3f(-1.9,0.1,4.5);
  glTexCoord2f(1,1); glVertex3f(-1.9,0.5,4.5);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.5,4.5);
  //back
  glNormal3f(0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-1.9,0.1,-1.75);
  glTexCoord2f(1,0); glVertex3f(-2.25,0.1,-1.75);
  glTexCoord2f(1,1); glVertex3f(-2.25,0.5,-1.75);
  glTexCoord2f(0,1); glVertex3f(-1.9,0.5,-1.75);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,0.1,-1.75);
  glTexCoord2f(5,0); glVertex3f(-2.25,0.1,4.5);
  glTexCoord2f(5,1); glVertex3f(-2.25,0.5,4.5);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.5,-1.75);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-1.9,0.1,4.5);
  glTexCoord2f(5,0); glVertex3f(-1.9,0.1,-1.75);
  glTexCoord2f(5,1); glVertex3f(-1.9,0.5,-1.75);
  glTexCoord2f(0,1); glVertex3f(-1.9,0.5,4.5);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,0.5,4.5);
  glTexCoord2f(5,0); glVertex3f(-1.9,0.5,4.5);
  glTexCoord2f(5,1); glVertex3f(-1.9,0.5,-1.75);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.5,-1.75);
  glEnd();

  // first right
  glBegin(GL_QUADS);
  //front
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(1.3,0.1,4.5);
  glTexCoord2f(1,0); glVertex3f(1.65,0.1,4.5);
  glTexCoord2f(1,1); glVertex3f(1.65,0.5,4.5);
  glTexCoord2f(0,1); glVertex3f(1.3,0.5,4.5);
  //back
  glNormal3f(0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(1.65,0.1,-2.35);
  glTexCoord2f(1,0); glVertex3f(1.3,0.1,-2.35);
  glTexCoord2f(1,1); glVertex3f(1.3,0.5,-2.35);
  glTexCoord2f(0,1); glVertex3f(1.65,0.5,-2.35);
  //left
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(1.3,0.1,-2.35);
  glTexCoord2f(5,0); glVertex3f(1.3,0.1,4.5);
  glTexCoord2f(5,1); glVertex3f(1.3,0.5,4.5);
  glTexCoord2f(0,1); glVertex3f(1.3,0.5,-2.35);
  //right
  glNormal3f(0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(1.65,0.1,4.5);
  glTexCoord2f(5,0); glVertex3f(1.65,0.1,-2.35);
  glTexCoord2f(5,1); glVertex3f(1.65,0.5,-2.35);
  glTexCoord2f(0,1); glVertex3f(1.65,0.5,4.5);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(1.3,0.5,4.5);
  glTexCoord2f(5,0); glVertex3f(1.65,0.5,4.5);
  glTexCoord2f(5,1); glVertex3f(1.65,0.5,-2.35);
  glTexCoord2f(0,1); glVertex3f(1.3,0.5,-2.35);
  glEnd();

  //front bend wall
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(1.3,0.1,-2.35);
  glTexCoord2f(5,0); glVertex3f(7,0.1,-6);
  glTexCoord2f(5,1); glVertex3f(7,0.5,-6);
  glTexCoord2f(0,1); glVertex3f(1.3,0.5,-2.35);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(7.55,0.1,-6);
  glTexCoord2f(5,0); glVertex3f(1.65,0.1,-2.15);
  glTexCoord2f(5,1); glVertex3f(1.65,0.5,-2.15);
  glTexCoord2f(0,1); glVertex3f(7.55,0.5,-6);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(7,0.5,-6);
  glTexCoord2f(5,0); glVertex3f(7.55,0.5,-6);
  glTexCoord2f(5,1); glVertex3f(1.65,0.5,-2.15);
  glTexCoord2f(0,1); glVertex3f(1.3,0.5,-2.35);
  glEnd();

  //front middle wall
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(10.95,0.1,-6);
  glTexCoord2f(5,0); glVertex3f(7,0.1,-6);
  glTexCoord2f(5,1); glVertex3f(7,0.5,-6);
  glTexCoord2f(0,1); glVertex3f(10.95,0.5,-6);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(7,0.1,-5.7);
  glTexCoord2f(5,0); glVertex3f(10.95,0.1,-5.7);
  glTexCoord2f(5,1); glVertex3f(10.95,0.5,-5.7);
  glTexCoord2f(0,1); glVertex3f(7,0.5,-5.7);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(7,0.5,-6);
  glTexCoord2f(5,0); glVertex3f(7,0.5,-5.7);
  glTexCoord2f(5,1); glVertex3f(10.95,0.5,-5.7);
  glTexCoord2f(0,1); glVertex3f(10.95,0.5,-6);
  glEnd();

  //left end wall
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(10.65,0.1,-6);
  glTexCoord2f(5,0); glVertex3f(10.65,0.1,-2.375);
  glTexCoord2f(5,1); glVertex3f(10.65,0.5,-2.375);
  glTexCoord2f(0,1); glVertex3f(10.65,0.5,-6);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(10.95,0.1,-6);
  glTexCoord2f(5,0); glVertex3f(11.05,0.1,-2.375);
  glTexCoord2f(5,1); glVertex3f(11.05,0.5,-2.375);
  glTexCoord2f(0,1); glVertex3f(10.95,0.5,-6);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(10.65,0.5,-6);
  glTexCoord2f(5,0); glVertex3f(10.65,0.5,-2.375);
  glTexCoord2f(5,1); glVertex3f(11.05,0.5,-2.375);
  glTexCoord2f(0,1); glVertex3f(10.95,0.5,-6);
  glEnd();

  //right end wall
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(14.95,0.1,-10);
  glTexCoord2f(5,0); glVertex3f(14.95,0.1,-2.3775);
  glTexCoord2f(5,1); glVertex3f(14.95,0.5,-2.3775);
  glTexCoord2f(0,1); glVertex3f(14.95,0.5,-10);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(15.35,0.1,-10);
  glTexCoord2f(5,0); glVertex3f(15.35,0.1,-2.3775);
  glTexCoord2f(5,1); glVertex3f(15.35,0.5,-2.3775);
  glTexCoord2f(0,1); glVertex3f(15.35,0.5,-10);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(14.95,0.5,-10);
  glTexCoord2f(5,0); glVertex3f(14.95,0.5,-2.3775);
  glTexCoord2f(5,1); glVertex3f(15.35,0.5,-2.3775);
  glTexCoord2f(0,1); glVertex3f(15.35,0.5,-10);
  glEnd();

  //back middle wall
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(15.35,0.1,-10);
  glTexCoord2f(5,0); glVertex3f(7,1,-10);
  glTexCoord2f(5,1); glVertex3f(7,1.4,-10);
  glTexCoord2f(0,1); glVertex3f(15.35,0.5,-10);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(7,1,-9.7);
  glTexCoord2f(5,0); glVertex3f(15.35,0.1,-9.7);
  glTexCoord2f(5,1); glVertex3f(15.35,0.5,-9.7);
  glTexCoord2f(0,1); glVertex3f(7,1.4,-9.7);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(7,1.4,-10);
  glTexCoord2f(5,0); glVertex3f(7,1.4,-9.7);
  glTexCoord2f(5,1); glVertex3f(15.35,0.5,-9.7);
  glTexCoord2f(0,1); glVertex3f(15.35,0.5,-10);
  glEnd();

  //back bend wall
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,1,-4.5);
  glTexCoord2f(5,0); glVertex3f(7,1,-10);
  glTexCoord2f(5,1); glVertex3f(7,1.4,-10);
  glTexCoord2f(0,1); glVertex3f(-2.25,1.4,-4.5);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(7.55,1,-10);
  glTexCoord2f(5,0); glVertex3f(-1.9,1,-4.25);
  glTexCoord2f(5,1); glVertex3f(-1.9,1.4,-4.25);
  glTexCoord2f(0,1); glVertex3f(7.55,1.4,-10);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(7,1.4,-10);
  glTexCoord2f(5,0); glVertex3f(7.55,1.4,-10);
  glTexCoord2f(5,1); glVertex3f(-1.9,1.4,-4.25);
  glTexCoord2f(0,1); glVertex3f(-2.25,1.4,-4.5);
  glEnd();

  // triangle smoother
  glBegin(GL_QUADS);
  //left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,0.1,-1.75);
  glTexCoord2f(5,0); glVertex3f(-2.25,1,-4.5);
  glTexCoord2f(5,1); glVertex3f(-2.25,1.4,-4.5);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.5,-1.75);
  //right
  glNormal3f(1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-1.9,1,-4.25);
  glTexCoord2f(5,0); glVertex3f(-1.9,0.1,-1.75);
  glTexCoord2f(5,1); glVertex3f(-1.9,0.5,-1.75);
  glTexCoord2f(0,1); glVertex3f(-1.9,1.4,-4.5);
  //top
  glNormal3f(0, 1, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,1.4,-4.5);
  glTexCoord2f(5,0); glVertex3f(-1.9,1.4,-4.5);
  glTexCoord2f(5,1); glVertex3f(-1.9,0.5,-1.75);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.5,-1.75);
  glEnd();


  //back wallls
  //trinagle smoother
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,0.1,-1.75);
  glTexCoord2f(5,0); glVertex3f(-2.25,1,-4.5);
  glTexCoord2f(5,1); glVertex3f(-2.25,0.1,-4.5);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.1,-1.75);
  glEnd();
  //bend
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-2.25,1,-4.5);
  glTexCoord2f(5,0); glVertex3f(7,1,-10);
  glTexCoord2f(5,1); glVertex3f(7,0.1,-10);
  glTexCoord2f(0,1); glVertex3f(-2.25,0.1,-4.5);
  glEnd();
  //middle
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(15.35,0.1,-10);
  glTexCoord2f(5,0); glVertex3f(7,0.1,-10);
  glTexCoord2f(5,1); glVertex3f(7,1,-10);
  glTexCoord2f(0,1); glVertex3f(15.35,0.11,-10);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

static void fence(double x, double y, double z,
          double dx, double dy, double dz,
          double th){
  // Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  glPushMatrix();
  
  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);

  // adapted from drawing a house
  
  // bottom of fence
  glBegin(GL_QUADS);
  // Front
  glColor3ub(200,200,200);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1, 0, .5);
  glVertex3f(+1, 0, .5);
  glVertex3f(+1,+.6, .5);
  glVertex3f(-1,+.6, .5);
  // Back
  glNormal3f( 0, 0,-1);
  glVertex3f(+1, 0,-.5);
  glVertex3f(-1, 0,-.5);
  glVertex3f(-1,+.6,-.5);
  glVertex3f(+1,+.6,-.5);
  // Right
  glNormal3f(+1, 0, 0);
  glVertex3f(+1, 0,+.5);
  glVertex3f(+1, 0,-.5);
  glVertex3f(+1,+.6,-.5);
  glVertex3f(+1,+.6,+.5);
  // Left
  glNormal3f(-1, 0, 0);
  glVertex3f(-1, 0,-.5);
  glVertex3f(-1, 0,+.5);
  glVertex3f(-1,+.6,+.5);
  glVertex3f(-1,+.6,-.5);
  glEnd(); 

  // top of fence
  glBegin(GL_QUADS);
  // Right
  glNormal3f(+1, 0, 0);
  glVertex3f(+1,+.6,+.5);
  glVertex3f(+1,+.6,-.5);
  glVertex3f(0 ,+.7,-.5);
  glVertex3f(0 ,+.7,+.5);
  // Left
  glNormal3f(-1, 0, 0);
  glVertex3f(-1,+.6,-.5);
  glVertex3f(-1,+.6,+.5);
  glVertex3f(0 ,+.7,+.5);
  glVertex3f(0 ,+.7,-.5);
  glEnd();

  glBegin(GL_TRIANGLES);
  // Front
  glNormal3f(0, 0, 1);
  glVertex3f(+1,+.6,+.5);
  glVertex3f(-1,+.6,+.5);
  glVertex3f(0 ,+.7,+.5);
  // Back
  glNormal3f(0, 0,-1);
  glVertex3f(+1,+.6,-.5);
  glVertex3f(-1,+.6,-.5);
  glVertex3f(0, +.7,-.5);
  glEnd();

  glPopMatrix();
}

void treeTrunk(double x,double y,double z,
                 double dx,double dy,double dz, double xh, double yh,
                 double zh) {

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   double i;
   double radius = 0.3;
   double height = 0.5;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(xh,1,0,0);
   glRotated(yh,0,1,0);
   glRotated(zh,0,0,1);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[11]);

   int j;
   glBegin(GL_TRIANGLE_STRIP);
   for (j = 0; j <= 360; j++) {
      const float tc = (j / (float) 360);

      double x = radius * Cos(j);
      double y = height;
      double z = radius * Sin(j);

      glNormal3f(Cos(j), 1, Sin(j));

      glTexCoord2f(-tc, 0.0); glVertex3d(x, y, z);
      glTexCoord2f(-tc, 1.0); glVertex3d(x, -y, z);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[11]);

   glNormal3f(0,1,0);

   //top of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glTexCoord2f(0.5,0.5);  glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[11]);

   glNormal3f(0,-1,0);

   //bottom of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);  glVertex3d(0.0, -height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   glDisable(GL_TEXTURE_2D); 

   glPopMatrix();
}

static void leaves(double x,double y,double z,
                 double dx,double dy,double dz,
                 double xh, double yh, double zh) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);
  glScaled(dx,dy,dz);

  //glColor3f(1,1,1);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  //glColor3ub(32, 102, 23);
  glBindTexture(GL_TEXTURE_2D,texture[14]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-1,1,1);
  glTexCoord2f(1,0); glVertex3f(1,1,1);
  glTexCoord2f(1,1); glVertex3f(1,-1,1);
  glTexCoord2f(0,1); glVertex3f(-1,-1,1);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-1,1,-1);
  glTexCoord2f(1,0); glVertex3f(1,1,-1);
  glTexCoord2f(1,1); glVertex3f(1,-1,-1);
  glTexCoord2f(0,1); glVertex3f(-1,-1,-1);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-1,1,-1);
  glTexCoord2f(1,0); glVertex3f(-1,1,1);
  glTexCoord2f(1,1); glVertex3f(-1,-1,1);
  glTexCoord2f(0,1); glVertex3f(-1,-1,-1);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,1,-1);
  glTexCoord2f(1,0); glVertex3f(1,1,1);
  glTexCoord2f(1,1); glVertex3f(1,-1,1);
  glTexCoord2f(0,1); glVertex3f(1,-1,-1);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-1,1,1);
  glTexCoord2f(1,0); glVertex3f(1,1,1);
  glTexCoord2f(1,1); glVertex3f(1,1,-1);
  glTexCoord2f(0,1); glVertex3f(-1,1,-1);
  //bottom
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0); glVertex3f(-1,-1,1);
  glTexCoord2f(1,0); glVertex3f(1,-1,1);
  glTexCoord2f(1,1); glVertex3f(1,-1,-1);
  glTexCoord2f(0,1); glVertex3f(-1,-1,-1);

  glEnd();

  glDisable(GL_TEXTURE_2D); 

  glPopMatrix();
}

static void bench(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  glColor3f(1,1,1);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  //seat
  // top
  glBindTexture(GL_TEXTURE_2D,texture[15]);
  glBegin(GL_QUADS);
  //  Front
  //glColor3f(0,1,1);
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.5,-1.15, 1);
  glTexCoord2f(2,0); glVertex3f(.5,-1.15, 1);
  glTexCoord2f(2,2); glVertex3f(.5,-1, 1);
  glTexCoord2f(0,2); glVertex3f(-.5,-1, 1);
  //  Back
  //glColor3f(0,1,1);
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-.5,-1.15,.5);
  glTexCoord2f(2,0); glVertex3f(.5,-1.15,.5);
  glTexCoord2f(2,2); glVertex3f(.5,-1,.5);
  glTexCoord2f(0,2); glVertex3f(-.5,-1,.5);
  //  Right
  //glColor3f(0,1,1);
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.5,-1.15,+1);
  glTexCoord2f(2,0); glVertex3f(+.5,-1.15,.5);
  glTexCoord2f(2,2); glVertex3f(+.5,-1,.5);
  glTexCoord2f(0,2); glVertex3f(+.5,-1,+1);
  //  Left
  //glColor3f(0,1,1);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-1.15,.5);
  glTexCoord2f(2,0); glVertex3f(-.5,-1.15,+1);
  glTexCoord2f(2,2); glVertex3f(-.5,-1,+1);
  glTexCoord2f(0,2); glVertex3f(-.5,-1,.5);
  //  Top
  //glColor3f(0,1,1);
  glNormal3f( 0, 1,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-1,+1);
  glTexCoord2f(2,0); glVertex3f(+.5,-1,+1);
  glTexCoord2f(2,2); glVertex3f(+.5,-1,.5);
  glTexCoord2f(0,2); glVertex3f(-.5,-1,.5);
  //  Bottom
  //glColor3f(0,1,1);
  glNormal3f( 0, -1,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-1.15,.5);
  glTexCoord2f(2,0); glVertex3f(+.5,-1.15,.5);
  glTexCoord2f(2,2); glVertex3f(+.5,-1.15,+1);
  glTexCoord2f(0,2); glVertex3f(-.5,-1.15,+1);
  //end
  glEnd();

  //legs
  glBegin(GL_QUADS);
  // right front
  //left
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.4,-1.4,1);
  glTexCoord2f(1,0); glVertex3f(+.4,-1.4,.9);
  glTexCoord2f(1,1); glVertex3f(+.4,-1.15,.9);
  glTexCoord2f(0,1); glVertex3f(+.4,-1.15,1);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.5,-1.4,+1);
  glTexCoord2f(1,0); glVertex3f(+.5,-1.4,.9);
  glTexCoord2f(1,1); glVertex3f(+.5,-1.15,.9);
  glTexCoord2f(0,1); glVertex3f(+.5,-1.15,+1);
  // back
  glNormal3f( 0, 0,   -1);
  glTexCoord2f(0,0); glVertex3f(+.4,-1.4,.9);
  glTexCoord2f(1,0); glVertex3f(+.4,-1.15,.9);
  glTexCoord2f(1,1); glVertex3f(+.5,-1.15,.9);
  glTexCoord2f(0,1); glVertex3f(+.5,-1.4,.9);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(+.4,-1.4,+1);
  glTexCoord2f(1,0); glVertex3f(+.4,-1.15,+1);
  glTexCoord2f(1,1); glVertex3f(+.5,-1.15,+1);
  glTexCoord2f(0,1); glVertex3f(+.5,-1.4,+1);
  // top
  glVertex3f(+.4,-1.15,+1);
  glVertex3f(+.4,-1.15,.9);
  glVertex3f(+.5,-1.15,.9);
  glVertex3f(+.5,-1.15,1);
  glEnd();
  // right back
  //left
  glBegin(GL_QUADS);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.4,-1.4,.5);
  glTexCoord2f(1,0); glVertex3f(+.4,-1.4,.6);
  glTexCoord2f(1,1); glVertex3f(+.4,-1.15,.6);
  glTexCoord2f(0,1); glVertex3f(+.4,-1.15,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.5,-1.4,.5);
  glTexCoord2f(1,0); glVertex3f(+.5,-1.4,.6);
  glTexCoord2f(1,1); glVertex3f(+.5,-1.15,.6);
  glTexCoord2f(0,1); glVertex3f(+.5,-1.15,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(+.4,-1.4,.6);
  glTexCoord2f(1,0); glVertex3f(+.4,-1.15,.6);
  glTexCoord2f(1,1); glVertex3f(+.5,-1.15,.6);
  glTexCoord2f(0,1); glVertex3f(+.5,-1.4,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(+.4,-1.4,+.5);
  glTexCoord2f(1,0); glVertex3f(+.4,-1.15,+.5);
  glTexCoord2f(1,1); glVertex3f(+.5,-1.15,+.5);
  glTexCoord2f(0,1); glVertex3f(+.5,-1.4,+.5);
  // top
  glVertex3f(+.4,-1.15,+.5);
  glVertex3f(+.4,-1.15,.6);
  glVertex3f(+.5,-1.15,.6);
  glVertex3f(+.5,-1.15,.5);
  glEnd();
  // left back
  //right
  glBegin(GL_QUADS);
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.4,-1.4,.5);
  glTexCoord2f(1,0); glVertex3f(-.4,-1.4,.6);
  glTexCoord2f(1,1); glVertex3f(-.4,-1.15,.6);
  glTexCoord2f(0,1); glVertex3f(-.4,-1.15,.5);
  // left
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-1.4,.5);
  glTexCoord2f(1,0); glVertex3f(-.5,-1.4,.6);
  glTexCoord2f(1,1); glVertex3f(-.5,-1.15,.6);
  glTexCoord2f(0,1); glVertex3f(-.5,-1.15,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.4,-1.4,.6);
  glTexCoord2f(1,0); glVertex3f(-.4,-1.15,.6);
  glTexCoord2f(1,1); glVertex3f(-.5,-1.15,.6);
  glTexCoord2f(0,1); glVertex3f(-.5,-1.4,.6);
  // back
  glNormal3f( 0, 0,  -1);
  glTexCoord2f(0,0); glVertex3f(-.4,-1.4,+.5);
  glTexCoord2f(1,0); glVertex3f(-.4,-1.15,+.5);
  glTexCoord2f(1,1); glVertex3f(-.5,-1.15,+.5);
  glTexCoord2f(0,1); glVertex3f(-.5,-1.4,+.5);
  // top
  glVertex3f(-.4,-1.15,+.5);
  glVertex3f(-.4,-1.15,.6);
  glVertex3f(-.5,-1.15,.6);
  glVertex3f(-.5,-1.15,.5);
  glEnd();
  // left front
  //right
  glBegin(GL_QUADS);
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.4,-1.4,1);
  glTexCoord2f(1,0); glVertex3f(-.4,-1.4,.9);
  glTexCoord2f(1,1); glVertex3f(-.4,-1.15,.9);
  glTexCoord2f(0,1); glVertex3f(-.4,-1.15,1);
  // left
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-1.4,+1);
  glTexCoord2f(1,0); glVertex3f(-.5,-1.4,.9);
  glTexCoord2f(1,1); glVertex3f(-.5,-1.15,.9);
  glTexCoord2f(0,1); glVertex3f(-.5,-1.15,+1);
  // back
  glNormal3f( 0, 0,  -1);
  glTexCoord2f(0,0); glVertex3f(-.4,-1.4,.9);
  glTexCoord2f(1,0); glVertex3f(-.4,-1.15,.9);
  glTexCoord2f(1,1); glVertex3f(-.5,-1.15,.9);
  glTexCoord2f(0,1); glVertex3f(-.5,-1.4,.9);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.4,-1.4,+1);
  glTexCoord2f(1,0); glVertex3f(-.4,-1.15,+1);
  glTexCoord2f(1,1); glVertex3f(-.5,-1.15,+1);
  glTexCoord2f(0,1); glVertex3f(-.5,-1.4,+1);
  // top
  glVertex3f(-.4,-1.15,+1);
  glVertex3f(-.4,-1.15,.9);
  glVertex3f(-.5,-1.15,.9);
  glVertex3f(-.5,-1.15,1);
  glEnd();

  //back of bench
  // fifth
  glBegin(GL_QUADS);
  //left
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.38,-1,.5);
  glTexCoord2f(1,0); glVertex3f(+.38,-1,.6);
  glTexCoord2f(1,1); glVertex3f(+.38,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(+.38,-.7,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.5,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(+.5,-1,.6);
  glTexCoord2f(1,1); glVertex3f(+.5,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(+.5,-.7,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(+.38,-1,.6);
  glTexCoord2f(1,0); glVertex3f(+.38,-.7,.6);
  glTexCoord2f(1,1); glVertex3f(+.5,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(+.5,-1,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(+.38,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(+.38,-.7,+.5);
  glTexCoord2f(1,1); glVertex3f(+.5,-.7,+.5);
  glTexCoord2f(0,1); glVertex3f(+.5,-1,+.5);
  // top
  glVertex3f(+.38,-.7,+.5);
  glVertex3f(+.38,-.7,.6);
  glVertex3f(+.5,-.7,.6);
  glVertex3f(+.5,-.7,.5);
  glEnd();
  // fourth
  //left
  glBegin(GL_QUADS);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.16,-1,.5);
  glTexCoord2f(1,0); glVertex3f(+.16,-1,.6);
  glTexCoord2f(1,1); glVertex3f(+.16,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(+.16,-.7,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(+.28,-1,.5);
  glTexCoord2f(1,0); glVertex3f(+.28,-1,.6);
  glTexCoord2f(1,1); glVertex3f(+.28,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(+.28,-.7,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(+.16,-1,.6);
  glTexCoord2f(1,0); glVertex3f(+.16,-.7,.6);
  glTexCoord2f(1,1); glVertex3f(+.28,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(+.28,-1,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(+.16,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(+.16,-.7,+.5);
  glTexCoord2f(1,1); glVertex3f(+.28,-.7,+.5);
  glTexCoord2f(0,1); glVertex3f(+.28,-1,+.5);
  // top
  glVertex3f(+.16,-.7,+.5);
  glVertex3f(+.16,-.7,.6);
  glVertex3f(+.28,-.7,.6);
  glVertex3f(+.28,-.7,.5);
  glEnd();
  // third
  //left
  glBegin(GL_QUADS);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.06,-1,.5);
  glTexCoord2f(1,0); glVertex3f(-.06,-1,.6);
  glTexCoord2f(1,1); glVertex3f(-.06,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.06,-.7,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(.06,-1,.5);
  glTexCoord2f(1,0); glVertex3f(.06,-1,.6);
  glTexCoord2f(1,1); glVertex3f(.06,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(.06,-.7,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.06,-1,.6);
  glTexCoord2f(1,0); glVertex3f(-.06,-.7,.6);
  glTexCoord2f(1,1); glVertex3f(.06,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(.06,-1,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-.06,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(-.06,-.7,+.5);
  glTexCoord2f(1,1); glVertex3f(.06,-.7,+.5);
  glTexCoord2f(0,1); glVertex3f(.06,-1,+.5);
  // top
  glVertex3f(-.06,-.7,+.5);
  glVertex3f(-.06,-.7,.6);
  glVertex3f(.06,-.7,.6);
  glVertex3f(.06,-.7,.5);
  glEnd();
  // fourth
  //left
  glBegin(GL_QUADS);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.28,-1,.5);
  glTexCoord2f(1,0); glVertex3f(-.28,-1,.6);
  glTexCoord2f(1,1); glVertex3f(-.28,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.28,-.7,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.16,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(-.16,-1,.6);
  glTexCoord2f(1,1); glVertex3f(-.16,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.16,-.7,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.28,-1,.6);
  glTexCoord2f(1,0); glVertex3f(-.28,-.7,.6);
  glTexCoord2f(1,1); glVertex3f(-.16,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.16,-1,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-.28,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(-.28,-.7,+.5);
  glTexCoord2f(1,1); glVertex3f(-.16,-.7,+.5);
  glTexCoord2f(0,1); glVertex3f(-.16,-1,+.5);
  // top
  glVertex3f(-.28,-.7,+.5);
  glVertex3f(-.28,-.7,.6);
  glVertex3f(-.16,-.7,.6);
  glVertex3f(-.16,-.7,.5);
  glEnd();
  // fifth
  //left
  glBegin(GL_QUADS);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-1,.5);
  glTexCoord2f(1,0); glVertex3f(-.5,-1,.6);
  glTexCoord2f(1,1); glVertex3f(-.5,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.5,-.7,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.38,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(-.38,-1,.6);
  glTexCoord2f(1,1); glVertex3f(-.38,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.38,-.7,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.5,-1,.6);
  glTexCoord2f(1,0); glVertex3f(-.5,-.7,.6);
  glTexCoord2f(1,1); glVertex3f(-.38,-.7,.6);
  glTexCoord2f(0,1); glVertex3f(-.38,-1,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-.5,-1,+.5);
  glTexCoord2f(1,0); glVertex3f(-.5,-.7,+.5);
  glTexCoord2f(1,1); glVertex3f(-.38,-.7,+.5);
  glTexCoord2f(0,1); glVertex3f(-.38,-1,+.5);
  // top
  glVertex3f(-.5,-.7,+.5);
  glVertex3f(-.5,-.7,.6);
  glVertex3f(-.38,-.7,.6);
  glVertex3f(-.38,-.7,.5);
  glEnd();

  //top of back
  glBegin(GL_QUADS);
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(-.5,-.6,.5);
  glTexCoord2f(1,0); glVertex3f(-.5,-.6,.6);
  glTexCoord2f(1,1); glVertex3f(-.5,-.75,.6);
  glTexCoord2f(0,1); glVertex3f(-.5,-.75,.5);
  // right
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0,0); glVertex3f(.5,-.6,+.5);
  glTexCoord2f(1,0); glVertex3f(.5,-.6,.6);
  glTexCoord2f(1,1); glVertex3f(.5,-.75,.6);
  glTexCoord2f(0,1); glVertex3f(.5,-.75,+.5);
  // front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0,0); glVertex3f(-.5,-.6,.6);
  glTexCoord2f(1,0); glVertex3f(-.5,-.75,.6);
  glTexCoord2f(1,1); glVertex3f(.5,-.75,.6);
  glTexCoord2f(0,1); glVertex3f(.5,-.6,.6);
  // back
  glNormal3f( 0, 0, -1);
  glTexCoord2f(0,0); glVertex3f(-.5,-.6,+.5);
  glTexCoord2f(1,0); glVertex3f(-.5,-.75,+.5);
  glTexCoord2f(1,1); glVertex3f(.5,-.75,+.5);
  glTexCoord2f(0,1); glVertex3f(.5,-.6,+.5);
  // top
  glNormal3f( 0, 1, 0);
  glVertex3f(-.5,-.6,+.5);
  glVertex3f(-.5,-.6,.6);
  glVertex3f(.5,-.6,.6);
  glVertex3f(.5,-.6,.5);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  //  Undo transformations
  glPopMatrix();
}

static void golfClubHead(double x, double y, double z,
                        double rx, double ry, double rz, double xh, 
                        double yh, double zh, char color)
{
  int i,k;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glScaled(rx,ry,rz);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);
  
  if (color == 'r'){
    glColor3ub(239, 0, 0);
  } 
  else if (color == 'b') {
    glColor3ub(0, 218, 242); 
  }
  else if (color == 'y') {
    glColor3ub(240, 224, 0); 
  }
  else if (color == 'g') {
    glColor3ub(0, 241, 52); 
  }
  else if (color == 'p') {
    glColor3ub(255, 30, 206); 
  }

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,1);
  for (i=1;i>=-1;i-=2)
   {
      glBindTexture(GL_TEXTURE_2D,texture[13]);
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,texture[13]);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
  //  Undo transformations
  glPopMatrix();

}

void golfClubGrip(double x,double y,double z,
                 double dx,double dy,double dz, double xh, double yh,
                 double zh, char color) {

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   double i;
   double radius = 0.3;
   double height = 0.5;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(xh,1,0,0);
   glRotated(yh,0,1,0);
   glRotated(zh,0,0,1);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   if (color == 'r'){
      glColor3ub(239, 0, 0);
   } 
   else if (color == 'b') {
      glColor3ub(0, 218, 242); 
   }
   else if (color == 'y') {
      glColor3ub(240, 224, 0); 
   }
   else if (color == 'g') {
      glColor3ub(0, 241, 52); 
   }
   else if (color == 'p') {
      glColor3ub(255, 30, 206); 
   }
   else if (color == 'd') {
      glColor3ub(0, 61, 160); 
   }

   glBindTexture(GL_TEXTURE_2D,texture[13]);

   int j;
   glBegin(GL_TRIANGLE_STRIP);
   for (j = 0; j <= 360; j++) {
      const float tc = (j / (float) 360);

      double x = radius * Cos(j);
      double y = height;
      double z = radius * Sin(j);

      glNormal3f(Cos(j), 1, Sin(j));

      glTexCoord2f(-tc, 0.0); glVertex3d(x, y, z);
      glTexCoord2f(-tc, 1.0); glVertex3d(x, -y, z);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[13]);

   glNormal3f(0,1,0);

   //top of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glTexCoord2f(0.5,0.5);  glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[13]);

   glNormal3f(0,-1,0);

   //bottom of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);  glVertex3d(0.0, -height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   glDisable(GL_TEXTURE_2D); 

   glPopMatrix();
}

void golfClubPole(double x,double y,double z,
                 double dx,double dy,double dz, double xh, double yh,
                 double zh) {

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   double i;
   double radius = 0.3;
   double height = 0.5;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(xh,1,0,0);
   glRotated(yh,0,1,0);
   glRotated(zh,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(1,1,1);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[12]);

   int j;
   glBegin(GL_TRIANGLE_STRIP);
   for (j = 0; j <= 360; j++) {
      const float tc = (j / (float) 360);

      double x = radius * Cos(j);
      double y = height;
      double z = radius * Sin(j);

      glNormal3f(Cos(j), 1, Sin(j));

      glTexCoord2f(-tc, 0.0); glVertex3d(x, y, z);
      glTexCoord2f(-tc, 1.0); glVertex3d(x, -y, z);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[12]);

   glNormal3f(0,1,0);

   //top of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glTexCoord2f(0.5,0.5);  glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[12]);

   glNormal3f(0,-1,0);

   //bottom of cylinder
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);  glVertex3d(0.0, -height, 0.0);
      // go 360 degrees, increment slowy so seems flawless
      for(i = 0.0; i < 360; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   glDisable(GL_TEXTURE_2D); 

   glPopMatrix();
}

static void iceCreamCart(double x,double y,double z,
                 double dx,double dy,double dz,
                 double xh, double yh, double zh) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  // glColor3ub(83, 108, 232);
  //body of cart
  glBindTexture(GL_TEXTURE_2D,texture[17]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,1); glVertex3f(-1.5,.75,.5);
  glTexCoord2f(1,1); glVertex3f(1,.75,.5);
  glTexCoord2f(1,0); glVertex3f(1,-1,.5);
  glTexCoord2f(0,0); glVertex3f(-1.5,-1,.5);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(1,1); glVertex3f(-1.5,.75,-1);
  glTexCoord2f(0,1); glVertex3f(1,.75,-1);
  glTexCoord2f(0,0); glVertex3f(1,-1,-1);
  glTexCoord2f(1,0); glVertex3f(-1.5,-1,-1);
  glEnd();
  //left
  glBindTexture(GL_TEXTURE_2D,texture[18]);
  glBegin(GL_QUADS);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-1.5,.75,-1);
  glTexCoord2f(1,0); glVertex3f(-1.5,.75,.5);
  glTexCoord2f(1,1); glVertex3f(-1.5,-1,.5);
  glTexCoord2f(0,1); glVertex3f(-1.5,-1,-1);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,.75,-1);
  glTexCoord2f(1,0); glVertex3f(1,.75,.5);
  glTexCoord2f(1,1); glVertex3f(1,-1,.5);
  glTexCoord2f(0,1); glVertex3f(1,-1,-1);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-1.5,.75,.5);
  glTexCoord2f(1,0); glVertex3f(1,.75,.5);
  glTexCoord2f(1,1); glVertex3f(1,.75,-1);
  glTexCoord2f(0,1); glVertex3f(-1.5,.75,-1);
  //bottom
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0); glVertex3f(-1.5,-1,.5);
  glTexCoord2f(1,0); glVertex3f(1,-1,.5);
  glTexCoord2f(1,1); glVertex3f(1,-1,-1);
  glTexCoord2f(0,1); glVertex3f(-1.5,-1,-1);
  glEnd();
  //legs
  //front leg
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(.8,-1,.5);
  glTexCoord2f(1,0); glVertex3f(1,-1,.5);
  glTexCoord2f(1,1); glVertex3f(1,-2,.5);
  glTexCoord2f(0,1); glVertex3f(.8,-2,.5);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(.8,-1,.3);
  glTexCoord2f(1,0); glVertex3f(1,-1,.3);
  glTexCoord2f(1,1); glVertex3f(1,-2,.3);
  glTexCoord2f(0,1); glVertex3f(.8,-2,.3);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(.8,-1,.3);
  glTexCoord2f(1,0); glVertex3f(.8,-1,.5);
  glTexCoord2f(1,1); glVertex3f(.8,-2,.5);
  glTexCoord2f(0,1); glVertex3f(.8,-2,.3);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,-1,.3);
  glTexCoord2f(1,0); glVertex3f(1,-1,.5);
  glTexCoord2f(1,1); glVertex3f(1,-2,.5);
  glTexCoord2f(0,1); glVertex3f(1,-2,.3);
  glEnd();
  //back leg
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(.8,-1,-.8);
  glTexCoord2f(1,0); glVertex3f(1,-1,-.8);
  glTexCoord2f(1,1); glVertex3f(1,-2,-.8);
  glTexCoord2f(0,1); glVertex3f(.8,-2,-.8);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(.8,-1,-1);
  glTexCoord2f(1,0); glVertex3f(1,-1,-1);
  glTexCoord2f(1,1); glVertex3f(1,-2,-1);
  glTexCoord2f(0,1); glVertex3f(.8,-2,-1);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(.8,-1,-1);
  glTexCoord2f(1,0); glVertex3f(.8,-1,-.8);
  glTexCoord2f(1,1); glVertex3f(.8,-2,-.8);
  glTexCoord2f(0,1); glVertex3f(.8,-2,-1);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,-1,-1);
  glTexCoord2f(1,0); glVertex3f(1,-1,-.8);
  glTexCoord2f(1,1); glVertex3f(1,-2,-.8);
  glTexCoord2f(0,1); glVertex3f(1,-2,-1);
  glEnd();
  //roof
  glBindTexture(GL_TEXTURE_2D,texture[20]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,1); glVertex3f(-1.5,2.25,.5);
  glTexCoord2f(1,1); glVertex3f(1,2.25,.5);
  glTexCoord2f(1,0); glVertex3f(1,2.15,.5);
  glTexCoord2f(0,0); glVertex3f(-1.5,2.15,.5);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,1); glVertex3f(-1.5,2.25,-1);
  glTexCoord2f(1,1); glVertex3f(1,2.25,-1);
  glTexCoord2f(1,0); glVertex3f(1,2.15,-1);
  glTexCoord2f(0,0); glVertex3f(-1.5,2.15,-1);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-1.5,2.25,-1);
  glTexCoord2f(1,0); glVertex3f(-1.5,2.25,.5);
  glTexCoord2f(1,1); glVertex3f(-1.5,2.15,.5);
  glTexCoord2f(0,1); glVertex3f(-1.5,2.15,-1);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,2.25,-1);
  glTexCoord2f(1,0); glVertex3f(1,2.25,.5);
  glTexCoord2f(1,1); glVertex3f(1,2.15,.5);
  glTexCoord2f(0,1); glVertex3f(1,2.15,-1);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-1.5,2.25,.5);
  glTexCoord2f(1,0); glVertex3f(1,2.25,.5);
  glTexCoord2f(1,1); glVertex3f(1,2.25,-1);
  glTexCoord2f(0,1); glVertex3f(-1.5,2.25,-1);
  //bottom
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0); glVertex3f(-1.5,2.15,.5);
  glTexCoord2f(1,0); glVertex3f(1,2.15,.5);
  glTexCoord2f(1,1); glVertex3f(1,2.15,-1);
  glTexCoord2f(0,1); glVertex3f(-1.5,2.15,-1);
  glEnd();


  glDisable(GL_TEXTURE_2D); 

  glPopMatrix();
}

static void iceCreamWheel(double x, double y, double z,
                        double rx, double ry, double rz, double xh, 
                        double yh, double zh)
{
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glScaled(rx,ry,rz);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);

  glColor3f(1,1,1);
  
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,texture[16]);

  //edgem
  const double TAU = 2 * PI;

  double r = 0.05;
  double c = 0.55;
  int rSeg = 16;
  int cSeg = 20;

  int i,j,k;

  for (i = 0; i < rSeg; i++) {
    glBegin(GL_QUAD_STRIP);
    for (j = 0; j <= cSeg; j++) {
      for (k = 1; k >= 0; k--) {
        double s = (i + k) % rSeg + 0.5;
        double t = j % cSeg;

        double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
        double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
        double z = r * sin(s * TAU / rSeg);

        double u = (i + k) / (float) rSeg;
        double v = t / (float) cSeg;

        glTexCoord2d(u, v);
        glNormal3f(2 * x, 2 * y, 2 * z);
        glVertex3f(2 * x, 2 * y, 2 * z);
      }
    }
    glEnd();
  }

   //wheel spokes
  int q;
  glColor3ub(192,192,192);
  //glRotatef(frameNumber*20,0,0,1);
  glBegin(GL_LINES);
  for (q = 0; q < 20; q++) {
    glVertex3f(0,0,0.15);
    glVertex3d(cos(q*2*PI/20), sin(q*2*PI/20), 0.05);
  }
  glEnd();
  glBegin(GL_LINES);
  for (q = 0; q < 20; q++) {
    glVertex3f(0,0,-0.15);
    glVertex3d(cos(q*2*PI/20), sin(q*2*PI/20), -0.05);
  }
  glEnd();

  glDisable(GL_TEXTURE_2D); 

  //  Undo transformations
  glPopMatrix();

}

static void iceCreamCone(double x, double y, double z,
                        double rx, double ry, double rz, double xh, 
                        double yh, double zh)
{
  int i,k;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glScaled(rx,ry,rz);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,1);
  for (i=1;i>=-1;i-=2)
   {
      glBindTexture(GL_TEXTURE_2D,texture[19]);
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),3);
      }
      glEnd();
   }

  glDisable(GL_TEXTURE_2D); 
   
  //  Undo transformations
  glPopMatrix();

}

static void iceCream(double x,double y,double z,double r, char color)
{
   // const int d=5;
   // int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   double az;
   double el;
   double difference = 3.0;
   double bigdifference = difference * 4.0;

   if (color == 's'){
      glColor3ub(237, 166, 222);
   } 
   else if (color == 'c') {
      glColor3ub(79, 48, 21); 
   }
   else if (color == 'v') {
      glColor3ub(247, 234, 222); 
   }
   else if (color == 'm') {
      glColor3ub(169, 239, 115); 
   }

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[21]);

   for(el = -90.0; el < 90; el+=difference)
    {
      glBegin(GL_QUAD_STRIP);
      for(az = 0.0; az <= 360.0; az+=bigdifference)
      {
        glTexCoord2f(az/360,(el+90.0)/180);
        glNormal3d(Sin(az)*Cos(el),Sin(el),Cos(az)*Cos(el));
        glVertex3d(Sin(az)*Cos(el),Sin(el),Cos(az)*Cos(el));
        glTexCoord2f((az+bigdifference)/360,(el+90.0)/180);
        glNormal3d(Sin(az)*Cos(el+difference),Sin(el+difference),Cos(az)*Cos(el+difference));
        glVertex3d(Sin(az)*Cos(el+difference),Sin(el+difference),Cos(az)*Cos(el+difference));
      }
      glEnd();
    }

    glDisable(GL_TEXTURE_2D);

   //  Undo transformations
   glPopMatrix();
}

static void ferrisWheel(double x, double y, double z,
                        double rx, double ry, double rz, double xh, 
                        double yh, double zh)
{
  int k;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glScaled(rx,ry,rz);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);

  glColor3f(1,1,1);
  
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,1);
  //  Edge
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  glBegin(GL_QUAD_STRIP);
  for (k=0;k<=360;k+=10)
  {
    glNormal3f(Cos(k),Sin(k),0);
    glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
    glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
  }
  glEnd();

   //wheel spokes
  int j;
  glRotated(frameNumber * (3.5), 0, 0, 1);
  glColor3ub(255,255,255);
  glBegin(GL_LINES);
  for (j = 0; j < 12; j++) {
    glVertex3f(0,0,2.05);
    glVertex3d(cos(j*2*PI/12), sin(j*2*PI/12), 1.05);
  }
  glEnd();
  glBegin(GL_LINES);
  for (j = 0; j < 12; j++) {
    glVertex3f(0,0,-2.125);
    glVertex3d(cos(j*2*PI/12), sin(j*2*PI/12), -1.125);
  }
  glEnd();

  //seats
  int i;
  for (i = 0; i < 12; i++) {
    glRotated(30, 0, 0, 1);  // Note: These rotations accumulate.
    glColor3ub(221, 17, 17);
    //attachments
    //1st
    glBegin(GL_POLYGON);
    //back
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(1, 0.01, .75);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, .75);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, .75);
    glTexCoord2f(0,1); glVertex3f(1, -0.01, .75);
    //front
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(1, 0.01, .8);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, .8);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, .8);
    glTexCoord2f(0,1); glVertex3f(1, -0.01, .8);
    //top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(1, 0.01, .8);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, .8);
    glTexCoord2f(1,1); glVertex3f(1.15, 0.01, .75);
    glTexCoord2f(0,1); glVertex3f(1, 0.01, .75);
    //bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(1, -0.01, .8);
    glTexCoord2f(1,0); glVertex3f(1.15, -0.01, .8);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, .75);
    glTexCoord2f(0,1); glVertex3f(1, -0.01, .75);
    //right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(1.15, 0.01, .75);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, .8);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, .8);
    glTexCoord2f(0,1); glVertex3f(1.15, -0.01, .75);
    glEnd();
    //attachments
    //2nd
    glBegin(GL_POLYGON);
    //back
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(1, 0.01, -.8);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, -.8);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, -.8);
    glTexCoord2f(0,1); glVertex3f(1, -0.01, -.8);
    //front
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(1, 0.01, -.75);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, -.75);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, -.75);
    glTexCoord2f(0,1); glVertex3f(1, -0.01, -.75);
    //top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(1, 0.01, -.75);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, -.75);
    glTexCoord2f(1,1); glVertex3f(1.15, 0.01, -.8);
    glTexCoord2f(0,1); glVertex3f(1, 0.01, -.8);
    //bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(1, -0.01, -.75);
    glTexCoord2f(1,0); glVertex3f(1.15, -0.01, -.75);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, -.8);
    glTexCoord2f(0,1); glVertex3f(1, -0.01, -.8);
    //right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(1.15, 0.01, -.8);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.01, -.75);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.01, -.75);
    glTexCoord2f(0,1); glVertex3f(1.15, -0.01, -.8);
    glEnd();
    //seat
    glBegin(GL_POLYGON);
    //back
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(1.15, 0.15, -1.15);
    glTexCoord2f(1,0); glVertex3f(1.35, 0.15, -1.15);
    glTexCoord2f(1,1); glVertex3f(1.35, -0.15, -1.15);
    glTexCoord2f(0,1); glVertex3f(1.15, -0.15, -1.15);
    //front
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(1.15, 0.15, 1.15);
    glTexCoord2f(1,0); glVertex3f(1.35, 0.15, 1.15);
    glTexCoord2f(1,1); glVertex3f(1.35, -0.15, 1.15);
    glTexCoord2f(0,1); glVertex3f(1.15, -0.15, 1.15);
    //top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(1.15, 0.15, 1.15);
    glTexCoord2f(1,0); glVertex3f(1.35, 0.15, 1.15);
    glTexCoord2f(1,1); glVertex3f(1.35, 0.15, -1.15);
    glTexCoord2f(0,1); glVertex3f(1.15, 0.15, -1.15);
    //bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(1.15, -0.15, 1.15);
    glTexCoord2f(1,0); glVertex3f(1.35, -0.15, 1.15);
    glTexCoord2f(1,1); glVertex3f(1.35, -0.15, -1.15);
    glTexCoord2f(0,1); glVertex3f(1.35, -0.15, -1.15);
    //right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(1.35, 0.15, -1.15);
    glTexCoord2f(1,0); glVertex3f(1.35, 0.15, 1.15);
    glTexCoord2f(1,1); glVertex3f(1.35, -0.15, 1.15);
    glTexCoord2f(0,1); glVertex3f(1.35, -0.15, -1.15);
    //left
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(1.15, 0.15, -1.15);
    glTexCoord2f(1,0); glVertex3f(1.15, 0.15, 1.15);
    glTexCoord2f(1,1); glVertex3f(1.15, -0.15, 1.15);
    glTexCoord2f(0,1); glVertex3f(1.15, -0.15, -1.15);
    glEnd();
  }

  glDisable(GL_TEXTURE_2D); 

  //  Undo transformations
  glPopMatrix();

}

static void ringToss(double x,double y,double z,
                 double dx,double dy,double dz,
                 double xh, double yh, double zh) {
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  //glColor3ub(0, 61, 160); 
  //bottom of game
  glBindTexture(GL_TEXTURE_2D,texture[22]);
  glBegin(GL_QUADS);
  //bottom
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-.8);
  glTexCoord2f(1,0); glVertex3f(2.5,0,-.8);
  glTexCoord2f(1,1); glVertex3f(2.5,0,-4);
  glTexCoord2f(0,1); glVertex3f(-2.5,0,-4);
  glEnd();
  //sides
  //left side
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-.8);
  glTexCoord2f(1,0); glVertex3f(-2.3,0,-.8);
  glTexCoord2f(1,1); glVertex3f(-2.3,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(-2.5,.5,-.8);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-4);
  glTexCoord2f(1,0); glVertex3f(-2.3,0,-4);
  glTexCoord2f(1,1); glVertex3f(-2.3,2,-4);
  glTexCoord2f(0,1); glVertex3f(-2.5,2,-4);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-4);
  glTexCoord2f(1,0); glVertex3f(-2.5,0,-.8);
  glTexCoord2f(1,1); glVertex3f(-2.5,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(-2.5,2,-4);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,0,-.8);
  glTexCoord2f(1,0); glVertex3f(-2.3,0,-4);
  glTexCoord2f(1,1); glVertex3f(-2.3,2,-4);
  glTexCoord2f(0,1); glVertex3f(-2.3,.5,-.8);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.5,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(-2.3,.5,-.8);
  glTexCoord2f(1,1); glVertex3f(-2.3,2,-4);
  glTexCoord2f(1,0); glVertex3f(-2.5,2,-4);
  glEnd();
  // //right side
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(2.3,0,-.8);
  glTexCoord2f(1,0); glVertex3f(2.5,0,-.8);
  glTexCoord2f(1,1); glVertex3f(2.5,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(2.3,.5,-.8);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(2.3,0,-4);
  glTexCoord2f(1,0); glVertex3f(2.5,0,-4);
  glTexCoord2f(1,1); glVertex3f(2.5,2,-4);
  glTexCoord2f(0,1); glVertex3f(2.3,2,-4);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.3,0,-4);
  glTexCoord2f(1,0); glVertex3f(2.3,0,-.8);
  glTexCoord2f(1,1); glVertex3f(2.3,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(2.3,2,-4);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.5,0,-.8);
  glTexCoord2f(1,0); glVertex3f(2.5,0,-4);
  glTexCoord2f(1,1); glVertex3f(2.5,2,-4);
  glTexCoord2f(0,1); glVertex3f(2.5,.5,-.8);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(2.3,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(2.5,.5,-.8);
  glTexCoord2f(1,1); glVertex3f(2.5,2,-4);
  glTexCoord2f(1,0); glVertex3f(2.3,2,-4);
  glEnd();
  // top sign
  glBindTexture(GL_TEXTURE_2D,texture[23]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.501,0,-3.8);
  glTexCoord2f(1,0); glVertex3f(2.501,0,-3.8);
  glTexCoord2f(1,1); glVertex3f(2.501,3.5,-3.8);
  glTexCoord2f(0,1); glVertex3f(-2.501,3.5,-3.8);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,texture[22]);
  glBegin(GL_QUADS);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.501,0,-4.1);
  glTexCoord2f(1,0); glVertex3f(2.501,0,-4.1);
  glTexCoord2f(1,1); glVertex3f(2.501,3.5,-4.1);
  glTexCoord2f(0,1); glVertex3f(-2.501,3.5,-4.1);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.501,0,-4.1);
  glTexCoord2f(0,1); glVertex3f(-2.501,0,-3.8);
  glTexCoord2f(1,1); glVertex3f(-2.501,3.5,-3.8);
  glTexCoord2f(1,0); glVertex3f(-2.501,3.5,-4.1);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.501,0,-3.8);
  glTexCoord2f(0,1); glVertex3f(2.501,0,-4.1);
  glTexCoord2f(1,1); glVertex3f(2.501,3.5,-4.1);
  glTexCoord2f(1,0); glVertex3f(2.501,3.5,-3.8);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.501,3.5,-3.8);
  glTexCoord2f(1,0); glVertex3f(2.501,3.5,-3.8);
  glTexCoord2f(1,1); glVertex3f(2.501,3.5,-4.1);
  glTexCoord2f(0,1); glVertex3f(-2.501,3.5,-4.1);
  glEnd();
  // //bottom sign
  glBindTexture(GL_TEXTURE_2D,texture[22]);
  glBegin(GL_QUADS);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-.6);
  glTexCoord2f(1,0); glVertex3f(2.5,0,-.6);
  glTexCoord2f(1,1); glVertex3f(2.5,.5,-.6);
  glTexCoord2f(0,1); glVertex3f(-2.5,.5,-.6);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-.8);
  glTexCoord2f(1,0); glVertex3f(2.5,0,-.8);
  glTexCoord2f(1,1); glVertex3f(2.5,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(-2.5,.5,-.8);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.5,.5,-.6);
  glTexCoord2f(1,0); glVertex3f(2.5,.5,-.6);
  glTexCoord2f(1,1); glVertex3f(2.5,.5,-.8);
  glTexCoord2f(0,1); glVertex3f(-2.5,.5,-.8);
  glEnd();
  //ledges
  //1st
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  glBegin(GL_QUADS);
  glColor3ub(255,255,255);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.2,-1.05);
  glTexCoord2f(1,0); glVertex3f(2.3,0.2,-1.05);
  glTexCoord2f(1,1); glVertex3f(2.3,0.9,-1.05);
  glTexCoord2f(0,1); glVertex3f(-2.3,0.9,-1.05);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.2,-1.55);
  glTexCoord2f(1,0); glVertex3f(2.3,0.2,-1.55);
  glTexCoord2f(1,1); glVertex3f(2.3,0.9,-1.55);
  glTexCoord2f(0,1); glVertex3f(-2.3,0.9,-1.55);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.2,-1.55);
  glTexCoord2f(1,0); glVertex3f(-2.3,0.2,-1.05);
  glTexCoord2f(1,1); glVertex3f(-2.3,0.9,-1.05);
  glTexCoord2f(0,1); glVertex3f(-2.3,0.9,-1.55);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.3,0.2,-1.05);
  glTexCoord2f(1,0); glVertex3f(2.3,0.2,-1.55);
  glTexCoord2f(1,1); glVertex3f(2.3,0.9,-1.55);
  glTexCoord2f(0,1); glVertex3f(2.3,0.9,-1.05);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.9,-1.05);
  glTexCoord2f(1,0); glVertex3f(2.3,0.9,-1.05);
  glTexCoord2f(1,1); glVertex3f(2.3,0.9,-1.55);
  glTexCoord2f(0,1); glVertex3f(-2.3,0.9,-1.55);
  glEnd();
  //2nd
  glBegin(GL_QUADS);
  glColor3ub(255,255,255);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.7,-1.8);
  glTexCoord2f(1,0); glVertex3f(2.3,0.7,-1.8);
  glTexCoord2f(1,1); glVertex3f(2.3,1.2,-1.8);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.2,-1.8);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.7,-2.3);
  glTexCoord2f(1,0); glVertex3f(2.3,0.7,-2.3);
  glTexCoord2f(1,1); glVertex3f(2.3,1.2,-2.3);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.2,-2.3);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,0.7,-2.3);
  glTexCoord2f(1,0); glVertex3f(-2.3,0.7,-1.8);
  glTexCoord2f(1,1); glVertex3f(-2.3,1.2,-1.8);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.2,-2.3);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.3,0.7,-1.8);
  glTexCoord2f(1,0); glVertex3f(2.3,0.7,-2.3);
  glTexCoord2f(1,1); glVertex3f(2.3,1.2,-2.3);
  glTexCoord2f(0,1); glVertex3f(2.3,1.2,-1.8);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,1.2,-1.8);
  glTexCoord2f(1,0); glVertex3f(2.3,1.2,-1.8);
  glTexCoord2f(1,1); glVertex3f(2.3,1.2,-2.3);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.2,-2.3);
  glEnd();
  //3rd
  glBegin(GL_QUADS);
  glColor3ub(255,255,255);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.3,1,-2.55);
  glTexCoord2f(1,0); glVertex3f(2.3,1,-2.55);
  glTexCoord2f(1,1); glVertex3f(2.3,1.5,-2.55);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.5,-2.55);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.3,1,-3.05);
  glTexCoord2f(1,0); glVertex3f(2.3,1,-3.05);
  glTexCoord2f(1,1); glVertex3f(2.3,1.5,-3.05);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.5,-3.05);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,1,-3.05);
  glTexCoord2f(1,0); glVertex3f(-2.3,1,-2.55);
  glTexCoord2f(1,1); glVertex3f(-2.3,1.5,-2.55);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.5,-3.05);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.3,1,-2.55);
  glTexCoord2f(1,0); glVertex3f(2.3,1,-3.05);
  glTexCoord2f(1,1); glVertex3f(2.3,1.5,-3.05);
  glTexCoord2f(0,1); glVertex3f(2.3,1.5,-2.55);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,1.5,-2.55);
  glTexCoord2f(1,0); glVertex3f(2.3,1.5,-2.55);
  glTexCoord2f(1,1); glVertex3f(2.3,1.5,-3.05);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.5,-3.05);
  glEnd();
  //4th
  glBegin(GL_QUADS);
  glColor3ub(255,255,255);
  //front
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-2.3,1.3,-3.3);
  glTexCoord2f(1,0); glVertex3f(2.3,1.3,-3.3);
  glTexCoord2f(1,1); glVertex3f(2.3,1.8,-3.3);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.8,-3.3);
  //back
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(-2.3,1.3,-3.8);
  glTexCoord2f(1,0); glVertex3f(2.3,1.3,-3.8);
  glTexCoord2f(1,1); glVertex3f(2.3,1.8,-3.8);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.8,-3.8);
  //left
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,1.3,-3.8);
  glTexCoord2f(1,0); glVertex3f(-2.3,1.3,-3.3);
  glTexCoord2f(1,1); glVertex3f(-2.3,1.8,-3.3);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.8,-3.8);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.3,1.3,-3.3);
  glTexCoord2f(1,0); glVertex3f(2.3,1.3,-3.8);
  glTexCoord2f(1,1); glVertex3f(2.3,1.8,-3.8);
  glTexCoord2f(0,1); glVertex3f(2.3,1.8,-3.3);
  //top
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-2.3,1.8,-3.3);
  glTexCoord2f(1,0); glVertex3f(2.3,1.8,-3.3);
  glTexCoord2f(1,1); glVertex3f(2.3,1.8,-3.8);
  glTexCoord2f(0,1); glVertex3f(-2.3,1.8,-3.8);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);
  //left
  glColor3ub(186,11,11);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(-2.5,0,-.8);
  glTexCoord2f(0,1); glVertex3f(-2.5,0,-.6);
  glTexCoord2f(1,1); glVertex3f(-2.5,.5,-.6);
  glTexCoord2f(1,0); glVertex3f(-2.5,.5,-.8);
  //right
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(2.5,0,-.6);
  glTexCoord2f(0,1); glVertex3f(2.5,0,-.8);
  glTexCoord2f(1,1); glVertex3f(2.5,.5,-.8);
  glTexCoord2f(1,0); glVertex3f(2.5,.5,-.6);
  glEnd(); 

  glColor3f(1,1,1);

  glPopMatrix();
}

static void ring(double x, double y, double z, 
                double dx, double dy, double dz,
                double xh, double yh, double zh)
{
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glRotated(xh,1,0,0);
  glRotated(yh,0,1,0);
  glRotated(zh,0,0,1);
  glScaled(dx,dy,dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 
  glBindTexture(GL_TEXTURE_2D,texture[18]);

  glColor3ub(0, 61, 160);

  const double TAU = 2 * PI;

  double r = 0.05;
  double c = 0.15;
  int rSeg = 16;
  int cSeg = 18;

  int i,j,k;

  for (i = 0; i < rSeg; i++) {
    glBegin(GL_QUAD_STRIP);
    for (j = 0; j <= cSeg; j++) {
      for (k = 1; k >= 0; k--) {
        double s = (i + k) % rSeg + 0.5;
        double t = j % cSeg;

        double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
        double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
        double z = r * sin(s * TAU / rSeg);

        double u = (i + k) / (float) rSeg;
        double v = t / (float) cSeg;

        glTexCoord2d(u, v);
        glNormal3f(2 * x, 2 * y, 2 * z);
        glVertex3f(2 * x, 2 * y, 2 * z);
      }
    }
    glEnd();
  }

  glColor3f(1,1,1);

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void lightSource(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

//ex 8
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   //first person
   if (fps) {
      // need to use captital Sin and Cos otherwise rotates around pi
      cx = 2*dim*Sin(angle);
      cz = -2*dim*Cos(angle);
      gluLookAt(fx,fy,fz, fx+cx,fy,fz+cz, 0,1,0);
   }

   else if (view) {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
        
     }
  else {
        double px = -2*dim*Sin(th)*Cos(ph);
        double py = +2*dim        *Sin(ph);
        double pz = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(px,py,pz , 0,0,0 , 0,Cos(ph),0);
     }

      //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch ex13
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        lightSource(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //  Decide what to draw
   //ex8
   switch (mode)
   {
      //hole 1
      case 0:
        skybox(0,0,0, 1,1,1, 0);
        puttingGreen(0,0,0, 2,2,2, 0);
        golfBall(-.0125,.4,8.45, .175);
        wall(0,0,0, 2,2,2, 0);
        flagPole(0,1.85,-7, .25,3.5,.25, 0);
        flag(0,.95,0, 2,2,2, 0);
        windmill(0,3.1,-1.75, 1.85,1.6,1.6, 0);
        //tree 1
        //trunk
        treeTrunk(-7,1.35,-5, 1,2.5,1, 0,0,0);
        //first frow of branches
        treeTrunk(-7,3.25,-5.75, .65,2,.65, 315,0,0);
        treeTrunk(-7.75,3.25,-5, .65,2,.65, 0,0,45);
        treeTrunk(-6.25,3.25,-5, .65,2,.65, 0,0,315);
        //second row of branches
        //left
        treeTrunk(-8.4,3.35,-4.85, .45,1.15,.45, 27,0,75);
        //middle
        treeTrunk(-6.6,3.65,-6.05, .45,1.15,.45, 350,0,300);
        treeTrunk(-7.45,3.4,-5.8, .45,1.15,.45, 350,0,60);
        //right
        treeTrunk(-5.75,3.25,-5.125, .45,1.15,.45, 350,0,280);
        //third row of branches
        //left
        treeTrunk(-8.15,4.255,-5.125, .45,.75,.45, 350,0,345);
        treeTrunk(-8.7,4.15,-4.85, .45,.75,.45, 27,0,40);
        //middle
        treeTrunk(-6.8,4.255,-6.425, .45,.75,.45, 350,0,345);
        treeTrunk(-7.2,4.15,-6.15, .45,.75,.45, 45,0,20);
        //right
        treeTrunk(-5.4,4.15,-5.125, .45,.75,.45, 350,0,325);
        treeTrunk(-5.95,4.15,-4.85, .45,.75,.45, 27,0,40);
        //leaves
        leaves(-5.2,4.75,-5, .25,.25,.25, 20,0,0);
        leaves(-5.2,4.75,-5.3, .25,.25,.25, 20,0,0);
        leaves(-5.35,4.95,-5.15, .35,.35,.35, 20,0,30);
        leaves(-5.35,4.95,-5.55, .35,.35,.35, 20,0,30);
        leaves(-5.55,4.55,-4.8, .25,.25,.25, 50,0,330);
        leaves(-5.9,4.9,-4.8, .375,.375,.375, 20,0,0);
        leaves(-5.15,3.35,-5.1, .25,.25,.25, 20,0,0);
        leaves(-7,5.25,-5.15, .5,.5,.5, 20,0,70);
        leaves(-6.55,5.25,-5.45, .55,.55,.55, 20,0,70);
        leaves(-6.55,5.25,-4.5, .4,.4,.4, 40,0,100);
        leaves(-6.55,5.25,-6, .3,.3,.3, 40,0,20);
        leaves(-6.55,4.75,-6.25, .3,.3,.3, 40,0,20);
        leaves(-6.15,4.75,-6.25, .3,.3,.3, 40,0,20);
        leaves(-6.15,4.15,-6.25, .3,.3,.3, 40,0,20);
        leaves(-7.15,4.85,-6.25, .35,.35,.35, 40,0,20);
        leaves(-7.35,4.85,-5.95, .35,.35,.35, 320,0,20);
        leaves(-7.25,5.25,-4.5, .4,.4,.4, 340,0,50);
        leaves(-7.95,5.25,-4.5, .4,.4,.4, 20,0,320);
        leaves(-7.75,4.95,-4.5, .4,.4,.4, 20,0,320);
        leaves(-8.25,5.25,-4.5, .4,.4,.4, 340,0,50);
        leaves(-7.75,4.95,-5.5, .4,.4,.4, 20,0,320);
        leaves(-8.75,4.75,-4.55, .3,.3,.3, 20,0,320);
        leaves(-8.95,4.55,-4.55, .3,.3,.3, 320,0,20);
        leaves(-8.95,3.55,-4.55, .25,.25,.25, 320,0,20);
        leaves(-7.95,3.65,-5.95, .25,.25,.25, 320,0,20);
        leaves(-4.8,4.75,-5.55, .3,.3,.3, 20,0,30);
        //tree 2
        //trunk
        treeTrunk(7,1.35,-5, 1,2.5,1, 0,0,0);
        //first frow of branches
        treeTrunk(7,3.25,-5.75, .65,2,.65, 315,0,0);
        treeTrunk(6.25,3.25,-5, .65,2,.65, 0,0,45);
        treeTrunk(7.75,3.25,-5, .65,2,.65, 0,0,315);
        //second row of branches
        //left
        treeTrunk(5.6,3.35,-4.85, .45,1.15,.45, 27,0,75);
        //middle
        treeTrunk(7.4,3.65,-6.05, .45,1.15,.45, 350,0,300);
        treeTrunk(6.55,3.4,-5.8, .45,1.15,.45, 350,0,60);
        //right
        treeTrunk(8.25,3.25,-5.125, .45,1.15,.45, 350,0,280);
        //third row of branches
        //left
        treeTrunk(5.85,4.255,-5.125, .45,.75,.45, 350,0,345);
        treeTrunk(5.3,4.15,-4.85, .45,.75,.45, 27,0,40);
        //middle
        treeTrunk(7.2,4.255,-6.425, .45,.75,.45, 350,0,345);
        treeTrunk(6.8,4.15,-6.15, .45,.75,.45, 45,0,20);
        //right
        treeTrunk(8.6,4.15,-5.125, .45,.75,.45, 350,0,325);
        treeTrunk(8.05,4.15,-4.85, .45,.75,.45, 27,0,40);
        //leaves
        leaves(8.8,4.75,-5, .25,.25,.25, 20,0,0);
        leaves(8.8,4.75,-5.3, .25,.25,.25, 20,0,0);
        leaves(8.65,4.95,-5.15, .35,.35,.35, 20,0,30);
        leaves(8.65,4.95,-5.55, .35,.35,.35, 20,0,30);
        leaves(8.45,4.55,-4.8, .25,.25,.25, 50,0,330);
        leaves(8.1,4.9,-4.8, .375,.375,.375, 20,0,0);
        leaves(8.85,3.35,-5.1, .25,.25,.25, 20,0,0);
        leaves(7,5.25,-5.15, .5,.5,.5, 20,0,70);
        leaves(7.45,5.25,-5.45, .55,.55,.55, 20,0,70);
        leaves(7.45,5.25,-4.5, .4,.4,.4, 40,0,100);
        leaves(7.45,5.25,-6, .3,.3,.3, 40,0,20);
        leaves(7.45,4.75,-6.25, .3,.3,.3, 40,0,20);
        leaves(7.85,4.75,-6.25, .3,.3,.3, 40,0,20);
        leaves(7.85,4.15,-6.25, .3,.3,.3, 40,0,20);
        leaves(6.85,4.85,-6.25, .35,.35,.35, 40,0,20);
        leaves(6.65,4.85,-5.95, .35,.35,.35, 320,0,20);
        leaves(6.75,5.25,-4.5, .4,.4,.4, 340,0,50);
        leaves(6.05,5.25,-4.5, .4,.4,.4, 20,0,320);
        leaves(6.25,4.95,-4.5, .4,.4,.4, 20,0,320);
        leaves(5.75,5.25,-4.5, .4,.4,.4, 340,0,50);
        leaves(6.25,4.95,-5.5, .4,.4,.4, 20,0,320);
        leaves(5.25,4.75,-4.55, .3,.3,.3, 20,0,320);
        leaves(5.05,4.55,-4.55, .3,.3,.3, 320,0,20);
        leaves(5.05,3.55,-4.55, .25,.25,.25, 320,0,20);
        leaves(6.05,3.65,-5.95, .25,.25,.25, 320,0,20);
        leaves(9.2,4.75,-5.55, .3,.3,.3, 20,0,30);
        //benches
        bench(-15.5,2.85,-16, 3,2,3, 20);
        bench(15.5,2.85,-16, 3,2,3, 340);
        //red club
        golfClubHead(6.85,.25,6, .45,.175,.125, 0,0,0, 'r');
        golfClubGrip(7,.5,6, .3,.25,.3, 0,0,0, 'r');
        golfClubGrip(7,2.5,6, .3,1.15,.3, 0,0,0, 'r');
        golfClubPole(7,1.25,6, .25,1.5,.25, 0,0,0);
        //blue club
        golfClubHead(6.85,.25,6.5, .45,.175,.125, 0,0,0, 'b');
        golfClubGrip(7,.5,6.5, .3,.25,.3, 0,0,0, 'b');
        golfClubGrip(7,2.5,6.5, .3,1.15,.3, 0,0,0, 'b');
        golfClubPole(7,1.25,6.5, .25,1.5,.25, 0,0,0);
        //yellow club
        golfClubHead(6.85,.25,7, .45,.175,.125, 0,0,0, 'y');
        golfClubGrip(7,.5,7, .3,.25,.3, 0,0,0, 'y');
        golfClubGrip(7,2.5,7, .3,1.15,.3, 0,0,0, 'y');
        golfClubPole(7,1.25,7, .25,1.5,.25, 0,0,0);
        //green club
        golfClubHead(6.85,.25,7.55, .45,.175,.125, 0,0,0, 'g');
        golfClubGrip(7,.5,7.5, .3,.25,.3, 0,0,0, 'g');
        golfClubGrip(7,2.5,7.5, .3,1.15,.3, 0,0,0, 'g');
        golfClubPole(7,1.25,7.5, .25,1.5,.25, 0,0,0);
        //pink club
        golfClubHead(6.85,.25,8, .45,.175,.125, 0,0,0, 'p');
        golfClubGrip(7,.5,8, .3,.25,.3, 0,0,0, 'p');
        golfClubGrip(7,2.5,8, .3,1.15,.3, 0,0,0, 'p');
        golfClubPole(7,1.25,8, .25,1.5,.25, 0,0,0);
        //club rack
        golfClubPole(7.5,1.65,8.55, .35,3.15,.35, 0,0,0);
        golfClubPole(7.5,1.65,5.55, .35,3.15,.35, 0,0,0);
        golfClubPole(7.05,.15,8.55, .35,2,.35, 0,0,90);
        golfClubPole(7.05,.15,5.55, .35,2,.35, 0,0,90);
        golfClubPole(7.5,3.15,7.05, .35,3.175,.35, 0,90,90);
        golfClubPole(7.5,2.25,7.05, .35,3.155,.35, 0,90,90);
        golfClubPole(7.05,2.25,5.75, .35,1,.35, 0,0,90);
        golfClubPole(7.05,2.25,6.25, .35,1,.35, 0,0,90);
        golfClubPole(7.05,2.25,6.75, .35,1,.35, 0,0,90);
        golfClubPole(7.05,2.25,7.25, .35,1,.35, 0,0,90);
        golfClubPole(7.05,2.25,7.75, .35,1,.35, 0,0,90);
        golfClubPole(7.05,2.25,8.25, .35,1,.35, 0,0,90);
        // fence
        int i;
        // middle sides
        for(i = 0; i<=60; i+=2){
          fence(-5.25,.1,-1.25-(double)i/7, .075,1.5,.075, 90);
          fence(5.25,.1,-1.25-(double)i/7, .075,1.5,.075, 90);
        }
        //front middle
        for(i = -36; i<=36; i+=2){
          fence((double)i/7,.1,-10, .075,1.5,.075, 0);
        }
        //back
        for(i = -120; i<=120; i+=2){
          fence((double)i/7,.1,-18, .075,1.5,.075, 0);
        }
        // sides
        for(i = 0; i<=116; i+=2){
          fence(-17.325,.1,-1.25-(double)i/7, .075,1.5,.075, 90);
          fence(17.325,.1,-1.25-(double)i/7, .075,1.5,.075, 90);
        }
        //third front
        for(i = -8; i<=8; i+=2){
          fence(6.45-(double)i/7,.1,-1, .075,1.5,.075, 0);
        }
        //fourth front
        for(i = -8; i<=8; i+=2){
          fence(16.255-(double)i/7,.1,-1, .075,1.5,.075, 0);
        }
        //second front
        for(i = -8; i<=8; i+=2){
          fence(-6.45-(double)i/7,.1,-1, .075,1.5,.075, 0);
        }
        //first front
        for(i = -8; i<=8; i+=2){
          fence(-16.255-(double)i/7,.1,-1, .075,1.5,.075, 0);
        }
        //ice cream cart
        iceCreamCart(-10,2.075,6, 1.25,1,1, 0,0,0);
        iceCreamWheel(-11.5,1,6.575, .75,.75,1, 0,0,0);
        iceCreamWheel(-11.5,1,4.925, .75,.75,1, 0,0,0);
        golfClubPole(-11.5,1,5.725, .15,1.775,.15, 0,90,90);
        golfClubPole(-11.65,3.5,5.725, .15,1.775,.15, 0,0,0);
        golfClubPole(-9,3.5,5.725, .15,1.775,.15, 0,0,0);
        iceCreamCone(-11.15,3,5.75, .15,.15,.15, 270,0,0);
        iceCreamCone(-10.65,3,5.75, .15,.15,.15, 270,0,0);
        iceCreamCone(-10.15,3,5.75, .15,.15,.15, 270,0,0);
        iceCreamCone(-9.65,3,5.75, .15,.15,.15, 270,0,0);
        iceCream(-9.65,3.55,5.75, .175, 's');
        iceCream(-9.65,3.8,5.75, .175, 'c');
        iceCream(-10.15,3.55,5.75, .175, 'c');
        iceCream(-10.15,3.8,5.75, .175, 'v');
        iceCream(-10.65,3.55,5.75, .175, 'c');
        iceCream(-10.65,3.8,5.75, .175, 'm');
        iceCream(-11.15,3.55,5.75, .175, 'v');
        iceCream(-11.15,3.8,5.75, .175, 's');
        //hole
        hole(.8,.8, .375, -.785, .215, -7.75, 90);

        glColor3f(1,1,1);
        glWindowPos2i(10,5);
        Print("HOLE ONE:");

        break; 
      //hole 2
      case 1:
        skybox(0,0,0, 1,1,1, 0);
        golfBall(-8.3125,.25,6.25, .15);
        courseTwoHex(5,0,3, 2.5,2,2.5, 0);
        courseHexWall(5,0,3, 2.25,1.65,2.25, 60);
        courseHexWall(5,0,3, 2.25,1.65,2.25, 120);
        courseHexWall(5,0,3, 2.25,1.65,2.25, 180);
        courseHexWall(5,0,3, 2.25,1.65,2.25, 240);
        courseHexWall(5,0,3, 2.25,1.65,2.25, 300);
        //courseTwoCurve(-4,2.15,0, 3, 3, 90);
        courseTwo(-8,0,2, 1,1,1, 0);
        courseTwoWall(-8,0,2, 1,1,1, 0);
        //red club
        golfClubHead(-12.85,.25,4, .45,.175,.125, 0,0,0, 'r');
        golfClubGrip(-13,.5,4, .3,.25,.3, 0,0,0, 'r');
        golfClubGrip(-13,2.5,4, .3,1.15,.3, 0,0,0, 'r');
        golfClubPole(-13,1.25,4, .25,1.5,.25, 0,0,0);
        //blue club
        golfClubHead(-12.85,.25,4.5, .45,.175,.125, 0,0,0, 'b');
        golfClubGrip(-13,.5,4.5, .3,.25,.3, 0,0,0, 'b');
        golfClubGrip(-13,2.5,4.5, .3,1.15,.3, 0,0,0, 'b');
        golfClubPole(-13,1.25,4.5, .25,1.5,.25, 0,0,0);
        //yellow club
        golfClubHead(-12.85,.25,5, .45,.175,.125, 0,0,0, 'y');
        golfClubGrip(-13,.5,5, .3,.25,.3, 0,0,0, 'y');
        golfClubGrip(-13,2.5,5, .3,1.15,.3, 0,0,0, 'y');
        golfClubPole(-13,1.25,5, .25,1.5,.25, 0,0,0);
        //green club
        golfClubHead(-12.85,.25,5.5, .45,.175,.125, 0,0,0, 'g');
        golfClubGrip(-13,.5,5.5, .3,.25,.3, 0,0,0, 'g');
        golfClubGrip(-13,2.5,5.5, .3,1.15,.3, 0,0,0, 'g');
        golfClubPole(-13,1.25,5.5, .25,1.5,.25, 0,0,0);
        //pink club
        golfClubHead(-12.85,.25,6, .45,.175,.125, 0,0,0, 'p');
        golfClubGrip(-13,.5,6, .3,.25,.3, 0,0,0, 'p');
        golfClubGrip(-13,2.5,6, .3,1.15,.3, 0,0,0, 'p');
        golfClubPole(-13,1.25,6, .25,1.5,.25, 0,0,0);
        //club rack
        golfClubPole(-13.5,1.65,6.55, .35,3.15,.35, 0,0,0);
        golfClubPole(-13.5,1.65,3.55, .35,3.15,.35, 0,0,0);
        golfClubPole(-13.05,.15,6.55, .35,2,.35, 0,0,90);
        golfClubPole(-13.05,.15,3.55, .35,2,.35, 0,0,90);
        golfClubPole(-13.5,3.15,5.05, .35,3.175,.35, 0,90,90);
        golfClubPole(-13.5,2.25,5.05, .35,3.155,.35, 0,90,90);
        golfClubPole(-13.05,2.25,3.75, .35,1,.35, 0,0,90);
        golfClubPole(-13.05,2.25,4.25, .35,1,.35, 0,0,90);
        golfClubPole(-13.05,2.25,4.75, .35,1,.35, 0,0,90);
        golfClubPole(-13.05,2.25,5.25, .35,1,.35, 0,0,90);
        golfClubPole(-13.05,2.25,5.75, .35,1,.35, 0,0,90);
        golfClubPole(-13.05,2.25,6.25, .35,1,.35, 0,0,90);
        //benches
        bench(9,2.85,-15.75, 3,2,3, 350);
        bench(4,2.85,-15.75, 3,2,3, 10);
        //bench(-15,2.85,-7.75, 3,2,3, 30);
        //tree 1
        //trunk
        treeTrunk(12,1.35,-14, 1,2.5,1, 0,0,0);
        //first frow of branches
        treeTrunk(12,3.25,-14.75, .65,2,.65, 315,0,0);
        treeTrunk(11.25,3.25,-14, .65,2,.65, 0,0,45);
        treeTrunk(12.75,3.25,-14, .65,2,.65, 0,0,315);
        //second row of branches
        //left
        treeTrunk(10.6,3.35,-13.85, .45,1.15,.45, 27,0,75);
        //middle
        treeTrunk(12.4,3.65,-15.05, .45,1.15,.45, 350,0,300);
        treeTrunk(11.55,3.4,-14.8, .45,1.15,.45, 350,0,60);
        //right
        treeTrunk(13.25,3.25,-14.125, .45,1.15,.45, 350,0,280);
        //third row of branches
        //left
        treeTrunk(10.85,4.255,-14.125, .45,.75,.45, 350,0,345);
        treeTrunk(10.3,4.15,-13.85, .45,.75,.45, 27,0,40);
        //middle
        treeTrunk(12.2,4.255,-15.425, .45,.75,.45, 350,0,345);
        treeTrunk(11.8,4.15,-15.15, .45,.75,.45, 45,0,20);
        //right
        treeTrunk(13.6,4.15,-14.125, .45,.75,.45, 350,0,325);
        treeTrunk(13.05,4.15,-13.85, .45,.75,.45, 27,0,40);
        //leaves
        leaves(13.8,4.75,-14, .25,.25,.25, 20,0,0);
        leaves(13.8,4.75,-14.3, .25,.25,.25, 20,0,0);
        leaves(13.65,4.95,-14.15, .35,.35,.35, 20,0,30);
        leaves(13.65,4.95,-14.55, .35,.35,.35, 20,0,30);
        leaves(13.45,4.55,-13.8, .25,.25,.25, 50,0,330);
        leaves(13.1,4.9,-13.8, .375,.375,.375, 20,0,0);
        leaves(13.85,3.35,-14.1, .25,.25,.25, 20,0,0);
        leaves(12,5.25,-14.15, .5,.5,.5, 20,0,70);
        leaves(12.45,5.25,-14.45, .55,.55,.55, 20,0,70);
        leaves(12.45,5.25,-13.5, .4,.4,.4, 40,0,100);
        leaves(12.45,5.25,-15, .3,.3,.3, 40,0,20);
        leaves(12.45,4.75,-15.25, .3,.3,.3, 40,0,20);
        leaves(12.85,4.75,-15.25, .3,.3,.3, 40,0,20);
        leaves(12.85,4.15,-15.25, .3,.3,.3, 40,0,20);
        leaves(11.85,4.85,-15.25, .35,.35,.35, 40,0,20);
        leaves(11.65,4.85,-14.95, .35,.35,.35, 320,0,20);
        leaves(11.75,5.25,-13.5, .4,.4,.4, 340,0,50);
        leaves(11.05,5.25,-13.5, .4,.4,.4, 20,0,320);
        leaves(11.25,4.95,-13.5, .4,.4,.4, 20,0,320);
        leaves(10.75,5.25,-13.5, .4,.4,.4, 340,0,50);
        leaves(11.25,4.95,-14.5, .4,.4,.4, 20,0,320);
        leaves(10.25,4.75,-13.55, .3,.3,.3, 20,0,320);
        leaves(10.05,4.55,-13.55, .3,.3,.3, 320,0,20);
        leaves(10.05,3.55,-13.55, .25,.25,.25, 320,0,20);
        leaves(11.05,3.65,-14.95, .25,.25,.25, 320,0,20);
        leaves(14.2,4.75,-14.55, .3,.3,.3, 20,0,30);
        //tree 2
        //trunk
        treeTrunk(13.5,1.35,-11, 1,2.5,1, 0,0,0);
        //first frow of branches
        treeTrunk(13.5,3.25,-11.75, .65,2,.65, 315,0,0);
        treeTrunk(12.75,3.25,-11, .65,2,.65, 0,0,45);
        treeTrunk(14.25,3.25,-11, .65,2,.65, 0,0,315);
        //second row of branches
        //left
        treeTrunk(12.1,3.35,-10.85, .45,1.15,.45, 27,0,75);
        //middle
        treeTrunk(13.9,3.65,-12.05, .45,1.15,.45, 350,0,300);
        treeTrunk(13.05,3.4,-11.8, .45,1.15,.45, 350,0,60);
        //right
        treeTrunk(14.75,3.25,-11.125, .45,1.15,.45, 350,0,280);
        //third row of branches
        //left
        treeTrunk(12.35,4.255,-11.125, .45,.75,.45, 350,0,345);
        treeTrunk(11.8,4.15,-10.85, .45,.75,.45, 27,0,40);
        //middle
        treeTrunk(13.7,4.255,-12.425, .45,.75,.45, 350,0,345);
        treeTrunk(13.3,4.15,-12.15, .45,.75,.45, 45,0,20);
        //right
        treeTrunk(15.1,4.15,-11.125, .45,.75,.45, 350,0,325);
        treeTrunk(14.55,4.15,-10.85, .45,.75,.45, 27,0,40);
        //leaves
        leaves(15.3,4.75,-11, .25,.25,.25, 20,0,0);
        leaves(15.3,4.75,-11.3, .25,.25,.25, 20,0,0);
        leaves(15.15,4.95,-11.15, .35,.35,.35, 20,0,30);
        leaves(15.15,4.95,-11.55, .35,.35,.35, 20,0,30);
        leaves(14.95,4.55,-10.8, .25,.25,.25, 50,0,330);
        leaves(14.6,4.9,-10.8, .375,.375,.375, 20,0,0);
        leaves(15.35,3.35,-11.1, .25,.25,.25, 20,0,0);
        leaves(13.5,5.25,-11.15, .5,.5,.5, 20,0,70);
        leaves(13.95,5.25,-11.45, .55,.55,.55, 20,0,70);
        leaves(13.95,5.25,-10.5, .4,.4,.4, 40,0,100);
        leaves(13.95,5.25,-12, .3,.3,.3, 40,0,20);
        leaves(13.95,4.75,-12.25, .3,.3,.3, 40,0,20);
        leaves(14.35,4.75,-12.25, .3,.3,.3, 40,0,20);
        leaves(14.35,4.15,-12.25, .3,.3,.3, 40,0,20);
        leaves(13.35,4.85,-12.25, .35,.35,.35, 40,0,20);
        leaves(13.15,4.85,-11.95, .35,.35,.35, 320,0,20);
        leaves(13.25,5.25,-10.5, .4,.4,.4, 340,0,50);
        leaves(12.55,5.25,-10.5, .4,.4,.4, 20,0,320);
        leaves(12.75,4.95,-10.5, .4,.4,.4, 20,0,320);
        leaves(12.25,5.25,-10.5, .4,.4,.4, 340,0,50);
        leaves(12.75,4.95,-11.5, .4,.4,.4, 20,0,320);
        leaves(11.55,4.75,-10.55, .3,.3,.3, 20,0,320);
        leaves(11.55,4.55,-10.55, .3,.3,.3, 320,0,20);
        leaves(11.55,3.55,-10.55, .25,.25,.25, 320,0,20);
        leaves(12.55,3.65,-11.95, .25,.25,.25, 320,0,20);
        leaves(15.7,4.75,-11.55, .3,.3,.3, 20,0,30);
        //ferris wheel
        ferrisWheel(-10,6,-11.5, 4,4,.15, 0,0,0);
        golfClubPole(-10,6,-11.5, .15,1,.15, 0,90,90);
        golfClubPole(-8.9,2.95,-10.95, .25,6.15,.25, 0,0,20);
        golfClubPole(-11.15,2.95,-10.95, .25,6.15,.25, 0,0,340);
        golfClubPole(-10,6,-10.95, .75,.15,.75, 0,90,90);
        golfClubPole(-8.9,2.95,-12.05, .25,6.15,.25, 0,0,20);
        golfClubPole(-11.15,2.95,-12.05, .25,6.15,.25, 0,0,340);
        golfClubPole(-10,6,-12.05, .75,.15,.75, 0,90,90);
        golfClubPole(-10,.15,-11.5, .15,2.55,8.5, 0,90,90);
        //ring toss
        ringToss(12,0.1,0, 1,1,1, 0,0,0);
        //middle
        golfClubGrip(12,.9,-1.3, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(12,1.2,-2.05, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(12,1.5,-2.8, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(12,1.8,-3.55, .3,1.15,.3, 0,0,0, 'd');
        //left
        golfClubGrip(11.08,.9,-1.3, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(11.08,1.2,-2.05, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(11.08,1.5,-2.8, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(11.08,1.8,-3.55, .3,1.15,.3, 0,0,0, 'd');
        //right
        golfClubGrip(12.92,.9,-1.3, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(12.92,1.2,-2.05, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(12.92,1.5,-2.8, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(12.92,1.8,-3.55, .3,1.15,.3, 0,0,0, 'd');
        //most left
        golfClubGrip(10.16,.9,-1.3, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(10.16,1.2,-2.05, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(10.16,1.5,-2.8, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(10.16,1.8,-3.55, .3,1.15,.3, 0,0,0, 'd');
        //most right
        golfClubGrip(13.84,.9,-1.3, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(13.84,1.2,-2.05, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(13.84,1.5,-2.8, .3,1.15,.3, 0,0,0, 'd');
        golfClubGrip(13.84,1.8,-3.55, .3,1.15,.3, 0,0,0, 'd');
        //rings
        ring(12,0.25,1, 1,1,1, 90,0,0);
        ring(13,0.25,2, 1,1,1, 90,0,0);
        ring(14,0.25,1.5, 1,1,1, 90,0,0);
        ring(12.92,1.7,-2.8, 1,1,1, 270,0,0);
        ring(11.08,1.1,-1.3, 1,1,1, 270,0,0);
        //back
         for(i = -120; i<=120; i+=2){
           fence((double)i/7,.1,-18, .075,1.5,.075, 0);
         }
         // sides
         for(i = 0; i<=176; i+=2){
           fence(-17.325,.1,7.35-(double)i/7, .075,1.5,.075, 90);
           fence(17.325,.1,7.35-(double)i/7, .075,1.5,.075, 90);
         }

        flag(5.025,.95,6.555, 1,1,1, 0);
        flagPole(5.05,1.25,3.05, .1,2,.1, 0);
        hole(.8,.8, .2, 4.25, 0.3, 2.25, 90);

        glColor3f(1,1,1);
        glWindowPos2i(10,5);
        Print("HOLE TWO:");
        
        break;
   }

   glColor3f(1,1,1);
    //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(120,5);
   if (fps) {
    Print("FP Mode=ON  Angle=%d",angle);
   }
   else if (view) {
    //  Print the text string
    Print("Angle=%d,%d  Dim=%.1f  Projection=%s",th,ph,dim,"Orthogonal");
   }
   else {
    //  Print the text string
    Print("Angle=%d,%d  Dim=%.1f  FOV=%d  Projection=%s",th,ph,dim,fov,"Perspective");
   }

   // if (light)
   // {
   //    glWindowPos2i(5,45);
   //    Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
   //    glWindowPos2i(5,25);
   //    Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   // }

   ErrCheck("display");
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

// ex9
/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if (!fps) {
    //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
  }
  //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Update projection
   myProject();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

// ex8
/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    //  Exit on ESC
    if (ch == 27)
      exit(0);
    //  Switch display mode
    else if (ch == 'n' || ch == 'N')
      mode = (mode+1)%2;
    // else if (ch == 'b' || ch == 'B')
    //   mode = (mode+2)%2;
    //  Switch display view
    else if (ch == 'v' || ch == 'V')
      view = 1-view;
    else if (ch == 'f' || ch == 'F')
      fps = 1-fps;

    if (fps) {
      if (ch == 'w' || ch == 'W'){
         fx += cx*0.025; 
         fz += cz*0.025;
      }
      else if (ch == 'a' || ch == 'A'){
         angle -= 2;
      }
      else if (ch == 's' || ch == 'S'){
         fx -= cx*0.025;
         fz -= cz*0.025;
      }
      else if (ch == 'd' || ch == 'D'){
         angle += 2;
      }
      //reset view angle
      else if (ch == '0') {
        angle = 0;
        fz = 10;
      }

      //  Keep angles to +/-360 degrees
      angle %= 360;
    }

    else if (ch == '0') {
        th = 0;
        ph = 40;
        fov = 55;
        dim = 10;
        emission = 0;
        ambient = 30;
        diffuse = 100;
        specular = 0;
        shininess = 0;
        zh = 90;
        ylight = 7;
        move = 1;
        light = 1;
     }
     //  Change field of view angle
     else if (ch == '-' && ch>1)
        fov--;
     else if (ch == '+' && ch<179)
        fov++;
      //  Toggle lighting
     else if (ch == 'l' || ch == 'L')
      light = 1-light;
        //  Toggle light movement
     else if (ch == 'm' || ch == 'M')
        move = 1-move;
     //  Move light
     else if (ch == '<')
        zh += 1;
     else if (ch == '>')
        zh -= 1;
     //  Light elevation
     else if (ch=='[')
        ylight -= 0.1;
     else if (ch==']')
        ylight += 0.1;
     //  Ambient level
     else if (ch=='a' && ambient>0)
        ambient -= 5;
     else if (ch=='A' && ambient<100)
        ambient += 5;
     //  Diffuse level
     else if (ch=='d' && diffuse>0)
        diffuse -= 5;
     else if (ch=='D' && diffuse<100)
        diffuse += 5;
     //  Specular level
     else if (ch=='s' && specular>0)
        specular -= 5;
     else if (ch=='S' && specular<100)
        specular += 5;
     //  Emission level
     else if (ch=='e' && emission>0)
        emission -= 5;
     else if (ch=='E' && emission<100)
        emission += 5;
     //  Shininess level
     else if (ch=='h' && shininess>-1)
        shininess -= 1;
     else if (ch=='H' && shininess<7)
        shininess += 1;
     //  Translate shininess power to value (-1 => 0)
     shiny = shininess<0 ? 0 : pow(2.0,shininess);
     
   //  Reproject
   myProject();
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

// ex9
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   myProject();
}

void doFrame(int v) {
    frameNumber++;
    glutPostRedisplay();
    glutTimerFunc(30,doFrame,0);
}

//ex 7
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(1200,600);
   //  Create the window
   glutCreateWindow("Sarah Godine - Textures");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Load textures
   texture[0] = LoadTexBMP("puttingGreen.bmp");
   texture[1] = LoadTexBMP("golfBall.bmp");
   texture[2] = LoadTexBMP("metalPlate.bmp");
   texture[3] = LoadTexBMP("concrete.bmp");
   texture[4] = LoadTexBMP("redFlag.bmp");
   texture[5] = LoadTexBMP("sidewalk.bmp");
   texture[6] = LoadTexBMP("wood.bmp");
   texture[7] = LoadTexBMP("whiteWindmill.bmp");
   texture[8] = LoadTexBMP("windmillTop.bmp");
   texture[9] = LoadTexBMP("sky.bmp");
   texture[10] = LoadTexBMP("gravel.bmp");
   texture[11] = LoadTexBMP("treeTrunk.bmp");
   texture[12] = LoadTexBMP("metal.bmp");
   texture[13] = LoadTexBMP("rubber.bmp");
   texture[14] = LoadTexBMP("leaves.bmp");
   texture[15] = LoadTexBMP("bench.bmp");
   texture[16] = LoadTexBMP("tire.bmp");
   texture[17] = LoadTexBMP("iceCreamLogo.bmp");
   texture[18] = LoadTexBMP("bluePlastic.bmp");
   texture[19] = LoadTexBMP("cone.bmp");
   texture[20] = LoadTexBMP("glass.bmp");
   texture[21] = LoadTexBMP("iceCream.bmp");
   texture[22] = LoadTexBMP("circus.bmp");
   texture[23] = LoadTexBMP("ringToss.bmp");
   // timer function
   glutTimerFunc(200,doFrame,0); 
   // error check
   ErrCheck("init");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}