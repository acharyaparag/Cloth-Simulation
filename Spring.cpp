
#include "Camera.h"
#include "particle.h"
#include "strut.h"
#include <vector>
#include "stateVector.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#define RADIUS		1
#define P_I             3.14
#define RANDOM_RANGE(lo, hi) ((lo) + (hi - lo) * drand48())
#define WINDOW_WIDTH	940	/* window dimensions */
#define WINDOW_HEIGHT	740
#define width           14
#define height          10 

Camera *camera;
Vector3d Wind(0.03,0.0,0.01);

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;
static double TimeStep;
static double Time = 0;
static int TimerDelay;
static int TimeStepsPerDisplay;
static double DispTime;
static char *ParamFilename = NULL;
static double CoeffofRestitution;
static int NTimeSteps = -1;
int persp_win;

const int maxParticles = no_particles_width*no_particles_height;
StateVector SV;


void TimerCallback(int value);

using namespace std;

struct Ball
{
  Vector3d Velocity;
  Vector3d Position;
  Vector3d newVelocity;
  Vector3d newPosition;
} ;

Ball Sphere;

Particle *particles;
std::vector<Strut> struts;


float dotProduct(Vector3d vec1,Vector3d vec2)
{
  return(vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);  
}

Vector3d normalize(Vector3d vec)
{
  float mag = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
  vec.x = vec.x/mag;
  vec.y = vec.y/mag;
  vec.z = vec.z/mag;
  return(vec);  
}


float magnitude(Vector3d vec)
{
  return (sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z));
}

Particle* getParticle(int x, int y) 
{
return &particles[y*no_particles_width + x];
}

int getstateVector(int x,int y)
{
 return (y*no_particles_width + x);
}


Vector3d triangleNormalParticle(Particle *p1,Particle *p2,Particle *p3)
{
	Vector3d pos1 = p1->getPosition();
	Vector3d pos2 = p2->getPosition();
	Vector3d pos3 = p3->getPosition();

	Vector3d v1 = pos2-pos1;
	Vector3d v2 = pos3-pos1;

	return v1%v2;
}

Vector3d triangleNormalState(int s1, int s2, int s3)
{
	Vector3d pos1 = SV.getData(s1);
	Vector3d pos2 = SV.getData(s2);
	Vector3d pos3 = SV.getData(s3);

	Vector3d v1 = pos2-pos1;
	Vector3d v2 = pos3-pos1;

	return v1%v2;
}

void drawTriangle(int s1, int s2, int s3, const Vector3d color)
{
	glColor4f(color.x,color.y,color.z,1.0);
    
        glNormal3f(SV.getNormal(s1).x,SV.getNormal(s1).y,SV.getNormal(s1).z);
	glVertex3f(SV.getData(s1).x,SV.getData(s1).y,SV.getData(s1).z);

	glNormal3f(SV.getNormal(s2).x,SV.getNormal(s2).y,SV.getNormal(s2).z);
	glVertex3f(SV.getData(s2).x,SV.getData(s2).y,SV.getData(s2).z);

	glNormal3f(SV.getNormal(s3).x,SV.getNormal(s3).y,SV.getNormal(s3).z);
	glVertex3f(SV.getData(s3).x,SV.getData(s3).y,SV.getData(s3).z);
}
void init() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(width/2+1.4,-height/2,16), Vector3d(width/2, -height/2, 0), 
		      Vector3d(0, 1, 0));

