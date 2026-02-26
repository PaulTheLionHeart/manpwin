#include <string.h>
#include <ctype.h>
#include <time.h>
#include <vector>

#include "parser.h"
#include "prototyp.h"
#include "..\ManpWIN64\Complex.h"
#include "..\ManpWIN64\fract.h"
#include "..\ManpWIN64\BigDouble.h"
#include "..\ManpWIN64\Arithmetic.h"

#pragma once

#ifndef MAXVARS
#define MAXVARS 64
#endif

#ifndef MAXSTORES
#define MAXSTORES 64
#endif

#ifndef MAXLOADS
#define MAXLOADS 64
#endif

struct Arg;
struct ConstArg;

struct PEND_OP
    {
    void(*f)(void);			// legacy compatibility (temporary)
//    OpCode op = OpCode::LEGACY;		// THIS IS THE FIX
    int p = 0;
    };

// This is the per-execution state you are building toward.
struct ParserExecContext
    {
    // ===== Execution state =====
    Arg* Arg1 = nullptr;
    Arg* Arg2 = nullptr;
    int Arg1i = -1;
    int Arg2i = -1;

    std::vector<Arg> s;
    size_t v_count = 0;
    bool sto_pops = true;

    int StoPtr = 0;
    int LodPtr = 0;
    int OpPtr = 0;
    int overflow = 0;
    int jump_cursor = 0;

    // ===== Jump / INIT state =====
    int LastInitOp = 0;
    int InitJumpIndex = 0;
    short uses_jump = 0;
    int jump_index = 0;

    // ===== Program definition =====
    unsigned int LastOp = 0;
    void(**f)(void) = nullptr;

    std::vector<ConstArg> v_storage;
    ConstArg* v = nullptr;

    // legacy pointer tables (still needed for now)
    std::vector<Arg*> Store;
    std::vector<Arg*> Load;

    // index tables (new VM)
    std::vector<uint16_t> StoreIdx;
    std::vector<uint16_t> LoadIdx;

    // jump table
    JUMP_CONTROL_ST jump_control[MAX_JUMPS];
    };

inline void VmTrapIf(bool cond, int tag, const char* file = nullptr, int line = 0)
    {
#ifdef _DEBUG
    if (cond)
	{
	char buf[256];
	sprintf_s(buf, "VM TRAP %d at %s:%d\n", tag, file ? file : "?", line);
	OutputDebugStringA(buf);
	__debugbreak();
	}
#else
    (void)cond; (void)tag; (void)file; (void)line;
#endif
    }

#define VMTRAP(cond, tag) VmTrapIf((cond), (tag), __FILE__, __LINE__)
class CParser 
    {
public:
//    CParser();

    void RunVmInit();               // executes INIT section
    void RunVmIter();               // executes ITER section
    void InitContextFromGlobals();

    static thread_local CParser* s_current;
    ParserExecContext m_ctx;        // keep your ctx here (Arg1/Arg2/Sto/Lod/Op/jump etc)

    int	m_execStoCount = 0;
    int	FormPerPixel();
    int	FormPerPixelFloat(Complex *zIn, Complex *cIn);
    int	DDFormPerPixel(DDComplex *zIn, DDComplex *qIn);
    int	QDFormPerPixel(QDComplex *zIn, QDComplex *qIn);
    int	Formula();
    int	FormulaFloat(Complex *zIn, Complex *cIn);
    int	DDFormula(DDComplex *zIn, DDComplex *cIn);
    int	QDFormula(QDComplex *zIn, QDComplex *cIn);




private:
    void ExecOpWithContext(ParserExecContext& ctx, void(*op)(void));
    void ResetVmRegsForInit();
    void ExecOpWithContext(void(*fn)());
    void RebuildLoadStorePointers();

    int m_InitOpPtr = 0;
    int m_InitLodPtr = 0;
    int m_InitStoPtr = 0;
    int m_InitJumpIndex = 0;
    int m_InitArg1i;
    int m_InitArg2i;

    };

// ParserCtxAccess.h (or inside ParserCtx.h)
#pragma once
struct ParserExecContext;
class CParser;

static const char* VmFnName(void(*fn)(void))
    {
    if (fn == StkLod) return "Lod";
    if (fn == StkSto) return "Sto";
    if (fn == StkJump) return "Jmp";
    if (fn == StkJumpLabel || fn == PtrStkJumpLabel) return "Lbl";
    if (fn == StkJumpOnFalse || fn == StkJumpOnFalse) return "JOF";
    if (fn == StkJumpOnTrue || fn == StkJumpOnTrue)  return "JOT";
    if (fn == PtrEndInit) return "EndInit";
    return "?";
    }

inline ParserExecContext& Ctx()
    {
    return CParser::s_current->m_ctx;
    }

extern	unsigned int /*posp, vsp, */LastOp;				// CAE fp made non-static
extern	struct	ConstArg *v;						// was static CAE fp
extern	void ( **f)(void);
extern  JUMP_CONTROL_ST jump_control[];
extern	unsigned Max_Args;
// ParserCtx.h
extern std::vector<uint16_t> gLoadIdx;
extern std::vector<uint16_t> gStoreIdx;
extern unsigned gLoadCap;
extern unsigned gStoreCap;
extern short uses_jump;
extern thread_local int jump_index;//, InitJumpIndex;
