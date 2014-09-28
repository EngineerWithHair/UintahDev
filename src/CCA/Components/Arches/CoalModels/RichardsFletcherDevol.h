#ifndef Uintah_Component_Arches_RichardsFletcherDevol_h
#define Uintah_Component_Arches_RichardsFletcherDevol_h
#include <Core/ProblemSpec/ProblemSpec.h>
#include <Core/Grid/SimulationStateP.h>
#include <CCA/Components/Arches/CoalModels/Devolatilization.h>
#include <CCA/Components/Arches/CoalModels/ModelBase.h>
#include <CCA/Components/Arches/CoalModels/CoalModelFactory.h>
#include <CCA/Components/Arches/ArchesVariables.h>
#include <CCA/Components/Arches/Directives.h>

//===========================================================================

/**
  * @class    RichardsFletcherDevol
  * @author   Jeremy Thornock, Julien Pedel, Charles Reid
  * @date     May 2009        Check-in of initial version
  *           November 2009   Verification
  *
  * @brief    A class for calculating the DQMOM model term for the 
  *           Kobayashi-Sarofim coal devolatilization model.
  *
  * The Builder is required because of the Model Factory; the Factory needs
  * some way to create the model term and register it.
  *
  */

//---------------------------------------------------------------------------
// Builder
namespace Uintah{

class ArchesLabel;
class RichardsFletcherDevolBuilder: public ModelBuilder 
{
public: 
  RichardsFletcherDevolBuilder( const std::string               & modelName,
                                const std::vector<std::string>  & reqICLabelNames,
                                const std::vector<std::string>  & reqScalarLabelNames,
                                ArchesLabel                     * fieldLabels,
                                SimulationStateP                & sharedState,
                                int qn );

  ~RichardsFletcherDevolBuilder(); 

  ModelBase* build(); 

private:

}; 

// End Builder
//---------------------------------------------------------------------------

class RichardsFletcherDevol: public Devolatilization {
public: 

  RichardsFletcherDevol( std::string modelName, 
                         SimulationStateP& shared_state, 
                         ArchesLabel* fieldLabels,
                         std::vector<std::string> reqICLabelNames,
                         std::vector<std::string> reqScalarLabelNames,
                         int qn );

  ~RichardsFletcherDevol();

  ////////////////////////////////////////////////
  // Initialization method

  /** @brief Interface for the inputfile and set constants */ 
  void problemSetup(const ProblemSpecP& db, int qn);

  // No initVars() method because no special variables needed

  ////////////////////////////////////////////////
  // Model computation method

  /** @brief Schedule the calculation of the source term */ 
  void sched_computeModel( const LevelP& level, 
                           SchedulerP& sched, 
                           int timeSubStep );
  
  /** @brief Actually compute the source term */ 
  void computeModel( const ProcessorGroup* pc, 
                     const PatchSubset* patches, 
                     const MaterialSubset* matls, 
                     DataWarehouse* old_dw, 
                     DataWarehouse* new_dw );

private:

  const VarLabel* d_raw_coal_mass_label;
  const VarLabel* d_char_mass_label;
  const VarLabel* d_weight_label;
  const VarLabel* d_particle_temperature_label;
  const VarLabel* d_gas_temperature_label;

  std::vector<double>  RichardsFletcher_coefficients;  
  std::vector<double>  as_received;
  std::vector<double>  particle_sizes;
  std::vector<double>  ash_mass_init;
  std::vector<double>  char_mass_init;
  std::vector<double>  vol_dry;
  std::vector<double>  mass_dry;
  std::vector<double>  rc_mass_init;
  double Av1;        ///< Pre-exponential factors for devolatilization rate constants
  double Av2;        ///< Pre-exponential factors for devolatilization rate constants
  double Ev1;        ///< Activation energy for devolatilization rate constant
  double Ev2;        ///< Activation energy for devolatilization rate constant
  double Y1_;       ///< Volatile fraction from proximate analysis
  double Y2_;       ///< Fraction devolatilized at higher temperatures (often near unity)
  double k1;        ///< Rate constant for devolatilization reaction 1
  double k2;        ///< Rate constant for devolatilization reaction 2
  double c0_1;
  double c1_1;
  double c2_1;
  double c3_1;
  double c4_1;
  double c5_1;
  double c0_2;
  double c1_2;
  double c2_2;
  double c3_2;
  double c4_2;
  double c5_2;
  double Xv;
  double Fv1;
  double Fv2;
  double rhop;
  double total_rc;
  double total_dry;
  double rc_mass_frac;
  double char_mass_frac;
  double ash_mass_frac;
  
  double pi;
  
  bool compute_part_temp; ///< Boolean: is particle temperature computed? 
  bool part_temp_from_enth; ///< Boolean: is particle temperature computed? 
  bool compute_char_mass;

  double rateMax;
  double d_rc_scaling_factor;   ///< Scaling factor for raw coal internal coordinate
  double d_rh_scaling_factor;
  double d_pt_scaling_factor;   ///< Scaling factor for particle temperature internal coordinate
  double testVal_part;
  double testVal_gas;
  double testVal_char;
}; // end ConstSrcTerm
} // end namespace Uintah
#endif
