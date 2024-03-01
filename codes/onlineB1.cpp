#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>



#include<bits/stdc++.h>
#define PI (2*acos(0.0))

using namespace std;


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


class Matrix
{
    public:
    double** mat;
    int row,col;
    Matrix(int m,int n)
    {
        this->row=m;
        this->col=n;
        mat=new double*[m];
        for(int i=0;i<m;i++)
        {
            mat[i]=new double[n];
        }
    }
    void valueAssign(Point a ,Point b ,Point c) //00 10 20 01 11 21 02 12 22
    {
        mat[0][0]=a.x;
        mat[1][0]=a.y;
        mat[2][0]=a.z;
        mat[0][1]=b.x;
        mat[1][1]=b.y;
        mat[2][1]=b.z;
        mat[0][2]=c.x;
        mat[1][2]=c.y;
        mat[2][2]=c.z;
    }
    Matrix transpose()
    {
        Matrix res(this->row,this->col);
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                res.mat[i][j]=this->mat[j][i];
            }
        }
        return res;
    }

    Matrix operator*(Matrix m)
    {
        Matrix res(this->row,m.col);
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                res.mat[i][j]=0;
                for(int k=0;k<this->col;k++)
                {
                    res.mat[i][j]+=this->mat[i][k]*m.mat[k][j];
                }
            }
        }
        return res;
    }

 
    double determinant()
    {
        double res=0;
        if(this->row==1)
        {
            return this->mat[0][0];
        }
        else if(this->row==2)
        {
            return this->mat[0][0]*this->mat[1][1]-this->mat[0][1]*this->mat[1][0];
        }
        else
        {
            for(int i=0;i<this->row;i++)
            {
                Matrix temp(this->row-1,this->col-1);
                for(int j=1;j<this->row;j++)
                {
                    int k=0;
                    for(int l=0;l<this->col;l++)
                    {
                        if(l==i)
                        {
                            continue;
                        }
                        temp.mat[j-1][k]=this->mat[j][l];
                        k++;
                    }
                }
                res+=pow(-1,i)*this->mat[0][i]*temp.determinant();
            }
        }
        return res;
    }

    double cofactor(int r,int c)
    {
        Matrix temp(this->row-1,this->col-1);
        int tempRow=0,tempCol=0;
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                if(i!=r && j!=c)
                {
                    temp.mat[tempRow][tempCol++]=this->mat[i][j];
                    if(tempCol==this->row-1)
                    {
                        tempCol=0;
                        tempRow++;
                    }
                }
            }
        }
        return pow(-1,r+c)*temp.determinant();
    }

    Matrix adjoint()
    {
        Matrix res(this->row,this->col);
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->row;j++)
            {
                res.mat[i][j]=this->cofactor(i,j);
            }
        }
        return res.transpose();
    }

    Matrix inverse()
    {
        Matrix res(this->row,this->col);
        double det=this->determinant();
        if(det==0)
        {
            cout<<"Inverse does not exist\n";
            return res;
        }
        res=this->adjoint();
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->row;j++)
            {
                res.mat[i][j]/=det;
            }
        }
        return res;
    }
    void print()
    {
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                cout<<this->mat[i][j]<<" ";
            }
            cout<<"\n";
        }
    }

   Matrix pointToMatrix(Point p)
   {    
     
       Matrix res(3,1);
       res.mat[0][0]=p.x;
       res.mat[1][0]=p.y;
       res.mat[2][0]=p.z;   
    //    cout<<"Point to Matrix\n";
    //    res.print();    
       return res;
   }

    Matrix operator*(Point p)
    {
        Matrix mat2=pointToMatrix(p);
        Matrix res=*this*mat2;
        return res;
    }

};


Point rotate(Point p,Point n,double angle)//n vector, p point
{
    Point a,b;
    n=n.normalize();
    b=n*p;
    b=b.normalize();
    a=b*n;
    a=a.normalize();

    //construct T
    Matrix T(3,3);
    //value assign
    T.valueAssign(a,b,n);
    T=T.transpose();

    //construct R
    Matrix R(3,3);
    //value assign
    R.mat[0][0]=cos(angle);
    R.mat[0][1]=-sin(angle);
    R.mat[1][0]=sin(angle);
    R.mat[1][1]=cos(angle);
    R.mat[2][2]=1;

    //construct T inverse
    Matrix Tinv=T.inverse();
    Matrix ans=T*p;
    ans=R*ans;
    ans=Tinv*ans;

    Point res(ans.mat[0][0],ans.mat[1][0],ans.mat[2][0]);
    return res;

}


double angle = 1.0*PI/180.0;

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
int drawgrid;   
int drawaxes;
int drawwall;
int mode;
//task 3 vars
double r=1.0; //ratio to maintain octahedron to spehre
double maxLen=20.0; //max length of triangle
double curLen;
double maxRadius=maxLen/sqrt(3.0); //max radius of sphere
double curRadius;
double cubeAngle=0.0;




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



// double cubeAngle=0.0; //angle of rotation of cube



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
        glPushMatrix();{
            glRotatef(90*i,0,0,1);
            drawCylinder(r);
        }glPopMatrix();
    }

    glPushMatrix();{
        glRotatef(90,0,1,0);
        for(int i=0; i<4; i++){
            glPushMatrix();{
                glRotatef(90*i,0,0,1);
                drawCylinder(r);
            }glPopMatrix();
        }
    }glPopMatrix();


    glPushMatrix();{
        glRotatef(90,1,0,0);
        for(int i=0; i<4; i++){
            glPushMatrix();{
                glRotatef(90*i,0,0,1);
                drawCylinder(r);
            }glPopMatrix();
        }
    }glPopMatrix();
   
}



void drawAll(double r)
{

    // glPushMatrix();
    // {
    //     glTranslatef(10*cos(cubeAngle*PI/180),10*sin(cubeAngle*PI/180),10);
        glPushMatrix();
        {
            glTranslatef(0,0,curLen);
            drawTriangles(r);
            drawSpheres(r);
            drawCylinders(r);
        }
        glPopMatrix();
    // }
    // glPopMatrix();

}
void drawGrid()
{
    int cnt=100;
	for(int r=1;r<cnt;r++)
    {
        for(double theta=0;theta<=360;theta+=1)
        {
            if(r%5==0)
            {
                glColor3f(1,0,1);
                glBegin(GL_LINES);
                {
                    glVertex3f( r*cos(theta*PI/180) ,r*sin(theta*PI/180),0);
                    glVertex3f( r*cos((theta+1)*PI/180) ,r*sin((theta+1)*PI/180),0);  
                }
                glEnd();
            }
            
        }
    }

    for(double theta=0;theta<=360;theta+=20)
        {
           glPushMatrix();{
            glRotatef(theta,0,0,1);
                glBegin(GL_LINES);
                {
                    glVertex3f( 0,0,0);
                    glVertex3f(100,0,0);   
                }
                glEnd();
           }glPopMatrix();
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
       drawAll(r);
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
    drawwall=0;
    mode=0;
    double width = 100.0;
    // ball.wallinit(width);

	

	glClearColor(0,0,0,0);

	
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(70, 1, 1, 1000);

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
    // glutTimerFunc(0, timer, 0);
   


	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
