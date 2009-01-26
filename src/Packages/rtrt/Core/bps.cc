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



#include <Core/Thread/Thread.h>
#include <Core/Thread/Barrier.h>
#include <Core/Thread/Runnable.h>
#include <Core/Thread/ThreadGroup.h>
#include <Core/Thread/Mutex.h>

#include <sgi_stl_warnings_off.h>
#include <iostream>
#include <sgi_stl_warnings_on.h>

#include <cstdlib>
#include <sys/sysmp.h>
#include <unistd.h>

using SCIRun::Mutex;
using SCIRun::Runnable;
using SCIRun::Barrier;
using SCIRun::ThreadGroup;
using SCIRun::Thread;

Mutex io_lock_("io lock");

class BPS : public Runnable {
    Barrier* barrier;
    int count;
    int proc;
public:
    BPS(Barrier* barrier, int count, int proc);
    virtual void run();
};

void usage(char* progname)
{
    cerr << "usage: " << progname << " nprocessors count\n";
    exit(1);
}

int main(int argc, char* argv[])
{
    cout << "Program starting\n";
    int np=0;
    int count=0;
    if(argc != 3){
	usage(argv[1]);
    }
    np=atoi(argv[1]);
    count=atoi(argv[2]);
#if 0
    Barrier* barrier=new Barrier("test barrier");
    ThreadGroup* group=new ThreadGroup("test group");
    for(int i=0;i<np;i++){
	char buf[100];
	sprintf(buf, "worker %d", i);
	//	new Thread(new BPS(barrier, count, i), buf, group);
    }
    group->join();
#endif
    cout << "Program ending\n";
}

BPS::BPS(Barrier* barrier, int count, int proc)
    : barrier(barrier), count(count), proc(proc)
{
}

void BPS::run()
{
    int np=Thread::numProcessors();
    int p=(24+proc)%np;
#if 0
    io.lock();
    cerr << "Mustrun: " << p << "(pid=" << getpid() << ")\n";
    io.unlock();
    if(sysmp(MP_MUSTRUN, p) == -1){
	perror("sysmp - MP_MUSTRUN");
    }
#endif
    barrier->wait(np);
    //    double time=Thread::currentSeconds();
    for(int i=0;i<count;i++){
	barrier->wait(np);
	static int g=0;
	if(g != i) {
	    cerr << "bps.cc: OOPS!\n";
        }
	barrier->wait(np);
	if(proc==0)
	    g++;
    }
#if 0
    double etime=Thread::currentSeconds();
    if(proc==0){
	cerr << "done in " << etime-time << " seconds \n";
	cerr << count/(etime-time) << " barriers/second\n";
	cerr << (etime-time)/count*1000 << " ms/barrier\n";
    }
#endif
}
