#include <Packages/Uintah/CCA/Components/Solvers/HypreGenericSolver.h>
#include <Packages/Uintah/CCA/Components/Solvers/HypreSolverPFMG.h>
#include <Packages/Uintah/CCA/Components/Solvers/HypreDriverStruct.h>
#include <Packages/Uintah/CCA/Components/Solvers/HypreDriverSStruct.h>
#include <Packages/Uintah/Core/Exceptions/ProblemSetupException.h>
#include <Core/Util/DebugStream.h>

using namespace Uintah;
//__________________________________
//  To turn on normal output
//  setenv SCI_DEBUG "HYPRE_DOING_COUT:+"

static DebugStream cout_doing("HYPRE_DOING_COUT", false);

namespace Uintah {

  HypreGenericSolver::HypreGenericSolver(const HypreInterface& interface,
                                         const ProcessorGroup* pg,
                                         const HypreSolverParams* params,
                                         const int acceptableInterface)
    : _interface(interface), _pg(pg), _params(params)
  {
    assertInterface(acceptableInterface);

    //    _solverType = getSolverType(solverTitle);
    _results.numIterations = 0;
    _results.finalResNorm  = 1.23456e+30; // Large number
  }
  
  void
  HypreGenericSolver::assertInterface(const int acceptableInterface)
  { 
    if (acceptableInterface & _interface) {
      return;
    }
    throw InternalError("Solver does not support Hypre interface: "
                        +_interface,__FILE__, __LINE__);
  }

  // TODO: include all derived classes here.
  HypreGenericSolver*
  newHypreGenericSolver(const SolverType& solverType,
                        const HypreInterface& interface,
                        const ProcessorGroup* pg,
                        const HypreSolverParams* params,
                        const int acceptableInterface)
    /* Create a new solver object of specific solverType solver type
       but a generic solver pointer type. */
  {
    switch (solverType) {
#if 0
    case SMG:
      {
        return new HypreSolverSMG(interface,pg,params,acceptableInterface);
      }
#endif
    case PFMG:
      {
        return new HypreSolverPFMG(interface,pg,params,acceptableInterface);
      }
#if 0
    case SparseMSG:
      {
        return new HypreSolverSparseMSG(interface,pg,params,acceptableInterface);
      }
    case CG:
      {
        return new HypreSolverCG(interface,pg,params,acceptableInterface);
      }
    case Hybrid: 
      {
        return new HypreSolverHybrid(interface,pg,params,acceptableInterface);
      }
    case GMRES:
      {
        return new HypreSolverGMRES(interface,pg,params,acceptableInterface);
      }
#endif
    default:
      throw InternalError("Unsupported solver type: "+solverType,
                          __FILE__, __LINE__);
    } // switch (solverType)
    return 0;
  }

  SolverType
  getSolverType(const string& solverTitle)
  {
    // Determine solver type from title
    if ((solverTitle == "SMG") ||
        (solverTitle == "smg")) {
      return SMG;
    } else if ((solverTitle == "PFMG") ||
               (solverTitle == "pfmg")) {
      return PFMG;
    } else if ((solverTitle == "SparseMSG") ||
               (solverTitle == "sparsemsg")) {
      return SparseMSG;
    } else if ((solverTitle == "CG") ||
               (solverTitle == "cg") ||
               (solverTitle == "PCG") ||
               (solverTitle == "conjugategradient")) {
      return CG;
    } else if ((solverTitle == "Hybrid") ||
               (solverTitle == "hybrid")) {
      return Hybrid;
    } else if ((solverTitle == "GMRES") ||
               (solverTitle == "gmres")) {
      return GMRES;
    } else if ((solverTitle == "AMG") ||
               (solverTitle == "amg") ||
               (solverTitle == "BoomerAMG") ||
               (solverTitle == "boomeramg")) {
      return AMG;
    } else if ((solverTitle == "FAC") ||
               (solverTitle == "fac")) {
      return FAC;
    } else {
      throw InternalError("Unknown solver type: "+solverTitle,
                          __FILE__, __LINE__);
    } // end "switch" (solverTitle)
  } // end solverFromTitle()


  HypreInterface
  getSolverInterface(const SolverType& solverType)
    /* Determine the Hypre interface this solver uses */
  {
    switch (solverType) {
    case SMG:
      {
        return HypreDriver::Struct;
      }
    case PFMG:
      {
        return HypreDriver::Struct;
      }
    case SparseMSG:
      {
        return HypreDriver::Struct;
      }
    case CG:
      {
        return HypreDriver::Struct;
      }
    case Hybrid: 
      {
        return HypreDriver::Struct;
      }
    case GMRES:
      {
        return HypreDriver::Struct;
      }
    case FAC:
      {
        return HypreDriver::SStruct;
      }
    case AMG:
      {
        return HypreDriver::ParCSR;
      }
    default:
      throw InternalError("Unsupported solver type: "+solverType,
                          __FILE__, __LINE__);
    } // switch (solverType)
  } // end solverInterface()

} // end namespace Uintah
