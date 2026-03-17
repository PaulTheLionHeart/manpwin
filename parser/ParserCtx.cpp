// VM CONTRACT (legacy-compatible)
// 1) Parser builds opcode array f[] and Store[]/Load[] tables.
// 2) FormPerPixel() MUST reset: overflow=0; LodPtr=StoPtr=OpPtr=jump_index=0;
// 3) FormPerPixel() MUST set Arg1=&s[0]; Arg2=Arg1-1; then initialize v[] and v[0]=c.
// 4) INIT program runs ONCE per pixel: ops [0 .. initEnd), leaving valid InitLodPtr/InitStoPtr/InitOpPtr.
// 5) After INIT: snapshot InitLodPtr/LodPtr, InitStoPtr/StoPtr, InitOpPtr/OpPtr (and InitJumpIndex if used).
// 6) Formula() runs per-iteration: restore LodPtr/StaPtr/OpPtr/jump_index from Init*, reset Arg1/Arg2,
//    then execute ops [InitOpPtr .. LastOp).
// 7) Ops use legacy globals: Arg1/Arg2 (stack regs), StoPtr/LodPtr (store/load indices), Store[]/Load[] tables.
// 8) VM wrapper (ExecOpWithContext) temporarily binds ctx->globals, calls op(), copies globals->ctx.
// 9) Store/Lod pointers must never step outside table bounds; stack pointers must remain within s[] (plus allowed -1/-2).
// 10) INIT must be executed even in VM_TEST mode, otherwise Formula() will crash.

#include <cassert>
#include <atomic>
#include "ParserCtx.h"
#include "parser.h"   // for Arg, and the existing globals Store/Load etc (extern)

thread_local CParser* CParser::s_current = nullptr;

extern	std::atomic<bool> gStopRequested;	// force early exit

void CParser::InitContextFromGlobals()
    {
    m_ctx.s.resize(20);

    // load/store index streams that StkLod/StkSto consume
    m_ctx.LoadIdx = ::gLoadIdx;
    m_ctx.StoreIdx = ::gStoreIdx;

    m_ctx.v_storage.resize(Max_Args);
    // constants/variables table (backing store from parser_allocate/ParseStr)
    m_ctx.v_storage = std::vector<ConstArg>(::v, ::v + Max_Args);
    m_ctx.v = m_ctx.v_storage.data();
    m_ctx.v_count = Max_Args;   // or Max_Args, but vsp is the real “used” size

    // function dispatch table built by ParseStr()
    m_ctx.f = ::f;

    // program length
    m_ctx.LastOp = ::LastOp;

    // Runtime registers (reset per run)
    m_ctx.LodPtr = 0;
    m_ctx.StoPtr = 0;
    m_ctx.OpPtr = 0;
    m_ctx.jump_cursor = 0;

    // Jump tables: built by ParseStr/fill_jump_struct into globals
    m_ctx.jump_index = ::jump_index;     // COUNT of jump entries
    m_ctx.jump_cursor = 0;                // runtime cursor starts at first entry
    memcpy(m_ctx.jump_control, ::jump_control, MAX_JUMPS * sizeof(JUMP_CONTROL_ST));

    // Stack base (pre-increment model)
    m_ctx.Arg1 = &m_ctx.s[0] - 1;
    m_ctx.Arg2 = &m_ctx.s[0] - 2;

    VMTRAP(m_ctx.v == nullptr || m_ctx.v_count == 0, 9001);
    VMTRAP(m_ctx.f == nullptr, 9002);          // <-- this is the meaningful one
    VMTRAP(m_ctx.LastOp == 0, 9003);

    if (!m_ctx.Load.empty() && m_ctx.Load[0])
	{
	Arg* p = m_ctx.Load[0];
	VMTRAP(p < &m_ctx.s[0] || p >= (&m_ctx.s[0] + m_ctx.s.size()), 2001);
	}

    // pointers/registers
    m_ctx.overflow = 0;
    }

void CParser::ResetVmRegsForInit()
    {
    std::fill(m_ctx.s.begin(), m_ctx.s.end(), Arg{});

    // Legacy VM expects these "below stack" starts because StkLod/StkSto pre-increment.
    m_ctx.Arg1 = m_ctx.s.data() - 1;
    m_ctx.Arg2 = m_ctx.s.data() - 2;

    m_ctx.StoPtr = 0;
    m_ctx.LodPtr = 0;
    m_ctx.OpPtr = 0;
    m_ctx.jump_cursor = 0;
    m_ctx.overflow = 0;
    }

void CParser::RunVmInit()
    {
    // ALWAYS refresh program tables after any ParseStr() / HardStop reparse
    m_ctx.s.resize(20);
    m_ctx.v_storage.resize(Max_Args);
    s_current = this;           // keep if other code still expects it

    // --- HARD RESET FIRST ---
    ResetVmRegsForInit();       // sets Arg1/Arg2, stack, ptrs etc

    // --- INIT mode behaviour ---
    m_ctx.sto_pops = false;     // STO does NOT pop during INIT

    // explicit safety (should already be 0 from reset)
    m_ctx.StoPtr = 0;
    m_ctx.LodPtr = 0;
    m_ctx.OpPtr = 0;
    m_ctx.jump_cursor = 0;
    m_ctx.overflow = 0;

    // sanity checks
    assert(m_ctx.Arg1 != nullptr);
    assert(m_ctx.Arg2 != nullptr);

    // --- EXECUTE INIT PROGRAM ---
    while (m_ctx.OpPtr < (int)m_ctx.LastOp)
	{
	void(*fn)(void) = m_ctx.f[m_ctx.OpPtr];

	if (fn) fn();

	if (fn == PtrEndInit)
	    {
	    m_ctx.OpPtr++;     // iteration begins AFTER EndInit
	    break;
	    }

	m_ctx.OpPtr++;
	}

    // --- SNAPSHOT ITER START STATE ---
    m_InitOpPtr = m_ctx.OpPtr;
    m_InitLodPtr = m_ctx.LodPtr;
    m_InitStoPtr = m_ctx.StoPtr;
    m_InitJumpIndex = m_ctx.jump_cursor;
    m_InitArg1i = (int)(m_ctx.Arg1 - m_ctx.s.data());
    m_InitArg2i = (int)(m_ctx.Arg2 - m_ctx.s.data());
    }

int CParser::RunVmIter()
    {
    if (AbortRequested())
	return RENDER_ABORTED;

    m_ctx.sto_pops = true;

    m_ctx.OpPtr = m_InitOpPtr;
    m_ctx.LodPtr = m_InitLodPtr;
    m_ctx.StoPtr = m_InitStoPtr;
    m_ctx.jump_cursor = m_InitJumpIndex;

    m_ctx.Arg1 = m_ctx.s.data() + m_InitArg1i;
    m_ctx.Arg2 = m_ctx.s.data() + m_InitArg2i;

    while (m_ctx.OpPtr < (int)m_ctx.LastOp)
	{
	if (AbortRequested())
	    return RENDER_ABORTED;

	void(*fn)(void) = m_ctx.f[m_ctx.OpPtr];
	if (fn) fn();

	// SAFE VM guards (correct direction)
	assert(m_ctx.LodPtr >= 0 && m_ctx.LodPtr <= (int)m_ctx.LoadIdx.size());
	assert(m_ctx.StoPtr >= 0 && m_ctx.StoPtr <= (int)m_ctx.StoreIdx.size());
	assert(m_ctx.Arg1 >= m_ctx.s.data() - 1);
	assert(m_ctx.Arg1 < m_ctx.s.data() + m_ctx.s.size());

	m_ctx.OpPtr++;
	}
    return 0;
    }


