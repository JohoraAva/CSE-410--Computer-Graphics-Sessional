
#include<bits/stdc++.h>
#include "Point.cpp"
// #include "Matrix.cpp"
// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
using namespace std;
using namespace glm;

#define PI 3.14159265


double cubeAngle=0.0;
class Ball{

public:
    Point centre,axis,dir,look;
    double radius;
    double angle,angleForward,distForward;
    Point up;//fixed 

    //walls
    Point wall[4];
    double height=10;
    double width=100;

    void wallinit()
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
        angle=3.14159/180;

        up=Point (0,0,1);


        wallinit();
    }

    Ball(Point centre, Point axis, Point dir, Point look, double radius){
        this->centre=centre;
        this->axis = axis;
        this->dir = dir;
        this->look = look;
        this->radius = radius;
        angle=3.14159/180;

        wallinit();
    }

    

    void drawCylinder(double radius,double height,int segments)
    {
        Point points[101];

        for(int i=0;i<segments;i++)
        {
            points[i]=Point(radius*cos(2*PI*i/segments),radius*sin(2*PI*i/segments),0);
        }

        //draw cylinder

        for(int i=0;i<segments;i++)
        {
            glColor3f(i*0.03,(100-i)*0.06,(i)*0.5);
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i].x,points[i].y,0);
                glVertex3f(points[i].x,points[i].y,height);
                glVertex3f(points[(i+1)%segments].x,points[(i+1)%segments].y,height);
                glVertex3f(points[(i+1)%segments].x,points[(i+1)%segments].y,0);
            }
            glEnd();
        }
    }
   

    void drawSphereQuad(Point a, Point b, Point c, Point d,Point centre,int cnt)
    {   
        //
        
        Point points[101][101] ;

        a=a-centre;
        b=b-centre;
        c=c-centre;
        d=d-centre;
        // cout<<"centre";
        // centre.print();
       
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
                // points[i][j]=points[i][j]+centre;
                glBegin(GL_QUADS);
                {
                    if (cnt%2)
                        glColor3f(i*0.03,(100-i)*0.06,(j)*0.5);
                        // glColor3f((100-i)*0.0006,(i)*0.0001,(100-i)*0.0002);
                    else
                         glColor3f((100-i)*0.009,(i)*0.08,(100-i)*0.04);
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                }

                glEnd();
            }
        }
    }

Point rotateAroundVector(Point p, Point n,double angle)
{
    vec3 pVec(p.x,p.y,p.z);
    vec3 nVec(n.x,n.y,n.z);
    nVec = normalize(nVec);                // Normalize the axis
    // float radian = radians(angle);          // Convert angle to radians
    float radian = angle;          // if angle is in radians
    mat4 rotationMatrix = rotate(mat4(1.0f), radian, nVec);
    vec4 rotatedPoint = rotationMatrix * vec4(pVec, 1.0f);
    // Point ret = Point(vec3(rotatedPoint));
    return Point(vec3(rotatedPoint));
}
    void drawLine(Point a, Point b)
    {   
        glColor3f(0,0,0);
        glBegin(GL_LINES);{
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
        }glEnd();
    }

    void drawCylinder(Point a,Point b, Point c,Point d, Point c1, Point c2, int prec) // c1=centre1 , c2=centre2
    {
        Point points[101][101];

        //assign value to points
        for(int i=0;i<=prec;i++) //axis
        {
            for(int j=0;j<=prec;j++) //arc 
            {
                Point p1=(a*(prec-i)+c*i)*(1.0/(double)prec); //a:c distance
                Point p2=(b*(prec-i)+d*i)*(1.0/(double)prec); //b:d distance
                Point c0=(c1*(prec-i)+c2*i)*(1.0/(double)prec); //c1:c2 distance
                double radius=(c0-p1).magnitude(); //distance from  origin to a
                points[i][j]=(((p1-c0)*((double)prec-j)+(p2-c0)*j)*(1.0/(double)prec)).normalize()*radius; 
                points[i][j]=points[i][j]+c0;

            }
        }

        //draw quads using points
        for(int i=0;i<prec;i++)
        {
            for(int j=0;j<prec;j++)
            {
                glBegin(GL_QUADS);
                {
                    //  glColor3f(j*0.3,(i)*0.6,(100-i)*0.01);
                     
                    glColor3f((i)*(4/(double)prec),i*(2/(double)prec),(j)*(0.3/(double)prec));
                    // glColor3f(0.4,0.7,0.7);
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                }
                glEnd();
            }
        }
        
    }
 
    void drawArrow(Point a, Point b, Point n){
        double r = 1;
        Point c = a*0.3+b*0.7;
        n = n.normalize();
        for(int i=0; i<4; i++){
            Point m= rotateAroundVector(n,b-a,PI/2);
            Point a1 = a+n*r, a2 = a+m*r;
            Point b1 = b+n*(0.01), b2 = b+m*(0.01);
            // glColor3f(0,0,1);
            drawCylinder(a1,a2,c+n*r  ,c+m*r  ,a,c,10);
            // glColor3f(0,0,0.9);
            drawCylinder(b1,b2,c+n*(r*2),c+m*(r*2),b,c,10);
            n = m;
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
    
   
    void draw()
    {
        Point northPole=centre+axis*radius;
        Point southPole=centre-axis*radius;

        Point equators[8];
        equators[0]=look;
        for(int i=1;i<8;i++)
        {
            equators[i]=rotateAroundVector(equators[i-1],axis,PI/4);
        }

        // Point equators[8]={Point(1,0,0).normalize()*radius,Point(1,1,0).normalize()*radius,Point(0,1,0).normalize()*radius,Point(-1,1,0).normalize()*radius,Point(-1,0,0).normalize()*radius,Point(-1,-1,0).normalize()*radius,Point(0,-1,0).normalize()*radius,Point(1,-1,0).normalize()*radius};
        
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

            if(dis<=radius)
            {
                //draw line
                // dir=Point(0,0,0)-dir;
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
        axis=rotateAroundVector(axis,left,1.0/radius);
        look=rotateAroundVector(look,left,1.0/radius);

        reflect();
    }

    void moveBackward()
    {
        centre=centre-dir;
        Point left=up*dir;
        axis=rotateAroundVector(axis,left,-1.0/radius);
        look=rotateAroundVector(look,left,-1.0/radius);

        reflect();
    }

    void rotateClockWise()
    {
        dir=rotateAroundVector(dir,up,PI/60);
    }
    void rotateAntiClockWise()
    {
        dir=rotateAroundVector(dir,up,-PI/60);
    }


    


};