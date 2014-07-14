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


extern YYSTYPE yylval;