glClearColor(0.0, 0.0, 0.0, 0.0);	
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void PerspDisplay() {
  int i;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WINDOW_WIDTH,WINDOW_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


glDisable(GL_LIGHTING);

for (int i = 0;i<maxParticles; i++)
   {
     SV.getNormal(i).set(0,0,0); 
   }

for(int x = 0; x<no_particles_width-1; x++)
{
    for(int y=0; y<no_particles_height-1; y++)
       {
	Vector3d normal = triangleNormalState(getstateVector(x+1,y),getstateVector(x,y),getstateVector(x,y+1));
	SV.getNormal(getstateVector(x+1,y)) = normal + SV.getNormal(getstateVector(x+1,y));
	SV.getNormal(getstateVector(x,y)) = normal + SV.getNormal(getstateVector(x,y));
	SV.getNormal(getstateVector(x,y+1)) = normal + SV.getNormal(getstateVector(x,y+1));
	SV.getNormal(getstateVector(x+1,y)).normalize(); 
	SV.getNormal(getstateVector(x,y)).normalize(); 
	SV.getNormal(getstateVector(x,y+1)).normalize(); 

	normal = triangleNormalState(getstateVector(x+1,y+1),getstateVector(x+1,y),getstateVector(x,y+1));
	SV.getNormal(getstateVector(x+1,y+1)) = normal + SV.getNormal(getstateVector(x+1,y+1));
	SV.getNormal(getstateVector(x+1,y)) = normal + SV.getNormal(getstateVector(x+1,y));
	SV.getNormal(getstateVector(x,y+1)) = normal + SV.getNormal(getstateVector(x,y+1));
	SV.getNormal(getstateVector(x+1,y+1)).normalize(); 
	SV.getNormal(getstateVector(x+1,y)).normalize();
	SV.getNormal(getstateVector(x,y+1)).normalize();
	}
}

glBegin(GL_TRIANGLES);
{	
  for(int x = 0; x<no_particles_width-1; x++)
     {
	for(int y=0; y<no_particles_height-1; y++)
	   {
             Vector3d color(0,0,0);
             if (x%2)
                 color = Vector3d(0.2,0.1,0.3);
             else
                 color = Vector3d(0.1,0.1,0.1);
           
           drawTriangle(getstateVector(x+1,y),getstateVector(x,y),getstateVector(x,y+1),color);
           drawTriangle(getstateVector(x+1,y+1),getstateVector(x+1,y),getstateVector(x,y+1),color);
               
           }
     }   
}
glEnd();

glEnable(GL_LIGHTING);
glPushMatrix();
glTranslatef(Sphere.Position.x,Sphere.Position.y,Sphere.Position.z);
glutSolidSphere(RADIUS,100,100);
glPopMatrix();

glutSwapBuffers();
}

void do_lights()
{
 float light0_ambient[] = {0.5,0.5,0.5,1.0};
 float light0_diffuse[] = {0.7,0.7,0.7,1.0};
 float light0_position[] = {1.5,2.0,2.0,1.0};
 float light0_direction[] = {-1.5,-2.0,-2.0,1.0};

 glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);
 glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
 glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
 glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
 glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);

}

