/*

The MIT License

Copyright (c) 1997-2009 Center for the Simulation of Accidental Fires and 
Explosions (CSAFE), and  Scientific Computing and Imaging Institute (SCI), 
University of Utah.

License for the specific language governing rights and limitations under
Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

*/


#include <Packages/rtrt/Core/SolidNoise3.h>

using namespace rtrt;
using namespace SCIRun;

SolidNoise3::SolidNoise3()
{
   Vector v;
   bool accept;

   for (int i = 0; i < 256; i++)
   {
      phi[i] = i;
      accept = false;
      while (!accept)
      {
         v.Set( 2*drand48()-1,2*drand48()-1,2*drand48()-1);
         if (Dot(v,v) <= 1.0) accept = true;
      }
      v.normalize();
      grad[i] = v;
   }
   Permute(phi, 256);
}


double SolidNoise3::turbulence(const Point& p, int depth)
const
{
   double sum;
   double weight;
   Point ptemp;

   sum = 0.0;
   weight = 1.0;
   ptemp = p;
   double amp = 1;

   for (int i = 0; i < depth; i++)
   {
       sum += fabs(noise(ptemp)) * amp;
       weight = weight * 2;
       amp = amp / 2;

       ptemp.x((p.x() * weight));
       ptemp.y((p.y() * weight));
       ptemp.z((p.z() * weight));
   }
   return sum;
}

double SolidNoise3::dturbulence(const Point& p, int depth, double d)
const {
   double sum;
   double weight;
   Point ptemp;

   sum = 0.0;
   weight = 1.0;
   ptemp = p;

   for (int i = 0; i < depth; i++)
   {
       sum += fabs(noise(ptemp)) / d;
       weight = weight * d;
       ptemp.x(p.x() * weight);
       ptemp.y(p.y() * weight);
       ptemp.z(p.z() * weight);
   }
   return sum;
}

Vector SolidNoise3::vectorTurbulence(const Point& p, int depth)
const {
   Vector sum(0.0, 0.0, 0.0);
   Point ptemp = p + Vector(5.1, 56.2, 25.8);
   double amp = 1.0;
   Vector off(0.321, 0.112, 0.724);

   for (int i = 0; i < depth; i++)
   {
       sum += vectorNoise(ptemp) * amp;
       amp = amp * 0.6;
       ptemp.x(ptemp.x() * 2 + off.x());
       ptemp.y(ptemp.y() * 2 + off.y());
       ptemp.z(ptemp.z() * 2 + off.z());
   }
   return sum;
}

Vector SolidNoise3::vectorNoise(const Point& p) const {
    int i, j, k;
    int fi, fj, fk;
    Vector v;
    Vector sum(0.0, 0.0, 0.0);

    fi = int(floor(p.x()));
    fj = int(floor(p.y()));
    fk = int(floor(p.z()));
    for (i = fi; i <= fi+1; i++)
       for (j = fj; j <= fj+1; j++)
          for (k = fk; k <= fk+1; k++) {
               v.Set(p.x() - i, p.y() - j, p.z() - k);
               sum += vectorKnot(i, j, k, v);
          }
    return sum;
    
}

double SolidNoise3::noise(const Point& p) const {
    int fi, fj, fk;
    double sum;
    Vector v;

    fi = int(floor(p.x()));
    fj = int(floor(p.y()));
    fk = int(floor(p.z()));
/*
    sum = 0.0;
    for (int i = fi; i <= fi+1; i++)
       for (int j = fj; j <= fj+1; j++)
          for (int k = fk; k <= fk+1; k++)
          {
               v.Set(p.x() - i, p.y() - j, p.z() - k);
               sum += knot(i, j, k, v);
          }
*/
    sum = 0.0;

    v.Set(p.x() - fi, p.y() - fj, p.z() - fk);
    sum += knot(fi, fj, fk, v);

    v.Set(p.x() - fi - 1, p.y() - fj, p.z() - fk);
    sum += knot(fi + 1, fj, fk, v);

    v.Set(p.x() - fi, p.y() - fj - 1, p.z() - fk);
    sum += knot(fi, fj + 1, fk, v);

    v.Set(p.x() - fi, p.y() - fj, p.z() - fk -1);
    sum += knot(fi, fj, fk + 1, v);

    v.Set(p.x() - fi -1, p.y() - fj -1, p.z() - fk);
    sum += knot(fi + 1, fj + 1, fk, v);

    v.Set(p.x() - fi -1, p.y() - fj, p.z() - fk -1);
    sum += knot(fi + 1, fj, fk + 1, v);

    v.Set(p.x() - fi, p.y() - fj -1, p.z() - fk -1);
    sum += knot(fi, fj + 1, fk + 1, v);

    v.Set(p.x() - fi -1, p.y() - fj -1, p.z() - fk -1);
    sum += knot(fi + 1, fj + 1, fk + 1, v);
    
    return sum;
    
}
