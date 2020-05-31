#include "QatGenericFunctions/AbsFunction.h"
//------------------------------------------------------------------------------------------------------------//
//                                                                                                                         //
//                                                                                                                         //
//                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------//
#ifndef _<Function>_h_
#define _<Function>_h_
namespace Genfun {
class <Function>:public AbsFunction {

    FUNCTION_OBJECT_DEF(<Function>)

      public:
    
    // Constructor:
    <Function>();

    // Destructor:
    virtual ~<Function>();

    // Copy Constructor:
    <Function> (const <Function> & right);

    // Retrieve function value (here for function of one variable)
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    // Derivative.  (here we provide)
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?  (here we say yes)
    virtual bool hasAnalyticDerivative() const {return true;}

  private:
    
    const <Function> & operator=(const <Function> &right)=delete;

  };
}
#endif
