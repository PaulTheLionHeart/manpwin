// Worklist.h: interface for the CWorklist class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAXCALCWORK 12

struct workliststuff    // work list entry for std escape time engines
    {
    int xxstart;    // screen window for this entry
    int xxstop;
    int yystart;
    int yystop;
    int yybegin;    // start row within window, for 2pass/ssg resume
    int sym;        // if symmetry in window, prevents bad combines
    int pass;       // for 2pass and solid guessing
    };

typedef struct workliststuff        WorklistData;

class CWorklist
    {
    public:
    CWorklist();
    WorklistData	WorklistArray[MAXCALCWORK];

    virtual		~CWorklist();
    int			AddWorklist(int xfrom, int xto, int yfrom, int yto, int ybegin, int pass, int sym);
    void		CopyWorklist(int to, int from);

    short		num_worklist;
    int			workpass;
    int			totpasses;
    int			curpass;
    int			worksym;

    private:
    void		TidyWorklist();
    int			CombineWorklist();	// Look for 2 entries which can freely merge
    };

