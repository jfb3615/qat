#include "<Function>.h"
#include "QatGenericFunctions/FixedConstant.h"
namespace Genfun {

  FUNCTION_OBJECT_IMP(<Function>)

  // Constructor:
  <Function>::<Function>():AbsFunction() {
  }
  
  // Destructor:
  <Function>::~<Function> (){
  }

  // Copy Constructor:
  <Function>::<Function>(const <Function> & ):AbsFunction() {
  }
  
  // Function Call
  double <Function>::operator() ( double x) const {
    return x;
  }

  
  // Partial Derivative
  Derivative <Function>::partial(unsigned int index) const {
    const AbsFunction & fPrime=FixedConstant(0);
    return Derivative(&fPrime);
  }
}
