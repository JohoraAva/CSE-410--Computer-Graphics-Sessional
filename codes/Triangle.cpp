#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
#include "Point.cpp"

#include<bits/stdc++.h>

using namespace std;

#define pi (2*acos(0.0))

double xArr[2]={-30.0,30.0};
double yArr[2]={-30.0,30.0};
double zArr[2]={-30.0,30.0};
double r=1.0;
double fullLength=30.0;

class Triangle
{
public:
    Point a,b,c,m;

    Triangle()
    {
        this->a = Point(0,0,0);
        this->b = Point(0,0,0);
        this->c = Point(0,0,0);
        this->m = Point(0,0,0);
    }

    Triangle(Point a, Point b, Point c,Point m)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->m = m;
    }

    Triangle(const Triangle &t)
    {
        this->a = t.a;
        this->b = t.b;
        this->c = t.c;
        this->m = t.m;
    }

    void print()
    {
        cout << "Triangle: \n";
        this->a.print();
        this->b.print();
        this->c.print();
        this->m.print();
    }

    void draw(double cnt)
    {
        // cout<<"cnt = "<<cnt<<endl;
        // cout<<"a = "<<0.8*cnt<<", "<<0.2*cnt<<" ,"<<0.3*cnt<<endl;
        glBegin(GL_TRIANGLES);
        {
            // cout<<"inside drwa\n";
            // glColor3f(cnt*0.02,0.06*cnt,0.1*cnt);
            // glColor3f(0.08*cnt,0.02*cnt,0.05*cnt);
            glColor3f(0.9*cnt,0.7*cnt,0.8*cnt);
            // glColor3f(0.03*cnt,0.7*cnt,0.8*cnt);
          
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
            glVertex3f(c.x,c.y,c.z);
        }
        glEnd();
    }

};
// Triangle triangles[8]; //8 triangles
// Point points[24]; //24 points



void drawSphereQuad(Point a, Point b, Point c, Point d,Point centre)
{   
    //
    
    Point points[101][101] ;

    a=a-centre;
    b=b-centre;
    c=c-centre;
    d=d-centre;
     double radius=a.magnitude(); //distance from origin to a
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
                // glColor3f(i*0.03,(100-i)*0.06,(j)*0.5);
                glColor3f(i*0.3,(100-j)*0.6,(100-i)*0.01);
                
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
            }
            glEnd();
        }
    }
  

}


void drawCylinder(Point a,Point b, Point c,Point d, Point c1, Point c2) // c1=centre1 , c2=centre2
{
    Point points[101][101];
    double radius=(c1-a).magnitude(); //distance from origin to a

    //assign value to points
    for(int i=0;i<=100;i++) //axis
    {
        for(int j=0;j<=100;j++) //arc 
        {
            Point p1=(a*(100-i)+c*i)*(1.0/100.0);
            Point p2=(b*(100-i)+d*i)*(1.0/100.0);
            Point c0=(c1*(100-i)+c2*i)*(1.0/100.0);
            points[i][j]=(((p1-c0)*(100-j)+(p2-c0)*j)*(1.0/100.0)).normalize()*radius;
            points[i][j]=points[i][j]+c0;

        }
    }

    //draw quads using points
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<100;j++)
        {
            glBegin(GL_QUADS);
            {
                //  glColor3f(j*0.3,(i)*0.6,(100-i)*0.01);
                //  glColor3f(i*0.3,(100-j)*0.6,(100-i)*0.01);
                  glColor3f(i*0.03,(100-i)*0.06,(j)*0.5);
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
            }
            glEnd();
        }
    }
    
}



