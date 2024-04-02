#include "Preview.h"

#define	PREVIEW_HEIGHT	36
#define	PREVIEW_WIDTH	900
#define	HOR_OFFSET	10
#define	VERT_OFFSET	11

#define RANGE           0
#define INVERT          1
#define GREY            2
#define REDGREEN        3
#define REDBLUE         4
#define GREENBLUE       5
#define ROTATE          6
#define INDIVIDUAL      7

#define	STRETCH		0
#define	REPEAT		1

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class CEditPal
    {
    public:
	CEditPal();
	virtual		~CEditPal();
	int		RedStart;
	int		BlueStart;
	int		GreenStart;
	int		GreenEnd;
	int		RedEnd;
	int		BlueEnd;
	long		LocalThreshold;

	int		CopyPalPreview(HWND hwnd, BYTE *TempPal, long threshold, CPreview *PalPreview);
	void		ChangeRange(int color[], int StartIter, int EndIter, BYTE *TempPal);
	void		ChangePixel(int color[], int StartIter, int EndIter, BYTE *TempPal);
	void		InvertRange(int StartIter, int EndIter, BYTE *TempPal);
	void		GreyScaleRange(int StartIter, int EndIter, BYTE *TempPal);
	void		SwapColours(int type, int StartIter, int EndIter, BYTE *TempPal);
	int		atox(char c);
	void		SetupValues(HWND hDlg, int color[], int StartIter, int EndIter, BYTE *TempPal, char *StartRGB, char *EndRGB);
    private:
    };
