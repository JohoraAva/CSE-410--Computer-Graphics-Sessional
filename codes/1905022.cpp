#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <time.h>
#include <GL/glut.h>
using namespace std;

#define PI (2*acos(0.0))

double radius=60.0;
double angle=1.0*PI/180;
double amplitude =60.0;
double curveAngle=1.0*PI/180;
double xVal=amplitude*cos(curveAngle);
double yVal=amplitude*sin(curveAngle);
double maxLen=radius;
double len=10;




void drawCircle()
{
    int r=radius;
    int theta;
   for ( theta=0;theta<=360;theta+=1)
        {
            if(r%5==0)
            {
                glColor3f(0,0,1);
                glBegin(GL_LINES);
                {
                    glVertex3f( r*cos(theta*PI/180) ,r*sin(theta*PI/180),0);
                    glVertex3f( r*cos((theta+1)*PI/180) ,r*sin((theta+1)*PI/180),0);  
                }
                glEnd();
            }
            
        }

}


void drawLine()
{
    // for(double theta=0;theta<=360;theta+=20)
    //     {
           glPushMatrix();{
            glColor3f(1,0,0);
            glRotatef(angle,0,0,1);
                glBegin(GL_LINES);
                {
                    glVertex3f( 0,0,0);
                    glVertex3f(radius,0,0);   
                }
                glEnd();
           }glPopMatrix();
        // }

	 	// glPushMatrix();{
        //     glColor3f(1,1,0);
        //     glRotatef(angle,0,0,1);
        //     drawCircle();
		// 	glTranslatef(radius*cos(angle),radius*sin(angle),0);
        //    }glPopMatrix();

}
void drawLine2()
{
	len=maxLen-radius*cos(angle);

	glPushMatrix();{
            glTranslatef(radius*cos(angle),radius*sin(angle),0);
                glBegin(GL_LINES);
                {
                    glVertex3f( 0,0,0);
                    glVertex3f(len,0,0);   
                }
                glEnd();
    }glPopMatrix();
}
void drawSinCurve()
{
	double theta=curveAngle;
	  glPushMatrix();
            {
                glColor3f(1,1,1);
				glTranslatef(radius+20,0,0);
                glBegin(GL_LINES);
                {
                    glVertex3f( xVal,yVal,0);
                    glVertex3f( amplitude*cos((theta+0.1)*PI/180) ,amplitude*sin((theta+0.1)*PI/180),0);  
                }
                glEnd();
            }
			glPopMatrix();
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	
    drawCircle();
    drawLine();
	drawSinCurve();
	drawLine2();




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.6;
	curveAngle+=0.6;
	xVal=amplitude*cos(curveAngle);
	yVal=amplitude*sin(curveAngle);
	// len=maxlen-radius*cos(angle);
	
	

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){

	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)


	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
