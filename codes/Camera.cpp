#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
// #include "Point.cpp"
// #include "Triangle.cpp"
#include "temp2.cpp"

#include<bits/stdc++.h>

using namespace std;


#define pi (2*acos(0.0))
double angle = 1.0*pi/180.0;

class Camera
{
public:
    Point pos, look, up;
    // Point right,front,left;

    // double fovY, aspectRatio, nearDist, farDist;
    Camera()
    {
        this->pos = Point(0,0,0);
        this->look = Point(0,0,-1);
        this->up = Point(0,1,0);
        // this->front=(look-pos).normalize();
        // left=up*front;
        // this->right=front*left;
    }
    Camera(Point pos, Point look, Point up)
    {
        this->pos = pos;
        this->look = look;
        this->up = up;
        // this->front=Point(1,0,0);
        // this->right=Point(0,0,1);
    }
    Camera(const Camera &c)
    {
        this->pos = c.pos;
        this->look = c.look;
        this->up = c.up;
        // this->front=c.front;
        // this->right=c.right;
    }
    
    void reposition(){
        cout<< "ok\n";
        pos.print();
        look.print();
        up.print();
        gluLookAt(pos.x,pos.y,pos.z,look.x,look.y,look.z,up.x,up.y,up.z);
    }

    Point getFront()
    {
        //front unit vector
        Point front = (look-pos).normalize();
        return front;
    }
    Point getUp()
    {
        return up.normalize();
    }
    Point getLeft()
    {
        //left unit vector
        Point left = getUp()*getFront();
        left=left.normalize();
        return left;
    }
    Point getRight()
    {
        //right unit vector
        Point u=getFront();
        Point right=u*getUp();
        return right.normalize();
    }
    void getNewPosition(Point front,Point right,Point up)
    {
        //distant 
        double d= (look-pos).magnitude();
        this->look=pos+front*d;
        this->pos=pos;
        this->up=up;
    }
    void moveForward()
    {
        Point dir;
        dir = (this->look - this->pos).normalize();
        this->pos = this->pos + dir;
        this->look = this->look + dir;
    }

    void moveBackward()
    {
        Point dir;
        dir = (this->pos - this->look).normalize();
        this->pos = this->pos + dir;
        this->look = this->look + dir;
    }
    void moveRight()
    {
        Point dir;
        dir = this->getLeft();
        this->pos = this->pos + dir;
        this->look = this->look + dir;
    }

    void moveLeft()
    {
        Point dir;
        dir = this->getRight();
        this->pos = this->pos + dir;
        this->look = this->look + dir;
    }

    void moveUp()
    {
        Point dir;
        dir = this->up.normalize();
        this->pos = this->pos + dir;
        this->look = this->look + dir;
    }
    void moveDown()
    {
        Point dir;
        dir = (this->up*-1).normalize();
        this->pos = this->pos + dir;
        this->look = this->look + dir;
    }
    void lookLeft()
    {
        //radian angle
        Point front = getFront()*cos(angle)- getRight()*sin(angle);
        Point right = getRight()*cos(angle)+ getFront()*sin(angle);

        getNewPosition(front,right,up);
        

    }

    void lookRight()
    {
        //radian angle
        Point front = getFront()*cos(angle)+ getRight()*sin(angle);
        Point right = getRight()*cos(angle)- getFront()*sin(angle);

        getNewPosition(front,right,up);
    }

    void lookUp()
    {
        //radian angle
        Point front = getFront()*cos(angle)+ getUp()*sin(angle);
        Point up = getUp()*cos(angle)- getFront()*sin(angle);

        getNewPosition(front,getRight(),up);
    }

    void lookDown()
    {
        //radian angle
        Point front = getFront()*cos(angle)- getUp()*sin(angle);
        Point up = getUp()*cos(angle)+ getFront()*sin(angle);

        getNewPosition(front,getRight(),up);
    }

    void tiltAntiClockwise()
    {
        //radian angle
        Point up = getUp()*cos(angle)+ getRight()*sin(angle);
        Point right = getRight()*cos(angle)- getUp()*sin(angle);

        getNewPosition(getFront(),right,up);
    }

    void tiltClockwise()
    {
        //radian angle
        Point up = getUp()*cos(angle)- getRight()*sin(angle);
        Point right = getRight()*cos(angle)+ getUp()*sin(angle);

        getNewPosition(getFront(),right,up);
    }
};

Camera camera(Point(100,100,100), Point(0,0,1), Point(0,0,1));
int drawgrid;   
int drawaxes;


void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {
    case '1':
        //look left
        camera.lookLeft();
        break;
    case '2':
        //look right
        camera.lookRight();
        break;
    case '3':
        //look up
        camera.lookUp();
        break;
    case '4':
        //look down
        camera.lookDown();
        break;
    case '5':
        //tilt counter-clockwise
        camera.tiltAntiClockwise();
        break;
    case '6':
        //tilt clockwise
        camera.tiltClockwise();
        break;
    case '0':
        //reset camera
        camera = Camera(Point(0,0,0), Point(0,0,-1), Point(0,1,0));
        break;
    case ',':
        
        //oct to sphere
        if(r>0.001)
            r-=0.05;
        break;
    case '.':
        //sphere to oct
        if(r<1.0)
            r+=0.05;
        break;
    case 'a':
        cubeAngle+=2;
        break;
    case 'd':
        cubeAngle-=2;
        break;
    default:
        break;
    }
    camera.reposition();
}

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        //move forward
        camera.moveForward();
        break;
    case GLUT_KEY_DOWN:
        camera.moveBackward();
        break;
    case GLUT_KEY_RIGHT:
        camera.moveLeft();
        break;
    case GLUT_KEY_LEFT:
        camera.moveRight();
        break;
    case GLUT_KEY_PAGE_UP:
        camera.moveUp();
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.moveDown();
        // camera.pos = camera.pos - camera.up;
        break;
    
    default:
        break;
    }
    camera.reposition();
}


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
        // glColor3f(0,0,0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
    }
}

void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();

}
void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
                
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,0.7,0.5,0);	//color black
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

	// gluLookAt(100,100,100,	0,0,0,	0,0,1);
	// gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	// gluLookAt(0,-200,200,	0,0,0,	0,0.5,0.5);
    camera.reposition();


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
    glPushMatrix();
    {
        glRotatef(cubeAngle,0,0,1);
        drawAll();
    }
   
    glPopMatrix();
   
    
    
    
   



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void idle(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}
void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	// angle=

	//clear the screen
	glClearColor(0,0,0,0);
     

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
    // drawTriangles();

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
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(idle);		//what you want to do in the idle time (when no drawing is occuring)


	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	// glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
