#include "Point.cpp"
#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

class Object;

extern vector <Object*> objects;
extern vector <PointLight*> pointLights;
extern vector <SpotLight*> spotLights;
extern int recursionLevel;


class Object{

public:
    Point position;   
    double height,width,length;
    double color[3];
    double coEfficients[4]; // 0-ambient, 1-diffuse, 2-specular, 3-reflection

    int shine;

    Object()
    {

    }

    virtual void draw()=0;
    virtual void getColor(Point p, double* color)
    {
        for(int i=0;i<3;i++)
        {
            color[i]=this->color[i];
        }
    }
   
    void setColor(double r,double g,double b)
    {
        color[0]=r;
        color[1]=g;
        color[2]=b;
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

    virtual double intersect(Ray ray, double* color, int level)
    {
        double t=intersectHelper(ray);
        //
        if(t<0)
        {
            return -1;
        }

            getColor(ray.start+ray.dir*t,color);

        //  if(level==0)
        // {
        //     getColor(ray.start+ray.dir*t,color);
        //     return t;
        // }

        Point intersectPoint=ray.start+ray.dir*t;


        //color calculation
        // double* intersectColor=new double[3];
        double intersectColor[3];
        getColor(intersectPoint,intersectColor);
        for(int i=0;i<3;i++)
        {
            // cout<<"ekhane ashe?"<<endl;
            color[i]=intersectColor[i]*coEfficients[0];
        }

        // if(level==0) return t;

        Ray normal = getNormal(intersectPoint,ray);

        for(int i=0;i<pointLights.size();i++)
        {
            Point lightPosition=pointLights[i]->light_position;
            Point dir=intersectPoint-lightPosition;

            Ray lightRay(lightPosition,dir);
            // Ray normal = getNormal(intersectPoint,lightRay);

            
            // double t2=(intersectPoint-lightPosition).magnitude();
            double t2 = INT_MAX;
            double d = intersectHelper(lightRay);
            // if(t2<0.00001)
            // {
            //     continue;
            // }


            //isobscured
            bool isObscured=false;

            for(int j=0;j<objects.size();j++)
            {
                double t3=objects[j]->intersectHelper(lightRay);
                if(t3>0 && t3<t2) t2 = t3;
                // if(t3>0 && t3+0.0001<t2)
                // {
                //     isObscured=true;
                //     // cout<<"flag ki true?"<<endl;
                //     break;
                // }
               
            }
            if(abs(t2-d)>=0.001 || d<0) isObscured = true;
            if(!isObscured)
            {
                double lambertValue=-1*lightRay.dir.dot(normal.dir);
                lambertValue=max(0.0,lambertValue);

                // find reflected ray
                Ray reflectedRay = Ray(intersectPoint, lightRay.dir - normal.dir*2*lambertValue);
                double phong = max(0.0,-ray.dir.dot(reflectedRay.dir));
                
                for(int k=0;k<3;k++)
                {
                    // cout<<"ekhane ashe? 2"<<endl;
                    color[k]+=(pointLights[i]->color[k]*intersectColor[k]*coEfficients[1]*lambertValue);
                    color[k]+=(pointLights[i]->color[k]*intersectColor[k]*coEfficients[2]*pow(phong,shine));
                }
            }
        }

        for(int i=0;i<spotLights.size();i++)
        {
            Point lightPosition=spotLights[i]->pointLight.light_position;
            Point dir=intersectPoint-lightPosition;
            dir=dir.normalize();

            double dotVal=dir.dot(spotLights[i]->direction);
            double denumerator=dir.magnitude()*spotLights[i]->direction.magnitude();
            double angle=acos(dotVal/denumerator)*(180.0/pi); //angle in degree

            if(fabs(angle)<spotLights[i]->cutoffAngle)
            {
                Ray lightRay(lightPosition,dir);
                // Ray normal = getNormal(intersectPoint,lightRay);

               //reflection dir
                Point reflectionDir=normal.dir*2*(dir.dot(normal.dir));
                Ray reflectionRay(intersectPoint,dir-reflectionDir);


                double t2=(intersectPoint-lightPosition).magnitude();
                if(t2<0.00001)
                {
                    continue;
                }

                bool isObscured=false;

                for(int j=0;j<objects.size();j++)
                {
                    double t3=objects[j]->intersectHelper(lightRay);
                    if(t3>=0 && t3+0.00001<t2)
                    {
                        isObscured=true;
                        break;
                    }
                }

                if(!isObscured)
                {
                    double lambertValue=-1*lightRay.dir.dot(normal.dir);
                    lambertValue=max(0.0,lambertValue);

                    // find reflected ray
                    // Ray reflectedRay = Ray(intersectPoint, lightRay.dir - normal.dir*2*dotVal);
                    double phong = max(0.0,-ray.dir.dot(reflectionRay.dir));
                    
                    for(int k=0;k<3;k++)
                    {
                        // cout<<"ekhane ashe? 3"<<endl;
                        color[k]+=(spotLights[i]->pointLight.color[k]*intersectColor[k]*coEfficients[1]*lambertValue);
                        color[k]+=(spotLights[i]->pointLight.color[k]*intersectColor[k]*coEfficients[2]*pow(phong,shine));
                    }
                }

            }
        }
        if(level == 0) return t;
        // if(level<=recursionLevel)
        // if(level>0)
        // {
            //reflection
            // cout<<"checking level: "<<level<<endl;
            // Ray normal=getNormal(intersectPoint,ray);
            Point reflectionDir=normal.dir*2*(ray.dir.dot(normal.dir));
            Ray reflectionRay(intersectPoint,ray.dir-reflectionDir);


            reflectionRay.start=reflectionRay.start+reflectionRay.dir*0.00001;


            int nearest=-1;
            double tMin=INT_MAX;
            double t0=-1;
            for(int i=0;i<objects.size();i++)
            {
                t0=objects[i]->intersect(reflectionRay,color,0);
                if(t0>0 && t0<tMin)
                {
                    tMin=t0;
                    nearest=i;
                }
            }

            if(nearest!=-1)
            {
                // double* tempColor=new double[3];
                double tempColor[3];
                t=objects[nearest]->intersect(reflectionRay,tempColor,level-1);
                // if(t>=0)
                // {   
                    // cout<<"ekhane ashe? 4"<<endl;
                    for(int i=0;i<3;i++)
                    {
                        // cout<<"before: "<<tempColor[i]<<" , "<<color[i]<<endl;
                        color[i]+=tempColor[i]*coEfficients[3];
                        // cout<<"after1: "<<coEfficients[3]<<endl;
                        // cout<<"after2: "<<color[i]<<endl;
                        // cout<< coEfficients[3]<< endl;
                    }
                // }
            }
        // }

        return t;
    }

};




