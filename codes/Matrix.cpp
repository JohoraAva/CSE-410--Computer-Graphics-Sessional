#include<bits/stdc++.h>
#include "Point.cpp"

using namespace std;
#define PI 3.14159265

class Matrix
{
    public:
    double** mat;
    int row,col;
    Matrix(int m,int n)
    {
        this->row=m;
        this->col=n;
        mat=new double*[m];
        for(int i=0;i<m;i++)
        {
            mat[i]=new double[n];
        }
    }
    void valueAssign(Point a ,Point b ,Point c) //00 10 20 01 11 21 02 12 22
    {
        mat[0][0]=a.x;
        mat[1][0]=a.y;
        mat[2][0]=a.z;
        mat[0][1]=b.x;
        mat[1][1]=b.y;
        mat[2][1]=b.z;
        mat[0][2]=c.x;
        mat[1][2]=c.y;
        mat[2][2]=c.z;
    }

    Matrix identity()
    {
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                if(i==j)
                {
                    this->mat[i][j]=1;
                }
                else
                {
                    this->mat[i][j]=0;
                }
            }
        }
        return *this;
    }
    Matrix transpose()
    {
        Matrix res(this->row,this->col);
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                res.mat[i][j]=this->mat[j][i];
            }
        }
        return res;
    }

    Matrix operator*(Matrix m)
    {
        Matrix res(this->row,m.col);
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                res.mat[i][j]=0;
                for(int k=0;k<this->col;k++)
                {
                    res.mat[i][j]+=this->mat[i][k]*m.mat[k][j];
                }
            }
        }
        return res;
    }

 
    double determinant()
    {
        double res=0;
        if(this->row==1)
        {
            return this->mat[0][0];
        }
        else if(this->row==2)
        {
            return this->mat[0][0]*this->mat[1][1]-this->mat[0][1]*this->mat[1][0];
        }
        else
        {
            for(int i=0;i<this->row;i++)
            {
                Matrix temp(this->row-1,this->col-1);
                for(int j=1;j<this->row;j++)
                {
                    int k=0;
                    for(int l=0;l<this->col;l++)
                    {
                        if(l==i)
                        {
                            continue;
                        }
                        temp.mat[j-1][k]=this->mat[j][l];
                        k++;
                    }
                }
                res+=pow(-1,i)*this->mat[0][i]*temp.determinant();
            }
        }
        return res;
    }

    double cofactor(int r,int c)
    {
        Matrix temp(this->row-1,this->col-1);
        int tempRow=0,tempCol=0;
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                if(i!=r && j!=c)
                {
                    temp.mat[tempRow][tempCol++]=this->mat[i][j];
                    if(tempCol==this->row-1)
                    {
                        tempCol=0;
                        tempRow++;
                    }
                }
            }
        }
        return pow(-1,r+c)*temp.determinant();
    }

    Matrix adjoint()
    {
        Matrix res(this->row,this->col);
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->row;j++)
            {
                res.mat[i][j]=this->cofactor(i,j);
            }
        }
        return res.transpose();
    }

    Matrix inverse()
    {
        Matrix res(this->row,this->col);
        double det=this->determinant();
        if(det==0)
        {
            cout<<"Inverse does not exist\n";
            return res;
        }
        res=this->adjoint();
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->row;j++)
            {
                res.mat[i][j]/=det;
            }
        }
        return res;
    }
    void print()
    {
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<this->col;j++)
            {
                cout<<this->mat[i][j]<<" ";
            }
            cout<<"\n";
        }
    }

   Matrix pointToMatrix(Point p)
   {    
     
       Matrix res(3,1);
       res.mat[0][0]=p.x;
       res.mat[1][0]=p.y;
       res.mat[2][0]=p.z;   
    //    cout<<"Point to Matrix\n";
    //    res.print();    
       return res;
   }

//    Matrix multiplyPoint(Point p)
//    {
//         Matrix mat2=pointToMatrix(p);
//         Matrix res=*this*mat2;
//         return res;
//    }
    Matrix operator*(Point p)
    {
        Matrix mat2=pointToMatrix(p);
        Matrix res=*this*mat2;
        return res;
    }

};


Point rotate(Point p,Point n,double angle)//n vector, p point
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
// int main()
// {
//     //check 
//     Matrix m(3,3);
//     Point a(1,2,3);
//     Point b(4,8,6);
//     Point c(7,8,9);

//     m.valueAssign(a,b,c);
//     cout<<"Matrix\n";
//     m.print();


//     Matrix m1(3,3);
//     Point a1(1,2,3);
//     Point b1(4,-5,6);
//     Point c1(7,8,9);

//     m1.valueAssign(a1,b1,c1);
//     m1.print();
//     m1=m1*m;

//     m1.print();

//     //multiply point
//     // Point p(1,2,3);

//     // Matrix res=m*p;
//     // res.print();

//     // cout<<"det= "<<m.determinant()<<"\n";
//     // cout<<"det= "<<m1.determinant()<<"\n";
//     // m1=m1.inverse();
//      // m1.print();

//     //check rotation
//      Point p(0,-1,0);
//     for(int i=0;i<13;i++)
//     {
       
//         cout<<"i= "<<i<<endl;
//         p.print();
//         Point n(-1,0,1);
//         double angle=PI/6;
//         p=rotate(p,n,angle);

//     }
//     // Point p1(1,0,0);
//     // Point n(0,0,1);
//     // double angle=PI/4.0;
//     // Matrix res1=rotate(p1,n,angle);
//     // res1.print();
   
// }