#include<bits/stdc++.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;
using namespace glm;



class point{
    public:
	double x,y,z;
    point(double x=0, double y=0, double z=0):x(x),y(y),z(z){}
    point(vec3 g):x(g.x),y(g.y),z(g.z){}
    double r(){ return sqrt(x*x+y*y+z*z); }
    point unit(){ return point(x/r(),y/r(),z/r()); }
    point operator + (const point& dt){ return point(x+dt.x,y+dt.y,z+dt.z); }
    point operator - (const point& dt){ return point(x-dt.x,y-dt.y,z-dt.z); }
    double operator *(const point& dt){ return x*dt.x+y*dt.y+z*dt.z; }
    point operator * (const double u) { return point(x*u,y*u,z*u); }
    point operator ^(const point& dt){ return point(y*dt.z-z*dt.y,z*dt.x-x*dt.z,x*dt.y-y*dt.x); }
    point reflect(point a){ point b(x,y,z); return b+b-a; }
    double operator |(const point& dt){ return point(dt.x-x,dt.y-y,dt.z-z).r(); }
    friend ostream& operator<<(ostream& os, const point& dt);
    point bisector(point a, point b, double r = 0.5){
        point o(x,y,z);
        a = a-o;
        b = b-o;
        point c = a*(1.0-r)+b*r;
        c = c.unit();
        c = c*((a.r()+b.r())/2);
        return o+c;
    }
    point renormal(point n){
        point p(x,y,z);
        point b = n^p;
        point q = b^n;
        return q;
    }
};
ostream& operator<<(ostream& os, const point& dt){ os << setprecision(4)<< "("<< dt.x<< ","<< dt.y<< ","<< dt.z<< ")"; return os; }


point rotatePointAroundVector(point P, point N, float a) {
    vec3 p(P.x,P.y,P.z), n(N.x,N.y,N.z);
    n = normalize(n); // Normalize the axis
    float radian = radians(a); // Convert angle to radians
    mat4 rotationMatrix = rotate(mat4(1.0f), radian, n);
    vec4 rotatedPoint = rotationMatrix * vec4(p, 1.0f);
    return point(vec3(rotatedPoint));
}

int main() {
    // Define the point, axis, and angle
    point P(1, 1, 1);
    point Z(0, 0, 1); // Axis Z
    float angle = 45; // Angle in degrees

    // Rotate the point around the axis
    point rotatedP = rotatePointAroundVector(P, Z, angle);

    // Output the result
    cout << "Rotated Point: " << rotatedP << std::endl;

    return 0;
}