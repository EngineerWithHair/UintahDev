/*
  The contents of this file are subject to the University of Utah Public
  License (the "License"); you may not use this file except in compliance
  with the License.
  
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
  License for the specific language governing rights and limitations under
  the License.
  
  The Original Source Code is SCIRun, released March 12, 2001.
  
  The Original Source Code was developed by the University of Utah.
  Portions created by UNIVERSITY are Copyright (C) 2001, 1994 
  University of Utah. All Rights Reserved.
*/

//    File   : NIMRODConverter.h
//    Author : Allen Sanderson
//             School of Computing
//             University of Utah
//    Date   : July 2003

#if !defined(NIMRODConverter_h)
#define NIMRODConverter_h

#include <Dataflow/Network/Module.h>

#include <Core/Util/TypeDescription.h>
#include <Core/Util/DynamicLoader.h>

#include <Core/Datatypes/CurveField.h>
#include <Core/Datatypes/TriSurfField.h>
#include <Core/Datatypes/QuadSurfField.h>

#include <Core/Datatypes/TetVolField.h>
#include <Core/Datatypes/PrismVolField.h>
#include <Core/Datatypes/HexVolField.h>

#include <Core/Datatypes/StructHexVolField.h>
#include <Core/Datatypes/StructQuadSurfField.h>
#include <Core/Datatypes/StructCurveField.h>

#include <Core/Geometry/Vector.h>
#include <Core/Geometry/Tensor.h>

#include <Teem/Core/Datatypes/NrrdData.h>

namespace Fusion {

using namespace SCIRun;
using namespace SCITeem;

class NIMRODConverterAlgo : public DynamicAlgoBase
{
public:
  //! support the dynamically compiled algorithm concept
  static CompileInfoHandle get_compile_info( const string converter,
					     const unsigned int ntype );

public:
  virtual NrrdDataHandle execute(vector< NrrdDataHandle > nHandles,
				 vector< int > mesh,
				 vector< int > data,
				 int idim, int jdim, int kdim) = 0;
};

template< class NTYPE >
class NIMRODMeshConverterAlgoT : public NIMRODConverterAlgo
{
public:

  virtual NrrdDataHandle execute(vector< NrrdDataHandle > nHandles,
				 vector< int > mesh,
				 vector< int > data,
				 int idim, int jdim, int kdim);
};


template< class NTYPE >
NrrdDataHandle
NIMRODMeshConverterAlgoT< NTYPE >::execute(vector< NrrdDataHandle > nHandles,
					   vector< int > mesh,
					   vector< int > data,
					   int idim, int jdim, int kdim)
{
  int sink_size = 1;
  int ndims = 3;

  NrrdData *nout = scinew NrrdData(false);

  NTYPE* ndata = scinew NTYPE[idim*jdim*kdim*3];

  register int i,j,k,cc = 0;

  NTYPE *ptrR   = (NTYPE *)(nHandles[mesh[0]]->nrrd->data);
  NTYPE *ptrZ   = (NTYPE *)(nHandles[mesh[1]]->nrrd->data);
  NTYPE *ptrPhi = (NTYPE *)(nHandles[mesh[2]]->nrrd->data);
    
  for( j=0; j<jdim; j++ ) {
    for( k=0; k<kdim; k++ ) {
      
      int iRZ = k * jdim + j;
      
      for( i=0; i<idim; i++ ) {
	double cosPhi = cos( ptrPhi[i] );
	double sinPhi = sin( ptrPhi[i] );
      
	// Mesh
	ndata[cc*3  ] =  ptrR[iRZ] * cosPhi;
	ndata[cc*3+1] = -ptrR[iRZ] * sinPhi;
	ndata[cc*3+2] =  ptrZ[iRZ];
	
	++cc;
      }
    }
  }

  nrrdWrap(nout->nrrd, ndata, nHandles[mesh[2]]->nrrd->type,
	   ndims+1, sink_size, jdim, kdim, idim);
  nrrdAxesSet(nout->nrrd, nrrdAxesInfoCenter, nrrdCenterNode, 
	      nrrdCenterNode, nrrdCenterNode, nrrdCenterNode);

  vector< string > dataset;

  nHandles[mesh[2]]->get_tuple_indecies(dataset);

  dataset[0].replace( dataset[0].find( "PHI:Scalar" ), 10, "XYZ:Vector" );

  nout->nrrd->axis[0].label = strdup(dataset[0].c_str());
  nout->nrrd->axis[1].label = strdup("Radial");
  nout->nrrd->axis[2].label = strdup("Theta");
  nout->nrrd->axis[3].label = strdup("Phi");

  PropertyManager*  in = (PropertyManager*) (nHandles[mesh[2]]->nrrd);
  PropertyManager* out = (PropertyManager*) (nout);

  //  *in = *out;

  nout->set_property( "Topology", string("Structured"), false );
  nout->set_property( "Coordinate System", string("Cartesian - XYZ"), false );

  return  NrrdDataHandle( nout );	
}


template< class NTYPE >
class NIMRODRealSpaceConverterAlgoT : public NIMRODConverterAlgo
{
public:

  virtual NrrdDataHandle execute(vector< NrrdDataHandle > nHandles,
				 vector< int > mesh,
				 vector< int > data,
				 int idim, int jdim, int kdim);
};


template< class NTYPE >
NrrdDataHandle
NIMRODRealSpaceConverterAlgoT< NTYPE >::
execute(vector< NrrdDataHandle > nHandles,
	vector< int > mesh,
	vector< int > data,
	int idim, int jdim, int kdim)
{
  int sink_size = 1;
  int ndims = 3;

  NrrdData *nout = scinew NrrdData(false);

  NTYPE* ndata = scinew NTYPE[idim*jdim*kdim*3];

  register int i,j,k,cc = 0;

  NTYPE *ptrR   = (NTYPE *)(nHandles[data[0]]->nrrd->data);
  NTYPE *ptrZ   = (NTYPE *)(nHandles[data[1]]->nrrd->data);
  NTYPE *ptrPhi = (NTYPE *)(nHandles[data[2]]->nrrd->data);
  
  NTYPE *ptrMeshPhi = (NTYPE *)(nHandles[mesh[2]]->nrrd->data);

  for( i=0; i<idim; i++ ) {
    double cosPhi = cos( ptrMeshPhi[i] );
    double sinPhi = sin( ptrMeshPhi[i] );
      
    for( j=0; j<jdim; j++ ) {
      for( k=0; k<kdim; k++ ) {

	int index = (i * jdim + j) * kdim + k;

	// Value
	ndata[cc*3  ] =  ptrR[index] * cosPhi - ptrPhi[index] * sinPhi;
	ndata[cc*3+1] = -ptrR[index] * sinPhi - ptrPhi[index] * cosPhi;
	ndata[cc*3+2] =  ptrZ[index];
	  
	++cc;
      }
    }
  }

  nrrdWrap(nout->nrrd, ndata, nHandles[data[2]]->nrrd->type,
	   ndims+1, sink_size, jdim, kdim, idim);
  nrrdAxesSet(nout->nrrd, nrrdAxesInfoCenter, nrrdCenterNode, 
	      nrrdCenterNode, nrrdCenterNode, nrrdCenterNode);

  vector< string > dataset;

  nHandles[data[2]]->get_tuple_indecies(dataset);

  dataset[0].replace( dataset[0].find( "PHI:Scalar" ), 10, "XYZ:Vector" );

  nout->nrrd->axis[0].label = strdup(dataset[0].c_str());
  nout->nrrd->axis[1].label = strdup("Radial");
  nout->nrrd->axis[2].label = strdup("Theta");
  nout->nrrd->axis[3].label = strdup("Phi");

  PropertyManager*  in = (PropertyManager*) (nHandles[data[2]]->nrrd);
  PropertyManager* out = (PropertyManager*) (nout);

  //  *in = *out;

  nout->set_property( "Coordinate System", string("Cartesian - XYZ"), false );

  return  NrrdDataHandle( nout );	
}


} // end namespace Fusion

#endif // NIMRODConverter_h
