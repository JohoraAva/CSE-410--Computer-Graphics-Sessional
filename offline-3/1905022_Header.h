#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>

#include<bits/stdc++.h>
using namespace std;

#define pi (2*acos(0.0))


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
    Point color;

    PointLight()
    {

    }
    PointLight(Point p, Point color)
    {
        light_position = p;
        
        this->color = color;
    }
    void draw()
    {
        glPointSize(6);
        glBegin(GL_POINTS);
        {
            glColor3f(color.x,color.y,color.z);
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


    SpotLight(Point p, Point color,Point d, double cutoffAngle)
    {
        pointLight=PointLight(p,color);
        direction = d;
        this->cutoffAngle = cutoffAngle;
    }

    void draw()
    {
        glPointSize(20);
        glBegin(GL_POINTS);
        {
            glColor3f(pointLight.color.x,pointLight.color.y,pointLight.color.z);
            glVertex3f(pointLight.light_position.x,pointLight.light_position.y,pointLight.light_position.z);
        }
        glEnd();
    }

};



class Object;


vector <Object*> objects;
vector <PointLight*> pointLights;
vector <SpotLight*> spotLights;
int recursionLevel;




class Object{

public:
    Point position;   
    double height,width,length;
    Point color;//x=r,y=g,z=b
    double coEfficients[4]; // 0-ambient, 1-diffuse, 2-specular, 3-reflection

    int shine;

    Object()
    {

    }

    virtual void draw()=0;

    virtual Point getColor(Point p)
    {
        Point temColor;
        temColor.x = color.x;
        temColor.y = color.y;
        temColor.z = color.z;

        return temColor;
    }
   
    void setColor(double r,double g,double b)
    {
        color.x=r;
        color.y=g;
        color.z=b;
    }
    

    void setShine(int s)
    {
        shine = s;
    }


    void setCoEfficients(double a,double b,double c,double d)
    {
        coEfficients[0]=a;
        coEfficients[1]=b;
        coEfficients[2]=c;
        coEfficients[3]=d;
    }
    virtual Ray getNormal(Point intersectionPoint, Ray incident)=0;
    virtual double intersectHelper(Ray ray)=0;

   
    virtual double intersect(Ray ray, Point &color, int level)
    {
        double t=intersectHelper(ray);


        if(t<0)
        {
            return -1;
        }
        

        Point intersectPoint=ray.start+ray.dir*t;

        //color calculation
        Point intersectColor=getColor(intersectPoint);
        color = intersectColor*coEfficients[0];

        
        Ray normal=getNormal(intersectPoint,ray);
        Point normalDir = normal.dir;
        
        for(int i=0;i<pointLights.size();i++)
        {
            Point lightPosition=pointLights[i]->light_position;
            Point dir=intersectPoint-lightPosition;


            Ray lightRay(lightPosition,dir);
            

            double dist = intersectHelper(lightRay);
            double tmin = INT_MAX;


            //isObscured 
            bool flag=false;

            for(int j=0;j<objects.size();j++)
            {
                double t3=objects[j]->intersectHelper(lightRay);
                

                //finding minimum distance
                if(t3>0 && t3<tmin) 
                    tmin = t3;
               
            }
            

            //if tmin >dist, then the object is obscured, as object is far from the light source
            //dist<0 means object is behind the camera


            //tmin-dist>0.01
            if(abs(tmin-dist)>0.01 || dist<0) 
                    flag = true;

            if(!flag) //if not obscured ,then add color
            {       
                double lambert=(-1.0*lightRay.dir.dot(normalDir));
               
                // find reflected ray
                Point reflectedDir = (ray.dir - normal.dir*2*(ray.dir.dot(normalDir))).normalize();
                double phong = pow(-1.0*reflectedDir.dot(lightRay.dir),shine);
                
              

                // cout<< phong<< " "<< lambert<< endl;
                color.x = color.x - pointLights[i]->color.x*intersectColor.x*coEfficients[1]*lambert;
                color.x = color.x - pointLights[i]->color.x*intersectColor.x*coEfficients[2]*phong;

                color.y = color.y - pointLights[i]->color.y*intersectColor.y*coEfficients[1]*lambert;
                color.y = color.y - pointLights[i]->color.y*intersectColor.y*coEfficients[2]*phong;

                color.z = color.z - pointLights[i]->color.z*intersectColor.z*coEfficients[1]*lambert;
                color.z = color.z - pointLights[i]->color.z*intersectColor.z*coEfficients[2]*phong;


            }
        }

        for(int i=0;i<spotLights.size();i++)
        {
            Point lightPosition=spotLights[i]->pointLight.light_position;
            Point dir=intersectPoint-lightPosition;


            Ray lightRay(lightPosition,dir);
            

            double dist = intersectHelper(lightRay);
            double tmin = INT_MAX;

            bool flag=false;

            for(int j=0;j<objects.size();j++)
            {
                double t3=objects[j]->intersectHelper(lightRay);
               
                if(t3>0 && t3<tmin) 
                    tmin = t3;
               
            }

            if(abs(tmin-dist)>0.01 || dist<0) 
                flag = true;

            if(!flag)
            {       

                double angle = acos(abs(spotLights[i]->direction.dot(Point(0,0,0)-lightRay.dir)))*180.0/pi;
                if(abs(angle) > spotLights[i]->cutoffAngle) continue;


                double lambert=-1.0*lightRay.dir.dot(normalDir);

                // find reflected ray
                Point reflectedDir = (ray.dir - normalDir*2*(ray.dir.dot(normalDir))).normalize();
                double phong = pow(-1.0*reflectedDir.dot(lightRay.dir),shine);
                
            

                color.x+=spotLights[i]->pointLight.color.x*intersectColor.x*coEfficients[1]*lambert;
                color.x+=spotLights[i]->pointLight.color.x*intersectColor.x*coEfficients[2]*phong;

                color.y+=spotLights[i]->pointLight.color.y*intersectColor.y*coEfficients[1]*lambert;
                color.y+=spotLights[i]->pointLight.color.y*intersectColor.y*coEfficients[2]*phong;

                color.z+=spotLights[i]->pointLight.color.z*intersectColor.z*coEfficients[1]*lambert;
                color.z+=spotLights[i]->pointLight.color.z*intersectColor.z*coEfficients[2]*phong;
            }
        }

        if(level==0)
        {   
            return t;
        }

          

            Point reflectionDir = (ray.dir - normalDir*2*(ray.dir.dot(normalDir))).normalize();
            Ray reflectionRay(intersectPoint,reflectionDir);


            //to avoid self intersection
            reflectionRay.start=reflectionRay.start+reflectionRay.dir*0.00001;
            
            
            Point newColor;
            double tMin = INT_MAX;
            for(int k=0; k<objects.size();k++)
            {
                Point tempColor;
                double t = objects[k]->intersect(reflectionRay,tempColor, level-1);
                
                if(t<tMin && t>0)
                {
                    tMin = t;
                    newColor = tempColor;
                }
            }
            
            
            color = color + newColor*coEfficients[3];
            

        return t;
    }

};



class Sphere : public Object
{
    public:
    Sphere (Point c, double r)
    {
        position = c;
        length = r;
    }

    Sphere (Point centre,double len, double color[3], double coEfficients[4], int shine)
    {
        position = centre;
        length = len;
        setColor(color[0],color[1],color[2]);
        setCoEfficients(coEfficients[0],coEfficients[1],coEfficients[2],coEfficients[3]);
        setShine(shine);
    }

    virtual void draw()
    {
        Point points[101][101] ;
        double radius=length;
        double h,r;
        //assign value to points
        for(int i=0;i<=100;i++)
        {
            h=radius*sin(((double)i/100)*(pi/2));
            r=radius*cos(((double)i/100)*(pi/2));
            for(int j=0;j<=100;j++)
            {
                points[i][j].x=r*cos(((double)j/100)*2*pi);
                points[i][j].y=r*sin(((double)j/100)*2*pi);
                points[i][j].z=h;
            }
        }

        //draw quads using points
        // cout<<"inside sphere draw\n";
        double prec=100;
        for(int i=0;i<100;i++)
        {
            for(int j=0;j<100;j++)
            {
                glPushMatrix();
                glTranslatef(position.x,position.y,position.z);
                glColor3f(color.x,color.y,color.z);
                glBegin(GL_QUADS);
                
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);

                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                }
                glEnd();
                glPopMatrix();
            }
        }
    }

    virtual Ray getNormal(Point intersectionPoint, Ray incident)
    {
        Point dir=intersectionPoint - position;
        Ray normal(intersectionPoint,dir);
        if(normal.dir.dot(incident.dir)<0) 
            normal.dir = normal.dir*(-1.0);
        return normal;
    }


    virtual double intersectHelper(Ray ray)
    {
        double t1,t2;
        double a = ray.dir.dot(ray.dir);
        double b = 2*ray.dir.dot(ray.start-position);
        double c= (ray.start-position).dot(ray.start-position) - length*length;
        double d = b*b - 4*a*c;
        double t=-1;
        if(d<0) //no intersection
        {
            return -1;
        }
        else
        {
            t1=(-b+sqrt(d))/(2*a);
            t2=(-b-sqrt(d))/(2*a);

            //select max and compare with 0
            double tMax=max(t1,t2);
            double tMin=min(t1,t2);

            //if min is negative, then it's behind the eye,so have to check max
            // min is nearest intersection point
            if(tMin>0)
            {
                t=tMin;
            }
            //if max is also negative, then both are behind the eye
            else if(tMax>0)
            {
                t=tMax;
            }
            else //both are negative
            {
                t=-1;
            }
           
        }

        return t;
    }

};


