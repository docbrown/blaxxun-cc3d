
/*  A Bison parser, made from parse.ypp with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	tNUMBER	258
#define	tSTRING	259
#define	tIDENTIFIER	260
#define	tUNIMPL_KEYWORD	666
#define	tASSIGN	261
#define	tFUNCTION	262
#define	tIF	263
#define	tELSE	264
#define	tFOR	265
#define	tIN	266
#define	tWHILE	267
#define	tWITH	268
#define	tBREAK	269
#define	tCONTINUE	270
#define	tRETURN	271
#define	tNEW	272
#define	tVAR	273
#define	tNULL	274
#define	tCOMMA	275
#define	tPLUSEQ	276
#define	tMINUSEQ	277
#define	tMULTIPLYEQ	278
#define	tDIVIDEEQ	279
#define	tMODEQ	280
#define	tRSHIFTEQ	281
#define	tLSHIFTEQ	282
#define	tRSHIFTFILLEQ	283
#define	tANDEQ	284
#define	tXOREQ	285
#define	tOREQ	286
#define	tCONDTEST	287
#define	tCONDSEP	288
#define	tLOR	289
#define	tLAND	290
#define	tOR	291
#define	tXOR	292
#define	tAND	293
#define	tEQ	294
#define	tNE	295
#define	tLT	296
#define	tLE	297
#define	tGE	298
#define	tGT	299
#define	tRSHIFT	300
#define	tLSHIFT	301
#define	tRSHIFTFILL	302
#define	tPLUS	303
#define	tMINUS	304
#define	tMULTIPLY	305
#define	tDIVIDE	306
#define	tMOD	307
#define	tNOT	308
#define	tNEG	309
#define	tONESCOMP	310
#define	tINCREMENT	311
#define	tDECREMENT	312
#define	tDOT	313
#define	tRIGHTBRACKET	314
#define	tLEFTBRACKET	315

#line 5 "parse.ypp"


#if 0
#include "stdafx.h"
// move to top
// uncomment yystype because in y_tab.h
#endif


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string.h>

#include "vsexpr.h"
#include "vsfunction.h"
#include "vsstatement.h"
#include "vsparse.h"

#include "y_tab.h"

#include "GReporter.h"

// Lex and YACC are hard-coded for these routine names:
#define input() VsParseTree::input()
#define unput(_char) VsParseTree::unput(_char)
#define yyerror(_error) VsParseTree::yyerror(_error)

#define gettxt(a,b) (b)  /* SGI YACC produces these... */
#define YYDEBUG 0


#line 41 "parse.ypp"
typedef union {
    const char *string;
    double number;
    VsExpr *expr;
    VsVar *var;
    VsArrayVar *arrayvar;
    VsFunctionDef *funcDef;
    VsFunctionCall *funcCall;
    VsMethodCall   *objMethodCall;
    VsMemberAccess *objMemberAccess;
    VsStatement *stmnt;
    VsExprList *exprList;
    VsStatementList *stmntList;
    VsNewCall   *newCall;   
    void *funcList;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		197
#define	YYFLAG		-32768
#define	YYNTBASE	67

#define YYTRANSLATE(x) ((unsigned)(x) <= 666 ? yytranslate[x] : 97)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    63,
    64,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    21,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    65,     2,    66,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
    48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
    58,    59,    60,    61,    62,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     6
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     6,     8,    15,    17,    21,    23,    24,
    28,    31,    33,    36,    38,    39,    41,    43,    45,    48,
    51,    53,    56,    59,    62,    64,    67,    69,    75,    83,
    93,   101,   103,   105,   111,   117,   119,   123,   127,   129,
   130,   133,   135,   136,   138,   142,   146,   149,   152,   155,
   159,   163,   167,   171,   175,   179,   183,   187,   191,   195,
   199,   203,   206,   209,   212,   215,   221,   225,   229,   233,
   237,   241,   245,   249,   253,   257,   261,   265,   269,   273,
   277,   281,   285,   289,   293,   297,   299,   301,   303,   305,
   307,   309,   311,   313,   315,   319,   321,   322,   327,   334,
   340,   342,   344,   346,   350,   355,   357
};

static const short yyrhs[] = {    68,
     0,     0,    68,    69,     0,    69,     0,     8,    70,    63,
    71,    64,    72,     0,     5,     0,    71,    22,     5,     0,
     5,     0,     0,    65,    73,    66,     0,    65,    66,     0,
    74,     0,    73,    74,     0,    74,     0,     0,    76,     0,
    77,     0,    81,     0,    86,    21,     0,    75,    21,     0,
    80,     0,    78,    21,     0,    79,    21,     0,    84,    21,
     0,    21,     0,    17,    86,     0,    17,     0,     9,    63,
    86,    64,    72,     0,     9,    63,    86,    64,    72,    10,
    72,     0,    11,    63,    85,    21,    85,    21,    85,    64,
    72,     0,    11,    63,    95,    12,    87,    64,    72,     0,
    15,     0,    16,     0,    14,    63,    87,    64,    72,     0,
    13,    63,    86,    64,    72,     0,     5,     0,     5,     7,
    87,     0,    83,    22,    82,     0,    82,     0,     0,    19,
    83,     0,    86,     0,     0,    87,     0,    87,    22,    86,
     0,    63,    86,    64,     0,    51,    87,     0,    55,    87,
     0,    57,    87,     0,    92,     7,    87,     0,    92,    23,
    87,     0,    92,    24,    87,     0,    92,    25,    87,     0,
    92,    26,    87,     0,    92,    27,    87,     0,    92,    31,
    87,     0,    92,    33,    87,     0,    92,    32,    87,     0,
    92,    29,    87,     0,    92,    28,    87,     0,    92,    30,
    87,     0,    58,    87,     0,    59,    87,     0,    87,    58,
     0,    87,    59,     0,    87,    34,    87,    35,    87,     0,
    87,    41,    87,     0,    87,    42,    87,     0,    87,    43,
    87,     0,    87,    44,    87,     0,    87,    45,    87,     0,
    87,    46,    87,     0,    87,    37,    87,     0,    87,    36,
    87,     0,    87,    50,    87,     0,    87,    51,    87,     0,
    87,    52,    87,     0,    87,    53,    87,     0,    87,    54,
    87,     0,    87,    40,    87,     0,    87,    38,    87,     0,
    87,    39,    87,     0,    87,    48,    87,     0,    87,    47,
    87,     0,    87,    49,    87,     0,     4,     0,     3,     0,
    20,     0,    90,     0,    89,     0,    91,     0,    93,     0,
    94,     0,    95,     0,    88,    22,    87,     0,    87,     0,
     0,     5,    63,    88,    64,     0,    87,    60,     5,    63,
    88,    64,     0,    18,     5,    63,    88,    64,     0,    93,
     0,    94,     0,    96,     0,    87,    60,     5,     0,    87,
    61,    86,    62,     0,     5,     0,     5,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   106,   107,   110,   111,   114,   118,   121,   122,   123,   127,
   128,   129,   134,   135,   136,   141,   142,   143,   144,   145,
   146,   147,   148,   149,   150,   159,   160,   163,   164,   167,
   169,   177,   181,   185,   189,   193,   195,   200,   201,   202,
   206,   209,   210,   213,   214,   217,   218,   219,   220,   221,
   222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
   232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
   242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
   252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
   262,   263,   264,   265,   268,   269,   270,   273,   297,   305,
   314,   315,   317,   322,   330,   334,   342
};

static const char * const yytname[] = {   "$","error","$undefined.","tNUMBER",
"tSTRING","tIDENTIFIER","tUNIMPL_KEYWORD","tASSIGN","tFUNCTION","tIF","tELSE",
"tFOR","tIN","tWHILE","tWITH","tBREAK","tCONTINUE","tRETURN","tNEW","tVAR","tNULL",
"';'","tCOMMA","tPLUSEQ","tMINUSEQ","tMULTIPLYEQ","tDIVIDEEQ","tMODEQ","tRSHIFTEQ",
"tLSHIFTEQ","tRSHIFTFILLEQ","tANDEQ","tXOREQ","tOREQ","tCONDTEST","tCONDSEP",
"tLOR","tLAND","tOR","tXOR","tAND","tEQ","tNE","tLT","tLE","tGE","tGT","tRSHIFT",
"tLSHIFT","tRSHIFTFILL","tPLUS","tMINUS","tMULTIPLY","tDIVIDE","tMOD","tNOT",
"tNEG","tONESCOMP","tINCREMENT","tDECREMENT","tDOT","tRIGHTBRACKET","tLEFTBRACKET",
"'('","')'","'{'","'}'","script","funcs","func","bgnFunc","args","stmntblk",
"stmnts","stmnt","return","if","for","break","continue","with","while","varentry",
"varentrys","varstmt","opt_expr","comma_expr","expr","params","funccall","objmethodcall",
"newcall","leftvar","objmemberaccess","arrayderef","var","varassign",""
};
#endif

