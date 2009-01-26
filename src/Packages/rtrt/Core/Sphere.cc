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



#include <Packages/rtrt/Core/Sphere.h>
#include <Packages/rtrt/Core/HitInfo.h>
#include <Packages/rtrt/Core/Ray.h>
#include <Packages/rtrt/Core/Light.h>
#include <Packages/rtrt/Core/BBox.h>
#include <Packages/rtrt/Core/Stats.h>
#include <Packages/rtrt/Core/TrivialAllocator.h>

#include <sgi_stl_warnings_off.h>
#include <iostream>
#include <sgi_stl_warnings_on.h>

using namespace rtrt;
using namespace SCIRun;

Persistent* sphere_maker() {
  return new Sphere();
}

// initialize the static member type_id
PersistentTypeID Sphere::type_id("Sphere", "Object", sphere_maker);


#if 0
static TrivialAllocator Sphere_alloc(sizeof(Sphere));

void* Sphere::operator new(size_t)
{
    return Sphere_alloc.alloc();
}

void Sphere::operator delete(void* rp, size_t)
{
    Sphere_alloc.free(rp);
}
#endif

Sphere::Sphere(Material* matl, const Point& cen, double radius)
    : Object(matl), cen(cen), radius(radius)
{
}

Sphere::~Sphere()
{
}

void Sphere::intersect(Ray& ray, HitInfo& hit, DepthStats* st,
		       PerProcessorContext*)
{
    Vector OC=cen-ray.origin();
    double tca=Dot(OC, ray.direction());
    double l2oc=OC.length2();
    double rad2=radius*radius;
    st->sphere_isect++;
    if(l2oc <= rad2){
	// Inside the sphere
	double t2hc=rad2-l2oc+tca*tca;
	double thc=sqrt(t2hc);
	double t=tca+thc;
	hit.hit(this, t);
	st->sphere_hit++;
	return;
    } else {
	if(tca < 0.0){
	    // Behind ray, no intersections...
	    return;
	} else {
	    double t2hc=rad2-l2oc+tca*tca;
	    if(t2hc <= 0.0){
		// Ray misses, no intersections
		return;
	    } else {
		double thc=sqrt(t2hc);
		hit.hit(this, tca-thc);
		hit.hit(this, tca+thc);
		st->sphere_hit++;
		return;
	    }
	}
    }	
}

// Maybe this could be improved - steve
void Sphere::light_intersect(Ray& ray, HitInfo& hit, Color&,
			     DepthStats* st, PerProcessorContext*)
{
  Vector OC=cen-ray.origin();
  double tca=Dot(OC, ray.direction());
  double l2oc=OC.length2();
  double rad2=radius*radius;
  st->sphere_light_isect++;
  if(l2oc <= rad2){
    // Inside the sphere
    double t2hc=rad2-l2oc+tca*tca;
    double thc=sqrt(t2hc);
    double t=tca+thc;
    hit.shadowHit(this, t);
    st->sphere_light_hit++;
    return;
  } else {
    if(tca < 0.0){
      // Behind ray, no intersections...
      return;
    } else {
      double t2hc=rad2-l2oc+tca*tca;
      if(t2hc <= 0.0){
	// Ray misses, no intersections
	return;
      } else {
	double thc=sqrt(t2hc);
	hit.shadowHit(this, tca-thc);
	hit.shadowHit(this, tca+thc);
	st->sphere_light_hit++;
	return;
      }
    }
  }	
}

Vector Sphere::normal(const Point& hitpos, const HitInfo&)
{
    Vector n=hitpos-cen;
    n*=1./radius;
    return n;
}

void Sphere::softshadow_intersect(Light* light, Ray& ray, HitInfo& hit,
				  double dist, Color& atten, DepthStats* st,
				  PerProcessorContext*)
{
    Vector OC=cen-ray.origin();
    double tca=Dot(OC, ray.direction());
    st->sphere_light_isect++;
    if(tca<1.e-4)
	return;

    double delta=light->radius*tca/dist;
    double Ri=radius;
    double Ro=radius+delta;
    double Ri2=Ri*Ri;
    double Ro2=Ro*Ro;

    Vector normal=ray.direction()*tca-OC;
    double d2=normal.length2();

    if(d2>Ro2){
	return;
    }

    if(d2<Ri2){
	atten=Color(0,0,0);
	st->sphere_light_hit++;
	hit.hit(this, 1);
	return;
    }



    st->sphere_light_penumbra++;
    double t=(sqrt(d2)-Ri)/(delta);
    double g=3*t*t-2*t*t*t;

    atten=g<atten.luminance()?Color(g,g,g):atten;
}

void Sphere::multi_light_intersect(Light*, const Point& orig,
				   const Array1<Vector>& dirs,
				   const Array1<Color>& attens,
				   double,
				   DepthStats*, PerProcessorContext*)
{
    Vector OC=orig-cen;
    double rad2=radius*radius;
    double C=OC.length2()-rad2;
    for(int i=0;i<dirs.size();i++){
	const Vector& dir=dirs[i];
	double B=Dot(dir, OC);
	double B2=B*B;
	if(B2 > C){
	    double disc=sqrt(B2-C); // A=1
	    double t=(-B+disc);
	    if(t>1.e-6)
		attens[i]=Color(0,0,0);
	}
    }
}

void Sphere::compute_bounds(BBox& bbox, double offset)
{
    bbox.extend(cen, radius+offset);
}

void Sphere::print(ostream& out)
{
    out << "Sphere: cen=" << cen << ", radius=" << radius << '\n';
}

void
Sphere::updatePosition( const Point & pos )
{
  cen = pos;
}

void
Sphere::updateRadius( double new_radius ) {
  if (new_radius < 0) {
    new_radius = 0;
  }
  radius = new_radius;
}

const int SPHERE_VERSION = 1;

void 
Sphere::io(SCIRun::Piostream &str)
{
  str.begin_class("Sphere", SPHERE_VERSION);
  Object::io(str);
  Pio(str, cen);
  Pio(str, radius);
  str.end_class();
}

namespace SCIRun {
void Pio(SCIRun::Piostream& stream, rtrt::Sphere*& obj)
{
  SCIRun::Persistent* pobj=obj;
  stream.io(pobj, rtrt::Sphere::type_id);
  if(stream.reading()) {
    obj=dynamic_cast<rtrt::Sphere*>(pobj);
    //ASSERT(obj != 0)
  }
}
} // end namespace SCIRun
