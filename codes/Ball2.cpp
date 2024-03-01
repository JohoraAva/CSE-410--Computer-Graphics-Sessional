///version 1
#include<bits/stdc++.h>
// #include "Point.cpp"
#include "Matrix.cpp"
using namespace std;



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
                    //  glColor3f(j*0.3,(i)*0.6,(100-i)*0.01);
                     
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


    void drawArrow(Point a, Point b, Point n)
    {
        double r = 1;
        Point c = a*0.3+b*0.7;
        n = n.normalize();
        for(int i=0; i<4; i++){
            Point m= rotate(n,b-a,PI/2);
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

            if(dis<=radius)
            {   
                while(distance(p,q)<=radius) centre = centre - centre.normalize();
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

        cout<<"from centre to wall "<<dis<<endl;
        if(dis<=radius+2)
        {
            return true;
        }
        return false;
    }


};