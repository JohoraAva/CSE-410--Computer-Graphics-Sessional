#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
#define __push glPushMatrix();
#define __pop glPopMatrix();

#include<bits/stdc++.h>
#define pi (2*acos(0.0))


class Point 
{
public:
    double x,y,z;
    Point() //initializes to origin
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }

    //operator overloading
    Point operator+(const Point &p)
    {
        Point ret;
        ret.x = this->x + p.x;
        ret.y = this->y + p.y;
        ret.z = this->z + p.z;
        return ret;
    }
    Point operator-(const Point &p)
    {
        Point ret;
        ret.x = this->x - p.x;
        ret.y = this->y - p.y;
        ret.z = this->z - p.z;
        return ret;
    }
    Point operator*(const double &d)
    {
        Point ret;
        ret.x = this->x * d;
        ret.y = this->y * d;
        ret.z = this->z * d;
        return ret;
    }
    Point operator/(const double &d)
    {
        Point ret;
        ret.x = this->x / d;
        ret.y = this->y / d;
        ret.z = this->z / d;
        return ret;
    }
    Point operator=(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        return *this;
    }
    Point operator*(const Point &p)
    {
        Point ret;
        ret.x = this->y * p.z - this->z * p.y;
        ret.y = this->z * p.x - this->x * p.z;
        ret.z = this->x * p.y - this->y * p.x;
        return ret;
    }
    double dot(const Point &p)
    {
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }
    Point normalize()
    {
        double d = sqrt(x*x + y*y + z*z);
        Point ret;
        ret.x = x/d;
        ret.y = y/d;
        ret.z = z/d;
        return ret;
    }
    double magnitude()
    {
        return sqrt(x*x + y*y + z*z);
    }
    void print()
    {
        printf("Point: %lf %lf %lf\n", x, y, z);
    }
};

double angle = 1.0*pi/180.0;

class Camera
{
public:
    Point pos, look, up;
    Camera()
    {
        this->pos = Point(0,0,0);
        this->look = Point(0,0,-1);
        this->up = Point(0,1,0);
    }
    Camera(Point pos, Point look, Point up)
    {
        this->pos = pos;
        this->look = look;
        this->up = up;
    }
    Camera(const Camera &c)
    {
        this->pos = c.pos;
        this->look = c.look;
        this->up = c.up;
    }
    
