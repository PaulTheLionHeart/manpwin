// FRACTINTFILTERS.CPP Filter types in FRACTINT.EXE..
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "manpwin.h"

struct FractintFilterStuff				// database of Fractint Outside filters
    {
    char    *name;				// name of the fractal 
    char    method;				// only allow '1', '2', blinds or spiral 
    };

struct	FractintFilterStuff	FractintFilter[]=	// default values for each 
    {
	{
	"color", 
	NONE,
	},
	{
	"iter",
	NONE,
	},
	{
	"real",
	REAL,
	},
	{
	"imag", 
	IMAG,
	},
	{
	"mult",
	MULT,
	},
	{
	"summ",
	SUM,
	},
	{
	"bof60", 
	BOF60,
	},
	{
	"bof61",
	BOF61,
	},
	{
	"zmag",
	ZMAG,
	},
	{
	"atan", 
	ATAN,
	},
	{
	"epsiloncross",
	EPSCROSS,
	},
	{
	"startrail",
	STARTRAIL,
	},
	NULL, 0
    };
