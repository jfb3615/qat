*
* $Id: d506dp.inc,v 1.1 2008/10/27 20:58:47 jfb Exp $
*
* $Log: d506dp.inc,v $
* Revision 1.1  2008/10/27 20:58:47  jfb
* commit minuit
*
* Revision 1.1  2007/01/04 15:44:42  joe
* Populating...
*
* Revision 1.1.1.1  2000/06/08 11:19:21  andras
* import of MINUIT from CERNlib 2000
*
* Revision 1.1.1.1  1996/03/07 14:31:32  mclareni
* Minuit
*
*
#ifndef CERNLIB_MINUIT_D506DP_INC
#define CERNLIB_MINUIT_D506DP_INC
*
*
* d506dp.inc
*
#if defined(CERNLIB_DOUBLE)
C ************ DOUBLE PRECISION VERSION *************
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
#endif
#if !defined(CERNLIB_DOUBLE)
C ************ SINGLE PRECISION VERSION *************
#endif

#endif