void do_material()
{
 float mat_ambient[] = { 0.0,0.0,0.0,1.0};
 float mat_diffuse[] = { 0.54,0.16,0.16,1.0};
 float mat_specular[] = { 1.0,1.0,1.0,1.0};
 float mat_shininess[] = {2.0};
 glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
 glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
 glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
 glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


Vector3d externalForce(int i){

  Vector3d G(0,-0.01,0),acceleration;
  acceleration = G ;
  return acceleration;
}

void addWind(Particle *p1,Particle *p2,Particle *p3, const Vector3d dirWind)
{
   Vector3d normal = triangleNormalParticle(p1,p2,p3);
   Vector3d d = normal.normalize();
   Vector3d force = normal*(dotProduct(d,dirWind));
   p1->getForce() = p1->getForce() + force;
   p2->getForce() = p2->getForce() + force;
   p3->getForce() = p3->getForce() + force;
}


StateVector calcForce(StateVector inSV,float t)
{
 StateVector SVDot;

  for ( int i = 0 ; i<maxParticles; i++)
      {
       Vector3d force(0,0,0);
       particles[i].setForce(force);
      }

  std::vector<Strut>::iterator strut ;
  for (strut = struts.begin(); strut!=struts.end(); strut++)
     {
         float Kij = strut->getK();
         float Dij = strut->getD();
         float Lij0 = strut->getrestLength();
         Particle* P0 = strut->getParticle1();
         Particle* P1 = strut->getParticle2();
         Vector3d Xi = P0->getPosition();
         Vector3d Xj = P1->getPosition();
         Vector3d Vi = P0->getVelocity();
         Vector3d Vj = P1->getVelocity();
         Vector3d Xij = Xj - Xi;
         float Lij = Xij.norm();
         Vector3d Uij = Xij.normalize(); 
         Vector3d forceSpringij = Kij*(Lij - Lij0)*Uij;
         Vector3d forceDampij = Dij*dotProduct((Vj-Vi),Uij)*Uij;
         Vector3d forcei = P0->getForce() ;
         forcei = forcei +  forceSpringij + forceDampij;
         P0->setForce(forcei);
         Vector3d  forcej = P1->getForce() ;
         forcej = forcej - ( forceSpringij + forceDampij);
         P1->setForce(forcej);
       
     }

 for ( int i = 0 ; i<maxParticles; i++)
      {
        Vector3d force = particles[i].getForce();
        force = force + externalForce(i);
        particles[i].setForce(force);
      }

for(int x = 0; x<no_particles_width-1; x++)
  {
    for(int y=0; y<no_particles_height-1; y++)
	{
	addWind(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),Wind);
	addWind(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),Wind);
	}
  }

 for ( int i = 0 ; i<maxParticles; i++)
     {
       particles[i].getAccel() = particles[i].getForce()/particles[i].getMass();
      }

  for ( int i = 0 ; i<maxParticles; i++)
     {
       if(!particles[i].getFixed())
         {
          SVDot.getData(i) = inSV.getData(maxParticles + i);
          SVDot.getData(maxParticles + i)  = particles[i].getAccel();
         }
     }
  return SVDot;
}


void Simulate(){
 
  StateVector K1 = calcForce(SV,TimeStep)*TimeStep;
  StateVector K2 = calcForce(SV + K1*0.5,TimeStep)*TimeStep;
  StateVector K3 = calcForce(SV + K2*0.5,TimeStep)*TimeStep;
  StateVector K4 = calcForce(SV + K3,TimeStep)*TimeStep;
  SV = SV + (K1+K2*2+K3*2+K4)*0.1666;

  for ( int i = 0 ; i<maxParticles; i++)
      {
           particles[i].getVelocity() =  SV.getData(maxParticles+i);
           particles[i].getPosition() = SV.getData(i);
      }

  Sphere.newVelocity =  Sphere.Velocity + TimeStep * (0,0,-0.2);
  Sphere.newPosition =  Sphere.Position + TimeStep *  Sphere.Velocity;

  for ( int i = 0 ; i<maxParticles; i++)
      {
        if(!particles[i].getFixed())
          {
           float rad = 1.1;
           Vector3d vec = particles[i].getPosition() - Sphere.Position;
           float len = vec.norm();
           if(len<rad)
             {
              Vector3d newPos = vec.normalize()*(rad-len);
              particles[i].getPosition() = particles[i].getPosition() + newPos;
              SV.getData(i) = particles[i].getPosition();      
             }
     
           } 
      }


// advance the timestep and set the velocity and position to their new values
  Time += TimeStep;
  NTimeSteps++;

  Sphere.Velocity =  Sphere.newVelocity;
  Sphere.Position =  Sphere.newPosition;

  if(NTimeSteps % TimeStepsPerDisplay == 0)
   PerspDisplay();
 
   glutTimerFunc(TimerDelay, TimerCallback, 0);
}

/*
  Run a single time step in the simulation
*/
void TimerCallback(int){

  Simulate();
}

void LoadParameters(char *filename){
  
  FILE *paramfile;

  if((paramfile = fopen(filename, "r")) == NULL){
    fprintf(stderr, "error opening parameter file %s\n", filename);
    exit(1);
  }

  ParamFilename = filename;

  if(fscanf(paramfile, "%lf %lf %lf",
	    &CoeffofRestitution,
	    &TimeStep, &DispTime) != 3){ 
    fprintf(stderr, "error reading parameter file %s\n", filename);
    fclose(paramfile);
    exit(1);
  }

  TimeStepsPerDisplay = Max(1, int(DispTime / TimeStep + 0.5));
  TimerDelay = int(0.5 * TimeStep * 1000);
}

