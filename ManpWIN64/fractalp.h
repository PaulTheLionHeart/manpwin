/* FRACTALP.H - default values for the ManpWIN routines */
#include <Windows.h>

//#define	PRINTOSC				// used for listing oscillator names in d:\temp\OscDump.txt
#undef	PRINTOSC
#pragma once

//**************************************************************************
#define	MAXTYPE	245				// Remember to edit this when we add fractals
//**************************************************************************

#define	NUMPARAM		10

struct fractalspecificstuff			// ManpWIN version
{
   char	    *name;				// name of the fractal 
						// (leading "*" supresses name display) 
   char	    *paramname[NUMPARAM];		// name of the parameters 
   double   paramvalue[NUMPARAM];		// default parameter values 
   double   hor;				// default horizontal corner 
   double   vert;				// default vertical corner
   double   width;				// default YMIN corner 
   int	    BailoutType;			// bailout schemes for slope using fwd diff: ESCAPING, CONVERGING, MAGNET etc
   int	    juliaflag;				// JULIAFP if julia, or anything else otherwise (only used in reading Fractint Par files)
   int	    SlopeDegree;			// Default degree for Slope using Fwd Diff slope calcs - if = -1, then degree is taken from param[]
   int	    numparams;				// Number of parameters 
   int	    numfn;				// number of functions 
   char	    *fn1;				// first function 
   char	    *fn2;				// second function 
   DWORD    flags;				// constraints, bits defined below 
						// for Newton types from Fractint Par files: 
						//   "newton" or "complexnewton", subtype = 'N' 
						//   else "newtonbasin" or "complexbasin", subtype = (param2 == 0) ? 'B' : 'S' 
   int	    symmetry;				// applicable symmetry logic
						//   0 = no symmetry
						//  -1 = y-axis symmetry (If No Params)
						//   1 = y-axis symmetry
						//  -2 = x-axis symmetry (No Parms)
						//   2 = x-axis symmetry
						//  -3 = y-axis AND x-axis (No Parms)
						//   3 = y-axis AND x-axis symmetry
						//  -4 = polar symmetry (No Parms)
						//   4 = polar symmetry
						//   5 = PI (sin/cos) symmetry
						//   6 = NEWTON (power) symmetry
                                                                
   double   rqlim;				// bailout value 
//   int	    (*orbitcalc)();			// function that calculates one orbit 
   int	    (*per_pixel)();			// once-per-pixel init 
//   int	    (*per_image)();			// once-per-image setup 
   int	    (*calctype)();			// name of main fractal function 
   char	    *DialogueName;			// name of dialogue box
   DLGPROC  DialogueType;			// function call to dialogue box
   double   orbit_bailout;			// usual bailout value for orbit calc 
};

extern	struct	fractalspecificstuff	fractalspecific[];	// default values for each 
extern	struct	fractalspecificstuff	curfractalspecific;	// info about current fractal

//extern	int	subtype;		

#define	NUMMANDERIVPARAM	4
#define	NUMPERTPARAM		10
#define	NUMSLOPEDERIVPARAM	10

struct AlternativeSpecificStuff			// database of alternative fractals, e.g. Tierazon and Oscillator fractals
    {
    char    *name;				// name of the fractal 
    int(*per_pixel)();				// once-per-pixel init 
    int(*calctype)();				// name of main fractal function 
    int(*big_per_pixel)();			// Bignum versions 
    int(*big_calctype)();
    double  paramvalue[NUMMANDERIVPARAM];	// default parameter values 
    double  rqlim;				// bailout value 
    int	    symmetry;				// see above
    int	    SlopeDegree;			// set to -1 if taken from param
    int	    BailoutType;			// bailout schemes for slope using fwd diff: ESCAPING, CONVERGING, MAGNET etc
    };

struct PerturbationSpecificStuff		// database of Perturbation fractals
    {
    char	*name;				// name of the fractal 
    int		SlopeType;			// none, FwdDiff, Derivative
    int		PaletteStart;			// where does the colour start?
    double	lightDirectionDegrees;		// Slope parameters
    double	bumpMappingDepth;
    double	bumpMappingStrength;
    int		ColourOptions;			// future use?
    char	*paramname[NUMPERTPARAM];	// name of the parameters 
    double	paramvalue[NUMPERTPARAM];	// default parameter values 
    int		numparams;			// Number of parameters 
    double	rqlim;				// bailout value 
    int		RedShiftRider;			// true if fractal = RedShiftRider
    };

struct SlopeSpecificStuff			// database of Slope fractals
    {
    char	*name;				// name of the fractal 
    double	paramvalue[NUMSLOPEDERIVPARAM];	// default parameter values 
    int		numparams;			// Number of parameters 
    double	rqlim;				// bailout value 
    };

//extern	struct	fractalspecificstuff	MarcusSpecific[];	// default values for each 
extern	struct	AlternativeSpecificStuff	TierazonSpecific[];	// default values for each 
extern	struct	AlternativeSpecificStuff	MandelDerivSpecific[];	// default values for each 
extern	struct	SlopeSpecificStuff		SlopeDerivSpecific[];	// default values for each 
extern	struct	SlopeSpecificStuff		SlopeFwdDiffSpecific[];	// default values for each 
extern	struct	PerturbationSpecificStuff	PerturbationSpecific[];	// default values for each 

#define	ROTATION	0			// axes and animation schemes
#define XAXIS		1
#define YAXIS		2
#define ZAXIS		4
#define EVOLVE		8
#define MORPH		16
#define POINCARE	32

						// flags in some of the databases
						// LSB: Oscillator: plot lines, Fractal map: special colouring, Surfaces, Knots, Curves: Hide Background. 
						// Bits 1,2,3,4 are co-ordinate system: CARTESIAN = 0, SPHERICAL = 1, CYLINDRICAL = 2, PARABOLIC = 3, PARABOLOIDAL = 4, 
						//	ELLIPTICAL = 5, BIPOLAR = 6, TOROIDAL = 7, PROLATE_SPHERE = 8, OBLATE_SPHERE = 9, BISPHERICAL = 10, CONICAL = 11
						// Bit 5: block animation - used for 2D or less to preventanimating non-existent dimensions
						// Bit 6: calculate rotational centre based on screen rather than points that go off to infinity
						// Bit 7: 3D flag usd for perspective (surfaces only at this stage)
						// Bit 8: functions are specified in Functions.cpp and used in CPixel (first step towards multi-threading)
						// Bit 9: functions are specified in Fractals.cpp and used in CPixel (first step towards multi-threading)

#define	BOUNDARY	0			// Oscillator: plot lines, Fractal map: special colouring, Surfaces, Knots, Curves: Hide Background
#define	BLOCKANIM	32			// used for 2D or less to preventanimating non-existent dimensions
#define	AVERAGE		64			// used in oscillators and a few surfaces
#define	PERSPECTIVE	128			// used in surfaces
#define	FUNCTIONINPIXEL	256			// functions are specified in Functions.cpp and used in CPixel (first step towards multi-threading)
#define	FRACTINTINPIXEL	512			// functions are specified in Fractals.cpp and used in CPixel (first step towards multi-threading)
#define	FORMULAINPIXEL	1024			// functions are specified in Parser.cpp
#define	TRIGINPIXEL	2048			// functions are specified in FractintTrig.cpp and used in CPixel (these use globals to allow trig functions)
#define	AXISIMAGEDISP	4096			// display axes in oscillators
#define	OTHERFNINPIXEL	8192			// non-rastor functions still in pixel
#define	USEDOUBLEDOUBLE	16384			// allows for both double double and quad douple floating point types

#define	ESCAPING	0			// bailout schemes for slope using fwd diff
#define	ESCAPING1	1
#define	ESCAPING2	2
#define CONVERGING	3
#define CONVERGING1	4
#define CONVERGINGMAG	5
#define CONVERGINGMAG1	6

#define	CARTESIAN	0			// co-ordinate systems
#define	SPHERICAL	1
#define	CYLINDRICAL	2
#define	PARABOLIC	3
#define	PARABOLOIDAL	4
#define	ELLIPTICAL	5
#define	BIPOLAR		6
#define	TOROIDAL	7
#define	PROLATE_SPHERE	8
#define	OBLATE_SPHERE	9
#define	BISPHERICAL	10
#define	CONICAL		11
//#define	BIPOLAR_CYL	11		// removed as it's the same as BIPOLAR

struct OscillatorSpecificStuff			// ManpWIN version
{
   char	    *name;				// name of the fractal 
						// (leading "*" supresses name display) 
   char	    *paramname[NUMPARAM];		// name of the parameters 
   double   paramvalue[NUMPARAM];		// default parameter values 
   char	    *variablename[NUMPARAM];		// name of the variables 
   double   variablevalue[NUMPARAM];		// default variable values 
   double   hor;				// default horizontal corner 
   double   vert;				// default vertical corner
   double   width;				// default YMIN corner 
   int	    xAxis;				// x axis uses this dimension
   int	    yAxis;				// y axis uses this dimension
   int	    zAxis;				// z axis uses this dimension 
   int	    numparams;				// Number of parameters 
   int	    MaxDimensions;			// max number of dimensions 
   int	    numvariables;			// Number of variables 
   char	    *function;				// function (used in Thomas oscillator)
   unsigned flags;				// see above

   int	    RotationAxes;			// x, y, z axes             
   double   iterations;				// iterations value 
   int	    (*per_pixel)();		// once-per-pixel init 
   int	    (*calctype)();		// name of main fractal function 
   char	    *DialogueName;			// name of dialogue box
   DLGPROC  DialogueType;			// function call to dialogue box
   double   dt;					// delta time 
   double   VertBias;				// vertical bias 
   double   zBias;				// z axis bias 
};

struct CoordSysInfoStuff
{
   char	    *CoordSys[14];			// name of the co-ordinate system 
};


extern	struct	OscillatorSpecificStuff		OscillatorSpecific[];	// default values for each 
extern	struct	OscillatorSpecificStuff		FractalMapSpecific[];	// default values for each 
extern	struct	OscillatorSpecificStuff		SprottMapSpecific[];	// default values for each 
extern	struct	OscillatorSpecificStuff		SurfaceSpecific[];	// default values for each 
extern	struct	OscillatorSpecificStuff		KnotSpecific[];		// default values for each 
extern	struct	OscillatorSpecificStuff		CurveSpecific[];	// default values for each 
extern	struct	CoordSysInfoStuff		CoordSysInfo;		// default values for each 

struct BigFractalStuff				// database of functions for Bignum fractals
    {
    int	    type;				// label of the fractal 
    int	    (*big_per_pixel)();	// Bignum versions 
    int	    (*big_calctype)(); 
    };

extern	struct	BigFractalStuff	BigFractalSpecific[];	// default values for each 

extern	int	init_mandel_df();
extern	int	mountain();
extern	int	do_mandel_df();
//extern	int	init_Newton5thOrder();
extern	int	init_mandel_df();
extern	int	NullSetup();				// sometimes we just don't want to do anything 
extern	int	SetupTetrate();
extern	int	orbit3dfloatsetup();
extern	int	orbit2dfloat();
extern	int	orbit3dfloatcalc();
extern	int	bifurcation();
extern	int	DoTetration();
extern	int	InitTetration();
//extern	int	InitBifurcation();
extern	int	DoBuddhaBrot();
extern	int	Fibonacci();
//extern	int	demowalk();

extern	int	Fourier();
extern	int	dynam2dfloat();
extern	int	dynam2dfloatsetup();

extern	int	InitPerturbation();
extern	int	DoPerturbation();

//extern	int	RunForm(char *, int);
//extern	int	FormPerPixel();
extern	int	DoHenon();
extern	int	DoApollonius();

//extern	int	DoSlope();
extern	int	DoSlopeFractal();

extern	int	InitSlopeFractal();




