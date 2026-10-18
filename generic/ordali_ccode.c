#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tcl.h>

/*
 * Tcl 9 compatibility: sizes (string/list lengths) are Tcl_Size in
 * Tcl 9 (ptrdiff_t, 64 bits) instead of int. Tcl 8.6 does not define
 * Tcl_Size, so provide it here so that the same code builds against
 * both 8.6 and 9.x (see https://wiki.tcl-lang.org/page/Porting+extensions+to+Tcl+9).
 */
#ifndef TCL_SIZE_MAX
#include <limits.h>
#define TCL_SIZE_MAX INT_MAX
#ifndef Tcl_Size	/* TEA >= 3.13 already passes -DTcl_Size=int */
typedef int Tcl_Size;
#endif
#define TCL_SIZE_MODIFIER ""
#endif


/*
 * Prototypes
 */
int Ccode_DoBootstrap (ClientData cdata,
		       Tcl_Interp *interp,
		       int objc,
		       Tcl_Obj *const objv[]);

int Ccode_DonnePosSG (ClientData cdata, 
		      Tcl_Interp *interp, 
		      int objc, 
		      Tcl_Obj *const objv[]);

int Ccode_SumOfPairs (ClientData cdata, 
		      Tcl_Interp *interp, 
		      int objc, 
		      Tcl_Obj *const objv[]);

int Ccode_Pairwise (ClientData cdata, 
		    Tcl_Interp *interp, 
		    int objc, 
		    Tcl_Obj *const objv[]);
int Ccode_Pairwise2 (ClientData cdata, 
		     Tcl_Interp *interp, 
		     int objc, 
		     Tcl_Obj *const objv[]);

double SeqMatch (char *seq1, 
		 char *seq2, 
		 Tcl_Size l);
double SeqMatch2(char *seq1,
		 char *seq2,
		 Tcl_Size l);

#include "matrices.inc"		 

/*
 * fin Prototypes
 */


double SeqMatch(char *seq1, 
		char *seq2, 
		Tcl_Size l) 
{
  Tcl_Size r, i;
  
  r = 0;
  for (i=0;i<l;i++) {
    if (seq1[i] == seq2[i] && seq1[i] != '.') r++;
  }
  
  return (double)r;
}


double SeqMatch2(char *seq1,
		 char *seq2,
		 Tcl_Size l)
{
  Tcl_Size r, i, total;
  double score;
  
  r = total = 0;
  for (i=0;i<l;i++) {
    if (seq1[i] != '.' && seq2[i] != '.') {
      total++;
      if (seq1[i] == seq2[i]) r++;
    }
  }
  
  if (total==0) 
    score=0.0;
  else 
    score = 100.0*(double)r/(double)total;

  return score;
}