class Triangle: public Object
{
    public:
    Point a,b,c;
    Triangle(Point a,Point b,Point c)
    {
        this->a=a;
        this->b=b;
        this->c=c;
    }
    Triangle(Point a,Point b,Point c, double color[3], double coEfficients[4], int shine)
    {
        this->a=a;
        this->b=b;
        this->c=c;
        setColor(color[0],color[1],color[2]);
        setCoEfficients(coEfficients[0],coEfficients[1],coEfficients[2],coEfficients[3]);
        setShine(shine);
    }
    virtual void draw()
    {
        glBegin(GL_TRIANGLES);
        {
            glColor3f(color.x,color.y,color.z);
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
            glVertex3f(c.x,c.y,c.z);
        }
        glEnd();
    }

    virtual Ray getNormal(Point intersectionPoint, Ray incident)
    {
        Point dir=(b-a)*(c-a);
        Ray normal(intersectionPoint,dir);

        if(normal.dir.dot(incident.dir)<0) 
            normal.dir = normal.dir*(-1.0);
        return normal;
    }

    double getArea(Point a,Point b,Point c)
    {
        Point res= (b-a)*(c-a);
        double area=res.magnitude()*0.5;
        return area;
    }
    bool isInside(Point p,Point a,Point b,Point c)
    {
        if(getArea(p,a,b)+getArea(p,b,c)+getArea(p,c,a)-getArea(a,b,c)<0.0006)
        {
            return true;
        }
        return false;
    }


