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


#include "NexusSpMessage.h"

void NexusSpMessage::printDebug(string d) {
  cout << d << endl;
}

NexusSpMessage::NexusSpMessage(globus_nexus_startpoint_t* sp) { 
  //Get startpoint from Channel
  d_sp = sp;  
  _buffer = NULL;
  _recvbuff = NULL;
} 

NexusSpMessage::~NexusSpMessage() {  }

void NexusSpMessage::createMessage() {   
  if (kDEBUG) printDebug("NexusSpMessage::createMessage()");

  //Setting up reply
  _reply= ReplyEP::acquire();
  _reply->get_startpoint_copy(&_reply_sp);
  //Record the size of the reply startpoint
  msgsize = globus_nexus_sizeof_startpoint(&_reply_sp,1);
  //Creating the message buffer
  _buffer = new globus_nexus_buffer_t();
  if(int _gerr=globus_nexus_buffer_init(_buffer, BUF_SIZE, 0))
    throw CommError("buffer_init", _gerr);
}

void NexusSpMessage::marshalInt(int *i, int size) {
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);
  int s = globus_nexus_sizeof_int(size);
  msgsize += s;
  globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0);
  globus_nexus_put_int(_buffer, i, size);
}

void NexusSpMessage::marshalByte(char *b, int size) {
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);  
  globus_byte_t* gb = (globus_byte_t*) b;
  int s = globus_nexus_sizeof_byte(size);
  msgsize += s;
  globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0); 
  globus_nexus_put_byte(_buffer, gb, size);
}

void NexusSpMessage::marshalChar(char *c, int size) {
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);  
  int s = globus_nexus_sizeof_char(size);
  msgsize += s;
  globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0); 
  globus_nexus_put_char(_buffer, c, size);
}

void NexusSpMessage::marshalFloat(float *f, int size) {
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);  
  int s = globus_nexus_sizeof_float(size);
  msgsize += s;
  globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0); 
  globus_nexus_put_float(_buffer, f, size);
}

void NexusSpMessage::marshalDouble(double *d, int size) {
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);  
  int s = globus_nexus_sizeof_double(size);
  msgsize += s;
  globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0); 
  globus_nexus_put_double(_buffer, d, size);
}

void NexusSpMessage::marshalLong(long *l, int size) {
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);
  int s = globus_nexus_sizeof_long(size);
  msgsize += s;
  globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0);
  globus_nexus_put_long(_buffer, l, size);
}

void NexusSpMessage::marshalSpChannel(SpChannel* channel) {
  if (kDEBUG) printDebug("NexusSpMessage::marshalSpChan()");

  if (_buffer == NULL)
    throw CommError("uninitialized buffer on marshaling call",1000);
  NexusSpChannel * nex_chan = dynamic_cast<NexusSpChannel*>(channel);
  if (nex_chan) {
    int s = globus_nexus_sizeof_startpoint(&(nex_chan->d_sp),1);
    msgsize += s;
    globus_nexus_check_buffer_size(_buffer,s,BUF_SIZE,0,0); 
    globus_nexus_put_startpoint_transfer(_buffer, &(nex_chan->d_sp), 1); 
  }
  else { 
    throw CommError("Communication library discrepancy detected",1001);
  }
}

void NexusSpMessage::sendMessage(int handler) { 
  if (kDEBUG) printDebug("NexusSpMessage::sendMessage()");
 
  if (_buffer == NULL)
    throw CommError("uninitialized buffer on send message call",1000);
  // Marshal the reply startpoint
  globus_nexus_put_startpoint_transfer(_buffer, &_reply_sp, 1);
  // Send the message
  if(int _gerr=globus_nexus_send_rsr(_buffer, d_sp,
                                     handler, GLOBUS_TRUE, GLOBUS_FALSE))
    throw CommError("send_rsr", _gerr);
  delete _buffer; 
  _buffer = NULL;
}

void NexusSpMessage::waitReply() {
  _recvbuff = new globus_nexus_buffer_t();
  (*_recvbuff) = _reply->wait();
}

void NexusSpMessage::unmarshalReply() { }

void NexusSpMessage::unmarshalInt(int* i, int size) { 
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);
  globus_nexus_get_int(_recvbuff, i, size);
}

void NexusSpMessage::unmarshalByte(char *b, int size) {  
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);
  globus_byte_t gb;
  globus_nexus_get_byte(_recvbuff, &gb, size);
  *b = (char) gb;
}

void NexusSpMessage::unmarshalChar(char *c, int size) {
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);  
  globus_nexus_get_char(_recvbuff, c, size);
}

void NexusSpMessage::unmarshalFloat(float *f, int size) {
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);  
  globus_nexus_get_float(_recvbuff, f, size);
}

void NexusSpMessage::unmarshalDouble(double *d, int size) {
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);  
  globus_nexus_get_double(_recvbuff, d, size);
}

void NexusSpMessage::unmarshalLong(long* l, int size) {
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);
  globus_nexus_get_long(_recvbuff, l, size);
}

void NexusSpMessage::unmarshalSpChannel(SpChannel* channel) {
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on unmarshaling call",1000);
  NexusSpChannel* nex_chan = dynamic_cast<NexusSpChannel*>(channel);
  if (nex_chan == NULL)
    throw CommError("error in comm. libraries in unmarshaling call",1000); 
  if(int _gerr=globus_nexus_get_startpoint(_recvbuff,&(nex_chan->d_sp), 1))
    throw CommError("get_startpoint",_gerr);
}

void* NexusSpMessage::getLocalObj() { 
  if (d_sp == NULL) {
    throw CommError("d_sp = NULL (getLocalObj)",1000);
  }

  if(globus_nexus_startpoint_to_current_context(d_sp)){  
    globus_nexus_endpoint_t *ep;
    if(int _gerr=globus_nexus_startpoint_get_endpoint(d_sp, &ep))  
      throw CommError("get_endpoint", _gerr);
    return (globus_nexus_endpoint_get_user_pointer(ep));
  }
  else {
    return NULL;
  }
}

void NexusSpMessage::destroyMessage() {
  if (_recvbuff == NULL)
    throw CommError("empty reply buffer on destroy message call",1000);
  ReplyEP::release(_reply);
  if(int _gerr=globus_nexus_buffer_destroy(_recvbuff))
    throw CommError("buffer_destroy", _gerr);
  delete _recvbuff;
  _recvbuff = NULL;
}








