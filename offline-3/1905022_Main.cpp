#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
#include "bitmap_image.hpp"
#include "1905022_Header.h"

#include<bits/stdc++.h>

using namespace std;



double angle = 1.0*pi/180.0;  //camera rotation angle

//variables
int imageWidth, imageHeight;
bitmap_image image;
int imageCount = 0;


int windowWidth = 500;
int windowHeight = 500;
double viewAngle = 80;


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
Camera camera(Point(100,100,100), Point(0,0,1), Point(0,0,1));



void capture()
{
    //init bitmap image and set color to black
    image = bitmap_image(imageWidth, imageHeight);
    for(int i=0;i<imageWidth;i++)
    {
        for(int j=0;j<imageHeight;j++)
        {
            image.set_pixel(i,j,0,0,0);
        }
    }


    //calculating l,r,u
    Point l,r,u;
    l=camera.getFront();
    r=camera.getRight();
    u=camera.getUp();


    //calculating top left point
    double planeDistance = (windowHeight/2)/tan(pi*viewAngle/360.0);
    Point topLeft=camera.pos+l*planeDistance-r*(windowWidth/2.0)+u*(windowHeight/2.0);
    double du = (windowWidth*1.0)/imageWidth;
    double dv = (windowHeight*1.0)/imageHeight;
  

    topLeft=topLeft+r*du*0.5-u*dv*0.5;

    int nearest;
    double t,tMin;

    for(int i=0;i<imageWidth;i++)
    {
        cout<< "i : "<< i<< endl;
        for(int j=0;j<imageHeight;j++)
        {
           
        //    cout<<"i: "<<i<<", j: "<<j<<endl;
            Point pixel = topLeft + r*i*du - u*j*dv;
            Point dir = pixel - camera.pos;

            Ray ray(camera.pos,dir);


           
            Point color;

            //to find the nearest object
            double tMin = INT_MAX;
            for(int k=0; k<objects.size();k++)
            {
                Point tempColor;
                //distance of the intersection point
                double t = objects[k]->intersect(ray,tempColor, recursionLevel);
                
                if(t<tMin && t>0)  //if t is negative then the object is behind the camera
                {
                    //update distance of nearest object
                    tMin = t;
                    color = tempColor;
                    if(color.x<0) color.x=0;
                    if(color.y<0) color.y=0;
                    if(color.z<0) color.z=0;
                    if(color.x>1) color.x=1;
                    if(color.y>1) color.y=1;
                    if(color.z>1) color.z=1;

                }
            }
            //set pixel color
            image.set_pixel(i,j,color.x*255,color.y*255,color.z*255);
        }
    }

    
	image.save_image("Output_1"+to_string(imageCount)+".bmp");
	imageCount++;
	cout<<"Saving Image "<<endl;		

}



void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {
    case '0':
        capture();
        break;
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
        // camera.pos = camera.pos - camera.up;
        break;
    
    default:
        break;
    }
}


void loadData()
{
    ifstream file("scene.txt");
    file>>recursionLevel;
    file>>imageHeight;
    imageWidth = imageHeight;

    int totalObjects;
    file>>totalObjects;

    for(int i=0;i<totalObjects;i++)
    {
        string objectType;
        file>>objectType;

        if(objectType=="sphere")
        {
            Point center;
            double radius;
            double color[3];
            double coEfficients[4];
            double shine;

            file>>center.x>>center.y>>center.z;
            file>>radius;
            file>>color[0]>>color[1]>>color[2];
            file>>coEfficients[0]>>coEfficients[1]>>coEfficients[2]>>coEfficients[3];
            file>>shine;

            Sphere *sphere = new Sphere(center, radius, color, coEfficients, shine);
            objects.push_back(sphere);
        }
        else if(objectType=="triangle")
        {
            Point a,b,c;
            double color[3];
            double coEfficients[4];
            double shine;

            file>>a.x>>a.y>>a.z;
            file>>b.x>>b.y>>b.z;
            file>>c.x>>c.y>>c.z;
            file>>color[0]>>color[1]>>color[2];
            file>>coEfficients[0]>>coEfficients[1]>>coEfficients[2]>>coEfficients[3];
            file>>shine;
            Triangle *triangle = new Triangle(a,b,c, color, coEfficients, shine);
           objects.push_back(triangle);
        }
        else if(objectType=="general")
        {
            double coEfficients[4];
            double shine;
            double color[3];
            double a,b,c,d,e,f,g,h,i,j;
            Point pos;
            double height, width, length;
            file>>a>>b>>c>>d>>e>>f>>g>>h>>i>>j;
            file>>pos.x>>pos.y>>pos.z;
            file>>length>>width>>height;
            file>>color[0]>>color[1]>>color[2];
            file>>coEfficients[0]>>coEfficients[1]>>coEfficients[2]>>coEfficients[3];
            file>>shine;
            General *general = new General(a,b,c,d,e,f,g,h,i,j, pos, height, width, length, color, coEfficients, shine);
            objects.push_back(general);
        }
        else
        {
            cout<<"Invalid object type"<<endl;
        }
       
    }


    int totalLights;
    file>>totalLights;
    for(int i=0;i<totalLights;i++)
    {
        Point lightPos;
        Point color;
        file>>lightPos.x>>lightPos.y>>lightPos.z;
        file>>color.x>>color.y>>color.z;
        PointLight *light = new PointLight(lightPos, color);
        pointLights.push_back(light);
    }

    int totalSpotLights;
    file>>totalSpotLights;

    for(int i=0;i<totalSpotLights;i++)
    {
        Point lightPos;
        Point color;
        Point direction;
        double angle;
        file>>lightPos.x>>lightPos.y>>lightPos.z;
        file>>color.x>>color.y>>color.z;
        file>>direction.x>>direction.y>>direction.z;
        file>>angle;
        SpotLight *light = new SpotLight(lightPos, color, direction, angle);
        spotLights.push_back(light);
    }

   
  
    Floor *floor = new Floor(1000, 20);
    // floor->setColor(1,1,1);
    floor->setCoEfficients(0.4,0.2,0.2,0.4);
    floor->setShine(5);
    objects.push_back(floor);
 
}
void drawAll()
{
    for(int i=0;i<objects.size();i++)
        objects[i]->draw();
    for(int i=0;i<pointLights.size();i++)
        pointLights[i]->draw();
    for(int i=0;i<spotLights.size();i++)
        spotLights[i]->draw();
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

    camera.reposition();


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

    drawAll();

	


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void idle(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}
void init(){
	//codes for initialization
	

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
	gluPerspective(80,	1,	1,	800.0);
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
    //loading data from input file
    loadData();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(idle);		//what you want to do in the idle time (when no drawing is occuring)


	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	// glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

    objects.clear();
    pointLights.clear();
    spotLights.clear();

    objects.shrink_to_fit();
    pointLights.shrink_to_fit();
    spotLights.shrink_to_fit();

	return 0;
}