    virtual double intersectHelper(Ray ray)
    {

        Point temp1=a-b;
        Point temp2=a-c;
        Point temp3=temp1*temp2;

        double D=-a.dot(temp3);
        double t=-(temp3.dot(ray.start)+D)/(temp3.dot(ray.dir));
        Point p=ray.start+ray.dir*t;

        //check bound 
        bool inside=isInside(p,a,b,c);
        if(inside)
        {
            return t;
        }
        else
        {
            return -1;
        }
        
    }
};

class Floor: public  Object{
    public:
    int tiles;
    
    Floor(double floorWidth, int tileWidth)
    {
        this->length=floorWidth;
        this->position=Point(-floorWidth/2, -floorWidth/2, 0);
        tiles=floorWidth/tileWidth;
    }

    virtual void draw()
    {
        glPushMatrix();
        {
            glTranslatef(position.x,position.y,0);
            for(int i=0;i<tiles;i++)
            {
                for(int j=0;j<tiles;j++)
                {
                    if((i+j)%2==0)
                    {
                        glColor3f(1,1,1);
                    }
                    else
                    {
                        glColor3f(0,0,0);
                    }
                    glBegin(GL_QUADS);
                    {
                        glVertex3f(i*tiles,j*tiles,0);
                        glVertex3f(i*tiles,(j+1)*tiles,0);
                        glVertex3f((i+1)*tiles,(j+1)*tiles,0);
                        glVertex3f((i+1)*tiles,j*tiles,0);
                    }
                    glEnd();
                    glTranslatef(0,1,0);
                }
                glTranslatef(1,-tiles,0);
            }
        }
        glPopMatrix();
    }

    virtual Ray getNormal(Point intersectionPoint, Ray incident)
    {
        Ray normal(intersectionPoint,Point(0,0,1));
        if(normal.dir.dot(incident.dir)<0) 
            normal.dir = normal.dir*(-1.0);
        return normal;
    }