static const short yyr1[] = {     0,
    67,    67,    68,    68,    69,    70,    71,    71,    71,    72,
    72,    72,    73,    73,    73,    74,    74,    74,    74,    74,
    74,    74,    74,    74,    74,    75,    75,    76,    76,    77,
    77,    78,    79,    80,    81,    82,    82,    83,    83,    83,
    84,    85,    85,    86,    86,    87,    87,    87,    87,    87,
    87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
    87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
    87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
    87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
    87,    87,    87,    87,    88,    88,    88,    89,    90,    91,
    92,    92,    92,    93,    94,    95,    96
};

static const short yyr2[] = {     0,
     1,     0,     2,     1,     6,     1,     3,     1,     0,     3,
     2,     1,     2,     1,     0,     1,     1,     1,     2,     2,
     1,     2,     2,     2,     1,     2,     1,     5,     7,     9,
     7,     1,     1,     5,     5,     1,     3,     3,     1,     0,
     2,     1,     0,     1,     3,     3,     2,     2,     2,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     2,     2,     2,     2,     5,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     3,     1,     0,     4,     6,     5,
     1,     1,     1,     3,     4,     1,     1
};

static const short yydefact[] = {     2,
     0,     1,     4,     6,     0,     3,     9,     8,     0,     0,
     0,     7,    87,    86,   106,     0,     0,     0,     0,    32,
    33,    27,     0,    40,    88,    25,     0,     0,     0,     0,
     0,     0,     0,     5,    12,     0,    16,    17,     0,     0,
    21,    18,     0,     0,    44,    90,    89,    91,     0,    92,
    93,    94,   103,    97,     0,    43,     0,     0,    26,     0,
    36,    39,    41,    47,    48,    49,    62,    63,     0,    11,
     0,    14,    20,    22,    23,    24,    19,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    96,     0,     0,     0,    42,    94,
     0,     0,    97,     0,     0,    46,    10,    13,    45,     0,
    74,    73,    81,    82,    80,    67,    68,    69,    70,    71,
    72,    84,    83,    85,    75,    76,    77,    78,    79,   104,
     0,    50,    51,    52,    53,    54,    55,    60,    59,    61,
    56,    58,    57,     0,    98,     0,    43,     0,     0,     0,
     0,    37,    38,     0,    97,   105,    95,    28,     0,     0,
    35,    34,   100,    66,     0,     0,    43,     0,    99,    29,
     0,    31,     0,    30,     0,     0,     0
};

static const short yydefgoto[] = {   195,
     2,     3,     5,     9,    34,    71,    35,    36,    37,    38,
    39,    40,    41,    42,    62,    63,    43,   118,    44,    45,
   116,    46,    47,    48,    49,    50,    51,    52,    53
};

static const short yypact[] = {     6,
    19,     6,-32768,-32768,   -38,-32768,    21,-32768,   -21,    25,
   199,-32768,-32768,-32768,   198,   -32,   -15,   -12,   -11,-32768,
-32768,    24,    36,    44,-32768,-32768,    24,    24,    24,    24,
    24,    24,     2,-32768,-32768,    34,-32768,-32768,    37,    41,
-32768,-32768,    43,    45,   229,-32768,-32768,-32768,   393,   404,
   431,-32768,-32768,    24,    24,    24,    24,    24,-32768,    10,
    67,-32768,    54,    11,    11,    11,    11,    11,    13,-32768,
   135,-32768,-32768,-32768,-32768,-32768,-32768,    24,    24,    24,
    24,    24,    24,    24,    24,    24,    24,    24,    24,    24,
    24,    24,    24,    24,    24,    24,    24,    24,-32768,-32768,
    73,    24,    24,    24,    24,    24,    24,    24,    24,    24,
    24,    24,    24,    24,   345,   -19,    20,    64,-32768,    74,
    46,   257,    24,    24,    44,-32768,-32768,-32768,-32768,   317,
   454,   478,   501,   523,   544,   563,   563,   185,   185,   185,
   185,    76,    76,    76,   387,   387,    11,    11,    11,    48,
    47,   345,   345,   345,   345,   345,   345,   345,   345,   345,
   345,   345,   345,    24,-32768,   199,    24,    24,   199,   199,
   -18,   345,-32768,    24,    24,-32768,   345,    98,   103,   286,
-32768,-32768,-32768,   429,   -14,   199,    24,   199,-32768,-32768,
    61,-32768,   199,-32768,   131,   141,-32768
};