// Chaotic Oscillators
extern	int	DoDifferent();
extern	int	DoChua();
extern	int	DoVanderpol();
extern	int	DoMooreSpiegel();
extern	int	DoRabinovich();
extern	int	DoLorenz();
extern	int	DoAizawa();
extern	int	DoThomas();
extern	int	DoNoseHoover();
extern	int	DoSprottLinzA();
extern	int	DoSprottLinzB();
extern	int	DoSprottLinzC();
extern	int	DoSprottLinzD();
extern	int	DoSprottLinzE();
extern	int	DoSprottLinzF();
extern	int	DoSprottLinzG();
extern	int	DoSprottLinzH();
extern	int	DoSprottLinzI();
extern	int	DoSprottLinzJ();
extern	int	DoSprottLinzK();
extern	int	DoSprottLinzL();
extern	int	DoSprottLinzM();
extern	int	DoSprottLinzQ();
extern	int	DoSprottLinzS();
extern	int	DoACT();
extern	int	DoArneodo();
extern	int	DoChen();
extern	int	DoBurkeShaw();
extern	int	DoGenesioTesi();
extern	int	DoHadley();
extern	int	DoHalfInverted();
extern	int	DoHalvorsen();
extern	int	DoHyperRossler();
extern	int	DoIkedaAttractor();
extern	int	DoKnotHolder();
extern	int	DoLi();
extern	int	DoLiuChen();
extern	int	DoLotkaVolterra();
extern	int	DoNewChaos();
extern	int	DoNewChaosN();
extern	int	DoNewJerk();
extern	int	DoNewtonLeipnik();
extern	int	DoQuadratic();
extern	int	DoRayleighBenard();
extern	int	DoRikitake();
extern	int	DoRucklidge();
extern	int	DoSakarya();
extern	int	DoShawPol();
extern	int	DoShimizu();
extern	int	DoSimplest();
extern	int	DoSimplest2();
extern	int	DoLovingLorenz();
extern	int	DoStrizhak();
extern	int	DoSymmetricFlow();
extern	int	DoUeda();
extern	int	DoUnimodal();
extern	int	DoWindmi();
extern	int	DoWillamowski();
//extern	int	DoAnimatedSierpinskiFlower();
extern	int	DoAnishchenko();
//extern	int	DoBouali1();
extern	int	DoBouali2();
extern	int	DoChuaCubic();
extern	int	DoCoullet();
extern	int	DoCells();
extern	int	DoChenLee();
extern	int	DoDadras();
extern	int	DoDequanLi();
extern	int	DoElhadjSprott();
extern	int	DoElhadjSprott1();
extern	int	DoFinance();
extern	int	DoFourWing1();
extern	int	DoFourWing2();
extern	int	DoFourWing3();
extern	int	DoWang();
extern	int	DoWangSun();
extern	int	DoWimolBanlue();
extern	int	DoXingYun();
extern	int	DoYuWang();
extern	int	DoZhou();
extern	int	DoDuffings3D();
extern	int	DoHyperChaotic();
extern	int	DoSaito();
extern	int	DoButterfly();
extern	int	DoThreeLayer();
extern	int	DoZaidan2();
extern	int	DoZaidan3();
extern	int	DoZaidan4();
extern	int	DoMirandaStone();
extern	int	DoWien();
extern	int	DoQiSlave();
extern	int	DoSilvaYoung();
extern	int	DoTsucs();
extern	int	DoRossler();
extern	int	DoRayleighBenard9D();
extern	int	DoHindmarshRose();
extern	int	DoFluxOfFluids5D();
extern	int	DoZang5D();
extern	int	DoNovel();
extern	int	DoOanceo();
extern	int	DoQi3D();
extern	int	DoLorenzMod();
//extern	int	DoLorenzMod2();
//extern	int	DoLorenzStenflo();
extern	int	DoMachado();
extern	int	DoYangChen4D();
extern	int	DoBoa4D();
extern	int	DoCai4D();
extern	int	DoJha4D();
extern	int	DoLu4D();
extern	int	DoLeipnik4D();
extern	int	DoPang4D();
extern	int	DoXu4D();
extern	int	DoBoualiSafieddine();
extern	int	DoLeonovKuznetsov4D();
extern	int	DoMarcus01();
extern	int	DoMarcus02();
extern	int	DoMarcus03();
extern	int	DoMarcus04();
extern	int	DoMarcus06();
extern	int	DoMarcus08();
//extern	int	DoMarcus10();
//extern	int	DoMarcus11();
extern	int	DoMarcus11();
extern	int	DoSprott302();
extern	int	DoSprott330();
extern	int	DoSprott390();
extern	int	DoSprott391();
extern	int	DoSprott395();
extern	int	DoSprott409();
extern	int	DoSprott411();
extern	int	DoSprott416();
extern	int	DoSprott423();
extern	int	DoLeviFlight();
extern	int	DoSprott406();
extern	int	DoSprott374();
extern	int	DoJuliana8D();
extern	int	DoCarlos5D();
//extern	int	DoJoanaMarcusAB();
//extern	int	DoJoanaMarcusC();
extern	int	DoJoanaMarcus();
extern	int	DoBistability();
extern	int	DoBlueSky();
extern	int	DoThirdOrder4D();
extern	int	DoThirdOrder7D();
extern	int	DoOrchestra();
extern	int	DoBaskets4D();
extern	int	DoTrumpets();
extern	int	DoHyperbolic();
extern	int	DoPolyWings4D();
extern	int	DoSimple();
extern	int	DoQiChen();
extern	int	DoPendulum();
extern	int	DoOrderChaos4D();
extern	int	DoSynchronisation();
extern	int	DoWaves1();
extern	int	DoWaves3();
extern	int	DoWaves9();
extern	int	DoWaves19();
extern	int	DoMusical();
extern	int	DoShell();
extern	int	DoBraid();
extern	int	DoLeviZigZag();
extern	int	DoPickoverOsc();
extern	int	DoPendulumDiverse4D();
extern	int	DoTamari();
extern	int	DoPendulumDelayed();
extern	int	DoPendulumModified();
extern	int	DoChaosExcitedContinuous();
extern	int	DoLacesAndTurns();
extern	int	DoMasterSlave();
extern	int	DoDissipativeChaos();
extern	int	DoSafieddine();
extern	int	DoWorldWithoutWar();
extern	int	DoSprottMarcus();
extern	int	DoOlsen();
extern	int	DoArtificialNeural();
extern	int	DoBoldrighini();
extern	int	DoGrassiMascolo();
extern	int	DoQiSlaveMarcus();
extern	int	DoShilnikov();
extern	int	DoBloch();
extern	int	DoCrackedBeams();
extern	int	DoLSD();
extern	int	DoSynchronisation2by4D();
extern	int	DoMotionMagField();
extern	int	DoResonance();
extern	int	DoSyncPhenomena();
extern	int	DoChenMultiscroll();
extern	int	DoGenPolynomialChaos();
extern	int	DoColpitts();
extern	int	DoHypotheticalGalaxies();
extern	int	DoSyncPhenomena1();
extern	int	DoSpatiotemporal();
extern	int	DoWindmi6D();
extern	int	DoSevenDimensions();
extern	int	DoDetectingChaos();
extern	int	DoLorenz8D();
extern	int	DoHyperChaotic9D();
extern	int	DoJoanna2Neurons();
extern	int	DoJoanna3Neurons();
extern	int	DoRabbitsAndFoxes();
extern	int	DoMultiscrollLorenz();
extern	int	DoMultiLorenz();
extern	int	DoBandsAndBranes();
extern	int	DoNonlinearDynamics();
extern	int	DoControlledPredatorPrey();
extern	int	DoCoronarianArtery();
extern	int	DoEnergyResource();
extern	int	DoEntangled();
extern	int	DoFluidInstabilities();
extern	int	DoNutrient();
extern	int	DoEnergyStochastic();
extern	int	DoRosslerLike();
extern	int	DoLorenzStenflo4D();
extern	int	DoTrialAndError();
extern	int	DoSphericallyScrew();
extern	int	DoRetailersDynamics();
extern	int	DoHyperchaoticYanChen();
extern	int	DoChuaQuartic();
extern	int	DoLorenzUnlike();
extern	int	DoSyncUNFractional();
extern	int	DoCardiovascular();
extern	int	DoModifiedChua();
extern	int	DoTransport();
extern	int	DoCrossWaves();
extern	int	DoClown();
extern	int	DoPendulumSystems();
extern	int	DoHiddenChua();
extern	int	DoBoseEinstein();
extern	int	DoColpittsStretched();
extern	int	DoSyncWindmi();
extern	int	DoTransientChaos();
extern	int	DoQuadraticEquations();
extern	int	DoAntinoise();
extern	int	DoMorseOscillator();
extern	int	DoChemicalChaos();
extern	int	DoHyperchaoticYujun();
extern	int	DoNewChaoticDynamicSystem();
extern	int	DoHighDimensional();
extern	int	DoAdaptiveSync();
extern	int	DoTumorGrowth();
extern	int	DoHarb();
extern	int	DoTriternion();
extern	int	DoChenLike();
extern	int	DoLorenzLike();
extern	int	DoLuLike();
extern	int	DoTsucsMarcus();
extern	int	DoHenonHeiles();
extern	int	DoTokamaks();
extern	int	DoMinimalHeteroclinic();
extern	int	DoSmoothNLayer();
extern	int	DoNonLinearArabesquesI();
extern	int	DoNonLinearArabesquesII();
extern	int	DoNonLinearArabesquesIII();
extern	int	DoStrangeButterfly();
extern	int	DoIrregularChaoticAttractor();
extern	int	DoBrusselator();
extern	int	DoPredatorPrey();
extern	int	DoWienRessonator();
extern	int	DoCoupledRossler();
extern	int	DoMirandaStone4();
extern	int	DoMuthuswamy();
extern	int	DoEmpiricalNonlinear();
extern	int	DoDimensions();
extern	int	DoStrangeNonChaotic();
extern	int	DoSprott412();
extern	int	DoSprott421();
extern	int	DoSprott407a();
extern	int	DoSprott407b();
extern	int	DoSprott407c();
extern	int	DoSprott407d();
extern	int	DoSprott408();
extern	int	DoSprott392();
extern	int	DoSprott397();
extern	int	DoHyperJerk();
extern	int	DoTakagiSugeno();
extern	int	DoJosephsonJunction();
extern	int	DoNewChaoticAttractor();
extern	int	DoExponentialChen();
extern	int	DoNewLorenz();
extern	int	DoFourMemristor();
extern	int	DoPulseExcited();
extern	int	DoCompoundLorenzChenLu();
extern	int	DoCompoundLorenzChenLSMRucklidge();
extern	int	DoMultiScrollChen();
extern	int	DoBirds();
extern	int	DoSprott409II();
extern	int	DoInfinitesimalChaoticSpheres();
extern	int	DoMultifoldedTorus();
extern	int	DoSimpleMemristor();
extern	int	DoDynamicalAnalysis();
extern	int	DoMemristive();
extern	int	DoMartin();
extern	int	DoBifurcationPhenomena();
extern	int	DoLinearArabesque();
extern	int	DoEpilepticSeizures();
extern	int	DoInventedMuthuswamy();
extern	int	DoImpulsiveSyncI();
extern	int	DoImpulsiveSyncII();
extern	int	DoTritrophicFoodChains();
//extern	int	DoLowDimensional();
extern	int	DoSilnikov();
extern	int	DoDeterministicChaos();
extern	int	DoDepthTwoHeteroclinic1();
extern	int	DoDepthTwoHeteroclinic2();
extern	int	DoElectronicCctFreire();
extern	int	DoUnscrew();
extern	int	DoChuaMatsumoto();
extern	int	DoHyperChaosLu();
extern	int	DoSimpleChaoticFlowError();
extern	int	DoSimpleChaoticFlow();
extern	int	DoChaosControl();
extern	int	DoBonhoffer();
extern	int	DoChaosEncryption();
extern	int	DoMagnet();
extern	int	DoSprottWind();
extern	int	DoGeneNetworks();
extern	int	DoMagneticField();
extern	int	DoMEMSResonator();
extern	int	DoEightWing();
extern	int	DoSineCosineShock();
extern	int	DoAnalyticalNumerical();
extern	int	DoDynamicalSystems();
extern	int	DoGissinger();
extern	int	DoNewAttractor();
extern	int	DoHitzlZeleIII();
extern	int	DoHitzlZeleMod();
extern	int	DoInfiniteScroll();
extern	int	DoLorenzMinusCatastrophe();
extern	int	DoNewQuadratic();
extern	int	DoNovelStrange();
extern	int	DoOregonator();
extern	int	DoRussianPendulum();
extern	int	DoModernFirm();
extern	int	DoThreeDimensional();
extern	int	DoNovelMinusCatastrophe();
extern	int	DoNewChaoticCatastrophe();
extern	int	DoModernFirmPlusCatastrophe();
extern	int	DoSyncFractional();
extern	int	DoQuantumCellular();
extern	int	DoSimplest3D();
extern	int	DoNovel3DCatastrophe();
extern	int	DoChaoticAttractor();
extern	int	DoLaserCatastrophe();
extern	int	DoAdaptiveInverse();
extern	int	DoHybridSystem();
extern	int	DoGoldenMean();
extern	int	DoHopfBifurcation();
extern	int	DoFeigenbaumLike();
extern	int	DoComplexSystem();
extern	int	DoAnticontrolHopf();
extern	int	DoHopfBifurcationEntanglement();
extern	int	DoHorseshoeChaos();
extern	int	DoUltimateBound();
extern	int	DoHopfBifCascade();
extern	int	DoTheoreticalHyperchaoticI();
extern	int	DoTheoreticalHyperchaoticII();
extern	int	DoTheoreticalHyperchaoticIII();
extern	int	DoTestForChaos();
extern	int	DoRobustAdaptive();
extern	int	DoRobustSynchronisation();
extern	int	DoHybridDislocated();
extern	int	DoFiveDLorenzLike();
extern	int	DoApplicationSilnikov();
extern	int	DoAnisochronousSelfExcited();
extern	int	DoNonlinearGear();
extern	int	DoChaosEntanglement();
extern	int	DoHomoclinicBifurcation();
extern	int	DoFiniteTime();
extern	int	DoModifiedLiu();
extern	int	DoAttractorAtmosphere();
extern	int	DoHyperchaoticZhou();
extern	int	DoElegantChaosFractionalOrderSystem();
extern	int	DoIdentification();
extern	int	DoNovelControl();
extern	int	DoGlobalAnalysis();
extern	int	DoCombinationHyperchaos();
extern	int	DoPeculiarities();
extern	int	DoNonautonomousWienBridge();
extern	int	DoOneScroll();
extern	int	DoBelyakov();
extern	int	DoUnilateralCoupling();
extern	int	DoMultiWingI();
extern	int	DoMultiWingII();
extern	int	DoMultiWingIII();
extern	int	DoMultiWingIV();
extern	int	DoMultiWingV();
extern	int	DoSimpleChaoticFlows1();
extern	int	DoSimpleChaoticFlows7();
extern	int	DoSimpleChaoticFlows8();
extern	int	DoSimpleChaoticFlows10();
extern	int	DoSimpleChaoticFlows11();
extern	int	DoSimpleChaoticFlows12();
extern	int	DoSimpleChaoticFlows13();
extern	int	DoStudyOfOCoexistence();
extern	int	DoSystemInfinite();
extern	int	DoChaoticRelaxation();
extern	int	DoDynamicsCoupledOscillators();
extern	int	DoFiveDCircuitSimulation();
extern	int	DoLorenz84Osc();
extern	int	DoNonautonomousPredatorPrey();
extern	int	DoDoubleWell();
extern	int	DoDifferentPairs();
extern	int	DoInvariantSets4D();
extern	int	DoModifiedTigan();
extern	int	DoPreyPredatorTop();
extern	int	DoLoziMapI();
extern	int	DoLoziMapII();
extern	int	DoRikitake4D();
extern	int	DoCouplingDoubleWell();
extern	int	DoCostFunction();
extern	int	DoAntiSynchronisation();
extern	int	DoProbabilisticConceptsI();
extern	int	DoHyperChaoticFinance();
extern	int	DoChaoticEvolution();
extern	int	DoCouplingDoubleWell8D();
extern	int	DoHyperChaoticSakarya();
extern	int	DoHalfDuplexRelay();
extern	int	DoPiecewiseLinear();
extern	int	DoUnknownFountain();
extern	int	DoComplexBehaviours();
extern	int	DoDyadicTransformationI();
extern	int	DoDyadicTransformationII();
extern	int	DoNewMemristive();
extern	int	DoFastAdaptive();
extern	int	DoChangingClimate();
extern	int	DoHiddenAttractor();
extern	int	DoTentPlusLogistic();
extern	int	DoHiddenHyperchaoticAttractor();
extern	int	DoHyperChaoticComplex();
extern	int	DoTrilobyte();
extern	int	DoNovel5D();
extern	int	DoTwoHyperchaoticSystems();
extern	int	DoSpatiotemporalHyperchaotic();
extern	int	DoEconomicSystems();
extern	int	DoModifiedHyperchaoticYu();
extern	int	DoCoupledSystems();
extern	int	DoDesigningCryptosystems();
extern	int	DoDeterministicHydrodynamic();
extern	int	DoHyperChaosNumerical();
extern	int	DoSyncIrregularComplexNetworks();
extern	int	DoAutonomousSilvaYoung();
extern	int	DoSinusoidallyForcedLorenz();
extern	int	DoProbabilisticConceptsII();
extern	int	DoHyperbolicSin();
extern	int	DoImpulsiveControl();
extern	int	DoDoubleAttractor();
extern	int	DoIlogisticAttractor();
extern	int	DoTamingChaos();
extern	int	DoErgodicTimeReversible();
extern	int	DoBistabilityHyperchaotic();
extern	int	DoFitzHughNagumoe();
extern	int	DoMemristiveHyperchaotic();
extern	int	DoUniqueSignum();
extern	int	DoSnapHyperchaos();
extern	int	DoChaoticHyperchaotic();
extern	int	DoGenerating3Scroll();
extern	int	DoDichotomyNonlinear();
extern	int	DoHighDimensionalHyperchaos();
extern	int	DoFuzzySynchronisation();
extern	int	DoPrototypesAttractors4D();
extern	int	DoDamageAssessment5D();
extern	int	DoDynamicsAndSynchronisation();
extern	int	DoNewChaoticAttractor4D();
extern	int	DoSyncScalarSignal();
extern	int	DoSinusoidallyDrivenLorenz();
extern	int	DoSixDHyperchaotic();
extern	int	DoSimpleChaoticFlowAdjustable();
extern	int	DoNewHyperchaoticComplexPatterns();
extern	int	DoEmulatingComplexBusiness();
extern	int	DoNewChaoticAttractorMod();
extern	int	DoAnalysisWilsonCowan();
extern	int	DoNewButterfly();
extern	int	DoDynamicalChaos();
extern	int	DoChaosHyperchaosTransition();
extern	int	DoSPICERikitakeDynamo();
extern	int	DoHyperChaoticAttractor();
extern	int	DoSimpleChaoticOscillator();
extern	int	DoCryptanalysis();
extern	int	DoUsingRecurrences();
extern	int	DoDynamicsAndFeedback();
extern	int	DoChaoticAttractorQuadratic();
extern	int	DoSyncCoupledBistable();
extern	int	DoDataAnalysis();
extern	int	DoDataAnalysisCP();
extern	int	DoModifiedProjective();
extern	int	DoAntiSynchronisationNovel();
extern	int	DoPhaseCoherence();
extern	int	DoOptimalLinearControl();
extern	int	DoChaoticSignalInduced();
extern	int	DoSnapHyperchaosModified();
extern	int	DoNovel4D();
extern	int	DoMultiNumberCommunication();
extern	int	DoGeneratingTriChaos();
extern	int	DoSimpleChaoticFlowAdjustableAttractor();
extern	int	DoUnknownAuthor();
extern	int	DoDifferent5DLorenzLookAlike();
extern	int	DoSimpleChaoticFlowAdjustableAttractorIII();
extern	int	DoDecompositionMethod();
extern	int	DoIntegerFractional();
extern	int	DoActiveTracking();
extern	int	DoHeatConduction();
extern	int	DoOnTransition2Hyperchaos();
extern	int	DoControllingHyperchaotic();
extern	int	DoHyperchaoticBehavior();
extern	int	DoDegenerateHopfbifurcations();
extern	int	DoInducedSynchronization();
extern	int	DoChaoticAttractor4D();
extern	int	DoEightTermFourScrollChaoticSystem();
extern	int	DoModifiedTinkerbell();
extern	int	DoChaoticSystem();
extern	int	DoFiniteTimeObserver();
extern	int	DoDynamicalNoseHoover();
extern	int	DoControlNew3DChaoticSystem();
extern	int	DoEquivalentCircuitChua();
extern	int	DoMultipulseChaoticDynamics();
extern	int	DoDynamicsNonautonomous();
extern	int	DoA3DStrangeAttractor();
extern	int	DoGaussianMixture();
extern	int	DoImprovingRungeKutta();
extern	int	DoUnknownOscillator4D();
extern	int	DoGeneralizedStabilityTheorem();
extern	int	DoImageEncryptionAlgorithm();
extern	int	DoGenDynamicSwitchedSync();
extern	int	DoBifurcationSynchronisation();
extern	int	DoImprovingLorenzDynamics();
extern	int	DoMeminductiveWeinbridge();
extern	int	DoNew7DHyperchaotic();
extern	int	DoHiddenBifurcations();
extern	int	DoDiscontinuityInducedBifurcations();
extern	int	DoNovelFourWing();
extern	int	DoCoexistingOscillation();
extern	int	DoDynamicsCircuitRealization();
extern	int	DoOnNonElementarySingularities();
extern	int	DoSimpleChaoticHyperjerk();
extern	int	DoTheoreticalCircuitVerificationI();
extern	int	DoTheoreticalCircuitVerificationII();
extern	int	DoBistableHidden();
extern	int	DoNewThreeDimensionalChaotic();
extern	int	DoAmplitudePhaseControl();
extern	int	DoCharacteristicsChaoticParameters();
extern	int	DoSecureCommunicationScheme();
extern	int	DoChimeralikeStates();
extern	int	DoMinimalHeteroclinicChanged();
extern	int	DoNovelHyperjerk();
extern	int	DoChaoticDifferentFamilies();
extern	int	DoSynchronisationNeuralNetworks();
extern	int	DoSynchronisationNeuralNetworksPi();
extern	int	DoChaoticDynamicsFractionalOrder();
extern	int	DoPredictabilityThresholdExceedance();
extern	int	DoRecentNewHiddenAttractors();
extern	int	DoBasinsAttractionPlasticity();
extern	int	DoGeometrySurfacesCurvesPolyhedra();
extern	int	DoSimpleChaoticLorenzTypeSystem();
extern	int	DoA3DAutonomousContinuousSystem();
extern	int	DoANoEquilibriumHyperchaoticSystem();
extern	int	DoANovelMemcapacitorModel();
extern	int	DoDifferentChaos();
extern	int	DoDynamicAnalysis();
extern	int	DoDynamicsNewHyperchaoticSystem();
extern	int	DoHyperchaoticChameleon();
extern	int	DoStimulusInducedChaoticSync();
extern	int	DoSomeAttractorsComplexLorenz();
extern	int	DoModelReferenceControl();
extern	int	DoImageEncryptionHyperChaos();
extern	int	DoNonIntegrabilityDysonPotential();
extern	int	DoHiddenHyperchaoElectronicCircuit();
extern	int	DoComputerAssistedProof();
extern	int	DoPiecewiseLinearHyperchaoticI();
extern	int	DoPiecewiseLinearHyperchaoticII();
extern	int	DoAnalysisAdaptiveControlSync();
extern	int	DoHowBrokenEggAttractor();
extern	int	DoNonsmoothBifurcations();
extern	int	DoNovel4WingHyperChaotic();
extern	int	DoImprovedMemristiveDiodeBridge();
extern	int	DoDynamicAnalysisFractOrderAutonomousChaoticSystem();
extern	int	DoHighThroughputPseudorandomNumberGenerator();
extern	int	DoChaoticDadrasMomeniSystem();
extern	int	DoNewSimpleChaoticLorenzTypeSystem();
extern	int	DoHyperChaosControl();
extern	int	DoAChaoticSystemInfiniteEquilibria();
extern	int	DoCoupledDynamics();
extern	int	DoParameterIndependentDynamicalBehaviors();
extern	int	DoBifurcationAnalysisI();
extern	int	DoBifurcationAnalysisII();
extern	int	DoBifurcationAndChaos();
extern	int	DoCoexistenceHiddenChaoticAttractors();
extern	int	DoFractionalOrderMemristor();
extern	int	DoMultistability();
extern	int	DoConnectingKuramotoModel();
extern	int	DoA4DHyperchaoticMemristiveDynamicalSystem();
extern	int	DoNovelChaoticSystemWithoutEquilibrium();
extern	int	DoSimpleChaoticCircuitLightEmittingDiode();
extern	int	DoSolitaryStatesForCoupledOscillators();
extern	int	DoSynchronizationCoupledMultistableSystems();
extern	int	DoBasinsAttractionChimeraStates();
extern	int	DoNonLinearDynamicsFractionalOrderHIVModel();
extern	int	DoStabilizingNearNonhyperbolicChaoticSystems();
extern	int	DoDynamicsCubicNonlinearSystem();
extern	int	DoMultimediaSecurityApplication();
extern	int	DoDynamicsCircuitDesignSynchronization();
extern	int	DoA4DHyperchaoticFinanceSystem();
extern	int	DoChaosIn3DSystemWithAbsoluteNonlinearity();
extern	int	DoChaosIn6DSystemWithAbsoluteNonlinearity();
extern	int	DoMultipleAttractorsDelayedMackeyGlassSystems();
extern	int	DoPinningHybridSynchronization();
extern	int	DoMultipleDelayRosslerSystem();
extern	int	DoStabilityAnalysisControlChaos();
extern	int	DoGenCombinationComplexSync();
extern	int	DoChemicalOscillations();
extern	int	DoTimeReversalSymmetry();
extern	int	DoOneWayCoupledVanderPolSystem();
extern	int	DoAdaptiveHybridDislocatedSync();
extern	int	DoCircuitImplementationFourWingMemristiveChaoticSystem();
extern	int	DoDynamicEvolutionAnalysisStockPriceFluctuation();
extern	int	DoHiddenAttractorOnOnePath();
extern	int	Do2Dand3DSolvableMaps();
extern	int	DoMarcus05();
extern	int	DoInfluenceDelayFactorsDeterministicChaos();
extern	int	DoControlSimpleChaoticFlow();
extern	int	DoFractionalOrderFormSystem();
extern	int	DoDataAssistedReducedOrderModeling();
extern	int	DoNewChaoticFinanceSystem();
extern	int	DoA4DMemristiveChaoticSystem();
extern	int	DoA5DHyperchaoticSystem();
extern	int	DoSixTermNovelChaoticSystem();
extern	int	DoNovel3ScrollChuasAttractor();
extern	int	DoBifurcationAnalysisChaoticBehaviorsI();
extern	int	DoBifurcationAnalysisChaoticBehaviorsII();
extern	int	DoAmplitudeDeath();
extern	int	DoForcedSynchronisation();
extern	int	DoTwoTimeSynchronism();
extern	int	DoAnalysisTwoTimeScaleProperty();
extern	int	DoEvolvingChaosLorenzXY21();
extern	int	DoEvolvingChaosLorenzXZ11();
extern	int	DoEvolvingChaosLorenzXYZ4();
extern	int	DoChaoticSystemMultipleAttractors();
extern	int	DoEvolvingChaosLorenzYZ76();
extern	int	DoGeneratingChaosSwitchingPiecewiseLinearController();
extern	int	DoPeriodicallyForcedChaoticSystem();
extern	int	DoDynamicsCircuitChaoticSystem();
extern	int	DoSynchronisationNovelFractionalOrderStretchTwistFold();
extern	int	DoImpulsiveControlTSFuzzyModel();
extern	int	DoA4DChaoticHyperjerkSystem();
extern	int	DoANewTransientlyChaoticFlow();
extern	int	DoNewClassChaoticSystemsEquilibriumPointsI();
extern	int	DoNewClassChaoticSystemsEquilibriumPointsII();
extern	int	DoNewClassChaoticSystemsEquilibriumPointsIII();
extern	int	DoIrregularAttractors();
extern	int	DoCircuitImplementationNonlinearSystemIntegerFractionalOrders();
extern	int	DoQuasiperiodicitySuppressionMultistability();
extern	int	DoChaoticFlowHiddenAttractor();
extern	int	DoChaoticChameleon();
extern	int	DoANewChaoticSystemSelfExcitedAttractorEntropyMeasurement();
extern	int	DoControllingExtremeMultistability();
extern	int	DoFourWingAttractors();
extern	int	DoLabexperiment();
extern	int	DoModellingChaosHyperchaosTransition();
extern	int	DoCarpetOscillator();
extern	int	DoDifferentFamiliesHiddenAttractors();
extern	int	DoCubicEquilibriumChaoticSystem();
extern	int	DoWrongMixup();
extern	int	DoSynchronisationControl();
extern	int	DoGridMultiwingChaoticSystem();
extern	int	DoGeneratingOneToFourWingHiddenAttractors();
extern	int	DoChaoticSystemEquilibriaLocatedRoundedSquareLoop();
extern	int	DoResearch3DPredatorPreyEvolutionarySystem();
extern	int	DoDynamicsOfImpurities();
extern	int	DoInitialConditionDependentDynamics();
extern	int	DoANoEquilibriumHyperchaoticSystemCubicNonlinearTerm();
extern	int	DoANovelFourDimensionalHyperchaoticCoupledDynamos();
extern	int	DoAFlexibleChaoticSystemAdjustableAmplitude();
extern	int	DoHyperchaosConservativeSystem();
extern	int	DoNewFourWingHyperchaoticAttractor();
extern	int	DoOnFormationHiddenChaoticAttractors();
extern	int	DoAnInfinite2DLatticeStrangeAttractors();
extern	int	Do4DChaoticSystem1stLyapunovExponent22();
extern	int	DoSynchronisationPhenomena();
extern	int	DoHyperChaoticOscillatorGyrators();
extern	int	DoAnalysisCharacterisationHyperchaos();
extern	int	DoPerpetualPointsPeriodicPerpetualLoci();
extern	int	DoJerkSystemSelfExcitingHiddenFlowsI();
extern	int	DoJerkSystemSelfExcitingHiddenFlowsII();
extern	int	DoANewChaoticSystemMultipleAttractors();
extern	int	DoHyperchaoticAttractorNovelHyperjerkSystemI();
extern	int	DoHyperchaoticAttractorNovelHyperjerkSystemII();
extern	int	DoAnImprovedButterflyOptimizationAlgorithm();
extern	int	DoChaosMultiScrollAttractors();
extern	int	DoMemristorOscillatorBasedOnTwinTNetwork();
extern	int	DoGeneratingHiddenExtremeMultistability();
extern	int	DoOne2FourWingChaoticAttractors();
extern	int	DoAnalysisImplementationFloatingMemristorChaoticCircuit();
extern	int	DoArePerpetualPointsSufficient();
extern	int	DoSimpleChaoticFlowsCurveEquilibriaCE1();
extern	int	DoSimpleChaoticFlowsCurveEquilibriaCE2();
extern	int	DoSimpleChaoticFlowsCurveEquilibriaCE3();
extern	int	DoSimpleChaoticFlowsCurveEquilibriaCE4();
extern	int	DoDriveResponseSynchronisationFractionalOrderHyperchaoticSystem();
extern	int	DoASimpleSnapOscillator();
extern	int	DoDesignProcessingNovelChaosBased();
extern	int	DoBasicDynamicalAnalyses();
extern	int	DoFourWingChaoticAttractor();
extern	int	DoFrequencyLimitationsCircuitRealisation();
extern	int	DoChaotificationSTFFlow();
extern	int	DoFourDimensionalAutonomousDynamicalSystems();
extern	int	DoDynamicAnalysisCircuitDesign();
extern	int	DoChaosBasedOptimisation();
extern	int	DoANovel4DChaoticSystemBased2DegreesFreedom();
extern	int	DoSyncPhenomenaCoupledBirkhoffShawChaoticSystems();
extern	int	DoBifurcationAndChaosTimeDelayedFractionalOrder();
extern	int	DoDynamicAnalysesFPGAImplementationEngineeringApplications();
extern	int	DoControllingChaoticResonator();
extern	int	DoDesignControlMultiWingDissipativeChaoticSystem();
extern	int	DoANew3DChaoticFlowOneStableEquilibrium();
extern	int	DoMultiScrollChaoticAttractors();
extern	int	DoOccasionalUncoupling();
extern	int	DoDynamicAnalysisElectronicCircuitImplementation();
extern	int	DoANewChaoticSystemMultipleAttractorsB();
extern	int	DoCoherentMotionChaoticAttractors();
extern	int	DoNewFamily4DHyperchaoticChaoticSystems();
extern	int	DoImplementationSynchronisationChenLiuYangChaoticSystems();
extern	int	DoMegastabilityQuasiPeriodicallyForcedSystem();
extern	int	DoHiddenAttractorOnOnePathGlukhovskyDolzhansky();
extern	int	DoHiddenChaoticPathPlanningControl();
extern	int	DoRabinovichSystemIV();
extern	int	DoUncertainDestinationDynamics();
extern	int	DoANew4DChaoticSystemHiddenAttractor();
extern	int	DoANewOscillatorInfiniteCoexistingAsymmetricAttractors();
extern	int	DoScenarioBirthHiddenAttractors();
extern	int	DoFPGAImplementationAdaptiveSlidingModeControl();
extern	int	DoChaosBasedApplicationNovelNoEquilibriumChaoticSystem();
extern	int	DoSynchronisationBehaviorsCoupledSystems();
extern	int	DoCrackSynchronisationChaoticCircuits();
extern	int	DoAPlethoraCoexistingStrangeAttractors();
extern	int	DoANovel4DNoEquilibriumHyperChaoticSystem();
extern	int	DoImplementationFPGABasedRealTimeNovelChaoticOscillator();
extern	int	DoANovelGridMultiwingChaoticSystem();
extern	int	DoAnalysis4DHyperchaoticFractionalOrderMemristiveSystem();
extern	int	DoDynamicsANewCompositeFourscrollChaoticSystem();
extern	int	DoANovelClassChaoticFlowsInfiniteEquilibria2();
extern	int	DoANovelClassChaoticFlowsInfiniteEquilibria4();
extern	int	DoANovelClassChaoticSystemsDifferentShapesEquilibrium();
extern	int	DoApplicationChaoticOscillatorAutonomousMobileRobot();
extern	int	DoSpontaneousSynchronisation();
extern	int	DoUnknownOscillator();
extern	int	DoANovelMethodConstructingGridMultiWingButterfly();
extern	int	DoSimplestCirculantSystem();
extern	int	DoLabyrinthChaos();
extern	int	DoDixonSystem();
extern	int	DoElegantChaos();
extern	int	DoSystemMultipleAttractorsDynamicAnalysis();
extern	int	DoFractionalOrderPWCSystems();
extern	int	DoAsymmetricDoubleStrangeAttractors();
extern	int	DoThirdOrderRLCMFourElements();
extern	int	DoFractionalOrderSimplestMemristor();
extern	int	DoElegantChaosVF3();
extern	int	DoANovelHyperchaoticSystem();
extern	int	DoANewCostFunctionParameterEstimationChaoticSystems();
extern	int	DoANewNonlinearChaoticComplexModels();
extern	int	DoANewHyperchaoticSystemBasedDesign();
extern	int	DoSolutionDynamicsFractionalOrder5DHyperchaoticSystem();
extern	int	DoSwitchingSynchronisationControl();
extern	int	DoCrisisInverseCrisisRoute();
extern	int	DoMultipleAttractorsChua();
extern	int	DoNumericalExperimentalConfirmations();
extern	int	DoANew4DChaoticSystemHiddenAttractorCircuitImplementation();
extern	int	DoANew3DChaoticSystemWithoutEquilibriumPoints();
extern	int	DoANewFinanceChaoticSystem();
extern	int	DoMarcusTorusKnotOsc();
extern	int	DoTrefoilKnotAOsc();
extern	int	DoGrannyKnotOsc();
extern	int	DoEightKnotOsc();
extern	int	DoTorusKnotOsc();
extern	int	DoCubicTrigonometricNonuniformSplineCurves();
extern	int	DoANewChaoticOscillatorWithFreeControl();
extern	int	DoASimpleChaoticOscillator4EducationalPurposes();
extern	int	DoChaosSymbolComplexity();
extern	int	DoWaveletCharacterisation();
extern	int	DoASimpleChaoticCircuitWithHyperbolicSineFunction();
extern	int	DoKnotUniversesITempestuousOsc();
extern	int	DoSecondOrderAdaptiveTimeVaryingSlidingModeControl();
extern	int	DoResearchNew3DAutonomousChaoticSystem();
extern	int	DoSteadyPeriodicMemristorOscillator();
extern	int	DoASimpleInductorFreeMemristiveCircuit();
extern	int	DoCircuitDesignPredatorPreyModel();
extern	int	DoMegastabilityMultistability();
extern	int	DoConstructingChaoticSystemsAB1();
extern	int	DoConstructingChaoticSystemsAB5();
extern	int	DoANewFractionalOrderChaoticSystem();
extern	int	DoANovelNoEquilibriumHyperchaoticMultiWingSystem();
extern	int	DoANovelFourWingHyperchaoticComplexSystem();
extern	int	DoDynamicAnalysisSynchronisationControl();
extern	int	DoMultivariateMultiscaleComplexityAnalysis14();
extern	int	DoMultivariateMultiscaleComplexityAnalysis16();
extern	int	DoMultivariateMultiscaleComplexityAnalysis17();
extern	int	DoAnalysisControlCircuitDesign();
extern	int	DoObserverBasedSynchronisationChaoticSystems();
extern	int	DoANovelAutonomous5DHyperjerkRCCircuit();
extern	int	DoSingleAmplifierBiquadBasedInductorFreeChuasCircuit();
extern	int	DoHyperchaoticMemcapacitorOscillator1();
extern	int	DoHyperchaoticMemcapacitorOscillator2();
extern	int	DoThreeDimensionalMemristiveHindmarshRoseNeuronModel();
extern	int	DoCoexistingAttractorsCircuitImplementation();
extern	int	DoAnalysisThreeDimensionalChaoticSystem();
extern	int	DoAnalysisNewThreeDimensionalChaoticSystem();
extern	int	DoExtremeMultiStabilityHyperjerkMemristiveSystem();
extern	int	DoBistabilityImprovedMemristorBased3rdOrderWienBridgeOscillator();
extern	int	DoCryptanalysisImprovementImageEncryptionAlgorithm();
extern	int	DoThirdOrderGeneralisedMemristorBasedChaoticCircuit();
extern	int	DoVariousTypesCoexistingAttractors();
extern	int	DoOnA3DGeneralisedHamiltoniansModel();
extern	int	DoSuppressionChaosPorousMedia();
extern	int	DoSynchronisationChaoticSystemsFeedbackController();
extern	int	DoApproximatePredictionBasedControlMethod();
extern	int	DoANew4DChaoticAttractor();
extern	int	DoDynamicalBehavior3DJerkSystem();
extern	int	DoHowToBridgeAttractorsRepellorsII();
extern	int	DoMemristorBasedCanonicalChuasCircuit();
extern	int	DoDynamicalBehaviorsCircuitRealisationChaosControl();
extern	int	DoGeneratingMultiscrollChaoticAttractors();
extern	int	DoMultiswitchingSynchronisation();
extern	int	DoANovelDigitalProgrammableMultiScrollChaoticSystem();
extern	int	DoANovel3DFractionalOrderChaoticSystem();
extern	int	DoChaosNovelFractionalOrderSystem();
extern	int	DoANew3DChaoticSystemConchShapedEquilibriumCurve();
extern	int	DoDeterminingChaoticBehaviorFractionalOrderFinanceSystem();
extern	int	DoAStrangeNovelChaoticSystemFullyGoldenProportionEquilibria();
extern	int	DoAdaptiveHybridSynchronisationChaoticSystems();
extern	int	DoDesignMultiwingMultiscrollGridCompoundChaoticSystem();
extern	int	DoHopfBifurcationAnalysisChaosControl();
extern	int	DoANovelNoEquilibriumChaoticMultiWingButterfly();
extern	int	DoDynamicsFPGARealisationApplicationChaoticSystem();
extern	int	DoStabilisationSynchronisationUncertainZhangSystem();
extern	int	DoMagneticStochasticity();
extern	int	DoCoexistenceSingleMultiScrollChaoticOrbits();
extern	int	DoANew5DHyperchaoticSystem();
extern	int	DoCoexistingAttractorsCrisisRoute2Chaos();
extern	int	DoChaosIntegerOrderFractionalOrderFinancialSystems();
extern	int	DoSineCosineOptimisationBasedBijectiveSubstitution();
extern	int	DoConstructingAnalysingSynchronisingChaoticSystem();
extern	int	DoDynamicsChaosControlSynchronisationFractionalOrder();
extern	int	DoNoChatteringAdaptiveSlidingModeControl();
extern	int	DoMechanicsAnalysisHardwareImplementation();
extern	int	DoDynamicsAtInfinityHopfBifurcation();
extern	int	DoYetAnotherChaoticAttractor();
extern	int	DoSelfExcitedHiddenAttractors();
extern	int	DoAnalysisNewThreeDimensionalSystem();
extern	int	DoDynamicsFeatureSynchronisation();
extern	int	DoSynchronisationAntiSynchronisation();
extern	int	DoAnImageEncryptionAlgorithm();
extern	int	DoBifurcationsNewFractionalOrderSystem();
extern	int	DoChaosAdaptiveSynchronisationsFractionalOrderSystems();
extern	int	DoCombinationCombinationPhaseSynchronisation();
extern	int	DoANovelMemductorBasedChaoticSystem();
extern	int	DoAChaoticSystemInfiniteEquilibriaSBox();
extern	int	DoDynamicsComplexityNew4DChaoticLaserSystem();
extern	int	DoANewHyperchaoticMapOsc();
extern	int	DoParallelisingBoundarySurfaceComputationChuasCircuit();
extern	int	DoComplexDynamicsHiddenAttractors();
extern	int	DoAnalysisImplementationNewSwitchingHyperchaoticSystem();
extern	int	DoAntimonotonicityCrisisMultipleAttractors();
extern	int	DoBluetoothBasedChaosSynchronisation();
extern	int	DoMultiSwitchingCombinationSynchronisation();
extern	int	DoInventionOpened();
extern	int	DoANew3DChaoticSystemHiddenAttractor();
extern	int	DoANew3DChaoticSystemHiddenAttractor6D();
extern	int	DoHybrid6D();
extern	int	DoSynchronisationDependenceInitialSetting3D();
extern	int	DoSynchronisationDependenceInitialSetting6D();
extern	int	DoExistenceStabilityChimeraStates();
extern	int	DoAnalysisProjectiveSynchronisationNew4DHyperchaoticSystem();
extern	int	DoANovelControlMethodCounteractDynamicalDegradation();
extern	int	DoNonstationaryChimeras();
extern	int	DoDormandPrinceEldest();
extern	int	DoCoexistingAttractorsGenerated4DSmoothChaoticSystem();
extern	int	DoFullyIntegratedMemristor();
extern	int	DoParameterIdentificationFractionalOrderDiscreteChaoticSystems();
extern	int	DoConstructingChaoticSystemWithMultipleCoexistingAttractors();
//extern	int	DoTheDynamicsCoupledOscillators();
extern	int	DoMatlabCodeForLyapunovExponents();
//extern	int	DoCryptanalysingNovelColorImageEncryption();
extern	int	DoAnEightTermNovelFourScrollChaoticSystem();
extern	int	DoDegeneratingButterflyAttractor();
extern	int	DoMarcusBiMap();
extern	int	DoSpiralWavesExternallyExcitedNeuronalNetwork();
extern	int	DoAMinimal3TermChaoticFlowCoexistingRoutesChaos();
extern	int	DoANewMemristorBased5DChaoticSystem();
extern	int	DoExtremeMultiStabilityHyperjerkMemristiveSystem2D();
extern	int	DoOne2FourWingChaoticAttractorsCoinedNovel3DFractionalOrderChaoticSystem();
extern	int	DoEnergyFeedBackSynchronousDynamicsHindmarshRoseNeuronModel();
extern	int	DoHyperchaosEvolvedLiuChaoticSystem();
extern	int	DoPeriodicSolutionHigherDimensionalEcologicalSystem();
extern	int	DoColourImageCompressionEncryptionAlgorithm();
extern	int	DoDissipativeConservativeChaoticNatureQuasiPeriodicallyForcedOscillator();
extern	int	DoCascadingMethod();
extern	int	DoANewChaoticSystemMultipleLineEquilibria();
extern	int	DoSlidingModeDisturbanceObserverControlBasedAdaptiveSynchronisation();
extern	int	DoSimplestHyperchaoticSystem();
extern	int	DoChaoticDynamicsModifiedWienBridgeOscillator();
extern	int	DoANewFourScrollChaoticSystem();
extern	int	DoANewTwoScrollChaoticSystem();
extern	int	DoDynamicsEntropyAnalysis();
extern	int	DoOnDifferentFamiliesHiddenChaoticAttractors();
extern	int	DoNumericalSimulation();
extern	int	DoNoseHoover4D();
extern	int	DoCoexistenceHiddenAttractors2Torus3Torus();
extern	int	DoNonlinearDynamicalModelThreeQuadraticNonlinearTerms();
extern	int	DoASimpleChaoticSystemWithTopologicallyDifferentAttractors();
extern	int	DoANewFamilyChaoticSystemsDifferentClosedCurveEquilibrium();
extern	int	DoANewNoEquilibriumChaoticSystem();
extern	int	DoANew3DCMemristiveTimeDelayChaoticSystem();
extern	int	DoNumericalAnalysisSimplestFractionalOrderHyperchaoticSystem();
extern	int	DoGenerationSuppressionNewHyperchaoticNonlinearModel();
extern	int	DoAGigaStableOscillatorHiddenSelfExcitedAttractors();
extern	int	DoAFishBiologyChaoticSystemCircuitDesign();
extern	int	DoAMemristorMeminductorBasedChaoticSystem();
extern	int	DoAnalysisControlFPGAImplementation();
extern	int	DoCoexistenceGeneralisedSynchronisation();
extern	int	DoFPGABasedChaoticOscillator();
extern	int	DoDesignSimulationMemristorChaoticCircuit();
extern	int	DoPeriodicallySwitchedMemristor();
extern	int	DoANovelChaoticSystem2Circles();
extern	int	DoAnalysisChaoticSystemWithLineEquilibrium();
extern	int	DoFractionalSymbolicNetworkEntropyAnalysis();
extern	int	DoPeriodDoublingRoute2Chaos();
extern	int	DoSecureCommunicationUsingNewHyperchaoticSystem();
extern	int	DoDynamicBehaviorsAnalysisChaoticCircuit();
extern	int	DoEntropyAnalysisImageEncryptionApplication();
extern	int	DoDynamicsAnalysisImplementationMultiscroll();
extern	int	DoANew3DChaoticSystem4QuadraticNonlinearTerms();
extern	int	DoANew4DHyperchaoticSystem();
extern	int	DoControlMultistability();
extern	int	DoCombinationSynchronisation3NonidenticalEcologicalSystems3D();
extern	int	DoCombinationSynchronisation3NonidenticalEcologicalSystems4D();
extern	int	DoANewColourimageEncryption();
extern	int	DoFractionalOrderSimpleChaoticOscillator();
extern	int	DoCategorisingChaoticFlows();
extern	int	DoEntropyAnalysisNeuralNetworkBasedAdaptiveControl();
extern	int	DoDesignFixedPointBasedDualEntropyCore();
extern	int	DoExtremeMultistabilityAnalysis();
extern	int	DoNovelMultiwingFractionalorderChaoticSystem();
extern	int	DoSynchronisationApplicationNovelFractionalOrderKingCobra();
extern	int	DoAbundantCoexistingMultipleAttractorsBehaviors();
extern	int	DoDimensionalityReductionReconstitutionExtremeMultistability();
extern	int	DoInductorFreeMultiStableChuasCircuit();
extern	int	DoYetAnotherUnknownChaoticAttractor();
extern	int	DoChaoticCuttlesh();
extern	int	DoElementaryQuadraticChaoticFlows();
extern	int	DoANovelPlaintextRelatedImageEncryptionAlgorithm();
extern	int	DoResearchGridScrollChaoticExtendedSequenceAlgorithm();
extern	int	DoGenerationFamilyFractionalOrderHyperChaoticMultiScrollAttractors();
extern	int	DoGenerationFamilyFractionalOrderHyperChaoticMultiScrollAttractors5D();
extern	int	DoNewNonlinearActiveElement();
extern	int	DoANewChaoticImageEncryptionScheme();
extern	int	DoSynchronisingNonautonomousChaoticCircuits();
extern	int	DoMedicalImageEncryptionAlgorithm();
extern	int	DoChaoticBehaviorFeedbackControl();
extern	int	DoHardwareOptimisedFPGAImplementations();
extern	int	DoANovelMultiAttractorPeriodMultiScroll();
extern	int	DoBirth1to4WingChaoticAttractors();
extern	int	DoConstructingInfinitelyManyAttractors();
extern	int	DoDynamicsSynchronizationElectronicImplementations();
extern	int	DoInfinitelyManyCoexistingConservativeFlows();
extern	int	DoSecureCommunicationScheme5DMultistableFourWing();
extern	int	DoGeneratedSecondGenerationCurrentConveyorCircuit();
extern	int	DoConstructingMultiwingAttractors();
extern	int	DoOffsetBoostingBreedingConditionalSymmetry();
extern	int	DoAConditionalSymmetricMemristiveSystem();
extern	int	DoAnInfinite3DLatticeStrangeAttractors();
extern	int	DoInfiniteMultistability();
extern	int	DoMultistabilityButterflyFlow();
extern	int	DoHypogeneticChaoticJerkFlowsTypeHJ7();
extern	int	DoSimulationImplementationMemristiveChaoticSystem();
extern	int	DoFieldCouplingSynchronisation();
extern	int	DoColourImageCompressionEncryptionAlgorithmNeuralNetworkDNA();
extern	int	DoFPGAImplementationNovelChaoticOscillator();
extern	int	DoAMemristiveChaoticOscillatorControllableAmplitudeFrequency();
extern	int	DoModellingChaoticProcessesCaputoFractionalOrderDerivative();
extern	int	DoANovelChaoticSystemApplicationSecureCommunications();
extern	int	DoAnUnforcedMegastableChaoticOscillator();
extern	int	DoBistabilityFifthOrderVoltageControlledMemristorBasedChua();
extern	int	DoRiddledAttractionBasinMultistability();
extern	int	DoDynamicsControlTokamaks();
extern	int	DoEffectMarketConfidenceFinancialSystem();
extern	int	DoRosslerPrototype4System();
extern	int	DoAnalysisStabilisationDSPBasedImplementation();
extern	int	DoDoWeneedMoreChaosExamples();
extern	int	DoDesignInterfaceRandomNumberGenerators();
extern	int	DoCoexistingMultiStablePatterns();
extern	int	DoDesignImplementationNewMemristiveChaoticSystem();
extern	int	DoEmergenceSquareChaoticAttractor();
extern	int	DoHeterogeneousMultistability();
extern	int	DoInitialOffsetBoostingCoexistingAttractors();
extern	int	DoMultistabilityAnalysisCoexistingMultipleAttractors();
extern	int	DoANovelMegastableHamiltonianSystem();
extern	int	DoChaosLearningSimple2PersonGame();
extern	int	DoDynamicsSynchronisationMemristorBasedChaoticSystem();
extern	int	DoModellingChaoticSystemMotionInVideo();
extern	int	DoDynamicalAnalysisElectronicCircuitRealisation();
extern	int	DoStudyTwoMemcapacitorCircuitModel();
extern	int	DoSynchronisationChaoticSystemsAdaptiveControl();
extern	int	DoAsymmetryCoefcientsIndicatorsChaos();
extern	int	DoChaosSynchronisation();
extern	int	DoConstructingChaoticSystem2Equilibria();
extern	int	DoConstructingChaoticSystem3Equilibria();
extern	int	DoAnEfficientApproachFractionalNonlinearChaoticModel();
extern	int	DoSelfReproducingChaosBurstingOscillationAnalysis();
extern	int	DoANovelFourDimensionalHyperchaoticSystem();
extern	int	DoControlSynchronisation7DHyperchaoticSystem();
extern	int	DoDescribingChaoticAttractors();
extern	int	DoExtremeMultistabilityComplexDynamics();
extern	int	DoGloballyExponentialSynchronisationCriterionChaoticOscillators();
extern	int	DoTransientChaosFractionalBlochEquations();
extern	int	DoCAMOChaoticFlows();
extern	int	DoNewChaoticAttractorsFractionalDifferentiationIntegration();
extern	int	DoASemiSymmetricImageEncryptionScheme();
extern	int	DoANewHamiltonianChaoticSystem();
extern	int	DoANovelNoEquilibriumHRNeuronModel();
extern	int	DoAUniqueSignumSwitch();
extern	int	DoAnalysisThreeTypesInitialOffsetBoostingBehaviour();
extern	int	DoDynamicsAirfoil();
extern	int	DoCoexistingInfinitelyManyAttractors();
extern	int	DoConstructingChaoticRepellors();
extern	int	DoHiddenAttractorDynamicalCharacteristic();
extern	int	DoModellingCircuitRealisation();
extern	int	DoMultistabilitySymmetricChaoticSystems();
extern	int	DoANovelChaosApplication();
extern	int	DoCircuitImplementationModifiedChaoticSystem();
extern	int	DoLambdaPhiGeneralisedSynchronisation();
extern	int	DoEffectsPadeNumericalIntegration();
extern	int	DoDeterministicChaosStochasticFluctuation();
extern	int	DoSynchronisationCyclicCoupledHyperchaoticSystems();
extern	int	DoDynamicsControlSymmetryBreaking();
extern	int	DoPseudoRandomNumberGenerator();
extern	int	DoASimpleMemristiveJerkSystem();
extern	int	DoHyperChaoticCircuitMemristorFeedback();
extern	int	DoLowPowerPseudoRandomNumberGenerator();
extern	int	DoPerformanceEnhancingRSAPublicKey();
extern	int	DoInvisibleGrazings();
extern	int	DoMultistageSpectralRelaxationMethod();
extern	int	DoSpatialFeedbackControl();
extern	int	DoAStudyonFourSpeciesFractionalPopulation();
extern	int	DoA9DModelScholarpaedia();
extern	int	DoExtremeMultistabilityAnalysis5D();
extern	int	DoAMultiWingSphericalChaoticSystem();
extern	int	DoMegastabilityCoexistenceCountableInfinity1();
extern	int	DoMegastabilityCoexistenceCountableInfinity2();
extern	int	DoMultistabilityBifurcations5D();
extern	int	DoMultistabilityNovelChaoticSystem();
extern	int	DoOptimalSynchronisationOscillatorsI();
extern	int	DoOptimalSynchronisationOscillatorsII();
extern	int	DoHighDimensional4PositiveLyapunovExponents();
extern	int	DoAnalysisSynchronisationRoboticApplication();
extern	int	DoAFamilyCirculantMegastableChaoticOscillators();
extern	int	DoATrueThreeScrollChaoticAttractorCoined();
extern	int	DoANewNoEquilibriumFractionalOrderChaoticSystem();
extern	int	DoMultistabilityAnalysis();
extern	int	DoFPGATabanhFarkhNumerik();
extern	int	DoOnOffsetBoostingChaoticSystem7();
extern	int	DoANovelMegastableOscillator();
extern	int	DoOysterOscillator();
extern	int	DoExperimentalVerificationVoltAmpereCharacteristicCurve();
extern	int	DoMegastableSystemWith2DStrip();
extern	int	DoPeriodicOffsetBoostingAttractorSelfReproducing();
extern	int	DoImproved7DHyperchaoticMapBasedImageEncryptiontechnique();
extern	int	DoDesignDSPImplementationFractionalOrderDetunedLaser();
extern	int	DoNovelDynamicalBehaviors();
extern	int	DoHamiltonEnergy();
extern	int	DoExtremelyRichDynamics();
extern	int	DoFractionalOrderBiologicalSnapOscillator();
extern	int	DoComplexDynamicsMemristiveDiodeBridge();
extern	int	DoMultistabilitySelfReproducingSystems();
extern	int	DoDesignMultiwingMultiscroll3DChaoticSystems();
extern	int	DoDesignMultiWing3DChaoticSystems11();
extern	int	DoDesignMultiWing3DChaoticSystems15();
extern	int	DoExperimentalStudyDynamicBehaviorDoubleScrollCircuit();
extern	int	DoAMemristiveChaoticystemHypermultistability();
extern	int	DoANewHyperchaotic2ScrollSystemBifurcationStudy();
extern	int	DoA5DHyperchaoticRikitakeDynamoSystem();
extern	int	DoDynamicalAnalysisNovelFractionalOrderChaoticSystem();
extern	int	DoHarmonicOscillatorsNonlinearDampingNoseHoover();
extern	int	DoHarmonicOscillatorsNonlinearDampingKBBOscillator();
extern	int	DoHarmonicOscillatorsNonlinearDampingKMunmuangsaenscillator();
extern	int	DoHarmonicOscillatorsNonlinearDampingMKTErgodicOscillator();
extern	int	DoHarmonicOscillatorsNonlinearDamping0532ErgodicOscillator();
extern	int	DoANewMultiWingChaoticAttractor();
extern	int	DoDynamicalStudyNovel4DHyperchaoticSystem();
extern	int	DoAChaoticSystemInfiniteNumberEquilibriumPoints();
extern	int	DoTorusFixedPointAttractors();
extern	int	DoNumericalSensitivityAnalysis();
extern	int	DoChaosPendulumAdaptiveFrequencyOscillator();
extern	int	DoBifurcationBehaviorPDControlMechanism();
extern	int	DoMemristorBasedChaoticSystem();
extern	int	DoAConservativeSystemTriangularWaveMemristor();
extern	int	DoANewImageEncryptionScheme();
extern	int	DoImageEncryptionBase_nPRNGs();
extern	int	DoRobustMultipleScrollDynamics();
extern	int	DoFractionalOrderInducedBifurcations();
extern	int	DoA3DJerkSystem();
extern	int	DoAnIntroductions2HarmonicKnotsOsc();
extern	int	DoSyncBetween2NonAutonomousChaoticSystems();
extern	int	DoMultistabilityVariableMultiDirectionalHiddenAttractors();
extern	int	DoMultiFoldedHiddenChuasAttractors30();
extern	int	DoMultiFoldedHiddenChuasAttractors33();
extern	int	DoAConservativeMemristiveChaoticSystem();
extern	int	DoBouali3();
extern	int	DoDynamicalAnalysisQuadraticMegastableChaoticOscillator();
extern	int	DoChenCircuitLikeModel();
extern	int	DoDynamicsNew3DChaoticSystem();
extern	int	DoAnalyticProofEmergenceNewTypeLorenzAttractors();
extern	int	DoNonlinearAnalysis4DFractionalHyperChaoticSystem();
extern	int	DoDNADynamicCodingImageEncryptionAlgorithm();
extern	int	DoDynamicBehaviorsFarNearMemristiveElectromagneticInduction();
extern	int	DoNovelGrayscaleImageEncryption();
extern	int	DoDynamicsStabilizationChaoticMonetarySystem();
extern	int	DoRosslerChaoticOscillatorControlledPhaseImpact();