int Ccode_DoBootstrap (ClientData cdata,
		       Tcl_Interp *interp,
		       int objc,
		       Tcl_Obj *const objv[]) 
{
  char *pil, **tseq;
  int ip, seed, useIdx = 0;
  Tcl_Size nseq, i, j, lgSeq, pilLen, nidx = 0;
  double dist, rd, **tres, l1, l2, *tlg;
  Tcl_Obj *seqObj, *Mres;	
  Tcl_Obj **idxv = NULL;

  /* Bug fix: objc was never checked before accessing objv[1] below.
   * Calling this command with too few arguments read past the end of
   * the objv[] array (an array of Tcl_Obj* sized objc), dereferencing
   * a garbage pointer -- a real crash risk on a simple scripting
   * mistake. This missing check was systemic across every command in
   * this file; fixed in all of them. */
  /* Optional 2nd argument, as used by the Tcl callers
   * (ordali_ccode.tcl) :
   * - an integer : seed of the random generator (srand), called by
   *   C_DoBootstrap with a new seed drawn from Tcl rand() at each
   *   replicate (reproducible, and independent replicates) ;
   * - a list of lgSeq column indices (0..lgSeq-1) : the columns to
   *   draw, as T_DoBootstrap2 (C_DoBootstrap2, to compare C and Tcl).
   * Without it, the generator seeded at load time is used. */
  if (objc != 2 && objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "pillar-list ?seed|column-index-list?");
    return TCL_ERROR;
  }

  if (Tcl_ListObjLength(interp,objv[1],&lgSeq) != TCL_OK) 
    return TCL_ERROR;
  if (lgSeq == 0) {
    Mres = Tcl_NewListObj(0,NULL);
    Tcl_SetObjResult(interp,Mres);
    return TCL_OK;
  }

  if (objc == 3) {
    if (Tcl_ListObjGetElements(interp,objv[2],&nidx,&idxv) != TCL_OK)
      return TCL_ERROR;
    if (nidx == 1 && lgSeq != 1) {
      if (Tcl_GetIntFromObj(interp,idxv[0],&seed) != TCL_OK)
	return TCL_ERROR;
      srand((unsigned) seed);
    } else if (nidx == lgSeq) {
      /* check the indices before any allocation */
      for (i=0; i<nidx; i++) {
	if (Tcl_GetIntFromObj(interp,idxv[i],&ip) != TCL_OK)
	  return TCL_ERROR;
	if (ip < 0 || ip >= lgSeq) {
	  Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	      "column index %d out of range (0..%" TCL_SIZE_MODIFIER "d)",
	      ip, lgSeq-1));
	  return TCL_ERROR;
	}
      }
      useIdx = 1;
    } else {
      Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	  "second argument must be a seed or a list of %" TCL_SIZE_MODIFIER
	  "d column indices, got %" TCL_SIZE_MODIFIER "d elements",
	  lgSeq, nidx));
      return TCL_ERROR;
    }
  }

  Tcl_ListObjIndex(interp,objv[1],0,&seqObj);
  Tcl_GetStringFromObj(seqObj,&nseq);
  //fprintf(stderr,"nseq %d lseq %d\n",nseq,lgSeq);

  /* Allocate memory */
  tseq = (char **)ckalloc(nseq*sizeof(char *));
  tres = (double **)ckalloc(nseq*sizeof(double *));
  tlg = (double *)ckalloc(nseq*sizeof(double));
  for (i=0; i<nseq; i++) { 
    tseq[i] = (char *)ckalloc(lgSeq*sizeof(char));
    tres[i] = (double *)ckalloc(nseq*sizeof(double));
    tlg[i] = 0.0;
  }
  /* end memory allocation */
  
  /* create random sequence from pilars */
  for (i=0; i<lgSeq; i++) {
    if (useIdx) {
      /* indices already checked above */
      Tcl_GetIntFromObj(interp,idxv[i],&ip);
    } else {
      rd = ((double)rand()/((double)(RAND_MAX)+(double)(1)));
      ip = (int)(rd*(double)lgSeq);
    }
    //fprintf(stderr,"%d %f ",ip,rd);
    
    Tcl_ListObjIndex(interp,objv[1],ip,&seqObj);
    pil = Tcl_GetString(seqObj);
    /* Bug fix: "nseq" (the number of sequences) is derived from the
     * length of the FIRST pillar only; nothing verified that every
     * other pillar in the list has that same length. A shorter
     * pillar elsewhere in the list caused an out-of-bounds read on
     * "pil[j]" below. Reject the whole call cleanly instead of
     * reading past the end of a shorter pillar's string. */
    pilLen = (Tcl_Size)strlen(pil);
    if (pilLen < nseq) {
      Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	  "inconsistent pillar length at position %d: expected %"
	  TCL_SIZE_MODIFIER "d characters, got %" TCL_SIZE_MODIFIER "d",
	  ip, nseq, pilLen));
      for (j=0; j<nseq; j++) {
	  ckfree((char *)tseq[j]);
	  ckfree((char *)tres[j]);
      }
      ckfree((char *)tseq);
      ckfree((char *)tres);
      ckfree((char *)tlg);
      return TCL_ERROR;
    }
    for (j=0; j<nseq; j++) {
      tseq[j][i]=pil[j];
      if (pil[j] != '.') 
	tlg[j] += 1.;
    }
  }
  //fprintf(stderr,"\n");

  /*
    for (i=0; i<nseq; i++) {
    fprintf(stderr,"%d ",i);
    for (j=0; j<lgSeq; j++)
    fprintf(stderr,"%c",tseq[i][j]);
    fprintf(stderr,"\n");
    }
  */

  /* compute distance matrix */
  for (i=0; i<nseq; i++) {
    tres[i][i] = 0.0;
    l1=tlg[i];
    for (j=i+1; j<nseq; j++) {
      l2=tlg[j];
      if (l1 == 0 || l2 == 0) 
	dist = 1.0;
      else 
	dist = 1. - (SeqMatch(tseq[i],tseq[j],lgSeq)/(l1<l2?l1:l2));
      tres[i][j] = tres[j][i] = dist;
    }
  }
  
  /* output distance matrix */
  Mres = Tcl_NewListObj(0,NULL);
  for (i=0; i<nseq; i++) {
    for (j=0; j<nseq; j++) {
      Tcl_ListObjAppendElement(interp,Mres,Tcl_NewDoubleObj(tres[i][j]));
    }
  }
  
  Tcl_SetObjResult(interp,Mres);

  /* Desallocate memory */
  ckfree((char *)tlg);
  for (i=0;i<nseq;i++) {
    ckfree((char *)tseq[i]);
    ckfree((char *)tres[i]);
  }
  ckfree((char *)tres);
  ckfree((char *)tseq);

  return TCL_OK;
}

