/*
 *  IsoSurfaceSAGE.cc 
 *      View Depended Iso Surface Extraction
 *      for Structures Grids (Bricks)
 *  Written by:
 *   Yarden Livnat
 *   Department of Computer Science
 *   University of Utah
 *   Dec 1996
 *
 *  Copyright (C) 1996 SCI Group
 */


#include <stdio.h>
#include <unistd.h>

#include <SCICore/Thread/Time.h>
#include <SCICore/Containers/String.h>
#include <PSECore/Dataflow/Module.h>
#include <SCICore/Datatypes/ScalarFieldRGdouble.h>
#include <SCICore/Datatypes/ScalarFieldRGfloat.h>
#include <SCICore/Datatypes/ScalarFieldRGshort.h>
#include <PSECore/Datatypes/ScalarFieldPort.h>
#include <PSECore/Datatypes/ColorMapPort.h>
#include <PSECore/Datatypes/GeometryPort.h>
#include <PSECore/Datatypes/GeometryComm.h>
#include <PSECore/Datatypes/SurfacePort.h>
#include <SCICore/Thread/Thread.h>

#include <SCICore/Geom/Color.h>

#include <SCICore/TclInterface/TCLTask.h>
#include <SCICore/TclInterface/TCLvar.h>
#include <SCICore/TclInterface/TCL.h>
#include <tcl.h>
#include <tk.h>

#include <SCICore/Geom/Material.h>
#include <SCICore/Geom/GeomTriangles.h>
#include <SCICore/Geom/View.h>
#include <SCICore/Geom/GeomGroup.h>
#include <SCICore/Geom/GeomObj.h>
#include <SCICore/Geom/GeomTri.h>
#include <SCICore/Geom/GeomLine.h>
#include <SCICore/Geom/GeomBox.h>
#include <SCICore/Geom/Pt.h>
#include <SCICore/Geom/GeomTransform.h>
#include <SCICore/Geometry/Point.h>
#include <SCICore/Geometry/Transform.h>
#include <SCICore/Geom/BBoxCache.h>
#include <SCICore/Malloc/Allocator.h>
#include <SCICore/Geometry/Vector.h>
#include <SCICore/Math/Trig.h>

#include <PSECommon/Algorithms/Visualization/Sage.h>

#include <math.h>
#include <iostream>
#include <sstream>
#include <values.h>



namespace PSECommon {
  namespace Modules {
    
    using namespace PSECore::Dataflow;
    using namespace PSECore::Datatypes;
    using namespace SCICore::Containers;
    using namespace SCICore::GeomSpace;
    using namespace SCICore::Geometry;
    using namespace SCICore::Math;
    using namespace SCICore::TclInterface;
    using namespace SCICore::Thread;
    using namespace PSECommon::Algorithms;
    
    
    SysTime::SysClock extract_timer, vis_timer;

    int bbox_count;
    int debug = 1;
    int scan_yes;
    int scan_no;
  

    GeometryData *gd;

    //
    // IsoSurfaceSAGE
    //

    class IsoSurfaceSAGE : public Module 
    {
      // input
      ScalarFieldIPort* infield;  // input scalar fields (bricks)
      ScalarFieldIPort* incolorfield;
      ColorMapIPort* incolormap;
      //      GeometryIPort* igeom;       // input from salmon - view point

      // ouput
      GeometryOPort* ogeom;       // input from salmon - view point

      // UI variables
      TCLdouble isoval;
      TCLdouble isoval_min, isoval_max;
      TCLint tcl_bbox, tcl_value,tcl_visibility, tcl_scan;
      TCLint tcl_reduce, tcl_all;
      TCLint tcl_min_size, tcl_poll;
  
      int value, bbox_visibility, visibility;
      int scan, count_values, extract_all;

      int box_id;
      int surface_id;
      int points_id;

      MaterialHandle bone;
      MaterialHandle flesh;
      MaterialHandle matl;
      MaterialHandle box_matl;
      MaterialHandle points_matl;
  
      GeomPts* points;
      GeomGroup* group;
      GeomObj* topobj;
  
      GeomTrianglesP *triangles;
      GeomMaterial *surface;

      SageBase<Module> *sage;
      Screen *screen;

      Point bmin, bmax;
      int dx, dy, dz, dim;
      int field_generation;
      double iso_value;

      int reduce;
  
      double bbox_limit_x, bbox_limit_y;
      double bbox_limit_x1, bbox_limit_y1;
      double left, right, top, bottom;

      GeometryData *default_gd;
      int xres, yres;

      //int busy;
      //int portid;

    public:
      IsoSurfaceSAGE( const clString& id);
      virtual ~IsoSurfaceSAGE();
      
      virtual void execute();
      //virtual void do_execute();

    private:
      void search();
    };
    
    extern "C" Module* make_IsoSurfaceSAGE(const clString& id)
    {
      return scinew IsoSurfaceSAGE(id);
    }
    
