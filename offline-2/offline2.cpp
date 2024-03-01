#include<bits/stdc++.h>
// #include "Point.cpp"
#include "Matrix.cpp"
#include "bitmap_image.hpp"
using namespace std;


Point eye,look,up;
double fovY,aspectRatio,near,far;
int screen_Width,screen_Height;

double z_max=2.0;

static unsigned long int g_seed = 1;
inline int Random()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

void printTriangle(Point a, Point b,Point c,ofstream &fout)
{
    fout<< fixed << setprecision(7);
    fout<<a.x<<" "<<a.y<<" "<<a.z<<" "<<endl;
    fout<<b.x<<" "<<b.y<<" "<<b.z<<" "<<endl;
    fout<<c.x<<" "<<c.y<<" "<<c.z<<" "<<endl;
    fout<<endl;
}


void stage1()
{
    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");

    fin>>eye.x>>eye.y>>eye.z;
    fin>>look.x>>look.y>>look.z;
    fin>>up.x>>up.y>>up.z;
    fin>>fovY>>aspectRatio>>near>>far;

    stack<Matrix> stk;
    Matrix I(4,4);
    stk.push(I.identity());

    string command;
    int cnt=0;

    while(fin>>command)
    {
        Point a,b,c;
        if(command=="triangle")
        {
            // Point a,b,c;
            fin>>a.x>>a.y>>a.z;
            fin>>b.x>>b.y>>b.z;
            fin>>c.x>>c.y>>c.z;

            
            a=stk.top()*a;
            b=stk.top()*b;
            c=stk.top()*c;

            printTriangle(a,b,c,fout);
          
            
        }
        else if(command=="translate")
        {
            double tx,ty,tz;
            fin>>tx>>ty>>tz;
            Matrix m(4,4);
            m.identity();
            m.mat[0][3]=tx;
            m.mat[1][3]=ty;
            m.mat[2][3]=tz;
            stk.top()=stk.top()*m;
        }
        else if(command=="scale")
        {
            double sx,sy,sz;
            fin>>sx>>sy>>sz;
            Matrix m(4,4);
            m.identity();
            m.mat[0][0]=sx;
            m.mat[1][1]=sy;
            m.mat[2][2]=sz;
            stk.top()=stk.top()*m;
        }
        else if(command=="rotate")
        {
            double angle;
            Point p;
            fin>>angle>>p.x>>p.y>>p.z;
            Matrix m(4,4);
            m= m.rotate(angle,p);
            stk.top()=stk.top()*m;
           
        }

        else if(command=="push")
        {
            stk.push(stk.top());
        }
        else if(command=="pop")
        {
            stk.pop();
        }
        else if(command=="end")
        {
            break;
        }
        
    }
    fin.close();
    fout.close();

}


void stage2()
{
    ifstream fin("stage1.txt");
    ofstream fout("stage2.txt");


    Point l,r,u;
    l=look-eye;
    l=l.normalize();
    r=l*up;
    r=r.normalize();
    u=r*l;

    Matrix m(4,4);
    m.identity();

    m.mat[0][3]=-eye.x;
    m.mat[1][3]=-eye.y;
    m.mat[2][3]=-eye.z;

    Matrix r1(4,4);
    r1.identity();

    r1.mat[0][0]=r.x;
    r1.mat[0][1]=r.y;
    r1.mat[0][2]=r.z;
    r1.mat[1][0]=u.x;
    r1.mat[1][1]=u.y;
    r1.mat[1][2]=u.z;
    r1.mat[2][0]=-l.x;
    r1.mat[2][1]=-l.y;
    r1.mat[2][2]=-l.z;

    Matrix view=r1*m;

    Point p1,p2,p3;
    while(fin>>p1.x>>p1.y>>p1.z)
    {
        fin>>p2.x>>p2.y>>p2.z;
        fin>>p3.x>>p3.y>>p3.z;


        printTriangle(view*p1,view*p2,view*p3,fout);
       
    }

    fin.close();
    fout.close();

}