    void reposition()
    {
        // cout<< "ok\n";
        // pos.print();
        // look.print();
        // up.print();
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


//camera vars
Camera camera(Point(150,150,150), Point(0,0,0), Point(0,0,1));
int drawaxes;


//task 3 vars
double r=1.0; //ratio to maintain octahedron to spehre
double maxLen=60.0; //max length of triangle
double curLen;
double maxRadius=maxLen/sqrt(3.0); //max radius of sphere
double curRadius;
double cubeAngle=0.0; //angle of rotation of cube



void drawTriangle(int cnt)
{
   

    if(cnt%2)
        glColor3f(197.0/255, 1, 0.9);
    else
        glColor3f(0.6,0.8,0.8);
    
    
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(curLen,0,0);
        glVertex3f(0,curLen,0);
        glVertex3f(0,0,curLen);
    }
    glEnd();
}

void drawTriangles(double r)
{
    // set current length of triangle
    curLen=r*maxLen;
    int isNeg=-1; // to maintain sign of rotation

    for(int i=0;i<4;i++)
    {
        glPushMatrix();
        {
            glRotatef(90*i,1,0,0);
            double m=maxLen*(1-r)/3.0; //median of triangle

            glTranslatef(m,m,m);
            drawTriangle(i+1);
        }
        glPopMatrix();
    }


     for(int i=0;i<4;i++)
    {
        glPushMatrix();
        {
            glRotatef(90*i,1,0,0);
            glRotatef(180,0,1,1);
            double m=maxLen*(1-r)/3.0; //median of triangle

            glTranslatef(m,m,m);
            drawTriangle(i+4);
        }
        glPopMatrix();
    }

    
    
}


void drawQuad(Point a, Point b, Point c, Point d)
{   

   
    //draw quads using points
   
     glBegin(GL_QUADS);
            {
                // glColor3f(1,1,0);  //yellowish
                
                glVertex3f(a.x,a.y,a.z);
                glVertex3f(b.x,b.y,b.z);
                glVertex3f(c.x,c.y,c.z);
                glVertex3f(d.x,d.y,d.z);
            }
            glEnd();
  

}

void drawSpheres(double r)
{
    curRadius=(1-r)*maxRadius;

    double curPos=maxLen*(1-r)/3.0; //current position of sphere...?
    for(int i=0;i<4;i++)
    {
        glPushMatrix();
        {
             glColor3f(1,1,0);
            glRotatef(90*i,1,0,0);
            glTranslatef(0,0,curLen);  
            drawQuad(Point(curPos,curPos,curPos),Point(curPos,-curPos,curPos),Point(-curPos,-curPos,curPos),Point(-curPos,curPos,curPos)); //centre=origin
        }
        glPopMatrix();
    }

   for(int i=0;i<2;i++)
   {
    glPushMatrix();
    {
        glColor3f(1,1,0);
        if(i%2)
            glRotatef(90,0,1,0);
        else
            glRotatef(-90,0,1,0);
        glTranslatef(0,0,curLen);
        drawQuad(Point(curPos,curPos,curPos),Point(curPos,-curPos,curPos),Point(-curPos,-curPos,curPos),Point(-curPos,curPos,curPos));
    }
    glPopMatrix();
   }

}

void drawCylinder(double r)
{
                glColor3f(1,0,1);
                Point c1(maxLen/3,maxLen/3,maxLen/3);
                Point c2(maxLen/3,maxLen/3,-maxLen/3);
                Point y1 = Point(0,maxLen,0)*r+c1*(1-r);
                Point y2 = Point(0,maxLen,0)*r+c2*(1-r);
                Point x1 = Point(maxLen,0,0)*r+c1*(1-r);
                Point x2 = Point(maxLen,0,0)*r+c2*(1-r);
                drawQuad(y1,x1,x2,y2);
}

void drawCylinders(double r)
{
    for(int i=0; i<4; i++){
        __push{
            glRotatef(90*i,0,0,1);
            drawCylinder(r);
        }__pop
    }

    __push{
        glRotatef(90,0,1,0);
        for(int i=0; i<4; i++){
            __push{
                glRotatef(90*i,0,0,1);
                drawCylinder(r);
            }__pop
        }
    }__pop


    __push{
        glRotatef(90,1,0,0);
        for(int i=0; i<4; i++){
            __push{
                glRotatef(90*i,0,0,1);
                drawCylinder(r);
            }__pop
        }
    }__pop
   
}



void drawAll(double r)
{

    drawTriangles(r);
    drawSpheres(r);
    drawCylinders(r);

}



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
     case 'w':
        camera.pos.z+=2;
        break;
    case 's':
        camera.pos.z-=2;
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
        cubeAngle-=2;
        break;
    case 'd':
        cubeAngle+=2;
        break;
    default:
        break;
    }
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
        break;
    
    default:
        break;
    }
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
void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,0.7,0.5,0);	//color set for background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    camera.reposition();

	glMatrixMode(GL_MODELVIEW);

	drawAxes();
    glPushMatrix();
    {
        glRotatef(cubeAngle,0,0,1);
        drawAll(r);

        // glRotatef(cubeAngle,0,0,1);
        // r=1-r;
        // glTranslatef(20,0,0);
        // drawAll();
    }
    
   
    glPopMatrix();


       glPushMatrix();
    {
        glRotatef(cubeAngle,0,1,0);
        // drawAll(r);

        glTranslatef(0,100,0);
        
        drawAll(1-r);
    }
    
   
    glPopMatrix();
   
	glutSwapBuffers();
}


void idle()
{
	glutPostRedisplay();
}
void init(){
	//codes for initialization
	drawaxes=1;

	//clear the screen
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80,	1,	1,	1000.0);
}
int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	

	glutCreateWindow("Magic Cube(1905022)");
	glEnable(GL_DEPTH_TEST);	

	glutDisplayFunc(display);	
	glutIdleFunc(idle);		


	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    init();

	glutMainLoop();		

	return 0;
}