    virtual Point getColor(Point p)
    {
        int i=(p.x-position.x)/tiles;
        int j=(p.y-position.y)/tiles;

        if(i<0 || i>=tiles || j<0 ||j>=tiles)
        {
            return Point(0,0,0);
        }
        else if((i+j)%2==0)
        {
            return Point(1,1,1);
        }
        else
        {
            return Point(0,0,0);
        }
    }
    virtual double intersectHelper(Ray ray)
    {

        Point normal(0,0,1);
        double dotProduct=ray.dir.dot(normal);

       

        double t=-ray.start.dot(normal)/dotProduct;

        Point p=ray.start+ray.dir*t;

        if(p.x <= position.x || p.x >= abs(position.x) && p.y <= position.y && p.y >= abs(position.y))
        {
            return -1;
        }
        
        return t;
    }

      

};


class General :public Object
{
    public:
    double a,b,c,d,e,f,g,h,i,j;

    General(double a,double b,double c,double d,double e,double f,double g,double h,double i,double j,Point p,double hg,double w,double len,double color[3], double coEfficients[4], int shine)
    {
        this->a=a;
        this->b=b;
        this->c=c;
        this->d=d;
        this->e=e;
        this->f=f;
        this->g=g;
        this->h=h;
        this->i=i;
        this->j=j;
        this->position=p;
        this->height=hg;
        this->width=w;
        this->length=len;
        setColor(color[0],color[1],color[2]);
        setCoEfficients(coEfficients[0],coEfficients[1],coEfficients[2],coEfficients[3]);
        setShine(shine);
    }
    virtual void draw()
    {
        //no need to draw :')
    }
    virtual Ray getNormal(Point intersectionPoint, Ray incident)
    {
        Ray normal;
        normal.start = intersectionPoint;
        normal.dir = Point(2*a*intersectionPoint.x + d*intersectionPoint.y + e*intersectionPoint.z + g,
                            2*b*intersectionPoint.y + d*intersectionPoint.x + f*intersectionPoint.z + h,
                            2*c*intersectionPoint.z + e*intersectionPoint.x + f*intersectionPoint.y + i);


        normal.dir=normal.dir.normalize();
        if(normal.dir.dot(incident.dir)<0) 
            normal.dir = normal.dir*(-1.0);

        return normal;
    }

    bool bounded(Point p)
    {
       if(length!=0)
       {
           if(p.x<position.x || p.x>position.x+length)
           {
               return false;
           }
       }

         if(width!=0)
         {
              if(p.y<position.y || p.y>position.y+width)
              {
                return false;
              }
         }

        if(height!=0)
        {
            if(p.z<position.z || p.z>position.z+height)
            {
                return false;
            }
        }
        return true;
    }

    virtual double intersectHelper(Ray ray)
    {

        double x0=ray.start.x;
        double y0=ray.start.y;
        double z0=ray.start.z;

        double x1=ray.dir.x;
        double y1=ray.dir.y;
        double z1=ray.dir.z;

        double c0=a*x1*x1 + b*y1*y1 + c*z1*z1 + d*x1*y1 + e*x1*z1 + f*y1*z1;
        double c1=2*a*x0*x1 + 2*b*y0*y1 + 2*c*z0*z1 + d*(x0*y1 + x1*y0) + e*(x0*z1 + x1*z0) + f*(y0*z1 + y1*z0) + g*x1 + h*y1 + i*z1;
        double c2=a*x0*x0 + b*y0*y0 + c*z0*z0 + d*x0*y0 + e*x0*z0 + f*y0*z0 + g*x0 + h*y0 + i*z0 + j;


        double d=c1*c1 - 4*c0*c2;
        if(d<0)
        {
            return -1;
        }
       

        double t1=(-c1+sqrt(d))/(2*c0);
        double t2=(-c1-sqrt(d))/(2*c0);


        if(t1<0 && t2<0)
        {
            return -1;
        }
        double tMin = min(t1,t2);
        double tMax = max(t1,t2);

        if(!bounded(ray.start+ray.dir*tMin)) 
            tMin = -1.0;
        if(!bounded(ray.start+ray.dir*tMax)) 
            tMax = -1.0;

        if(tMin>0)
        {
            return tMin;

        }
        else if(tMax>0)
        {
            return tMax;
        }

        return -1;
    }

};