/*
  Search the global position corresponding to a
  sequence position
*/
int Ccode_DonnePosSG (ClientData cdata, 
		      Tcl_Interp *interp, 
		      int objc, 
		      Tcl_Obj *const objv[]) 
{
  char *seq;
  Tcl_Size len, gen, pos;
  int x;

  if (objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "sequence position");
    return TCL_ERROR;
  }

  seq = Tcl_GetString(objv[1]);
  len = (Tcl_Size)strlen(seq);
  /* Bug fix: return value was not checked. On a non-integer "position"
   * argument, x was left uninitialized (garbage stack value) and used
   * anyway -- not a crash here (the while loop's own bounds keep gen
   * within [0,len]), but a silently wrong result instead of a clean
   * Tcl error. */
  if (Tcl_GetIntFromObj(interp,objv[2],&x) != TCL_OK) {
    return TCL_ERROR;
  }

  pos = gen = 0;
  while (pos < x && gen < len) {
    if (seq[gen] != '.') pos++;
    gen++;
  }
  while (gen < len && seq[gen] == '.') gen++;
    
  Tcl_SetObjResult(interp, Tcl_NewWideIntObj((Tcl_WideInt)gen));

  return TCL_OK;
}

/* Arguments :
 * - matrix name
 * - List of sequences
 * - List of weights
 */
