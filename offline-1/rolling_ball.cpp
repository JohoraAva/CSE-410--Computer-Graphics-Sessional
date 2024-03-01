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


//ball class
class Ball{

public:
    Point centre,axis,dir,look;
    double radius;
    Point up;//fixed 
    int simulation;

    //walls
    Point wall[4];

    void wallinit(double width)
    {
        wall[0]=Point(width,width,0);
        wall[1]=Point(width,-width,0);
        wall[2]=Point(-width,-width,0);
        wall[3]=Point(-width,width,0);
    }
    Ball(){
        centre = Point(1,0,10);
        axis = Point(0,0,1).normalize();
        dir = Point(1,1,0).normalize();
        look = Point(1,0,0).normalize();
        radius = 10;
        simulation = 0;

        up=Point (0,0,1);

    }
  
    void drawSphereQuad(Point a, Point b, Point c, Point d,Point centre,int cnt)
    {   
        
        Point points[101][101] ;

        a=a-centre;
        b=b-centre;
        c=c-centre;
        d=d-centre;
       
        //assign value to points
        for(int i=0;i<=100;i++)
        {
            for(int j=0;j<=100;j++)
            {
                points[i][j]=((a*(100-i)+b*i)*(100-j)+(d*(100-i)+c*i)*j)*(1.0/100.0);
                points[i][j]=(points[i][j]).normalize()*radius;
                points[i][j]=points[i][j]+centre;
            }
        }

        //draw quads using points

        for(int i=0;i<100;i++)
        {
            for(int j=0;j<100;j++)
            {
                glBegin(GL_QUADS);
                {
                    if (cnt%2)
                        glColor3f(i*0.03,(100-i)*0.06,(j)*0.5);  //white
                    else
                         glColor3f((100-i)*0.009,(i)*0.08,(100-i)*0.04);  //blue
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                }

                glEnd();
            }
        }
    }


    void drawCylinder(Point a,Point b, Point c,Point d, Point c1, Point c2, int cnt) // c1=centre1 , c2=centre2
    {
        Point points[101][101];

        //assign value to points
        for(int i=0;i<=cnt;i++) //axis
        {
            for(int j=0;j<=cnt;j++) //arc 
            {
                Point p1=(a*(cnt-i)+c*i)*(1.0/(double)cnt); //a:c distance
                Point p2=(b*(cnt-i)+d*i)*(1.0/(double)cnt); //b:d distance
                Point c0=(c1*(cnt-i)+c2*i)*(1.0/(double)cnt); //c1:c2 distance
                double radius=(c0-p1).magnitude(); //distance from  origin to a
                points[i][j]=(((p1-c0)*((double)cnt-j)+(p2-c0)*j)*(1.0/(double)cnt)).normalize()*radius; 
                points[i][j]=points[i][j]+c0;

            }
        }

        //draw quads using points
        for(int i=0;i<cnt;i++)
        {
            for(int j=0;j<cnt;j++)
            {
                glBegin(GL_QUADS);
                {
                    // //  glColor3f(j*0.3,(i)*0.6,(100-i)*0.01);
                    // glColor3f(0,1,0);
                     
                    glColor3f((i)*(0.4/(double)cnt),i*(2/(double)cnt),(j)*(0.3/(double)cnt));
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                }
                glEnd();
            }
        }
        
    }

    
    Point rotate(Point p,Point n,double angle) //p=look , n=noraml
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

    void drawArrow(Point start,Point end,Point n) //n=normal
    {
        double r = 1.3; //radius
        Point middle = start*0.3 + end*0.7; 
        n = n.normalize();
        for(int i=0; i<4; i++){
            Point n1= rotate(n,end-start,PI/2);
            Point temStart1 = start + n*r;
            Point temStart2= start +n1*r;
            Point temEnd1 = end + n*0.01;
            Point temEnd2 = end + n1*0.01;
            drawCylinder(temStart1,temStart2,middle+n*r,middle+n1*r,start,middle,10);
            drawCylinder(temEnd1,temEnd2,middle+n*r*2,middle+n1*r*2,end,middle,10);
            n = n1;
        }
    }

    double distance(Point p,Point q) //from point to a line 
    {
        double a,b,c,d;
        a=q.y-p.y;
        b=p.x-q.x;
        c=-(a*p.x+b*p.y);

        double nominator=a*centre.x+b*centre.y+c;
        double denominator=sqrt(a*a+b*b);

        return nominator/denominator;
    }

    Point getNormal(Point p,Point q) //from point to a line 
    {
        double a,b,c,d;
        a=q.y-p.y;
        b=p.x-q.x;
        
        return Point(-b,a,0).normalize();

        
    }
    
    void draw()
    {

        // centre=centre.normalize();
        Point northPole=centre+axis*radius;
        Point southPole=centre-axis*radius;

        Point equators[8];
        equators[0]=look;
        for(int i=1;i<8;i++)
        {
            equators[i]=rotate(equators[i-1],axis,PI/4);
        }
        
        for(int i=0;i<8;i++)
        {
            equators[i]=equators[i]*radius+centre;
        }

        for(int i=0;i<8;i++)
        {
            drawSphereQuad(northPole,equators[i],equators[(i+1)%8],northPole,centre,i);
            drawSphereQuad(southPole,equators[i],equators[(i+1)%8],southPole,centre,i+1);
        }

        drawArrow(centre+dir*radius,centre+dir*(radius+40),up);
        drawArrow(centre+up*radius,centre+up*(radius+50),dir);


    }

    void reflect()
    {
         for(int i=0;i<4;i++)
        {
            Point p=wall[i];
            Point q=wall[(i+1)%4];

            double dis=distance(p,q); //from centre to wall containing p and q point

            if(dis<=radius+2)
            {   
               // while(distance(p,q)<=radius) centre = centre - centre.normalize();
                Point n(q.y-p.y,p.x-q.x,0); //normal vector of wall

                double temp=n.normalize().dot(dir.normalize());
                dir=dir-n.normalize()*temp*2;
            }
        }

    }

    void moveForward()
    {
        centre=centre+dir;
        Point left=up*dir;
        axis=rotate(axis,left,2.0/radius);
        look=rotate(look,left,2.0/radius);

        if(!simulation)
        {
            reflect();
        }
    }

    void moveBackward()
    {
        centre=centre-dir;
        Point left=up*dir;
        axis=rotate(axis,left,-2.0/radius);
        look=rotate(look,left,-2.0/radius);

        if(!simulation)
        {
            reflect();
        }
    }

    void rotateClockWise()
    {
        dir=rotate(dir,up,-PI/60);
    }
    void rotateAntiClockWise()
    {
        dir=rotate(dir,up,PI/60);
    }

    bool checkCollision(Point p,Point q)
    {
        double dis=distance(p,q); //from centre to wall containing p and q point

      //  cout<<"from centre to wall "<<dis<<endl;
        if(dis<=radius+2.5)
        {
            return true;
        }
        return false;
    }


};

//camera vars
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
//    cout<< endl;
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
