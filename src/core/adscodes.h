/* ADSCODES.H - Definitions of the AutoCAD function request and result
                codes used by ADS programs.
*/

#ifndef _ADSCODES_H
#define _ADSCODES_H 1


/*  AutoCAD to ADS program request type codes  */

#define RQTERM   0                    /* Force terminate ADS program */
#define RQSAVE  14                    /* ACAD is saving the drawing */
#define RQEND   15                    /* ACAD is ending the drawing editor */
#define RQQUIT  16                    /* ACAD quits */

/* The following are LISP to Application ONLY request codes.  They form
    the interface for ADS */
#define RQXLOAD 100                   /* Load external functions */
#define RQXUNLD 101                   /* UN-Load application  */
#define RQSUBR  102                   /* External subroutine request */
 
/*  ADS program to AutoCAD result type codes  */

#define RSRSLT   1                    /* Result returned */
#define RSERR    3                    /* Error in evaluation -- no result */
 
/*  Result value type codes  */
 
    /* These are used in the resbuf.  These ARE NOT the same
       as the group codes used in the entity access routines, and sould
       not be confused with those.  The same result buffer IS used for 
       entity records, however, in which case the restype field may take
       on many more values than those listed here. */
 
#define RTNONE    5000                /* No result */
#define RTREAL    5001                /* Real number */
#define RTPOINT   5002                /* 2D point X and Y only */
#define RTSHORT   5003                /* Short integer */
#define RTANG     5004                /* Angle */
#define RTSTR     5005                /* String */
#define RTENAME   5006                /* Entity name */
#define RTPICKS   5007                /* Pick set */
#define RTORINT   5008                /* Orientation */
#define RT3DPOINT 5009                /* 3D point - X, Y, and Z */
#define RTLONG    5010                /* Long integer */
#define RTVOID    5014                /* Blank symbol */
#define RTLB      5016                /* list begin */
#define RTLE      5017                /* list end */
#define RTDOTE    5018                /* dotted pair */
#define RTNIL     5019                /* nil */
#define RTDXF0    5020                /* DXF code 0 for ads_buildlist only */

/* The following RT codes are for the ADS program interface.
   They are used to signal success or failure (error) of the
   ADS library functions.  RTFAIL in particular means that 
   the link has failed (most likely because AutoLISP has
   died or exited), and the application should cleanup and exit.
*/
 
#define RTNORM    5100                /* Request succeeded */
 
/* Various error codes returned to ADS application by library */
#define RTERROR  -5001                /* Some other error */
#define RTCAN    -5002                /* User cancelled request -- Ctl-C */
#define RTREJ    -5003                /* AutoCAD rejected request -- invalid */
#define RTFAIL   -5004                /* Link failure -- Lisp probably died */
#define RTKWORD  -5005                /* Keyword returned from getxxx() routine */
 
 
#endif    /* _ADSCODES_H */