int Ccode_SumOfPairs (ClientData cdata,
		      Tcl_Interp *interp,
		      int objc,
		      Tcl_Obj *const objv[]) 
{
  char **tseq, *matName;
  Tcl_Size nseq, nwgt, strLen, seqLen, i, j, k;
  int **mat, ica, icb;
  Tcl_Obj *seqObj;
  double *wgt, sum;

  if (objc != 4) {
    Tcl_WrongNumArgs(interp, 1, objv, "matrixName sequenceList weightList");
    return TCL_ERROR;
  }

  matName = Tcl_GetString(objv[1]);
  
  if (Tcl_ListObjLength(interp,objv[2],&nseq) != TCL_OK) {
    return TCL_ERROR;
  }
  /* Bug fix: nseq==0 (empty list) was never rejected -- see above.
   * Also extended to nseq==1: with a single sequence there are no
   * pairs to sum, and "sum * 2.0 / (nseq*(nseq-1))" below would
   * divide by zero (1*0), silently producing NaN instead of a
   * sensible result. Both cases return 0.0 cleanly. */
  if (nseq < 2) {
    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(0.0));
    return TCL_OK;
  }
  /* Bug fix: the weight list's length was never checked against the
   * sequence list's length. If shorter, Tcl_ListObjIndex(objv[3], i,
   * ...) for i beyond its end also silently returns NULL, and
   * Tcl_GetDoubleFromObj(interp, NULL, ...) below crashed. */
  if (Tcl_ListObjLength(interp,objv[3],&nwgt) != TCL_OK) {
    return TCL_ERROR;
  }
  if (nwgt != nseq) {
    Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	"weight list length (%" TCL_SIZE_MODIFIER "d) does not match"
	" sequence list length (%" TCL_SIZE_MODIFIER "d)",
	nwgt, nseq));
    return TCL_ERROR;
  }

  Tcl_ListObjIndex(interp,objv[2],0,&seqObj);
  Tcl_GetStringFromObj(seqObj,&strLen);
  
  /* allocation */
  tseq = (char **)ckalloc(nseq*sizeof(char *));
  for (i=0; i<nseq; i++)
    tseq[i] = (char *)ckalloc((strLen+1)*sizeof(char));
  wgt =(double *)ckalloc(nseq*sizeof(double));  
  /* end allocation */
  
  /* fill in seqs and weights arrays */
  for (i=0; i<nseq; i++) {
    Tcl_ListObjIndex(interp,objv[2],i,&seqObj);
    /* Bug fix: strLen comes only from sequence 0; nothing verified
     * that every other sequence in the list has that same length.
     * A longer sequence anywhere else in the list overflowed
     * tseq[i], which is sized (strLen+1) -- a real heap buffer
     * overflow via strcpy() on a plausible caller mistake (same bug
     * class as the tkBiotext.c BiotextSequencesCmd fix). */
    Tcl_GetStringFromObj(seqObj, &seqLen);
    if (seqLen != strLen) {
      Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	  "all sequences must have the same length (sequence #%"
	  TCL_SIZE_MODIFIER "d has length %" TCL_SIZE_MODIFIER
	  "d, expected %" TCL_SIZE_MODIFIER "d)",
	  i, seqLen, strLen));
      for (j=0; j<=i; j++) {
	  ckfree((char *)tseq[j]);
      }
      ckfree((char *)tseq);
      ckfree((char *)wgt);
      return TCL_ERROR;
    }
    strcpy(tseq[i],Tcl_GetString(seqObj));

    Tcl_ListObjIndex(interp,objv[3],i,&seqObj);
    if (Tcl_GetDoubleFromObj(interp, seqObj,&wgt[i]) != TCL_OK) {
      for (j=0; j<=i; j++) {
	  ckfree((char *)tseq[j]);
      }
      ckfree((char *)tseq);
      ckfree((char *)wgt);
      return TCL_ERROR;
    }
  }
  
  mat = load_matrix(matName);
  /* Bug fix: load_matrix()'s return value was never checked. If it
   * can return NULL for an unknown/invalid matrix name (its exact
   * contract lives in matrices.inc, not shown here), every
   * "mat[ica][icb]" access below would crash; guard against it
   * defensively either way. */
  if (mat == NULL) {
    Tcl_SetObjResult(interp, Tcl_ObjPrintf("unknown scoring matrix \"%s\"", matName));
    for (i=0; i<nseq; i++) {
	ckfree((char *)tseq[i]);
    }
    ckfree((char *)tseq);
    ckfree((char *)wgt);
    return TCL_ERROR;
  }

  sum = 0.0;
  for (i=0; i<nseq; i++) {
    for (j=i+1; j<nseq; j++) {
      for (k=0; k<strLen; k++) {
	if (tseq[i][k] != '.' && tseq[j][k] != '.') {
	  ica = aa2int[tseq[i][k]-'A'];
	  icb = aa2int[tseq[j][k]-'A'];
	  sum = sum + wgt[i]*wgt[j]*(double)mat[ica][icb];
	}
      }
    }
  }
  sum = sum * 2.0 / ((double)nseq*((double)nseq-1));
  free_matrix(mat);

  for (i=0; i<nseq; i++) {
    ckfree((char *)tseq[i]);
  }
  ckfree((char *)tseq);
  ckfree((char *)wgt);
  
  Tcl_SetObjResult(interp,Tcl_NewDoubleObj(sum));
  return TCL_OK;
}


