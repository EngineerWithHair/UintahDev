/*
 * The MIT License
 *
 * Copyright (c) 2010-2016 The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <expression/Expression.h>
#include <pokitt/CanteraObjects.h>

#ifndef DRhoDEnthalpy_h
#define DRhoDEnthalpy_h

namespace WasatchCore{

/**
 *  \class DRhoDEnthalpy
 *  \author Josh McConnell
 *  \date   March 2017
 *
 *  \brief Computes \f$\frac{\partial \rho}{\partial h}\f$
 *  where \f$h\f$ is the mixture enthalpy given by
 *  \f[
 *   \frac{\partial \rho}{\partial h} = -\frac{\rho}{c_p T},
 *  \f]
 *  where \f$\T\f$ is the temperature and \f$\c_p\f$ is the
 *  mixture capacity.
 */
template<typename FieldT>
class DRhoDEnthalpy : public Expr::Expression<FieldT>
{
  DECLARE_FIELDS(FieldT, rho_, cp_, t_ )

    DRhoDEnthalpy( const Expr::Tag& rhoTag,
                   const Expr::Tag& cpTag,
                   const Expr::Tag& temperatureTag );

public:
  class Builder : public Expr::ExpressionBuilder
  {
  public:
    /**
     *  @brief Build a DRhoDEnthalpy expression
     *  @param resultTag the tag for the value that this expression computes
     */

    Builder( const Expr::Tag& resultTag,
             const Expr::Tag& rhoTag,
             const Expr::Tag& cpTag,
             const Expr::Tag& temperatureTag );

    Expr::ExpressionBase* build() const{
      return new DRhoDEnthalpy( rhoTag_, cpTag_, temperatureTag_ );
    }

  private:
    const Expr::Tag rhoTag_, cpTag_, temperatureTag_;
  };

  void evaluate();

};


// ###################################################################
//
//                          Implementation
//
// ###################################################################

template<typename FieldT>
DRhoDEnthalpy<FieldT>::
DRhoDEnthalpy( const Expr::Tag& rhoTag,
               const Expr::Tag& cpTag,
               const Expr::Tag& temperatureTag )
: Expr::Expression<FieldT>()
{
  this->set_gpu_runnable(true);

  rho_ = this->template create_field_request<FieldT>( rhoTag         );
  cp_  = this->template create_field_request<FieldT>( cpTag          );
  t_   = this->template create_field_request<FieldT>( temperatureTag );
}

//--------------------------------------------------------------------

template< typename FieldT >
void
DRhoDEnthalpy<FieldT>::
evaluate()
{
  using namespace SpatialOps;
  FieldT& result = this->value();

  const FieldT& rho = rho_->field_ref();
  const FieldT& cp  = cp_ ->field_ref();
  const FieldT& t   = t_  ->field_ref();

  result <<= -rho / (t*cp);
}

//--------------------------------------------------------------------

template<typename FieldT>
DRhoDEnthalpy<FieldT>::
Builder::Builder( const Expr::Tag& resultTag,
                  const Expr::Tag& rhoTag,
                  const Expr::Tag& cpTag,
                  const Expr::Tag& temperatureTag )
  : ExpressionBuilder( resultTag ),
    rhoTag_        ( rhoTag         ),
    cpTag_         ( cpTag          ),
    temperatureTag_( temperatureTag )
{}

//====================================================================
}
#endif // DRhoDEnthalpy_h

