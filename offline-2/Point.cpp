#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"

#include<bits/stdc++.h>

// using namespace glm;


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

//pos of camera
// Point pos(100, 100, 100);
// //rightlook up vector
// Point up(0, 0, 1);
// Point right(0,1,0);
// Point look(-1,0,0);

// //camera angle
// double angle=1.00;	//in degree


// void axes()
// {
//         glColor3f(1.0, 1.0, 1.0);
// 		glBegin(GL_LINES);{
// 			glVertex3f( 100,0,0);
// 			glVertex3f(-100,0,0);

// 			glVertex3f(0,-100,0);
// 			glVertex3f(0, 100,0);

// 			glVertex3f(0,0, 100);
// 			glVertex3f(0,0,-100);
// 		}glEnd();
    
// }
// /

