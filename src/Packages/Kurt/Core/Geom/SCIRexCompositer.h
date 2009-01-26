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


#ifndef SCIREXCOMPOSITER_H
#define SCIREXCOMPOSITER_H


#include <sci_defs/ogl_defs.h>

#if defined(HAVE_GLEW)
#include <GL/glew.h>
#include <GL/glxew.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#endif


#include <Core/Thread/Mailbox.h>
#include <Core/Thread/Runnable.h>
#include <vector>

namespace SCIRun {
class Mutex;
}
namespace Kurt {
using std::vector;
using SCIRun::Runnable;
using SCIRun::Mailbox;
using SCIRun::Mutex;

class SCIRexRenderData;
class SCIRexWindow;
class SCIRexCompositer : public Runnable {
public:

  virtual void run();
  SCIRexCompositer(SCIRexRenderData *rd);
  virtual ~SCIRexCompositer();
  void add( SCIRexWindow* r);
  void doComposite();
  void kill(){ die_ = true; }
  void SetFrame(int,int,int,int);

protected:

  vector<SCIRexWindow *> renderers;
  int xmin,ymin,xmax,ymax, begin_offset, end_offset;
  SCIRexRenderData *render_data_;
  Mutex *mutex_;
  bool die_;
};

}// end namespace Kurt
#endif