void stage3()
{
    ifstream fin("stage2.txt");
    ofstream fout("stage3.txt");

    double fovX=fovY*aspectRatio;
    double t=near*tan((fovY/2)*(PI/180));
    double r=near*tan((fovX/2)*(PI/180));


    Matrix projection(4,4);
    projection.mat[0][0]=near/r;
    projection.mat[1][1]=near/t;
    projection.mat[2][2]=-(1.0*(far+near))/(far-near);
    projection.mat[2][3]=-(2.0*far*near)/(far-near);
    projection.mat[3][2]=-1.0;

    Point p1,p2,p3;
    while(fin>>p1.x>>p1.y>>p1.z)
    {
        fin>>p2.x>>p2.y>>p2.z;
        fin>>p3.x>>p3.y>>p3.z;

        printTriangle(projection*p1,projection*p2,projection*p3,fout);
       
    }


    fin.close();
    fout.close();


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
// ax+by+cz+d=0
double getZ(double A,double B,double C,double D,double x,double y)
{
    double z=-(A*x+B*y+D)/C;
    return z;
}

void stage4()
{
    ifstream fin("stage3.txt");
    ofstream fout("z_buffer.txt");

    ifstream fin2("config.txt");
    fin2>>screen_Width>>screen_Height;
    fin2.close();


    double  left_x=-1.0;
    double  right_x=1.0;
    double  bottom_y=-1.0;
    double  top_y=1.0;
    double near_z=1.0;
    double far_z=-1.0;

    double dx=(right_x-left_x)/screen_Width;
    double dy=(top_y-bottom_y)/screen_Height;
    double topY=top_y-dy/2.0;
    double leftX=left_x+dx/2.0;

    vector<vector<double> > z_buffer(screen_Height,vector<double>(screen_Width,z_max));


    Point p1,p2,p3;
    bitmap_image image(screen_Width,screen_Height);
    image.set_all_channels(0,0,0);

    while(fin>>p1.x>>p1.y>>p1.z)
    {
        fin>>p2.x>>p2.y>>p2.z;
        fin>>p3.x>>p3.y>>p3.z;

        
        double minY=max(bottom_y,min(p1.y,min(p2.y,p3.y)));
        double maxY=min(top_y,max(p1.y,max(p2.y,p3.y)));
        double minX=max(left_x,min(p1.x,min(p2.x,p3.x)));
        double maxX=min(right_x,max(p1.x,max(p2.x,p3.x)));
     //   cout<<"minY: "<<minY<<" maxY: "<<maxY<<" minX: "<<minX<<" maxX: "<<maxX<<endl;


        Point temp1=p1-p2;
        Point temp2=p1-p3;
        Point temp3=temp1*temp2;

        double D=-p1.dot(temp3);
        
        double r,g,b;
        r=Random();
        g=Random();
        b=Random();
       
        for(double i=maxY;i>=minY-0.003;i-=dy)
        {
            for(double j=minX;j<=maxX;j+=dx)
            {
                double z=getZ(temp3.x,temp3.y,temp3.z,D,j,i);
               
               
                if(isInside(Point(j,i,z),p1,p2,p3))
                {
                    
                    int x=(j-leftX)/dx;
                    int y=(topY-i)/dy;
                    if(z<z_buffer[y][x] && z>far_z && z<near_z)
                    {
                      
                        z_buffer[y][x]=z;  
                        image.set_pixel(x,y,r,g,b);
                
                    }
                   
                }
               
            }
        }
     
    }

    image.save_image("out.bmp");

    for(int i=0;i<screen_Height;i++)
    {
        for(int j=0;j<screen_Width;j++)
        {
            if(z_buffer[i][j]<z_max)
            {
                fout << setprecision(6) << fixed << z_buffer[i][j] << "\t";
            }
           
        }
        fout<<endl;
    }

    fin.close();
    fout.close();

    z_buffer.clear();
    z_buffer.shrink_to_fit();

}

int main()
{

    stage1();
    stage2();
    stage3();
    stage4();

   

    
    return 0;

}