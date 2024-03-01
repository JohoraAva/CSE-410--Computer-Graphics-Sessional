#include<bits/stdc++.h>
// #include "Point.cpp"
#include "Matrix.cpp"
using namespace std;



ifstream fin("scene.txt");
ofstream fout("stage1.txt");
Point eye,look,up;
double fovY,aspectRatio,near,far;
int screenPixelWidth,screenPixelHeight;


int main()
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

    while(fin>>command)
    {
        if(command=="triangle")
        {
            Point a,b,c;
            fin>>a.x>>a.y>>a.z;
            fin>>b.x>>b.y>>b.z;
            fin>>c.x>>c.y>>c.z;

            Matrix m(4,4);
            m.valueAssign(a,b,c);
            m=stk.top()*m;
            fout<<command<<endl;
            fout<<m.mat[0][0]<<" "<<m.mat[1][0]<<" "<<m.mat[2][0]<<endl;
            fout<<m.mat[0][1]<<" "<<m.mat[1][1]<<" "<<m.mat[2][1]<<endl;
            fout<<m.mat[0][2]<<" "<<m.mat[1][2]<<" "<<m.mat[2][2]<<endl;
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
            stk.push(stk.top()*m);
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
            stk.push(stk.top()*m);
        }
        else if(command=="rotate")
        {
            double angle,rx,ry,rz;
            fin>>angle>>rx>>ry>>rz;
            Matrix m(4,4);
            m.identity();
            double rad=(angle*PI)/180.0;
            m.mat[0][0]=cos(rad)+(rx*rx)*(1-cos(rad));
            m.mat[0][1]=rx*ry*(1-cos(rad))-rz*sin(rad);
            m.mat[0][2]=rx*rz*(1-cos(rad))+ry*sin(rad);
            m.mat[1][0]=ry*rx*(1-cos(rad))+rz*sin(rad);
            m.mat[1][1]=cos(rad)+(ry*ry)*(1-cos(rad));
            m.mat[1][2]=ry*rz*(1-cos(rad))-rx*sin(rad);
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
    else
    {
        cout<<"Invalid command"<<endl;
    }



    //
    fin.close();
    fout.close();

    // fin.open("stage1.txt");
    // fout.open("stage2.txt");


    return 0;


}

}