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



#include <Packages/rtrt/Core/Object.h>
#include <Packages/rtrt/Core/Array1.h>
#include <Packages/rtrt/Core/Ray.h>
#include <Packages/rtrt/Core/UVPlane.h>
#include <Packages/rtrt/Core/HitInfo.h>
#include <Packages/rtrt/Core/Material.h>
#include <Packages/rtrt/Core/Names.h>

#include <sgi_stl_warnings_off.h>
#include <iostream>
#include <string>
#include <sgi_stl_warnings_on.h>

using namespace rtrt;
using namespace SCIRun;
using std::cerr;
using std::endl;

static UVPlane default_mapping(Point(0,0,0), Vector(1,0,0), Vector(0,1,0));
// initialize the static member type_id
PersistentTypeID Object::type_id("Object", "Persistent", 0);

Object::Object(Material* matl, UVMapping* uv)
  : matl(matl), uv(uv), animGrid(0), was_preprocessed(false)
{
  if(!uv)
    this->uv=&default_mapping;
}

Object::~Object()
{
}

void Object::animate(double, bool&)
{
}

void Object::collect_prims(Array1<Object*>& prims)
{
  prims.add(this);
}

void Object::preprocess(double, int&, int& /*scratchsize*/)
{
  //  scratchsize=Max(scratchsize, matl->get_scratchsize());
}

void Object::print(ostream& out)
{
  out << "Unknown object: " << this << '\n';
}

void Object::light_intersect(Ray& ray, HitInfo& hit, Color&,
			     DepthStats* st, PerProcessorContext* ppc)
{
  intersect(ray, hit, st, ppc);
}

void Object::softshadow_intersect(Light*, Ray& ray,
				  HitInfo& hit, double, Color& atten,
				  DepthStats* st, PerProcessorContext* ppc)
{
  light_intersect(ray, hit, atten, st, ppc);
}

void Object::multi_light_intersect(Light*, const Point& orig,
				   const Array1<Vector>& dirs,
				   const Array1<Color>& attens,
				   double dist,
				   DepthStats* st, PerProcessorContext*)
{
  for(int i=0;i<dirs.size();i++){
    if(attens[i].luminance() != 0){
      Color atten;
      Ray ray(orig, dirs[i]);
      HitInfo hit;
      intersect(ray, hit, st, 0);
      if(hit.was_hit && hit.min_t < dist)
	atten = Color(0,0,0);
      else
	atten=Color(1,1,1);
      attens[i]=atten;
    }
  }
}

void Object::remove(Object*, const BBox&)
{
    cerr << "Object::remove should never be called!" << number << "\n";
}

void Object::insert(Object*, const BBox&)
{
    cerr << "Object::insert should never be called!\n";
}

void Object::allow_animation ()
{
}

void Object::disallow_animation ()
{
}

void Object::rebuild ()
{
}

void Object::recompute_bbox ()
{
}

void Object::set_scene (Object *)
{
}

void Object::update(const Vector&)
{
}

const int OBJECT_VERSION = 1;
void 
Object::io(SCIRun::Piostream &str)
{
  str.begin_class("Object", OBJECT_VERSION);
  SCIRun::Pio(str, matl);
  SCIRun::Pio(str, uv);
  str.end_class();
}

const string& Object::get_name() const
{
  return Names::getName(this);
}

void Object::set_name(const string &s)
{
  Names::nameObject(s, this);
}

namespace SCIRun {
void Pio(SCIRun::Piostream& stream, rtrt::Object*& obj)
{
  SCIRun::Persistent* p;
  if (stream.reading()) {
    stream.io(p, rtrt::Object::type_id);
    obj=dynamic_cast<rtrt::Object*>(p);
  } else {
    p = obj;
    stream.io(p, rtrt::Object::type_id);
  }
}
} // end namespace SCIRun
