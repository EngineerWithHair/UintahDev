/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2004 Scientific Computing and Imaging Institute,
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



/*
 *  PIDL.h: Include a bunch of PIDL files for external clients
 *
 *  Written by:
 *   Steven G. Parker
 *   Department of Computer Science
 *   University of Utah
 *   July 1999
 *
 *  Copyright (C) 1999 SCI Group
 */

#include <sci_defs/mpi_defs.h>
#include <sci_defs/globus_defs.h>

#include <Core/CCA/PIDL/PIDL.h>
#include <Core/CCA/PIDL/Object_proxy.h>
#include <Core/CCA/PIDL/Warehouse.h>
#include <Core/CCA/PIDL/SocketSpChannel.h>
#include <Core/CCA/PIDL/SocketEpChannel.h>
#include <Core/CCA/PIDL/SocketMessage.h>
//#include <Core/CCA/PIDL/Intra/IntraComm.h>
#include <Core/Exceptions/InternalError.h>
#include <Core/CCA/DT/DataTransmitter.h>

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////
// removed in revision 30469:
//
//#ifdef HAVE_MPI
//#include <Core/CCA/PIDL/Intra/IntraCommMPI.h>
//#endif
//////////////////////////////////////////////////////////////////////////

#ifdef HAVE_GLOBUS
# include <Core/CCA/PIDL/NexusSpChannel.h>
# include <Core/CCA/PIDL/NexusEpChannel.h>
# include <Core/CCA/PIDL/CommNexus.h>
#endif

//Inter-Component Comm libraries supported
#define COMM_NEXUS  1
#define COMM_SOCKET 2

//////////////////////////////////////////////////////////////////////////
// removed in revision 30469:
//
//Intra-Component Comm libraries supported
//#define INTRA_COMM_MPI 1
//static int intra_comm_type = 0;
//////////////////////////////////////////////////////////////////////////

static int comm_type = 0;
//static int intra_comm_type = 0;

using namespace SCIRun;

Warehouse* PIDL::warehouse;

DataTransmitter *PIDL::theDataTransmitter;

int
PIDL::rank(0);

int
PIDL::size(1);

bool
PIDL::isfrwk(false);

Object::pointer*
PIDL::optr(NULL);

bool
PIDL::sampleProxy(false);

void
PIDL::initialize(int rank,int size)
{
  //Default for communication purposes
#ifdef HAVE_GLOBUS
  setCommunication(COMM_NEXUS);
#else
  setCommunication(COMM_SOCKET);
#endif
  switch (comm_type) {
  case COMM_SOCKET:
    if(!theDataTransmitter){
      theDataTransmitter = new DataTransmitter;
      theDataTransmitter->run();
      //TODO: sb should delete theDataTransmitter ???
    }
    break;
#ifdef HAVE_GLOBUS
  case COMM_NEXUS:
    CommNexus::initialize();
    break;
#endif
  }

//////////////////////////////////////////////////////////////////////////
// removed in revision 30469:
//
//#ifdef HAVE_MPI
//  setIntraCommunication(INTRA_COMM_MPI);
//#endif
//////////////////////////////////////////////////////////////////////////

  PIDL::rank = rank;
  PIDL::size = size;

  if (!warehouse) {
    warehouse = new Warehouse;
  }

}

bool
PIDL::isFramework(){
  return isfrwk;
}


void
PIDL::finalize()
{
  if(sampleProxy) {
#if defined (HAVE_MPI) || defined (HAVE_MPICH)
    if(PIDL::size > 1) { ::std::cerr << "YOYO\n"; (*optr)->getException(); }
    delete optr;
#endif
  }
  switch (comm_type) {
  case COMM_SOCKET:
    theDataTransmitter->exit();
    break;
#ifdef HAVE_GLOBUS
  case COMM_NEXUS:
    CommNexus::finalize();
    break;
#endif
  }
}

SpChannel*
PIDL::getSpChannel() {
  switch (comm_type) {
  case COMM_SOCKET:
    return (new SocketSpChannel());
#ifdef HAVE_GLOBUS
  case COMM_NEXUS:
    return (new NexusSpChannel());
#endif
  default:
    return NULL;
  }
}

EpChannel*
PIDL::getEpChannel() {
  switch (comm_type) {
  case COMM_SOCKET:
    return (new SocketEpChannel());
#ifdef HAVE_GLOBUS
  case COMM_NEXUS:
    return (new NexusEpChannel());
#endif
  default:
    return (new SocketEpChannel());
  }
}

Warehouse*
PIDL::getWarehouse()
{
  if(!warehouse)
    throw SCIRun::InternalError("Warehouse not initialized!", __FILE__, __LINE__);
  return warehouse;
}

Object::pointer
PIDL::objectFrom(const URL& url)
{
  return Object::pointer(new Object_proxy(url));
}

Object::pointer
PIDL::objectFrom(const int urlc, const URL urlv[], int mysize, int myrank)
{
  return Object::pointer(new Object_proxy(urlc,urlv,mysize,myrank));
}

Object::pointer
PIDL::objectFrom(const std::vector<URL>& urlv, int mysize, int myrank)
{
  return Object::pointer(new Object_proxy(urlv,mysize,myrank));
}

Object::pointer
PIDL::objectFrom(const std::vector<Object::pointer>& pxy, int mysize, int myrank)
{
  return Object::pointer(new Object_proxy(pxy,mysize,myrank));
}

void
PIDL::serveObjects()
{
  if(!warehouse)
    throw SCIRun::InternalError("Warehouse not initialized!", __FILE__, __LINE__);
  warehouse->run();
}

//////////////////////////////////////////////////////////////////////////
// removed in revision 30469:
//
// IntraComm*
// PIDL::getIntraComm()
// {
// #ifdef HAVE_MPI
//   switch (intra_comm_type) {
//   case (INTRA_COMM_MPI):
//     return (new IntraCommMPI());
//   default:
//     return (new IntraCommMPI());
//   }
// #endif
//   return NULL;
// }
//////////////////////////////////////////////////////////////////////////

//PRIVATE:

void
PIDL::setCommunication(int c)
{
  if (comm_type != 0)
    throw SCIRun::InternalError("Cannot modify communication setting more than once", __FILE__, __LINE__);
  else {
    comm_type = c;
  }
}


//////////////////////////////////////////////////////////////////////////
// removed in revision 30469:
//
// void
// PIDL::setIntraCommunication(int c)
// {
//   if (intra_comm_type != 0)
//     throw SCIRun::InternalError("Cannot modify communication setting after it has been set once\n");
//   else {
//     intra_comm_type = c;
//   }
// }
//////////////////////////////////////////////////////////////////////////


DataTransmitter*
PIDL::getDT()
{
  return theDataTransmitter;
}

bool
PIDL::isNexus()
{
  return comm_type == COMM_NEXUS;
}
