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



#ifndef CUTGROUP_H
#define CUTGROUP_H 1

#include <Packages/rtrt/Core/BBox.h>
#include <Packages/rtrt/Core/Group.h>
#include <Core/Geometry/Point.h>
#include <Core/Geometry/Vector.h>
#include <Packages/rtrt/Core/Array1.h>
#include <Packages/rtrt/Core/CutPlaneDpy.h>
#include <Packages/rtrt/Core/Material.h>

//This is a replacement for the buggy CutPlane object.

//It is a group so that related objects, like the parts of the visible woman, can be
//treated together.
//Multiple cutting planes in a scene are fine, multiple cutting planes on the same
//object are not allowed.


//see also, CutVolumeDpy, CutMaterial, CutPlaneDpy, and ColorMap.

namespace rtrt {

#define CUTGROUPDIST 48
#define CUTGROUPPTR 56
//Note, this uses 
//8 bytes at HitInfo.scratchpad+CUTGROUPDIST for CutPlane distance and
//4 bytes at HitInfo.scratchpad+CUTGROUPPTR for CutGroup detection
//If your scene uses them for something else and also has cutgroups in it there will be
//conflicts.

class CutGroup : public Group, public Material {
  double d;
  CutPlaneDpy* dpy;
  bool on;
  bool animdpy;
public:
  Vector n;
  CutGroup(const Vector& n, const Point& cen);
  CutGroup(CutPlaneDpy *dpy, bool animdpy=false);

  virtual ~CutGroup();

  virtual void io(SCIRun::Piostream &stream);

  virtual void intersect(Ray& ray, HitInfo& hit, DepthStats* st,
			 PerProcessorContext*);
  virtual void sub_intersect(Ray& ray, HitInfo& hit, DepthStats* st,
			     PerProcessorContext*);
  virtual void light_intersect(Ray& ray, HitInfo& hit, Color& atten,
			       DepthStats* st, PerProcessorContext* ppc);
  virtual void softshadow_intersect(Light* light, Ray& ray, HitInfo& hit,
				    double dist, Color& atten, DepthStats* st,
				    PerProcessorContext* ppc);
  virtual void multi_light_intersect(Light* light, const Point& orig,
				     const Array1<Vector>& dirs,
				     const Array1<Color>& attens,
				     double dist,
				     DepthStats* st, PerProcessorContext* ppc);
  virtual void animate(double t, bool& changed);
  virtual void collect_prims(Array1<Object*>& prims);
  virtual void toggleOn() { 
    on = !on; 
    if (dpy) {
      dpy->on = on; 
      dpy->redisplay();
    }
  };
  virtual void toggleAnimate() { animdpy = !animdpy; };

  virtual bool interior_value(double& ret_val, const Ray &ref, const double t);


  virtual void shade(Color& result, const Ray& ray,
		     const HitInfo& hit, int depth,
		     double atten, const Color& accumcolor,
		     Context* cx);

  virtual Vector normal(const Point&, const HitInfo& hit);
};


} // end namespace rtrt

#endif