// Fractal Maps
extern	int	DoTinkerbell();
extern	int	DoBasin();
extern	int	DoKnot();
extern	int	DoFeigenbaum();
extern	int	DoHyperLogistics();
extern	int	DoClifford();
extern	int	DoPolynomA();
extern	int	DoPolynomB();
extern	int	DoPickover();
extern	int	DoMiraGumowski();
extern	int	DoFractalDream();
extern	int	DoHopaJong();
extern	int	DoLorenz3D();
extern	int	Do2DRational();
extern	int	DoLorenz2D();
extern	int	DoLorenz84();
extern	int	Do2DCircular();
extern	int	DoQuadruple2();
extern	int	DoIcons3D();
extern	int	DoDelayedLogistic();
extern	int	DoDuffingChaotic();
extern	int	DoLozi();
extern	int	DoSvensson();
extern	int	DoDuffings();
extern	int	DoIkeda2D();
extern	int	DoMaluquice();
extern	int	DoSierpinskiPoly();
extern	int	DoChaoticEvolutionMap();
extern	int	DoIlogisticMap();
extern	int	DoHenonMap();
extern	int	DoFatHenonMap();
extern	int	DoMulticavityFormations();
extern	int	DoANewClassThreeDimensionalMaps1();
extern	int	DoANewClassThreeDimensionalMaps2();
extern	int	DoANewClassThreeDimensionalMaps3();
extern	int	DoANewClassThreeDimensionalMaps4();
extern	int	DoANewClassThreeDimensionalMaps5();
extern	int	DoANewClassThreeDimensionalMaps6();
extern	int	DoANewClassThreeDimensionalMaps13();
extern	int	DoANewClassThreeDimensionalMaps14();
extern	int	DoANewClassThreeDimensionalMaps16();
extern	int	DoThreeDimensionalHenonLikeMaps();
extern	int	DoSwirlPickover();
extern	int	DoAHyperchaoticMap();
extern	int	DoKanekoI();
extern	int	DoKanekoII();
extern	int	DoWuYang();
extern	int	DoCubicHenon();
extern	int	DoKakadu();
extern	int	DoMaynardSmith();
extern	int	DoMacMillan();
extern	int	DoDualHenonMap();
extern	int	DoModifizierteMira();
extern	int	DoBusinessCycle();
extern	int	DoElhadjSprottMap();
extern	int	DoSFSIMMHighDimensionalHyperchaoticMap();
extern	int	DoLauwerierAttractor();
extern	int	DoGumowskiMira();
extern	int	DoCathalaMAP();
extern	int	DoBurgers();
extern	int	DoMarottoLorenz();
extern	int	DoPeterDeJong();
extern	int	DoUshiki();
extern	int	DoPopcorn2();
extern	int	DoMetz();
extern	int	DoStrick();
extern	int	DoSquareHenon();
extern	int	DoBifurcationsInLoziMap();
extern	int	DoGingerBreadMan();
extern	int	DoSynchronisationChaoticSystemsMap();
extern	int	DoANovelClassControlMethodCounteractDynamicalDegradation();
extern	int	DoGingerbreadWoman();
extern	int	DoExistenceStabilityChimeraStatesMap();
extern	int	DoExistenceStabilityChimeraStates3DMap();
extern	int	DoDesignNewChaoticMaps();
extern	int	DoA2DimensionalChaoticQuasiConservativeMap();
extern	int	DoChaosHyperchaosSymmetricCoupling();
extern	int	DoBogdanovMapBifurcations();
extern	int	DoFractionalFormNew3DGeneralisedHenonMap();
extern	int	DoAnAlgorithmImageEncryptionUsingLogistic2DimensionalChaoticEconomicMaps();
extern	int	DoChaosSynchronisation3DHenonMap();
extern	int	DoChaosSynchronisationFoldMap();
extern	int	Do2DSIMMMap();
extern	int	Do2DSineLogisticModulationMap();
extern	int	DoHyperchaoticTowelMap();
extern	int	DoChaosSynchronisationHyperchaos();
extern	int	DoChaoticMapNoFixedPoints();
extern	int	Do2DLogisticSineCouplingMapImageEncryption();
extern	int	DoRobustCompressionEncryptionImagesBasedSPIHTCoding();
extern	int	DoDynamicsHigherDimensionalFractionalOrderChaoticMap();
extern	int	DoComplexDynamicsInGeneralisedHénonMap();
extern	int	DoOnDynamicsControlSynchronisationFractionalOrderIkedaMap();
extern	int	DoDynamicalTrappingAreaPreservingHénonMap();
extern	int	DoChaosHyperchaosTransitionMap();
extern	int	DoANewChaoticMapSecureTransmission();
extern	int	DoRouteChaosViaStrangeNonChaoticAttractors();
extern	int	DoRobustChaosCubicPolynomialDiscreteMaps();
extern	int	Do2DLogisticModulatedSineCouplingLogisticChaoticMap();
extern	int	DoNovel2DFractionalOrderDiscreteChaoticMap();
extern	int	DoBogdanovTakensBifurcationMap();
extern	int	DoChirikovStandardMap();
extern	int	DoANewSimple2DPiecewiseLinearMap();
extern	int	DoClassifyingQuantifyingBasinsAttraction();
extern	int	DoClassifyingQuantifyingBasinsAttractionSineSine();
extern	int	DoVarietyStrangePseudohyperbolicAttractorsI();
extern	int	DoVarietyStrangePseudohyperbolicAttractorsII();
extern	int	DoVarietyStrangePseudohyperbolicAttractorsIII();
extern	int	DoVarietyStrangePseudohyperbolicAttractorsIV();
extern	int	DoVarietyStrangePseudohyperbolicAttractorsV();
extern	int	DoANewClass2DChaoticMaps();
extern	int	DoANewHyperchaoticMap();
extern	int	DoDynamicsOptimisationControl();
extern	int	DoNewAlgorithmEncryptionDecryptionMap();
extern	int	DoElectronicallyImplementedCoupledLogisticMaps();
extern	int	DoHenonMarcusMap();
extern	int	DoANewChaoticAttractorDiscreteMapping();
extern	int	DoANewNonlinearOscillatorInfiniteNumberMap();
extern	int	DoBifurcationsChaos3DHénonMap();
extern	int	DoMultistabilityCyclicAttractors();
extern	int	DoPiObserverDesign();
extern	int	DoErgodicTheoryMap();
extern	int	DoDynamicsNewMultiCavityHyperChaoticMap();
extern	int	DoConstructingChaoticMapMultiCavity();
extern	int	DoMultistabilityCoexistingAttractors();
extern	int	DoMeltzlerMap();
extern	int	DoControllingChaosMapsMultipleStrangeAttractors();
extern	int	DoLorenzDiscreteTimeMap();
extern	int	DoAbundanceAttractingRepellingEllipticPeriodicOrbits();
extern	int	DoAHigherDimensionalChaoticMap2();
extern	int	DoAHigherDimensionalChaoticMap3();
extern	int	DoControllingHiddenDynamicsMultistabilityXII();
extern	int	DoControllingHiddenDynamicsMultistabilityXIV();
extern	int	DoControllingHiddenDynamicsMultistabilityXVI();
extern	int	DoShadowingHiddenAttractorsEquationIV();
extern	int	DoShadowingHiddenAttractorsEquationV();
extern	int	DoBoostedCoexistingChaos();
extern	int	DoApplicationNew4DChaoticMap();
extern	int	DoChaosBasedEngineeringApplications();
extern	int	DoInitialSwitchedBoostingBifurcations();
extern	int	Do2DMemristiveHyperchaoticMapsMLM();
extern	int	Do2DMemristiveHyperchaoticMapsMTM();
extern	int	Do2DMemristiveHyperchaoticMapsSMM();
extern	int	Do2DMemristiveHyperchaoticMapsMSN();
extern	int	DoComplexityDynamicCharacteristicsMap();
extern	int	DoGumowskiMira3Wings();
extern	int	DoGumowskiMira5Wings();
extern	int	DoOn3DFractionalOrderHenonMap();
extern	int	DoAnAmplitudeControllable3DHyperchaoticMap();
extern	int	DoChossatGolubitskyMaps();
extern	int	DoImageEncryptionTechnologyFractional2DTriangleFunction();
extern	int	DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth();
extern	int	DoChaosControlSynchronisation();
extern	int	DoMultikeyImageEncryptionAlgorithm();
extern	int	DoTheSimplestCaseStrangeAttractorsMap();
extern	int	DoALightweightImageEncryptionAlgorithmMap();
extern	int	DoAnImprovedImageEncryptionAlgorithmMap();
extern	int	DoHyperchaoticAttractors3DMaps();
extern	int	DoImageEncryptionUsing2DLogisticChaoticMap();
extern	int	DoLorenzDuffingAlikeMap();
extern	int	DoOnFamilyMaps();
extern	int	Do2DMAPModeloExperimental();
extern	int	DoASecureImageEncryptionSchemeMap();
extern	int	DoSomeOpenProblemsChaosTheory2();
extern	int	DoSomeOpenProblemsChaosTheory3();
extern	int	DoDynamicsControlFractionalFormsChangMap();
extern	int	DoDynamicsControlFractionalFormsRulkovMap();
extern	int	DoDynamicsControlFractionalFormsZeraculiaMap();
extern	int	DoANovelChaosBasedImageEncryptionAlgorithmMap();
extern	int	DoAsymmetryEvolvementControllabilityMap();
extern	int	DoMemristiveRulkovNeuronModelMap();
extern	int	DoAreMathematicsSourceArtisticCreationPropertiesMap430();
extern	int	DoAreMathematicsSourceArtisticCreationPropertiesMap44();
extern	int	DoAreMathematicsSourceArtisticCreationPropertiesMap451();
extern	int	DoAreMathematicsSourceArtisticCreationPropertiesMap452();
extern	int	DoAreMathematicsSourceArtisticCreationPropertiesMap453();
extern	int	DoParameterIdentificationMap();
extern	int	DoFourDimensionalTinkerbelle();
extern	int	DoMiraMap08();
extern	int	DoMiraMap09();
extern	int	DoMiraMap12();
extern	int	DoMiraMap13();
extern	int	DoMiraMap15();
extern	int	DoMiraMap16();
extern	int	DoMiraMap19();
extern	int	DoMiraMap21();
extern	int	DoMiraMap23();
extern	int	DoMiraMap24();
extern	int	DoMiraMap25();
extern	int	DoMiraMap26();
extern	int	DoMiraMap29();
extern	int	DoMiraMap30();
extern	int	DoMiraMapX();
extern	int	DoANovelClassTwoDimensionalChaoticMaps();
extern	int	DoAnInitialControlledDoubleScrollHyperchaoticMap();
extern	int	DoANovelMegastableOscillatorMAP();
extern	int	DoEffectSymmetryBreakingInertialNeuralSystem();
extern	int	DoHomoclinicTangenciesMap();
extern	int	DoCoexistingInfiniteOrbits();
extern	int	DoTwoVariableBoostingBifurcationMap();
extern	int	DoIdentificationChaosFractionalChaoticMapBurgersMap();
extern	int	DoAColorImageEncryptionAlgorithmMap();
extern	int	DoSymmetricKeyEncryption();
extern	int	DoTransformationsSpikeBurstOscillations();
extern	int	DoInfluenceParametricSymmetryDynamics3DSinusoidalDiscreteSystemsMap();
extern	int	DoComplexDynamicsSimplestNeuronModelMap();
extern	int	DoBiometricInformationSecurityMap();
extern	int	DoChipMap();
extern	int	DoThreePlyMap();
extern	int	DoAntiMartinMap();
extern	int	DoQuadruple2Fractint();
extern	int	DoGingerbreadManVariations();
extern	int	DoCoexistenceThreadSheetChaoticAttractors();
extern	int	DoExplorePotentialDeepLearningMap();
extern	int	DoDynamicFractalMap();
extern	int	DoComplexDynamics2DSineBoundedMemristiveMap();
extern	int	DoFurtherStudyontheRulkovNeuronMap();
extern	int	DoAttractorMergingAmplitudeControlHyperchaosMap();
extern	int	DoDesignHardwareImplementation4DMemristiveHyperchaoticMap();
extern	int	DoTorusHyperchaos3DLotkaVolterraMap();
extern	int	DoAnalysisFIPS140();


