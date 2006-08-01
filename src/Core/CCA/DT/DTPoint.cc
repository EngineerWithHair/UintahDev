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
 *  DTPoint.cc: Data Communication Point (Sender/Receiver)
 *
 *  Written by:
 *   Keming Zhang
 *   Department of Computer Science
 *   University of Utah
 *   Jun 2003
 *
 *  Copyright (C) 1999 SCI Group
 */

#include <Core/Thread/Semaphore.h>
#include <Core/CCA/DT/DTPoint.h>
#include <Core/CCA/DT/DTMessage.h>
#include <Core/CCA/DT/DTMessageTag.h>
#include <Core/CCA/DT/DataTransmitter.h>


using namespace SCIRun;

DTPoint::DTPoint(DataTransmitter *dt) : dt(dt), object(NULL), service(NULL)
{
  //dt->registerPoint(this);
  //service=NULL;
}

DTPoint::~DTPoint()
{
  //delete sema;
  //dt->unregisterPoint(this);
}

DTMessage *
DTPoint::getMessage(const DTMessageTag &tag)
{
  return dt->getMessage(tag);
}

DTMessage *
DTPoint::getMsg()
{
  return dt->getMsg();
}

DTMessageTag
DTPoint::putInitialMessage(DTMessage *msg)
{
  msg->sender = this;
  return dt->putMessage(msg);
}

void
DTPoint::putReplyMessage(DTMessage *msg)
{
  msg->sender = this;
  dt->putReplyMessage(msg);
}

void
DTPoint::putMsg(DTMessage *msg)
{
  msg->sender = this;
  dt->putMsg(msg);
}