static const short yypgoto[] = {-32768,
-32768,   140,-32768,-32768,    -8,-32768,   -24,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    18,-32768,-32768,  -155,   -22,     9,
  -121,-32768,-32768,-32768,-32768,-32768,-32768,    91,-32768
};


#define	YYLAST		624


static const short yytable[] = {    59,
    10,   171,   164,   164,    13,    14,    15,   164,    72,    69,
    16,   179,    17,     1,    18,    19,    20,    21,    22,    23,
    24,    25,    26,     4,     7,     8,    13,    14,    15,    12,
    55,   191,   117,   119,   121,    64,    65,    66,    67,    68,
    60,    23,    11,    25,   165,   183,   128,    56,    61,   189,
    57,    58,    27,   185,    73,   129,    28,    74,    29,    30,
    31,    75,   115,    76,    32,    77,   122,    70,    99,   100,
   101,   102,   123,   124,    27,   125,   126,   150,    28,   151,
    29,    30,    31,   166,   167,   168,    32,   130,   131,   132,
   133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
   143,   144,   145,   146,   147,   148,   149,   186,   176,   169,
   175,   152,   153,   154,   155,   156,   157,   158,   159,   160,
   161,   162,   163,   187,   193,    94,    95,    96,    97,    98,
   196,   115,   172,    99,   100,   101,   102,    13,    14,    15,
   197,     6,   173,    16,   119,    17,   120,    18,    19,    20,
    21,    22,    23,    24,    25,    26,     0,   178,     0,     0,
   181,   182,     0,     0,   119,     0,     0,     0,     0,     0,
     0,     0,   177,     0,     0,     0,   180,   190,     0,   192,
     0,     0,   184,   115,   194,    27,     0,     0,     0,    28,
     0,    29,    30,    31,     0,     0,     0,    32,     0,     0,
   127,    13,    14,    15,  -107,     0,     0,    16,     0,    17,
     0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,
  -107,    91,    92,    93,    94,    95,    96,    97,    98,     0,
     0,     0,    99,   100,   101,   102,     0,     0,     0,    27,
    78,     0,     0,    28,     0,    29,    30,    31,     0,     0,
    54,    32,    79,    33,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,     0,     0,     0,    99,   100,   101,   102,
    79,     0,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
    98,     0,     0,     0,    99,   100,   101,   102,     0,    79,
   170,    80,    81,    82,    83,    84,    85,    86,    87,    88,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     0,     0,     0,    99,   100,   101,   102,     0,     0,   188,
    79,   174,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
    98,     0,     0,     0,    99,   100,   101,   102,    79,     0,
    80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,   103,
     0,     0,    99,   100,   101,   102,     0,     0,     0,     0,
  -101,     0,     0,     0,     0,   104,   105,   106,   107,   108,
   109,   110,   111,   112,   113,   114,  -101,  -101,  -101,  -101,
  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -102,    96,    97,
    98,     0,     0,     0,    99,   100,   101,   102,     0,     0,
     0,     0,     0,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
  -102,  -102,  -102,  -102,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,     0,     0,     0,    99,   100,   101,   102,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,     0,     0,
     0,    99,   100,   101,   102,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,     0,     0,     0,    99,   100,   101,   102,    83,
    84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
    94,    95,    96,    97,    98,     0,     0,     0,    99,   100,
   101,   102,    84,    85,    86,    87,    88,    89,    90,    91,
    92,    93,    94,    95,    96,    97,    98,     0,     0,     0,
    99,   100,   101,   102,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,     0,     0,
     0,    99,   100,   101,   102,    87,    88,    89,    90,    91,
    92,    93,    94,    95,    96,    97,    98,     0,     0,     0,
    99,   100,   101,   102
};

