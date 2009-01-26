/*

The MIT License

Copyright (c) 1997-2009 Center for the Simulation of Accidental Fires and 
Explosions (CSAFE), and  Scientific Computing and Imaging Institute (SCI), 
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


#include <Packages/Uintah/CCA/Components/Parent/ComponentFactory.h>
#include <Packages/Uintah/CCA/Components/Parent/Switcher.h>
#include <Packages/Uintah/CCA/Components/MPM/SerialMPM.h>
#include <Packages/Uintah/CCA/Components/MPM/AMRMPM.h>
#include <Packages/Uintah/CCA/Components/MPM/FractureMPM.h>
#include <Packages/Uintah/CCA/Components/MPM/RigidMPM.h>
#include <Packages/Uintah/CCA/Components/MPM/ShellMPM.h>
#include <Packages/Uintah/CCA/Components/MPM/ImpMPM.h>
#include <Packages/Uintah/CCA/Components/ICE/ICE.h>
#include <Packages/Uintah/CCA/Components/ICE/AMRICE.h>
#include <Packages/Uintah/CCA/Components/MPMICE/MPMICE.h>
#include <Packages/Uintah/CCA/Components/MPMArches/MPMArches.h>
#include <Packages/Uintah/CCA/Components/Examples/Poisson1.h>
#include <Packages/Uintah/CCA/Components/Examples/Poisson2.h>
#include <Packages/Uintah/CCA/Components/Examples/Burger.h>
#include <Packages/Uintah/CCA/Components/Examples/Wave.h>
#include <Packages/Uintah/CCA/Components/Examples/AMRWave.h>
#include <Packages/Uintah/CCA/Components/Examples/ParticleTest1.h>
#include <Packages/Uintah/CCA/Components/Examples/RegridderTest.h>
#include <Packages/Uintah/CCA/Components/Examples/Poisson3.h>
#include <Packages/Uintah/CCA/Components/Angio/Angio.h>
#include <Packages/Uintah/CCA/Components/Examples/SolverTest1.h>
#include <Packages/Uintah/CCA/Components/PatchCombiner/PatchCombiner.h>
#include <Packages/Uintah/CCA/Components/PatchCombiner/UdaReducer.h>

#include <Packages/Uintah/Core/Exceptions/ProblemSetupException.h>
#include <Packages/Uintah/Core/Parallel/Parallel.h>
#include <Packages/Uintah/Core/Parallel/ProcessorGroup.h>


#include <iosfwd>

#include <sci_defs/uintah_defs.h>

using std::cerr;
using std::endl;

using namespace Uintah;

UintahParallelComponent* ComponentFactory::create(ProblemSpecP& ps, const ProcessorGroup* world, 
                                                  bool doAMR, string sim_comp, string uda)
{
  if (sim_comp == "") {
    ProblemSpecP sim_ps = ps->findBlock("SimulationComponent");
    if (sim_ps)
      sim_ps->get("type",sim_comp);
  }

  if (world->myrank() == 0)
    cout << "Simulation Component: \t" << sim_comp << endl;

#ifndef NO_MPM
  if (sim_comp == "mpm" || sim_comp == "MPM") {
    return scinew SerialMPM(world);
  } 
  if (sim_comp == "mpmf" || sim_comp == "fracturempm" || sim_comp == "FRACTUREMPM") {
    return scinew FractureMPM(world);
  } 
  if (sim_comp == "rmpm" || sim_comp == "rigidmpm" || sim_comp == "RIGIDMPM") {
    return scinew RigidMPM(world);
  } 
  if (sim_comp == "amrmpm" || sim_comp == "AMRmpm" || sim_comp == "AMRMPM") {
    return scinew AMRMPM(world);
  } 
  if (sim_comp == "smpm" || sim_comp == "shellmpm" || sim_comp == "SHELLMPM") {
    return scinew ShellMPM(world);
  } 
  if (sim_comp == "impm" || sim_comp == "IMPM") {
    return scinew ImpMPM(world);
  } 
#endif
#ifndef NO_ICE
  if (sim_comp == "ice" || sim_comp == "ICE") {
    if (doAMR)
      return scinew AMRICE(world);
    else
      return scinew ICE(world);
  } 
#endif
#if !defined(NO_MPM) && !defined(NO_ICE)
  if (sim_comp == "mpmice" || sim_comp == "MPMICE") {
    return scinew MPMICE(world,STAND_MPMICE, doAMR);
  } 
  if (sim_comp == "smpmice" || sim_comp == "shellmpmice" || sim_comp == "SHELLMPMICE") {
    return scinew MPMICE(world,SHELL_MPMICE, doAMR);
  } 
  if (sim_comp == "rmpmice" || sim_comp == "rigidmpmice" || sim_comp == "RIGIDMPMICE") {
    return scinew MPMICE(world,RIGID_MPMICE, doAMR);
  } 
#endif
#ifndef NO_ARCHES
  if (sim_comp == "arches" || sim_comp == "ARCHES") {
    if( !Uintah::Parallel::usingMPI() ) {
      throw ProblemSetupException("You must be using MPI when running an arches problem!", __FILE__, __LINE__);
    }
    return scinew Arches(world);
  } 
#endif
#if !defined(NO_MPM) && !defined(NO_ARCHES)
  if (sim_comp == "mpmarches" || sim_comp == "MPMARCHES") {
    return scinew MPMArches(world);
  } 
#endif
  if (sim_comp == "burger" || sim_comp == "BURGER") {
    return scinew Burger(world);
  } 
  if (sim_comp == "wave" || sim_comp == "WAVE") {
    if (doAMR)
      return scinew AMRWave(world);
    else
      return scinew Wave(world);
  } 
  if (sim_comp == "poisson1" || sim_comp == "POISSON1") {
    return scinew Poisson1(world);
  } 
  if (sim_comp == "regriddertest" || sim_comp == "REGRIDDERTEST") {
    return scinew RegridderTest(world);
  } 
  if (sim_comp == "poisson2" || sim_comp == "POISSON2") {
    return scinew Poisson2(world);
  } 
  if (sim_comp == "poisson3" || sim_comp == "POISSON3") {
    return scinew Poisson3(world);
  } 
  if (sim_comp == "angio") {
    return scinew Angio(world);
  } 
  if (sim_comp == "particletest" || sim_comp == "PARTICLETEST") {
    return scinew ParticleTest1(world);
  } 
  if (sim_comp == "solvertest" || sim_comp == "SOLVERTEST") {
    return scinew SolverTest1(world);
  } 
  if (sim_comp == "switcher" || sim_comp == "SWITCHER") {
    return scinew Switcher(world, ps, doAMR, uda);
  } 
  if (sim_comp == "combine_patches") {
    return scinew PatchCombiner(world, uda);
  } 
  if (sim_comp == "reduce_uda") {
    return scinew UdaReducer(world, uda);
  } 

  throw ProblemSetupException("Unknown simulationComponent. Must specify -arches, -ice, -mpm, "
                              "-impm, -mpmice, -mpmarches, -burger, -wave, -poisson1, -poisson2, -poisson3 or -angio"
                              "\nMake sure that component is supported in this build", __FILE__, __LINE__);
}