// Sprott Maps

extern	int	DoSprottFractaLMapE();
extern	int	DoSprottFractaLMapF();
extern	int	DoSprottFractaLMapG();
extern	int	DoSprottFractaLMapH();
extern	int	DoSprottFractaLMapI();
extern	int	DoSprottFractaLMapJ();
extern	int	DoSprottFractaLMapM();
extern	int	DoSprottFractaLMapQ();
extern	int	DoSprottFractaLMapR();
extern	int	DoSprottFractaLMapU();
extern	int	DoSprottFractaLMapY();
extern	int	DoSprottFractaLMapZ();
extern	int	DoSprottFractaLMap1();
extern	int	DoSprottFractaLMap2();
extern	int	DoSprottFractaLMap3();
extern	int	DoSprottFractaLMap4();


// Surfaces

extern	int	SurfaceSetup();

extern	int	DoBoysSurface();
extern	int	DoSteinbachScrew();
extern	int	DoDinisSurface();
extern	int	DoKleinBottle();
extern	int	DoKuensSurface();
extern	int	DoEnnepersSurface();
extern	int	DoHelicoid();
extern	int	DoTranguloidTrefoil();
extern	int	DoKidneySurface();
extern	int	DoPlückersConoid();
extern	int	DoCosineSurfaceI();
extern	int	DoSineSurface();
extern	int	DoMöbiusBand();
extern	int	DoHennebergsSurface();
extern	int	DoHyperbolicHelicoid();
extern	int	DoHorn();
extern	int	DoTwistedEightTorus();
extern	int	DoEightTorus();
extern	int	DoWhitneyUmbrella();
extern	int	DoAntisymmetricTorus();
extern	int	DoNormalTorus();
extern	int	DoMaedersOwl();
extern	int	DoStilettoSurface();
extern	int	DoAppleISurface();
extern	int	DoPillowShape();
extern	int	DoBohemianDome();
extern	int	DoWaveBall();
extern	int	DoSchnecke();
extern	int	DoSnailSurface();
extern	int	DoCornucopia();
extern	int	DoCrossCup();
extern	int	DoHandkerchiefSurface();
extern	int	DoMonkeySaddle();
extern	int	DoRomanSurface();
extern	int	DoSeashell();
extern	int	DoShoeSurface();
extern	int	DoCatenoid();
extern	int	DoParaboloid();
extern	int	DoFunnel();
extern	int	DoTractroid();
extern	int	DoCatalansSurface();
extern	int	DoHyperboloid();
extern	int	DoPseudosphere();
extern	int	DoSwallowSurface();
extern	int	DoWallisConicalEdge();
extern	int	DoRichmondSurface();
extern	int	DoScherkSurface();
extern	int	DoEllipsoid();
extern	int	DoEightSurface();
extern	int	DoJetSurface();
extern	int	DoDropWater();
extern	int	DoTriaxialTeardrop();
extern	int	DoFishSurface();
extern	int	DoBentHorns();
extern	int	DoHyperbolicOctahedron();
extern	int	DoPseudoCrossCap();
extern	int	DoCrescent();
extern	int	DoTrefoilKnotI();
extern	int	DoUmbilicalTorus();
extern	int	DoWaveTorus();
extern	int	DoWreath();
extern	int	DoTwistedPipeSurface();
extern	int	DoLoop();
extern	int	DoDupinCyclide();
extern	int	DoEllipticalTorus();
extern	int	DoGuimardSurface();
extern	int	DoMennsSurface();
extern	int	DoMilkCarton();
extern	int	DoSaddleTorus();
extern	int	DoTriaxialTritorus();
extern	int	DoVerrillSurface();
extern	int	DoZindlersConoid();
extern	int	DoFolium();
extern	int	DoLemniscape();
extern	int	DoTriplePointTwist();
extern	int	DoTrashCan();
extern	int	DoSpring();
extern	int	DoBodyRevolution();
extern	int	DoCone();
extern	int	DoInsulator();
extern	int	DoEgg();
extern	int	DoSnailsnShells();
extern	int	DoOloid();
extern	int	DoHyperbolicParaboloid();
extern	int	DoAppleIISurface();
extern	int	DoCylinder();
extern	int	DoFacingSnail();
extern	int	DoPaperBag();
extern	int	DoJeenerKleinSurface();
extern	int	DoBonanJeenerKleinSurface();
extern	int	DoBraidedTorus();
extern	int	DoKleinCycloid();
extern	int	DoLimpetTorus();
extern	int	DoDoubleCone();
extern	int	DoHoleDisk();
extern	int	DoPisotTriaxial();
extern	int	DoTriaxialHexatorus();
extern	int	DoTwistedHeart();
extern	int	DoTwistedSphere();
extern	int	DoHoleDiscus();
extern	int	DoSphereI();
extern	int	DoSphereII();
extern	int	DoSphereIII();
extern	int	DoSphereIV();
extern	int	DoWaves();
extern	int	DoSineWaves();
extern	int	DoCosineWaves();
extern	int	DoSpiralWaves();
extern	int	DoBell();
extern	int	DoBellPolar();
extern	int	DoBellShaft();
extern	int	DoSoucoupoid();
extern	int	DoEnnepersSurfacePolar();
extern	int	DoLemon();
extern	int	DoSineCone();
extern	int	DoStrangledTorus();
extern	int	DoAsteroidTorus();
extern	int	DoTricuspoidTorusI();
extern	int	DoTricuspoidTorusII();
extern	int	DoNephroidTorusI();
extern	int	DoNephroidTorusII();
extern	int	DoLemniscateTorusI();
extern	int	DoLemniscateTorusII();
extern	int	DoEpicycloidTorusI();
extern	int	DoEpicycloidTorusII();
extern	int	DoHypocycloidTorusI();
extern	int	DoHypocycloidTorusII();
extern	int	DoGyro();
extern	int	DoEpicycloidCylinder();
extern	int	DoHypocycloidCylinder();
extern	int	DoLemniscateCylinder();
extern	int	DoStrophoidCylinder();
extern	int	DoCissoidCylinder();
extern	int	DoVersieraAgnesiCylinder();
extern	int	DoGaussianCylinder();
//extern	int	DoFresnelElasticityArea();
extern	int	DoTrefoilKnotII();
extern	int	DoBorromeanRingsSurface();
extern	int	DoPiriformTorusI();
extern	int	DoPiriformTorusII();
extern	int	DoBicornTorusI();
extern	int	DoBicornTorusII();
extern	int	DoCardioidTorusI();
extern	int	DoCardioidTorusII();
extern	int	DoCassinianOvalTorusI();
extern	int	DoCassinianOvalTorusII();
extern	int	DoGeronoLemniscateTorusI();
extern	int	DoGeronoLemniscateTorusII();
extern	int	DoBowCurve();
extern	int	DoWaveyTorusI();
extern	int	DoWaveyTorusII();
extern	int	DoSpiralTorus();
extern	int	DoTorusNodes();
extern	int	DoBreatherSurface();
extern	int	DoTripleCorkscrewI();
extern	int	DoTripleCorkscrewII();
extern	int	DoTripleCorkscrewIII();
extern	int	DoBicornSurface();
extern	int	DoPiriformSurface();
extern	int	DoKappaSurface();
extern	int	DoBulletNose();
extern	int	DoGearTorus();
extern	int	DoSievertSurface();
extern	int	DoCosineSurfaceII();
extern	int	DoArchimedeanSpiral();
extern	int	DoHyperbolicSpiral();
extern	int	DoFermatSpiral();
extern	int	DoTanhSpiral();
extern	int	DoLogarithmicSpiral();
extern	int	DoDoubleBall();
extern	int	DoSuperTorus();
extern	int	DoRoundedCube();
extern	int	DoTwoTorus();
extern	int	DoKinkyTorus();
extern	int	DoGraysKleinbottle();
extern	int	DoBowTie();
extern	int	DoSuperShape();
extern	int	DoSphericalHarmonics();
extern	int	DoTwistedPlaneSurface();
extern	int	DoTetrahedralEllipse();
extern	int	DoBourMinimalSurface();
extern	int	DoGhostPlane();
extern	int	DoHexagonalDrum();
extern	int	DoPseudocatenoid();
extern	int	DoTractrixBasedMinimalSurface();
extern	int	DoSlippersSurface();
extern	int	DoTwistedTriaxial();
extern	int	DoWhatLowestOrderFractionalOrderChaoticSystems();
extern	int	DoCymbelloidDiatom();
extern	int	DoGomphonemoidDiatom();
extern	int	DoGomphocymbelloidDiatom();
extern	int	DoNaviculoidDiatom();
extern	int	DoAbundantWaveSolutions();
extern	int	DoAsteroidEllipsoid();
extern	int	DoSphericalFrustum();
extern	int	DoBinormalSurface();
extern	int	DoProductSurface();
extern	int	DoCrossedTrough();
extern	int	DoCatalansSurfaceII();
extern	int	DoHornlet();
extern	int	DoClawsonBottle4D();
extern	int	DoSkidanRuledSurface();
extern	int	DoSinewurfel();
extern	int	DoNeoviusSurface();
extern	int	DoTwistedRibbonSurface();
extern	int	DoScherkSurfaceII();
extern	int	DoSpindelSurface();
extern	int	DoFourIntersectingDiscs();
extern	int	DoGoblet();
extern	int	DoEnnepersSurfaceII();
extern	int	DoKaiWingFung2Surface();
extern	int	DoEnnepersSurfaceIII();
extern	int	DoWavyEnnepersSurface();
extern	int	DoRichmondSurfaceIII();
extern	int	DoEvolventConoid();
extern	int	DoCayleySurface();
extern	int	DoKreiselSurfaceII();
extern	int	DoKnitterkugel();
extern	int	DoSchneckeII();
extern	int	DoRibbon();
extern	int	DoBellWave();
extern	int	DoParabolicHummingTop();
extern	int	DoRuledRotorCylindroid();
extern	int	DoVirichCyclicSurface();
extern	int	DoCuspidalButterfly();
extern	int	DoCuspidalBeaks();
extern	int	DoCuspidalEdge();
extern	int	DoVaseAndSpearHead();
extern	int	DoKreiselSurfaceI();
extern	int	DoPseudodevelopableHelicoid();
extern	int	DoToothSurface();
extern	int	DoBallsCylindroid();
extern	int	DoUmbrellaSurface();
extern	int	DoKleinBottleNordstrand();
extern	int	DoRastriginSurface();
extern	int	DoRosenbrockSurface();
extern	int	DoSingularitySurface();
extern	int	DoBanchoffKleinBottle();
extern	int	Do4DKleinBottle();
extern	int	DoLawsonBottle();
extern	int	DoEvolvedTorus();
extern	int	DoImmersionOfTheKleinBottle();
extern	int	DoKleinSurface();
extern	int	DoKnottedSurface();
extern	int	DoSphericalRose();
extern	int	DoTortuousEvolvedToroid();
extern	int	DoKyleHavensSurface();
extern	int	DoFanoPlanes();
extern	int	DoHeartSurfaceI();
extern	int	DoHeartSurfaceII();
extern	int	DoAsteroidEllipsoidII();
extern	int	DoBryantsSurface();
extern	int	DoStaircase();
extern	int	DoContourPlot();
extern	int	DoBalloonParametricEq();
extern	int	DoEllipticParaboloid();
extern	int	DoTwister();
extern	int	DoTwistedCones();
extern	int	DoPoweroid();
extern	int	DoDividedParallelepipoid();
extern	int	DoParametricPlot3D1();
extern	int	DoLinkedTori();



