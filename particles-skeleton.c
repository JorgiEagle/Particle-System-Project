////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2012-13
//
// Authors: Arturs Bekasovs and Toby Howard
//
/////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "frames.h"

#define particle_count 5000

//boolean used to reset particles on spacebar press
int reset = 0;
//boolean used to enable rotate mode
int rotate = 0;
//spacemode
int space_mode=0;
int space = 0;
//initial velocity modifiyer
int velocity_bonus = 1;

GLfloat eye_angle = 0;
GLfloat eyex = 0;
GLfloat eyey = 100.0;
GLfloat eyez = 1000; //Eye point
int time_modifier = 0;

//menu items
#define WIND 1
#define GRAVITY 2
#define TIME 3
#define VELOCITY 4
#define SPACE 5
#define ALL 6
int menu;

//acceleration struct
typedef struct acceleration_vector {
  GLfloat windx;
  GLfloat windz;
  GLfloat gravity;
} Acceleration_vector;

Acceleration_vector acceleration;

//particle structure
typedef struct particle {
  GLfloat position[3];
  GLfloat velocity[3];
  GLuint time_to_live;
} Particle;

Particle particles[particle_count];


//source
typedef struct source_position {
  int xcoord;
  int ycoord;
  int zcoord;
} Source;

Source source_1;
Source source_2;

// Display list for coordinate axis
GLuint axisList;
int AXIS_SIZE= 200;
int axisEnabled= 1;
///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////
//calculates the vector components for the wind effect
void calculate_velocity() {
  //calculate_wind();
  for (int i=0; i<particle_count; i++) {
    if(sqrt(particles[i].velocity[0]*particles[i].velocity[0] + particles[i].velocity[1]*particles[i].velocity[1]+particles[i].velocity[2]*particles[i].velocity[2])<2) {
    particles[i].velocity[0] += acceleration.windx;
    particles[i].velocity[1] += acceleration.gravity;
    particles[i].velocity[2] += acceleration.windz;
  }
  }
}
//only moves the particle if it is above the ground
void update_position() {
  for (int i=0; i<particle_count; i++) {
    if(particles[i].position[1] > -100) {
      particles[i].position[0] += particles[i].velocity[0];
      particles[i].position[1] += particles[i].velocity[1];
      particles[i].position[2] += particles[i].velocity[2];
    }
  }
}

void calculate_lookpoint() {

}

void reset_particle(int particle_no) {
  if(particle_no<particle_count/2) {
    particles[particle_no].position[0] = myRandom()*100 +source_1.xcoord;
    particles[particle_no].position[1] = source_1.ycoord;
    particles[particle_no].position[2] = myRandom()*100 +source_1.zcoord;
    //reset time to live to current
    particles[particle_no].time_to_live = 200+ round(myRandom()*(300 + time_modifier));
    //reset velocity
    particles[particle_no].velocity[0] = (myRandom()*2 -1) * velocity_bonus;
    particles[particle_no].velocity[1] = (myRandom()*(-2) +space) * velocity_bonus;
    particles[particle_no].velocity[2] = (myRandom()*2 -1) * velocity_bonus;
  }
  else {
    //reset particle to source 2
    particles[particle_no].position[0] = myRandom()*100 +source_2.xcoord;
    particles[particle_no].position[1] = source_2.ycoord;
    particles[particle_no].position[2] = myRandom()*100 +source_2.zcoord;
    //reset time to live to current
    particles[particle_no].time_to_live = 200+ round(myRandom()*(300 + time_modifier));
    //reset velocity
    particles[particle_no].velocity[0] = (myRandom()*2 -1) * velocity_bonus;
    particles[particle_no].velocity[1] = (myRandom()*(-2)) +space * velocity_bonus;
    particles[particle_no].velocity[2] = (myRandom()*2 -1) * velocity_bonus;
  }
}

void draw_particles() {
  glDisable(GL_LIGHTING);
  glColor3f(1.0,1.0,1.0);
  for(int i=0; i<particle_count; i++) {
    //if have died or reset then reset particle
    if(particles[i].time_to_live<=0 || reset)
      reset_particle(i);
    glPushMatrix();
      glMatrixMode(GL_MODELVIEW);
      glTranslatef(particles[i].position[0],particles[i].position[1],particles[i].position[2]);
      glScalef(1, 1, 1);
      glutSolidCube(2);
    glPopMatrix();
    particles[i].time_to_live -= 1;
  }
  reset =0;
  glutPostRedisplay();
}

////////////////////////////////////////////////

void display()
{
  glLoadIdentity();
  if(rotate) {
    if(space_mode)
      eyey += 0.001;
    eyex = 1000*sin(eye_angle);
    eyez = 1000*cos(eye_angle);
    eye_angle += 0.001*M_PI;
  }
  gluLookAt(eyex, eyey, eyez,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  frameStart();
  glMatrixMode(GL_MODELVIEW);
  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);
  calculate_velocity();
  update_position();
  draw_particles();
  frameEnd(GLUT_BITMAP_HELVETICA_10, 1.0, 1.0, 1.0, 0.05, 0.95);
  glutSwapBuffers();
  // glutPostRedisplay();
}

