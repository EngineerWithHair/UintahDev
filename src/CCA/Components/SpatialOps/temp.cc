
#include <Packages/Uintah/CCA/Components/SpatialOps/ModelBase.h>
#include <Packages/Uintah/CCA/Components/SpatialOps/ModelFactory.h>
#include <Packages/Uintah/Core/Grid/Variables/CCVariable.h>

using namespace std;
using namespace Uintah; 

ModelBase::ModelBase( std::string modelName, SimulationStateP& sharedState,
                        vector<std::string> reqLabelNames ) : 
d_modelName(modelName), d_sharedState( sharedState ), d_requiredLabels(reqLabelNames)
{
  //Create a label for this source term. 
  d_modelLabel = VarLabel::create(modelName, CCVariable<double>::getTypeDescription()); 

  d_labelSchedInit  = false; 
  d_labelActualInit = false; 
}

ModelBase::~ModelBase()
{
  VarLabel::destroy(d_modelLabel); 
}