static const short yycheck[] = {    22,
    22,   123,    22,    22,     3,     4,     5,    22,    33,    32,
     9,   167,    11,     8,    13,    14,    15,    16,    17,    18,
    19,    20,    21,     5,    63,     5,     3,     4,     5,     5,
    63,   187,    55,    56,    57,    27,    28,    29,    30,    31,
     5,    18,    64,    20,    64,    64,    71,    63,     5,    64,
    63,    63,    51,   175,    21,    78,    55,    21,    57,    58,
    59,    21,    54,    21,    63,    21,    58,    66,    58,    59,
    60,    61,    63,     7,    51,    22,    64,     5,    55,   102,
    57,    58,    59,    64,    21,    12,    63,    79,    80,    81,
    82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
    92,    93,    94,    95,    96,    97,    98,    10,    62,    64,
    63,   103,   104,   105,   106,   107,   108,   109,   110,   111,
   112,   113,   114,    21,    64,    50,    51,    52,    53,    54,
     0,   123,   124,    58,    59,    60,    61,     3,     4,     5,
     0,     2,   125,     9,   167,    11,    56,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    -1,   166,    -1,    -1,
   169,   170,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   164,    -1,    -1,    -1,   168,   186,    -1,   188,
    -1,    -1,   174,   175,   193,    51,    -1,    -1,    -1,    55,
    -1,    57,    58,    59,    -1,    -1,    -1,    63,    -1,    -1,
    66,     3,     4,     5,     7,    -1,    -1,     9,    -1,    11,
    -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
    -1,    -1,    58,    59,    60,    61,    -1,    -1,    -1,    51,
    22,    -1,    -1,    55,    -1,    57,    58,    59,    -1,    -1,
    63,    63,    34,    65,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,    61,
    34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    -1,    -1,    -1,    58,    59,    60,    61,    -1,    34,
    64,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
    -1,    -1,    -1,    58,    59,    60,    61,    -1,    -1,    64,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    -1,    -1,    -1,    58,    59,    60,    61,    34,    -1,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,     7,
    -1,    -1,    58,    59,    60,    61,    -1,    -1,    -1,    -1,
     7,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,     7,    52,    53,
    54,    -1,    -1,    -1,    58,    59,    60,    61,    -1,    -1,
    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,    61,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    -1,    -1,
    -1,    58,    59,    60,    61,    38,    39,    40,    41,    42,
    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    -1,    -1,    -1,    58,    59,    60,    61,    39,
    40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    -1,    -1,    -1,    58,    59,
    60,    61,    40,    41,    42,    43,    44,    45,    46,    47,
    48,    49,    50,    51,    52,    53,    54,    -1,    -1,    -1,
    58,    59,    60,    61,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    -1,    -1,
    -1,    58,    59,    60,    61,    43,    44,    45,    46,    47,
    48,    49,    50,    51,    52,    53,    54,    -1,    -1,    -1,
    58,    59,    60,    61
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
//HG #line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not __GNUC__ */
#if HAVE_ALLOCA_H
#include <alloca.h>
#else /* not HAVE_ALLOCA_H */
#ifdef _AIX
 #pragma alloca
#else /* not _AIX */
//HG
#include <malloc.h>
// #define alloca(X) _alloca(X)

//char *alloca ();
#endif /* not _AIX */
#endif /* not HAVE_ALLOCA_H */
#endif /* not __GNUC__ */

extern int yylex();
//extern void yyerror();

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 2:
#line 107 "parse.ypp"
{ yyval.funcList = NULL; ;
    break;}
case 3:
#line 110 "parse.ypp"
{ yyval.funcList = NULL; ;
    break;}
case 4:
#line 111 "parse.ypp"
{ yyval.funcList = NULL; ;
    break;}
case 5:
#line 115 "parse.ypp"
{ yyvsp[-4].funcDef->endDefinition(yyvsp[0].stmntList); yyval.funcDef = yyvsp[-4].funcDef; ;
    break;}
case 6:
#line 118 "parse.ypp"
{ yyval.funcDef = VsParseTree::beginFunction(yyvsp[0].string); free((void *)yyvsp[0].string); ;
    break;}
case 7:
#line 121 "parse.ypp"
{ VsFunctionDef::addArg(yyvsp[0].string); yyval.number = yyvsp[-2].number + 1.; free((void *)yyvsp[0].string); ;
    break;}
case 8:
#line 122 "parse.ypp"
{ VsFunctionDef::addArg(yyvsp[0].string); yyval.number = 1.; free((void *)yyvsp[0].string); ;
    break;}
case 9:
#line 123 "parse.ypp"
{ yyval.number = 0.; ;
    break;}
case 10:
#line 127 "parse.ypp"
{ yyval.stmntList = yyvsp[-1].stmntList; ;
    break;}
case 11:
#line 128 "parse.ypp"
{ yyval.stmntList = new VsStatementList; ;
    break;}
case 12:
#line 129 "parse.ypp"
{ yyval.stmntList = new VsStatementList; if (yyvsp[0].stmnt) yyval.stmntList->append(yyvsp[0].stmnt); ;
    break;}
case 13:
#line 134 "parse.ypp"
{ if (yyvsp[0].stmnt) yyvsp[-1].stmntList->append(yyvsp[0].stmnt); yyval.stmntList = yyvsp[-1].stmntList; ;
    break;}
case 14:
#line 135 "parse.ypp"
{ yyval.stmntList = new VsStatementList(); if (yyvsp[0].stmnt) yyval.stmntList->append(yyvsp[0].stmnt); ;
    break;}
case 15:
#line 136 "parse.ypp"
{ yyval.stmntList =  new VsStatementList(); ;
    break;}
case 16:
#line 141 "parse.ypp"
{ yyval.stmnt = yyvsp[0].stmnt; ;
    break;}
case 17:
#line 142 "parse.ypp"
{ yyval.stmnt = yyvsp[0].stmnt; ;
    break;}
case 18:
#line 143 "parse.ypp"
{ yyval.stmnt = yyvsp[0].stmnt; ;
    break;}
case 19:
#line 144 "parse.ypp"
{ yyval.stmnt = yyvsp[-1].expr; ;
    break;}
case 20:
#line 145 "parse.ypp"
{ yyval.stmnt = yyvsp[-1].stmnt; ;
    break;}
case 21:
#line 146 "parse.ypp"
{ yyval.stmnt = yyvsp[0].stmnt; ;
    break;}
case 22:
#line 147 "parse.ypp"
{ yyval.stmnt = yyvsp[-1].stmnt; ;
    break;}
case 23:
#line 148 "parse.ypp"
{ yyval.stmnt = yyvsp[-1].stmnt; ;
    break;}
case 24:
#line 149 "parse.ypp"
{ yyval.stmnt = yyvsp[-1].stmnt; ;
    break;}
case 25:
#line 150 "parse.ypp"
{ yyval.stmnt  = NULL; ;
    break;}
case 26:
#line 159 "parse.ypp"
{ yyval.stmnt = new VsReturn(yyvsp[0].expr); ;
    break;}
case 27:
#line 160 "parse.ypp"
{ yyval.stmnt = new VsReturn(NULL); ;
    break;}
case 28:
#line 163 "parse.ypp"
{ yyval.stmnt = new VsIf(yyvsp[-2].expr,yyvsp[0].stmntList,NULL);;
    break;}
case 29:
#line 164 "parse.ypp"
{ yyval.stmnt = new VsIf(yyvsp[-4].expr,yyvsp[-2].stmntList,yyvsp[0].stmntList);;
    break;}
case 30:
#line 168 "parse.ypp"
{  yyval.stmnt = new VsFor(yyvsp[-6].expr, yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].stmntList); ;
    break;}