///////////////////////////////////////////////

void keyboard(unsigned char key, int a, int b)
{
  switch(key){
    case 27:
      exit(0);
    break;
    case ' ':
      time_modifier = 0;
      reset = 1;
    break;
    case 'z':
      time_modifier+=50;
    break;
    case 'x':
      if(time_modifier > -500)
        time_modifier-=50;
    break;
    case 'a':
      acceleration.gravity -= 0.005;
    break;
    case 's':
      acceleration.gravity +=0.005;
    break;
    case 'd':
    if(velocity_bonus>1)
      velocity_bonus -= 1;
    break;
    case 'f':
      velocity_bonus += 1;
    break;
    case 'q':
      acceleration.windx -= 0.005;
    break;
    case 'w':
      acceleration.windx += 0.005;
    break;
    case 'e':
      acceleration.windz -= 0.005;
    break;
    case 'r':
      acceleration.windz += 0.005;
    break;
    case 'l':
      if(!rotate)
        rotate = 1;
      else {
        rotate = 0;
        eyex = 0;
        eyey = 100.0;
        eyez = 1000;
        eye_angle = 0;
      }
    break;
  }
  glutPostRedisplay();
}

void cursor_keys(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
    break;
  }
}
///////////////////////////////////////////////

void reshape(int width, int height)
{
  glClearColor(0.1, 0.1, 0.1, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////

void makeAxes() {
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
  glEndList();
}

// void drawGround() {
//   glDisable(GL_LIGHTING);
//   glDepthRange (0.1, 1.0);
//   glColor3f(1.0, 1.0, 1.0);
//   glBegin(GL_QUADS);
//   glVertex3f(-1000, 0, -1000);
//   glVertex3f(-1000, 0, 1000);
//   glVertex3f(1000, 0, 1000);
//   glVertex3f(-1000, 0, 1000);
//   glEnd();
//   glEnable(GL_LIGHTING);
// }

void initialize_world_effects() {
  //set global acceleration
  acceleration.windx = 0.0;
  acceleration.gravity = -0.02;
  acceleration.windz = 0.001;

  //set particle sources
  source_1.xcoord = -150;
  source_1.ycoord = 400;
  source_1.zcoord = 0;
  source_2.xcoord = 50;
  source_2.ycoord = 400;
  source_2.zcoord = 0;
}

/*initialize all particles on the xz plane at y = 500.
Initial positions are random in the {(-50,-50), (-50, 50), (50,50), (50,-50)} square
initial velocities are random downward
time to live is random from 200 frames to 500 frames */
void initialize_particles() {
  for(int i=0; i<particle_count; i++) {
      reset_particle(i);
      particles[i].time_to_live = + round(myRandom()*300);
  }
}
///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutCreateWindow("COMP37111 Particles");
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  //drawGround();
  makeAxes();
  initialize_world_effects();
  initialize_particles();

}
/////////////////////////////////////////////////

void processMenuEvents(int option) {
  switch(option) {
    case WIND:
      acceleration.windx = 0;
      acceleration.windz = 0;
    break;
    case GRAVITY:
      acceleration.gravity = -0.02;
    break;
    case TIME:
      time_modifier = 0;
    break;
    case VELOCITY:
      velocity_bonus = 1;
    case SPACE:
      acceleration.windx = 0;
      acceleration.gravity = 0;
      acceleration.windz = 0;
      eyey = 400.0;
      space = 1;
    break;
    case ALL:
    acceleration.windx = 0;
    acceleration.windz = 0;
    acceleration.gravity = -0.02;
    eyey = 100.0;
    space = 0;
    time_modifier = 0;
    velocity_bonus = 1;

    break;
  }
}

void createMenus() {
  menu = glutCreateMenu(processMenuEvents);

  glutAddMenuEntry("Reset Wind", WIND);
  glutAddMenuEntry("Reset Gravity", GRAVITY);
  glutAddMenuEntry("Reset Time to Live", TIME);
  glutAddMenuEntry("Reset Velocity", VELOCITY);
  glutAddMenuEntry("SPACE MODE", SPACE);
  glutAddMenuEntry("Reset All", ALL);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
  //glutDetachMenu(GLUT_RIGHT_BUTTON);
}
//////////////////////////////////////////////////


int main(int argc, char *argv[])
{
  //double f;
  srand(time(NULL));
  initGraphics(argc, argv);
  glEnable(GL_POINT_SMOOTH);
  glutKeyboardFunc(keyboard);
  createMenus();
  glutMainLoop();
  return 0;
}