// Knots
extern	int	DoKnot3_1();
extern	int	DoKnot4_1();
extern	int	DoKnot5_1();
extern	int	DoKnot5_2();
extern	int	DoTrefoilKnotA();
extern	int	DoTrefoilKnotB();
extern	int	DoGrannyKnot();
extern	int	DoSquareKnot();
extern	int	DoEightKnot();
extern	int	DoKnot6_1();
extern	int	DoKnot6_2();
extern	int	DoKnot6_3();
extern	int	DoKnot7_1();
extern	int	DoKnot7_2();
extern	int	DoKnot7_3();
extern	int	DoKnot7_4();
extern	int	DoKnot7_5();
extern	int	DoKnot7_6();
extern	int	DoKnot7_7();
extern	int	DoShastrisTrefoil();
extern	int	DoTorusKnot();
extern	int	DoLissajous();
extern	int	DoBorromeanRings();
extern	int	DoMarcusTorusKnot();
extern	int	Do6DKnot7_2();
extern	int	DoCinquefoilKnot();
extern	int	DoPaulBourkesKnot4();
extern	int	DoKnotUniversesI();
extern	int	DoKnotUniversesITempestuous();
extern	int	DoKnotUniversesICosmologyIII();
extern	int	DoFibonacciFourierKnots();
extern	int	DoFourierKnotI();
extern	int	DoFourierKnotIII();
extern	int	DoDecorativeKnotPatternsInnerLoopPattern3();
extern	int	DoDecorativeKnotPatternsInnerLoopPattern6();
extern	int	DoDecorativeKnotPatternsThreeMainLoops();
extern	int	DoDecorativeKnotPatternsFourMainLoops();
extern	int	DoDecorativeKnotPatternsCompoundPattern3();
extern	int	DoDecorativeKnotPatternsInnerLoopPattern5();
extern	int	DoPolynomialKnotsCrossings5();
extern	int	DoPolynomialKnotsCrossings6();
extern	int	DoPolynomialKnotsCrossings7();
extern	int	DoPolynomialKnotsCrossings8();
extern	int	DoPolynomialKnotsCrossings10();
extern	int	DoAnIntroductions2HarmonicKnots();