case 31:
#line 170 "parse.ypp"
{ 
        yyerror("<for..in> statement not implemented"); 
        yyval.stmnt = new VsForIn(yyvsp[-4].var, yyvsp[-2].expr, yyvsp[0].stmntList); 
        ;
    break;}
case 32:
#line 178 "parse.ypp"
{ yyval.stmnt = new VsBreak(); ;
    break;}
case 33:
#line 182 "parse.ypp"
{ yyval.stmnt = new VsContinue(); ;
    break;}
case 34:
#line 186 "parse.ypp"
{ yyerror("<with> statement not implemented"); yyval.stmnt = new VsWith(yyvsp[-2].expr, yyvsp[0].stmntList); ;
    break;}
case 35:
#line 190 "parse.ypp"
{ yyval.stmnt = new VsWhile(yyvsp[-2].expr, yyvsp[0].stmntList); ;
    break;}
case 36:
#line 194 "parse.ypp"
{ yyval.stmnt = new VsAssign( VsFunctionDef::makeLocalVar(yyvsp[0].string), NULL); free((void *)yyvsp[0].string); ;
    break;}
case 37:
#line 195 "parse.ypp"
{ yyval.stmnt = new VsAssign(VsFunctionDef::makeLocalVar(yyvsp[-2].string), yyvsp[0].expr);free((void*)yyvsp[-2].string); ;
    break;}
case 38:
#line 200 "parse.ypp"
{ if (yyvsp[0].stmnt) yyvsp[-2].stmntList->append(yyvsp[0].stmnt); yyval.stmntList = yyvsp[-2].stmntList; ;
    break;}
case 39:
#line 201 "parse.ypp"
{ yyval.stmntList = new VsStatementList(); if (yyvsp[0].stmnt) yyval.stmntList->append(yyvsp[0].stmnt); ;
    break;}
case 40:
#line 202 "parse.ypp"
{ yyval.stmntList = NULL; ;
    break;}
case 41:
#line 206 "parse.ypp"
{ yyval.stmnt = new VsVarStatement(yyvsp[0].stmntList); ;
    break;}
case 42:
#line 209 "parse.ypp"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 43:
#line 210 "parse.ypp"
{ yyval.expr = NULL; ;
    break;}
case 44:
#line 213 "parse.ypp"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 45:
#line 214 "parse.ypp"
{ yyval.expr = new VsComma(yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 46:
#line 217 "parse.ypp"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 47:
#line 218 "parse.ypp"
{ yyval.expr = new VsUArithOp(yyvsp[0].expr, tMINUS); ;
    break;}
case 48:
#line 219 "parse.ypp"
{ yyval.expr = new VsULogicOp(yyvsp[0].expr, tNOT); ;
    break;}
case 49:
#line 220 "parse.ypp"
{ yyval.expr = new VsUArithOp(yyvsp[0].expr, tONESCOMP); ;
    break;}
case 50:
#line 221 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, yyvsp[0].expr); ;
    break;}
case 51:
#line 222 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tPLUS));;
    break;}
case 52:
#line 223 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tMINUS)); ;
    break;}
case 53:
#line 224 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tMULTIPLY));;
    break;}
case 54:
#line 225 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tDIVIDE)); ;
    break;}
case 55:
#line 226 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tMOD)); ;
    break;}
case 56:
#line 227 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tAND)); ;
    break;}
case 57:
#line 228 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tOR)); ;
    break;}
case 58:
#line 229 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tXOR)); ;
    break;}
case 59:
#line 230 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tLSHIFT)); ;
    break;}
case 60:
#line 231 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tRSHIFT)); ;
    break;}
case 61:
#line 232 "parse.ypp"
{ yyval.expr = new VsAssign(yyvsp[-2].var, new VsArith(yyvsp[-2].var, yyvsp[0].expr, tRSHIFTFILL)); ;
    break;}
case 62:
#line 233 "parse.ypp"
{ yyval.expr = new VsIncDec(yyvsp[0].expr, TRUE,  TRUE); ;
    break;}
case 63:
#line 234 "parse.ypp"
{ yyval.expr = new VsIncDec(yyvsp[0].expr, FALSE, TRUE);;
    break;}
case 64:
#line 235 "parse.ypp"
{ yyval.expr = new VsIncDec(yyvsp[-1].expr, TRUE,  FALSE); ;
    break;}
case 65:
#line 236 "parse.ypp"
{ yyval.expr = new VsIncDec(yyvsp[-1].expr, FALSE, FALSE); ;
    break;}
case 66:
#line 237 "parse.ypp"
{ yyval.expr = new VsCondExpr(yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 67:
#line 238 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tEQ); ;
    break;}
case 68:
#line 239 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tNE); ;
    break;}
case 69:
#line 240 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tLT); ;
    break;}
case 70:
#line 241 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tLE); ;
    break;}
case 71:
#line 242 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tGE); ;
    break;}
case 72:
#line 243 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tGT); ;
    break;}
case 73:
#line 244 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tLAND); ;
    break;}
case 74:
#line 245 "parse.ypp"
{ yyval.expr = new VsLogicOp(yyvsp[-2].expr, yyvsp[0].expr, tLOR); ;
    break;}
case 75:
#line 246 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tPLUS); ;
    break;}
case 76:
#line 247 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tMINUS); ;
    break;}
case 77:
#line 248 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tMULTIPLY); ;
    break;}
case 78:
#line 249 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tDIVIDE); ;
    break;}
case 79:
#line 250 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tMOD); ;
    break;}
case 80:
#line 251 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tAND); ;
    break;}
case 81:
#line 252 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tOR); ;
    break;}
case 82:
#line 253 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tXOR); ;
    break;}
case 83:
#line 254 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tLSHIFT); ;
    break;}
case 84:
#line 255 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tRSHIFT); ;
    break;}
case 85:
#line 256 "parse.ypp"
{ yyval.expr = new VsArith(yyvsp[-2].expr, yyvsp[0].expr, tRSHIFTFILL); ;
    break;}
case 86:
#line 257 "parse.ypp"
{ yyval.expr = new VsConstant(yyvsp[0].string); ;
    break;}
case 87:
#line 258 "parse.ypp"
{ yyval.expr = new VsConstant(yyvsp[0].number); ;
    break;}
case 88:
#line 259 "parse.ypp"
{ yyval.expr = new VsConstant(VsValue::nothing); ;
    break;}