    static clString module_name("IsoSurfaceSAGE");
    static clString box_name("SageBox");
    static clString surface_name("Sage");
    
    IsoSurfaceSAGE::IsoSurfaceSAGE(const clString& id)
      : Module("IsoSurfaceSAGE", id, Filter ), 
      isoval("isoval", id, this),
      isoval_min("isoval_min", id, this), 
      isoval_max("isoval_max", id, this),
      tcl_bbox("bbox", id, this), 
      tcl_value("value", id, this), 
      tcl_visibility("visibility", id, this),
      tcl_scan("scan", id, this),  
      tcl_reduce("reduce",id,this), 
      tcl_all("all",id,this),
      tcl_min_size("min_size", id, this),
      tcl_poll("poll", id, this)
    {
      // input ports
      infield=scinew ScalarFieldIPort(this, "Field",ScalarFieldIPort::Atomic);
      add_iport(infield);
      
      incolorfield=scinew ScalarFieldIPort(this, "Color Field",
					   ScalarFieldIPort::Atomic);
      add_iport(incolorfield);

      incolormap=scinew ColorMapIPort(this,"Color Map",ColorMapIPort::Atomic);
      add_iport(incolormap);
    
//       add_iport(scinew GeometryIPort(this, "Geometry", GeometryIPort::Atomic));

      // output port
      ogeom=scinew GeometryOPort(this, "Geometry", GeometryIPort::Atomic);
      add_oport(ogeom);
  
      // for handling our own input geom
      //busy = 0;
      //have_own_dispatch = 1;

      Color Flesh = Color(1.0000, 0.4900, 0.2500);
      Color Bone = Color(0.9608, 0.8706, 0.7020);
      flesh = scinew Material( Flesh*.1, Flesh*.6, Flesh*.6, 20 );
      bone = scinew Material( Bone*.1, Bone*.6, Bone*.6, 20 );
      box_matl=scinew Material(Color(0.3,0.3,0.3), 
			       Color(.8,.8,.8), 
			       Color(.7,.7,.7), 
			       20);
      points_matl=scinew Material(Color(0.3,0,0), 
				  Color(.8,0,0), 
				  Color(.7,.7,.7), 
				  20);
      surface_id = 0;
      points_id = 0;
      box_id = 0;
      field_generation = -1;

      default_gd = scinew GeometryData;
      default_gd->xres = 512;
      default_gd->yres = 512;
      default_gd->znear = 1;
      default_gd->zfar = 2;
      default_gd->view = scinew View( Point(0.65, 0.5, -4.5),
				      Point(0.5,0.5,0.5),
				      Vector(0,1,0),
				      17 );
      
      screen = new Screen;
      screen->setup( 512, 512 );

      sage = 0;
    }

    IsoSurfaceSAGE::~IsoSurfaceSAGE()
    {
    }

    void
    IsoSurfaceSAGE::execute()
    {
      SysTime::SysClock start = SysTime::currentTicks();

      ScalarFieldHandle scalar_field;

      if(!infield->get(scalar_field)) {
	error("No input field\n");
	return;
      }
      cerr <<"got field\n";

      if ( scalar_field->generation !=  field_generation ) {
	// save a handle to this field
	
	// create a new Sage algorithm
	SageBase<Module> *tmp = SageBase<Module>::make( scalar_field, this );
	if ( !tmp) {
	  error( "Can not work with this type of ScalarField");
	  return;
	}
	
	if ( sage ) delete sage;
	sage = tmp;
	sage->setScreen ( screen );
	
	// save the current generation
 	field_generation = scalar_field->generation;
	
	// reset the tcl
	double min, max;
	scalar_field->get_minmax( min, max );
	isoval_min.set(min);
	isoval_max.set(max);
	isoval.set((min+max)/2);
	reset_vars();

	// send the bbox to Salmon
	scalar_field->get_bounds( bmin, bmax );
	GeomBox *box = scinew GeomBox( bmin, bmax, 1 );
	GeomObj *bbox= scinew GeomMaterial( box, box_matl);
	box_id = ogeom->addObj( bbox, box_name );

	if ( points_id ) {
	  ogeom->delObj(points_id);
	  points_id = 0;
	}
	
	if(surface_id ) {
	  ogeom->delObj(surface_id);
	  surface_id = 0;
	}
	
	return;
      }

      // Get View information from Salmon
      gd = ogeom->getData(0, GEOM_VIEW);
      if ( !gd ) {
	cerr << "using default view" << endl;
	gd = default_gd;
      }

      sage->setView( gd->view, gd->znear, gd->zfar, gd->xres, gd->yres );

      search();

      GeometryData *tmp_gd = ogeom->getData(0, GEOM_VIEW);

      if ( tcl_poll.get() ) {
	if ((*gd->view == *tmp_gd->view) ) 
	  usleep( 50000 );
	want_to_execute();
      }
    
      SysTime::SysClock end = SysTime::currentTicks();
      printf("Exec Timer: %.3f\n", (end-start) *SysTime::secondsPerTick() );
    }
   

