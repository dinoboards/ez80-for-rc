#pragma printf = "%s %c %u %f %d %u %ld %lld %llu %lu"

/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */

#include "dhry.h"

#ifdef STATIC
#define REG static
Boolean Regb = true;
#define REGSTRING "static"
#else
#ifdef REGISTER
#define REG       register
Boolean Regb = true;
#define REGSTRING "register"
#else
#define REG
Boolean Regb = false;
#define REGSTRING "no"
#endif
#endif

/* Global Variables: */

Rec_Pointer Ptr_Glob, Next_Ptr_Glob;
int         Int_Glob;
Boolean     Bool_Glob;
char        Ch_1_Glob, Ch_2_Glob;
int         Arr_1_Glob[50];
int         Arr_2_Glob[50][50];

/* variables for time measurement: */

#define Too_Small_Time 2
/* Measurements should last at least about 2 seconds */

uint32_t Begin_Time, End_Time;

double_t User_Time, Microseconds, Dhrystones_Per_Second;

/* end of variables for time measurement */

Rec_Type malloc_1;
Rec_Type malloc_2;

void main(void)
/*****/

/* main program, corresponds to procedures        */
/* Main and Proc_0 in the Ada version             */
{
  One_Fifty        Int_1_Loc = 0;
  REG One_Fifty    Int_2_Loc = 0;
  One_Fifty        Int_3_Loc = 0;
  REG char         Ch_Index;
  Enumeration      Enum_Loc = Indent_Unknown;
  Str_30           Str_1_Loc;
  Str_30           Str_2_Loc;
  REG unsigned int Run_Index;
  unsigned int     Number_Of_Runs;

  /* Initializations */

  Next_Ptr_Glob = &malloc_1;
  Ptr_Glob      = &malloc_2;

  Ptr_Glob->Ptr_Comp                = Next_Ptr_Glob;
  Ptr_Glob->Discr                   = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp  = 40;
  strcpy(Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING");
  strcpy(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob[8][7] = 10;
  /* Was missing in published program. Without this statement,    */
  /* Arr_2_Glob [8][7] would have an undefined value.             */
  /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
  /* overflow may occur for this array element.                   */

  printf("Please give the number of runs through the benchmark: ");
  scanf("%d", &Number_Of_Runs);

  printf("\r\n\r\n");

  /***************/
  /* Start timer */
  /***************/

#ifdef TIMEFUNC
  Begin_Time = native_timer_start();
#endif

  TIMER_START();

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index) {
    Proc_5();
    Proc_4();
    /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc  = Ident_2;
    Bool_Glob = !Func_2(Str_1_Loc, Str_2_Loc);
    /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc) /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
      /* Int_3_Loc == 7 */
      Proc_7(Int_1_Loc, Int_2_Loc, &Int_3_Loc);
      /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
    /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8(Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
    /* Int_Glob == 5 */
    Proc_1(Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
    /* loop body executed twice */
    {
      if (Enum_Loc == Func_1(Ch_Index, 'C'))
      /* then, not executed */
      {
        Proc_6(Ident_1, &Enum_Loc);
        strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob  = Run_Index;
      }
    }
    /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
    /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2(&Int_1_Loc);
    /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/

  TIMER_STOP();

#ifdef TIMEFUNC
  End_Time = native_timer_stop();
#endif

#ifdef TIMEFUNC
  User_Time = (float)(End_Time - Begin_Time) / (float)sysget_tick_rate();

  if (User_Time < Too_Small_Time) {
    printf("Measured time too small (less than %d) to obtain meaningful results\n", Too_Small_Time);
    printf("Please increase number of runs\n");
    printf("\n");
  } else {
    Microseconds          = (double_t)User_Time * 1e6 / (double_t)Number_Of_Runs;
    Dhrystones_Per_Second = (double_t)Number_Of_Runs / (double_t)User_Time;

    printf("Total Duration in seconds:                  ");
    printf("%6.1f seconds\n", User_Time);
    printf("Microseconds for one run through Dhrystone: ");
    printf("%6.1f \n", Microseconds);
    printf("Dhrystones per Second:                      ");
    printf("%6.1f \n", Dhrystones_Per_Second);
    printf("\n");
  }
#endif
}

#ifndef STATIC
void Proc_1(REG Rec_Pointer Ptr_Val_Par)
#else
void    Proc_1(Rec_Pointer Ptr_Val_Par)
#endif
/******************/

/* executed once */
{
#ifndef STATIC
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
#else
  REG Rec_Pointer Next_Record;
  Next_Record = Ptr_Val_Par->Ptr_Comp;
#endif
  /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign(*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp               = Ptr_Val_Par->Ptr_Comp;
  Proc_3(&Next_Record->Ptr_Comp);
  /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                      == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
  /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6(Ptr_Val_Par->variant.var_1.Enum_Comp, &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7(Next_Record->variant.var_1.Int_Comp, 10, &Next_Record->variant.var_1.Int_Comp);
  } else /* not executed */
    structassign(*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */

void Proc_2(One_Fifty *Int_Par_Ref)
/* *Int_Par_Ref == 1, becomes 4 */
{
  One_Fifty   Int_Loc;
  Enumeration Enum_Loc = Indent_Unknown;

  Int_Loc = *Int_Par_Ref + 10;
  do
    if (Ch_1_Glob == 'A') {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc     = Ident_1;
    }
  while (Enum_Loc != Ident_1); /* true */
}

void Proc_3(Rec_Pointer *Ptr_Ref_Par)
/* Ptr_Ref_Par becomes Ptr_Glob */
{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7(10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */

void Proc_4(void) /* without parameters */
/*******/
/* executed once */
{
  Boolean Bool_Loc;
  Bool_Loc  = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */

void Proc_5(void) /* without parameters */
/*******/
/* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */
