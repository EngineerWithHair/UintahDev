
#include <testprograms/Component/framework/SenderImpl.h>
#include <testprograms/Component/framework/PortInfoImpl.h>

#include <sstream>
#include <iostream>

namespace sci_cca {

using std::cerr;

SenderImpl::SenderImpl()
{
}

SenderImpl::~SenderImpl()
{
}

void 
SenderImpl::setServices( const Services &svc )
{
  ComponentImpl::setServices( svc );

  PortInfo info = new PortInfoImpl("Uses", "", 0);
  svc->registerUsesPort( info );
}

void
SenderImpl::go()
{
  TestPort port = pidl_cast<TestPort>(services_->getPort("Uses"));
  if ( !port ) {
    cerr << "go: could not find test port\n";
    return;
  }

  port->test();
}

} // namespace sci_cca

