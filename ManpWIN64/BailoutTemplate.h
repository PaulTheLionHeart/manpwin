/*
    Bailout.h - Template for handling bailout code for all arithmetic types.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#pragma once

template<typename TComplex, typename TReal>
inline bool BailoutCore(int type, TComplex* z, const TComplex* sqr, TReal rqlim, TComplex* oldZ)
    {
    // “If I don’t have sqr — compute it”
    auto sx = sqr ? sqr->x : (z->x * z->x);
    auto sy = sqr ? sqr->y : (z->y * z->y);
    TReal convergenceLimitSquared = TReal(1.0e-20);

    switch (type)
	{
	case BAIL_MOD:  return (sx + sy >= rqlim);
	case BAIL_REAL: return (sx >= rqlim);
	case BAIL_IMAG: return (sy >= rqlim);
	case BAIL_OR:   return (sx >= rqlim || sy >= rqlim);
	case BAIL_AND:  return (sx >= rqlim && sy >= rqlim);

	case MANH:
	    {
	    auto manh = fabs(z->x) + fabs(z->y);
	    return (manh * manh >= rqlim);
	    }

	case MANR:
	    {
	    auto manr = z->x + z->y;
	    return (manr * manr >= rqlim);
	    }

	case CONVERGE:
	    {
	    if (sx + sy >= rqlim)
		return true;

	    if (oldZ == nullptr)
		return false;

	    auto dx = z->x - oldZ->x;
	    auto dy = z->y - oldZ->y;

	    return (dx * dx + dy * dy <= convergenceLimitSquared);
	    }


	default:
	    return (sx + sy >= rqlim);
	}
    }

template<typename TComplex, typename TReal>
inline bool DoBailout(int type, TComplex* z, TReal rqlim, TComplex* oldZ)
    {
    return BailoutCore<TComplex, TReal>(type, z, (TComplex*)nullptr, rqlim, oldZ);
    }