int Ccode_Pairwise (ClientData cdata, 
		    Tcl_Interp *interp, 
		    int objc, 
		    Tcl_Obj *const objv[]) 
{
  char **tseq;
  Tcl_Size nseq, i, j, strLen, seqLen, l;
  Tcl_Obj *seqObj, *Tpci, *pair;
  double **tres, *tlg, pcid, l1, l2;

  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "sequenceList");
    return TCL_ERROR;
  }

  if (Tcl_ListObjLength(interp,objv[1],&nseq) != TCL_OK) {
    return TCL_ERROR;
  }
  /* Bug fix: same nseq==0 crash as Ccode_SumOfPairs -- Tcl_ListObjIndex
   * on an empty list silently yields a NULL Tcl_Obj*, and
   * Tcl_GetStringFromObj(NULL, ...) right after crashed. */
  if (nseq == 0) {
    Tcl_SetObjResult(interp, Tcl_NewListObj(0, NULL));
    return TCL_OK;
  }

  Tcl_ListObjIndex(interp,objv[1],0,&seqObj);
  Tcl_GetStringFromObj(seqObj,&strLen);
  
  /* allocation */
  tseq =   (char **)ckalloc(nseq*sizeof(char *));
  tres = (double **)ckalloc(nseq*sizeof(double *));
  for (i=0; i<nseq; i++) { 
    tseq[i] = (char *)ckalloc((strLen+1)*sizeof(char));
    tres[i] = (double *)ckalloc(nseq*sizeof(double));
  }
  tlg =(double *)ckalloc(nseq*sizeof(double));  
  /* end allocation */
  
  /* fill in sequence and lengths arrays */
  for (i=0;i<nseq;i++) {
    Tcl_ListObjIndex(interp,objv[1],i,&seqObj);
    /* Bug fix: same heap-overflow risk as Ccode_SumOfPairs -- strLen
     * comes only from sequence 0; a longer sequence anywhere else in
     * the list overflowed tseq[i] via strcpy(). */
    Tcl_GetStringFromObj(seqObj, &seqLen);
    if (seqLen != strLen) {
      Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	  "all sequences must have the same length (sequence #%"
	  TCL_SIZE_MODIFIER "d has length %" TCL_SIZE_MODIFIER
	  "d, expected %" TCL_SIZE_MODIFIER "d)",
	  i, seqLen, strLen));
      for (j=0; j<=i; j++) {
	  ckfree((char *)tseq[j]);
	  ckfree((char *)tres[j]);
      }
      ckfree((char *)tseq);
      ckfree((char *)tres);
      ckfree((char *)tlg);
      return TCL_ERROR;
    }
    strcpy(tseq[i],Tcl_GetString(seqObj));
    l=0;
    for (j=0;j<strLen;j++) {
      if (tseq[i][j] != '.') l++;
    }
    tlg[i]=(double)l;
  }
  
  /* compute all pairwise identities */
  for (i=0; i<nseq-1; i++) {
    for (j=i+1; j<nseq; j++) {
      tres[i][j] = SeqMatch(tseq[i],tseq[j],strLen);
    }
  }
  
  /* create output list */
  Tpci = Tcl_NewListObj(0,NULL);
  for (i=0; i<nseq-1; i++) {
    l1=tlg[i];
    for (j=i+1; j<nseq; j++) {
      l2=tlg[j];
      pair = Tcl_NewListObj(0,NULL);
      
      pcid = tres[i][j]/(l1<=l2?l1:l2);
      
      Tcl_ListObjAppendElement(interp,pair,Tcl_NewDoubleObj(pcid));
      Tcl_ListObjAppendElement(interp,pair,Tcl_NewWideIntObj((Tcl_WideInt)l1));
      Tcl_ListObjAppendElement(interp,pair,Tcl_NewWideIntObj((Tcl_WideInt)l2));
      Tcl_ListObjAppendElement(interp,pair,Tcl_NewDoubleObj((l1<l2?(double)(l1)/l2:(double)(l2)/l1)));
      Tcl_ListObjAppendElement(interp,Tpci,pair);
    }
  }
  
  Tcl_SetObjResult(interp, Tpci);

  /* desallocation */
  ckfree((char *)tlg);
  for (i=0; i<nseq; i++) { 
    ckfree((char *)tres[i]);
    ckfree((char *)tseq[i]);
  }
  ckfree((char *)tres);
  ckfree((char *)tseq);
  /* fin desallocation */

  return TCL_OK;
}


