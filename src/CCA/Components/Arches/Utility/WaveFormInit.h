#ifndef Uintah_Component_Arches_WaveFormInit_h
#define Uintah_Component_Arches_WaveFormInit_h

#include <CCA/Components/Arches/Task/TaskInterface.h>
#include <CCA/Components/Arches/Operators/Operators.h>
#include <spatialops/structured/FVStaggered.h>

namespace Uintah{ 

  //IT is the independent variable type
  //DT is the dependent variable type
  template <typename IT, typename DT>
  class WaveFormInit : public TaskInterface { 

public: 

    enum WAVE_TYPE { SINE, SQUARE, SINECOS };

    WaveFormInit<IT, DT>( std::string task_name, int matl_index, const std::string var_name ); 
    ~WaveFormInit<IT, DT>(); 

    void problemSetup( ProblemSpecP& db ); 

    //Build instructions for this (WaveFormInit) class. 
    class Builder : public TaskInterface::TaskBuilder { 

      public: 

      Builder( std::string task_name, int matl_index, std::string var_name ) : 
        _task_name(task_name), _matl_index(matl_index), _var_name(var_name){}
      ~Builder(){}

      WaveFormInit* build()
      { return scinew WaveFormInit<IT, DT>( _task_name, _matl_index, _var_name ); }

      private: 

      std::string _task_name; 
      int _matl_index; 
      std::string _var_name; 

    };

protected: 

    void register_initialize( std::vector<VariableInformation>& variable_registry );

    void register_timestep_init( std::vector<VariableInformation>& variable_registry ){} 

    void register_timestep_eval( std::vector<VariableInformation>& variable_registry, const int time_substep ){} 

    void initialize( const Patch* patch, ArchesTaskInfoManager* tsk_info, 
                     SpatialOps::OperatorDatabase& opr );
    
    void timestep_init( const Patch* patch, ArchesTaskInfoManager* tsk_info, 
                        SpatialOps::OperatorDatabase& opr ){}

    void eval( const Patch* patch, ArchesTaskInfoManager* tsk_info, 
               SpatialOps::OperatorDatabase& opr ){}

private:

    const std::string _var_name; 
    std::string _ind_var_name;
    std::string _ind_var_name_2;
    WAVE_TYPE _wtype; 
    VAR_TYPE _D_type; 
    VAR_TYPE _I_type; 
    double _amp; 
    double _two_pi; 
    double _f1, _f2; 
    double _A, _B; 
    double _offset; 
    double _min_sq; 
    double _max_sq; 
  
  };

  //Function definitions: 

  template <typename IT, typename DT>
  WaveFormInit<IT, DT>::WaveFormInit( std::string task_name, int matl_index, const std::string var_name ) : 
  _var_name(var_name), TaskInterface( task_name, matl_index ){

    // This needs to be done to set the variable type 
    // for this function. This comes in handy often for templated tasks. 
    // In this specific function, we don't use it but rather look at the 
    // dependent and indepenent variables and set separate types for them for 
    // use in the variable registration. 
    set_task_type<DT>(); 

    _two_pi = 2.0*acos(-1.0);
  
  }

  template <typename IT, typename DT>
  WaveFormInit<IT, DT>::~WaveFormInit()
  {}

  template <typename IT, typename DT>
  void WaveFormInit<IT, DT>::problemSetup( ProblemSpecP& db ){ 

    std::string wave_type; 
    db->findBlock("wave")->getAttribute("type",wave_type); 
    db->findBlock("wave")->findBlock("independent_variable")->getAttribute("label",_ind_var_name); 

    if ( wave_type == "sine"){ 

      ProblemSpecP db_sine = db->findBlock("wave")->findBlock("sine"); 

      _wtype = SINE; 
      db_sine->getAttribute("A",_A); 
      db_sine->getAttribute("f",_f1); 
      db_sine->getAttribute("offset",_offset); 

    } else if ( wave_type == "square"){ 

      ProblemSpecP db_square= db->findBlock("wave")->findBlock("square"); 

      _wtype = SQUARE; 
      db_square->getAttribute("f",_f1); 
      db_square->getAttribute("min",_min_sq ); 
      db_square->getAttribute("max",_max_sq ); 
      db_square->getAttribute("offset",_offset); 

    } else if ( wave_type == "sinecos"){ 
    
      ProblemSpecP db_sinecos= db->findBlock("wave")->findBlock("sinecos"); 
      db->findBlock("wave")->findBlock("independent_variable_2")->getAttribute("label",_ind_var_name_2); 

      _wtype = SINECOS; 
      db_sinecos->getAttribute("A",_A); 
      db_sinecos->getAttribute("B",_B); 
      db_sinecos->getAttribute("f1",_f1 ); 
      db_sinecos->getAttribute("f2",_f2 ); 
      db_sinecos->getAttribute("offset",_offset); 

    } else { 

      throw InvalidValue("Error: Wave type not recognized.",__FILE__,__LINE__);

    }

    //This sets the type of the independent and dependent variable types as needed by the variable 
    //registration step. 
    DT* d_test; 
    IT* i_test; 

    set_type(d_test, _D_type); 
    set_type(i_test, _I_type); 

  }

  template <typename IT, typename DT>
  void WaveFormInit<IT, DT>::register_initialize( std::vector<VariableInformation>& variable_registry ){ 

    //FUNCITON CALL     STRING NAME(VL)     TYPE       DEPENDENCY    GHOST DW     VR
    register_variable( _ind_var_name,       _I_type,   REQUIRES,       0, NEWDW,  variable_registry ); 
    if ( _wtype == SINECOS ){ 
      register_variable( _ind_var_name_2,       _I_type,   REQUIRES,       0, NEWDW,  variable_registry ); 
    }
    register_variable( _var_name,           _D_type,   MODIFIES,       0, NEWDW,  variable_registry );
  
  }
  
  template <typename IT, typename DT> 
  void WaveFormInit<IT,DT>::initialize( const Patch* patch, ArchesTaskInfoManager* tsk_info, 
                                        SpatialOps::OperatorDatabase& opr ){ 

    using namespace SpatialOps;
    using SpatialOps::operator *; 
    typedef SpatialOps::SpatFldPtr<IT> ITptr; 
    typedef SpatialOps::SpatFldPtr<DT> DTptr; 

    //build an operator to interpolate the independent variable to the grid 
    //location of the dependent variable
    typedef typename OperatorTypeBuilder< SpatialOps::Interpolant, IT, DT >::type InterpT;
    const InterpT* const interp = opr.retrieve_operator<InterpT>();

    DTptr dep_field = tsk_info->get_so_field<DT>( _var_name ); 
    ITptr ind_field = tsk_info->get_const_so_field<IT>( _ind_var_name ); 

    switch (_wtype){ 
      case SINE:

        *dep_field <<= _A*sin( _two_pi * _f1 * (*interp)( *ind_field ) ) + _offset; 

        break; 
      case SQUARE: 

        *dep_field <<= sin( _two_pi * _f1 * (*interp)( *ind_field )) + _offset; 

        *dep_field <<= cond( *dep_field < 0.0, _min_sq )
                           ( *dep_field > 0.0, _max_sq )
                           ( 0.0 ); 

        break; 

      case SINECOS:

        *dep_field <<= _A*sin(_two_pi * _f1 * (*interp)(*ind_field)) + _B*cos(_two_pi * _f2 * (*interp)(*ind_field)); 

        break;

      default:
        break;

    } 
  }
}
#endif 
