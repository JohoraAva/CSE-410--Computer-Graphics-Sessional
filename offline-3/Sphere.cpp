#include "classes.cpp"
#define pi (2*acos(0.0))



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
                glColor3f(color[0],color[1],color[2]);
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
        return normal;
    }


    virtual double intersectHelper(Ray ray)
    {
        double t1,t2;
        double a = 1;
        double b = 2*ray.dir.dot(ray.start-position);
        double c= (ray.start-position).magnitude()*(ray.start-position).magnitude() - length*length;
        double d = b*b - 4*a*c;
        double t=-1;
        if(d<0)
        {
            t=-1;
        }
        else
        {
        //    if(fabs(a)<0.0001)
        //    {
        //         return -c*1.0/b;
        //    }
            t1=(-b+sqrt(d))/(2*a);
            t2=(-b-sqrt(d))/(2*a);

            //select max and compare with 0
            double tMax=max(t1,t2);
            double tMin=min(t1,t2);
            if(tMin>0)
            {
                t=tMin;
            }
            else if(tMax>0)
            {
                t=tMax;
            }
            else
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
            glColor3f(color[0],color[1],color[2]);
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

        if(incident.dir.dot(normal.dir)<0)
        {
            normal.dir=normal.dir*(-1);
        }
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
        Point dir;

        if(incident.dir.z>0)
        {
            dir = Point(0,0,1);
        }
        else
        {
            dir = Point(0,0,-1);
        }

        Ray normal(intersectionPoint,dir);
        return normal;
    }


    virtual void getColor(Point p, double* color)
    {
        int i=(p.x-position.x)/tiles;
        int j=(p.y-position.y)/tiles;

        if(i<0 || i>=tiles || j<0 ||j>=tiles)
        {
            color[0]=0;
            color[1]=0;
            color[2]=0;
            // return;
        }
        else if((i+j)%2==0)
        {
            color[0]=1;
            color[1]=1;
            color[2]=1;
        }
        else
        {
            color[0]=0;
            color[1]=0;
            color[2]=0;
        }
    }
    virtual double intersectHelper(Ray ray)
    {
        Point normal(0,0,1);
        double dotProduct=ray.dir.dot(normal);

        // if (round(dotProduct * 100) == 0)
		// 	return -1;


        double t=-ray.start.dot(normal)/dotProduct;

        Point p=ray.start+ray.dir*t;

        //check bound??
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

    }
    virtual Ray getNormal(Point intersectionPoint, Ray incident)
    {
        Ray normal;
        normal.start = intersectionPoint;
        normal.dir = Point(2*a*intersectionPoint.x + d*intersectionPoint.y + e*intersectionPoint.z + g,
                            2*b*intersectionPoint.y + d*intersectionPoint.x + f*intersectionPoint.z + h,
                            2*c*intersectionPoint.z + e*intersectionPoint.x + f*intersectionPoint.y + i);
        normal.dir=normal.dir.normalize();

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
        // if(fabs(c0)<0.0001)
        // {
        //     return -1;
        // }
        double t1=(-c1+sqrt(d))/(2*c0);
        double t2=(-c1-sqrt(d))/(2*c0);
        if(t1<0 && t2<0)
        {
            return -1;
        }
        double tMin = min(t1,t2);
        double tMax = max(t1,t2);

        if(!bounded(ray.start+ray.dir*tMin)) tMin = -1.0;
        if(!bounded(ray.start+ray.dir*tMax)) tMax = -1.0;

        if(tMin>0)
        {
            Point intersectPoint=ray.start+ray.dir*tMin;
            // if(bounded(intersectPoint))
            // {
                return tMin;
            // }
        }
        else if(tMax>0)
        {
            Point intersectPoint=ray.start+ray.dir*tMax;
            // if(bounded(intersectPoint))
            // {
                return tMax;
            // }
        }

        return -1;
    }

};