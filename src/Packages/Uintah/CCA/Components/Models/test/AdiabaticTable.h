
#ifndef Packages_Uintah_CCA_Components_Examples_AdiabaticTable_h
#define Packages_Uintah_CCA_Components_Examples_AdiabaticTable_h
#include <Packages/Uintah/CCA/Ports/ModelInterface.h>

#include <Packages/Uintah/Core/Grid/ComputeSet.h>
#include <Packages/Uintah/Core/Grid/CCVariable.h>
#include <Packages/Uintah/Core/Grid/SFCXVariable.h>
#include <Packages/Uintah/Core/Grid/SFCYVariable.h>
#include <Packages/Uintah/Core/Grid/SFCZVariable.h>
#include <Packages/Uintah/Core/Grid/VarTypes.h>
#include <map>
#include <vector>

namespace Uintah {
  class ICELabel;
  class TableInterface;

/**************************************

CLASS
   AdiabaticTable
   
   AdiabaticTable simulation

GENERAL INFORMATION

   AdiabaticTable.h

   Todd Harman
   Department of Computer Science
   University of Utah

   Center for the Simulation of Accidental Fires and Explosions (C-SAFE)
  
   Copyright (C) 2000 SCI Group

KEYWORDS
   AdiabaticTable

DESCRIPTION
   Long description...
  
WARNING
  
****************************************/

  class GeometryPiece;
  class AdiabaticTable :public ModelInterface {
  public:
    AdiabaticTable(const ProcessorGroup* myworld, ProblemSpecP& params);
    virtual ~AdiabaticTable();
    
    virtual void problemSetup(GridP& grid, SimulationStateP& sharedState,
			      ModelSetup* setup);
    
    virtual void scheduleInitialize(SchedulerP&,
				    const LevelP& level,
				    const ModelInfo*);

    virtual void restartInitialize() {}
      
    virtual void scheduleComputeStableTimestep(SchedulerP&,
					       const LevelP& level,
					       const ModelInfo*);
      
    virtual void scheduleMassExchange(SchedulerP&,
				      const LevelP& level,
				      const ModelInfo*);
                                  
    virtual void scheduleMomentumAndEnergyExchange(SchedulerP&,
						   const LevelP& level,
						   const ModelInfo*);
                                            
   virtual void scheduleModifyThermoTransportProperties(SchedulerP&,
                                                const LevelP&,
                                                const MaterialSet*);
                                                
   virtual void computeSpecificHeat(CCVariable<double>&,
                                    const Patch*,
                                    DataWarehouse*,
                                    const int);
  private:
    ICELabel* lb;
                                                
   void modifyThermoTransportProperties(const ProcessorGroup*, 
                                        const PatchSubset* patches,        
                                        const MaterialSubset*,             
                                        DataWarehouse*,                    
                                        DataWarehouse* new_dw);             
   
    void initialize(const ProcessorGroup*, 
                    const PatchSubset* patches,
		      const MaterialSubset* matls, 
                    DataWarehouse*, 
		      DataWarehouse* new_dw);
                     
    void momentumAndEnergyExchange(const ProcessorGroup*, 
                                   const PatchSubset* patches,
	                            const MaterialSubset* matls, 
                                   DataWarehouse*, 
	                            DataWarehouse* new_dw, 
                                   const ModelInfo*);
                                   
    void massExchange(const ProcessorGroup*, 
                      const PatchSubset* patches,
                      const MaterialSubset*,
                      DataWarehouse* old_dw,
                      DataWarehouse* new_dw,
                      const ModelInfo* mi);

    AdiabaticTable(const AdiabaticTable&);
    AdiabaticTable& operator=(const AdiabaticTable&);

    ProblemSpecP params;

    const Material* d_matl;
    MaterialSet* d_matl_set;

    class Region {
    public:
      Region(GeometryPiece* piece, ProblemSpecP&);

      GeometryPiece* piece;
      double initialScalar;
    };

    class Scalar {
    public:
      int index;
      string name;
      // labels for this particular scalar
      VarLabel* scalar_CCLabel;
      VarLabel* scalar_source_CCLabel;
      VarLabel* diffusionCoefLabel;
      
      vector<Region*> regions;
      double diff_coeff;
    };

    double oldProbeDumpTime;
    Scalar* d_scalar;
    
    SimulationStateP sharedState;
    Output* dataArchiver;
    vector<Vector> d_probePts;
    vector<string> d_probePtsNames;
    bool d_usingProbePts;
    double d_probeFreq;

    TableInterface* table;
    struct TableValue {
      string name;
      int index;
      VarLabel* label;
    };
    vector<TableValue*> tablevalues;

    int density_index;
    int gamma_index;
    int cv_index;
    int viscosity_index;
    int temp_index;
  };
}

#endif
