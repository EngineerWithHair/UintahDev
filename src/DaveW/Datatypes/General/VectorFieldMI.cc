/*
 *  VectorFieldMI.cc
 *
 *  Written by:
 *   Robert Van Uitert
 *   Department of Computer Science
 *   University of Utah
 *   June 1999
 *
 *  Copyright (C) 1999 SCI Group
 */

#include <DaveW/Datatypes/General/VectorFieldMI.h>
#include <SCICore/Geometry/Vector.h>
#include <SCICore/Util/NotFinished.h>
#include <SCICore/Malloc/Allocator.h>
#include <iostream.h>

namespace DaveW {
namespace Datatypes {

using SCICore::Containers::Array1;
using SCICore::Geometry::Cross;
using namespace SCICore::Datatypes;

static Persistent* make_VectorFieldMI()
{
    return scinew VectorFieldMI();
}

PersistentTypeID VectorFieldMI::type_id("VectorFieldMI", "VectorField", make_VectorFieldMI);


VectorFieldMI::VectorFieldMI():VectorField(RegularGrid)
{ 
}

VectorFieldMI::VectorFieldMI(VectorField* vf):VectorField(RegularGrid)
{
  field = vf;

  VectorFieldUG* ug = dynamic_cast<VectorFieldUG*> (field);  

  if(ug != NULL) {

    MeshHandle mesh = ((VectorFieldUG*)(field))->mesh;
    
    nelems=mesh->elems.size();

    interp.resize(nelems);
    vol.resize(nelems);
    centro.resize(nelems);

    for (int j=0; j<nelems; j++) {
      
      Vector valueJ;
      Element* e=mesh->elems[j];
      Point centroid = e->centroid();    
      int jj = j;
      ((VectorFieldUG*)(field))->interpolate(centroid,valueJ,jj);
      interp[j] = valueJ;
      vol[j] = e->volume();
      centro[j] = e->centroid();
    }
  
  }
  
}

VectorFieldMI::~VectorFieldMI()
{
}

int VectorFieldMI::interpolate(const Point& p, Vector& value)
{

  value = Vector(0,0,0);

  VectorFieldRG* rg = dynamic_cast<VectorFieldRG*> (field);  

  if(rg != NULL) {

    int nx = ((VectorFieldRG*)field)->nx;
    int ny = ((VectorFieldRG*)field)->ny;
    int nz = ((VectorFieldRG*)field)->nz;

    for (int i=0; i<nx; i++) {
      for (int j=0; j<ny;j++) {
	for (int k=0; k<nz;k++) {
	
	  Vector valueJ = ((VectorFieldRG*)(field))->grid(i,j,k);
	  Point centroid = ((VectorFieldRG*)(field))->get_point(i,j,k);

	  Vector radius = p - centroid;
	  Vector valueJXR = Cross(valueJ,radius);
	  int length = radius.length();
	  
	  Point maxPt, minPt;
	  field->get_bounds(maxPt, minPt);
	  Vector v = Vector(maxPt-minPt);

	  double volume = (v.x()*v.y()*v.z())/(nx*ny*nz);
	  value += (valueJXR/(length*length*length)) * volume;
	}
      }
    }

  } else {  //Unstructured Grid

    MeshHandle mesh = ((VectorFieldUG*)(field))->mesh;

    for (int j=0; j<nelems; j++) {

      if (!(mesh->inside(p,mesh->elems[j]))) {

	Point centroid = centro[j];
	Vector valueJ = interp[j];
	
	Vector radius = p - centroid;
	Vector valueJXR = Cross(valueJ,radius);
	double length = radius.length();

	value += (valueJXR/(length*length*length)) * vol[j];

      }
    }    
  }
  return 1;
}

#define VECTORFIELDMI_VERSION 1

void VectorFieldMI::io(Piostream& stream)
{
  using SCICore::PersistentSpace::Pio;
  using SCICore::Containers::Pio;    
  using DaveW::Datatypes::Pio;

  VectorFieldRG* rg = dynamic_cast<VectorFieldRG*> (field);
  
  if(rg != NULL) {
    stream.begin_class("VectorFieldMI", VECTORFIELDMI_VERSION);
    // Do the base class first...
    VectorField::io(stream);
    if (field == NULL) field = new VectorFieldRG();
    Pio(stream,*field);
    stream.end_class();
  } else {  //Unstructured Grid
    stream.begin_class("VectorFieldMI", VECTORFIELDMI_VERSION);
    // Do the base class....
    VectorField::io(stream);
    if (field == NULL) {
      field = new VectorFieldUG(VectorFieldUG::ElementValues);
      Pio(stream,*field);

      MeshHandle mesh = ((VectorFieldUG*)(field))->mesh;
      
      nelems=mesh->elems.size();
      
      vol.resize(nelems);
      centro.resize(nelems);
      
      for (int j=0; j<nelems; j++) {
	
	Element* e=mesh->elems[j];
	vol[j] = e->volume();
	centro[j] = e->centroid();
      }
      Pio(stream,interp);
      stream.end_class();
    } else {
      Pio(stream,*field);
      Pio(stream,interp);
      stream.end_class();
    }
  }

}

int VectorFieldMI::interpolate(const Point& p, Vector& value, 
			       int& /*a*/, int /*b*/)
{
  return interpolate(p, value);
}

void VectorFieldMI::compute_bounds()
{

  if (have_bounds || field == 0) return;
  field->get_bounds(bmin,bmax);
  have_bounds = 1;
 
}

void VectorFieldMI::get_boundary_lines(Array1<Point>& lines)
{
  VectorFieldRG* rg = dynamic_cast<VectorFieldRG*> (field);
  
  if(rg != NULL) {
    ((VectorFieldRG*)(field))->get_boundary_lines(lines);
  } else {
    ((VectorFieldUG*)(field))->get_boundary_lines(lines);
  }

}

VectorField* VectorFieldMI::clone()
{
  VectorFieldRG* rg = dynamic_cast<VectorFieldRG*> (field);
  
  if(rg != NULL) {
    return ((VectorFieldRG*)(field))->clone();
  }
  return ((VectorFieldUG*)(field))->clone();
 
}

MeshHandle VectorFieldMI::getMesh()  {
   VectorFieldUG* ug = dynamic_cast<VectorFieldUG*> (field);
   
   if (ug != NULL) {
    return(((VectorFieldUG*)(field))->mesh);
   }
   return(NULL);
}
} // End namespace Datatypes
} // End namespace DaveW

//
// $Log$
// Revision 1.1  1999/09/02 04:45:51  dmw
// magnetic field
//
