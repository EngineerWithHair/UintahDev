//static char *id="@(#) $Id$";

/*
 *  cVectorPort.cc : ?
 *
 *  Written by:
 *   Author: ?
 *   Department of Computer Science
 *   University of Utah
 *   Date: ?
 *
 *  Copyright (C) 199? SCI Group
 */

#include <PSECore/Datatypes/cVectorPort.h>
#include <SCICore/Malloc/Allocator.h>

namespace PSECore {
namespace Datatypes {

using namespace SCICore::Datatypes;

extern "C" {
PSECORESHARE IPort* make_cVectorIPort(Module* module, const clString& name) {
  return scinew SimpleIPort<cVectorHandle>(module,name);
}
PSECORESHARE OPort* make_cVectorOPort(Module* module, const clString& name) {
  return scinew SimpleOPort<cVectorHandle>(module,name);
}
}

template<> clString SimpleIPort<cVectorHandle>::port_type("cVector");
template<> clString SimpleIPort<cVectorHandle>::port_color("yellow");

} // End namespace Datatypes
} // End namespace PSECore

//
// $Log$
// Revision 1.6  2000/11/29 09:49:38  moulding
// changed all instances of "new" to "scinew"
//
// Revision 1.5  2000/11/22 17:14:42  moulding
// added extern "C" make functions for input and output ports (to be used
// by the auto-port facility).
//
// Revision 1.4  1999/08/30 20:19:25  sparker
// Updates to compile with -LANG:std on SGI
// Other linux/irix porting oscillations
//
// Revision 1.3  1999/08/25 03:48:26  sparker
// Changed SCICore/CoreDatatypes to SCICore/Datatypes
// Changed PSECore/CommonDatatypes to PSECore/Datatypes
// Other Misc. directory tree updates
//
// Revision 1.2  1999/08/17 06:38:14  sparker
// Merged in modifications from PSECore to make this the new "blessed"
// version of SCIRun/Uintah.
//
// Revision 1.1  1999/07/27 16:55:51  mcq
// Initial commit
//
// Revision 1.1.1.1  1999/04/24 23:12:48  dav
// Import sources
//
//
