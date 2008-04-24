#ifndef UINTAH_COMPARE_MMS_SineMMS_H
#define UINTAH_COMPARE_MMS_SineMMS_H


#include <Packages/Uintah/StandAlone/tools/compare_mms/MMS.h>

class SineMMS : public MMS {

public:
  SineMMS(double A, double viscosity, double p_ref);
  virtual ~SineMMS();

  double pressure( double x, double y, double z, double time );
  double uVelocity( double x, double y, double z, double time );
  double vVelocity( double x, double y, double z, double time );
  double wVelocity( double x, double y, double z, double time );
  

private:
  double d_A;
  double d_viscosity;
  double d_p_ref;
};

#endif
