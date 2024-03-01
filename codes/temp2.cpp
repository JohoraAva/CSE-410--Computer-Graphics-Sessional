#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
#include "Point.cpp"


#include<bits/stdc++.h>

using namespace std;

#define pi (2*acos(0.0))


double r=1.0;
double maxLen=30.0;
double curLen=r*maxLen;
double maxRadius=maxLen/sqrt(3.0);
double curRadius=(1-r)*maxRadius;
double cubeAngle=0.0;

void drawTriangle(int cnt)
{

    // if(cnt<5)
    //     glColor3f(0.03,0.7,0.8);
    // else
   glColor3f(0.03*cnt,0.7*cnt,0.8*cnt);
    
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);
        glVertex3f(0,0,1);
    }
    glEnd();
}

void drawTriangles()
{
    //
    
   curLen=r*maxLen;
    int isNeg=-1; //wait,later
    for(int i=0;i<6;i++)
    {
        isNeg=1;
        glPushMatrix();
        {
            if((i>>2)%2) //mapping 110  -> rotate about x,y
            {
                isNeg*=-1;
                glRotatef(90*isNeg,1,0,0);
                // glColor3f(1,0,0);
                cout<<"ok 1: "<<i<<endl ;
            }
            if((i>>1)%2) //mapping 101  -> rotate about x,z
            {
                 isNeg*=-1;
                glRotatef(90*isNeg,0,1,0);
                // glColor3f(0,1,0);
                cout<<"ok 2: "<<i<<endl ;
            }
            if((i>>0)%2) //mapping 011  -> rotate about y,z
            {
                 isNeg*=-1;
                glRotatef(90*isNeg,0,0,1);
                // glColor3f(0,0,1);
                cout<<"ok 3: "<<i<<endl ;
            }
           
            double m=maxLen*(1-r)/3.0;

            glTranslatef(m,m,m);
            glScalef(curLen,curLen,curLen);
            drawTriangle(i+1);

        }
        glPopMatrix();


    }

    for(int i=0;i<2;i++)
    {
        glPushMatrix();
        {
            glRotatef(180,0,0,1);
            if(i)
            {
                glRotatef(-90,1,0,0);
            }
            double m=maxLen*(1-r)/3.0;

            glTranslatef(m,m,m);
            glScalef(curLen,curLen,curLen);
            drawTriangle(8);
            // i++;

        }
        glPopMatrix();
    }
}


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

void drawSpheres()
{
    // glColor3f(0,0,1);
    curRadius=(1-r)*maxRadius;

    double curPos=maxLen*(1-r)/3.0;
   for(int i=0;i<4;i++)
   {
    glPushMatrix();
    {
        glRotatef(90*i,1,0,0);
        glTranslatef(0,0,curLen);
        drawSphereQuad(Point(curPos,curPos,curPos),Point(curPos,-curPos,curPos),Point(-curPos,-curPos,curPos),Point(-curPos,curPos,curPos),Point(0,0,0));
        // glTranslatef(0,0,-2*curPos);
    }
    glPopMatrix();
   }

   for(int i=0;i<2;i++)
   {
    glPushMatrix();
    {
        if(i%2)
            glRotatef(90,0,1,0);
        else
            glRotatef(-90,0,1,0);
        glTranslatef(0,0,curLen);
        drawSphereQuad(Point(curPos,curPos,curPos),Point(curPos,-curPos,curPos),Point(-curPos,-curPos,curPos),Point(-curPos,curPos,curPos),Point(0,0,0));
        // glTranslatef(0,0,-2*curPos);
    }
    glPopMatrix();
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

void drawFullCylinder(float height)
{
    drawCylinder(Point(curRadius,0,height/2),Point(0,curRadius,height/2),Point(curRadius,0,-height/2),Point(0,curRadius,-height/2),Point(0,0,height/2),Point(0,0,-height/2));
    drawCylinder(Point(-curRadius,0,height/2),Point(0,curRadius,height/2),Point(-curRadius,0,-height/2),Point(0,curRadius,-height/2),Point(0,0,height/2),Point(0,0,-height/2));
    drawCylinder(Point(curRadius,0,height/2),Point(0,-curRadius,height/2),Point(-curRadius,0,-height/2),Point(0,-curRadius,-height/2),Point(0,0,height/2),Point(0,0,-height/2));
    drawCylinder(Point(curRadius,0,height/2),Point(0,-curRadius,height/2),Point(curRadius,0,-height/2),Point(0,-curRadius,-height/2),Point(0,0,height/2),Point(0,0,-height/2));
}
void drawCylinders()
{
    double height=curLen*sqrt(2);

    glPushMatrix();
    {
        glRotatef(90,1,1,0);
        for(int i=0;i<4;i++)
        {
            glPushMatrix();
            {
                glRotatef(90*i,-1,1,0);
                glTranslatef(curLen/2,curLen/2,0);
                drawFullCylinder(height);
            }
            glPopMatrix();
        }
    }
    glPopMatrix();


    //x axis

    glPushMatrix();
    {
        glRotatef(45,1,0,0);
        for(int i=0;i<4;i++)
        {
            glPushMatrix();
            {
                glRotatef(90*i,0,1,1);
                glTranslatef(0,curLen/sqrt(2),0);
                drawFullCylinder(height);
                // drawCylinder(Point(curRadius,0,height/2),Point(0,curRadius,height/2),Point(curRadius,0,-height/2),Point(0,curRadius,-height/2),Point(0,0,height/2),Point(0,0,-height/2));
            }
            glPopMatrix();
        }
    }
    glPopMatrix();

    // //y axis

    glPushMatrix();
    {
        glRotatef(45,0,1,0);
        for(int i=0;i<4;i++)
        {
            glPushMatrix();
            {
                glRotatef(90*i,1,0,1);
                glTranslatef(curLen/sqrt(2),0,0);
                drawFullCylinder(height);
                // drawCylinder(Point(curRadius,0,height/2),Point(0,curRadius,height/2),Point(curRadius,0,-height/2),Point(0,curRadius,-height/2),Point(0,0,height/2),Point(0,0,-height/2));
            }
            glPopMatrix();
        }
    }
    glPopMatrix();
}
void drawAll()
{

    drawTriangles();
    drawSpheres();
    drawCylinders();
}


   