void drawTriangles()
{
    int cnt=0;
    Point arr[8][3];

  

    //  for(int i = 0; i < 8; i++){
        // glPushMatrix();
    //     // if(i%4==0)
    //     //     glRotatef(i * 90, 0, 0, 0);
    //     // else if(i%4==1)
    //     //     glRotatef(0 , i*90, 0, 0);
    //     // glRotatef(0, 0, 0, i*90);
    //     // // glTranslatef(0, 0, 20);
         
    //         // glRotatef(-i * 90, 1, 0, 0);
    //     Point x=Point(xArr[0],0,0);
    //     Point y=Point(0,yArr[0],0);
    //     Point z=Point(0,0,zArr[1]);
    //     Point m=Point(xArr[0]*(1.0/3),yArr[0]*(1.0/3),zArr[1]*(1.0/3));

    //                 //
    //     x=x*r+m*(1-r);
    //     y=y*r+m*(1-r);
    //     z=z*r+m*(1-r);
    //      glRotatef(i * 90, 1, 0, 0);
    //       glTranslatef(0, 0, 0);

    //     Triangle t=Triangle(x,y,z,m);
    //     t.draw(cnt++);
    //     cout<<cnt<<": "<<endl;
    //     t.print();
         
    //     glPopMatrix();
    // }
   
   for (int i = 0; i < 2; ++i)
   {
       for (int j = 0; j < 2; ++j)
       {
           for (int k = 0; k < 2; ++k)
           {
                Point x=Point(xArr[i],0,0);
                Point y=Point(0,yArr[j],0);
                Point z=Point(0,0,zArr[k]);
                Point m=Point(xArr[i]*(1.0/3),yArr[j]*(1.0/3),zArr[k]*(1.0/3));

                //
                x=x*r+m*(1-r);
                y=y*r+m*(1-r);
                z=z*r+m*(1-r);

                Triangle t=Triangle(x,y,z,m);
                arr[cnt][0]=x;
                arr[cnt][1]=y;
                arr[cnt][2]=z;
               
                cnt++;
                t.draw(cnt*0.2);


           }
       }
   }
    
    drawSphereQuad(arr[0][0],arr[2][0],arr[3][0],arr[1][0],Point(-r*fullLength,0,0));
    drawSphereQuad(arr[4][0],arr[6][0],arr[7][0],arr[5][0],Point(r*fullLength,0,0));
    drawSphereQuad(arr[0][1],arr[1][1],arr[5][1],arr[4][1],Point(0,-r*fullLength,0));
    drawSphereQuad(arr[6][1],arr[2][1],arr[3][1],arr[7][1],Point(0,r*fullLength,0));
    drawSphereQuad(arr[0][2],arr[2][2],arr[6][2],arr[4][2],Point(0,0,-r*fullLength));
    drawSphereQuad(arr[1][2],arr[3][2],arr[7][2],arr[5][2],Point(0,0,r*fullLength));

    //cylinder
    Point centres[6]={Point(-r*fullLength,0,0),Point(r*fullLength,0,0),Point(0,-r*fullLength,0),Point(0,r*fullLength,0),Point(0,0,-r*fullLength),Point(0,0,r*fullLength)};
   
 
    drawCylinder(arr[0][0],arr[1][0],arr[0][1],arr[1][1],centres[0],centres[2]);
    drawCylinder(arr[2][0],arr[3][0],arr[2][1],arr[3][1],centres[0],centres[3]);
    drawCylinder(arr[0][0],arr[2][0],arr[0][2],arr[2][2],centres[0],centres[4]);
    drawCylinder(arr[1][0],arr[3][0],arr[1][2],arr[3][2],centres[0],centres[5]);
    drawCylinder(arr[4][0],arr[5][0],arr[4][1],arr[5][1],centres[1],centres[2]);
    drawCylinder(arr[6][0],arr[7][0],arr[6][1],arr[7][1],centres[1],centres[3]);
    drawCylinder(arr[4][0],arr[6][0],arr[4][2],arr[6][2],centres[1],centres[4]);
    drawCylinder(arr[5][0],arr[7][0],arr[5][2],arr[7][2],centres[1],centres[5]);
    drawCylinder(arr[0][1],arr[4][1],arr[0][2],arr[4][2],centres[2],centres[4]);
    drawCylinder(arr[1][1],arr[5][1],arr[1][2],arr[5][2],centres[2],centres[5]);
    drawCylinder(arr[2][1],arr[6][1],arr[2][2],arr[6][2],centres[3],centres[4]);
    drawCylinder(arr[3][1],arr[7][1],arr[3][2],arr[7][2],centres[3],centres[5]);


}



void drawCone()
{
    glPushMatrix();
    {
        glColor3f(0.6,0.8,0.3);
        glTranslatef(0,0,0);
        glutSolidCone(30,20,20,20);
    }
    glPopMatrix();
}