    void
    IsoSurfaceSAGE::search()
    {
      //  SysTime::SysClock start = SysTime::currentTicks();
      scan_yes = scan_no = 0;
  
      iso_value = isoval.get();
      value = tcl_value.get();
      scan = tcl_scan.get();
      visibility = tcl_visibility.get();
      bbox_visibility = tcl_bbox.get();
      reduce =  tcl_reduce.get();
      extract_all = tcl_all.get();
      int min_size = tcl_min_size.get() ? 2 : 1;

      sage->setParameters( scan, bbox_visibility, reduce, extract_all,
			   min_size);

      points = scinew GeomPts(2000);
      group=scinew GeomGroup;
      topobj=group;
  
      ScalarFieldHandle colorfield;
      ColorMapHandle cmap;
      int have_colorfield=incolorfield->get(colorfield);
      int have_colormap=incolormap->get(cmap);
      if(have_colormap && !have_colorfield){
	// Paint entire surface based on colormap
	topobj=scinew GeomMaterial(group, cmap->lookup(iso_value));
      } else if(have_colormap && have_colorfield){
	// Nothing - done per vertex
      } else {
	// Default material
	topobj=scinew GeomMaterial(group, iso_value < 800 ? flesh : bone);
      }
      

      // SEARCH 
      screen->clear();
      sage->search(iso_value, group, points);

      // OUTPUT
      if ( points_id ) {
	ogeom->delObj(points_id);
	points_id = 0;
      }
  
      if(surface_id ) 
	ogeom->delObj(surface_id);
  
      if ( group->size() == 0 && points->pts.size() == 0 ) {
	if ( !box_id ) {
	  GeomBox *box = scinew GeomBox( bmin, bmax, 1 );
	  GeomObj *bbox= scinew GeomMaterial( box, box_matl);
	  box_id = ogeom->addObj( bbox, box_name );
	}
      }
      else if ( box_id ) {
	ogeom->delObj(box_id);
	box_id = 0;
      }
      
      if ( group->size() == 0 ) {
	delete group;
	surface_id=0;
	//surface_id2 = 0;
      } else {
	GeomBBoxCache *bbc = scinew GeomBBoxCache( topobj,
						   BBox( bmin, bmax) );
	surface_id=ogeom->addObj( bbc, surface_name ); // , &lock );
      }
      if ( points->pts.size() > 0 ) {
	if ( points->pts.size() > 2000 )
	  printf("NOTE: there are %d points! \n",points->pts.size() );
	points_id =ogeom->addObj( scinew GeomMaterial( points, 
						       iso_value < 800 ? flesh 
						       : bone ),
				  "Dividing Cubes");
      }
    }
    
    //  SysTime::SysClock end = SysTime::currentTicks();			
    //  printf("Scan: %d cells\n", statistics.extracted );
    //   printf("Scan : %d %d\n", scan_yes, scan_no );	
    
    //   printf(" Search Timers: \n\tinit %.3f  \n"
    // 	 "\tsearch %.3f (%.3f  %.3f) \n"
    // 	 "\tall %.3f\n ",
    //    	 (end-start -(end1-start1))*cycleval*1e-9,
    //    	 (end1-start1)*cycleval*1e-9, 
    //    	 vis_timer*cycleval*1e-9, extract_timer*cycleval*1e-9, 
    //    	 (end-start)*cycleval*1e-9);
    


#ifdef COMMENT
void
IsoSurfaceSAGE::do_execute()
{
  using PSECore::Comm::MessageTypes;

  for(;;){
    MessageBase* msg=mailbox.receive();
    GeometryComm* gmsg=(GeometryComm*)msg;
    switch(msg->type){
    case MessageTypes::ExecuteModule:
      execute();
      break;
//     case MessageTypes::TriggerPort:
//       {
// 	Scheduler_Module_Message* smsg=(Scheduler_Module_Message*)msg;
// 	smsg->conn->oport->resend(smsg->conn);
//       }
//       break;
    case MessageTypes::GeometryInit:
      cerr << "got geom Init\n";
      //gmsg->reply->send(GeomReply(portid++, &busy));
      break;	
    case MessageTypes::GeometrySetView:
      //gd = gmsg-view;
      cerr << "got geom set view\n";
      break;
    case MessageTypes::GeometryAddObj:
    case MessageTypes::GeometryDelObj:
    case MessageTypes::GeometryDelAll:
      cerr << "Geometry msg add/del/delall " << endl;
      break;
    case MessageTypes::GeometryFlush:
    case MessageTypes::GeometryFlushViews:
    case MessageTypes::GeometryGetNRoe:
    case MessageTypes::GeometryGetData:
      cerr << "Geometry msg flush/flush views/get NRoe/get data" << endl;
      //ogeom->forward(gmsg);
      break;
    default:
      cerr << "Illegal Message type: " << msg->type << endl;
      break;
    }
    delete msg;
  }
}

#endif

  } // namespace Modules
} //  namespace PSECommon
