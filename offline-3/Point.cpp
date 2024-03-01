#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>

#include<bits/stdc++.h>



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
    //cross product
    Point operator*(const Point &p)
    {
        Point ret;
        ret.x = this->y * p.z - this->z * p.y;
        ret.y = this->z * p.x - this->x * p.z;
        ret.z = this->x * p.y - this->y * p.x;
        return ret;
    }
    //dot product
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



class Ray
{
public:
    Point start;
    Point dir;
    Ray()
    {
        start = Point(0,0,0);
        dir = Point(0,0,1);
    }
    Ray(Point start, Point dir)
    {
        this->start = start;
        this->dir = dir.normalize();
    }
};
class PointLight{

public:
    Point light_position;
    double color[3];

    PointLight()
    {

    }
    PointLight(Point p, double color[3])
    {
        light_position = p;
        for(int i=0; i<3; i++)
        {
            this->color[i] = color[i];
        }
    }

    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        {
            glColor3f(color[0],color[1],color[2]);
            glVertex3f(light_position.x,light_position.y,light_position.z);
        }
        glEnd();
    }

};

class SpotLight{

public:
    PointLight pointLight;
    Point direction;
    double cutoffAngle;


    SpotLight(Point p, double color[3],Point d, double cutoffAngle)
    {
        pointLight=PointLight(p,color);
        direction = d;
        this->cutoffAngle = cutoffAngle;
    }

    void draw()
    {
        pointLight.draw(); //pointLight_size=15
      
    }

};

