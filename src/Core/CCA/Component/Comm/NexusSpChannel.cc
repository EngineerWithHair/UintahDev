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

#include "NexusSpChannel.h"
 
void NexusSpChannel::printDebug(string d) {
  cout << d << endl;
}

NexusSpChannel::NexusSpChannel() {
  //  if (kDEBUG) printDebug("NexusSpChannel::NexusSpChannel()");

  globus_nexus_startpoint_set_null(&d_sp);
  msg = NULL;
}

NexusSpChannel::~NexusSpChannel() { 
  if (kDEBUG) printDebug("NexusSpChannel::~NexusSpChannel()");

  if (&d_sp != NULL) {
    if(int gerr=globus_nexus_startpoint_destroy_and_notify(&d_sp)){
      throw CommError("nexus_startpoint_destroy_and_notify", gerr);
    }
  }
  if (msg != NULL)
    delete msg;
}

SpChannel* NexusSpChannel::SPFactory(bool deep) {
  if (kDEBUG) printDebug("NexusSpChannel::SpFactory()");

  NexusSpChannel* new_sp = new NexusSpChannel();
  if (deep) {
    if(!globus_nexus_startpoint_is_null(&d_sp)) { 
      if( int gerr = globus_nexus_startpoint_copy
	  (&(new_sp->d_sp), const_cast<globus_nexus_startpoint_t*>(&d_sp)) ) {
	throw CommError("startpoint_copy", gerr);
      }
    }
  }
  else {
    new_sp->d_sp = d_sp;
  }
  return new_sp;
}

void NexusSpChannel::openConnection(const PIDL::URL& url) {
  string s1("NexusSpChannel::openConnection() ");
  s1 += url.getString();
  if (kDEBUG) printDebug(s1);

  if(int gerr=globus_module_activate(GLOBUS_NEXUS_MODULE))
    throw CommError("Unable to initialize nexus", gerr);
  globus_nexus_enable_fault_tolerance(NULL, 0);

  std::string s(url.getString());
  char* str=const_cast<char*>(s.c_str());
  if(int gerr=globus_nexus_attach(str, &d_sp)){
    throw CommError("nexus_attach", gerr);
  }
}

void NexusSpChannel::closeConnection() { 
  if (kDEBUG) printDebug("NexusSpChannel::closeConnection()");

  int size=0;
  globus_nexus_buffer_t buffer;

  if(int gerr=globus_nexus_buffer_init(&buffer, size, 0)) {
    throw CommError("buffer_init", gerr);
  }

  //Send the message
  int handler=PIDL::TypeInfo::vtable_deleteReference_handler;
  if(int gerr=globus_nexus_send_rsr(&buffer, &d_sp,
  				    handler, GLOBUS_TRUE, GLOBUS_FALSE)) {
    throw CommError("ProxyBase: send_rsr", gerr);
  }
  //No reply is sent for this
  if(int gerr=globus_nexus_startpoint_destroy_and_notify(&d_sp)){
    throw CommError("nexus_startpoint_destroy_and_notify", gerr);
  }
}

Message* NexusSpChannel::getMessage() {
  if (kDEBUG) printDebug("NexusSpChannel::getMessage()");

  if (msg == NULL)
    msg = new NexusSpMessage(&d_sp);
  return msg;
}  