// Curves
extern	int	DoEpicycloids();
extern	int	DoHypotrochoid();
extern	int	DoCycloids();
extern	int	DoRoseCurve();
extern	int	DoAstroid();
extern	int	DoTricuspoid();
extern	int	DoLemniskate();
extern	int	DoHypocycloids();
extern	int	DoStrophoid();
extern	int	DoCissoid();
extern	int	DoGaussCurve();
extern	int	DoPiriform();
extern	int	DoBicornCurve();
extern	int	DoCassinianOval();
extern	int	DoGears();
extern	int	DoArchimedeanSpiralCurve();
extern	int	DoHyperbolicSpiralCurve();
extern	int	DoFermatSpiralCurve();
extern	int	DoTanhSpiralCurve();
extern	int	DoLogarithmicSpiralCurve();
extern	int	DoChrysanthemumCurve();
extern	int	DoPlateauCurves();
extern	int	DoHyperbola();
extern	int	DoEllipse();
extern	int	DoQuadratrixHippias();
extern	int	DoInvoluteCircle();
extern	int	DoFoliumDescartes();
extern	int	DoLissajousCurves();
extern	int	DoTalbotsCurve();
extern	int	DoTractrix();
extern	int	DoPowerCurves();
extern	int	DoCatenary();
extern	int	DoLituus();
extern	int	DoEquangularSpiral();
extern	int	DoLimaconPascal();
extern	int	DoEvoluteEllipse();
extern	int	DoTrisectrix();
extern	int	DoMalteseCross();
extern	int	DoMordellCurve();
extern	int	DoCruciform();
extern	int	DoSemiCubicParabola();
extern	int	DoSerpentine();
extern	int	DoTschirnhausenCubic();
extern	int	DoConchoidDeSluze();
extern	int	DoBifoliate();
extern	int	DoOphiuride();
extern	int	DoCycloidOfCeva();
extern	int	DoButterflyCurve();
extern	int	DoDevilsCurve();
extern	int	DoEightCurve();
extern	int	DoFishCurve();
extern	int	DoHippopede();
extern	int	DoKampyleOfEudoxus();
extern	int	DoPearShapedCurve();
extern	int	DoSwastikaCurve();
extern	int	DoCayleysSextic();
extern	int	DoCornoid();
extern	int	DoDumbbellCurve();
extern	int	DoFreethsNephroid();
extern	int	DoScarabaeus();
extern	int	DoRanunculoid();
extern	int	DoAbdankAbakanowiczQuadratrix();
extern	int	DoAnguinea();
extern	int	DoBesace();
extern	int	DoTrifolium();
extern	int	DoNodalCurve();
extern	int	DoBrachistochroneCurve();
extern	int	DoBiquarticCarlosSacre();
extern	int	DoDoubleHeartCurveKerner();
extern	int	DoLHospitalQuintic();
extern	int	DoTearDropCurve();
extern	int	DoTorpedoCurve();
extern	int	DoVivianiCurve();
extern	int	DoMarcusCurve();
extern	int	DoBoothsCurve();
extern	int	DoPolygasteroid();
extern	int	DoCochleoid();
extern	int	DoConvictsCurve();
extern	int	DoClelieCurve();
extern	int	DoPoinsotsSpirals();
extern	int	DoSquircle();
extern	int	DoIlluminationCurve();
extern	int	DoLilliputBrobdingnag();
extern	int	DoPedaloftheParabola();
extern	int	DoEpitrochoid();
extern	int	DoRhodoneaCurves();
extern	int	DoTrigonometricCurveII();
extern	int	DoTrigonometricCurveIII();
extern	int	DoTrigonometricCurveIV();
extern	int	DoTrigonometricCurveV();
extern	int	DoTrigonometricCurveVI();
extern	int	DoTrigonometricCurveVII();
extern	int	DoTrigonometricCurveVIII();
extern	int	DoTrigonometricCurveIX();
extern	int	DoTrigonometricCurveX();
extern	int	DoFreethsNephroidIrrational();
extern	int	DoCornoidFamily();
extern	int	DoParametricCurvesI();
extern	int	DoParametricCurvesII();
extern	int	DoParametricCurvesIII();
extern	int	DoParametricCurvesQT();
extern	int	DoParametricCurvesQT1();
extern	int	DoParametricCurvesQT2();
extern	int	DoParametricCurvesGeneral();
extern	int	DoAmpersandCurve();
extern	int	DoBeanCurve();
extern	int	DoMarcusButterfly1();
extern	int	DoMarcusButterfly2();
extern	int	DoMarcusButterfly3();
extern	int	DoMarcusButterfly4();