case 89:
#line 260 "parse.ypp"
{ yyval.expr = yyvsp[0].objMethodCall; ;
    break;}
case 90:
#line 261 "parse.ypp"
{ yyval.expr = yyvsp[0].funcCall; ;
    break;}
case 91:
#line 262 "parse.ypp"
{ yyval.expr = yyvsp[0].newCall; ;
    break;}
case 92:
#line 263 "parse.ypp"
{ yyval.expr = yyvsp[0].objMemberAccess; ;
    break;}
case 93:
#line 264 "parse.ypp"
{ yyval.expr = yyvsp[0].arrayvar; ;
    break;}
case 94:
#line 265 "parse.ypp"
{ yyval.expr = yyvsp[0].var; ;
    break;}
case 95:
#line 268 "parse.ypp"
{ if (yyvsp[0].expr) yyvsp[-2].exprList->append(yyvsp[0].expr); yyval.exprList = yyvsp[-2].exprList; ;
    break;}
case 96:
#line 269 "parse.ypp"
{ yyval.exprList = new VsExprList; if (yyvsp[0].expr) yyval.exprList->append(yyvsp[0].expr); ;
    break;}
case 97:
#line 270 "parse.ypp"
{ yyval.exprList = new VsExprList; ;
    break;}
case 98:
#line 274 "parse.ypp"
{ yyval.funcCall = NULL; 
          VsFunctionDef *funcDef = VsParseTree::getCurrent()->findFunction(yyvsp[-3].string);  
          if (funcDef != NULL) {
              yyval.funcCall = new VsFunctionCall( funcDef, *(yyvsp[-1].exprList)); 
          } else if (VsFunctionCall::isAnUnimplementedFunction(yyvsp[-3].string)) {
              char msg[256];
              sprintf(msg, "Built-in function \'%s\' is not implemented", yyvsp[-3].string);
              yyerror(msg);
          } else {
              yyval.funcCall = new VsFunctionCallByName(yyvsp[-3].string,*(yyvsp[-1].exprList));
              VsParseTree::getCurrent()->addUnresolvedFunctionCall((VsFunctionCallByName *) yyval.funcCall);                
              /*
              if (c == NULL) {
                  char msg[256];
                  sprintf(msg, "Function \'%s\' must be defined before it can be used", $1);
                  yyerror(msg);
              } else $$ = c;    
              */
          }
          free((void *)yyvsp[-3].string);
        ;
    break;}
case 99:
#line 298 "parse.ypp"
{
        yyval.objMethodCall = NULL;
        yyval.objMethodCall = new VsMethodCall (yyvsp[-5].expr, yyvsp[-3].string, *(yyvsp[-1].exprList)); 
        free((void *)yyvsp[-3].string); 
        ;
    break;}
case 100:
#line 306 "parse.ypp"
{
        yyval.newCall = NULL;
        yyval.newCall = new VsNewCall (yyvsp[-3].string, *(yyvsp[-1].exprList)); 
        free((void *)yyvsp[-3].string); 
        ;
    break;}
case 101:
#line 315 "parse.ypp"
{ yyval.var = yyvsp[0].objMemberAccess; ;
    break;}
case 102:
#line 317 "parse.ypp"
{ yyval.var = yyvsp[0].arrayvar; ;
    break;}
case 103:
#line 319 "parse.ypp"
{ yyval.var = yyvsp[0].var; ;
    break;}
case 104:
#line 323 "parse.ypp"
{ 
            yyval.objMemberAccess = NULL;
            yyval.objMemberAccess = new VsMemberAccess (yyvsp[-2].expr, yyvsp[0].string); 
            free((void *)yyvsp[0].string); 
        ;
    break;}
case 105:
#line 331 "parse.ypp"
{ yyval.arrayvar = new VsArrayVar(yyvsp[-3].expr, yyvsp[-1].expr); ;
    break;}
case 106:
#line 334 "parse.ypp"
{ 
        yyval.var = NULL;
        if (strcmp(yyvsp[0].string, "Math") != 0) {
            yyval.var = VsFunctionDef::findIdentifier(yyvsp[0].string); 
        }
        free((void *)yyvsp[0].string); ;
    break;}
case 107:
#line 342 "parse.ypp"
{ 
        yyval.var = NULL;
        if (strcmp(yyvsp[0].string, "Math") != 0) {
            yyval.var = VsFunctionDef::findIdentifier(yyvsp[0].string,TRUE); 
        }
        free((void *)yyvsp[0].string); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 351 "parse.ypp"