void InitParticles(int argc, char* argv[]){

  if(argc <= 1){
    fprintf(stderr, "Parameter file not specified\n");
    exit(1);
  }
  
  LoadParameters(argv[1]);
  Time = 0;
  NTimeSteps = -1;

particles = new Particle[maxParticles];

  // Grid of particles from (0,0,0) to (width,-height,0)
  for(int x=0; x<no_particles_width; x++)
     {
	for(int y=0; y<no_particles_height; y++)
	   {
             int index = y*no_particles_width+x;

	     Vector3d pos(width * (x/(float)no_particles_width),
		     	-height * (y/(float)no_particles_height),0);
             Vector3d Velocity(0,0,0);
             float Lifespan = 1.0f;
             int Age = 0;
             float Fade = (RANDOM_RANGE(0.0,0.01)) ; 
             float Mass = 1.0; 
             Vector3d force(0,0,0);
             Vector3d accel(0,0,0);
             particles[index] = Particle(Velocity,pos,force,accel,Mass,Lifespan,Fade);//Column x at y'th row  
             SV.getData(index) = pos; 
             SV.getData(maxParticles + index) = Velocity;
           }
     }
    particles[0].setFixed(true);
    particles[no_particles_width-1].setFixed(true);
    particles[1].setFixed(true);
    particles[no_particles_width-2].setFixed(true);

  for(int x=0; x<no_particles_width; x++)
    {
       for(int y=0; y<no_particles_height; y++)
	{
		if (x<no_particles_width-1) struts.push_back(Strut(getParticle(x,y),getParticle(x+1,y)));
		if (y<no_particles_height-1) struts.push_back(Strut(getParticle(x,y),getParticle(x,y+1)));
		if (x<no_particles_width-1 && y<no_particles_height-1)struts.push_back(Strut(getParticle(x,y),getParticle(x+1,y+1)));
		if (x<no_particles_width-1 && y<no_particles_height-1) struts.push_back(Strut(getParticle(x+1,y),getParticle(x,y+1)));
	}
     }


   for(int x=0; x<no_particles_width; x++)
	{
	for(int y=0; y<no_particles_height; y++)
	   {
	      if (x<no_particles_width-2) struts.push_back(Strut(getParticle(x,y),getParticle(x+2,y)));
	      if (y<no_particles_height-2) struts.push_back(Strut(getParticle(x,y),getParticle(x,y+2)));
	      if (x<no_particles_width-2 && y<no_particles_height-2) struts.push_back(Strut(getParticle(x,y),getParticle(x+2,y+2)));
	      if (x<no_particles_width-2 && y<no_particles_height-2) struts.push_back(Strut(getParticle(x+2,y),getParticle(x,y+2)));	
           }
	}
     

Sphere.Position.x = width/2;
Sphere.Position.y =-height/2 - 5;
Sphere.Position.z =-5;
Sphere.Velocity.x = 1;
Sphere.Velocity.y = 0;
Sphere.Velocity.z = 1;

  glutIdleFunc(Simulate);   
}

void handleKey(unsigned char key, int x, int y){
  
  switch(key){
    case 'q':		// 'quit the program'
    case 'Q':
    case 27:
        {
          exit(0);
        }
      
    default:		// not a valid key -- just ignore it
      return;
  }
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void doReshape(int w, int h){
  int vpw, vph;
  
  float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
  if(float(w) / float(h) > aspect){
    vph = h;
    vpw = int(aspect * h + 0.5);
  }
  else{
    vpw = w;
    vph = int(w / aspect + 0.5);
  }
  
  glViewport(0, 0, vpw, vph);
  WinWidth = w;
  WinHeight = h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char *argv[]) {

  // set up opengl window
  glutInit(&argc, argv);
  InitParticles(argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow("Camera Test");
  // initialize the camera and such
  init();
  do_lights();
  do_material();
  // set up opengl callback functions
  glutDisplayFunc(PerspDisplay);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(handleKey);

  glutMainLoop();
  return(0);
}