extern	INT_PTR CALLBACK  ImageSizeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  RTJuliaLocDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  PlotTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  FractTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  FractalDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK  FractalTestDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK  ColourDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  EditPalDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  CubicTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  ExpTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  PowerTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  LSystemDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  NewtonGeneral (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  SpecNewton (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  SelectFractal (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  SelectIFS (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  RatTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  NewtonTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  HenonTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  PlasmaDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  FrothDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  CellularDlg (HWND, UINT, WPARAM, LPARAM) ;
//extern	INT_PTR CALLBACK  LyapunovDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  BifTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  FibTypeDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  WalkDlg (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK  Param3D (HWND, UINT, WPARAM, LPARAM) ;
extern	INT_PTR CALLBACK 	JuliaDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK 	InsideDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK 	SaveColFileOpenDlg (HWND, LPSTR, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	SaveMAPFileOpenDlg (HWND, LPSTR, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	ColFileOpenDlg (HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	MAPFileOpenDlg (HWND, LPSTR, LPSTR);
//extern				GetParamData(HWND, LPSTR, LPSTR, LPSTR);
extern	INT_PTR CALLBACK	AnimationDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	JuliaAnimDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	AnimStartDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	CoordDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	ParamDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	CreateDirDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	SpecialAnimDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	FourierTypeDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	FastFourierDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	FourierAnimDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	LyapDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	MountainDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	SelectFracParams(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	PertDlg(HWND, UINT, WPARAM, LPARAM);
//extern	INT_PTR CALLBACK		SelectPertParamDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	SelectOscParams (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	ScrnFormDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	MalthusDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	TriangleDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	GeometryDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	CircleDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	NumTriangleDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	ApolloniusDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	SelectOscillatorDlg (HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	HailstoneDlg(HWND, UINT, WPARAM, LPARAM);
/*
extern	int init_Marcus();			// generic Marcus fractal
extern	int do_Marcus();
*/
extern	int init_Tierazon();		// generic Tierazon fractal
extern	int do_Tierazon();
/*
extern	int init_MandelDeriv();		// generic Mandelbrot Derivatives fractal
extern	int do_MandelDeriv();
*/
//extern	int init_Oscillator();		// generic Oscillator fractal
extern	int DoOscillator();
extern	int DoFractalMaps();
extern	int DoSprottMaps();
extern	int DoSurface();
extern	int DoKnots();
extern	int DoCurves();



