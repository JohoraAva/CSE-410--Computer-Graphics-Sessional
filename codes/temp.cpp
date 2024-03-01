#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
// #include "Point.cpp"
// #include "Triangle.cpp"
#include "Ball2.cpp"

#include<bits/stdc++.h>

using namespace std;


#define pi (2*acos(0.0))
double angle = 1.0*pi/180.0;


class Camera
{
public:
    Point pos, look, up;
   
    Camera()
    {
        this->pos = Point(0,0,0);
        this->look = Point(0,0,1);
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
    
    void reposition(){
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

Camera camera(Point(150,150,150), Point(0,0,0), Point(0,0,1));
int drawgrid;   
int drawaxes;
int drawwall;
int mode;
Ball ball;


void update(int value)
{   

    ball.reflect();
   for(int i=0;i<4;i++)
   {
        //for each wall check collision
        if(ball.checkCollision(ball.wall[i],ball.wall[(i+1)%4]) || value)
        {
            //if collision occurs, change velocity
            double dis=ball.distance(ball.wall[i],ball.wall[(i+1)%4]); //shortest dis
            double cosTheta=ball.dir.normalize().dot(ball.getNormal(ball.wall[i],ball.wall[(i+1)%4]).normalize());
            dis=(dis-ball.radius)*1.0;
            glutTimerFunc(dis*40,update,0);
           
        }
   }
   cout<< endl;
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
    case '0':
        //reset camera
        camera = Camera(Point(0,0,0), Point(0,0,-1), Point(0,1,0));
        break;
    case 'i':
        //move forward
        if(!ball.simulation)
            ball.moveForward();
        break;
    case 'k':
        if(!ball.simulation)
            ball.moveBackward();
        break;
    case 'j':
        ball.rotateAntiClockWise();
        if(ball.simulation) 
        glutTimerFunc(0,update,1);
        break;
    case 'l':
        ball.rotateClockWise();
        if(ball.simulation) 
        glutTimerFunc(0,update,1);
        break;
    case ' ':
        if(ball.simulation==0){
            ball.simulation=1;
            glutTimerFunc(0,update,1);
        }
        else
            ball.simulation=0;
       
        break;
    case 'w':
        camera.pos.z+=2;
        break;
    case 's':
        camera.pos.z-=2;
        break;
    case 'm':  //toggle mode
        if(mode==0)
            mode=1;
        else
            mode=0;
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

void drawSquare(Point a,Point b,Point c,Point d){
    
    glBegin(GL_QUADS);{
        glVertex3f( a.x,a.y,a.z);
        glVertex3f(b.x,b.y,b.z);
        glVertex3f(c.x,c.y,c.z);
        glVertex3f(d.x,d.y,d.z);
    }glEnd();

}

void drawWall()
{
    if(drawwall)
    {
        //from -100 to 100
        glColor3f(1,0,0);
        for(int i=0;i<4;i++)
        {
            glPushMatrix();
            {
                glRotatef(90*i,0,0,1);
                drawSquare( Point(100,100,0),Point(100,100,10),Point(100,-100,10),Point(100,-100,0));   
            }
            glPopMatrix();
        }       


    }
}
void drawGrid()
{
	int i,cnt;
    cnt=20;

	if(drawgrid==1)
	{
        for(int i=-cnt;i<=cnt;i++)
        {
            for(int j=-cnt;j<=cnt;j++)
            {
                glPushMatrix();
                {
                    glTranslatef(i*20,j*20,0);
                    if((i+j)%2==0)
                        glColor3f(0,0,0);
                    else
                        glColor3f(1,1,1);
                    drawSquare( Point(10,10,0),Point(10,-10,0),Point(-10,-10,0),Point(-10,10,0));
                }
                glPopMatrix();
            }
        }
	}
}

void display(){


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glMatrixMode(GL_MODELVIEW);

	
	glLoadIdentity();
    camera.reposition();


	glMatrixMode(GL_MODELVIEW);


	drawAxes();
	drawGrid();
    drawWall();
    
    glPushMatrix();
    {
        ball.draw();
    }
    glPopMatrix();
    
    
    


    // if(simulation)
    // {
    //     ball.moveForward();
    // }
   



	
	glutSwapBuffers();
}


void idle()
{
	glutPostRedisplay();
}
void init(){
	drawgrid=1;
	drawaxes=0;
    drawwall=1;
    mode=0;
    double width = 100.0;
    ball.wallinit(width);

	

	glClearColor(0,0,0,0);

	
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(70, 1, 1, 1000);

}
void timer(int)
{
    if(ball.simulation)
    {
        ball.moveForward();
    }
    glutTimerFunc(10, timer, 0);
}
int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	

	glutCreateWindow("Rolling Ball (1905022)");

	init();

	glEnable(GL_DEPTH_TEST);	

	glutDisplayFunc(display);	
	glutIdleFunc(idle);		
    glutTimerFunc(0, timer, 0);
   


	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