int Ccode_Pairwise2 (ClientData cdata, 
		     Tcl_Interp *interp, 
		     int objc, 
		     Tcl_Obj *const objv[]) 
{
  char **tseq;
  Tcl_Size nseq, i, j, strLen, seqLen;
  Tcl_Obj *Tpci, *seqObj;
  double **tres;

  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "sequenceList");
    return TCL_ERROR;
  }

  if (Tcl_ListObjLength(interp,objv[1],&nseq) != TCL_OK) {
    return TCL_ERROR;
  }
  /* Bug fix: same nseq==0 crash as Ccode_SumOfPairs/Ccode_Pairwise. */
  if (nseq == 0) {
    Tcl_SetObjResult(interp, Tcl_NewListObj(0, NULL));
    return TCL_OK;
  }
  
  Tcl_ListObjIndex(interp,objv[1],0,&seqObj);
  Tcl_GetStringFromObj(seqObj,&strLen);
  
  /* Memory allocation */
  tseq = (char **)ckalloc(nseq*sizeof(char *));
  tres = (double **)ckalloc(nseq*sizeof(double *));
  for (i=0; i<nseq; i++) { 
    tseq[i] = (char *)ckalloc((strLen+1)*sizeof(char));
    tres[i] = (double *)ckalloc(nseq*sizeof(double));
  }
  
  for (i=0; i<nseq; i++) { 
    Tcl_ListObjIndex(interp,objv[1],i,&seqObj);
    /* Bug fix: same heap-overflow risk as Ccode_SumOfPairs/
     * Ccode_Pairwise -- strLen comes only from sequence 0. */
    Tcl_GetStringFromObj(seqObj, &seqLen);
    if (seqLen != strLen) {
      Tcl_SetObjResult(interp, Tcl_ObjPrintf(
	  "all sequences must have the same length (sequence #%"
	  TCL_SIZE_MODIFIER "d has length %" TCL_SIZE_MODIFIER
	  "d, expected %" TCL_SIZE_MODIFIER "d)",
	  i, seqLen, strLen));
      for (j=0; j<=i; j++) {
	  ckfree((char *)tseq[j]);
	  ckfree((char *)tres[j]);
      }
      ckfree((char *)tseq);
      ckfree((char *)tres);
      return TCL_ERROR;
    }
    strcpy(tseq[i],Tcl_GetString(seqObj));
  }

  /* fill results array */
  for (i=0; i<nseq-1; i++) {
    for (j=i+1; j<nseq; j++) {
      tres[i][j] = tres[j][i] = SeqMatch2(tseq[i],tseq[j],strLen);
    }
  }

  /* fill in tcl output list */
  Tpci = Tcl_NewListObj(0,NULL);
  for (i=0; i<nseq; i++) {
    tres[i][i]=100.0;
    for (j=0; j<nseq; j++)
      Tcl_ListObjAppendElement(interp,Tpci,Tcl_NewDoubleObj(tres[i][j]));
  }
  
  Tcl_SetObjResult(interp,Tpci);

  /* desallocate memory */
  for (i=0;i<nseq;i++) {
    ckfree((char *)tseq[i]);
    ckfree((char *)tres[i]);
  }
  ckfree((char *)tseq);
  ckfree((char *)tres);
  
  return TCL_OK;
}


DLLEXPORT int Tclordalie_Init(Tcl_Interp *interp) 
{
  /* Tcl 9: "8.6-" accepts any Tcl >= 8.6, including 9.x (a plain "8.6"
   * would only accept 8.x). The stub table must match the headers the
   * extension was compiled with (8.6 headers -> 8.6 runtime, 9.x
   * headers -> 9.x runtime). */
  if (Tcl_InitStubs(interp, "8.6-", 0) == NULL) {
    return TCL_ERROR;
  }

  /* Bug fix: srand() used to be called inside Ccode_DoBootstrap() on
   * every single call, seeded from time(NULL) (1-second resolution).
   * A bootstrap procedure is by nature called many times in a tight
   * loop to generate independent replicates; any two calls landing
   * within the same wall-clock second re-seeded the generator with
   * the exact same seed, producing IDENTICAL "random" resamplings
   * instead of independent ones -- silently corrupting the
   * statistical validity of the bootstrap analysis (e.g. duplicate
   * distance matrices across supposedly-independent replicates).
   * Seed once, here, at extension load time instead. */
  srand((unsigned) time(NULL));
    
  Tcl_CreateObjCommand(interp,"Ccode_DoBootstrap",Ccode_DoBootstrap,NULL,NULL);
  Tcl_CreateObjCommand(interp,"Ccode_SumOfPairs",Ccode_SumOfPairs,NULL,NULL);
  Tcl_CreateObjCommand(interp,"Ccode_Pairwise",Ccode_Pairwise,NULL,NULL);
  Tcl_CreateObjCommand(interp,"Ccode_Pairwise2",Ccode_Pairwise2,NULL,NULL);
  Tcl_CreateObjCommand(interp,"Ccode_DonnePosSG",Ccode_DonnePosSG,NULL,NULL);

  return Tcl_PkgProvide(interp,"tclordalie","0.1");
}

