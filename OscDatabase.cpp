/*
	This module consists of the OscillatorSpecific structure
	and a *slew* of defines needed to get it to compile
*/

/* includes needed for fractalspecific */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include ".\parser\fractint.h"
//#include ".\parser\mpmath.h"
//#include "helpdefs.h"
#include "fractype.h"
#include "fractalp.h"

/* empty string */
static char ES[] = "";

//////////////////////////////////////////////////////////////////
// Database of Chaotic Oscillators
//////////////////////////////////////////////////////////////////

struct OscillatorSpecificStuff OscillatorSpecific[]=
    {
	{
	"2D and 3D Solvable Maps", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.4, -2.1, 4.8, 3, 5, 2, 0, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, Do2Dand3DSolvableMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.6, 1.0
	},
	{
	"3D Tinkerbell", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.9, -0.6115, 2.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.5, 2.0, 1, 2, 3, 4, 3, 2, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoModifiedTinkerbell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.8, 1.0
	},
	{
	"A 4D Chaotic System with 1st Lyapunov Exponent of 22, Hyperbolic Curve & Circular Paraboloid Equilibria", 
	    "a", "b", "c", "d", "p", ES, ES, ES, ES, ES, 40.0, 105.0, 0.152, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 55.0, -0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, 2.9, 4.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, Do4DChaoticSystem1stLyapunovExponent22, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 0.1, 1.0
	},
	{
	"A 3D Autonomous Continuous System with Two Isolated Chaotic Attractors and Its Topological Horseshoes", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 4.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -15.0, 30.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoA3DAutonomousContinuousSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"A 3D Jerk System, Its Bifurcation Analysis, Electronic Circuit Design and a Cryptographic Application", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES,
	    1.2, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.3, -1.8, 3.6, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoA3DJerkSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"A 4D Chaotic Hyperjerk System, Sync, Circuit Design & Apps in RNG, Encryption & Chaos Steganography", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    2.0, 4.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, -0.5, 0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -15.0, 25.0, 1, 3, 2, 3, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoA4DChaoticHyperjerkSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"A 3D Strange Attractor with a Distinctive Silhouette The Butterfly Effect Revisited", "alpha", "beta", "gamma", "mu", ES, ES, ES, ES, ES, ES, 
	    3.0, 2.2, 1.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoA3DStrangeAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 12.0, 1.0
	},
	{
	"A 4 Dimensional Hyperchaotic Finance System and Its Control Problems I", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 
	    0.9, 0.2, 1.5, 0.2, 0.17, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.001, -150.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 5, 4, 5, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoA4DHyperchaoticFinanceSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"A 4 Dimensional Hyperchaotic Finance System and Its Control Problems II", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 
	    0.9, 0.2, 1.5, 0.2, 0.17, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 2.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.5, 3.0, 2, 5, 4, 5, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoA4DHyperchaoticFinanceSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.6, 1.0
	},
	{
	"A 4 Dimensional Hyperchaotic Memristive Dynamical System-II", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 
	    0.46, 3.0, 0.1, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -4.5, 0.1, -5.7, 7.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -7.0, 9.0, 4, 7, 2, 6, 4, 4, NULL, 1, YAXIS, 900000.0,
	    NullSetup, DoA4DHyperchaoticMemristiveDynamicalSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.1, 1.0
	},
	{
	"A 4 Dimensional Memristive Chaotic System with Different Families of Hidden Attractors", "alpha", "beta", "gamma", "delta", "eta", "epsilon", ES, ES, ES, ES, 
	    1.0, 1.0, 7.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1400.0, -1500.0, 3000.0, 3, 5, 1, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoA4DMemristiveChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.04, 1.0
	},
	{
	"A 5 Dimensional Hyperchaotic and Chaotic Systems with Non-hyperbolic Equilibria and Many Equilibria", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    10.0, 45.0, 0.0183, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.001, 0.002, 0.003, 0.001, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.65, 2.4, 4, 5, 9, 3, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoA5DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.2, 1.0
	},
	{
	"A 5-D Hyperchaotic Rikitake Dynamo System with Hidden Attractors", "a", "b", "c", "p", "q", ES, ES, ES, ES, ES,
	    1.0, 1.0, 0.7, 1.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -1.3, 5.8, 1, 3, 2, 5, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoA5DHyperchaoticRikitakeDynamoSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"A 9 D-Model in Scholarpaedia", "a", "r", "s", ES, ES, ES, ES, ES, ES, ES, 0.212, 43.3, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "r", "s", "t", ES, 10.0, -12.0, 32.0, -24.0, 25.0, -16.0, 17.0, -9.0, 10.0, 0.0,
	    -476.0, -344.0, 486.0, 2, 3, 4, 3, 9, 9, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoA9DModelScholarpaedia, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000054, 0.4, 1.0
	},
	{
	"A Chaotic System with Infinite Equilibria and Its S-Box Constructing Application", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.5, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.2, -3.2, 6.4, 3, 5, 1, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAChaoticSystemInfiniteEquilibriaSBox, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"A Chaotic System with an Infinite No. of Equilibrium Points on a Line & a Hyperbola & its Fract-Order Form", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.15, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.47, -0.27, 0.53, 4, 5, 6, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAChaoticSystemInfiniteNumberEquilibriumPoints, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000875, 1.0, 1.0
	},
	{
	"A Chaotic System with Infinite Equilibria Located on a Piecewise Linear Curve", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.3, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 2.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.2, -0.4, 1.5, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoAChaoticSystemInfiniteEquilibria, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"A Conditional Symmetric Memristive System With Infinitely Many Chaotic Attractors", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.5, 1.4, 3.0, 6.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.8, 3.1, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAConditionalSymmetricMemristiveSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.5, 0.2
	},
	{
	"A Conditional Symmetric Memristive System With Infinitely Many Chaotic Attractors - Multiplus", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.45, 1.54, 3.0, 5.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.11, -12.0, 23.8, 1, 3, 2, 4, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAConditionalSymmetricMemristiveSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 2.0, 1.0
	},
	{
	"A Conservative Memristive Chaotic System with Extreme Multistability & Its Application in Image Encryption", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.18, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.1, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.56, -1.87, 3.88, 1, 3, 2, 3, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoAConservativeMemristiveChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"A Conservative System based on a Triangular Wave Memristor and its Application in Image Encryption", "n", "a", "b", "en", "q", "start", ES, ES, ES, ES, 3.0, 5.0, 5.0, 0.068, 1.005, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.2, -0.2, 0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.9, 2.0, 2, 3, 4, 6, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAConservativeSystemTriangularWaveMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0004, 4.0, 1.0
	},
	{
	"A Family of Megastable Chaotic Oscillators for the Detection of a Feeble Signal using Chaotic SCA Algorithm", "b", "w", "w0", "a", ES, ES, ES, ES, ES, ES, 0.3, 1.5, 10.0, 1.8756, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0001, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -95.0, -50.0, 100.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAFamilyCirculantMegastableChaoticOscillators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 1.0, 1.0
	},
	{
	"A Fish Biology Chaotic System and its Circuit Design", "r1", "r2", "c12", "c21", "a13", "a23", "d1", "d2", "t1", "t2", 0.41, 0.21, 0.001, 0.001, 0.02, 0.01, 0.1, 0.1, 0.02, 0.02,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -0.7, 4.8, 5, 6, 4, 10, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAFishBiologyChaoticSystemCircuitDesign, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.05, 0.1
	},
	{
	"A Flexible Chaotic System with Adjustable Amp, Max Lyapunov Exponent & Local Kaplan-Yorke Dimension", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 5.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.19, 0.29, -0.34, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAFlexibleChaoticSystemAdjustableAmplitude, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.5, 1.0
	},
	{
	"A Giga-Stable Oscillator with Hidden and Self-Excited Attractors: A Megastable Oscillator Forced by His Twin", "a", "k", "w", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.33, 2.77, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 10.0, 10.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -25.0, 50.0, 1, 3, 2, 3, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAGigaStableOscillatorHiddenSelfExcitedAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 5.0, 1.0
	},
	{
	"A Memristive Chaotic Oscillator with Controllable Amplitude and Frequency", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.58, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, 0.0, 8.0, 3, 4, 2, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAMemristiveChaoticOscillatorControllableAmplitudeFrequency, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0075, 2.0, 1.0
	},
	{
	"A Memristive Chaotic System with Hypermultistability and its Application in Image Encryption", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.3, 0.0, 3.8, 3, 4, 2, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAMemristiveChaoticystemHypermultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"A Memristor-Meminductor-Based Chaotic System with Abundant Dynamical Behaviours", "a", "b", "c", "d", "e", "alpha", "beta", ES, ES, ES, 0.0002, 0.5, 0.5, 10.0, 4.0, 0.1, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, -0.5, -0.5, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -2.4, 6.8, 1, 5, 4, 7, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoAMemristorMeminductorBasedChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 0.5
	},
	{
	"A Minimal 3-Term Chaotic Flow with Coexisting Routes to Chaos, Multiple Solutions, and Circuit Realisation", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 2.088, 4.0, 0.025, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.088, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-12.0, -6.0, 10.0, 1, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	NullSetup, DoAMinimal3TermChaoticFlowCoexistingRoutesChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.02, 1.0
	},
	{
	"A Multi-Wing Spherical Chaotic System using Fractal Process", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 14.0, 43.0, -1.0, 16.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-28.0, -16.0, 32.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoAMultiWingSphericalChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 0.4
	},
	{
	"A New 3D Chaotic System with 4 Quadratic Nonlinear Terms, Global Passive Control Method & Cct Design", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.2, 3.0, 5.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-30.0, -18.0, 36.0, 1, 4, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoANew3DChaoticSystem4QuadraticNonlinearTerms, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 0.06, 0.7
	},
	{
	"A New 3D Chaotic System with Conch-Shaped Equilibrium Curve and its Circuit Implementation", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.001, 0.01, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.5, 3.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoANew3DChaoticSystemConchShapedEquilibriumCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00354, 1.0, 3.0
	},
	{
	"A New 3D Chaotic Flow with One Stable Equilibrium: Dynamical Properties and Complexity Analysis", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.424, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.95, -3.91, 3.17, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.8, -3.3, 4.0, 2, 6, 1, 1, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoANew3DChaoticFlowOneStableEquilibrium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.05, 1.0
	},
	{
	"A New 3D Chaotic System with a Hidden Attractor, Circuit Design and Application in Wireless Mobile Robot", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.1, 0.05, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -35.0, 50.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANew3DChaoticSystemHiddenAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"A New 3D Chaotic System with a Hidden Attractor, Circuit Design and Application Now Extended to 6D", "a", "b", "c", "d", "l", ES, ES, ES, ES, ES, 0.1, 0.05, 0.1, 1.0, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -12.0, 20.0, 5, 4, 1, 5, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANew3DChaoticSystemHiddenAttractor6D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.2
	},
	{
	"A New 3D Chaotic System Without Equilibrium Points, Its Dynamical Analyses & Electronic Circuit Application", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.424, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -7.0, 12.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANew3DChaoticSystemWithoutEquilibriumPoints, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.4, 1.0
	},
	{
	"A New 3D Memristive Time-Delay Chaotic System with Multi-scroll and Hidden Attractors", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.15, 1.9, 0.5, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 5.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -7.0, 12.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoANew3DCMemristiveTimeDelayChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.5, 1.0
	},
	{
	"A New 4D Chaotic Attractor Based on Two-Degrees of Freedom Nonlinear Mechanical System", 
	    "a", "b", "c", "s", "d", "l", ES, ES, ES, ES, 1.5, 10.0, 20.0, 1.25, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.65, -0.32, 0.54, 1, 7, 2, 6, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANew4DChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"A New 4D Chaotic System with Hidden Attractor and its Engineering Applications: Analogue Circuit Design", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, -0.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.45, -1.2, 3.31, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -2.9, 10.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANew4DChaoticSystemHiddenAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A New 4D Chaotic System with Hidden Attractor and its Circuit Implementation", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 4.0, 40.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.3, 0.3, 0.3, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -156.0, -44.0, 167.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANew4DChaoticSystemHiddenAttractorCircuitImplementation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"A New 4D Hyperchaotic System with Four-Scroll Hidden Attractor, Its Properties and Bifurcation Analysis", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.16, 29.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -350.0, -180.0, 375.0, 5, 8, 6, 3, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANew4DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 5.0, 1.0
	},
	{
	"A New 5D Hyperchaotic System with Stable Equilibrium, Transient Chaotic Behaviour & Fractional Form", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 35.0, 30.0, 17.0, 0.78, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -20.0, 40.0, 4, 5, 2, 5, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANew5DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.25, 1.0
	},
	{
	"A new Chaotic Oscillator with Free Control", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 8.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.4, -0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -1.0, 1.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewChaoticOscillatorWithFreeControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.8, 1.0
	},
	{
	"A New Chaotic Image Encryption Scheme Using Breadth-First Search and Dynamic Diffusion", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 7.0, 8.0 / 1.22, 25.0, 2.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.01, 0.95, 1.01, 1.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -58.0, -34.0, 65.0, 1, 2, 3, 4, 4, 4, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoANewChaoticImageEncryptionScheme, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0185, 1.0, 1.0
	},
	{
	"A New Chaotic System with Self-Excited Attractor-Entropy, Signal Encryption & Parameter Estimation", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    4.0, 1.0, 1.0, 1.0, 1.0, 4.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.8, -1.5, -2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -1.7, 3.4, 2, 3, 1, 7, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoANewChaoticSystemSelfExcitedAttractorEntropyMeasurement, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.3, 1.0
	},
	{
	"A New Chaotic System with Multiple Attractors: Dynamic Analysis, Circuit Realisation and S-Box Design", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 
	    4.0, 9.0, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -50.0, 100.0, 3, 6, 1, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewChaoticSystemMultipleAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 1.0
	},
	{
	"A New Chaotic System with Multiple Attractors: Dynamic Analysis, Circuit Realization and S-Box Design-B", "n", "s22", ES, ES, ES, ES, ES, ES, ES, ES, 
	    10.0, 1.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.02, -0.2, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -9.0, 18.0, 1, 6, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANewChaoticSystemMultipleAttractorsB, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A New Chaotic System with Line of Equilibria: Dynamics, Passive Control and Circuit Design", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES,
	    1.6, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -44.0, -26.0, 52.0, 4, 5, 6, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewChaoticSystemMultipleLineEquilibria, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A New Colour Image Encryption based on Multi Chaotic Maps", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES,
	    36.0, 3.0, 28.0, 16.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.3, -0.4, 1.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -64.0, -12.0, 62.0, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewColourimageEncryption, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.6
	},
	{
	"A New Cost Function for Estimation of Chaotic Systems Using Return Maps as Fingerprints - Jafari oscillator", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -1.6, 10.0, 4, 6, 5, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewCostFunctionParameterEstimationChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 0.4, 0.4
	},
	{
	"A New Family of Chaotic Systems with Different Closed Curve Equilibrium", "a", "b", "k", ES, ES, ES, ES, ES, ES, ES,
	    5.0, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -0.72, 1.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoANewFamilyChaoticSystemsDifferentClosedCurveEquilibrium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"A New Finance Chaotic System, its Electronic Circuit Realisation & an Application to Voice Encryption", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    8.1, 0.1, 1.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, 1.4, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, 7.0, 3.0, 1, 2, 3, 4, 3, 3, NULL, 1+AVERAGE, YAXIS, 100000.0,
	    NullSetup, DoANewFinanceChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"A New Four-Scroll Chaotic System with a Self-Excited Attractor and Circuit Implementation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	    3.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -130.0, -80.0, 160.0, 4, 5, 6, 3, 3, 3, NULL, 1 + AVERAGE, YAXIS, 200000.0,
	    NullSetup, DoANewFourScrollChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.8, 1.0
	},
	{
	"A New Fractional Order Chaotic System with Different Families of Hidden and Self-Excited Attractors", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.35, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -33.0, 55.0, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewFractionalOrderChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.15, 0.1
	},
	{
	"A New Hamiltonian Chaotic System with Coexisting Chaotic Orbits and its Dynamical Analysis", "a", "b", "begin", ES, ES, ES, ES, ES, ES, ES,
	    1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.001, -0.2, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.37, -0.24, 0.47, 1, 4, 3, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANewHamiltonianChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"A New Hyperchaotic Map and its Application for Image Encryption", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    3.0, 0.45, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.2, 2.5, 1, 4, 2, 2, 2, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewHyperchaoticMapOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 1.0
	},
	{
	"A New Hyperchaotic System-Based Design for Efficient Bijective Substitution-Boxes", "c1", "c2", "c3", "c4", "c5", "c6", "c7", ES, ES, ES,
	30.0, 10.0, 15.7, 5.0, 2.5, 4.45, 38.5, 0.0, 0.0, 0.0,
	"x", "y", "z", "u", "v", ES, ES, ES, ES, ES, -2.0, -2.5, 4.0, -7.0, -60.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-42.0, -20.0, 50.0, 2, 3, 1, 7, 5, 5, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoANewHyperchaoticSystemBasedDesign, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00006, 1.0, 1.0
	},
	{
	"A New Hyperchaotic Two-Scroll System: Bifurcation, Multistability, Circuit Simulation, & FPGA Realisation", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES,
	    6.0, 8.0, 90.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.8, 0.2, 0.4, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -108.0, -53.0, 112.0, 1, 2, 3, 4, 4, 4, NULL, 1 + AVERAGE, YAXIS, 100000.0,
	    NullSetup, DoANewHyperchaotic2ScrollSystemBifurcationStudy, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"A New Image Encryption Scheme based on Fractional Order Hyperchaotic System & Multiple Image Fusion", "h", "c", "e", "g", "n", "p", "s", "m1", "m2", "start",
	    20.0, 150.0 / 7.0, 15.0, 0.15, 2.0, 0.05, 0.1, 0.1, 1.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -2.8, 5.8, 1, 4, 3, 9, 5, 5, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoANewImageEncryptionScheme, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"A New Memristor-Based 5D Chaotic System and Circuit Implementation", "k", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-200.0, -120.0, 230.0, 1, 5, 3, 1, 5, 5, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoANewMemristorBased5DChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00051, 6.0, 1.0
	},
	{
	"A New Multi-Wing Chaotic Attractor with Unusual Variation in the Number of Wings", "a", "b", "c", "k", "k11", "k22", ES, ES, ES, ES,
	2.0, 1.0, 1.0, 0.4, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-10.5, -3.35, 11.8, 2, 3, 1, 6, 3, 3, NULL, 1, YAXIS, 150000.0,
	NullSetup, DoANewMultiWingChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00075, 1.0, 1.0
	},
	{
	"A New No-Equilibrium Chaotic System and Its Topological Horseshoe Chaos", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES,
	2.0, 5.0, 10.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-18.0, -5.0, 20.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 10000.0,
	NullSetup, DoANewNoEquilibriumChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.016, 0.6, 1.0
	},
	{
	"A New No Equilibrium Fractional Order Chaotic System, Synchronisation, and Its Digital Implementation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	0.5, 1.5, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-4.1, -2.38, 4.7, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoANewNoEquilibriumFractionalOrderChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.2
	},
	{
	"A New Nonlinear Chaotic Complex Model and Its Complex Antilag Synchronisation", "r", "s", "m", ES, ES, ES, ES, ES, ES, ES, 
	    21.0, 10.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, -7.75, -1.0, -0.1, -1.0, 12.0, -1.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 1, 7, 2, 3, 6, 6, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoANewNonlinearChaoticComplexModels, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"A New Oscillator with Infinite Coexisting Asymmetric Attractors", "a", "w", "real", "imag", ES, ES, ES, ES, ES, ES, 
	    0.8, 0.7, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 2, 5, 1, 4, 3, 2, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoANewOscillatorInfiniteCoexistingAsymmetricAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.225, 1.0, 1.0
	},
	{
	"A New Transiently Chaotic Flow with Ellipsoid Equilibria", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -2.15, -0.83, -4.29, -0.11, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.2, -1.4, 2.0, 1, 4, 3, 1, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewTransientlyChaoticFlow, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.6, 1.0
	},
	{
	"A New Two-Scroll Chaotic System with Two Nonlinearities: Dynamical Analysis and Circuit Simulation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 6.0, 1.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -2.15, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -75.0, -45.0, 90.0, 4, 5, 6, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANewTwoScrollChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.4, 0.1
	},
	{
	"A No-Equilibrium Hyperchaotic System and Its Fractional-Order Form", "a", "n", "b", "m", "c", "d", ES, ES, ES, ES, 1.0, 1.0, 0.5, 0.5, 0.2, 2.5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 3, 4, 6, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoANoEquilibriumHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"A No-Equilibrium Hyperchaotic System with a Cubic Nonlinear Term", "a", "b", "c", "d", "m", "n", ES, ES, ES, ES, 10.0, 2.0, 28.0, 0.1, 27.0, 0.5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -55.0, 140.0, 3, 8, 4, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANoEquilibriumHyperchaoticSystemCubicNonlinearTerm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0052, 0.01, 1.0
	},
	{
	"A Novel 3D Fractional-order Chaotic System with Multifarious Coexisting Attractors", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 5.0, 1.0, 8.0, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -1.5, 16.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANovel3DFractionalOrderChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 3.0, 1.0
	},
	{
	"A Novel 4D No-Equilibrium Hyper-Chaotic System with Grid Multi-wing Hyper-chaotic Hidden Attractors I", 
	    "a", "b", "c", "d", "n", "m", "azao", ES, ES, ES, 8.0, 4.0, 1.0, 0.01, 2.0, 4.0, 0.4, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 1, 2, 4, 7, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoANovel4DNoEquilibriumHyperChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.25, 0.1
	},
	{
	"A Novel 4D No-Equilibrium Hyper-Chaotic System with Grid Multi-wing Hyper-chaotic Hidden Attractors II", 
	    "a", "b", "c", "d", "n", "m", "azao", ES, ES, ES, 8.0, 4.0, 1.0, 0.01, 2.0, 0.0, 0.4, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.8, -3.7, 7.2, 2, 7, 1, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovel4DNoEquilibriumHyperChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.08, 1.0
	},
	{
	"A Novel Autonomous 5D Hyperjerk RC Circuit with Hyperbolic Sine Function",
	    "a0", "a1", "a2", "a3", "b", "gamma", ES, ES, ES, ES, 1.75, 3.0, 2.5, 1.0, 3.0, 0.011, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 3, 5, 4, 6, 5, 5, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoANovelAutonomous5DHyperjerkRCCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"A Novel Chaos Application to Observe Performance of Asynchronous Machine Under Chaotic Load",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -128.0, -12.6, 156.0, 3, 4, 2, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANovelChaosApplication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 2.5, 1.0
	},
	{
	"A Novel Chaotic System with Application to Secure Communications",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.4, -4.0, 6.7, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANovelChaoticSystemApplicationSecureCommunications, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A Novel Chaotic System with 2 Circles of Equilibrium Points: Multistability, Electronic Cct & FPGA Realisation",
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 4.0, 4.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.85, -0.75, 1.6, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelChaoticSystem2Circles, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A Novel Class of Chaotic Flows & Their Application in Chaos-Based Comms Design Using DCSK - CE-2", 
	    "a1", "a2", "a3", "b", "c", ES, ES, ES, ES, ES, -1.0, 0.01, 0.1, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8, -0.9, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.25, 0.56, 2, 6, 1, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelClassChaoticFlowsInfiniteEquilibria2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 0.2
	},
	{
	"A Novel Class of Chaotic Flows & Their Application in Chaos-Based Comms Design Using DCSK - CE-4", 
	    "a1", "a2", "a3", "a4", "b", "c", ES, ES, ES, ES, -2.0, 0.1, 1.0, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.23, 3.89, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 6, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelClassChaoticFlowsInfiniteEquilibria4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.01, 0.1
	},
	{
	"A Novel Class of Chaotic Systems and Microcontroller-Based Cost-Effective Design for Digital Apps", 
	    "a", "b", "c",  ES, ES, ES, ES, ES, ES, ES, 0.2, 1.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.5, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANovelClassChaoticSystemsDifferentShapesEquilibrium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001266, 0.1, 0.1
	},
	{
	"A Novel Control Method to Counteract the Dynamical Degradation of a Digital Chaotic Sequence", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 5.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 6.0, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    5.0, -5.0, 20.0, 1, 4, 2, 2, 3, 3, NULL, 1+AVERAGE, XAXIS, 100000.0,
	    NullSetup, DoANovelControlMethodCounteractDynamicalDegradation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 40.0, 1.0
	},
	{
	"A Novel 4 Dimensional Hyper-chaotic Coupled Dynamos System & Its Tracking Control & Synchronisation", "n", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -12.5, 25.0, 3, 5, 4, 1, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelFourDimensionalHyperchaoticCoupledDynamos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"A Novel Four-Dimensional Hyperchaotic System", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 55.0, 25.0, 40.0, 13.0, 23.0, 8.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -25.0, -50.0, 26.0, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -215.0, -158.0, 172.0, 1, 4, 3, 6, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoANovelFourDimensionalHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00015, 0.5, 0.5
	},
	{
	"A Novel 4 Wing Hyperchaotic Complex System and Its Complex Modified Hybrid Projective Synchronisation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 40.0, 14.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.9, 0.3, 0.2, 1.7, 0.23, 0.24, 0.0, 0.0, 0.0, 0.0,
	    -270.0, -150.0, 300.0, 3, 5, 4, 3, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelFourWingHyperchaoticComplexSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 5.0
	},
	{
	"A Novel Digital Programmable Multi-Scroll Chaotic System & its Application in FPGA Audio Secure Comms", 
	    "a", "b", "L (multiple of -8)", "U (multiple of 8)", ES, ES, ES, ES, ES, ES, 1.0, 0.6, -24.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -75.0, -45.0, 90.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1500000.0,
	    NullSetup, DoANovelDigitalProgrammableMultiScrollChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.006, 1.0, 1.0
	},
	{
	"A Novel Hyperchaotic System with Infinitely Many Heteroclinic Orbits Coined", 
	    "a1", "d1", "e1", "c2", "d2", "e2", "b", "a1", "a2", ES, 4.0, -1.0, -1.0, 2.0, -1.0, 2.0, 0.0, 6.0, -1.0, 0.0,
	    "x", "y", "z", "w", "u", "v", ES, ES, ES, ES, 1.6181 / 100.0, 3.14 / 100.0, 2.718 / 100.0, 4.66921 / 100.0, 0.618 / 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -13.0, 26.0, 6, 7, 8, 9, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.15, 0.15
	},
	{
	"A Novel Grid Multiwing Chaotic System with Only Non-Hyperbolic Equilibria", 
	    "a", "b", "c", "n", "m", "azao", ES, ES, ES, ES, 2.0, 1.0, 8.0, 2.0, 2.0, 0.5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 4, 3, 6, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoANovelGridMultiwingChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 0.1
	},
	{
	"A Novel Megastable Hamiltonian System with Infinite Hyperbolic and Nonhyperbolic Equilibria",
	    "a", "b", "k", "w", "azao", "m", ES, ES, ES, ES, sqrt(2.0)/2.0, 0.05, 2.0, 0.7, 1.58, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 20.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -10.0, 20.0, 4, 5, 6, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelMegastableHamiltonianSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A Novel Megastable Oscillator with a Structure of Coexisting Attractors: Design, Analysis, & FPGA Implementation",
	    "w", "r", ES, ES, ES, ES, ES, ES, ES, ES, PI / 0.5555, 0.05, 5*PI, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.38, -0.2, 0.4, 4, 5, 6, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoANovelMegastableOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 1.0, 1.0
	},
	{
	"A Novel Memcapacitor Model and Its Application for Generating Chaos", "m", "n", "j", "k", "a", "b", "b1", "b2", ES, ES, 7.35, 0.17, 4.8, 2.1, 0.01, 0.1, 0.7, -2.5, 0.0, 0.0,
	    "x", "y", "z", "w", "v", ES, ES, ES, ES, ES, 0.019, 0.1, 0.007, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.07, -0.04, 0.08, 1, 6, 3, 8, 5, 5, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoANovelMemcapacitorModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.5, 1.0
	},
	{
	"A Novel Memductor-Based Chaotic System and Its Applications in Circuit Design and Experimental Validation", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 4, 5, 3, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelMemductorBasedChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.15, 1.0
	},
	{
	"A Novel Method for Constructing Grid Multi-Wing Butterfly Chaotic Attractors via Nonlinear Coupling Control", 
	    "a", "b", "c", "h", "a1", "a2", "a3", "a4", "a5", "a6", 10.0, 8.0/3.0, 28.0, 5.0, 14.38, 10.0, 19.48, 27.83, 9.2, 2.67,
	    "x", "y", "z", "w", "v", ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 4, 6, 3, 10, 5, 5, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoANovelMethodConstructingGridMultiWingButterfly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.016, 0.2, 3.0
	},
	{
	"A Novel Multi-Attractor Period Multi-Scroll Chaotic Integrated Circuit Based on CMOS Wide Adjustable CCCII",
	    "a", "b", "c", "d", "e", "h", "m", ES, ES, ES, 2.0, 1.0, 1.0, 1.5, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.72, -0.42, 0.84, 1, 2, 3, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelMultiAttractorPeriodMultiScroll, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"A Novel No-Equilibrium Chaotic System with Multiwing Butterfly Attractors", 
	    "azinho", "n", "a", "k", "c", ES, ES, ES, ES, ES, 1.0, 3.0, 2.3, 2.25, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -2.0, 6.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoANovelNoEquilibriumChaoticMultiWingButterfly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 1.0, 0.01
	},
	{
	"A Novel No-Equilibrium HR Neuron Model with hidden Homogeneous Extreme Multistability",
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 1.0, 3.0, 1.0, 5.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 3*PI, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.46, -1.83, 2.38, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoANovelNoEquilibriumHRNeuronModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.1, 0.1
	},
	{
	"A Novel No-Equilibrium Hyperchaotic Multi-Wing System Via Introducing Memristor", 
	    "alpha", "beta", "a", "n=(0 to 4)", "p", "k", ES, ES, ES, ES, 2.0, 1.0, 4.0, 2.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -4.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 30000.0,
	    NullSetup, DoANovelNoEquilibriumHyperchaoticMultiWingSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 0.2
	},
	{
	"A Novel Plaintext-Related Image Encryption Algorithm Based on Stochastic Sig Insertion & Block Swapping",
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -11.0, 25.0, 4, 5, 3, 0, 5, 5, NULL, 1, YAXIS, 32000.0,
	    NullSetup, DoANovelPlaintextRelatedImageEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000151, 0.2, 0.5
	},
	{
	"A Plethora of Coexisting Strange Attractors in a Simple Jerk System with Hyperbolic Tangent Nonlinearity", 
	    "a", "g", "mu", ES, ES, ES, ES, ES, ES, ES, 6.5, 1.08, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.61, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoAPlethoraCoexistingStrangeAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 1.0, 0.7
	},
	{
	"A Semi-Symmetric Image Encryption Scheme based on the Projective Sync of Two Hyperchaotic Systems",
	    "b", "w01", "w02", "w03", ES, ES, ES, ES, ES, ES, 0.28, 0.5, 0.2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -5.1, -17.5, 21.0, 2, 6, 5, 4, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoASemiSymmetricImageEncryptionScheme, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"A Simple Chaotic Circuit with a Hyperbolic Sine Function and its Use In a Sound Encryption Scheme", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.0003, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -18.0, 36.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoASimpleChaoticCircuitWithHyperbolicSineFunction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"A Simple Chaotic Oscillator for Educational Purposes", 
	    "a", "b", "e", "c", ES, ES, ES, ES, ES, ES, 0.4, 30.0, 0.13, 4.0E-9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -115.0, -60.0, 120.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoASimpleChaoticOscillator4EducationalPurposes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"A Simple Chaotic System with Topologically Different Attractors",
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.12, 0.115, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -2.0, 3.4, 4, 6, 5, 4, 3, 3, NULL, 1, YAXIS, 1500000.0,
	    NullSetup, DoASimpleChaoticSystemWithTopologicallyDifferentAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"A Simple Inductor-Free Memristive Circuit with Three Line Equilibria", 
	    "a", "b", "c", "d", "m", "n", "alpha", "beta", "eta", "xi", 20.0, 40.0, 10.0, 2.0, 2.0, 2.0, -1.0, 0.1, 1.0, -0.1,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.5, 15.0, 3, 4, 2, 10, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoASimpleInductorFreeMemristiveCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 0.3
	},
	{
	"A Simple Memristive Jerk System",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.239, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -2.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.8, 5.2, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 750000.0,
	    NullSetup, DoASimpleMemristiveJerkSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000925, 0.7, 0.3
	},
	{
	"A Simple Snap Oscillator with Coexisting Attractors, Its Time-Delayed Form and Communication Designs", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.5, 1, 8, 3, 3, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoASimpleSnapOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.006, 1.0
	},
	{
	"A Strange Novel Chaotic System with Golden Proportion Equilibria & Mobile Microcomputer-Based RNG I", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 3.0, 11.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 5, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAStrangeNovelChaoticSystemFullyGoldenProportionEquilibria, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.2, 0.3
	},
	{
	"A Strange Novel Chaotic System with Golden Proportion Equilibria & Mobile Microcomputer-Based RNG II", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -(3.0 + sqrt(5.0)) / 2.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.6, -5.0, 10.0, 2, 5, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAStrangeNovelChaoticSystemFullyGoldenProportionEquilibria, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.2, 1.0
	},
	{
	"A Study on Four-Species Fractional Population Competition Dynamical Model",
	    "r", "k", "m1", "m2", "m3", "d1", "d2", "d3", ES, ES, 1.0, 5.0, 5.0, 6.0, 7.0, 4.0, 4.0, 4.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.08, 0.08, 0.06, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.5, 0.9, 5, 6, 7, 8, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAStudyonFourSpeciesFractionalPopulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0004, 10.0, 1.0
	},
	{
	"A True Three-Scroll Chaotic Attractor Coined I",
	    "a", "d", "f", "e", "c", ES, ES, ES, ES, ES, 40.0, 0.5, 20.0, 0.65, 5.0/6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.0, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -111.0, -25.0, 120.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoATrueThreeScrollChaoticAttractorCoined, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 1.0, 1.0
	},
	{
	"A True Three-Scroll Chaotic Attractor Coined II",
	    "a", "d", "f", "e", "c", ES, ES, ES, ES, ES, 168.0, 0.4, 100.0, 0.7, 10.25, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.618, -1.618, 1.618, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -450.0, -130.0, 490.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoATrueThreeScrollChaoticAttractorCoined, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000025, 1.0, 1.0
	},
	{
	"A Unique Signum Switch for Chaos and Hyperchaos",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.395, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.3, -2.5, 5.1, 5, 6, 7, 2, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAUniqueSignumSwitch, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.5, 1.0
	},
	{
	"Abundant Coexisting Multiple Attractors’ Behaviours in Three-Dimensional Sine Chaotic System",
	    "k1", "k2", ES, ES, ES, ES, ES, ES, ES, ES, 3.6, 3.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.00000025, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAbundantCoexistingMultipleAttractorsBehaviors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"ACT Chaotic Attractor", "a", "b", "d", "m", ES, ES, ES, ES, ES, ES, 1.8, -0.07, 1.5, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -52.0, -40.0, 80.0, 3, 2, 1, 4, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoACT, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Active Tracking Control of the Hyperchaotic LC Oscillator System", "a", "b", "c", "d", "mu", "epsilon", ES, ES, ES, ES, 
	    0.6, 0.05, 0.015, 10.0, 0.3, 0.33, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 56.0, 1, 4, 3, 6, 4, 4, NULL, 1, YAXIS, 4000000.0,
	    NullSetup, DoActiveTracking, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00011, 1.0, 1.0
	},
	{
	"Adaptive Hybrid Dislocated Sync for Two Identical & Different Memristor Chaotic Oscillator Systems", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    0.31, 0.35, 0.29, 0.41, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.6, 1, 2, 6, 4, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoAdaptiveHybridDislocatedSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Adaptive Synchronisation of a Hyperchaotic Oscillator", "s", "e1", "e2", "a", "g", "q", ES, ES, ES, ES, 0.65, 3.0, 3.226, 5.249E-5, 5.249E-6, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -1.0, -1.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -62.0, -25.0, 50.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAdaptiveSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Adaptive Synchronisation of a 4D Oscillator I", "sigma", "e1", "e2", "alpha", "gamma", "q", ES, ES, ES, ES, 0.65, 3.0, 3.226, 5.249E-5, 5.249E-6, 5.249E-5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -1.0, -1.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0,  -100.0,  200.0, 2, 3, 4, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAdaptiveSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Adaptive Hybrid Synchronisation of Chaotic Systems for Chaotic Masking", "a", "b", "c", "p", "q", ES, ES, ES, ES, ES, 
	    30.0, 16.5, 10.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.5, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -720.0, -200.0, 700.0, 4, 5, 6, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAdaptiveHybridSynchronisationChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"Adaptive Synchronisation of a 4D Oscillator II", 
	    "e1", "e2", "alpha", "sigma", "phi", ES, ES, ES, ES, ES, 3.0, 3.226, 5.249 * 0.00001, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -0.9, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 2, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTheoreticalHyperchaoticII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Adaptive Synchronisation of a 4D Oscillator III", "sigma", "e1", "e2", "alpha", "gamma", "q", ES, ES, ES, ES, 0.32, 3.0, 3.226, 5.249E-5, 5.249E-6, 5.249E-5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -1.0, -1.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -15.0, 40.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAdaptiveSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Adaptive Synchronisation of a 4D Oscillator IV", "sigma", "e1", "e2", "alpha", "gamma", "q", ES, ES, ES, ES, 0.4, 3.0, 3.226, 5.249E-5, 5.249E-6, 5.249E-5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -1.0, -1.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -40.0, 60.0, 2, 4, 3, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoAdaptiveSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Adaptive Inverse Optimal Control of a Novel Fractional-Order Four-Wing Hyperchaotic System I", 
	    "a", "b", "c", "h", "k", "g", "m", "n", "e", ES, 10.0, 2.5, 1.0, 1.0, 4.0, 4.0, 1.0, 0.125, 1.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -15.0, 30.0, 1, 2, 3, 9, 4, 4, NULL, 0, YAXIS, 2000000.0,
	    NullSetup, DoAdaptiveInverse, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 3.0, 1.0
	},
	{
	"Adaptive Inverse Optimal Control of a Novel Fractional-Order Four-Wing Hyperchaotic System II", 
	    "a", "b", "c", "h", "k", "g", "m", "n", "e", ES, 10.0, 2.5, 1.0, 1.0, 4.0, 4.0, 1.0, 0.125, -1.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 3, 1, 9, 4, 4, NULL, 0, YAXIS, 2000000.0,
	    NullSetup, DoAdaptiveInverse, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.5, 1.0
	},
	{
	"Aizawa Chaotic Attractor", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.95, 0.7, 0.6, 3.5, 0.25, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.6, 4.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAizawa, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Amplitude Death & Sync States in Nonlinear Time-Delay Systems Coupled Through Mean-Field Diffusion", 
	    "n", "m", "L", "a", "b", ES, ES, ES, ES, ES, 1.19, 0.97, 2.19, 1.0, 2.4, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.0, 2.0, 1, 2, 2, 5, 1, 1, NULL, 1, YAXIS+EVOLVE, 5000.0,
	    NullSetup, DoAmplitudeDeath, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
/*
	{
	"Aizawa Chaotic Attractor (Cartesian)", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.95, 0.7, 0.6, 3.5, 0.25, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.6, 4.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAizawa, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Aizawa Chaotic Attractor (Prolate)", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.95, 0.7, 0.6, 3.5, 0.25, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.6, 4.0, 1, 2, 3, 6, 3, 3, NULL, 1+16, YAXIS, 100000.0,
	    NullSetup, DoAizawa, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Aizawa Chaotic Attractor (Oblate)", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.95, 0.7, 0.6, 3.5, 0.25, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.6, 4.0, 1, 2, 3, 6, 3, 3, NULL, 1+18, YAXIS, 100000.0,
	    NullSetup, DoAizawa, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Aizawa Chaotic Attractor (BiSpherical)", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.95, 0.7, 0.6, 3.5, 0.25, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -450.0, -230.0, 520.0, 1, 2, 3, 6, 3, 3, NULL, 1+20, YAXIS, 100000.0,
	    NullSetup, DoAizawa, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Aizawa Chaotic Attractor (Bipolar Cylindrical)", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.95, 0.7, 0.6, 3.5, 0.25, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -690.0, -360.0, 750.0, 1, 2, 3, 6, 3, 3, NULL, 1+22, YAXIS, 100000.0,
	    NullSetup, DoAizawa, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
*/
	{
	"Amplitude-Phase Control of a Novel Chaotic Attractor I", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.5, 3.75, 1.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -247.0, -126.0, 275.0, 5, 6, 4, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAmplitudePhaseControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Amplitude-Phase Control of a Novel Chaotic Attractor II", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.5, 3.75, 1.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.20, 2.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -276.0, -190.0, 340.0, 5, 6, 4, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAmplitudePhaseControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"An Efficient Approach for Fractional Nonlinear Chaotic Model with Mittag-Leffler Law", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.9, -1.5, 2.8, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnEfficientApproachFractionalNonlinearChaoticModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Analysis, Stabilisation, and DSP-Based Implementation of a Chaotic System with Nonhyperbolic Equilibrium", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 1.0, 6.0, 5.0, 1.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.20, 2.0, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -12.0, 29.0, 5, 6, 4, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnalysisStabilisationDSPBasedImplementation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"An Eight-Term Novel Four-Scroll Chaotic System with Cubic Nonlinearity and its Circuit Simulation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 3.0, 14.0, 3.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -84.0, 200.0, 4, 5, 6, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnEightTermNovelFourScrollChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 3.0, 1.0
	},
	{
	"An Image Encryption Algorithm based on Chaotic System and Compressive Sensing", "a", "b", "c", "d", "g", "r", ES, ES, ES, ES, 10.0, 8.0, 15.0, 5.2, 5.0, 1.5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -50.0, 150.0, 4, 7, 6, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnImageEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 0.3
	},
	{
	"An Improved Butterfly Optimisation Algorithm with Chaos -  Chebyshev", "k", "real", "imag", ES, ES, ES, ES, ES, ES, ES, 13.5, 0.003, 0.003, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, -0.02, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.3, 2.6, 1, 3, 2, 3, 2, 2, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoAnImprovedButterflyOptimizationAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 1.0, 1.0, 1.0
	},
	{
	"An Infinite 2D Lattice of Strange Attractors", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.025, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 4, 5, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnInfinite2DLatticeStrangeAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 2.0, 1.0
	},
	{
	"An Infinite 3D Lattice of Strange Attractors", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, (sqrt(5.0) + 1.0) / 2.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -22.5, 60.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAnInfinite3DLatticeStrangeAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"An Introductions to Harmonic Knots - Generic knots - page 359", "p", ES, ES, ES, ES, ES, ES, ES, ES, ES, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.4, 2.8, 4, 5, 6, 1, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoAnIntroductions2HarmonicKnotsOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"An Unforced Megastable Chaotic Oscillator and its Application on Protecting Electrophysiological Signals", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.421, 0.7, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -2.6, 5.3, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnUnforcedMegastableChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 0.1
	},
	{
	"Analysis, Adaptive Control & Sync of a Novel 4D Hyperchaotic Hyperjerk System & its SPICE Implementation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 3.7, 0.2, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 1, 4, 3, 3, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoAnalysisAdaptiveControlSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.2, 1.0
	},
	{
	"Analysis of Hyperchaos Generated by a Semiconductor Laser Subject to a Delayed Feedback Loop", 
	    "b", "real", "imag", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.05, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -0.3, 0.75, 1, 3, 4, 3, 2, 2, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoAnalysisCharacterisationHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 0.2, 1.0
	},
	{
	"Analysis and Implementation of a Floating Memristor Chaotic Circuit", 
	    "alpha", "beta", "c", "d", "a", "b", ES, ES, ES, ES, 3.5, 2.17, 10.0, 0.2, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.2, 4.4, 1, 4, 2, 6, 2, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnalysisImplementationFloatingMemristorChaoticCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Analysis, Control and Circuit Design of a Novel Chaotic System with Line Equilibrium", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 2.8, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.2, 2.6, 1, 4, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnalysisControlCircuitDesign, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.2, 0.4
	},
	{
	"Analysis, Control and FPGA Implementation of a Fractional Order Modified Shinriki Circuit", "a1", "b1", "sigma", "gamma", "alpha", "beta", "eta1", "eta2", "delta", ES,
	    4.0, 0.01, 1.5, 8.143724 * 0.00001, 0.005, 0.17, 5.5, 0.1063829, 0.06, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0010, -1.0, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -20.0, 36.0, 5, 6, 4, 9, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnalysisControlFPGAImplementation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Analysis of a 4-D Hyperchaotic Fractional-Order Memristive System with Hidden Attractors",
	    "b", "c", ES, ES, ES, ES, ES, ES, ES, ES, 5.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 4, 5, 2, 2, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAnalysis4DHyperchaoticFractionalOrderMemristiveSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00069, 0.5, 1.0
	},
	{
	"Analysis of a Chaotic System with Line Equilibrium Applied to Secure Comms Using a Descriptor Observer",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.65, -1.7, 3.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAnalysisChaoticSystemWithLineEquilibrium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Analysis of Chaotic Oscillations Induced in Two Coupled Wilson-Cowan Models", 
	    "te", "ti", "ke", "ki", "c1", "c2", "c3", "c4", "alpha", "beta", 1.0, 1.0, 1.0, 1.0, 16.0, 12.0, 15.0, 3.0, 5.3, -2.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.105, 0.1, 0.1, 0.145, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.5, 0.8, 2, 4, 1, 10, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoAnalysisWilsonCowan, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.3, 1.0
	},
	{
	"Analysis of a New Three-Dimensional System with Multiple Chaotic Attractors", 
	    "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 4.0, 9.0, 1.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -11.0, 20.0, 3, 4, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnalysisNewThreeDimensionalSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.03, 1.0
	},
	{
	"Analysis of a New Three-Dimensional Chaotic System", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 28.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2600.0, -540.0, 3000.0, 4, 6, 5, 3, 3, 3, NULL, 1, YAXIS, 30000.0,
	    NullSetup, DoAnalysisNewThreeDimensionalChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Analysis of a Three-Dimensional Chaotic System and its FPGA Implementation", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -10.0, -4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -640.0, -400.0, 800.0, 5, 6, 4, 3, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoAnalysisThreeDimensionalChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Analysis of Three Types of Initial Offset-Boosting Behaviour for a New Fractional-Order Dynamical System",
	    "a", "b", "c", "d", "e", "g", "k", ES, ES, ES, 7.0, 3.0, 2.0, 1.0, -2.0, 2.0, -2.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.28, 0.0001, 0.0001, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.3, -3.3, 3.8, 3, 4, 2, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnalysisThreeTypesInitialOffsetBoostingBehaviour, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000805, 0.3, 0.5
	},
	{
	"Analysis of Two Time Scale Property of Singularly Perturbed System on Chaotic Attractor-II", 
	    "b1", "b2", "d1", "d2", "e", ES, ES, ES, ES, ES, 0.3, 0.1, 0.1, 0.62, 0.3, 12.0, 15.0, 3.0, 5.3, -2.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.15, -0.13, 0.3, 3, 5, 6, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoAnalysisTwoTimeScaleProperty, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Analysis, Synchronisation, and Robotic Application of a Modified Hyperjerk Chaotic System",
	    "a", "b", "c", "d", "gamma", ES, ES, ES, ES, ES, 1.4, 0.3, 1.4, 0.85, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -4.0, 8.0, 3, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnalysisSynchronisationRoboticApplication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.2, 0.1
	},
	{
	"Anishchenko-Astakhov Chaotic Attractor", "a", "b", "c", "s", ES, ES, ES, ES, ES, ES, 40.0, 33.0, 10.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 3, 2, 1, 4, 3, 3, NULL, 1, YAXIS, 50000,
	    NullSetup, DoAnishchenko, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Analysis and Implementation of a New Switching Memristor Scroll Hyperchaotic System in Secure Comms", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 10.0, 60.0, 10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 3.0, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -95.0, -50.0, 100.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 80000,
	    NullSetup, DoAnalysisImplementationNewSwitchingHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.6, 0.1
	},
	{
	"Analysis and Projective Synchronisation of New 4D Hyperchaotic System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.5, 2.5, 4.9, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 2.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 4, 7, 3, 4, 4, 4, NULL, 1, YAXIS, 200000,
	    NullSetup, DoAnalysisProjectiveSynchronisationNew4DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 0.04, 1.0
	},
	{
	"Analytical-Numerical Methods for Search of Hidden Oscillations", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.3, -0.4, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 50000,
	    NullSetup, DoAnalyticalNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Anisochronous Self-Excited Systems", "e", "beta", "mi", "delta", ES, ES, ES, ES, ES, ES, 1.25, 1.0, 1.5, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, -0.1, -0.3, 0.4, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -2.0, 4.0, 2, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 500000,
	    NullSetup, DoAnisochronousSelfExcited, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.6, 1.0
	},
	{
	"Anticontrol of Hopf Bifurcation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 4.0, 0.01, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, -0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -2.5, 5.0, 2, 3, 1, 3, 3, 3, NULL, 1, XAXIS, 500000,
	    NullSetup, DoAnticontrolHopf, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Antimonotonicity, Crisis and Multiple Attractors in a Simple Memristive Circuit", "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 3.846 * 0.0001, 0.6, 3.846 * 0.0001, 20.0, 5 * 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.15, -0.2, 0.4, 4, 7, 1, 5, 4, 4, NULL, 1, YAXIS, 200000,
	    NullSetup, DoAntimonotonicityCrisisMultipleAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0018, 0.001, 0.015
	},
	{
	"Anti-Synchronisation of a Novel Hyperchaotic System with Parameter Mismatch and External Disturbances", 
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 10.0, 40.0, 1.0, 3.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", "x1", "y1", "z1", "w1", ES, ES, 1.0, 2.0, -1.0, -2.0, 1.0, 2.0, -1.0, -2.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 4, 8, 2, 5, 8, 8, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoAntiSynchronisationNovel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00013, 0.5, 1.0
	},
	{
	"Anti-Synchronisation of Two Chaotic Systems (Modified)", "alpha", "lambda", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.45, 0.75, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.8, 0.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -8.0, 20.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAntiSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.4, 1.0
	},
	{
	"Application of a Chaotic Oscillator in an Autonomous Mobile Robot-II", 
	    "a", "b", "c", "d1", "k", "alpha", "s", "h", "p", "q", 0.7, 0.7, 0.7, 0.7, 1.0, 0.0064, 156.25, 2.0, 1.0, 1.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.14, 0.109, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -5.6, 12.0, 1, 6, 3, 10, 3, 3, NULL, 1, YAXIS, 2000.0,
	    NullSetup, DoApplicationChaoticOscillatorAutonomousMobileRobot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.065, 1.0, 1.0
	},
	{
	"Antinoise Performances of Improved Tent Chaos-based Phase Modulation Radar Signal", "k", "mu", ES, ES, ES, ES, ES, ES, ES, ES, 8.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -18.0, 40.0, 1, 2, 3, 2, 3, 3, NULL, 0, YAXIS, 50000,
	    NullSetup, DoAntinoise, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.3, 1.0
	},
	{
	"Application of the Silnikov Theorem I", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, -0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -3.8, 8.0, 1, 2, 3, 1, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoApplicationSilnikov, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.2, 1.0
	},
	{
	"Approx Prediction-Based Control Method for Nonlinear Oscillatory Systems with Apps to Chaotic Systems",
	    "a", "n", "w", ES, ES, ES, ES, ES, ES, ES, 0.989, 0.01, 0.45, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -3.8, 8.0, 2, 5, 6, 3, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoApproximatePredictionBasedControlMethod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.8, 2.0
	},
	{
	"Are Perpetual Points Sufficient for Locating Hidden Attractors?", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, -0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.5, 1.2, 3, 6, 1, 0, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoArePerpetualPointsSufficient, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.3, 0.3
	},
	{
	"Arneodo Chaotic System", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -5.5, 3.5, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -8.0, 16.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoArneodo, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 0.4, 1.0
	},
	{
	"Artificial Neural Network Chaotic Attractor", "k", "s", "d", "m", ES, ES, ES, ES, ES, ES, 65.0, 0.005, 0.02, 0.154, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.0, 3.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoArtificialNeural, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 1.0
	},
	{
	"Asymmetric Double Strange Attractors in a Simple Autonomous Jerk Circuit", 
	    "epsilon", "mu", "ro", "a", ES, ES, ES, ES, ES, ES, 5.4291 / 10000.0, 1.31, 0.2, 2.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -10.0, -0.01, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -22.0, 44.0, 1, 3, 2, 4, 3, 3, NULL, 1+AVERAGE, YAXIS, 50000.0,
	    NullSetup, DoAsymmetricDoubleStrangeAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 5.0, 1.0
	},
	{
	"Asymmetry Coefcients as Indicators of Chaos - Hyperchaotic Qi System",
	    "a", "c", "d", "e", "f", "g", ES, ES, ES, ES, 50.0, 13.0, 8.0, 33.0, 30.0, 1.85, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -330.0, -280.0, 400.0, 2, 3, 1, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAsymmetryCoefcientsIndicatorsChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00004, 0.7, 1.0
	},



	{
	"Attractor Embedded in the Atmosphere", "a", "b", "f", "g", ES, ES, ES, ES, ES, ES, 0.25, 4.0, 8.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.5, 3.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoAttractorAtmosphere, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 0.6, 1.0
	},
	{
	"Autonomous Silva-Young Type Chaotic Oscillator with Flat Power Spectrum", "a", "b", "c", "d", "k", "mu", "e", ES, ES, ES, 0.6, 0.2, 0.15, 0.15, 2.0, -1.0, -1.2, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.5, -0.55, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -28.0, -15.5, 32.0, 1, 2, 3, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAutonomousSilvaYoung, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Bands and branes-1", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "xp", "yp", ES, ES, ES, ES, ES, ES, 0.006, -0.1475, 0.31010, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.35, -0.15, 0.3, 2, 4, 3, 0, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoBandsAndBranes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Bands and branes-2", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "xp", "yp", ES, ES, ES, ES, ES, ES, 0.06, -0.1475, 0.31010, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.5, 1.0, 2, 4, 3, 0, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoBandsAndBranes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Basic Dynamical Analyses and Electronic Circuit Implementation of a new 3D Chaotic System", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -10.0, 18.0, 4, 5, 6, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBasicDynamicalAnalyses, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Basins of Attraction for Chimera States", "mu", "ni", "beta", ES, ES, ES, ES, ES, ES, ES, 1.2, 0.5, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "r01", "r02", "psi", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.0, -PI, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.62, 1.3, 1, 4, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoBasinsAttractionChimeraStates, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0055, 1.5, 1.0
	},
	{
	"Basins of Attraction Plasticity of a Strange Attractor with a Swirling Scroll", "alpha", "beta", "fi", "mu", "eta", "s", ES, ES, ES, ES, 2.0, -3.0, 0.8, 1.0, -2.0, 0.30, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.8, -2.15, 7.4, 5, 6, 4, 6, 3, 3, NULL, 1, YAXIS, 800000.0,
	    NullSetup, DoBasinsAttractionPlasticity, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Baskets Hyper-Chaotic Attractor in 4 Dimensions type 1", "a1", "a2", "a3", "a4", "b1", "c1", "c2", "c3", "d1", "d1", 
	    0.09, 0.05, 0.15, 0.001, 0.06, 0.1, 0.03, 0.34, 0.1, 0.06,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.78, 0.52, 0.3, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0,  -12.0,  16.0, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoBaskets4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.4, 1.0
	},
	{
	"Baskets Hyper-Chaotic Attractor in 4 Dimensions type 2", "a1", "a2", "a3", "a4", "b1", "c1", "c2", "c3", "d1", "d1", 
	    0.09, 0.05, 0.15, 0.001, 0.06, 0.1, 0.03, 0.401, 0.1, 0.06,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.78, 0.52, 0.3, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0,  -7.0,  10.0, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoBaskets4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.4, 1.0
	},
	{
	"Belyakov Homoclinic Bifurcations in a Tritrophic Food Chain Model", "a1", "a2", "b1", "b2", "d1", "d2", "r", "k", ES, ES, 
	    5.0, 0.1, 3.0, 2.0, 0.4, 0.01, 0.8751, 0.93752, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.9, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.06, 1.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoBelyakov, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Bifurcation Analysis and Chaos Control of a Fractional Order Portal Frame with Nonideal Loading I", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    0.1, 1.0, 2.0, 8.473, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, 226.0, 24.0, 2, 7, 1, 4, 4, 4, NULL, 1, YAXIS+EVOLVE, 50000.0,
	    NullSetup, DoBifurcationAnalysisI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 120.0, 1.0
	},
	{
	"Bifurcation Analysis and Chaos Control of a Fractional Order Portal Frame with Nonideal Loading II", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    0.1, 1.0, 2.0, 8.473, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    1.9, -0.05, 0.1, 4, 6, 1, 4, 4, 4, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoBifurcationAnalysisII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0012, 0.0003, 1.0
	},
	{
	"Bifurcation Analysis and Chaotic Behaviors of Fractional-Order Singular Biological Systems-I", 
	    "0-2=trig, 3-5=log(xyz), 6=|x|/x", "r", "k", "sigma", "a", "p", "m", "beta", "w", "c", 
	    2.0, 0.2, 5.0, 0.01, 0.2, 1.5, -0.00110, 0.2, 0.4, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.3, 0.7, 0.00025, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -2.6, 4.0, 1, 2, 4, 10, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoBifurcationAnalysisChaoticBehaviorsI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Bifurcation Analysis and Chaotic Behaviors of Fractional-Order Singular Biological Systems-II", "p", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    -0.0008, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.6, -0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.4, 2.8, 1, 2, 4, 1, 3, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBifurcationAnalysisChaoticBehaviorsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.5, 1.0, 1.0
	},
	{
	"Bifurcation Behavior and PD Control Mechanism of a Fractional Delayed Genetic Regulatory Model", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 4.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.74, -1.93, 3.5, 1, 5, 6, 0, 3, 2, NULL, 0, YAXIS, 2000000.0,
	    NullSetup, DoBifurcationBehaviorPDControlMechanism, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 16.0, 1.0
	},
	{
	"Bifurcations of a New Fractional-Order System with a One-Scroll Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.3, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.2, 5.4, 4, 5, 6, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBifurcationsNewFractionalOrderSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 0.1
	},
	{
	"Bifurcation & Chaos in Integer & Fractional Order 2 Degree of Freedom Shape Memory Alloy Oscillators", "a", "b", "c", "d", "w1", "w2", "e1", "e2", "e3", "f1", 
	    -20.0, 1.0, 1.5, -1.0, 1.0, 1.0, 0.2, 0.2, 0.2, 1300.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.2, 0.4, 3, 5, 7, 10, 4, 4, NULL, 1, XAXIS, 200000.0,
	    NullSetup, DoBifurcationAndChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.05, 1.0
	},
	{
	"Bifurcation Phenomena and Chaotic Attractors in a Six-Dimensional Nonlinear System", 
	    "k1", "k2", "k3", "k4 (main variable to be changed around 0.5)", "k5", "k6", "k7", "b1i", "b00", "b10", 
	    0.05, 0.011, 20.0, 0.448, 0.2, 0.5, 0.5, 1.5, 1.8, 1.3,
	    "x1", "x2", "x3", "x4", "x5", "x6", ES, ES, ES, ES, 1.1, 1.0, 5.0, 2.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.6, 1.0, 1, 6, 3, 10, 6, 6, NULL, 1, XAXIS+EVOLVE, 100000.0,
	    NullSetup, DoBifurcationPhenomena, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 0.04, 1.0
	},
	{
	"Bifurcation and Chaos in Time Delayed Fractional Order Memfractor Oscillator and Sync with Uncertainties", 
	    "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", ES, 
	    -1.89, -2.16, 4.8, 7.35, -0.0735, -0.17, 0.6528, 0.571, -0.816, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.8, -2.0, 4.0, 4, 5, 3, 9, 5, 5, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoBifurcationAndChaosTimeDelayedFractionalOrder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00008, 3.0, 1.0
	},
	{
	"Bifurcations and Synchronisation of the Fractional-Order Bloch System", "d", "g", "c", "g1", "g2", ES, ES, ES, ES, ES, 
	    1.26, 10.0, 0.7764, 0.5, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -6.0, 12.0, 4, 5, 6, 5, 3, 3, NULL, 1, YAXIS, 60000.0,
	    NullSetup, DoBifurcationSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.062, 1.0, 1.0
	},
	{
	"Birds with Points", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 5.0, 30.0, 3.0, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -60.0, 120.0, 1, 2, 3, 4, 4, 4, NULL, 0, YAXIS, 2000000.0,
	    NullSetup, DoBirds, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Birds with Lines", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 5.0, 30.0, 3.0, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -75.0, -45.0, 90.0, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoBirds, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.2, 1.0
	},
	{
	"Birth of One-to-Four-Wing Chaotic Attractors in a Class of Simplest 3D Continuous Memristive Systems", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.0, 12.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -3.0, 6.2, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBirth1to4WingChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00092, 0.2, 1.0
	},
	{
	"Bistable Hidden Attractors in a Novel Chaotic System with Hyperbolic Sine Equilibrium", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.28, 0.025, 4.0, 3.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.0, 2.0, 1, 5, 3, 4, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoBistableHidden, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Bistability in a Fifth-Order Voltage-Controlled Memristor-Based Chua’s Chaotic Circuit", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 0.22, 0.55, 0.28, 4.68, 0.022, 0.018, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.44, -0.22, 0.46, 6, 7, 8, 7, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoBistabilityFifthOrderVoltageControlledMemristorBasedChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Bistability in a 3 Dimensional System with a Line Equilibrium", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.04, 0.003, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 2.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -10.0, 50.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoBistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.7, 1.0
	},
	{
	"Bistability in a Hyperchaotic System with a Line Equilibrium I", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 5.0, 0.28, 0.05, 38.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.7, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.4, 0.8, 1, 2, 3, 4, 4, 4, NULL, 1, XAXIS, 90000.0,
	    NullSetup, DoBistabilityHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 0.2, 1.0
	},
	{
	"Bistability in a Hyperchaotic System with a Line Equilibrium II", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 30.0, 0.28, 0.05, 38.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.7, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -0.75, 1.5, 3, 4, 1, 4, 4, 4, NULL, 1, XAXIS, 90000.0,
	    NullSetup, DoBistabilityHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 10.0, 1.0
	},
	{
	"Bistability in a Hyperchaotic System with a Line Equilibrium III", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 30.0, 0.28, 0.05, 38.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.7, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, 0.1, 0.6, 1, 3, 4, 4, 4, 4, NULL, 1, YAXIS, 90000.0,
	    NullSetup, DoBistabilityHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 0.3, 1.0
	},
	{
	"Bistability in an Improved Memristor-Based Third-Order Wien-Bridge Oscillator", "a", "b", "c", "m", "k", "mu", ES, ES, ES, ES, 1.4286, 0.2, 1.7857, 0.15, 3.2, 0.0879, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.5, 3.0, 4, 5, 6, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBistabilityImprovedMemristorBased3rdOrderWienBridgeOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Bloch Equations Under Chaos", "l", "p", "t1", "t2", ES, ES, ES, ES, ES, ES, 30.0, 0.173, 5.0, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.0, 2.5, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBloch, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Blue Sky Catastrophe Chaotic Oscillator", "b", "m", "e", ES, ES, ES, ES, ES, ES, ES, 10.0, 0.456, 0.0357, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.3, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -2.5, 3.0, 3, 2, 1, 3, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoBlueSky, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Bluetooth Based Chaos Sync Using Particle Swarm Optimisation and Its Applications to Image Encryption", 
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 3.0, 11.0, 8.0, 0.5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 3, 6, 1, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBluetoothBasedChaosSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 0.5
	},
	{
	"Boa Hyper-Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 35.0, 3.0, 20.0, 0.1, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 8.0, 12.0, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -30.0, 60.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoBoa4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Boldrighini-Francheschini Chaotic Oscillator", "re", ES, ES, ES, ES, ES, ES, ES, ES, ES, 33.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, -0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.5, 4.0, 5, 2, 3, 1, 5, 5, NULL, 1, YAXIS, 100.0,
	    NullSetup, DoBoldrighini, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Bonhoffer-van der Pol Chaotic Oscillator", "a", "b", "d", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBonhoffer, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Bose-Einstein Condensate", "i0", "mu", "ni", "g", "u1", "u2", ES, ES, ES, ES, 5.5, 0.05, 2.03, -0.75, 0.0, 0.8985, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.3, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, 0.0, 4.0, 1, 3, 2, 6, 4, 4, NULL, 1, YAXIS, 40000.0,
	    NullSetup, DoBoseEinstein, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Bouali Chaotic Attractor Type 2", "alpha", "s", ES, ES, ES, ES, ES, ES, ES, ES, 0.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -3.6, 24.0, 3, 2, 1, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoBouali2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"Bouali Chaotic Attractor Type 3", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.0, 2.2, 1.0, 1.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.59, 0.574, 4.2, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBouali3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000875, 2.0, 1.0
	},
/*
	{
	"Bouali-Safieddine-1997 Chaotic Attractor", "b", "p", "q", "s", "k", "m", 0.2, 10.0, 0.1, 0.004, 0.02, 0.4,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.06, -0.05, 0.1, 1, 2, 3, 6, 3, NULL, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoBoualiSafieddine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.1, 1.0
	},
*/
	{
	"Bouali-Safieddine-1998 Chaotic Attractor", "b", "p", "q", "s", "k", "m", ES, ES, ES, ES, 0.2, 10.0, 0.1, 1.5, 0.02, 0.4, 0.0, 0.01, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.35, -0.2, 0.4, 1, 2, 3, 6, 3, 3, NULL, 1, XAXIS, 2000000.0,
	    NullSetup, DoBoualiSafieddine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.04, 1.0
	},
	{
	"Bouali-Safieddine-Modified Chaotic Attractor", "b", "p", "q", "s", "k", "m", ES, ES, ES, ES, 0.2, 10.0, 0.1, 35.0, 0.02, 0.4, -0.1, 1.0, 1.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.08, -0.05, 0.1, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBoualiSafieddine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.1, 1.0
	},
	{
	"Braid Chaotic Attractor", "a", "alpha", "v1", "v2", "e", "d", ES, ES, ES, ES, 20.0, 15.0, 0.5, -0.3, 1.4, 1.5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.025, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.5, 5.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBraid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 0.8, 1.0
	},
	{
	"Brusselator Chaotic Attractor", "a", "azao", "b", "c", "d", ES, ES, ES, ES, ES, 0.5, 0.09, 1.2, 3.9, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, -2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.1, -0.26, 0.2, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBrusselator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Burke-Shaw Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 10.0, 4.272, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBurkeShaw, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Butterfly V-shape Multiscroll Chaotic Attractor", "m", "d1", "d2", "s0", "s1", ES, ES, ES, ES, ES, 2.0, 2.0, 8.0, 0.865, 2.53, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -1.0, 10.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoButterfly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Cai Hyper-Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 27.5, 3.0, 19.3, 2.9, 3.3, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 8.0, 12.0, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -30.0, 60.0, 1, 2, 4, 5, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCai4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"CAMO - Self-Excited and Hidden Chaotic Flows", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.08, -1.7, 3.24, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCAMOChaoticFlows, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Cardiovascular Applications in 4 Dimensions", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 5.0, 0.0021, 1.56, 0.00004, 0.88, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, 10.0, 30.0, -19.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -10.0, 20.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCardiovascular, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Carlos Hyper-Chaotic Oscillator in 4+time Dimensions", "c1", "c2", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "t", ES, ES, ES, ES, ES, 10.0, -0.2, 0.3, -0.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2000.0, -1500.0, 3000.0, 1, 2, 3, 2, 5, 5, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCarlos5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 12.0, 1.0
	},
	{
	"Carpet Oscillator: Mega-Stable Nonlinear Oscillator with Infinite Islands of Self Excited & Hidden Attractors I", 
	    "horizontal range", "vertical range", "step size", ES, ES, ES, ES, ES, ES, ES, 160.0, 140.0, 24.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -230.0, 430.0, 1, 2, 3, 3, 3, 2, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCarpetOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Carpet Oscillator: Mega-Stable Nonlinear Oscillator with Infinite Islands of Self Excited & Hidden Attractors II", 
	    "horizontal range", "vertical range", "step size", ES, ES, ES, ES, ES, ES, ES, 160.0, 140.0, 25.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.4, 2.8, 3, 4, 1, 3, 3, 2, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCarpetOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 0.6, 1.0
	},
	{
	"Cascading Method for Constructing New Discrete Chaotic Systems with Better Randomness",
	    "a1", "a2", ES, ES, ES, ES, ES, ES, ES, ES, 1.99, 1.99, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.17, 1.4, 1, 2, 1, 2, 1, 1, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCascadingMethod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Categorising Chaotic Flows from the Viewpoint of Fixed Points and Perpetual Points",
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 3.0, 2.0, 10.0, 0.3, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 3.0, 2.0,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.77, -3.74, 7.8, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCategorisingChaoticFlows, "SelectOscParams", (DLGPROC)SelectOscParams, 0.028, 1.0, 1.0
	},
	{
	"Cells CNN Chaotic Attractor", "p1", "p2", "r", "s", ES, ES, ES, ES, ES, ES, 1.24, 1.1, 4.4, 3.21, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 3, 2, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Changing Climate: A Snapshot Attractor Picture", 
	    "a", "b", "g", "azao", ES, ES, ES, ES, ES, ES, 0.25, 4.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoChangingClimate, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Chaos and Multi-Scroll Attractors in RCL-Shunted Junction Coupled Jerk Circuit Connected by Memristor", 
	    "bl", "i", ES, ES, ES, ES, ES, ES, ES, ES, 2.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.5, 3.0, 3, 5, 2, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaosMultiScrollAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Chaos and Symbol Complexity in a Conformable Fractional-Order Memcapacitor System", 
	    "a1", "b1", "a2", "b2", "c", "d", "e", ES, ES, ES, -0.17, 10.0, 0.25, 0.6, 8.96, 4.0, 6.9, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.27, -0.2, 0.32, 1, 5, 2, 7, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoChaosSymbolComplexity, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Chaos-Based Application of a Novel No-Equilibrium Chaotic System with Coexisting Attractors", 
	    "k1", "k2", "k3", "k4", "k5", "k6", ES, ES, ES, ES, 0.0, -1.0, -1.0, -0.49, 0.0, -0.75, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -4.0, 6.4, 1, 6, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosBasedApplicationNovelNoEquilibriumChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0011, 0.02, 1.0
	},
	{
	"Chaos-Based Optimisation - A Review", 
	    "a", "b", "c", "mu", "gamma", "w", "real", "imag", ES, ES, 0.35, 0.05, 7.5, 0.2, 8.0, 1.02, 0.005, 0.005, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.8, 1.6, 4, 5, 3, 8, 3, 2, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoChaosBasedOptimisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.3, 0.4, 0.3
	},
	{
	"Chaos Control of Single Time-scale Brushless DC Motor with Sliding Mode Control Method", 
	    "v1", "v2", "ro", "delta", "eta", "tl", "sigma", ES, ES, ES, 0.168, 20.66, 60.0, 0.875, 0.26, 0.53, 4.5, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -5.0, 60.0, 2.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, 50.0, 20.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoChaosControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Chaos and Adaptive Synchronisations in Fractional-Order Systems", 
	    "b1", "b2", "b3", "b4", ES, ES, ES, ES, ES, ES, -1.65, 3.5, 0.3, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.3, 2.6, 1, 6, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaosAdaptiveSynchronisationsFractionalOrderSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.1, 0.1
	},
	{
	"Chaos Encryption Algorithm for Wireless Sensor Networks Based on the Reconfigure Technology of FPGA", 
	    "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, -25.4, 66.3, 39.21, -1.0, -0.9, 0.0175, -3.0, -4.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -800.0, -100.0, 1200.0, 1, 3, 2, 8, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosEncryption, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 10.0, 1.0
	},
	{
	"Chaos Entanglement Function", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 36.0, 18.0, 0.0175, -3.0, -4.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -10.0, 40.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoChaosEntanglement, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.003, 1.0
	},
	{
	"Chaos in a Novel Fractional Order System Without a Linear Term", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.7, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -0.8, 3.8, 3, 6, 1, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChaosNovelFractionalOrderSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Chaos in a Pendulum Adaptive Frequency Oscillator Circuit Experiment",
	    "a", "c", "kw", "w", ES, ES, ES, ES, ES, ES, 1.8, 0.35, 0.707, 1.67, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -1.3, 2.6, 4, 5, 6, 4, 3, 3, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoChaosPendulumAdaptiveFrequencyOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000875, 1.0, 1.0
	},
	{
	"Chaos in Learning a Simple Two-Person Game",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, -2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.1, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.0, 2.0, 1, 4, 3, 1, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosLearningSimple2PersonGame, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0004, 1.0, 1.0
	},
	{
	"Chaos in Integer Order and Fractional Order Financial Systems and their Synchronisation", 
	    "b", "c", "p", "q", "m", "n", "d", ES, ES, ES, 0.2, 1.0, 0.8, 1.8, 0.9, 0.4, 0.2, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.5, 3.8, 7, 8, 6, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosIntegerOrderFractionalOrderFinancialSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 3.0, 0.4
	},
	{
	"Chaos in Parametrically Excited Continuous Systems", "k: try -1 to 10", "a", "b1", "b2", "b4", "b6", ES, ES, ES, ES, 0.0, 0.01, 0.1, 0.1, -1.2, 1.12, 0.0, 0.0, 0.0, 0.0,
	    "p1", "q1", "p2", "q2", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosExcitedContinuous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Chaos in a 3D System with an Absolute Nonlinearity and Chaos Synchronisation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 6.8, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -4.4, 8.0, 1, 4, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoChaosIn3DSystemWithAbsoluteNonlinearity, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.3, 1.0
	},
	{
	"Chaos Synchronisation of Nonlinear Fractional Discrete Dynamical Systems via Linear Control", "k", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.091, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.4, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.4, 8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoChaosSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003 , 0.7, 0.2
	},
	{
	"Chaos in a 6D System with an Absolute Nonlinearity and Chaos Synchronization", "a", "b", "c", "kx", ES, ES, ES, ES, ES, ES, 6.8, 4.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.4, 4.0, 6, 12, 4, 4, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosIn6DSystemWithAbsoluteNonlinearity, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.4, 1.0
	},
	{
	"Chaos-Hyperchaos Transition In Coupled Rössler Systems", 
	    "b", "c", "d", "a", ES, ES, ES, ES, ES, ES, 2.0, 4.05, 0.25, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 4.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -2.0, 20.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoChaosHyperchaosTransition, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Chaotic and Hyperchaotic Complex Jerk Equations", "alpha", "beta", "ni", "eta", ES, ES, ES, ES, ES, ES, 1.0, 4.0, -5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", "x6", ES, ES, ES, ES, 4.0, 1.0, -2.0, 2.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -6.0, 12.0, 2, 5, 3, 4, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoChaoticHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.7, 1.0
	},
	{
	"Chaotic Dynamics of Modified Wien Bridge Oscillator with Fractional Order Memristor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.0, 1.0, 0.5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -6.0, 12.0, 3, 5, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaoticDynamicsModifiedWienBridgeOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.7, 1.0
	},
	{
	"Chaotic Attractor in a Simple Hybrid System", 
	    "a", "b", "c", "d", "h", "alpha", "beta", "k", "l", ES, -1.0, 8.0, -8.0, -1.0, 20.0, -114.0, 3.0, 1.0, 6.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.5, 15.0, 2, 3, 1, 9, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Chaotic Attractor in 4 Dimensions", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 23.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1400.0, -800.0, 1600.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 360000.0,
	    NullSetup, DoChaoticAttractor4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 0.5, 1.0
	},
	{
	"Chaotic Attractors of Relaxation Oscillators", 
	    "w", "a", "epsilon", ES, ES, ES, ES, ES, ES, ES, 11.509, 41.85815, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -3.0, 8.0, 1, 2, 3, 3, 3, 3, NULL, 1, XAXIS+YAXIS, 1500000.0,
	    NullSetup, DoChaoticRelaxation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Chaotic Attractor with Quadratic Exponential Nonlinear Term from Chen's Attractor I", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 36.0, 3.0, 28.0, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -64.0, 128.0, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoChaoticAttractorQuadratic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 3.0, 1.0
	},
	{
	"Chaotic Attractor with Quadratic Exponential Nonlinear Term from Chen's Attractor II", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 36.0, 3.0, 28.0, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -90.0, 144.0, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoChaoticAttractorQuadratic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000099, 3.0, 1.0
	},
	{
	"Chaotic Behavior & Feedback Control of Magnetorheological Suspension System With Fractional Derivative",
	    "a0", "a", "b", "c", "d", "w", ES, ES, ES, ES, 5.0, 66.667, -125.0, -0.1042, 1.0417, 7.78, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.22, -0.13, 0.25, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 10000.0,
	    NullSetup, DoChaoticBehaviorFeedbackControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.1, 1.0
	},
	{
	"Chaotic Chameleon: Dynamic Analyses, Circuit Implementation, FPGA Design and Fractional Order I", 
	    "A", "c", "b", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.01, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.4, 0.8, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaoticChameleon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Chaotic Chameleon: Dynamic Analyses, Circuit Implementation, FPGA Design and Fractional Order II", 
	    "A", "c", "b", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaoticChameleon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Chaotic Cuttlesh: King of Camouage with Self-Excited and Hidden Flows and Communication Designs I",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.49, -0.93, 1.73, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChaoticCuttlesh, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.6
	},
	{
	"Chaotic Cuttlesh: King of Camouage with Self-Excited and Hidden Flows and Communication Designs II",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.19, 2.39, 2, 1, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChaoticCuttlesh, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.6
	},
	{
	"Chaotic Dadras-Momeni System:  Control and Hyperchaotification-4D", 
	    "r", "m", "g", "k", ES, ES, ES, ES, ES, ES, 8.0, 0.5, 35.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.13, 0.5, 0.4, 1.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -4.0, 8.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaoticDadrasMomeniSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.8, 1.0
	},
	{
	"Chaotic Dynamics of a Fractional-Order System, its Chaos-Suppressed Sync and Circuit Implementation", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -75.0, -34.0, 80.0, 2, 5, 1, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaoticDynamicsFractionalOrder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.06, 1.0
	},
	{
	"Chaotic Evolution towards Philosophy and Methodology of Chaotic Optimisation", 
	    "a1", "a2", "a3", "b1", "b2", "b3", "d1", "d2", "d3", "mu", 2.0, 4.0, 3.0, 0.2, 2.0, 1.5, 0.3, 0.5, 0.6, 2.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.1, 1.2, 1, 4, 3, 10, 4, 4, NULL, 1, YAXIS+EVOLVE, 1000.0,
	    NullSetup, DoChaoticEvolution, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.0, 1.0
	},
	{
	"Chaotic Flow with Hidden Attractor - The First Hyperjerk System with No Equilibrium", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 2.0, 5.05, 2.5, 0.7, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -5.0, 8.0, 1, 2, 6, 5, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaoticFlowHiddenAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Chaotic Signal-Induced Dynamics of Degenerate Optical Parametric Oscillator I", 
	    "g", "d0", "d1", "ea", "u", ES, ES, ES, ES, ES, 1.0, 1.0, -5.0, 10.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -0.7, 2.4, 2, 3, 4, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoChaoticSignalInduced, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.25, 1.0
	},
	{
	"Chaotic Signal-Induced Dynamics of Degenerate Optical Parametric Oscillator II", 
	    "g", "d0", "d1", "ea", "u", ES, ES, ES, ES, ES, 1.0, 1.0, -5.0, 10.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -12.0, 8.0, 1, 4, 2, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoChaoticSignalInduced, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 2.4, 1.0
	},
	{
	"Chaotic Signal-Induced Dynamics of Degenerate Optical Parametric Oscillator III", 
	    "g", "d0", "d1", "ea", "u", ES, ES, ES, ES, ES, 1.0, 1.0, -5.0, 10.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.8, -8.0, 6.0, 3, 4, 2, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoChaoticSignalInduced, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.2, 1.0
	},
	{
	"Chaotic System with Different Families of Hidden Attractors", 
	    "a", "b", "c", "d", "r", "k", ES, ES, ES, ES, 0.1, 3.0, 2.2, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.175, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.8, 3.6, 2, 5, 3, 6, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaoticDifferentFamilies, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Chaotic System with Multiple Attractors - Dynamic Analysis, Circuit Realization and S-Box Design", 
	    "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 2.0, 8.0, 2.9, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -10.0, 22.0, 3, 6, 1, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaoticSystemMultipleAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.04, 1.0
	},
	{
	"Chaotic System with Rounded Square Equilibrium and with No-Equilibrium", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 5.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.8, 2.0, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.4, 1.0
	},
	{
	"Chaotic System with Equilibria Located on the Rounded Square Loop and its Circuit Implementation", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.1, 3.0, 2.2, 0.15, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.1, -1.25, 2.5, 2, 5, 3, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoChaoticSystemEquilibriaLocatedRoundedSquareLoop, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.4, 1.0
	},
	{
	"Chaotification in the Stretch-Twist-Fold (STF) Flow", "alpha", "beta", ES, ES, ES, ES, ES, ES, ES, ES, 0.2, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, -0.2, 0.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.4, 1, 6, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChaotificationSTFFlow, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.4, 1.0
	},
	{
	"Characteristics of the Chaotic Parameters for a Loss Type of Modified Coupled Dynamic System", "a", "mu", "e", ES, ES, ES, ES, ES, ES, ES, 4.0, 2.0, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -8.0, 16.0, 3, 4, 6, 3, 3, 3, NULL, 1, YAXIS, 35000.0,
	    NullSetup, DoCharacteristicsChaoticParameters, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Chemical Chaos", "d", "lambda", "e", ES, ES, ES, ES, ES, ES, ES, 0.51, 1.339, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -0.5, 5.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 400000.0,
	    NullSetup, DoChemicalChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Chemical Oscillations Out of Chemical Noise", "mu", "sigma", ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.53, 0.91, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.65, -0.6, 1.2, 1, 4, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChemicalOscillations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Chen Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -54.0, -5.0, 64.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoChen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.7, 1.0
	},
	{
	"Chen Circuit-Like Model: High Periodicity Leading to Chaotic Dynamics", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 61.0, 72.0, 23.0, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.001, 0.002, 0.003, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.7, -4.14, 8.5, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChenCircuitLikeModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008755, 1.0, 1.0
	},
	{
	"Chen-Lee Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 5.0, -10.0, -0.38, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 4.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -24.0, 48.0, 3, 2, 1, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoChenLee, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 1.0, 1.0
	},
	{
	"Chen Like Chaotic Attractor", "a", "c", "d", "e", "f", "gamma", ES, ES, ES, ES, 60.0, 5.0/6.0, 0.4, 0.65, 25.0, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 1.0, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -480.0, -250.0, 540.0, 1, 3, 2, 6, 4, 4, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoChenLike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Chen-Multiscroll Chaotic Attractor", "a", "b", "c", "d1", "d2 (try 5,8,14,22,28)", ES, ES, ES, ES, ES, 36.0, 3.0, 28.7, 1.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -54.0, -2.0, 64.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChenMultiscroll, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Chimeralike States in a Network of Oscillators under Attractive and Repulsive Global Coupling", "a", "f", "w", "m", "e", ES, ES, ES, ES, ES, 0.2, 1.0, 0.94, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.1, -0.2, 0.4, 1, 2, 3, 5, 2, 2, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoChimeralikeStates, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Chua Chaotic Attractor", "a", "b", "alpha", "beta", ES, ES, ES, ES, ES, ES, -1.14286, -0.714286, 15.6, 28.58, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.25, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.0, 6.0, 3, 1, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Chua Cubic Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 16.0, -0.243, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.0, 2.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoChuaCubic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 1.0, 1.0
	},
	{
	"Chua's Chaotic System in 4 Dimensions", "a", "c", "s", "alpha", "beta", "gamma", ES, ES, ES, ES, 0.03, -1.2, 0.4, 30.0, 50.0, 0.8, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, 40.0, 19.0, 15.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 1, 2, 3, 6, 4, 4, NULL, 1, ZAXIS, 20000.0,
	    NullSetup, DoChuaQuartic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Chua-Matsumoto's Chaotic Oscillator", "alpha", "beta", "a", "b", "c", "d", ES, ES, ES, ES, 10.814, 14.0, 1.3, 0.11, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChuaMatsumoto, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 10.0, 1.0
	},
	{
	"Circuit Design and Experimental Investigations for a Predator-Prey Model", "m", "l", "k", "e", ES, ES, ES, ES, ES, ES, 0.22, -0.2, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.82, -0.14, 1.26, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoCircuitDesignPredatorPreyModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 0.01
	},
	{
	"Circuit Implementation of a Modified Chaotic System with Hyperbolic Sine Nonlinearities Using Bi-Colour LED", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.0004, 0.5, 0.0003846, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.000001, 0.1, 0.000001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.9, -129.0, 244.0, 5, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCircuitImplementationModifiedChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000751, 0.5, 1.0
	},
	{
	"Circuit Implementations of a Nonlinear System for Integer and Fractional Orders", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.25, -0.25, -0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -18.0, 36.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCircuitImplementationNonlinearSystemIntegerFractionalOrders, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Circuit Implementation, Sync of Multistability, and Image Encryption of a 4-Wing Memristive Chaotic System", "a", "b", "alpha", "beta", "epsilon", "gamma", ES, ES, ES, ES, 
	    -0.5, 2.4, 15.0, 8.0, 1.68, 15.15, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 2, 5, 3, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCircuitImplementationFourWingMemristiveChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.03, 1.0
	},
	{
	"Clown-Lorenz+Function Projective Synchronisation", "a", "b", "d", ES, ES, ES, ES, ES, ES, ES, 1.5, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.1, 0.1, 0.01, 1.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -320.0, -250.0, 500.0, 2, 3, 4, 3, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoClown, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.5, 1.0
	},
	{
	"Coexistence of Generalised Sync & Inverse Generalised Sync Between Chaotic and Hyperchaotic Systems", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.1, 3.0, 2.2, 0.2, 0.81, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -0.95, 4.1, 5, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoCoexistenceGeneralisedSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 1.0, 1.0
	},
	{
	"Coexistence of Hidden Attractors, 2-Torus and 3-Torus in a New 4D Chaotic System with cosh() Nonlinearity", "a", "b", "e", "c", ES, ES, ES, ES, ES, ES, 1.0, 0.6, 0.5, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.001, 0.001, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.2, 1.2, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoexistenceHiddenAttractors2Torus3Torus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.1
	},
	{
	"Coexistence of Hidden Chaotic Attractors in a Novel No-equilibrium System", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.35, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -3.1, 14.0, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCoexistenceHiddenChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Coexistence of Single & Multi-Scroll Chaotic Orbits in a Single-Link Flexible Joint Robot Manipulator", 
	    "p1", "p2", "p3", "p4", "p5", "a", ES, ES, ES, ES, -1.0, 1.0, 1.0, 1.0, -1.0, 0.5, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.8, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.85, -0.55, 1.0, 2, 8, 1, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCoexistenceSingleMultiScrollChaoticOrbits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0058, 0.5, 0.02
	},
	{
	"Coexisting Attractors and Circuit Implementation of a New 4D Chaotic System with Two Equilibria", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 10.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 1, 4, 3, 3, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCoexistingAttractorsCircuitImplementation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.2, 1.0
	},
	{
	"Coexisting Attractors, Crisis Route to Chaos in a Novel 4D System & Variable-Order Circuit Implementation", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 4.55, 9.0, 1.0, 16.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, -80.0, 160.0, 3, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoCoexistingAttractorsCrisisRoute2Chaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0007, 10.0, 1.5
	},
	{
	"Coexisting Attractors Generated from a New 4D Smooth Chaotic System", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.0, 22.0, 8.1, 4.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -28.0, 70.0, 1, 7, 2, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoexistingAttractorsGenerated4DSmoothChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 0.03, 1.0
	},
	{
	"Coexisting Infinitely Many Attractors in Active Band-Pass Filter-Based Memristive Circuit",
	    "a", "b1", "b2", "c", "r1", "r2", "k", ES, ES, ES, 3.0, 15.0/14.0, 3.0/28.0, 20.0, 15.0, 0.15, 0.05, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.3, 0.001, 0.001, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.375, 4.75, 1, 2, 3, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCoexistingInfinitelyManyAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000675, 1.0, 1.0
	},
	{
	"Coexisting Multi-Stable Patterns in Memristor Synapse-Coupled Hopfield Neural Network with Two Neurons",
	    "k", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.17, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.44, -4.26, 6.24, 4, 6, 5, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCoexistingMultiStablePatterns, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.7, 0.7
	},
	{
	"Coexisting Oscillation and Extreme Multistability for a Memcapacitor-Based Circuit", "a", "b", "c", "d", "e", "f", "m", "n", ES, ES, 
	    5.8, 1.5, 2.6, 0.1, -3.4, 0.2, 6.8, 2.8, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.4, 5.0, 4, 6, 3, 8, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCoexistingOscillation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.02, 1.0
	},
	{
	"Coherent Motion of Chaotic Attractors", "a", "b", "g", ES, ES, ES, ES, ES, ES, ES, 
	    0.025, 0.765, 0.0938, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.01, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -9.0, 48.0, 1, 4, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCoherentMotionChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0035, 4.0, 0.2
	},
	{
	"Colour Image Compression-Encryption Algorithm Based on Discrete Chaotic Neural Network and DNA Sequence", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	    6.0, 1.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.01, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.2, -5.2, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoColourImageCompressionEncryptionAlgorithmNeuralNetworkDNA, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Colour Image Compression-Encryption Algorithm Based on Fractional-Order Memristor Chaotic Circuit", "a", "b", "c", "g", ES, ES, ES, ES, ES, ES,
	    12.375, 7.0213, 2.475, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -6.0, 12.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoColourImageCompressionEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 4.0
	},
	{
	"Colpitts-Hyperchaos in Coupled Colpitts Oscillators", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 30.0, 0.8, 20.0, 0.08, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -130.0, -30.0, 100.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoColpitts, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Colpitts-Oscillator Stretched", "s1", "s2", "gamma", "epsilon", ES, ES, ES, ES, ES, ES, 1.25, 1.0, 2.1, 1.175, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.3, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -300.0, -30.0, 200.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoColpittsStretched, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Combination-Combination Phase Sync among Non-Identical Fractional Order Complex Chaotic Systems", 
	    "a11", "a12", "a13", ES, ES, ES, ES, ES, ES, ES, 10.0, 180.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 2.0, 3.0, 5.0, 6.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -32.0, 64.0, 3, 7, 2, 3, 5, 5, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoCombinationCombinationPhaseSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.05, 0.05
	},
	{
	"Combination Hyperchaos Synchronisation of Complex Memristor Oscillator System I", 
	    "a1", "a2", "a3", "a4", "a5", "a6", "a7", "Subtract x? 0 for no", ES, ES, 9.0, -10.8, 10.8, 1.0, 30.0, 30.0, 15.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8,  -0.5,  1.0, 2, 3, 4, 8, 5, 5, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoCombinationHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 1.0
	},
	{
	"Combination Hyperchaos Synchronisation of Complex Memristor Oscillator System II", 
	    "a1", "a2", "a3", "a4", "a5", "a6", "a7", "Subtract x? 0 for no", ES, ES, 9.0, -10.8, 10.8, 1.0, 30.0, 30.0, 15.0, 1.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 3, 5, 4, 8, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCombinationHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 5.0, 1.0
	},
	{
	"Combination Synchronisation of Three Nonidentical Ecological Systems with Species Invasion 3D Version",
	    "w31", "w32", "w33", "w34", "w35", "w36", "w37", "w38", "w39", ES, 0.48, 2.93, 0.54, 0.08, 0.1, 1.15, 0.35, 0.2, 0.25, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.5, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.52, 0.0, 1.0, 2, 3, 1, 9, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoCombinationSynchronisation3NonidenticalEcologicalSystems3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 2.0, 0.7
	},
	{
	"Combination Synchronisation of Three Nonidentical Ecological Systems with Species Invasion 4D Version",
	    "w11", "w12", "w13", "w14", "w15", "w16", "w17", "w18", "w19", ES, 1.0, 3.0, 0.1, 2.0, 0.2, 2.0, 0.1, 0.01, 0.02, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.2, 0.052, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.187, -0.066, 0.2, 6, 7, 5, 9, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCombinationSynchronisation3NonidenticalEcologicalSystems4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.045, 4.0, 0.7
	},
	{
	"Complex Behaviours of a Novel Three Dimensional Autonomous System", "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, 0.5, 1.0, 1.0, 4.0, 1.2, 1.0, 6.0, 0.1, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, 1.5, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -90.0, 150.0, 1, 3, 2, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoComplexBehaviours, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00305, 1.0, 1.0
	},
	{
	"Complex Dynamics, Hidden Attractors & Continuous Approx of a Fractional-Order Hyperchaotic PWC System", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 2.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.2, 0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -12.0, 24.0, 4, 6, 2, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoComplexDynamicsHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"Complex Dynamics in a Memristive Diode Bridge-Based MLC Circuit: Coexisting Attractors and Double-Transient Chaos -Second Version",
	    "a1", "a2", "b", "c", "e", "f", "w", ES, ES, ES, 1.0307, 1.34, 0.09975, 7.275E-5, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.9, -3.4, 6.8, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 900000.0,
	    NullSetup, DoComplexDynamicsMemristiveDiodeBridge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Complex System and Its AntiSynchronisation", "a1", "a2", "a3", ES, ES, ES, ES, ES, ES, ES, 0.5, 5.0, -0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", ES, ES, ES, ES, ES, 0.1, 1.0, -8.0, 2.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3000.0, -1600.0, 3200.0, 1, 2, 3, 3, 5, 5, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoComplexSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 1.0, 1.0
	},
	{
	"Compound Lorenz + Chen + Lu Chaotic Attractors", "a1", "b1", "c1", "a2", "b2", "c2", "a3", "b3", "c3", ES, 
	    10.0, 28.0, 8.0/3.0, 35.0, 8.0/3.0, 28.0, 36.0, 3.0, 20.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.12, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 3, 2, 9, 3, 3, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoCompoundLorenzChenLu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Compound Lorenz + Chen + L + SM + Rucklidge and Piecewise-Lorenz Chaotic Attractors", "a1", "b1", "c1", "a2", "b2", "c2", "a3", "b3", "c3", ES, 
	    10.0, 30.0, 8.0/3.0, 35.0, 8.0/3.0, 28.0, 36.0, 3.0, 20.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 1.7, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -2.0, 10.0, 2, 3, 1, 9, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCompoundLorenzChenLSMRucklidge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Computer Assisted Proof of the Existence of the Lorenz Attractor in the Shimizu-Morioka System", "alpha", "lambda", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.5, 0.85, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.7, 5.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoComputerAssistedProof, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Connecting the Kuramoto Model and the Chimera State-II", "mu", "ni", "d", "beta", ES, ES, ES, ES, ES, ES, 
	    0.525, 0.475, 0.0006, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.167, 0.267, -0.167, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.0, 6.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 710000.0,
	    NullSetup, DoConnectingKuramotoModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 5.0, 1.0
	},
	{
	"Constructing a Chaotic System with any Number of Equilibria - 2 Equilibria", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    0.003, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.35, -0.72, 1.5, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoConstructingChaoticSystem2Equilibria, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 0.4 
	},
	{
	"Constructing a Chaotic System with any Number of Equilibria - 3 Equilibria", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.72, -0.16, 1.9, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoConstructingChaoticSystem3Equilibria, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 1.0, 0.7
	},
	{
	"Constructing Chaotic Repellors", "k", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.2, 2.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -31.0, -22.0, 27.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoConstructingChaoticRepellors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Constructing Infinitely Many Attractors in a Programmable Chaotic Circuit", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	1.7, 1.7, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-5.0, -5.7, 6.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoConstructingInfinitelyManyAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Constructing Multiwing Attractors from a Robust Chaotic System with Non-Hyperbolic Equilibrium Points", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES,
	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 1.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-7.7, -4.86, 8.9, 3, 5, 2, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoConstructingMultiwingAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000058, 0.02, 1.0
	},
	{
	"Control of Multistability in a Self-Excited Memristive Hyperchaotic Oscillator", "a1", "a2", "c", "e1", "e2", "e3", "k11", "k22", ES, ES,
	    0.98, 0.90289, 4.9019e-05, 3.2281, 15.0, 0.0184, 0.0166, 0.0089, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.5, 1.0, 0.5, 0.3, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -37.0, -24.0, 42.0, 2, 6, 3, 8, 5, 5, NULL, 1, YAXIS, 80000.0,
	    NullSetup, DoControlMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"Constructing, Analysing and Synchronising a Chaotic System with Equilibria of Rectangle Loop", "a", "b", "c", "d", "r", ES, ES, ES, ES, ES, 
	    0.525, 0.475, 0.0006, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 3.0, 0.1, -0.1, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1200.0, -800.0, 1300.0, 5, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoConstructingAnalysingSynchronisingChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 100.0
	},
	{
	"Constructing Chaotic System With Multiple Coexisting Attractors", "a", "b", "c", "l", "k", ES, ES, ES, ES, ES, 
	    39.0, 30.0, 3.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -500000.0, -300000.0, 600000.0, 6, 7, 8, 5, 4, 4, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoConstructingChaoticSystemWithMultipleCoexistingAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0000151, 0.2, 0.01
	},
	{
	"Constructing Chaotic Systems with Total Amplitude Control-AB1", "m", "a", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.4, -5.0, 10.0, 1, 4, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoConstructingChaoticSystemsAB1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Constructing Chaotic Systems with Total Amplitude Control-AB5", "m", "a", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.4, -5.0, 10.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoConstructingChaoticSystemsAB5, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Control and Synchronisation of a Novel Seven-Dimensional Hyperchaotic System with Active Control", "a", "b", "c", "d", "e", "f", "g", "h", ES, ES,
	    15.0, 5.0, 0.5, 25.0, 10.0, 4.0, 0.1, 1.5, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "r", ES, ES, ES, 6.5, -3.5, -4.5, -5.5, -1.0, -7.0, 0.1, 0.0, 0.0, 0.0,
	    -22.0, -12.5, 25.0, 1, 2, 3, 8, 7, 7, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoControlSynchronisation7DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00015, 1.0, 1.0
	},
	{
	"Control Aspects of a Theoretical Model for Epileptic Seizures", "alpha", "beta", "gamma", "w", "b", "e1", "e2", "u1", ES, ES, 
	    0.4, 0.33, 5.0, 0.95, 1.5, 0.015, 0.03, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -2.0, 20.0, 1, 3, 2, 8, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoEpilepticSeizures, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Control of a Simple Chaotic Flow Having a Line Equilibrium by Means of a Single Passive Controller", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.62, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -13.0, 20.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoControlSimpleChaoticFlow, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.5, 1.0
	},
	{
	"Control of New 3D Chaotic System", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    1.5, 2.5, 4.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoControlNew3DChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Controlling a Chaotic Resonator by Means of Dynamic Track Control", "i", "bc", "bl", ES, ES, ES, ES, ES, ES, ES, 
	    1.2, 0.707, 2.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.4, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -2.4, 4.8, 2, 5, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoControllingChaoticResonator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Controlling Extreme Multistability of Memristor Emulator-Based Dynamical Circuit in Flux-Charge Domain", "a", "b", "c", "d", "e", "f", "g", "n0", "x9", ES, 
	    4.0, 1.0, 1.0, 1.0, 1.0, 4.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.8, -1.5, -2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -34.0, 80.0, 1, 6, 3, 9, 3, 3, NULL, 0, XAXIS, 1000000.0,
	    NullSetup, DoControllingExtremeMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.0001, 1.0
	},
	{
	"Controlled Predator-Prey Model", "a", "b", "c1", "c2", "c3", "c4", "c5", "c6", "d1", "d2", 1.72, 0.52, 0.83, 0.64, 0.23, 0.32, 0.76, 0.48, 0.12, 0.25,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.8, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoControlledPredatorPrey, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.25, 1.0
	},
	{
	"Controlling Hyperchaotic nXm-Scroll Attractors", "alpha", "beta", "m1", "m2", "a", "b", "c1", "d1", "c2", "d2", 
	    10.814, 14.0, 0.25, 0.25, 1.3, 0.11, 2.0, PI, 3.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -5.0, 20.0, 1, 6, 3, 10, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoControllingHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.45, 1.0
	},
	{
	"Coronary Artery Chaos Phenomenon", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.005, -0.0025, 0.005, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoCoronarianArtery, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"Cost Function Based on Gaussian Mixture", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 3.4, -1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -6.0, 8.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCostFunction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 2.0, 1.0
	},
	{
	"Coullet Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.8, -1.1, -0.45, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.41, 0.31, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 3, 1, 2, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCoullet, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Coupled Dynamics of Two Particles with Different Limit Sets", "gamma", "h1", "h2", "omega", "kappa", ES, ES, ES, ES, ES, 0.1, 1.3, 1.5, 2.25, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.13, -0.5, -0.4, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 1, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCoupledDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.6, 1.0
	},
	{
	"Coupled Rossler Systems", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.368, 2.0, 4.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "y1", "y2", "y3", ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.3, -0.3, 0.6, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 10.0, 5, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCoupledRossler, "SelectOscParams", (DLGPROC)SelectOscParams, 0.006, 1.0, 1.0
	},
	{
	"Coupled Systems with Hyperchaos and Quasiperiodicity I", "a", "c", "d", "e", "k", "b", ES, ES, ES, ES, 
	    35.0, 25.0, 5.0, 35.0, 430.0, 12.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -64.0, 120.0, 1, 2, 4, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 5.0, 1.0
	},
	{
	"Coupled Systems with Hyperchaos and Quasiperiodicity II", "a", "c", "d", "e", "k", "b", ES, ES, ES, ES, 
	    35.0, 25.0, 5.0, 35.0, 430.0, 12.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -12.0, 120.0, 1, 3, 4, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 10.0, 1.0
	},
	{
	"Coupled Systems with Hyperchaos and Quasiperiodicity III", "a", "c", "d", "e", "k", "b", ES, ES, ES, ES, 
	    35.0, 25.0, 5.0, 35.0, 430.0, 12.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -1.0, 24.0, 2, 3, 4, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Coupled Systems with Hyperchaos and Quasiperiodicity IV", "a", "c", "d", "e", "k", "b", ES, ES, ES, ES, 
	    35.0, 25.0, 5.0, 35.0, 430.0, 12.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -110.0, -75.0, 120.0, 1, 4, 2, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Coupled Systems with Hyperchaos and Quasiperiodicity V", "a", "c", "d", "e", "k", "b", ES, ES, ES, ES, 
	    35.0, 25.0, 5.0, 35.0, 430.0, 12.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -16.0, 25.0, 2, 4, 3, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.06, 1.0
	},
	{
	"Coupled Systems with Hyperchaos and Quasiperiodicity VI", "a", "c", "d", "e", "k", "b", ES, ES, ES, ES, 
	    35.0, 25.0, 5.0, 35.0, 430.0, 12.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -6.4, 10.0, 3, 4, 2, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.025, 1.0
	},
	{
	"Coupling of the Double-Well Duffing System I", "a", "k", ES, ES, ES, ES, ES, ES, ES, ES, -0.1, 19.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 2.0, 0.5, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -8.0, 16.0, 1, 3, 4, 2, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoCouplingDoubleWell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Coupling of the Double-Well Duffing System II", "a", "k", ES, ES, ES, ES, ES, ES, ES, ES, -0.1, 19.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "x1", "y1", "z1", "w1", ES, ES, 0.2, 2.0, 0.5, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -12.0, 24.0, 1, 6, 3, 2, 8, 8, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoCouplingDoubleWell8D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 1.0
	},
	{
	"Crack Synchronisation of Chaotic Circuits Under Field Coupling", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.4, 0.3, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.25, -0.18, 0.6, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoCrackSynchronisationChaoticCircuits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},

#ifdef _WIN64
	{
	"Cracked Beams Chaotic Attractor", "d", "k1", "k2", "f", "w", ES, ES, ES, ES, ES, -0.05, 0.1, 1.0, 7.5, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -7.44403, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0, -170.0, 350.0, 3, 2, 1, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCrackedBeams, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
#else
	{
	"Cracked Beams Chaotic Attractor", "d", "k1", "k2", "f", "w", ES, ES, ES, ES, ES, -0.05, 0.1, 1.0, 7.5, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -7.4, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -72.0, -48.0, 96.0, 3, 2, 1, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCrackedBeams, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
#endif
/*
	{
	"Cryptanalysing & Improving Novel Color Image Encryption Algorithm Using RT-Enhanced Chaotic Tent Maps", "mu: try between -4 & 4", ES, ES, ES, ES, ES, ES, ES, ES, ES, -0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.5, 2.0, 1, 2, 1, 1, 1, 1, NULL, 1, YAXIS+EVOLVE, 3000.0,
	    NullSetup, DoCryptanalysingNovelColorImageEncryption, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
*/
	{
	"Crisis & Inverse Crisis Route to Chaos in a 3D Chaotic System with Stable Node Foci Nature of Equilibria", 
	    "a", "b", "c", "d", "r", "r0", ES, ES, ES, ES, 10.0, 0.098, 4.5, 30.0, 45.6, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 5.0, 44.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -27.0, 52.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCrisisInverseCrisisRoute, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 0.4
	},
	{
	"Cross Waves on Free Surface I", "k", "a", "b", "b1", "b2", "b4", "b6", "colour: 0 = multicolour", ES, ES, 5.0, 0.01, -1.531, 0.2, 0.6, 0.25, 1.12, 0.0, 0.0, 0.0,
	    "p1", "q1", "p2", "q2", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -25.0, 50.0, 2, 4, 3, 8, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoCrossWaves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 5.0, 1.0
	},
	{
	"Cross Waves on Free Surface II", "k", "a", "b", "b1", "b2", "b4", "b6", "colour: 0 = multicolour", ES, ES, 1.0, 0.01, -1.531, 0.2, 0.6, 0.25, 1.12, 0.0, 0.0, 0.0,
	    "p1", "q1", "p2", "q2", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.5, 7.0, 1, 4, 2, 8, 4, 4, NULL, 1, XAXIS, 1000000.0,
	    NullSetup, DoCrossWaves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Cryptanalysis and Improvement on an Image Encryption Algorithm Design Using a Novel Chaos Based S-Box", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    1.0, 1.0, 2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -1.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.3, -2.8, 4.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCryptanalysisImprovementImageEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.5
	},
	{
	"Cryptanalysis of a Chaotic Communication Scheme Using Parameter Observer", "b", "c", "d", "emezao", "m", ES, ES, ES, ES, ES, 
	    28.0, 1.0, 8.0, 5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -55.0, -30.0, 60.0, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCryptanalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.5, 1.0
	},
	{
	"Cubic-Equilibrium Chaotic System with Coexisting Hidden Attractors: Analysis & Circuit Implementation", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.28, 3.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 5, 6, 3, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCubicEquilibriumChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Cubic Trigonometric Nonuniform Spline Curves", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -25.0, 27.0, 4, 6, 5, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoCubicTrigonometricNonuniformSplineCurves, "SelectOscParams", (DLGPROC)SelectOscParams, 10.0, 0.5, 0.5
	},
	{
	"Dadras Chaotic Attractor", "p", "q", "r", "s", "e", ES, ES, ES, ES, ES, 4.0, 2.7, 1.7, 2.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -25.0, 50.0, 3, 1, 2, 5, 3, 3, NULL, 0, YAXIS, 80000.0,
	    NullSetup, DoDadras, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions A", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -1.0, 36.0, 2, 3, 1, 7, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 0.6, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions B", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -10.0, 20.0, 1, 2, 5, 7, 5, 5, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 0.1, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions C", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -1.0, 140.0, 2, 3, 5, 7, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 2.5, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions D", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 2.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -120.0, 300.0, 1, 5, 3, 7, 5, 5, NULL, 1, XAXIS, 1000000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 5.0, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions E", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 2.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -35.0, 70.0, 2, 4, 3, 7, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.02, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions F", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 2.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -24.0, 60.0, 3, 5, 2, 7, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions G", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 2.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2400.0, -1000.0, 2800.0, 4, 5, 3, 7, 5, 5, NULL, 1, XAXIS+EVOLVE, 1000000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 50.0, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions H", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 3.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -3.0, 30.0, 2, 3, 5, 7, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.5, 1.0
	},
	{
	"Damage Assessment Using Hyperchaotic Excitation and Nonlinear Prediction Error in 5 Dimensions I", 
	    "s", "r", "b", "d", "k", "delta", "Version (0 - 3) thanks to Marcus Rezende", ES, ES, ES, 10.0, 28.0, 8.0/3.0, 2.0, 10.0, 0.2, 3.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -44.0, -25.0, 50.0, 2, 4, 5, 7, 5, 5, NULL, 1, YAXIS+EVOLVE, 1000000.0,
	    NullSetup, DoDamageAssessment5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 1.0
	},
	{
	"Data Analysis Techniques for Nonlinear Dynamical Systems I", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.25, 3.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -15.0, 11.0, 0.2, 23.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, -10.0, 150.0, 2, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDataAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Data Analysis Techniques for Nonlinear Dynamical Systems II", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.25, 3.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -15.0, 11.0, 0.2, 23.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, 40.0, 280.0, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDataAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 5.0, 1.0
	},
	{
	"Data Analysis Techniques for Nonlinear Dynamical Systems: Conjugate Pairs I", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.25, 3.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", "x", "y", "z", "w", ES, ES, -15.0, 11.0, 0.2, 23.0, -15.0, 11.0, 0.2, 23.0, 0.0, 0.0,
	    -200.0, -140.0, 200.0, 1, 5, 3, 4, 8, 8, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDataAnalysisCP, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Data Analysis Techniques for Nonlinear Dynamical Systems: Conjugate Pairs II", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.25, 3.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", "x", "y", "z", "w", ES, ES, -15.0, 11.0, 0.2, 23.0, -15.0, 11.0, 0.2, 23.0, 0.0, 0.0,
	    -170.0, -90.0, 180.0, 2, 6, 1, 4, 8, 8, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDataAnalysisCP, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Data Analysis Techniques for Nonlinear Dynamical Systems: Conjugate Pairs III", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.25, 3.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", "x1", "y1", "z1", "w1", ES, ES, -15.0, 11.0, 0.2, 23.0, -15.0, 11.0, 0.2, 23.0, 0.0, 0.0,
	    -100.0, -125.0, 250.0, 3, 7, 1, 4, 8, 8, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDataAnalysisCP, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.04, 1.0
	},
	{
	"Data-Assisted Reduced-Order Modelling of Extreme Events in Complex Dynamical Systems", 
	    "x01", "x04", "c", "beta", "gamma", "d", ES, ES, ES, ES, 0.95, -0.76095, 0.1, 1.25, 0.2, 0.5, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, -0.9, 3.0, 2.0, -1.7, -23.0, 24.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -3.5, 7.0, 2, 11, 3, 6, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDataAssistedReducedOrderModeling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Decomposition Method for Studying Smooth Chua’s Equation with Application to Multiscroll Attractors", "alpha", "beta", "gamma", "k", "a", "b", ES, ES, ES, ES, 
	    3.7091002664, 24.0799705758, -0.8592556780, 1.0, 0.4530092443, -2.93154465321, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.4, -2.5, 5.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoDecompositionMethod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.7, 1.0
	},
	{
	"Degenerate Hopf Bifurcations, Hidden Attractors and Control in the Extended Sprott E System", "f", "g", "e", ES, ES, ES, ES, ES, ES, ES, 
	    -0.1, 0.02, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.6, 0.9, -1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 2.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDegenerateHopfbifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.25, 1.0
	},
	{
	"Degenerating the Butterfly Attractor in a Plasma Perturbation Model using Nonlinear Controllers", "a", "h", "d", "eta", ES, ES, ES, ES, ES, ES, 
	    0.85, 2.2, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 0.005, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -4.8, 11.0, 1, 4, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDegeneratingButterflyAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"Depth-Two Heteroclinic Network I", "a", "b", "w", ES, ES, ES, ES, ES, ES, ES, 0.2, 40.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.01, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoDepthTwoHeteroclinic1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 1.0, 1.0
	},
	{
	"Depth-Two Heteroclinic Network II", "alpha", "beta", "w", ES, ES, ES, ES, ES, ES, ES, 0.0001, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 8.194e-40, -0.1, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoDepthTwoHeteroclinic2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 1.0, 1.0
	},
	{
	"Dequan-Li Chaotic Attractor", "a", "c", "d", "e", "k", "f", ES, ES, ES, ES, 40.0, 1.833, 0.16, 0.65, 55.0, 20.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.349, 0.0, -0.16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -350.0, -200.0, 400.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDequanLi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Describing Chaotic Attractors: Regular and Perpetual Points - L. Ying & Alli Model", "a3", "b1", "b2", "b3", "omega", "a0", "a1", ES, ES, ES, 7.48, 0.28, 0.0285, 0.0285, 1.34, 2.23, 2.13, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.38, 0.8, 2, 5, 4, 7, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoDescribingChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 0.2
	},
	{
	"Design and Control of a Multi-Wing Dissipative Chaotic System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, -1.0, -2.0, 1.0, 8.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -58.0, -30.0, 68.0, 4, 5, 6, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDesignControlMultiWingDissipativeChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.6, 1.0
	},
	{
	"Design and DSP in a Fractional-Order Detuned Laser Hyperchaotic Circuit with App's in Image Encryption", "L1", "L2", "L3", "L4", "L5", "L6", ES, ES, ES, ES, 33.0, 0.08, 60.0, 5.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "r", ES, ES, ES, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 1.0, 0.0, 0.0, 0.0,
	    -130.0, -75.0, 150.0, 1, 2, 3, 6, 7, 7, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoDesignDSPImplementationFractionalOrderDetunedLaser, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Design and Implementation of a New Memristive Chaotic System in Touchless Fingerprint Encryption", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 10.0, 36.0, 8.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -52.0, -2.53, 57.0, 1, 3, 2, 4, 4, 4, NULL, 1, YAXIS, 70000.0,
	    NullSetup, DoDesignImplementationNewMemristiveChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00051, 0.6, 0.6
	},
	{
	"Design and Processing of a Novel Chaos-Based Stepped Frequency Synthesised Radar Signal - Bernoulli", 
	    "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, -0.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.55, -0.02, 0.1, 1, 3, 2, 1, 2, 2, NULL, 1+AVERAGE, YAXIS, 10000.0,
	    NullSetup, DoDesignProcessingNovelChaosBased, "SelectOscParams", (DLGPROC)SelectOscParams, 0.018, 0.03, 1.0
	},
	{
	"Design and Simulation of a Memristor Chaotic Circuit Based on Current Feedback Op Amp",
	    "alpha", "beta", "k", "gamma", "a", "b", ES, ES, ES, ES, 0.4, 1.0, 0.3, 1.0, 0.57, -0.45, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.57, -0.342, 0.63, 3, 5, 6, 6, 4, 4, NULL, 1 + AVERAGE, YAXIS, 500000.0,
	    NullSetup, DoDesignSimulationMemristorChaoticCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Design of an Interface for Random Number Generators - Labyrinth Chaotic System",
	    "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.5, -12.5, 20.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDesignInterfaceRandomNumberGenerators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 1.0, 1.0
	},
	{
	"Design of Fixed Point-Based Dual Entropy Core Chaotic True Random Number Generator on FPGA",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.5, -9.25, 14.2, 5, 6, 4, 2, 3, 3, NULL, 1 + AVERAGE, YAXIS, 100000.0,
	    NullSetup, DoDesignFixedPointBasedDualEntropyCore, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Design of Multi-Wing 3D Chaotic Systems with only Stable Equilibria or No Equilibrium Point (System 11) I",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.1236, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.5, -10.0, 21.0, 1, 2, 3, 1, 3, 3, NULL, 1 + AVERAGE, YAXIS, 250000.0,
	    NullSetup, DoDesignMultiWing3DChaoticSystems11, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Design of Multi-Wing 3D Chaotic Systems with only Stable Equilibria or No Equilibrium Point (System 11) II",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, -1.1236, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -2.8, 5.5, 1, 2, 3, 1, 3, 3, NULL, 1 + AVERAGE, YAXIS, 250000.0,
	    NullSetup, DoDesignMultiWing3DChaoticSystems11, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Design of Multi-Wing 3D Chaotic Systems with only Stable Equilibria or No Equilibrium Point (System 15) I",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.1236, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.5, -5.7, 13.0, 1, 2, 3, 1, 3, 3, NULL, 1 + AVERAGE, YAXIS, 250000.0,
	    NullSetup, DoDesignMultiWing3DChaoticSystems15, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Design of Multi-Wing 3D Chaotic Systems with only Stable Equilibria or No Equilibrium Point (System 15) II",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, -1.1236, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.7, 3.6, 1, 2, 3, 1, 3, 3, NULL, 1 + AVERAGE, YAXIS, 250000.0,
	    NullSetup, DoDesignMultiWing3DChaoticSystems15, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Design of Multiwing-Multiscroll 3D Chaotic Systems using Rotation Symmetry - Equation 21",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.7, -2.4, 4.3, 4, 5, 6, 1, 3, 3, NULL, 1 + AVERAGE, YAXIS, 100000.0,
	    NullSetup, DoDesignMultiwingMultiscroll3DChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	},
	{
	"Design of Multiwing-Multiscroll Grid Compound Chaotic System and its Circuit Implementation", 
	    "c", "beta", "p", "l1", "l2", "l3", "a1", "a2", "z0", "f0", 1.0, 0.75, 20.0, 1.1, 2.4, 3.5, 2.1, 1.1, 3.5, 400.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 5, 6, 4, 10, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDesignMultiwingMultiscrollGridCompoundChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Designing Cryptosystems Based on Hyperchaos", "a", "b", "k", "e", ES, ES, ES, ES, ES, ES, 
	    3.8, 0.2, 0.05, -0.17, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.07, 2.222, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.4, 7.0, 2, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDesigningCryptosystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Detecting Chaos in a Complex System", "m", "n", "p", "rm", ES, ES, ES, ES, ES, ES, 2.0, 1.0, 10.0, 56.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", "x6", "x7", ES, ES, ES, -6.0, 20.0, -15.0, 10.0, 5.0, 15.0, 16.0, 0.0, 0.0, 0.0,
	    -750.0, -900.0, 1000.0, 6, 7, 5, 4, 7, 7, NULL, 1, YAXIS, 40000.0,
	    NullSetup, DoDetectingChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Deterministic Chaos Of A Spherical Pendulum Under Limited Excitation I", "c", "d", "f", "e", ES, ES, ES, ES, ES, ES, -0.5, -1.0, 0.5, -1.42, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "y1", "y2", "y3", "y4", "y5", ES, ES, ES, ES, ES, 1.0, 2.0, 3.0, -10.0, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -90.0, -50.0, 100.0, 1, 2, 4, 4, 5, 5, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDeterministicChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Deterministic Chaos Of A Spherical Pendulum Under Limited Excitation II", "c", "d", "f", "e", ES, ES, ES, ES, ES, ES, -0.6, -1.0, 0.5, -1.42, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "y1", "y2", "y3", "y4", "y5", ES, ES, ES, ES, ES, 1.0, 2.0, 3.0, -10.0, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -18.0, 36.0, 1, 2, 4, 4, 5, 5, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDeterministicChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Deterministic Chaos vs. Stochastic Fluctuation in an Eco-epidemic Model", "k", "lambda", "mu", "m", "a", "theta", "delta", "r", ES, ES, 400.0, 0.06, 3.4, 15.5, 15.0, 10.0, 8.3, 24.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 200.0, 150.0, 40.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    9.6, 25.0, 226.0, 1, 3, 2, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDeterministicChaosStochasticFluctuation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00025, 3.0, 1.0
	},
	{
	"Determining the Chaotic Behavior in a Fractional-Order Finance System with Negative Parameters", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.005, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.4, 2.8, 2, 4, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDeterminingChaoticBehaviorFractionalOrderFinanceSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00008, 0.5, 1.0
	},
	{
	"Deterministic Nonideal Hydrodynamic Systems I", "azao", "b", "mu", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.5, -1.0, -0.3131, -0.045, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 4, 6, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDeterministicHydrodynamic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Deterministic Nonideal Hydrodynamic Systems II", "azao", "b", "mu", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.5, -1.0, -0.3131, -0.045, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, -3.0, 6.0, 1, 5, 4, 6, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDeterministicHydrodynamic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Deterministic Nonideal Hydrodynamic Systems III", "azao", "b", "mu", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.5, -1.0, -0.3131, -0.045, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 2, 4, 3, 6, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDeterministicHydrodynamic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Deterministic Nonideal Hydrodynamic Systems IV", "azao", "b", "mu", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.5, -1.0, -0.3131, -0.045, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, -3.0, 6.0, 4, 5, 3, 6, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDeterministicHydrodynamic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dichotomy of nonlinear systems: Application to chaos control of nonlinear electronic circuit", "alpha", "beta", "gamma", "m0", "m1", "k", ES, ES, ES, ES, -1.301, -0.0136, -0.02969, 0.1690, -0.4767, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.5, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -9.0, 18.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS+EVOLVE, 1000000.0,
	    NullSetup, DoDichotomyNonlinear, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 22.0, 1.0
	},
	{
	"Different 5D Lorenz Look Alike I", "sigma", "r", "b", "k", "d", "delta", ES, ES, ES, ES, 
	    10.0, 28.0, 8.0/3.0, 10.0, 2.6, 0.4, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -35.0, 60.0, 1, 2, 3, 6, 5, 5, NULL, 1, YAXIS, 80000.0,
	    NullSetup, DoDifferent5DLorenzLookAlike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Different 5D Lorenz Look Alike II", "sigma", "r", "b", "k", "d", "delta", ES, ES, ES, ES, 
	    10.0, 28.0, 8.0/3.0, 10.0, 2.6, 0.4, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -200.0, 600.0, 4, 5, 3, 6, 5, 5, NULL, 1, YAXIS, 140000.0,
	    NullSetup, DoDifferent5DLorenzLookAlike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 3.0, 1.0
	},
	{
	"Different Chaos - Marcus Rezende 2015", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.66, 0.201, 0.165, 21.28, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -2.0, 4.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDifferentChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 1.0, 1.0
	},
	{
	"Different Families of Hidden Attractors in a New Chaotic System with Variable Equilibrium", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.23, -0.18, 0.3, 5, 6, 2, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoDifferentFamiliesHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Different Pairs Of Synchronized Chaotic Systems", "ni", "s", "b", ES, ES, ES, ES, ES, ES, ES, 100.0, 0.35, 300.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.15, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 5000000.0,
	    NullSetup, DoDifferentPairs, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 3.0, 1.0
	},
	{
	"Dimensionality Reduction Reconstitution for Extreme Multistability in Memristor-Based Colpitts System", "a", "b", "alpha", "beta", ES, ES, ES, ES, ES, ES, 5.2, 0.9, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.4, 3, 4, 2, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDimensionalityReductionReconstitutionExtremeMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 0.1
	},
	{
	"Different Oscillator with Restricted Bifurcations", "a", "b", "d", "mi", ES, ES, ES, ES, ES, ES, 4.0, 3.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -40.0, 50.0, 1, 2, 3, 4, 3, 3, NULL, 0, YAXIS, 400.0,
	    NullSetup, DoDifferent, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 0.3, 1.0
	},
	{
	"Dimensions of an Attractor", "r1", "r2", "e", "sigma", "b", ES, ES, ES, ES, ES, 60.0, 0.02, 0.06, 2.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "y1", "x2", "y2", "z", ES, ES, ES, ES, ES, 10.0, -2.0, 32.0, -12.0, 76.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 5, 5, 5, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoDimensions, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Discontinuity Induced Hopf And Neimark-Sacker Bifurcations", "beta", "f", "omega", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.24, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.01, 1.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.4, 2.7, 1, 6, 3, 3, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDiscontinuityInducedBifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Dissipative & Conservative Chaotic Nature of a New Quasi-Periodically Forced Oscillator with Megastability", "a1", "b1", "a", "b", ES, ES, ES, ES, ES, ES, 1.0, 1.0, sqrt(2.0), sqrt(3.0), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.8, 3.6, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDissipativeConservativeChaoticNatureQuasiPeriodicallyForcedOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.01
	},
	{
	"Dissipative Chaos", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 12.0, 17.0, 29.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    4.0, 4.0, 5.0, 1, 2, 3, 3, 3, 3, NULL, 1, ZAXIS, 40000.0,
	    NullSetup, DoDissipativeChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.3, 1.0
	},
	{
	"Dixon System from the Elegant Chaos Book", "real", "imag", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, -0.6, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.66, -0.32, 0.82, 4, 5, 6, 2, 3, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDixonSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 0.6
	},
	{
	"Do We need More Chaos Examples? Julian Sprott", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.32, -1.9, 4.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoDoWeneedMoreChaosExamples, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.6, 1.0
	},
	{
	"Dormand-Prince the Eldest of Chaotic Oscillator Design of on the FPGA Implementation", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -10.0, 16.0, 5, 6, 4, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDormandPrinceEldest, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.6
	},
	{
	"Double Attractor", "r", "a", "b", ES, ES, ES, ES, ES, ES, ES, 4.9, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.48, -0.06, 0.12, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS+EVOLVE, 50000.0,
	    NullSetup, DoDoubleAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 0.0001, 1.0
	},
	{
	"Double-Well Duffing System I", "a", "k", ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.14, 0.109, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 2, 3, 4, 2, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDoubleWell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Double-Well Duffing System II", "a", "k", ES, ES, ES, ES, ES, ES, ES, ES, 0.1, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.14, 0.109, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.5, 5.0, 2, 3, 4, 2, 4, 4, NULL, 0, XAXIS, 100000.0,
	    NullSetup, DoDoubleWell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Drive-Response Synchronisation of a Fractional-Order Hyperchaotic System and Its Circuit Implementation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.5, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -2.0, -0.5, -1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -14.0, 20.0, 1, 5, 4, 3, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDriveResponseSynchronisationFractionalOrderHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.2, 1.0
	},
	{
	"Duffings Oscillator 3D", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.01, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -5.0, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, XAXIS+YAXIS, 50000.0,
	    NullSetup, DoDuffings3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 1.0, 1.0
	},
	{
	"Dyadic Transformation with a modification I", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 38.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.8, 1.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 5000.0,
	    NullSetup, DoDyadicTransformationI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.001, 1.0
	},
	{
	"Dyadic Transformation with a modification II", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 38.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.36, -0.09, 0.16, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 5000.0,
	    NullSetup, DoDyadicTransformationII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.001, 1.0
	},
	{
	"Dynamic Analysis and Circuit Design of a Novel Hyperchaotic System with Fractional-Order Terms", "a", "b", "c", "f", "m", "r", "h", "k", ES, ES, 
	    0.93, 1.11, -0.11, -0.21, 6.26, 1.32, 0.001, 14.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.001, 1.0, 1.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -20.0, 40.0, 1, 8, 3, 8, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.08, 1.0
	},
	{
	"Dynamic Analysis and Synchronisation Control of an Unusual Chaotic System with Exponential Term", "m", "n", ES, ES, ES, ES, ES, ES, ES, ES, 
	    5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.0, 2.0, 1, 4, 3, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicAnalysisSynchronisationControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 0.5
	},
	{
	"Dynamical Analysis and Circuit Realisation of an Equilibrium Free 3D Chaotic System with a Many Attractors", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    0.52, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.1, 0.025, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicalAnalysisElectronicCircuitRealisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamic Analysis for a Fractional-Order Autonomous Chaotic System", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    0.4, 3.0, -1.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.25, 2.5, 1, 5, 3, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoDynamicAnalysisFractOrderAutonomousChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Dynamic Analyses, FPGA Implementation and Engineering Applications of Multi-Butterfly Chaotic Attractors", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "z", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.7, -2.0, 4.3, 1, 3, 2, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicAnalysesFPGAImplementationEngineeringApplications, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00725, 0.2, 1.0
	},
	{
	"Dynamic Behaviors Analysis of a Chaotic Circuit Based on a Novel Fractional-Order Generalised Memristor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	    0.9, 0.5, 0.005, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, -0.95, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.6, -8.83, 17.5, 1, 6, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicBehaviorsAnalysisChaoticCircui, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Dynamic Evolution Analysis of Stock Price Fluctuation and Its Control", "a0", "a1", "a2", "a3", "q", "w", ES, ES, ES, ES, 
	    0.0, 0.6, 0.5, -0.2, 0.8, 0.6, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -20.0, 90.0, 2, 5, 3, 6, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicEvolutionAnalysisStockPriceFluctuation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.15, 1.0
	},
	{
	"Dynamical Analysis of a Novel Fractional-Order Chaotic System Based on Memcapacitor and Meminductor", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES,
	    2.0, 20.0, 10.0, 10.0, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 5.0, 10.0, 0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -272.0, -158.0, 314.0, 1, 3, 2, 5, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicalAnalysisNovelFractionalOrderChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.1, 1.0
	},
	{
	"Dynamical Behavior of a 3D Jerk System with a Generalised Memristive Device", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 
	    0.5, 0.8, 0.6, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicalBehavior3DJerkSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Dynamical Behaviors, Circuit Realisation, Chaos Control & Sync of a New Fractional Order Chaotic System", "a1", "C1", "a2", "b2", "h1", "c2", "b3", "a3", "h2", "b1", 
	    5.9, -19.0, 7.82, -1.5, -8.5, 2.7, 7.825, 11.6, -5.731, 3.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.8, 1.6, 1, 4, 3, 10, 4, 4, NULL, 1, YAXIS, 170000.0,
	    NullSetup, DoDynamicalBehaviorsCircuitRealisationChaosControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 0.5
	},
	{
	"Dynamical Analysis of a Chaotic System with Two Double Scroll Attractors", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 4.975, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -6.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -2.0, 20.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicalAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.006, 1.0, 1.0
	},
	{
	"Dynamical Analysis of a Quadratic Chaotic Oscillator and Its Application in Biometric Fingerprint Encryption", "a", "b", "azao", ES, ES, ES, ES, ES, ES, ES,
	    0.1, 0.4, 5000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.7, -2.1, 4.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDynamicalAnalysisQuadraticMegastableChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Dynamical Study of a Novel 4D Hyperchaotic System: An Integer and Fractional Order Analysis", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES,
	    20.0, 20.0, 20.0, 20.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.94, -0.55, 1.0, 1, 3, 2, 5, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoDynamicalStudyNovel4DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamical Systems from Output Regular and Chaotic Signals", "c", "e", "f", "d", ES, ES, ES, ES, ES, ES, 
	    -0.1, -0.59, 0.19, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -12.0, 24.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDynamicalSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 8.0, 1.0
	},
	{
	"Dynamical System Generalized Nose-Hoover Oscillators with Temperature Gradient", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.16, -0.5, 1.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDynamicalNoseHoover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.2, 1.0
	},
	{
	"Dynamical Chaos and Uniformly Hyperbolic Attractors from Mathematics to Physics", 
	    "mu", "k", "delta", ES, ES, ES, ES, ES, ES, ES, 
	    0.02, -1.5, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -5.0, 8.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicalChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 10.0, 1.0
	},
	{
	"Dynamic Analysis and Circuit Design of a Novel Hyperchaotic System with Fractional-Order Terms - B", 
	    "a", "b", "c", "d", "m", "h", "k", ES, ES, ES, 1.0, 1.0, -0.11, -0.21, 5.0, 0.01, 14.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, -85.0, 170.0, 4, 7, 1, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicAnalysisCircuitDesign, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 6.0, 10.0
	},
	{
	"Dynamic Analysis and Electronic Circuit Implementation of a 3D Autonomous System Without Linear Terms",
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 3.0, -4.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -8.0, 9.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoDynamicAnalysisElectronicCircuitImplementation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Dynamics, Analysis and Implementation of a Multiscroll Memristor-Based Chaotic Circuit",
	    "a", "b", "c", "L1", "L2", "p", "r", ES, ES, ES, 0.1, 2.1, 100.0, 0.4, 1.25, 5.0, 0.01, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.0, 0.0551, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.1, -2.56, 4.5, 4, 7, 6, 7, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDynamicsAnalysisImplementationMultiscroll, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 0.8
	},
	{
	"Dynamics and Circuit Realisation of a No-equilibrium Chaotic System with a Boostable Variable", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 0.8, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -14.0, 18.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDynamicsCircuitRealization, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.2, 1.0
	},
	{
	"Dynamics and Complexity of a New 4D Chaotic Laser System", "sigma", "delta", "r", "b", ES, ES, ES, ES, ES, ES,
	    4.0, 0.5, 27.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 1.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -590.0, -300.0, 600.0, 6, 7, 8, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsComplexityNew4DChaoticLaserSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 25.0, 1.0
	},
	{
	"Dynamics and Entropy Analysis for a New 4-D Hyperchaotic System with Coexisting Hidden Attractors", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES,
	    0.05, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, -1.0, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoDynamicsEntropyAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 1.0, 1.0
	},
	{
	"Dynamics, FPGA Realisation & Application of a Chaotic System with Infinite Number of Equilibrium Points", "a", "h", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.1, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.2, 3.2, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoDynamicsFPGARealisationApplicationChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Dynamics and Control of Tokamak System with Symmetric and Magnetically Confined Plasma", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.8, 3.0, 5, 6, 4, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicsControlTokamaks, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamics and Synchronisation of a Memristor-Based Chaotic System with No Equilibrium", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 3.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.15, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.5, -4.2, 8.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicsSynchronisationMemristorBasedChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamics at Infinity and a Hopf Bifurcation Arising in a Quadratic System with Coexisting Attractors", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.9, 1.6, 3, 6, 1, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicsAtInfinityHopfBifurcation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.055, 0.3, 1.0
	},
	{
	"Dynamics, Control and Symmetry Breaking Aspects of an Infinite-Equilibrium Chaotic System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES,
	    0.28, 0.027, 4.0, 3.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.6, 1.2, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicsControlSymmetryBreaking, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 0.5
	},
	{
	"Dynamics Feature and Synchronisation of a Robust Fractional-Order Chaotic System", "a", "b", "c", "n", "m", ES, ES, ES, ES, ES, 
	    2.0, 1.0, 1.0, 30.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -520.0, -244.0, 600.0, 5, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsFeatureSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 5.0
	},
	{
	"Dynamics and Feedback Control for a New Hyperchaotic System", 
	    "a", "b", "c", "h", "f", "k", ES, ES, ES, ES, 20.0, 14.0, 10.6, 2.8, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -55.0, -30.0, 60.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsAndFeedback, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"Dynamics and Synchronisation of New Hyperchaotic Complex Lorenz System in 7 Dimensions I", 
	    "a", "b", "g", "k1", "k2", ES, ES, ES, ES, ES, 14.0, 3.0, 20.0, -5.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "q", ES, ES, ES, 0.1, 0.1, 0.1, 0.0, -0.5, 0.1, 0.1, 0.0, 0.0, 0.0,
	    -30.0, -18.0, 36.0, 1, 7, 5, 5, 7, 7, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoDynamicsAndSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.4, 1.0
	},
	{
	"Dynamics and Synchronisation of New Hyperchaotic Complex Lorenz System in 7 Dimensions II", 
	    "a", "b", "g", "k1", "k2", ES, ES, ES, ES, ES, 14.0, 3.0, 20.0, -5.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "q", ES, ES, ES, 0.1, 0.1, 0.1, 0.0, -0.5, 0.1, 0.1, 0.0, 0.0, 0.0,
	    -30.0, -6.0, 36.0, 1, 5, 4, 5, 7, 7, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoDynamicsAndSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.8, 1.0
	},
	{
	"Dynamics and Synchronisation of New Hyperchaotic Complex Lorenz System in 7 Dimensions III", 
	    "a", "b", "g", "k1", "k2", ES, ES, ES, ES, ES, 14.0, 3.0, 150.0, -5.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "q", ES, ES, ES, 0.1, 0.1, 0.1, 0.0, -0.5, 0.1, 0.1, 0.0, 0.0, 0.0,
	    -130.0, -80.0, 160.0, 1, 4, 5, 5, 7, 7, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoDynamicsAndSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.8, 1.0
	},
	{
	"Dynamics and Synchronisation of New Hyperchaotic Complex Lorenz System in 7 Dimensions IV", 
	    "a", "b", "g", "k1", "k2", ES, ES, ES, ES, ES, 14.0, 3.0, 150.0, -5.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "q", ES, ES, ES, 0.1, 0.1, 0.1, 0.0, -0.5, 0.1, 0.1, 0.0, 0.0, 0.0,
	    -85.0, -50.0, 100.0, 2, 3, 1, 5, 7, 7, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoDynamicsAndSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.4, 1.0
	},
	{
	"Dynamics of an Airfoil with Higher-Order Plunge and Pitch Stiffnesses in Incompressible Flow",
	    "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 7.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.0001, 0.1, 0.00001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.78, -0.55, 0.9, 2, 3, 4, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoDynamicsAirfoil, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00075, 1.0, 1.0
	},
	{
	"Dynamics, Chaos Control and Synchronisation in a Fractional-Order Samardzija-Greller Population System", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    0.8, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.7, 0.25, 0.251, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.2, 4.0, 2, 3, 1, 3, 3, 3, NULL, 1+AVERAGE, YAXIS, 300000.0,
	    NullSetup, DoDynamicsChaosControlSynchronisationFractionalOrder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.1, 0.1
	},
	{
	"Dynamics, Circuit Design, and Synchronisation of a New Chaotic System with Closed Curve Equilibrium-II", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    4.1, 2.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.2, 2.4, 1, 5, 2, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDynamicsCircuitDesignSynchronization, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.06, 1.0
	},
	{
	"Dynamics and Circuit of a Chaotic System with a Curve of Equilibrium Points", "a", "b", "c", "d", "error != 0", ES, ES, ES, ES, ES, 
	    0.28, 10.0, 0.1, 2.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 4, 5, 6, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDynamicsCircuitChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamics and Circuit of a Chaotic System with a Curve of Equilibrium Points (with error)", "a", "b", "c", "d", "error != 0", ES, ES, ES, ES, ES, 
	    0.28, 10.0, 0.1, 2.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.6, 3.0, 1, 5, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsCircuitChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamics of a Cubic Nonlinear System with No Equilibrium Point", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    10.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -10.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.2, -2.40, 4.7, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDynamicsCubicNonlinearSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.1, 1.0
	},
	{
	"Dynamics of a New Composite Four - Scroll Chaotic System with Complex Dynamics", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 
	    6.8, 15.1, 0.6, -1.1, 0.45, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.3, 2.6, 1, 6, 2, 1, 3, 3, NULL, 1, YAXIS, 150000.0,
	    NullSetup, DoDynamicsANewCompositeFourscrollChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.1, 1.0
	},
	{
	"Dynamics of a New Hyperchaotic System with Only One Equilibrium Point", "a", "b", "c", "d", "e", "alpha", "beta", ES, ES, ES, 
	    10.0, 2.6666667, 28.0, 2.0, 12.0, 4.0, 0.01, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.2, 0.1, 0.18, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -1.5, 15.0, 1, 3, 2, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsNewHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Dynamics of a New Three-Dimensional Chaotic Systems with Multiple Attractors", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES,
	    0.7303, 0.48867, 0.5785, 0.2373, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.2, 0.1, 0.18, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.4, -3.42, 7.4, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsNew3DChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Dynamics of Coupled Nonlinear Oscillators of Different Attractors", "mi", "alpha", "k", ES, ES, ES, ES, ES, ES, ES, 
	    0.8, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.8, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.2, 4.0, 3, 4, 1, 3, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoDynamicsCoupledOscillators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Dynamics of a Nonautonomous Predator-Prey Model with Infertility Control in the Prey", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.014, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -130.0, -20.0, 400.0, 1, 3, 5, 0, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDynamicsNonautonomous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Dynamics of Impurities in a Three-Dimensional Volume-Preserving Map", "a", "b", "c", "Pi approx", "Use integer MOD? yes=1,no=0", ES, ES, ES, ES, ES, 
	    2.0, 1.5, 0.16, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.0, 8.0, 4, 5, 6, 5, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoDynamicsOfImpurities, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Dynamics, Sync & Electronic Implementations of a New Lorenz Chaotic System with Nonhyperbolic Equilibria", "a", "c", ES, ES, ES, ES, ES, ES, ES, ES,
	1.4, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-18.0, -3.6, 20.0, 5, 6, 4, 2, 3, 3, NULL, 0, YAXIS, 500000.0,
	NullSetup, DoDynamicsSynchronizationElectronicImplementations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00085, 1.0, 1.0
	},
	{
	"Economic Systems With Fractional Order Dynamics", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    3.0, 0.01, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -2.0, 4.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoEconomicSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Effect of Market Confidence on a Financial System from the Perspective of Calculus: Circuit Realisation", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.4, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.92, -0.32, 0.73, 3, 4, 2, 0, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoEffectMarketConfidenceFinancialSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.3, 0.1
	},
	{
	"Effects of Padé Numerical Integration in Simulation of Conservative Chaotic Systems", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES,
	    0.7, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.00001, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.59, -4.13, 8.53, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoEffectsPadeNumericalIntegration, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Effect of Symmetry Breaking in an Inertial Neural System with a Non-Monotonic Activation Function", "c1", "c2", "b", "k1", "k2", ES, ES, ES, ES, ES,
	    5.0, 15.48, 0.035, 1.0, 1.6, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 1.0, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.44, -1.38, 3.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 70000.0,
	    NullSetup, DoEffectSymmetryBreakingInertialNeuralSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.15, 1.0
	},
	{
	"Eight Term Novel Four Scroll Chaotic System with Cubic Nonlinearity and its Circuit Simulation", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 3.0, 14.0, 3.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.4, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoEightTermFourScrollChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 3.0, 1.0
	},
	{
	"Eight Wing Chaotic Attractor", "a", "b", "c", "d", "m", "k", "w", ES, ES, ES, 
	    14.0, 43.0, -1.0, 16.0, 9.0, 10.0, PI / 25.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -360.0, -200.0, 400.0, 1, 2, 3, 7, 3, 3, NULL, 1, XAXIS+YAXIS, 500000.0,
	    NullSetup, DoEightWing, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 3.0, 1.0
	},
	{
	"Electronic Circuit of Freire et al", "ni", "beta", "gamma", "r", "a3", "b3", ES, ES, ES, ES, 
	    -0.721309, 0.6, -0.1, 0.6, 0.328578, .933578, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.85, -0.5, 1.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoElectronicCctFreire, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
/*
	{
	"Elegant Chaos Equation 1.3", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 2, 3, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoElegantChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.03
	},
*/
	{
	"Elegant Chaos in Fractional-Order System without Equilibria", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    -0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 3.0, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 12.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoElegantChaosFractionalOrderSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 3.0, 1.0
	},
	{
	"Elegant Chaos, from Elegant Chaos Book - VF3", "real", "imag", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.021, 2.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -8.0, 16.0, 4, 5, 6, 2, 3, 2, NULL, 1, YAXIS, 1500000.0,
	    NullSetup, DoElegantChaosVF3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00125, 1.0, 1.0
	},
	{
	"Elementary Quadratic Chaotic Flows with No Equilibria - NE - 16", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -125.0, -66.0, 132.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoElementaryQuadraticChaoticFlows, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000275, 1.0, 1.0
	},
	{
	"Elhadj-Sprott Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 60.0, 33.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.0, 50.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoElhadjSprott, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Elhadj-Sprott-1 Chaotic Attractor", "alpha", "beta", "a", "m0", "m1", ES, ES, ES, ES, ES, 7.0, 14.0, 1.0, -0.43, 0.41, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, 0.25, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 5, 3, 3, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoElhadjSprott1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005,1.0, 1.0
	},
	{
	"Emergence of a Square Chaotic Attractor through the Collision of Heteroclinic Two Orbits", "c1", "c2", "s1", "s2", "w", ES, ES, ES, ES, ES, 0.2, 0.25, 0.2, 0.15, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.0002, -0.00008, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.0, 2.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 400000.0,
	    NullSetup, DoEmergenceSquareChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005,1.0, 1.0
	},
	{
	"Empirical Nonlinear Model", "alpha", "beta", "gamma", "m0", "m1", "d", "k", ES, ES, ES, 3.0, 30.0, -0.86, -3.0, 0.4, 3.0, 0.2, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -14.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -14.0, 28.0, 1, 2, 3, 7, 3, 3, NULL, 0, YAXIS+EVOLVE, 50000.0,
	    NullSetup, DoEmpiricalNonlinear, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.1, 1.0
	},
	{
	"Emulating Complex Business Cycles by Using an Electronic Analogue", 
	    "k", "mu", "b", "p", "q", "s", ES, ES, ES, ES, 0.2, 0.4, 0.2, 10.0, 0.1, 310.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.4, -0.25, 0.5, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoEmulatingComplexBusiness, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00007, 0.01, 1.0
	},
	{
	"Energy Feed Back and Synchronous Dynamics of Hindmarsh-Rose Neuron Model with Memristor",
	    "a", "b", "errao", "r", "i", "xc", "alpha", "beta", "gamma", "k1", 3.0, 5.0, 4.0, 0.006, 3.1, -1.61, 0.1, 1.0, 1.0, 0.8,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -3.0, 3.5, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoEnergyFeedBackSynchronousDynamicsHindmarshRoseNeuronModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Energy Resource System with Constraint", "m", "n", "a1", "a2", "b1", "b2", "b3", "c1", "c2", "c3", 1.8, 1.0, 0.09, 0.15, 0.06, 0.082, 0.07, 0.2, 0.5, 0.4,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.82, 0.29, 0.48, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -20.0, 24.0, 1, 2, 3, 10, 3, 3, NULL, 1, XAXIS, 1000000.0,
	    NullSetup, DoEnergyResource, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.6, 1.0
	},
	{
	"Energy Stochastic System in 4 Dimensions", "m", "n", "a1", "a2", "b1", "b2", "b3", "c1", "c2", "c3", 2.5, 1.0, 0.1, 0.15, 0.06, 0.082, 0.07, 0.2, 0.5, 0.4,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, 0.82, 0.29, 0.48, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -200.0, -200.0, 240.0, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoEnergyStochastic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 0.1, 1.0
	},
	{
	"Entanglement Function Hopf Bifurcation Analysis in a New Chaotic System", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 
	    -1.0, 1.0, -1.0, 36.0, 18.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.82, 0.29, 0.48, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoEntangled, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.6, 1.0
	},
	{
	"Entropy Analysis and Image Encryption Application Based on a New Chaotic System Crossing a Cylinder", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.29, -2.0, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -0.75, 1.5, 2, 6, 3, 1, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoEntropyAnalysisImageEncryptionApplication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 0.12, 0.1
	},
	{
	"Entropy Analysis and Neural Network-based Adaptive Control of a Non-Equilibrium 4D Chaotic System", "a", "b", "c", "e", "f", "g", ES, ES, ES, ES,
	    1.05, 0.7, 0.19, 1.37, 1.79, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "z", ES, ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, -1.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, -11.0, 22.0, 2, 8, 3, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoEntropyAnalysisNeuralNetworkBasedAdaptiveControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.03, 0.1
	},
	{
	"Equivalent Circuit in Function and Topology to Chua’s Circuit and the Design Methods of these Circuits", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 
	    1.37, 0.5, 5.0, 6.67, 2.33, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 4, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoEquivalentCircuitChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Ergodic Time-Reversible Chaos for Gibbs' Canonical Oscillator I", "r", "ni", "type = 0, 1 or 2", ES, ES, ES, ES, ES, ES, ES, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 7.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoErgodicTimeReversible, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.2, 1.0
	},
	{
	"Ergodic Time-Reversible Chaos for Gibbs' Canonical Oscillator II", "r", "ni", "type = 0, 1 or 2", ES, ES, ES, ES, ES, ES, ES, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.5, 5.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoErgodicTimeReversible, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Ergodic Time-Reversible Chaos for Gibbs' Canonical Oscillator III", "r", "ni", "type = 0, 1 or 2", ES, ES, ES, ES, ES, ES, ES, 5.0, 5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -40.0, 100.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoErgodicTimeReversible, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Ergodic Time-Reversible Chaos for Gibbs' Canonical Oscillator IV", "r", "ni", "type = 0, 1 or 2", ES, ES, ES, ES, ES, ES, ES, 5.0, 5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -16.0, 36.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoErgodicTimeReversible, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Evolving Chaos: Identifying New Attractors of the Generalised Lorenz Family - XY21", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 27.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -10.0, 50.0, 1, 6, 2, 1, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoEvolvingChaosLorenzXY21, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.03, 1.0
	},
	{
	"Evolving Chaos: Identifying New Attractors of the Generalised Lorenz Family - XZ11", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.2154, 0.45577, 2.6667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -384.0, -186.0, 450.0, 4, 6, 5, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoEvolvingChaosLorenzXZ11, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Evolving Chaos: Identifying New Attractors of the Generalised Lorenz Family - XYZ4", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 7.11111, 1.66667, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 3, 4, 5, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoEvolvingChaosLorenzXYZ4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.01, 1.0
	},
	{
	"Evolving Chaos: Identifying New Attractors of the Generalised Lorenz Family - YZ76", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 28.0, 4.1059, 2.66667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -84.0, -25.0, 110.0, 2, 6, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoEvolvingChaosLorenzYZ76, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Existence and Stability of Chimera States in a Minimal System of Phase Oscillators", "p", "a", "b", ES, ES, ES, ES, ES, ES, ES, 0.3, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -1.9, 3.8, 4, 5, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoExistenceStabilityChimeraStates, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 1.0, 0.01
	},
	{
	"Experimental Study of the Dynamic Behavior of a Double Scroll Circuit", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 0.5, 0.95, 1.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.7, -3.1, 6.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoExperimentalStudyDynamicBehaviorDoubleScrollCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.5, 1.0
	},
	{
	"Experimental Verification of Volt-Ampere Characteristic Curve for a Memristor-Based Chaotic Circuit", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.64, -1.9, 3.8, 2, 4, 3, 0, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoExperimentalVerificationVoltAmpereCharacteristicCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.5, 0.2
	},
	{
	"Exponential Chen Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    35.0, 3.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -4.0, 20.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoExponentialChen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.1, 1.0
	},
	{
	"Extreme Multistability Analysis of Memristor-Based Chaotic System and its Application in Image Decryption", "a", "b", "c", "m", "n", ES, ES, ES, ES, ES,
	    9.0, 30.0, 17.0, -1.2, 1.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 8, 9, 10, 5, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoExtremeMultistabilityAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015151, 0.1, 1.0
	},
	{
	"Extreme Multi-Stability Analysis of a Novel 5D Chaotic System and its Secure Communication Scheme", "a", "start", ES, ES, ES, ES, ES, ES, ES, ES,
	    1.0, 80000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, -1.44, 0.56, -0.822, -1.62, -0.74, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.0, 2.0, 3, 4, 5, 2, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoExtremeMultistabilityAnalysis5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 0.5, 1.0
	},
	{
	"Extreme Multistability and Complex Dynamics of a Memristor-Based Chaotic System", "a", "b", "c", "r", ES, ES, ES, ES, ES, ES,
	    1.775, 1.4, 12.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -2.7, 5.3, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 900000.0,
	    NullSetup, DoExtremeMultistabilityComplexDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00017985, 0.6, 0.2
	},
	{
	"Extreme Multistability in Hyperjerk Memristive System with Hidden Attractors 2D Version", "f", "a", "p", ES, ES, ES, ES, ES, ES, ES, 
	    0.2, 1.5, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.24, 2.0, 3, 4, 2, 3, 2, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoExtremeMultiStabilityHyperjerkMemristiveSystem2D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 8.0, 1.0
	},
	{
	"Extreme Multistability in Hyperjerk Memristive System with Hidden Attractors and its Adaptive Sync Scheme", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES,
	    0.52, 0.22, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 8, 2, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoExtremeMultiStabilityHyperjerkMemristiveSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.6, 1.0
	},
	{
	"Extremely Rich Dynamics from Hyperchaotic Hopeld Neural Network: Analog Circuit Implementation", "w22", "w33", "w44", ES, ES, ES, ES, ES, ES, ES,
	    1.5, 1.0, 170.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 1.68, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.35, -1.1, 2.88, 1, 2, 3, 3, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoExtremelyRichDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Fast Adaptive Image Encryption using Chaos by Dynamic State Variables Selection", 
	    "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 91.7, 5.023, 0.01, 91.0, 87.001, 18.0, 9.5072, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -72.0, -40.0, 80.0, 2, 4, 3, 7, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoFastAdaptive, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Feigenbaum-Like Chaotic 3D System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    -20.0, 1.0, 1.58, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -3.5, 7.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFeigenbaumLike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.0, 1.0
	},
	{
	"Field Coupling Synchronisation between Chaotic Circuits via a Memristor", "a", "b", "k", ES, ES, ES, ES, ES, ES, ES, 0.006, 0.001, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", "u", "v", "w", "r", ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.0, 0.0, 0.0,
	-4.36, -2.15, 4.65, 2, 3, 6, 3, 7, 7, NULL, 1, YAXIS, 180000.0,
	NullSetup, DoFieldCouplingSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 1.0, 1.0
	},
	{
	"Finance Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.002, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -10.0, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoFinance, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 1.0, 1.0
	},
	{
	"Finite-Time Observer for Nonlinear Systems Applications to Synchronization of Lorenz-Like Systems", "a", "b", "c", "alpha", "beta", ES, ES, ES, ES, ES, 
	    6.0, 1.2, 2.5, 91.0, -1.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 5.0, 4.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -10.0, 20.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoFiniteTimeObserver, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.15, 1.0
	},
	{
	"Finite Time Stabilization", "a1", "a2", "a3", "k", ES, ES, ES, ES, ES, ES, -6.8, -3.9, -1.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFiniteTime, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"FitzHugh-Nagumo Model", "step size", "upper limit", "z multiplier", ES, ES, ES, ES, ES, ES, ES, 0.1, 3.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.15, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.0, 5.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFitzHughNagumoe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.2, 1.0
	},
	{
	"Five-Dimensional Hyperchaotic System and its Circuit Simulation by EWB I", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    37.0, 14.5, 10.5, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 1.0, 2.0, 6.0, -6.1, -9.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -45.0, 50.0, 1, 5, 3, 4, 5, 5, NULL, 0, XAXIS, 50000.0,
	    NullSetup, DoFiveDCircuitSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.0, 1.0
	},
	{
	"Five-Dimensional Hyperchaotic System and its Circuit Simulation by EWB II", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    37.0, 14.5, 10.5, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 7.0, 5.0, 7.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, 5.0, 50.0, 1, 3, 5, 4, 5, 5, NULL, 1, YAXIS+EVOLVE, 2000000.0,
	    NullSetup, DoFiveDCircuitSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0000075, 10.0, 1.0
	},
	{
	"Five-Dimensional Hyperchaotic System and its Circuit Simulation by EWB III", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    37.0, 14.5, 10.5, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 7.0, 5.0, 7.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -25.0, 50.0, 1, 4, 5, 4, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoFiveDCircuitSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0000075, 1.0, 1.0
	},
	{
	"Five-Dimensional Hyperchaotic System and its Circuit Simulation by EWB IV", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    37.0, 14.5, 10.5, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 7.0, 5.0, 7.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -20.0, 40.0, 2, 4, 3, 4, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoFiveDCircuitSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0000075, 0.6, 1.0
	},
	{
	"Five-Dimensional Hyperchaotic System and its Circuit Simulation by EWB V", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    37.0, 14.5, 10.5, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 7.0, 5.0, 7.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -1.5, 3.0, 3, 4, 5, 4, 5, 5, NULL, 1, YAXIS+EVOLVE, 1800000.0,
	    NullSetup, DoFiveDCircuitSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0000075, 0.05, 1.0
	},
	{
	"Five-Dimensional Hyperchaotic System and its Circuit Simulation by EWB VI", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES, 
	    37.0, 14.5, 10.5, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 7.0, 5.0, 7.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -20.0, 40.0, 4, 5, 3, 4, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoFiveDCircuitSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0000075, 0.6, 1.0
	},
	{
	"Five-Dimensional Lorenz-Like System", "le", "rs", "r", "pr", "lambda", ES, ES, ES, ES, ES, 0.1, 35.0, 250.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "w", ES, ES, ES, ES, ES, 0.9, 0.9, 0.9, 0.9, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.25, -0.05, 0.8, 3, 5, 2, 5, 5, 5, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoFiveDLorenzLike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 3.0, 1.0
	},
	{
	"Fluid Instabilities", "h", "w", "alpha", "m", "b", "l", "k", ES, ES, ES, 0.18, 0.25, 1.0 / 3.5, 1.75, 1.14, 8.0, 5.5, 0.0, 0.0, 0.0,
	    "p", "f", "a", ES, ES, ES, ES, ES, ES, ES, 0.66, 0.5, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.2, -0.2, 0.5, 1, 3, 2, 7, 3, 3, NULL, 1, YAXIS, 30000.0,
	    NullSetup, DoFluidInstabilities, "SelectOscParams", (DLGPROC)SelectOscParams, 0.9, 0.1, 1.0
	},
	{
	"Flux of Fluids Chaotic Attractor in 5 Dimensions", "re (number of Reynolds)", ES, ES, ES, ES, ES, ES, ES, ES, ES, 33.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "q", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -5.0, 10.0, 3, 2, 1, 1, 5, 5, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoFluxOfFluids5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Forced Synchronisation of a Self-Sustained Chaotic Oscillator", 
	    "delta", "alpha", "epsilon", "gamma", "sigma", ES, ES, ES, ES, ES, 0.43, 33.0, 0.603, 0.1, 0.72, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, -0.7, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.5, 3.0, 2, 5, 1, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoForcedSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Four-Dimensional Autonomous Dynamical Systems with Conservative Flows - Two-Case Study", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.0, 5.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "z", ES, ES, ES, ES, ES, ES, 1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 4, 6, 1, 4, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoFourDimensionalAutonomousDynamicalSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"Four Memristor Chaotic Oscillators", "a1", "a2", "a3", "a4", "a5", ES, ES, ES, ES, ES, 16.4, 3.28, 19.7, 15.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoFourMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, -4.0, 1.0
	},
	{
	"Four-Wing Chaotic Attractor Generated from a New 3-D Quadratic Autonomous System", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 14.0, 43.0, -5.5, 16.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 25.7, -15.3, -9.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.0, 280.0, 2, 6, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFourWingChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.01, 1.0
	},
	{
	"Four-Wing Attractors in a Novel Chaotic System with Hyperbolic Sine Nonlinearity", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -10.0, 20.0, 1, 5, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFourWingAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.3, 1.0
	},
	{
	"Four Wing 1 Chaotic Attractor", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 4.0, 6.0, 10.0, 5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -56.0, -30.0, 70.0, 3, 2, 1, 5, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoFourWing1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Four Wing 2 Chaotic Attractor", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, -14.0, 5.0, 1.0, 16.0, -43.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 4.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0, -180.0, 360.0, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 20000.0,
	    NullSetup, DoFourWing2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Four Wing 3 Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -12.0, 20.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFourWing3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"FPGA Based Chaotic Oscillator Designs with Different Numerical Algorithms", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -3.2, 7.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFPGABasedChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"FPGA Implementation of Adaptive Sliding Mode Control and Genetically Optimised PID Control", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.1, 0.2, -0.2, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -60.0, 180.0, 6, 7, 8, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFPGAImplementationAdaptiveSlidingModeControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"FPGA Implementation of a Novel Chaotic Oscillator System With Odd and Even Symmetry", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.3, 1.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0002, 0.0002, -20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -39.0, -32.0, 42.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFPGAImplementationNovelChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.4, 1.0
	},
	{
	"FPGA Tabanh Farkh Numerik Algoritmalar Ile Kaotik Osilator Tasarimlari", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.9, -3.4, 7.5, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFPGATabanhFarkhNumerik, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00075, 1.0, 1.0
	},
	{
	"Fractional Order Biological Snap Oscillator: Analysis and Control", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 5.0, 24.0, 0.05, 2.001, 2.55, 1.7, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.2, 0.5, 0.51, 0.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.6, -3.0, 6.6, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFractionalOrderBiologicalSnapOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.5, 0.7
	},
	{
	"Fractional Order Form of a System with Stable Equilibria and its Synchronisation", "a", "b", "e", ES, ES, ES, ES, ES, ES, ES, 14.1, 15.0, 90.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.1, 5.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -75.0, -50.0, 100.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFractionalOrderFormSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Fractional Order-Induced Bifurcations in a Delayed Neural Network with Three Neurons", "k11", "k12", "k13", "k21", "k22", "k23", "k31", "k32", "k33", "L11", 0.6, -0.9, 2.9, -9.0, 0.9, -1.3, -1.2, 2.3, 0.7, 1.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -4.0, 8.3, 2, 3, 1, 10, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFractionalOrderInducedBifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.7, 1.0, 1.0
	},
	{
	"Fractional Order Memristor No Equilibrium Chaotic System with Its Adaptive Sliding Mode Synchronisation", "a", "b", "c", "a1", "b1", ES, ES, ES, ES, ES, 10.0, 9.0, 8.7, 4.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.7, -2.0, 4.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoFractionalOrderMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00008, 1.0, 1.0
	},
	{
	"Fractional-Order PWC Systems Without Zero Lyapunov Exponents", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.1, 0.2, -0.2, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.6, -5.0, 10.0, 6, 8, 7, 2, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoFractionalOrderPWCSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Fractional Order Simple Chaotic Oscillator with Saturable Reactors and Its Engineering Applications", "a1", "a2", "a3", "a4", "a5", "a6", "a7", ES, ES, ES, 0.043, 0.125, 0.375, 0.22, 0.01875, 0.00625, 0.03, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.36, -0.05, 0.42, 5, 6, 4, 7, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoFractionalOrderSimpleChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 10.0, 1.0
	},
	{
	"Fractional-Order Simplest Memristor-based Chaotic Circuit with New Derivative", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -2.0, 2.7, 5, 6, 4, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoFractionalOrderSimplestMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0068, 0.6, 1.0
	},
	{
	"Fractional Symbolic Network Entropy Analysis for the Fractional-Order Chaotic Systems", "e", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.9, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -5.4, -7.4, -13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -11.0, 18.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoFractionalSymbolicNetworkEntropyAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 1.0, 1.0
	},
	{
	"Frequency Limitations from the Circuit Realisation of Saw-Tooth Based Multi-Scroll Oscillators", 
	    "alpha", "beta", "gamma", "epsilon", "b", "p", "k", ES, ES, ES, 3.0, 4.0, 1.0, 0.8, 8.0, 5.0, 6.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1600.0, -850.0, 1700.0, 1, 4, 3, 7, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoFrequencyLimitationsCircuitRealisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Fully Integrated Memristor and Its Application on the Scroll-Controllable Hyperchaotic System", "beta", "alpha", "a", "b", ES, ES, ES, ES, ES, ES, 0.72, 1.0, 0.5, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 6, 7, 8, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFullyIntegratedMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Fuzzy Synchronisation for Fractional Order Chaos I", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 3.0, 51.0, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -62.0, -60.0, 70.0, 1, 4, 3, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFuzzySynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.01, 1.0
	},
	{
	"Fuzzy Synchronisation for Fractional Order Chaos II", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 3.0, 51.0, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -62.0, 70.0, 3, 4, 1, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFuzzySynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.01, 1.0
	},
	{
	"Gaussian Mixture Model Based Cost Function for Parameter Estimation of Chaotic Biological Systems", "ro", "i", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.007, 3.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.9696, -3.70669, 3.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -1.6, 2.0, 1, 2, 3, 2, 3, 3, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoGaussianMixture, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"Gene Networks", "r", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.9800459350, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.004, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.5, 1.2, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoGeneNetworks, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Generalised Combination Complex Synchronisation for Fractional-Order Chaotic Complex Systems", "a1", "a2", "a3", "a4", "a5", "b1", "b2", "b3", "b4", ES, 
	    35.0, 3.0, 12.0, 7.0, 0.5, 0.25, 3.0, 0.5, 0.05, 0.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", ES, ES, 3.0, -4.0, 2.0, 2.0, -15.0, 9.0, -4.0, 18.0, 0.0, 0.0,
	    -192.0, -75.0, 200.0, 1, 3, 8, 9, 8, 8, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoGenCombinationComplexSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0065, 1.0, 1.0
	},
	{
	"Generalised Dynamic Switched Synchronisation between Combinations of Fractional-Order Chaotic Systems", "a1", "a2", "a3", "b1", "b2", "b3", "c1", "c2", "c3", "d1", 
	    1.24, 1.0, 1.0, 1.1, 2.5, 0.1, 1.0, 5.0, 1.0, 4.4,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", ES, 0.1, 0.1, 0.1, 0.2, 0.0, 0.5, 2.0, -1.0, 1.0, 0.0,
	    -4.5, -3.0, 6.0, 1, 12, 7, 10, 9, 9, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoGenDynamicSwitchedSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0007, 1.0, 1.0
	},
	{
	"Generalised Polynomial Chaos for Analysing the Stability of Oscillators", "a", "b", "g", "k", ES, ES, ES, ES, ES, ES, 10.0, 0.5, 1.0, -100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.0122, -1.42, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.0, 2.0, 3, 1, 2, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoGenPolynomialChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 1.0, 1.0
	},
	{
	"Generalised Stability Theorem for Discrete-Time Nonautonomous Chaos System with Applications", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.32, 0.3, 0.5, -0.12, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 5, 7, 6, 0, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGeneralizedStabilityTheorem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.5, 1.0
	},
	{
	"Generated by the Second-Generation Current Conveyor Circuit", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES,
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 5.0, 2.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -8.0, 20.0, 5, 8, 6, 0, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoGeneratedSecondGenerationCurrentConveyorCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.1, 1.0
	},
	{
	"Multi-Folded Hidden Chua’s Attractors: Two-case study (Eq. 30)", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES,
	    8.45, 12.0, 0.3532, -1.1468, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.85, 0.00001, 1.6008, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.82, -3.33, 6.8, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoMultiFoldedHiddenChuasAttractors30, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multi-Folded Hidden Chua’s Attractors: Two-case study (Eq. 33)", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES,
	    8.45, 12.0, 0.3532, -1.1468, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.85, 0.00001, 1.6008, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.82, -3.33, 6.8, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoMultiFoldedHiddenChuasAttractors33, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Generation and Suppression of a New Hyperchaotic Nonlinear Model with Complex Variables", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	    22.0, 2.0, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 2 * sqrt(5.0), 0.1, 2 * sqrt(5.0), 0.2, 10.0, 0.3, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 2, 3, 3, 6, 6, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoGenerationSuppressionNewHyperchaoticNonlinearModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Generating Hidden Extreme Multistability in Memristive Chaotic Oscillator via Micro-Perturbation", "a", "b", "c", "d", "alpha", "beta", ES, ES, ES, ES, 
	    1.0, 5.0, 0.65, 0.001, -0.4, 0.8, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.35, 2.4, 1, 7, 6, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGeneratingHiddenExtremeMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 0.05, 1.0
	},
	{
	"Generating 3-Scroll Attractors from one Chua Circuit", "m0", "m1", "a", "alpha", "beta", ES, ES, ES, ES, ES, -0.43, 0.41, 1.0, 10.0, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoGenerating3Scroll, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00015, 5.0, 1.0
	},
	{
	"Generating Chaos with a Switching Piecewise-Linear Controller I", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.3, 20.0, -20.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoGeneratingChaosSwitchingPiecewiseLinearController, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Generating Chaos with a Switching Piecewise-Linear Controller II", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, -0.016, 5.0, -20.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoGeneratingChaosSwitchingPiecewiseLinearController, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Generating Chaos with a Switching Piecewise-Linear Controller III", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.1016, 5.0, -20.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoGeneratingChaosSwitchingPiecewiseLinearController, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Generating Chaos with a Switching Piecewise-Linear Controller IV", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.5, -1.0, -20.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoGeneratingChaosSwitchingPiecewiseLinearController, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Generating Chaos with a Switching Piecewise-Linear Controller V", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.5, -1.0, -20.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 4, 2, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoGeneratingChaosSwitchingPiecewiseLinearController, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Generating Multiscroll Chaotic Attractors - Theories, Methods And Applications", "alpha", "beta", "a", "b", "c", ES, ES, ES, ES, ES, 10.814, 14.0, 1.3, 0.11, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -14.0, 28.0, 1, 4, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGeneratingMultiscrollChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 20.0
	},
	{
	"Generating 1-4-Wing Hidden Attractors in a 4D No-Equilibrium Chaotic System with Extreme Multistability I", "a", "b", "c", "d", "e", 
	    ES, ES, ES, ES, ES, 10.0, 4.5, 1.0, 1.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -69.0, -72.0, 90.0, 2, 4, 1, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGeneratingOneToFourWingHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00051, 1.0, 1.0
	},
	{
	"Generating 1-4-Wing Hidden Attractors in a 4D No-Equilibrium Chaotic System with Extreme Multistability II", "a", "b", "c", "d", "e", 
	    ES, ES, ES, ES, ES, 10.0, 1.7, 1.0, 1.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -32.0, 64.0, 3, 7, 1, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGeneratingOneToFourWingHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0051, 0.1, 1.0
	},
	{
	"Generating 1-4-Wing Hidden Attractors in a 4D No-Equilibrium Chaotic System with Extreme Multistability III", "a", "b", "c", "d", "e", 
	    ES, ES, ES, ES, ES, 4.0, 6.0, 10.0, 5.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -40.0, 50.0, 2, 4, 1, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGeneratingOneToFourWingHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0006, 0.3, 1.0
	},
	{
	"Generating 1-4-Wing Hidden Attractors in a 4D No-Equilibrium Chaotic System with Extreme Multistability IV", "a", "b", "c", "d", "e", 
	    ES, ES, ES, ES, ES, 4.0, 6.0, 10.0, 5.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -115.0, -83.0, 130.0, 1, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGeneratingOneToFourWingHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0012, 1.0, 1.0
	},
	{
	"Generating Tri-Chaos Attractors with 3 +ve Lyapunov Exp in New 4 Order System via Linear Coupling I", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    91.7, 5.023, 0.01, 91.0, 87.001, 0.018, 9.5072, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.0, 2.0, 1, 3, 2, 7, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoGeneratingTriChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.0, 1.0
	},
	{
	"Generating Tri-Chaos Attractors with 3 +ve Lyapunov Exp in New 4 Order System via Linear Coupling II", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    91.7, 5.023, 0.01, 91.0, 87.001, 0.018, 9.5072, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.0, 2.0, 1, 4, 2, 7, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoGeneratingTriChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.25, 1.0
	},
	{
	"Generation of a Family of Fractional Order Hyper-Chaotic Multi-Scroll Attractors", "a1", "a2", "k", "mu", "p1", "p2", ES, ES, ES, ES,
	    1.0, 1.0, 1.0, 2.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -4.5, 9.0, 2, 4, 1, 6, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoGenerationFamilyFractionalOrderHyperChaoticMultiScrollAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0045, 1.0, 1.0
	},
	{
	"Generation of a Family of Fractional Order Hyper-Chaotic Multi-Scroll Attractors in 5D", "a1", "a2", "d1", "d2", "d3", "k1", "k2", "p1", "p2", "p3",
	    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 3.0, 2.0, 3.0,
	    "x", "y", "z", "u", "v", "par mu", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 2.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.93, 5.57, 4, 5, 3, 10, 5, 6, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoGenerationFamilyFractionalOrderHyperChaoticMultiScrollAttractors5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00215 , 1.0, 1.0
	},
	{
	"Genesio-Tesi Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.44, 1.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -1.0, 2.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGenesioTesi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.8, 1.0
	},
	{
	"Geometry, Surfaces, Curves, Polyhedra", "p step", "r step", "colour factor", ES, ES, ES, ES, ES, ES, ES, 0.001, 0.0015, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 3, 4, 4, NULL, 0, YAXIS, 100.0,
	    NullSetup, DoGeometrySurfacesCurvesPolyhedra, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.4, 1.0
	},
	{
	"Gissinger Chaotic Oscillator Function Projetive Synchronisation", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.119, 0.1, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -3.0, 6.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGissinger, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Global Analysis on n-scroll Chaotic Attractors of Modified Chua’s Circuit", 
	    "number of scrolls", "alpha", "beta", "a", "b", ES, ES, ES, ES, ES, 8.0, 10.814, 14.0, 1.3, 0.726, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -54.0, -30.0, 60.0, 1, 3, 2, 5, 3, 3, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoGlobalAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Globally Exponential Synchronisation Criterion of Chaotic Oscillators using Active Control",
	    "a", "b", "beta", "delta", ES, ES, ES, ES, ES, ES, 1.0, 1.2, 2.53, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.3, -7.4, 14.0, 1, 4, 2, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGloballyExponentialSynchronisationCriterionChaoticOscillators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Golden Mean 3D Chaotic System", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, -0.8, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.4, -2.0, 4.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGoldenMean, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Grassi-Mascolo Synchroniser", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -3.0, 4.0, 1, 2, 3, 0, 4, 4, NULL, 1, XAXIS+YAXIS, 5000.0,
	    NullSetup, DoGrassiMascolo, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.8, 1.0
	},
	{
	"Grid Multiwing Chaotic System with only Non-Hyperbolic Equilibria", "a", "b", "c", "n", ES, ES, ES, ES, ES, ES, 2.0, 1.0, 8.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.0, 2.0, 1, 4, 2, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGridMultiwingChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Hadley Chaotic Circulation", "a", "b", "f", "g", ES, ES, ES, ES, ES, ES, 0.2, 4.0, 8.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 5.0, 1, 2, 3, 4, 3, 3, NULL, 1, XAXIS, 10000.0,
	    NullSetup, DoHadley, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.4, 1.0
	},
	{
	"Half Inverted Rossler Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.1, 0.09375, 0.38, 0.0015, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.9, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, 0.2, 1.0, 3, 2, 1, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHalfInverted, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 0.8, 1.0
	},
	{
	"Half-Duplex Relay Channel (Logistics Map)", "a", "b", "1 for Logistics Map, 0 for Tent Map", ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.2, -0.2, 0.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 1000.0,
	    NullSetup, DoHalfDuplexRelay, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Half-Duplex Relay Channel (Tent Map)", "a", "b", "1 for Logistics Map, 0 for Tent Map", ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.1, 1.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 1000.0,
	    NullSetup, DoHalfDuplexRelay, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Halvorsen Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -18.0, 30.0, 1, 2, 3, 1, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoHalvorsen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.6, 1.0
	},
	{
	"Hamilton Energy, Complex Dynamical Analysis of a New Memristive Fitzhugh-Nagumo Neural Network", "a", "b", "c", "e", "i", "alpha", ES, ES, ES, ES, 0.7, 0.333333333, 0.8, 13.0, 0.6, 0.4, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0002, 0.00003, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -2.2, 3.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoHamiltonEnergy, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 0.02, 1.0
	},
	{
	"Harb Chaotic System", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.18, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 10.0, 1, 2, 3, 2, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoHarb, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Hardware FPGA Implementations of High-Speed True Random Bit Generators Based on Chaotic Oscillators", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.8, -2.6, 4.0, 1, 4, 3, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHardwareOptimisedFPGAImplementations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 2.0, 1.0
	},
	{
	"Harmonic Oscillators with Nonlinear Damping (0532 Thermostated Ergodic Oscillator)", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.05, 0.32, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.18, -2.67, 5.63, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHarmonicOscillatorsNonlinearDamping0532ErgodicOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.6, 1.0
	},
	{
	"Harmonic Oscillators with Nonlinear Damping (KBB Oscillator)", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.95, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -1.54, 3.2, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHarmonicOscillatorsNonlinearDampingKBBOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000875, 0.6, 1.0
	},
	{
	"Harmonic Oscillators with Nonlinear Damping (MKT Doubly-Thermostated Ergodic Oscillator)", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.6, -3.67, 6.9, 1, 3, 2, 1, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoHarmonicOscillatorsNonlinearDampingMKTErgodicOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0004, 1.0, 1.0
	},
	{
	"Harmonic Oscillators with Nonlinear Damping (Munmuangsaen Oscillator)", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.95, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -2.5, 5.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHarmonicOscillatorsNonlinearDampingKMunmuangsaenscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000875, 1.0, 1.0
	},
	{
	"Harmonic Oscillators with Nonlinear Damping (Nose-Hoover Oscillator with a temperature gradient)", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.95, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0001, 2.2, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.3, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHarmonicOscillatorsNonlinearDampingNoseHoover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000875, 0.8, 1.0
	},
	{
	"Heat Conduction, and the Lack Thereof, in Time-Reversible Dynamical Systems", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -1.2, 4.0, 3, 1, 2, 0, 3, 3, NULL, 1, YAXIS+XAXIS, 500000.0,
	    NullSetup, DoHeatConduction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hénon-Heiles System", "a", "b", "lambda", ES, ES, ES, ES, ES, ES, ES, 2.0, 2.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "px", "py", ES, ES, ES, ES, ES, ES, 0.2, 0.15, 0.25, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.70, 1.4, 1, 2, 3, 3, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoHenonHeiles, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Heterogeneous Multistability in a Novel System with Purely Nonlinear Terms", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 2.0, 1.5, 1.5, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.8, -5.0, 10.0, 4, 5, 6, 4, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoHeterogeneousMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Hidden Attractor and its Dynamical Characteristic in Memristive Self-Oscillating System", "a", "beta", "mu", "b", "w0", ES, ES, ES, ES, ES, 1.0, 0.5, 0.9, 0.1, 2.55, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -2.0, 0.000010, 0.000010, 0.000010, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.25, -1.43, 2.6, 1, 3, 2, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHiddenAttractorDynamicalCharacteristic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.4
	},
	{
	"Hidden Attractor in Smooth Chua Systems I", 
	    "alpha", "beta", "gamma", "m0", "m1", ES, ES, ES, ES, ES, 8.4562, 12.0732, 0.0052, 0.3532, -1.1468, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 8.82, 0.5561, -12.6008, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -12.5, 25.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHiddenAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 4.0, 1.0
	},
	{
	"Hidden Attractor in Smooth Chua Systems II", 
	    "alpha", "beta", "gamma", "m0", "m1", ES, ES, ES, ES, ES, 8.4562, 12.0732, 0.0052, 0.3532, -1.1468, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 8.82, 0.5561, -12.6008, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -18.0, 36.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHiddenAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Hidden Attractors On One Path-Glukhovsky-Dolzhansky, Lorenz, and Rabinovich Systems I", 
	    "r", "a", "s", "b", "q", ES, ES, ES, ES, ES, 346.0, 0.0, 4.0, 1.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -424.0, -282.0, 532.0, 3, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHiddenAttractorOnOnePath, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Hidden Attractors On One Path-Glukhovsky-Dolzhansky, Lorenz, and Rabinovich Systems II", 
	    "r", "a", "b", "sigma", ES, ES, ES, ES, ES, ES, 346.0, 0.01, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 60.0, 390.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -24.0, 48.0, 1, 4, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHiddenAttractorOnOnePathGlukhovskyDolzhansky, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 0.1
	},
	{
	"Hidden Bifurcations and Attractors in Nonsmooth Dynamical System", "m", "b", "p", "q", "s", ES, ES, ES, ES, ES, 4.0, 0.2, 10.0, 0.1, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.0018, 0.0025, 0.002, 1, 3, 5, 5, 3, 3, NULL, 1, YAXIS, 10000000.0,
	    NullSetup, DoHiddenBifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.1, 1.0
	},
	{
	"Hidden Chaotic Path Planning and Control of A Flexible Robot Manipulator", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -1.5, 6.0, 1, 6, 5, 5, 3, 3, NULL, 1, YAXIS+EVOLVE, 20000.0,
	    NullSetup, DoHiddenChaoticPathPlanningControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 1.0, 1.0
	},
	{
	"Hidden Chua Attractor", "a", "b", "d", "e", "g", "m0", "m1", ES, ES, ES, -1.398, -0.0136, 0.5, 1.4, -0.0297, -0.1768, -1.1468, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.1061, -0.7669, 0.0115, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoHiddenChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.021, 1.0, 1.0
	},
	{
	"Hidden Hyperchaos and Electronic Circuit Application in a 5D Self-Exciting Homopolar Disc Dynamo", 
	    "m", "g", "k2", "r", "k1", ES, ES, ES, ES, ES, 0.04, 140.6, 12.0, 7.0, 34.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.05, -0.5, 0.1, -1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 4, 5, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoHiddenHyperchaoElectronicCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Hidden Hyperchaotic Attractor in a Novel Simple Memristive Neural Network", 
	    "r1", "r4", "r5", "r8", "r11", "r13", "r2", "r3", "r10", "r6", 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 6.25, 5.0, 5.0, 2.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.1, 0.1, 6.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    5.95, -0.39, 0.8, 4, 5, 6, 10, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHiddenHyperchaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.08, 40.0, 1.0
	},
	{
	"High Dimensional, 4 Positive Lyapunov Exponents with 4 Scroll During a New Complex Nonlinear Model", "a", "b", "c", "k1", "k2", ES, ES, ES, ES, ES,
	    10.0, 1.5, 55.0, -50.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", ES, 1.0, 2.0, 3.0, 4.0, 0.0, 6.0, 7.0, 8.0, 9.0, 0.0,
	    -4.8, -0.7, 6.0, 1, 5, 2, 5, 9, 9, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHighDimensional4PositiveLyapunovExponents, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 8.0, 1.0
	},
	{
	"High Dimensional Hyperchaos: DUFFING + LORENZ + SPROTT J CHAOS = 10 Dimensions", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k", 
	    10.0, 8.0/3.0, 28.0, -2.5, 0.6, -8.0, 28.0, 2.0, 2.0, -2.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", "s", 1.0, -1.0, 3.0, -24.0, 5.0, -1.6, 0.17, -0.09, 1.0, 0.14,
	    -4.0, -3.0, 6.0, 10, 9, 8, 10, 10, 10, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoHighDimensional, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"High Throughput Pseudorandom Number Generator Based on Variable Argument Unified Hyperchaos", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    -0.14, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.8, -0.1, -0.1, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -24.0, 48.0, 3, 5, 2, 1, 4, 4, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoHighThroughputPseudorandomNumberGenerator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.2, 1.0
	},
	{
	"Hindmarsh-Rose Chaotic Attractor", "a", "b", "c", "d", "e", "s", "l", "x0", ES, ES, 1.0, 2.52, 1.0, 5.0, 0.01, 4.0, 4.0, -1.6, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -25.0, 30.0, 1, 2, 3, 8, 3, 3, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoHindmarshRose, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 1.0, 1.0
	},
	{
	"Hitzl-Zele Map-III", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.4, 12.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -90.0, 180.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHitzlZeleIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.006, 1.0, 1.0
	},
	{
	"Hitzl-Zele Map-Modified", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.25, 0.85, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, 1.0, 2.5, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHitzlZeleMod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 5.0, 1.0
	},
	{
	"Homoclinic Bifurcation and Chaos in a Noise-Induced Ö6 Potential", "w0", "L", "beta", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 2, 3, 1, 3, 3, 3, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoHomoclinicBifurcation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 5.0, 1.0
	},
	{
	"Hopf Bifurcation Analysis and Anticontrol of Hopf Circles of the Rössler-Like System", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -0.5, 0.2, -1.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.6, 2.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHopfBifurcation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.4, 1.0
	},
	{
	"Hopf Bifurcation Analysis and Chaos Control of a Chaotic System without ilnikov Orbits", 
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 40.0, -40.0, 33.0, -10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -10.0, 20.0, 2, 5, 1, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoHopfBifurcationAnalysisChaosControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 1.0
	},
	{
	"Hopf Bifurcation, Cascade of Period-Doubling, Chaos and the Possibility of Cure in a 3D Cancer Model", 
	    "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, 1.0, 2.5, 0.6, 1.5, 4.5, 1.0, 0.2, 0.5, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.201, 0.1, 0.41, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.2, 1.2, 2, 3, 1, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHopfBifCascade, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hopf Bifurcation Analysis in a New Chaotic System with Chaos Entanglement Function", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 36.0, 18.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -8.0, 16.0, 2, 3, 1, 5, 3, 3, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoHopfBifurcationEntanglement, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Horseshoe Chaos in a Simple Memristive Circuit", 
	    "alpha", "beta", ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0001, -0.00001, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -4.0, 10.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHorseshoeChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"How a Broken Egg Attractor Has Influenced the Dynamics of My Life-3D", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -8.5, 0.75, 1.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -16.0, 32.0, 1, 2, 3, 0, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoHowBrokenEggAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.1, 1.0
	},
	{
	"How to Bridge Attractors and Repellors-II", 
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.6, 1.2, 2, 5, 3, 1, 3, 3, NULL, 0, YAXIS, 250000.0,
	    NullSetup, DoHowToBridgeAttractorsRepellorsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 1.0, 1.0
	},
	{
	"Hybrid - 6D", "a", "b", "c", "l", ES, ES, ES, ES, ES, ES, 0.8, 3.0, 3.0, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.7, 0.25, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.6, -2.6, 3.6, 4, 5, 6, 4, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHybrid6D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.02
	},
	{
	"Hybrid Dislocated Control for Memristor Chaotic Oscillator System", 
	    "a1", "a2", "a3", "a4", "a", "b", ES, ES, ES, ES, 5.1, 0.69, 1.0, 0.12, 0.2, 17.8, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 4, 2, 6, 4, 4, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoHybridDislocated, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 3.2, 1.0
	},
	{
	"Hybrid System Plus Chaotic Attractor", 
	    "a", "b", "c", "d", "h", "alpha", "beta", "k", "l", ES, -1.0, 8.0, -8.0, -1.0, 20.0, -114.0, 3.0, 1.0, 6.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -8.0,  36.0, 1, 2, 3, 9, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHybridSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Hyper Jerk Chaotic System", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 35.0, 3.0, 21.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.2, -0.4, -0.5, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 2, 4, 1, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperJerk, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Hyper-Rossler Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.25, 3.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -10.0, -6.0, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -80.0, 160.0, 1, 2, 3, 4, 4, 4, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoHyperRossler, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hyperbolic Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 10.0, 9.2, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.8, 2.1, 1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -8.0, 16.0, 3, 2, 1, 4, 3, 3, NULL, 0, YAXIS, 65000.0,
	    NullSetup, DoHyperbolic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hyperbolic Sinusoidal and Cosinusoidal Nonlinearity Attractors I", "a", "b", "c", "d", "sign (1 or -1)", 
	    ES, ES, ES, ES, ES, 10.0, 92.0, 2.0, 10.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, -0.6, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -8.0, 24.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperbolicSin, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000001, 0.01, 1.0
	},
	{
	"Hyperbolic Sinusoidal and Cosinusoidal Nonlinearity Attractors II", "a", "b", "c", "d", "sign (1 or -1)", 
	    ES, ES, ES, ES, ES, 10.0, 98.0, 2.0, 10.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, -0.6, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, -8.0, 25.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperbolicSin, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000001, 0.002, 1.0
	},
	{
	"Hyperchaos and Hyperchaos Control of the Sinusoidally Forced Simplified Lorenz System", "omega", "c0", ES, ES, ES, ES, ES, ES, ES, ES, 
	    4.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.13, 0.5, 0.4, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -75.0, 250.0, 5, 7, 1, 2, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHyperChaosControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.4, 1.0
	},
	{
	"Hyperchaos Evolved from the Liu Chaotic System", "a", "b", "c", "d", "h", "k", ES, ES, ES, ES,
	    10.0, 40.0, 1.5, 92.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 55.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    22.0, -18.0, 36.0, 3, 4, 2, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperchaosEvolvedLiuChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.1, 1.0
	},
	{
	"Hyperchaos Based Cryptography - New 7D-System to Secure Communication 4th Order System", "a", "b", "yc", "d", "e", "f", "yg", "h", ES, ES, 15.0, 5.0, 0.5, 25.0, 10.0, 4.0, 0.1, 1.5, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", "x6", "x7", ES, ES, ES, 3.0, 1.0, -1.0, 1.0, 2.0, -2.0, 3.0, 0.0, 0.0, 0.0,
	    -85.0, -40.0, 80.0, 2, 10, 7, 8, 7, 7, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNew7DHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00000005, 0.0003, 1.0
	},
	{
	"Hyperchaos from an Augmented Lu System", "a", "b", "k", ES, ES, ES, ES, ES, ES, ES, -10.0, -4.0, 4.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 2.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -10.0, 20.0, 3, 4, 2, 3, 4, 4, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoHyperChaosLu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.2, 1.0
	},
	{
	"Hyperchaos in a Conservative System with Nonhyperbolic Fixed Points", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.5, 5.0, 2, 3, 4, 2, 5, 5, NULL, 0, YAXIS, 300000.0,
	    NullSetup, DoHyperchaosConservativeSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System I", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -30.0, 60.0, 1, 2, 3, 5, 4, 4, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System II", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -30.0, 60.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 2.0, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System III", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -68.0, -40.0, 80.0, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System IV", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -20.0, 40.0, 2, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System V", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -20.0, 40.0, 2, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System VI", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -28.0, -15.0, 30.0, 3, 4, 1, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Hyperchaos Numerical Simulation and Control in a 4D Hyperchaotic System VII", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 11.28, -11.21, -9.0, 20.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -45.0, 90.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperChaosNumerical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0255, 4.0, 1.0
	},
	{
	"Hyperchaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.56, 6.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 3, 1, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.2, 1.0
	},
	{
	"Hyperchaotic Attractor in a Novel Hyperjerk System with Two Nonlinearities I", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 3.7, 0.1, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.75, 3.5, 1, 4, 2, 3, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperchaoticAttractorNovelHyperjerkSystemI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.2, 1.0
	},
	{
	"Hyperchaotic Attractor in a Novel Hyperjerk System with Two Nonlinearities II", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 0.9, 0.2, 1.5, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 2.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -2.0, 4.0, 2, 5, 1, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperchaoticAttractorNovelHyperjerkSystemII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.8, 1.0
	},
	{
	"Hyperchaotic Attractor in 9 Dimenstions", "a", "r", "s", ES, ES, ES, ES, ES, ES, ES, 0.21, 43.3, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", ES, 10.0, -12.0, 32.0, -24.0, 25.0, -16.0, 17.0, -9.0, 10.0, 0.0,
	    -150.0, -100.0, 200.0, 1, 4, 7, 3, 9, 9, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoHyperChaotic9D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Hyperchaotic Attractor with Complex Patterns I", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 2.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.5, -0.5, 7.0, 1, 2, 3, 0, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHyperChaoticComplex, "SelectOscParams", (DLGPROC)SelectOscParams, 0.011, 0.5, 1.0
	},
	{
	"Hyperchaotic Attractor with Complex Patterns II", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 2.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -2.8, 6.0, 1, 4, 3, 0, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHyperChaoticComplex, "SelectOscParams", (DLGPROC)SelectOscParams, 0.011, 0.5, 1.0
	},
	{
	"Hyperchaotic Attractor with Complex Patterns III", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 2.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 3, 4, 1, 0, 4, 4, NULL, 1, XAXIS, 50000.0,
	    NullSetup, DoHyperChaoticComplex, "SelectOscParams", (DLGPROC)SelectOscParams, 0.011, 1.0, 1.0
	},
	{
	"Hyperchaotic Attractor with Multiple Positive Lyapunov Exponents I", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -100.0, 200.0, 1, 3, 4, 0, 5, 5, NULL, 1, YAXIS+EVOLVE, 400000.0,
	    NullSetup, DoHyperChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Hyperchaotic Attractor with Multiple Positive Lyapunov Exponents II", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -85.0, -50.0, 100.0, 1, 2, 3, 0, 5, 5, NULL, 1, YAXIS+EVOLVE, 400000.0,
	    NullSetup, DoHyperChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.5, 1.0
	},
	{
	"Hyperchaotic Attractor with Multiple Positive Lyapunov Exponents III", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -85.0, -50.0, 100.0, 1, 5, 3, 0, 5, 5, NULL, 1, YAXIS, 400000.0,
	    NullSetup, DoHyperChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.1, 1.0
	},
	{
	"Hyperchaotic Attractor with Multiple Positive Lyapunov Exponents - Starwars", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -180.0, -130.0, 160.0, 1, 5, 4, 0, 5, 5, NULL, 1, YAXIS+EVOLVE, 400000.0,
	    NullSetup, DoHyperChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.15, 1.0
	},
/*
	{
	"Hyperchaotic Attractor with Multiple Positive Lyapunov Exponents II", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -100.0, 200.0, 1, 4, 5, 0, 5, 5, NULL, 1, YAXIS+EVOLVE, 400000.0,
	    NullSetup, DoHyperChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00006, 1.0, 1.0
	},
*/
	{
	"Hyperchaotic Behavior in Arbitrary-Dimensional Fractional-Order Quantum Cellular Neural Network I", "b1", "b2", "b3", "t1", "t2", "t3", ES, ES, ES, ES, 
	    0.5, 0.3, 0.2, 0.28, 0.25, 0.28, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.3, 0.0, 0.2, 0.03, 0.76, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperchaoticBehavior, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.9, 1.0
	},
	{
	"Hyperchaotic Behavior in Arbitrary-Dimensional Fractional-Order Quantum Cellular Neural Network II", "b1", "b2", "b3", "t1", "t2", "t3", ES, ES, ES, ES, 
	    0.5, 0.3, 0.2, 0.28, 0.25, 0.28, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.3, 0.0, 0.2, 0.03, 0.76, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 3, 4, 6, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperchaoticBehavior, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.1, 1.0
	},
	{
	"Hyperchaotic Behavior in Arbitrary-Dimensional Fractional-Order Quantum Cellular Neural Network III", "b1", "b2", "b3", "t1", "t2", "t3", ES, ES, ES, ES, 
	    0.5, 0.3, 0.2, 0.28, 0.25, 0.28, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.3, 0.0, 0.2, 0.03, 0.76, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 4, 5, 6, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperchaoticBehavior, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.4, 1.0
	},
	{
	"Hyperchaotic Behavior in Arbitrary-Dimensional Fractional-Order Quantum Cellular Neural Network IV", "b1", "b2", "b3", "t1", "t2", "t3", ES, ES, ES, ES, 
	    0.5, 0.3, 0.2, 0.28, 0.25, 0.28, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.3, 0.0, 0.2, 0.03, 0.76, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 3, 4, 5, 6, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperchaoticBehavior, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.9, 1.0
	},
	{
	"Hyperchaotic Behavior in Arbitrary-Dimensional Fractional-Order Quantum Cellular Neural Network V", "b1", "b2", "b3", "t1", "t2", "t3", ES, ES, ES, ES, 
	    0.5, 0.3, 0.2, 0.28, 0.25, 0.28, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.3, 0.0, 0.2, 0.03, 0.76, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 3, 6, 4, 6, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperchaoticBehavior, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.06, 1.0
	},
	{
	"Hyperchaotic Behavior in Arbitrary-Dimensional Fractional-Order Quantum Cellular Neural Network VI", "b1", "b2", "b3", "t1", "t2", "t3", ES, ES, ES, ES, 
	    0.5, 0.3, 0.2, 0.28, 0.25, 0.28, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.3, 0.0, 0.2, 0.03, 0.76, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 5, 6, 2, 6, 6, 6, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoHyperchaoticBehavior, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.06, 1.0
	},
	{
	"Hyperchaotic Chameleon: Fractional Order FPGA Implementation", "a", "b", "c", "d", "alpha", "beta", ES, ES, ES, ES, 1.0, 0.5, 2.0, 1.4, 4.0, 0.01, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -20.0, 0.1, 18.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -80.0, 160.0, 1, 6, 4, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperchaoticChameleon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Hyperchaotic Finance System", "a", "b", "c", "d", "e", 
	    ES, ES, ES, ES, ES, 0.9, 0.2, 1.5, 0.2, 0.17, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 2.0, -1.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -2.4, 3.6, 2, 4, 3, 5, 4, 4, NULL, 1, XAXIS+YAXIS, 100000.0,
	    NullSetup, DoHyperChaoticFinance, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Hyperchaotic Circuit Based on Memristor Feedback with Multistability and Symmetries", "a", "b", "c", "d", "alpha", "beta", ES, ES, ES, ES, 24.0, 4.0, 10.0, 19.0, 1.0, 0.02, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0001, -0.1, -0.0001, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -270.0, -170.0, 310.0, 6, 8, 7, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperChaoticCircuitMemristorFeedback, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00025, 10.0, 1.0
	},
	{
	"Hyperchaotic Memcapacitor Oscillator with Infinite Equilibria and Coexisting Attractors 1", 
	    "a", "b", "c", "d", "e", "f", "m", "n", ES, ES, 5.8, 2.0, 2.6, 0.1, -3.4, 0.2, 2.8, 6.8, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.001, 0.001, 0.001, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -12.0, 24.0, 2, 3, 4, 8, 4, 4, NULL, 1, YAXIS, 800000.0,
	    NullSetup, DoHyperchaoticMemcapacitorOscillator1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 3.0
	},
	{
	"Hyperchaotic Memcapacitor Oscillator with Infinite Equilibria and Coexisting Attractors 2",
	    "a", "b", "c", "d", "e", "f", "m", "n", ES, ES, 5.8, 2.0, 2.6, 0.1, -3.4, 0.2, 2.8, 6.8, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.001, 0.001, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35000.0, -20600.0, 40400.0, 2, 6, 5, 8, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoHyperchaoticMemcapacitorOscillator2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000275, 0.2, 0.6
	},
	{
	"Hyperchaotic Oscillator with Gyrators", "a", "c", "e", "m", ES, ES, ES, ES, ES, ES, 
	    0.55, 40.0, 0.31, 0.33, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -4.0, 12.0, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoHyperChaoticOscillatorGyrators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"Hyperchaotic Sakarya Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "x1", "y1", "z1", ES, ES, ES, ES, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -64.0, -35.0, 70.0, 1, 4, 3, 2, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperChaoticSakarya, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 1.0
	},
	{
	"Hyperchaotic Yan-Chen Projective-3", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 37.0, -1.0, 26.0, 38.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.3, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -120.0, 300.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperchaoticYanChen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Hyperchaotic Yujun System", "a", "b", "c", "r", ES, ES, ES, ES, ES, ES, 35.0, 3.0, 21.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.4, -0.5, 0.28, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -28.0, 50.0, 2, 4, 3, 4, 4, 4, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoHyperchaoticYujun, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.1, 1.0
	},
	{
	"Hyperchaotic Zhou System", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 100.0, 1.0, 12.0, 0.005, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 25.0, -16.0, 20.0, -30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.3, 2.0, 2, 3, 4, 4, 4, 4, NULL, 1, YAXIS+EVOLVE, 1000000.0,
	    NullSetup, DoHyperchaoticZhou, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.6, 1.0
	},
	{
	"Hypogenetic Chaotic Jerk Flows - Type HJ7", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.1, 0.54, 0.365, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, -2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -2.4, 4.4, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoHypogeneticChaoticJerkFlowsTypeHJ7, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Hypothetical Galaxy Potentials", "a", "b", "e", ES, ES, ES, ES, ES, ES, ES, 18.0, 14.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "px", "y", "py", "z", "pz", ES, ES, ES, ES, 0.1, 0.1, 0.3, -0.5, 0.6, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.0, 2.0, 3, 2, 1, 9, 6, 6, NULL, 0, YAXIS, 30000.0,
	    NullSetup, DoHypotheticalGalaxies, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Identification of Unknown Parameters and Orders via Cuckoo Search Oriented Statistically",
	    "a", "b", "c", "g", ES, ES, ES, ES, ES, ES, 8.0, 40.0, 49.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, -2.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -300.0, -180.0, 360.0, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoIdentification, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0007, 12.0, 1.0
	},
	{
	"Ikeda Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.0, 0.9, 0.4, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -900.0, -650.0, 1200.0, 2, 3, 1, 4, 3, 3, NULL, 1, XAXIS+YAXIS, 100.0,
	    NullSetup, DoIkedaAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.1, 1.0
	},
	{
	"Ilogistic Attractor", "r", "a", "b", ES, ES, ES, ES, ES, ES, ES, 4.9, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.5, 0.8, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoIlogisticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.001, 1.0
	},
	{
	"Ilogistic Attractor + Twin", "r", "a", "b", ES, ES, ES, ES, ES, ES, ES, 10.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.4, 1, 2, 3, 3, 3, 3, NULL, 0, XAXIS, 500000.0,
	    NullSetup, DoIlogisticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.001, 1.0
	},
	{
	"Image Encryption Algorithm Based on Novel Improper Fractional-Order Attractor & a Wavelet Function Map", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 6.0, 5.0, 0.12, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -20.0, -40.0, -85.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 2, 4, 1, 3, 3, 3, NULL, 0, YAXIS, 2000000.0,
	    NullSetup, DoImageEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.1, 1.0
	},
	{
	"Image Encryption Algorithm Based on Hyper-chaos", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 33.0, 6.0, 28.0, -16.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.3, -0.4, 1.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 1, 2, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoImageEncryptionHyperChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Image Encryption via Base-n PRNGs and Parallel Base-n S-boxes", "a", "b", "c", "d", "e", "f", "g", "start", ES, ES, 1.5, 360.0, 0.0326, 36.0, -1.5, -0.0213, 0.08, 50000.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.3, 0.3, 0.3, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -11.0, 23.0, 1, 2, 3, 8, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoImageEncryptionBase_nPRNGs, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 3.0, 1.0
	},
	{
	"Implementation for Sync of Chen and Liu-Yang Chaotic Systems using SMC and Active Control Schemes", 
	    "a", "b", "d", ES, ES, ES, ES, ES, ES, ES, 5.0, 3.0, 35.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.19, 0.29, -0.34, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -40.0, 80.0, 2, 5, 4, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoImplementationSynchronisationChenLiuYangChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.06, 0.4
	},
	{
	"Implementation of FPGA-Based Real Time Novel Chaotic Oscillator", 
	    "alpha", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -4.0, 1.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -7.5, 10.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoImplementationFPGABasedRealTimeNovelChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Improved Memristive Diode Bridge-Based Band Pass Filter Chaotic Circuit", "a", "c", "k", ES, ES, ES, ES, ES, ES, ES, 5.0E-3, 6.02E-6, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 4, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoImprovedMemristiveDiodeBridge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 4.0, 1.0
	},
	{
	"Improved Seven-Dimensional (i7D) Hyperchaotic Map-Based Image Encryption technique", "k", "m", "l", "n", "s", "w", "t1", "t2", "h", "u", 10.0, 1.0, 28.0, 8.0/3.0, 2.0, 9.9, 1.0, 2.0, 1.0, 1.0,
	    "x", "y", "z", "t", "u", "v", "w", ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
	    -54.0, -30.0, 60.0, 1, 2, 3, 10, 7, 7, NULL, 1, YAXIS, 8000.0,
	    NullSetup, DoImproved7DHyperchaoticMapBasedImageEncryptiontechnique, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00255, 1.0, 1.0
	},
	{
	"Improving the Complexity of the Lorenz Dynamics", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 20.0, 2.0, 3.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.5, 0.8, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 7, 8, 5, 4, 4, 4, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoImprovingLorenzDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.3, 1.0
	},
	{
	"Impulsive Control and Synchronisation of a New 5D Hyperchaotic System I", "a1", "a2", "a3", "a4", "a5", 
	    ES, ES, ES, ES, ES, 37.0, 14.5, 10.5, 15.0, 9.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 2.0, -2.0, 3.5, -3.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -24.0, 48.0, 1, 4, 5, 5, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoImpulsiveControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 1.0, 1.0
	},
	{
	"Impulsive Control and Synchronisation of a New 5D Hyperchaotic System II", "a1", "a2", "a3", "a4", "a5", 
	    ES, ES, ES, ES, ES, 37.0, 14.5, 10.5, 15.0, 9.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 2.0, -2.0, 3.5, -3.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.6, -1.0, 2.0, 3, 4, 2, 5, 5, 5, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoImpulsiveControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 0.05, 1.0
	},
	{
	"Impulsive Control and Synchronisation of a New 5D Hyperchaotic System III", "a1", "a2", "a3", "a4", "a5", 
	    ES, ES, ES, ES, ES, 37.0, 14.5, 10.5, 15.0, 9.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 2.0, -2.0, 3.5, -3.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -24.0, 44.0, 1, 5, 2, 5, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoImpulsiveControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 1.0, 1.0
	},
	{
	"Impulsive Control and Synchronisation of a New 5D Hyperchaotic System IV", "a1", "a2", "a3", "a4", "a5", 
	    ES, ES, ES, ES, ES, 37.0, 14.5, 10.5, 15.0, 9.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "v", "w", ES, ES, ES, ES, ES, 2.0, -2.0, 3.5, -3.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -24.0, 44.0, 4, 5, 2, 5, 5, 5, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoImpulsiveControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 1.0, 1.0
	},
	{
	"Impulsive Control for T-S Fuzzy Model Consisting of Van der Pol Oscillators Coupled to Linear Oscillators", "e1", "e2", "lambda", "a", "omega", 
	    ES, ES, ES, ES, ES, 3.872, 0.000645, 9.12, 0.54, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -0.5, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -8.0, 16.0, 2, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoImpulsiveControlTSFuzzyModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Impulsive Synchronisation for a New Chaotic Oscillator I", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 1.5, -0.4, -0.5, -0.5, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.6, -1.0, 0.09, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoImpulsiveSyncI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.8, 1.0
	},
	{
	"Impulsive Synchronisation for a New Chaotic Oscillator II", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 1.5, -0.4, -0.5, -0.5, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.5, -1.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -12.0, 24.0, 1, 2, 3, 5, 3, 3, NULL, 1, XAXIS, 1000000.0,
	    NullSetup, DoImpulsiveSyncII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.4, 1.0
	},
	{
	"Inductor-Free Multistable Chua’s Circuit Made by Improved PI-Type Memristor and Active High-Pass Filter", "a", "b", "c", "d", "m", "k", "g", ES, ES, ES, 15.1515, 20.0, 26.6667, 10.0, 1.0, 2.2, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.00000001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, -0.51, 1.04, 1, 2, 3, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInductorFreeMultiStableChuasCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 1.0, 1.0
	},
	{
	"Induced Synchronisation of a Bistable Oscillator with Chaotic Dynamics in 6D", "a", "d", "b", "r", "g", "w0", "a0", 
	    ES, ES, ES, 9.0, 1.486, 0.4, 0.0, 0.52, 0.60274, 1.05, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.5, 3.0, 1, 2, 3, 7, 6, 6, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoInducedSynchronization, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0375, 1.6, 1.0
	},
	{
	"Infinite Multistability in a Self-Reproducing Chaotic System", "a", "b", "azao", "bezau", ES, ES, ES, ES, ES, ES, 2.8, 4.0, 2.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1101, 0.1, -0.1101, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.1, 6.0, 5, 6, 4, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInfiniteMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.2, 0.5
	},
	{
	"Infinite-Scroll Attractor Generated by the Complex Pendulum Model", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.3, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.3, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -10.0, 60.0, 1, 2, 3, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoInfiniteScroll, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Infinitely Many Coexisting Conservative Flows in a 4D Conservative System Inspired by LC Circuit", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.01, 0.01, -0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.5, 1.0, 3, 4, 2, 3, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoInfinitelyManyCoexistingConservativeFlows, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0004, 0.3, 0.1
	},
	{
	"Infinitesimal Chaotic Spheres", "a1", "a2", "a3", "b", "c1", "c2", "c3", "c4", "d", ES, -4.1, -0.5, 13.45, 0.161, 2.76, 0.6, 13.13, 3.5031, 1.6, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -28.0, -28.0, 30.0, 1, 2, 3, 9, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoInfinitesimalChaoticSpheres, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Influence of Delay Factors on the Genesis of Deterministic Chaos in Non-Ideal Pendulum Systems", 
	    "c", "d", "e", "f", "gamma", "delta", ES, ES, ES, ES, -0.1, -0.53, -0.59, 0.0, 0.15, 0.15, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -2.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -7.0, 14.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoInfluenceDelayFactorsDeterministicChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 1.0, 1.0
	},
	{
	"Initial Condition-Dependent Dynamics and Transient Period in Memristor-Based Hypogenetic Jerk System I", 
	    "a", "b", "c", "alpha", "beta", ES, ES, ES, ES, ES, 0.6, 1.3, 2.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -2.0, 3.7, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInitialConditionDependentDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"Initial Condition-Dependent Dynamics and Transient Period in Memristor-Based Hypogenetic Jerk System II", 
	    "a", "b", "c", "alpha", "beta", ES, ES, ES, ES, ES, 0.6, 1.3, 2.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, -2.8, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -2.7, 5.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInitialConditionDependentDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"Initial Offset Boosting Coexisting Attractors in Memristive Multi-Double-Scroll Hopfield Neural Network",
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 1.0, 0.03, 3.0, 1.3, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.5, 1, 5, 6, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoInitialOffsetBoostingCoexistingAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 1.0, 1.0
	},
	{
	"Integer and Fractional General System and Its Application to Control Chaos and Synchronisation", "a", "b1", "b2", "c", "d", "l", "k", ES, ES, ES, 
	    10.0, 40.0, 1.0, 2.5, 3.0, 0.9, 10.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.4, -0.5, 3.6, 2, 3, 1, 7, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoIntegerFractional, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 0.03, 1.0
	},
	{
	"Invariant Sets of a 4 Dimensional System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.1, 0.6, 1.5, 26.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 2.0, 0.5, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -15.0, 30.0, 2, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoInvariantSets4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 3.0, 1.0
	},
	{
	"Invented - Based on Muthuswamy-Chua Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, -1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.5, 7.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInventedMuthuswamy, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 1.0, 1.0
	},
	{
	"Invention - Opened", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 3, 4, 2, 3, 2, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInventionOpened, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.4, 0.03
	},
	{
	"Invisible Grazings and Dangerous Bifurcations in Impacting Systems--The Problem of Narrow-Band Chaos", "qsi", "b", "e", "w", "a", ES, ES, ES, ES, ES, 0.01, 29.0, 1.26, 0.84023, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -154700.0, -97500.0, 168000.0, 1, 2, 2, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoInvisibleGrazings, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Irregular Attractors", "m", "g", ES, ES, ES, ES, ES, ES, ES, ES, 1.42, 0.097, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, -0.9, -3.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -11.0, 20.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoIrregularAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Irregular Chaotic Attractor", "a", "m", "g", ES, ES, ES, ES, ES, ES, ES, 0.549, 1.42, 0.097, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -30.0, -0.5, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 30.0, 1, 2, 3, 3, 3, 3, NULL, 1, XAXIS+YAXIS, 500000.0,
	    NullSetup, DoIrregularChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Jerk System with Self-Exciting & Hidden Flows & the Effect of Time Delays on Existence of Multi-Stability I", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 9.0, 4.4, 0.95, 3.5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -4.5, -3.0, -2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -20.0, 25.0, 2, 3, 1, 5, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoJerkSystemSelfExcitingHiddenFlowsI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Jerk System with Self-Exciting & Hidden Flows & the Effect of Time Delays on Existence of Multi-Stability II", 
	    "a", "c", "d", ES, ES, ES, ES, ES, ES, ES, 2.55, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 2.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -1.3, 3.0, 3, 6, 2, 3, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoJerkSystemSelfExcitingHiddenFlowsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Jha Hyper-Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 10.0, 28.0, 2.66667, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -40.0, 80.0, 3, 2, 4, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoJha4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Joana-Marcus-A Chaotic Attractor (plot lines turned off)", "b", "r", "s", "x0", "i", "k", ES, ES, ES, ES, 1.96, 0.01, 6.0, 1.6, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -9.0, 12.0, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoJoanaMarcus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.09, 0.15, 1.0
	},
	{
	"Joana-Marcus-B Chaotic Attractor (plot lines turned off)", "b", "r", "s", "x0", "i", "k", ES, ES, ES, ES, 2.0, 0.005, 6.0, 1.6, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -9.0, 12.0, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoJoanaMarcus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.09, 0.15, 1.0
	},
	{
	"Joana-Marcus-C Chaotic Attractor (plot lines turned off)", "b", "r", "s", "x0", "i", "k", ES, ES, ES, ES, 2.0, 0.05, 6.0, 1.8, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -5.0, 10.0, 3, 1, 2, 6, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoJoanaMarcus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Joanna 2 Neurons Chaotic Attractor", "a", "b", "c", "g", ES, ES, ES, ES, ES, ES, 0.658764, 0.4, 6.0, 0.0552852, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.1, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoJoanna2Neurons, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Joanna 3 Neurons Chaotic Attractor", "a", "b", "c", "g", ES, ES, ES, ES, ES, ES, 0.668764, 0.4, 2.0, 0.1552852, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", "x6", ES, ES, ES, ES, -0.1, 0.5, 0.1, -0.3, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoJoanna3Neurons, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Josephson Junction System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.4144, 0.3731, 0.0478, 1.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.8, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -1.0, 8.0, 2, 3, 1, 4, 3, 3, NULL, 1, ZAXIS, 200000.0,
	    NullSetup, DoJosephsonJunction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 4.0, 1.0
	},
	{
	"Juliana interweaving neurons in 8 Dimensions", "a", "b", "c", "gamma", ES, ES, ES, ES, ES, ES, 0.658764, 0.4, 6.0, 0.0552852, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "y1", "x2", "y2", "x3", "y3", "x4", "y4", ES, ES, -0.1, 0.5, 0.1, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 5.0, 1, 2, 3, 4, 8, 8, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoJuliana8D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot-Holder Chaotic Oscillator", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 10.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -2.0, 4.0, 3, 2, 1, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoKnotHolder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 2.5, 1.0
	},
	{
	"Lab experiment: Frequency Response of Chaotic Oscillators", "a", "b", "c", "d", "k", "ha", "p", "q", "alpha", "i", 0.7, 0.8, 0.8, 0.9, 1.0, 1.0, 1.0, 1.0, 6.4E-3, 0.1,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 3, 2, 10, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoLabexperiment, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Labyrinth Chaos from the Elegant Chaos Book", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -75.0, 86.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 1500000.0,
	    NullSetup, DoLabyrinthChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Laces and Turns Chaotic Oscillator", "a", "b", "b1", "b2", "b3", "b4", "b5", "b6", ES, ES, 0.01, -1.531, 0.1, 0.1, 1.3, 1.2, 0.235, 1.12, 0.0, 0.0,
	    "p1", "q1", "p2", "q2", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8000.0, -5000.0, 10000.0, 1, 2, 3, 8, 4, 4, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoLacesAndTurns, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 1.0, 1.0
	},
	{
	"Lambda-Phi Generalised Synchronisation: App'n to Fractional Hyperchaotic Systems with Arbitrary Dimensions", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 8.0, -2.5, -30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 0.0001, 0.0001, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.48, -0.68, 1.6, 1, 2, 4, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoLambdaPhiGeneralisedSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003505947, 0.2, 0.05
	},
	{
	"Laser Oscillator Connected to Catastrophe's Oscillator", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.3, 0.0, 8.0, 0.035, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.9, 0.1, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -4.5, 15.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoLaserCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Leipnik Hyper-Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.4, 0.175, 2.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.3, 1.0, 4, 3, 1, 4, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoLeipnik4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Leonov-Kuznetsov Chaotic Attractor in 4 Dimensions", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 3.0, -1.0, -3.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -12.0, 20.0, 4, 1, 3, 0, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoLeonovKuznetsov4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Lévy Flight 3D survival procedure for Fishes and Birds", "alpha", "Monochrome colour or 0 for coloured flight", 
	    "Foraging(0.5) or Running(2)", ES, ES, ES, ES, ES, ES, ES, 1.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -250.0, 500.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoLeviFlight, "SelectOscParams", (DLGPROC)SelectOscParams, 1.0, 1.0, 1.0
	},
	{
	"Lévy footprints small herd of animals in the savanah after the rain", "alpha", "s", "m", "t", "k", 
	    "Monochrome or 0 for coloured", ES, ES, ES, ES, 1.5, 10000.0, 2.0, 1.25, -1.0, 15.0, 0.0, 0.0, 0.0, 0.0,
	    "x0", "x1", "y0", "y1", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -250.0, 500.0, 1, 2, 3, 6, 2, 4, NULL, 1+BLOCKANIM, YAXIS, 1000.0,
	    NullSetup, DoLeviZigZag, "SelectOscParams", (DLGPROC)SelectOscParams, 1.0, 1.0, 1.0
	},
	{
	"Li - Symmetrical Toroidal Chaos", "a", "c", "d", "e", "f", "k", ES, ES, ES, ES, 40.0, 1.8333333, 0.16, 0.65, 20.0, 55.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -500.0, -240.0, 450.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoLi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.5, 1.0
	},
	{
	"Linear and Non-linear Arabesques I", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.3, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 2, 3, 1, 0, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoLinearArabesque, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 1.0, 1.0
	},
	{
	"Linear and Non-linear Arabesques II", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 2, 3, 1, 0, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoLinearArabesque, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 1.0, 1.0
	},
	{
	"Linear and Non-linear Arabesques III", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.8, 3.6, 2, 3, 1, 0, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoLinearArabesque, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 1.0, 1.0
	},
	{
	"Liu-Chen Attractor", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 2.4, -3.78, 14.0, -11.0, 4.0, 5.58, -1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 3, 2, 1, 7, 3, 3, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoLiuChen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Lorenz Chaotic Attractor", "s", "r", "b", ES, ES, ES, ES, ES, ES, ES, 10.0, 28.0 ,2.6666667, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -20.0, 50.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Lorenz Chaotic Attractor in 8 Dimensions", "r", "s", "b", ES, ES, ES, ES, ES, ES, ES, 43.0, 10.0 ,2.6666667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "x1", "y1", "z1", "x2", "y2", ES, ES, 10.0, 5.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 0.0, 0.0,
	    -80.0, -50.0, 100.0, 1, 2, 8, 3, 8, 8, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoLorenz8D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Lorenz Like Chaotic Attractor", "a", "b", "c", "d", "e", "f", "gamma", ES, ES, ES, 40.0, 55.0, 11.0/6.0, 0.16, 0.65, 20.0, 0.2, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 1.0, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -300.0, -75.0, 320.0, 1, 3, 2, 7, 4, 4, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoLorenzLike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 1.0, 1.0
	},
	{
	"Lorenz Minus Catastrophe Equations", "a", "b", "c", "d", "frac", "h", ES, ES, ES, ES, 
	    -0.4, -0.5, -0.2, -0.4, 8.0/3.0, 0.01, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.1, -0.25, 0.5, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoLorenzMinusCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, -1.0, 0.3, 1.0
	},
	{
	"Lorenz Unlike Chaotic Attractor", "a", "b", "c", "d", "m", "n", ES, ES, ES, ES, 10.0, 96.0, 1.7, 2.9, 3.5, 9.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 2.0, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, 0.0, 20.0, 2, 3, 1, 6, 3, 3, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoLorenzUnlike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0006, 2.4, 1.0
	},
	{
	"Lorenz-84 Atmospheric Circulation Model", "b", "f", "g", "a", ES, ES, ES, ES, ES, ES, 4.0, 8.0, 1.0, 0.2435, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.5, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.8, 3.6, 1, 3, 2, 4, 3, 3, NULL, 0, XAXIS, 500000.0,
	    NullSetup, DoLorenz84Osc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.6, 1.0
	},
	{
	"Lorenz-Mod1 Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.1, 4.0, 14.0, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 1, 2, 3, 4, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoLorenzMod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.9, 1.0
	},
	{
	"Lorenz-Mod2 Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.9, 5.0, 9.9, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -15.0, 30.0, 1, 2, 3, 4, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoLorenzMod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 1.0, 1.0
	},
	{
	"Lorenz-Steflo Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.9, 5.0, 9.9, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -15.0, 30.0, 1, 2, 3, 4, 3, 3, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoLorenzMod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 1.0, 1.0
	},
	{
	"Lorenz-Steflo I Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 7.0, 1.5, 26.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 30.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -15.0, 30.0, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoLorenzStenflo4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 5.0, 1.0
	},
	{
	"Lorenz-Steflo II Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.0, 1.5, 26.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 30.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -15.0, 30.0, 3, 4, 1, 4, 4, 4, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoLorenzStenflo4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 3.0, 1.0
	},
	{
	"Lotka-Volterra Chaotic System I", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.9851, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.2, 0.5, 1.0, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS, 100.0,
	    NullSetup, DoLotkaVolterra, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.25, 1.0
	},
	{
	"Lotka-Volterra Chaotic System II", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.9851, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.20, 0.4, 1.2, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS+EVOLVE, 500.0,
	    NullSetup, DoLotkaVolterra, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.25, 1.0
	},
	{
	"Loving Lorenz Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.1, 4.0, 14.0, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -35.0, 80.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 80.0,
	    NullSetup, DoLovingLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"Lozi Fractal Map Equations Converted into a Chaotic Oscillator I", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.05, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 16800.0,
	    NullSetup, DoLoziMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Lozi Fractal Map Equations Converted into a Chaotic Oscillator II", "a", "b", "real", "imag", ES, ES, ES, ES, ES, ES, 1.7, 0.5, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.62, 1.2, 2, 4, 1, 4, 3, 2, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoLoziMapII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.5, 1.0
	},
	{
	"Low Power Pseudo-Random Number Generator Based on Lemniscate Chaotic Map", "r", "real", "imag", "z frequency", ES, ES, ES, ES, ES, ES, 7.0, 0.1, 0.1, 35.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.1, 2.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    2.499, 0.016, 0.005, 2, 4, 1, 4, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoLowPowerPseudoRandomNumberGenerator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.02, 0.02
	},
	{
	"LSD - Lines, Surfaces and Dots", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "u", "v", ES, ES, ES, ES, ES, ES, ES, ES, 0.001, 0.06, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -16.0, 20.0, 1, 2, 3, 0, 4, 2, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoLSD, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.05, 1.0
	},
	{
	"Lu Hyper-Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 36.0, 3.0, 20.0, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 8.0, 12.0, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -90.0, -60.0, 120.0, 1, 4, 3, 4, 4, 4, NULL, 0, YAXIS, 40000.0,
	    NullSetup, DoLu4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 0.3, 1.0
	},
	{
	"Lu Like Hyper-Chaotic Attractor", "a", "c", "d", "e", "f", "gamma", ES, ES, ES, ES, 40.0, 5.0/6.0, 0.5, 0.65, 20.0, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 1.0, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, 20.0, 80.0, 3, 4, 2, 6, 4, 4, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoLuLike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 48.0, 1.0
	},
	{
	"Machado, Savi, Pacheco Chaotic Attractor", "teta1", "teta2", "teta3", "m", "d1", "d2", "w1", "q1", "b1", "e1", 1.7, 1.7, -0.7, 1.0, 0.06, 0.06,  100.0, 0.02, 1300.0, 4.7e5,
	    "y0", "y1", "y2", "y3", ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.1, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.8, 1.6, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoMachado, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 0.01, 1.0
	},
	{
	"Magnet Synchronous Motor Systems", "gamma", "sigma", ES, ES, ES, ES, ES, ES, ES, ES, 20.0, 5.46, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -18.0, 40.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoMagnet, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Magnetic Field Induced Dynamical Chaos", "a", "b", "w", "omega", ES, ES, ES, ES, ES, ES, 0.25, 0.5, 1.0, 0.16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "ux", "uy", "z", "uz", ES, ES, ES, ES, 0.005, -0.001, 0.0, 0.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.0, 2.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoMagneticField, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Magnetic Stochasticity in Magnetically Confined Fusion Plasmas", "b", "a", "k", ES, ES, ES, ES, ES, ES, ES, 0.4, 1.5, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.6, 1.0, 0.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -22.0, 44.0, 2, 5, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMagneticStochasticity, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Marcus-1", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 7.5, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -4.0, 8.0, 3, 2, 1, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMarcus01, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Marcus-2", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 7.5, 0.18, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -0.2, 4.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMarcus02, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
/*
	{
	"Marcus-3", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 7.5, 0.28, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMarcus03, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
*/
	{
	"Marcus-4", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 4.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0,  -180.0,  320.0, 1, 2, 3, 2, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoMarcus04, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Marcus-5", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.45783, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4,  -1.5,  3.0, 3, 6, 2, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoMarcus05, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.2, 1.0
	},
	{
	"Marcus-6", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 7.5, -0.028, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -360.0,  -80.0,  240.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMarcus06, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 15.0, 1.0
	},
	{
	"Marcus-8", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 7.5, 0.28, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, -1.0, -13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0,  -50.0,  100.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMarcus08, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
/*
	{
	"Marcus-10", "a", "b",  "x", "y", "z", ES, ES, ES, ES, ES, 7.5, 0.28, 0.0, -1.0, -13.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0,  -10.0,  20.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMarcus10_11, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
*/
	{
	"Marcus-11", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 7.5, 0.28, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMarcus11, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Marcus Bi Map", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 1, 2, 1, 0, 1, 1, NULL, 0, YAXIS, 20000.0,
	    NullSetup, DoMarcusBiMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 0.1, 0.1
	},
	{
	"Martin I", "a", "b", "c", "0 for sin(), 1 for cos()", ES, ES, ES, ES, ES, ES, 45.0, 2.0, -200.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -1.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -200.0, 250.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMartin, "SelectOscParams", (DLGPROC)SelectOscParams, 1.8, 1.0, 1.0
	},
	{
	"Martin II", "a", "b", "c", "0 for sin(), 1 for cos()", ES, ES, ES, ES, ES, ES, 45.0, 2.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -1.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -3.0, 10.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMartin, "SelectOscParams", (DLGPROC)SelectOscParams, 1.3, 1.0, 1.0
	},
	{
	"Master-Slave Synchronisation", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "q", "w", "r", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 500.0,
	    NullSetup, DoMasterSlave, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Matlab Code for Lyapunov Exponents of Fractional-Order Systems", "p", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.98, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.6, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoMatlabCodeForLyapunovExponents, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Mechanics Analysis and Hardware Implementation of a New 3D Chaotic System", "a", "b", "c", "d", "k", "m", "h", ES, ES, ES, 4.0, 3.0, 1.0, 7.0, 1.0, 2.0, 2.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -6.2, 15.0, 2, 6, 3, 7, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMechanicsAnalysisHardwareImplementation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Medical Image Encryption Algorithm based on Latin Square and Memristive Chaotic System", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.5, 3.0, 0.1, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -11.0, 15.0, 6, 7, 8, 5, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoMedicalImageEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 0.3, 1.0
	},
	{
	"Megastable System with 2-D Strip of Hidden Attractors and Analytical Solutions", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 2.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	1.21, -0.96, 2.1, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 50000.0,
	NullSetup, DoMegastableSystemWith2DStrip, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0018, 1.0, 1.0
	},
	{
	"Megastability: Coexistence of a Countable Infinity of Nested Attractors in a Periodically-Forced Oscillator I", 
	    "real", "imag", ES, ES, ES, ES, ES, ES, ES, ES, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 13.77 * PI, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -800.0, -500.0, 1000.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 40000.0,
	    NullSetup, DoMegastabilityCoexistenceCountableInfinity1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0151, 2.0, 1.0
	},
	{
	"Megastability: Coexistence of a Countable Infinity of Nested Attractors in a Periodically-Forced Oscillator II",
	    "real", "imag", ES, ES, ES, ES, ES, ES, ES, ES, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.4, -4.0, 8.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 900000.0,
	    NullSetup, DoMegastabilityCoexistenceCountableInfinity2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 2.0, 1.0
	},
	{
	"Megastability in a Quasi-Periodically Forced System Exhibiting Multistability and Quasi-Periodic Behaviour", 
	    "a", "b", "real", "imag", ES, ES, ES, ES, ES, ES, sqrt(2.0), sqrt(3.0), 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 9.0 * PI, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -60.0, 120.0, 1, 4, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMegastabilityQuasiPeriodicallyForcedSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00753, 1.0, 1.0
	},
	{
	"Megastability, Multistability in Periodically Forced Conservative & Dissipative System with Signum Nonlinearity", 
	    "real", "imag", "a", "b", "w", ES, ES, ES, ES, ES, 10.0, 10.0, 0.1, 3.071, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.4 * PI, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.6, -1.6, 3.2, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoMegastabilityMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 0.01
	},
	{
	"Meminductive Wein-bridge Chaotic Oscillator", "k", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.0, 2.0, 7, 8, 5, 1, 4, 4, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoMeminductiveWeinbridge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Memristive Chaotic Circuits", "alpha", "beta", "gamma", "d", "c", ES, ES, ES, ES, ES, 4.0, 1.0, 0.65, 10.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, -0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMemristive, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 1.0, 1.0
	},
	{
	"Memristive Hyperchaotic System without Equilibrium", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 5.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.05, 0.05, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 1, 4, 3, 2, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMemristiveHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0017, 1.0, 1.0
	},
	{
	"Memristor-Based Canonical Chua’s Circuit: Extreme Multistability in VC Domain", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 12.375, 7.0213, 2.475, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMemristorBasedCanonicalChuasCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.6, 4.0
	},
	{
	"Memristor-Based Chaotic System with Upper-Lower Chaotic Attractors and Abundant Dynamical Behaviors",
	    "a", "b", "c", "d", "g", "r", "roff", "ron", "m0", "uv", 2.0, 8.2, 5.0, 3.0, 5.0, 10000.0, 20.0, 100.0, 16.0, 1.0e-14,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -141.0, -50.0, 162.0, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMemristorBasedChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 10.0, 1.0
	},
	{
	"Memristor Oscillator Based on a Twin-T Network", "alpha", "k", "r", "a", "b", ES, ES, ES, ES, ES, 7.8, 1.05, 0.6666666667, -0.6, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "u", ES, ES, ES, ES, ES, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 5, 6, 3, 5, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMemristorOscillatorBasedOnTwinTNetwork, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.2, 1.0
	},
	{
	"MEMS Resonator", "alpha", "gamma", "sigma", "w", "mu", ES, ES, ES, ES, ES, 12.0, 0.338, 0.03558, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.35, -0.25, 0.5, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMEMSResonator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.2, 1.0
	},
	{
	"Method for Improving the Imbedded Runge Kutta", "l1", "l2", "l3", ES, ES, ES, ES, ES, ES, ES, 0.5, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 4, 6, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoImprovingRungeKutta, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00007, 7.2, 1.0
	},
	{
	"Minimal Heteroclinic System-I", "a", "b", "w", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.2, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5,  -1.5,  3.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMinimalHeteroclinic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Minimal Heteroclinic System-II", "a", "b", "w", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.2, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 2, 3, 1, 3, 3, 3, NULL, 3, YAXIS, 1000000.0,
	    NullSetup, DoMinimalHeteroclinic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Minimal System with a Depth-Two Heteroclinic Network-Changed", "l", "r", "s", ES, ES, ES, ES, ES, ES, ES, 0.455, 7.0, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.27, 0.4, 2, 4, 1, 3, 3, 3, NULL, 3, YAXIS, 100000.0,
	    NullSetup, DoMinimalHeteroclinicChanged, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"Miranda-Stone 3 Multi-scroll Chaotic Oscillator", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 2.66666667, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 3, 3, 3, NULL, 1, ZAXIS, 10000.0,
	    NullSetup, DoMirandaStone, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Miranda-Stone 4 Multi-scroll Chaotic Oscillator", "r", "sigma", "b", ES, ES, ES, ES, ES, ES, ES, 28.0, 10.0, 2.66666667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "s", "t", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.5, 2.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 3, 3, 3, NULL, 1, ZAXIS, 300000.0,
	    NullSetup, DoMirandaStone4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Model Reference Control of Hyperchaotic Systems", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -10.0, 6.0, -5.5, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -35.0, 48.0, 1, 2, 3, 0, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoModelReferenceControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Modelling & Cct Realisation of a No-Equilibrium Chaotic System with Hidden Attractor & Coexisting Attractors", "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 35.0, 28.0, 3.0, 16.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 30.0, 80.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    20.0, 28.0, 18.4, 3, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoModellingCircuitRealisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00014, 0.4, 0.2
	},
	{
	"Modelling Chaos-Hyperchaos Transition In Coupled Rossler Systems", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.3673, 2.0, 4.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.2, 0.12, 0.19, 0.1, 0.21, 0.26, 0.0, 0.0, 0.0, 0.0,
	    -2.65, -0.25, 6.5, 3, 6, 1, 4, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoModellingChaosHyperchaosTransition, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Modelling of Chaotic Processes with Caputo Fractional Order Derivative", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", ES, 1.0, 0.7, 0.3, 4.0, 4.4, 1.0, 0.1, 10.0, 1.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 1.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -14.0, 20.0, 2, 3, 1, 9, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoModellingChaoticProcessesCaputoFractionalOrderDerivative, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Modelling of a Chaotic System Motion in Video with Artificial Neural Networks", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.12, 0.115, 0.5, -0.005, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.5, 1.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.6, 3.3, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1500000.0,
	    NullSetup, DoModellingChaoticSystemMotionInVideo, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000951, 0.7, 1.0
	},
	{
	"Modern Firm: A Strange Chaotic Oscillator", "v", "m", "n", "rz", "s", ES, ES, ES, ES, ES, 4.0, 0.04, 0.02, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "p", "r", "f", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -20.0, 40.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoModernFirm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 24.0, 1.0
	},
	{
	"Modern Firm: A Strange Chaotic Oscillator - Plus Catastrophe Equations", "v", "m", "n", "rz", "s", "a", "b", "c", "d", ES, 
	    4.0, 0.04, 0.02, 0.1, 0.2, 5.0, -2.0, -0.1, 4.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.6, -1.0, 4.0, 2, 3, 1, 9, 3, 3, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoModernFirmPlusCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.003, 1.0
	},
	{
	"Modified Chaotic Chua Oscillator I", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 9.35, 14.35, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 15.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    10.0, -5.0, 10.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoModifiedChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 4.0, 1.0
	},
	{
	"Modified Chaotic Chua Oscillator II", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 9.35, 14.35, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 10.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -7.0, 12.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoModifiedChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 0.2, 1.0
	},
	{
	"Modified Hyperchaotic Yu Systems I", "alpha", "beta", "gamma", "delta", "epsilon", ES, ES, ES, ES, ES, 
	    10.0, 40.0, 1.0, 3.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 7.0, -2.0, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -20.0, 40.0, 1, 2, 3, 5, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoModifiedHyperchaoticYu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Modified Hyperchaotic Yu Systems II", "alpha", "beta", "gamma", "delta", "epsilon", ES, ES, ES, ES, ES, 
	    10.0, 40.0, 1.0, 3.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 7.0, -2.0, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -10.0, 40.0, 1, 3, 2, 5, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoModifiedHyperchaoticYu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.3, 1.0
	},
	{
	"Modified Hyperchaotic Yu Systems III", "alpha", "beta", "gamma", "delta", "epsilon", ES, ES, ES, ES, ES, 
	    10.0, 40.0, 1.0, 3.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 7.0, -2.0, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -20.0, 40.0, 1, 4, 3, 5, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoModifiedHyperchaoticYu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.05, 1.0
	},
	{
	"Modified Hyperchaotic Yu Systems IV", "alpha", "beta", "gamma", "delta", "epsilon", ES, ES, ES, ES, ES, 
	    10.0, 40.0, 1.0, 3.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 7.0, -2.0, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, 0.0, 70.0, 2, 3, 1, 5, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoModifiedHyperchaoticYu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.8, 1.0
	},
	{
	"Modified Hyperchaotic Yu Systems V", "alpha", "beta", "gamma", "delta", "epsilon", ES, ES, ES, ES, ES, 
	    10.0, 40.0, 1.0, 3.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 7.0, -2.0, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -25.0, 50.0, 3, 4, 2, 5, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoModifiedHyperchaoticYu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.1, 1.0
	},
	{
	"Modified Liu System with Fractional Order", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 1.0, 1.0, -1.0, -4.0, 1.0, -9.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, 4.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -1.0, 40.0, 2, 3, 1, 7, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoModifiedLiu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000125, 0.3, 1.0
	},
	{
	"Modified Projective Synchronisation of New Hyper-chaotic Systems Ia", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 0.5, 0.3, 0.95, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 2.5, 4.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -600.0, -350.0, 700.0, 1, 2, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoModifiedProjective, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003, 0.01, 1.0
	},
	{
	"Modified Projective Synchronisation of New Hyper-chaotic Systems Ib", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 0.5, 0.3, 0.95, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 2.5, 4.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -600.0, -350.0, 700.0, 1, 2, 4, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoModifiedProjective, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003, 0.01, 1.0
	},
	{
	"Modified Projective Synchronisation of New Hyper-chaotic Systems II", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 0.5, 0.3, 0.95, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 2.5, 4.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -600.0, -350.0, 700.0, 1, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoModifiedProjective, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003, 0.01, 1.0
	},
	{
	"Modified Projective Synchronisation of New Hyper-chaotic Systems III", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 0.5, 0.3, 0.95, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 2.5, 4.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -440.0, -300.0, 512.0, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoModifiedProjective, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003, 0.4, 1.0
	},
	{
	"Modified Projective Synchronization of New Hyper-chaotic Systems IV", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 0.5, 0.3, 0.95, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 2.5, 4.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -52000.0, -37000.0, 62000.0, 2, 4, 1, 5, 4, 4, NULL, 1, XAXIS+YAXIS, 500000.0,
	    NullSetup, DoModifiedProjective, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003, 50.0, 1.0
	},
	{
	"Modified Projective Synchronization of New Hyper-chaotic Systems V", 
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 0.5, 0.3, 0.95, 0.8, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 2.5, 4.0, 3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35000.0, -24000.0, 40000.0, 3, 4, 1, 5, 4, 4, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoModifiedProjective, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00003, 30.0, 1.0
	},
	{
	"Modified Tigan System", "a", "b", "c", "p", ES, ES, ES, ES, ES, ES, 1.0, 0.7, 30.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 6.0, 12.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -30.0, 60.0, 1, 2, 3, 4, 3, 3, NULL, 0, XAXIS, 20000.0,
	    NullSetup, DoModifiedTigan, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.06, 1.0
	},
	{
	"Moore-Spiegel Oscillator", "t", "r", ES, ES, ES, ES, ES, ES, ES, ES, 6.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -6.0, 12.0, 1, 2, 3, 2, 3, 3, NULL, 1, XAXIS, 30000.0,
	    NullSetup, DoMooreSpiegel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.25, 1.0
	},
	{
	"Morse Oscillator", "beta", "gamma", "a", "f", "w", ES, ES, ES, ES, ES, 8.0, 0.8, 1.0, 3.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.5, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.25, -0.15, 0.3, 1, 2, 3, 5, 3, 3, NULL, 0, YAXIS+EVOLVE, 60000.0,
	    NullSetup, DoMorseOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 0.1, 1.0
	},
	{
	"Motion of Charged Particles in Magnetic Fields", "ro", "fi", "z", "a", "b", "c", ES, ES, ES, ES, 1.0, 20.0, 0.0000050, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 1, 2, 3, 6, 3, 0, NULL, 1, ZAXIS, 1000000.0,
	    NullSetup, DoMotionMagField, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multi-Number Communication System Based on 6th-Order System of Cellular Neural Network I", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    10.0, 8.0/3.0, 28.0, -2.5, 0.6, -8.0, 9.7, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.01, 0.015, 0.001, 0.02, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 4, 2, 7, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultiNumberCommunication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Multi-Number Communication System Based on 6th-Order System of Cellular Neural Network II", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    10.0, 8.0/3.0, 28.0, -2.5, 0.6, -8.0, 9.7, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.01, 0.015, 0.001, 0.02, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 2, 4, 3, 7, 6, 6, NULL, 0, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoMultiNumberCommunication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Multi-Number Communication System Based on 6th-Order System of Cellular Neural Network III", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    10.0, 8.0/3.0, 28.0, -2.5, 0.6, -8.0, 9.7, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.01, 0.015, 0.001, 0.02, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -8.0, 16.0, 3, 4, 2, 7, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultiNumberCommunication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.7, 1.0
	},
	{
	"Multi-Number Communication System Based on 6th-Order System of Cellular Neural Network IV", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 
	    10.0, 8.0/3.0, 28.0, -2.5, 0.6, -8.0, 9.7, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.01, 0.015, 0.001, 0.02, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 4, 6, 2, 7, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultiNumberCommunication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.05, 1.0
	},
	{
	"Multifolded Torus", "alpha", "beta", "m0", "m1", "x1", ES, ES, ES, ES, ES, 15.0, 1.0, 0.1, -0.07, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.5, 7.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultifoldedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Multi-Lorenz Grid multi-wing butterfly chaotic attractors", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.6, 0.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -8.0, 16.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Multimedia Security Application of a Ten-Term Chaotic System without Equilibrium", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.0, 0.1, 2.5, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -144.0, -80.0, 160.0, 2, 6, 1, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMultimediaSecurityApplication, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.1, 1.0
	},
	{
	"Multiple Attractors and Generalized Synchronization in Delayed Mackey-Glass Systems", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.0, 10.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", ES, ES, ES, ES, ES, ES, ES, ES, ES, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.96, -1.6, 3.2, 2, 1, 1, 1, 1, 1, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoMultipleAttractorsDelayedMackeyGlassSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.471, 1.0, 1.0
	},
	{
	"Multiple Attractors in a Non-Ideal Active Voltage-Controlled Memristor Based Chua's Circuit", 
	    "alpha", "beta", "gamma", "epsilon", "a", "b", ES, ES, ES, ES, 12.0, 28.0, 37.0, 12.0, 1.6, 0.16, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.4, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMultipleAttractorsChua, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.6, 0.6
	},
	{
	"Multiple Delay Rossler System - Bifurcation and Chaos Control", "alpha1", "alpha2", "beta1", "beta2", "gamma", ES, ES, ES, ES, ES, 0.2, 0.5, 0.2, 0.2, 5.7, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -13.0, 28.0, 4, 5, 1, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultipleDelayRosslerSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Multi-Scroll Chaotic Attractors in SC-CNN via Hyperbolic Tangent Function", "s22", "n", "i3", ES, ES, ES, ES, ES, ES, ES, 1.1, 10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.17, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.0, 4.0, 3, 6, 1, 3, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoMultiScrollChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Multipulse Chaotic Dynamics for Nonautonomous Nonlinear Systems and Application to a FGM Plate", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "b1", "b2", "b3", 0.22, 30.0, 1.6168, 0.1029, 1.1270, 0.6129, 0.9438, 0.20, 31.0, 0.5981,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.8913, 0.7621, 0.0664, 0.0185, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.8, 3.6, 1, 5, 7, 10, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoMultipulseChaoticDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multi-Scroll Chaotic Oscillator from the Chen System", "a", "b", "c", "d1", "d2", ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 1.0, 22.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -55.0, -0.0, 64.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiScrollChen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00095, 1.0, 1.0
	},
	{
	"Multistability Analysis, Multiple Attractors, and FPGA Implementation of Yu–Wang 4-Wing Chaotic System", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 7.0, 5.0, 12.0, 60.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -2.0, -1.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -217.0, -123.0, 242.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 250000.0,
	    NullSetup, DoMultistabilityAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Multistability and Coexisting Attractors in a New Circulant Chaotic System", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, -3.0, 9.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.2, 4.5, 2, 4, 1, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultistabilityCoexistingAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"Multi-Switching Combination Synchronisation of Chaotic Systems", "a2", "b2", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.175, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.31, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.35, -0.35, 0.7, 3, 4, 1, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultiSwitchingCombinationSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 0.5
	},
	{
	"Multi-Scroll in Coupled Lorenz Oscillators in 6 Dimensions", "b", "s", "r1", "r2", "e (0.1 to 5)", ES, ES, ES, ES, ES, 
	    3.66666667, 11.0, 35.0, 1.15, 1.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "y1", "z1", "x2", "y2", "z2", ES, ES, ES, ES, 10.0, -12.0, 12.0, -14.0, 15.0, -16.0, 0.0, 0.0, 0.0, 0.0,
	    -75.0, -50.0, 100.0, 1, 2, 3, 5, 6, 6, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiscrollLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Multistability and Bifurcations in a 5D Segmented Disc Dynamo with a Curve of Equilibria", "m", "r", "g", "k1", "k2", "k3", "k4", "k5", "k6", ES,
	    1.3, 1.0, 12.0, 0.01, -0.1, -0.001, -2.3, -1.0, 0.1, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -188.0, -100.0, 200.0, 3, 5, 4, 9, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultistabilityBifurcations5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multistability Analysis, Coexisting Multiple Attractors, and FPGA Imp of Yu–Wang Four-Wing Chaotic System", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES,
	    8.0, 5.0, 12.0, 60.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 10.0, -2.0, -1.0, 1.0, -2.0, -16.0, 0.0, 0.0, 0.0, 0.0,
	    -230.0, -131.0, 257.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoMultistabilityAnalysisCoexistingMultipleAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 1.0, 1.0
	},
	{
	"Multistability & Variable Multi-Directional Hidden Attractors of a 6D Dynamical System with No Equilib Points", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES,
	    0.03, 10.3, 10.1, 13.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.02, 3.1, 0.02, 0.02, 0.02, 0.02, 0.0, 0.0, 0.0, 0.0,
	    -0.087, 12.32, 0.1, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMultistabilityVariableMultiDirectionalHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 4.0, 0.1
	},
	{
	"Multistability in a Butterfly Flow", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.6, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.6, -6.9, 12.5, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultistabilityButterflyFlow, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008685, 1.0, 1.0
	},
	{
	"Multistability in a Chaotic System with Perpendicular Equilibrium: Analysis, Adaptive Sync & Circuit Design", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.6, 3.2, 4, 5, 6, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultistabilityNovelChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0025, 1.0, 1.0
	},
	{
	"Multistability in Horizontal Platform System with and Without Time Delays", "a", "b", "l", "h", "w", ES, ES, ES, ES, ES, 
	    1.333, 3.776, 4.6E-6, 11.333, 1.8, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -3.4, 2.1, 0.1825, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -7.0, 14.0, 1, 5, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Multistability in Self-Reproducing Systems I", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 8.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.16, -0.654, 1.36, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultistabilitySelfReproducingSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multistability in Self-Reproducing Systems II", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 8.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -290.0, -92.0, 186.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultistabilitySelfReproducingSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multistability in Self-Reproducing Systems III", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 8.0, 0.945, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.16, -0.654, 1.36, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultistabilitySelfReproducingSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multistability in Symmetric Chaotic Systems - with Symmetry", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES,
	    0.2, 0.2, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 2.51, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.8, 7.6, 5, 6, 4, 3, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoMultistabilitySymmetricChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 0.6
	},
	{
	"Multistage Spectral Relaxation Method for Solving the Hyperchaotic Complex Systems", "a1", "a2", "a3", "a4", ES, ES, ES, ES, ES, ES,
	    15.0, 36.0, 4.5, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
	    -46.0, -25.0, 53.6, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultistageSpectralRelaxationMethod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00051, 1.0, 1.0
	},
	{
	"Multiswitching Synchronisation of a Driven Hyperchaotic Circuit Using Active Backstepping", "a", "b", "m", "c", "mu", "a0", "w", ES, ES, ES, 
	    0.35, 300.0, 100.0, 0.2, 1.0, 1.51, 10.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -58.0, -32.0, 64.0, 5, 6, 4, 7, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiswitchingSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 0.3, 0.05
	},
	{
	"Multivariate Multiscale Complexity Analysis of Self-Reproducing Chaotic Systems - 14", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    2.8, 4.0, 2.2, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -12.0, 1.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, -3.7, 6.4, 5, 6, 4, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultivariateMultiscaleComplexityAnalysis14, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.3
	},
	{
	"Multivariate Multiscale Complexity Analysis of Self-Reproducing Chaotic Systems - 16", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.6, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.8, 3.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultivariateMultiscaleComplexityAnalysis16, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 0.6
	},
	{
	"Multivariate Multiscale Complexity Analysis of Self-Reproducing Chaotic Systems - 17", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    2.8, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.9, -0.75, 4.4, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMultivariateMultiscaleComplexityAnalysis17, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Multi-Wing Chaotic Lorenz-like System I", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoMultiWingI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Multi-Wing Chaotic Lorenz-like System II", "a", "b", "c", "alpha", ES, ES, ES, ES, ES, ES, 
	    5.0, 10.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.5, 25.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiWingII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000999, 1.0, 1.0
	},
	{
	"Multi-Wing Chaotic Lorenz-like System III", "a", "b", "c", "alpha", ES, ES, ES, ES, ES, ES, 
	    5.0, 10.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -18.0, 36.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiWingIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000999, 1.0, 1.0
	},
	{
	"Multi-Wing Chaotic Lorenz-like System IV", "a", "b", "c", "alpha", ES, ES, ES, ES, ES, ES, 
	    5.0, 5.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.0, 50.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMultiWingIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000999, 1.0, 1.0
	},
	{
	"Multi-Wing Chaotic Lorenz-like System V", "a", "b", "c", "alpha", ES, ES, ES, ES, ES, ES,
	    5.0, 5.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -36.0, 72.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoMultiWingV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000999, 1.0, 1.0
	},
	{
	"Multi-Wing Chaotic Lorenz-like System VI", "a", "b", "c", "alpha", ES, ES, ES, ES, ES, ES,
	    5.0, 5.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -40.0, 80.0, 3, 5, 2, 4, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoMultiWingV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000999, 0.08, 1.0
	},
	{
	"Musical Chaotic Oscillator", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 0.5, 0.6, 0.5, 4.0, 1.35, 1.0, 6.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, 1.95, 0.28, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0, -120.0, 240.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMusical, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Muthuswamy-Chua Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.5, 5.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMuthuswamy, "SelectOscParams", (DLGPROC)SelectOscParams, 0.07538, 1.0, 1.0
	},
	{
	"Neat-Scroll Hyperchaotic Attractor", "alpha", "beta", "gamma", "gammazero", "m0", "m1", "x1", ES, ES, ES, 2.0, 20.0, 1.5, 1.0, 0.2, 6.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, -3.0, 6.0, 4, 1, 2, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewChaosN, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"New Butterfly-Shaped Chaotic Attractor", 
	    "a", "b", "c", "u", ES, ES, ES, ES, ES, ES, 30.0, 15.0, 11.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -50.0, 80.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoNewButterfly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.5, 1.0
	},
	{
	"New Chaos 1-Scroll Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -10.0, -4.0, 11.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -10.0, 40.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNewChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"New Chaos 2-Scroll Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, -10.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -40.0, 80.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.7, 1.0
	},
	{
	"New Chaos N-Scroll Hyperchaotic Attractor", "alpha", "beta", "gamma", "gammazero", "m0", "m1", "x1", ES, ES, ES, 2.0, 20.0, 1.5, 1.0, -0.2, 3.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 4, 2, 1, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewChaosN, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"New Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.0, 12.0, 2.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.8, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -12.0,  20.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoNewChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"New Chaotic Attractor with Quadratic Exponential Nonlinear Term from Chen's Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    35.0, 3.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, 0.0,  12.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.1, 1.0
	},
	{
	"New Chaotic Attractor with Quadratic Exponential Nonlinear Term from Chen's Attractor (Modified)", 
	    "a", "b", "c", "e", ES, ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 1.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -3.0, 30.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewChaoticAttractorMod, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.55, 1.0
	},
	{
	"New Chaotic Attractor with Quadratic Exponential Nonlinear Term from Chen's Attractor in 4 Dimensions", 
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.7, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -110.0, 120.0, 3, 4, 1, 3, 4, 4, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoNewChaoticAttractor4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.0, 1.0
	},
	{
	"New Chaotic Attractors: Application of Fractal-Fractional Differentiation and Integration", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.8, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.54, -0.32,  0.63, 1, 3, 5, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewChaoticAttractorsFractionalDifferentiationIntegration, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00025, 0.5, 0.1
	},
	{
	"New Chaotic Dynamical System (If 'gamma' = 0 we have Lorenz)", "a", "b", "c", "gamma", ES, ES, ES, ES, ES, ES, 
	    10.0, 16.0, -1.0, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "y1", "y2", "y3", "k1", ES, ES, ES, ES, ES, ES, 1.5, -2.0, 3.2, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -16.0, 32.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNewChaoticDynamicSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"New Chaotic Finance System - Its Analysis, Control, Synchronisation and Circuit Design", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    0.6, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, -0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.25, -2.3, 4.6, 3, 4, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewChaoticFinanceSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"New Class of Chaotic Systems with Equilibrium Points like a Three-Leaved Clover I", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    0.29, 1.4, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.54, -0.69, 0.37, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.25, -0.8, 1.4, 1, 5, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewClassChaoticSystemsEquilibriumPointsI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"New Class of Chaotic Systems with Equilibrium Points like a Three-Leaved Clover II", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 
	    0.3, 1.8, 2.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.29, 0.37, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.65, -0.5, 0.8, 3, 5, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewClassChaoticSystemsEquilibriumPointsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"New Class of Chaotic Systems with Equilibrium Points like a Three-Leaved Clover III", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 
	    0.15, 0.2, 2.0, 2.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.19, 0.29, -0.34, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.0, 1, 5, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewClassChaoticSystemsEquilibriumPointsIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"New Family of 4-D Hyperchaotic and Chaotic Systems with Quadric Surfaces of Equilibria - QS6", "h", "d", "g", "a", "b", ES, ES, ES, ES, ES, 
	    5.0, 2.0, 1.0, 1.0, sqrt(1.0/5.0), 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "z", ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.01, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 6, 8, 5, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewFamily4DHyperchaoticChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 25.0, 1.0
	},
	{
	"New Chaotic System - Multiplied By Catastrophe Equations", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 
	    0.4, 12.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 5.0, 0.004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -110.0, -65.0, 130.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNewChaoticCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"New Four-Wing Hyper-chaotic Attractor and Its Circuit Implementation", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 
	    4.0, 6.0, 10.0, 5.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -18.0, 36.0, 2, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewFourWingHyperchaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0006, 1.0, 1.0
	},
	{
	"New Hyperchaotic Attractor with Complex Patterns", 
	    "a", "b", "c", "e", "add trig functions (0 for no)", ES, ES, ES, ES, ES, -2.0, 1.0, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 4, 2, 5, 4, 4, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoNewHyperchaoticComplexPatterns, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"New Hyperchaotic Attractor with Complex Patterns (Marcus Variation)", 
	    "a", "b", "c", "e", "add trig functions (0 for no)", ES, ES, ES, ES, ES, -2.0, 1.0, 0.2, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -2.5, 5.0, 1, 4, 2, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 1000000.0,
	    NullSetup, DoNewHyperchaoticComplexPatterns, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0004, 0.12, 1.0
	},
	{
	"New Jerk Chaotic Circuit", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.000001, 0.026, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.3, 2.5, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoNewJerk, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"New Lorenz's Family Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.5, 2.5, 4.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -25.0, 50.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoNewLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 1.0, 1.0
	},
	{
	"New Memristive Hyperchaotic System I", "a", "b", "mu", ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -8.0, 16.0, 1, 2, 3, 3, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewMemristive, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"New Memristive Hyperchaotic System II", "a", "b", "mu", ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -16.0, 20.0, 3, 4, 1, 3, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoNewMemristive, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"New Quadratic 3D Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 14.0, 8.0/3.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -4.0, 16.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNewQuadratic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"New Simple Chaotic Lorenz-Type System and Its Digital Realisation Using a TFT Touch-Screen Display", "a", "c", "k", ES, ES, ES, ES, ES, ES, ES, 5.0E-3, 6.02E-6, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.2, 1.8, 1, 6, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNewSimpleChaoticLorenzTypeSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"New Three-Dimensional Chaotic System Without Equilibrium Points and Electronic Circuit Application", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 2.8, 0.2, 1.4, 1.0, 10.0, 2.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -4.0, 1.0, -4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -16.0, 36.0, 3, 4, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewThreeDimensionalChaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.2, 1.0
	},
	{
	"New Way to Generate High-Dimensional Hyperchaos (10 Dimensions) I", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k", 10.0, 8.0/3.0, 28.0, 2.5, 0.6, -8.0, 28.0, 3.0, 2.0, -2.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", "s", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	    -3.8, -2.4, 4.0, 4, 5, 7, 10, 10, 10, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHighDimensionalHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 0.7, 1.0
	},
	{
	"New Way to Generate High-Dimensional Hyperchaos (10 Dimensions) II", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k", 10.0, 8.0/3.0, 28.0, 2.5, 0.6, -8.0, 28.0, 3.0, 2.0, -2.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", "s", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	    -4.2, -2.5, 5.0, 5, 6, 7, 10, 10, 10, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoHighDimensionalHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 0.012, 1.0
	},
	{
	"New Way to Generate High-Dimensional Hyperchaos (10 Dimensions) III", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k", 10.0, 8.0/3.0, 28.0, 2.5, 0.6, -8.0, 28.0, 3.0, 2.0, -2.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", "s", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	    -4.2, -2.5, 5.0, 5, 9, 7, 10, 10, 10, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHighDimensionalHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 1.2, 1.0
	},
	{
	"Newton-Leipnik Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.175, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.349, 0.0, -0.16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.2, 3.5, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewtonLeipnik, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 3.0, 1.0
	},
	{
	"No Chattering and Adaptive Sliding Mode Control of a Fractional-Order Phase Converter with Disturbances", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.1, 0.25, 0.75, 3.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -8.0, 16.0, 2, 7, 4, 5, 5, 5, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoNoChatteringAdaptiveSlidingModeControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00168, 0.3, 1.0
	},
	{
	"Non-Integrability of a System with the Dyson Potential", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.13, 0.5, 35.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -100.0, 200.0, 4, 8, 2, 0, 4, 4, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoNonIntegrabilityDysonPotential, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Nonlinear Active Element Dedicated to Modeling Chaotic Dynamics with Complex Polynomial Vector Fields", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.9, 0.42, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.37, -0.15, 0.53, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoNewNonlinearActiveElement, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 1.0, 1.0
	},
	{
	"Nonlinear Arabesques I", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.8, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.8, 3.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNonLinearArabesquesI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00002, 1.0, 1.0
	},
	{
	"Nonlinear Arabesques II", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.8, -0.8, -0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.8, 3.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNonLinearArabesquesII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Nonlinear Arabesques III", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", ES, ES, ES, ES, ES, ES, -0.8, -0.8, -0.08, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.6, 3.0, 1, 2, 3, 0, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNonLinearArabesquesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Nonlinear Dynamical Model with Three Quadratic Nonlinear Terms and Hidden Chaos",
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -4.0, 9.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNonlinearDynamicalModelThreeQuadraticNonlinearTerms, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Nonautonomous Predator-Prey Model with Infertility Control in the Prey", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.5, 8.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 8000.0,
	    NullSetup, DoNonautonomousPredatorPrey, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Nonautonomous Wien-Bridge Oscillator", "f1", "f2", "b", "w1", "w2", "k", ES, ES, ES, ES, 0.1, 2.55, 32.0, 5.0, 4.3, 3.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 6.0, 1, 3, 2, 6, 3, 3, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoNonautonomousWienBridge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Nonlinear Dynamics and Chaos in a Fractional-Order HIV Model", "a", "lambda", "b", "c", "delta", "d", "beta 1", "beta 2", ES, ES, 0.045, 10.0, 0.8, 0.8, 0.03, 0.02, 4.0 * 0.0001, 2.8 * 0.0001, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 500.0, 200.5, 300.1, 300.0, 500.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    74.0, -38.0, 290.0, 1, 2, 3, 8, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoNonLinearDynamicsFractionalOrderHIVModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.03, 1.0, 1.0
	},
	{
	"Nonlinear Dynamics of a SDOF Oscillator with Bouc-Wen Hysteresis", "n", "w", "g", "a", "b", "alpha", "beta", "p", ES, ES, 
	    0.02, 1.0, 0.5, 1.0, 1.0, 0.075, -0.93, 0.1, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, -0.1, 0.5, 0.1, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -20.0, 50.0, 3, 4, 2, 8, 4, 4, NULL, 1, XAXIS, 4000.0,
	    NullSetup, DoNonlinearDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 3.0, 1.0
	},
	{
	"Nonlinear Gear Systems I", "b", "c", "e", "delta", "beta", "mi", ES, ES, ES, ES, 
	    0.1422, 5.8093, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, -0.1, -0.3, 0.4, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.85, -0.5, 1.0, 2, 4, 3, 6, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNonlinearGear, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.6, 1.0
	},
	{
	"Nonlinear Gear Systems II", "b", "c", "e", "delta", "beta", "mi", ES, ES, ES, ES, 
	    0.1422, 5.8093, exp(1.0), 0.2, 0.4, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, -0.1, -0.3, 0.4, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.4, 0.8, 1, 4, 3, 6, 4, 4, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoNonlinearGear, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.03, 1.0
	},
	{
	"Nonsmooth Bifurcations, Transient Hyperchaos and Hyperchaotic Beats in Memristive Murali-Lakshmanan", "a1", "a2", "beta", "omega", "f", ES, ES, ES, ES, ES, 
	    0.465116, -0.120289, 0.163613, 0.325940, 0.747114, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.5, -2.5, -2.5, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -23.0, -15.0, 28.0, 3, 6, 2, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNonsmoothBifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Nonstationary Chimeras in a Neuronal Network", "im", "f", ES, ES, ES, ES, ES, ES, ES, ES, 1.2, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.5, 1.5, 2.81, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.5, 2.5, 1, 4, 2, 2, 3, 3, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoNonstationaryChimeras, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 0.2, 0.1
	},
	{
	"Nose-Hoover Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 2, 3, 1, 1, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoNoseHoover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 1.0, 1.0
	},
	{
	"Nose-Hoover 4D Version", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -4.2, 20.0, 1, 3, 4, 0, 4, 4, NULL, 0, YAXIS, 90000.0,
	    NullSetup, DoNoseHoover4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Novel 3-Scroll Chua’s Attractor with One Saddle-Focus and Two Stable Node-Foci", 
	    "alpha", "beta", "a", "b", "mu", ES, ES, ES, ES, ES, 5.8, 5.8, -1.1, 0.02, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.42, -0.25, 0.5, 2, 3, 5, 5, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoNovel3ScrollChuasAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Novel 4-Dimensional Hyperchaotic Attractor with Typical Wings I", "alpha", "beta", "phi", "s", "psi", ES, ES, ES, ES, ES, 
	    1.0, -0.7, -0.1, 1.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -1.0, 12.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNovel4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 2.0, 1.0
	},
	{
	"Novel 4-Dimensional Hyperchaotic Attractor with Typical Wings II", "alpha", "beta", "phi", "s", "psi", ES, ES, ES, ES, ES, 
	    1.0, -0.7, -0.1, 1.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 3, 2, 5, 4, 4, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNovel4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.75, 1.0
	},
	{
	"Novel 4-Dimensional Hyperchaotic Attractor with Typical Wings III", "alpha", "beta", "phi", "s", "psi", ES, ES, ES, ES, ES, 
	    1.0, -0.7, -0.1, 1.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNovel4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 4.0, 1.0
	},
	{
	"Novel 4-Dimensional Hyperchaotic Attractor with Typical Wings IV", "alpha", "beta", "phi", "s", "psi", ES, ES, ES, ES, ES, 
	    1.0, -0.7, -0.1, 1.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -3.0, 6.0, 2, 4, 3, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 5000000.0,
	    NullSetup, DoNovel4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 2.0, 1.0
	},
	{
	"Novel 4-Dimensional Hyperchaotic Attractor with Typical Wings V", "alpha", "beta", "phi", "s", "psi", ES, ES, ES, ES, ES, 
	    1.0, -0.7, -0.1, 1.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 3, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNovel4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 3.0, 1.0
	},
	{
	"Novel Chaotic System", "a", "b", "c", "k", "p",  ES, ES, ES, ES, ES, 5.0, 10.0, 3.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 6.0, 6.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -25.0, 50.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoNovel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 1.0, 1.0
	},
	{
	"Novel Chaotic System Without Equilibrium: Dynamics, Synchronisation, and Circuit Realisation", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.35, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.3, 0.8, 4, 6, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNovelChaoticSystemWithoutEquilibrium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Novel Control Method for Integer Orders Chaos Systems via Fractional-Order Derivative", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES,  ES, ES, ES, ES, ES, 0.1, -0.4, 1.9, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -5.0, 50.0, 2, 3, 4, 0, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNovelControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Novel Dynamical Behaviors in Fractional-Order Conservative Hyperchaotic System and DSP Implementation",
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 2.8, 1.3, 6.7, -2.8, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.32, -0.185, 0.37, 2, 3, 4, 5, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNovelDynamicalBehaviors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.3, 0.1
	},
	{
	"Novel Four-Wing Hyper-Chaotic System and Its Circuit Implementation", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, -100.0, -14.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -205.0, -115.0, 250.0, 1, 6, 2, 4, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoNovel4WingHyperChaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.03, 1.0
	},
	{
	"Novel Four-Wing Strange Attractor Born in Bistability", 
	    "a", "b", "M", ES, ES,  ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, -140.0, 180.0, 5, 6, 4, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNovelFourWing, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Novel Hyperjerk 4D System", 
	    "a", "b", "c", ES, ES,  ES, ES, ES, ES, ES, 1.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.4, 0.3, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 2, 3, 4, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNovelHyperjerk, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Novel Multiwing Fractional-order Chaotic System, its Sync Control & Application in Secure Communication",
	    "a", "b", "c", "r1", "r2", "r3", "s1", "s2", "s3", ES, 2.0, 0.6, 0.49, 1.1, 2.1, 3.1, 1.3, 2.3, 3.3, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.1, -2.3, 6.77, 1, 3, 2, 9, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNovelMultiwingFractionalorderChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Novel Strange Attractor and its Dynamic Analysis I", "a", "b", "c", "e", ES,  ES, ES, ES, ES, ES, 
	    60.0, 10.0, 15.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNovelStrange, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.8, 1.0
	},
	{
	"Novel Strange Attractor and its Dynamic Analysis II", "a", "b", "c", "e", ES,  ES, ES, ES, ES, ES, 
	    60.0, 10.0, 15.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -640.0, -400.0, 800.0, 4, 6, 5, 4, 3, 3, NULL, 01, YAXIS, 100000.0,
	    NullSetup, DoNovelStrange, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Novel Strange Attractor Minus Catastrophe Equations", "a", "b", "c", "e", ES,  ES, ES, ES, ES, ES, 
	    60.0, 10.0, 15.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -500.0, -300.0, 420.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 90000.0,
	    NullSetup, DoNovelMinusCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Novel Three Dimension Autonomous Chaotic System as Input Data to Catastrophe's Equations I", 
	    "a", "b", "c", "d", "v", "k", "Multiply or add? 1 for multiply, 0 otherwise", ES, ES, ES, 
	    10.0, 40.0, 2.0, 2.5, -3.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.2, 2.4, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, 12.0, 50.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNovel3DCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 0.4, 1.0
	},
	{
	"Novel Three Dimension Autonomous Chaotic System as Input Data to Catastrophe's Equations II", 
	    "a", "b", "c", "d", "v", "k", "Multiply or add? 1 for multiply, 0 otherwise", ES, ES, ES, 
	    10.0, 40.0, 2.0, 2.5, -3.0, 1.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.2, 2.4, 28.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -75.0, 200.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNovel3DCatastrophe, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 0.1, 1.0
	},
	{
	"Numerical Analysis of a Simplest Fractional-Order Hyperchaotic System", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.25, 2.5, 4, 6, 1, 2, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNumericalAnalysisSimplestFractionalOrderHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 0.2, 0.2
	},
	{
	"Numerical & Experimental Confirmations of Quasi-Periodic Behavior in 3rd-Order Memristive Oscillator", 
	    "a", "b", "k", ES, ES, ES, ES, ES, ES, ES, 0.0895, 2.4082E-4, 3.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 4.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -3.0, 6.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoNumericalExperimentalConfirmations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0011, 0.1, 0.05
	},
	{
	"Numerical & Experimental Confirmations of Quasi-Periodic Behavior in 3rd-Order Memristive Oscillator II", 
	    "a", "b", "k", ES, ES, ES, ES, ES, ES, ES, 0.188, 2.4082*0.0006, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.25, 0.5, 4, 5, 6, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNumericalExperimentalConfirmations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.016, 0.03, 0.01
	},
	{
	"Numerical Sensitivity Analysis and Hardware Verification of a Transiently-Chaotic Attractor",
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -634.0, -665.0, 746.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNumericalSensitivityAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 4.0, 1.0
	},
	{
	"Numerical Simulation of a Class of Hyperchaotic System Using Barycentric Lagrange Interpolation Method",
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.3, 0.5, 0.8, 0.063, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoNumericalSimulation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Nutrient-Phyloplankton-Zooplankton", "r0", "k", "a", "r1", "r2", "c1", "b1", "d1", "c2", "d2", 1.0, 1.0, 1.63, 0.01, 0.001, 0.95, 0.33, 0.4, 0.9, 0.01,
	    "n", "p", "z", ES, ES, ES, ES, ES, ES, ES, 0.7, 0.2, 10.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.4, 1.5, 1, 2, 3, 10, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoNutrient, "SelectOscParams", (DLGPROC)SelectOscParams, 0.06, 1.5, 1.0
	},
	{
	"Oanceo Chaotic Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 30.0, 10.0, 1.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -5.0, 20.0, 3, 4, 2, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoOanceo, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Occasional Uncoupling Overcomes Measure Desynchronisation", "kqq", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0140, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.18, 0.36, 7, 8, 6, 1, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoOccasionalUncoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Offset Boosting for Breeding Conditional Symmetry - ACS4 Model", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -4.0, 7.2, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoOffsetBoostingBreedingConditionalSymmetry, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0071, 0.3, 0.4
	},
	{
	"Olsen Chaotic Attractor", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "a0", ES, 0.35, 250.0, 0.0315, 20.0, 5.35, exp(-5.0), 0.1, 0.825, 8.0, 0.0,
	    "a", "b", "x", "y", ES, ES, ES, ES, ES, ES, 6.0, 58.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, 40.0, 20.0, 4, 2, 1, 9, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoOlsen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"On Offset Boosting in Chaotic System – Equation 7", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.55, 0.5, 0.001, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "a", "b", "x", "y", ES, ES, ES, ES, ES, ES, 6.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.05, 3.5, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoOnOffsetBoostingChaoticSystem7, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008751, 1.0, 1.0
	},
	{
	"On the Transition to Hyperchaos and the Structure of Hyperchaotic Attractors", "beta", "gamma", "z magnitude", ES, ES, ES, ES, ES, ES, ES, 
	    2.0, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,	
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.8, 3.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoOnTransition2Hyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.021, 0.7, 1.0
	},
	{
	"One Scroll Chaotic Attractor", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 2.4, -3.0, 14.0, -11.0, 4.0, 7.0, -1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, 4.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -4.0, 8.0, 1, 3, 2, 7, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoOneScroll, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000125, 1.0, 1.0
	},
	{
	"One-Way Coupled Van der Pol System", "mu", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.53, 1.0, 4, 6, 3, 1, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoOneWayCoupledVanderPolSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"One to Four-Wing Chaotic Attractors from a Novel 3D Fractional Chaotic System with Complex Dynamics I", 
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 3.0, 11.0, 8.0, 0.5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -9.4, 15.0, 1, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoOne2FourWingChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"One to Four-Wing Chaotic Attractors from a Novel 3D Fractional Chaotic System with Complex Dynamics II", 
	    "a", "b", "c", "d", "k", ES, ES, ES, ES, ES, 5.0, 10.0, 1.0, 0.05, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -1.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0, -120.0, 260.0, 5, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoOne2FourWingChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"On (non) Elementary Singularities in Liénard Systems", "a1", "a3", "c1", "c3", "c5", ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -1.4, 5.6, 1, 2, 3, 5, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoOnNonElementarySingularities, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"On a 3-D Generalised Hamiltonian Model with Conservative and Dissipative Chaotic Flows", "a", "b", "c", "u", ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.25, 2.5, 1, 3, 2, 4, 3, 3, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoOnA3DGeneralisedHamiltoniansModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"One to Four-Wing Chaotic Attractors Coined from a 3D Fractional-Order System with Complex Dynamics", "a1", "b1", "a2", "b2", "c", "d", "e", ES, ES, ES, 0.15, 10.0, -0.2, 0.6, 8.96, 4.0, 6.91, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.5, 3.0, 4, 5, 6, 7, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoOne2FourWingChaoticAttractorsCoinedNovel3DFractionalOrderChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"On Different Families of Hidden Chaotic Attractors in Fractional Order Dynamical Systems", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.006, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -0.4, 3.6, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoOnDifferentFamiliesHiddenChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.2
	},
	{
	"On the Formation of Hidden Chaotic Attractors and Nested Invariant Tori in the Sprott A System", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.025, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -2.8, 5.6, 2, 3, 1, 1, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoOnFormationHiddenChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Optimal Linear Control in the Chaos Stabilisation in the Mathieu-Van Der Pol Autonomous Oscillator", 
	    "a", "b", "c", "e", "g", ES, ES, ES, ES, ES, 91.7, 5.023, 0.01, 87.001, 9.5072, 0.0, 0.0, 0.0, 0.0, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.15, 0.1, -0.15, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -22.0, 44.0, 2, 3, 1, 5, 4, 4, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoOptimalLinearControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0007, 15.0, 1.0
	},
	{
	"Optimal Synchronisation of Circulant and Non-Circulant Oscillators I",
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 3.98, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.6, 3.6, 5, 6, 4, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoOptimalSynchronisationOscillatorsI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Optimal Synchronisation of Circulant and Non-Circulant Oscillators II",
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 3.98, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.77, 0.35, 1.13, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.14, -1.28, 2.8, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoOptimalSynchronisationOscillatorsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00025, 1.0, 1.0
	},
	{
	"Orchestra Chaotic Attractor", "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, 0.5, 1.0, 1.0, 4.0, 1.2, 1.0, 6.0, 0.1, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, 1.0, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, -100.0, 200.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoOrchestra, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Order-of-Chaos Attractor in 4 Dimensions", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.55, 0.2417, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8,  -1.0,  2.4, 1, 2, 3, 0, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoOrderChaos4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Oregonator CNN  Near Chaos", "k", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.4, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoOregonator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"Oyster Oscillator", "w", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -52.0, -32.0, 64.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoOysterOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Pang Hyper-Chaotic Attractor in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 36.0, 3.0, 20.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 10.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -40.0, 80.0, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoPang4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Parallelising Boundary Surface Computation of Chua's Circuit", "c1", "c2", "r", "l", "bp", "b0", "ro", "m0", "m1", "m2", 0.1, 2.0, 1.428, 1.0/7.0, 1.0, 900.0, 0.0, -4.0, -0.1, 5.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -9.0, 18.0, 1, 4, 3, 10, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoParallelisingBoundarySurfaceComputationChuasCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 0.4
	},
	{
	"Parameter Identification of Fractional-Order Discrete Chaotic Systems - Cat Map", "n", "a", "b", ES, ES, ES, ES, ES, ES, ES, 258.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 13.0, 13.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -240.0, 480.0, 4, 5, 6, 3, 3, 3, NULL, 0, YAXIS, 10000.0,
	    NullSetup, DoParameterIdentificationFractionalOrderDiscreteChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.9, 0.8, 1.0
	},
	{
	"Parameter-Independent Dynamical Behaviours in Memristor-Based Wien-Bridge Oscillator", "a", "b", "p", "q", "k", ES, ES, ES, ES, ES, 250.0 / 17.0, 5.0, 1.25, 0.05, 2.2, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 0.1, 0.1, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0,  -25.0,  50.0, 4, 5, 6, 5, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoParameterIndependentDynamicalBehaviors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Particle in a toroidal magnetic DISTURBED system", "p", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.75, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "r", "z", "m", "w", ES, ES, ES, ES, ES, ES, 0.6, 0.1, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6,  -1.0,  2.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoResonance, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 3.0, 1.0
	},
	{
	"Peculiarities of Transition to Chaos in Nonideal Hydrodynamics Systems", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "y1", "y2", "y3", ES, ES, ES, ES, 0.11, 0.96, 18.98, -0.69, 1.96, -11.09, 0.0, 0.0, 0.0, 0.0,
	    -10.0,  -1.0,  12.0, 6, 3, 2, 0, 6, 6, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPeculiarities, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.2, 1.0
	},
	{
	"Pendulum (Non-ideal) Chaotic Oscillator", "c", "d", "e", "f", "gamma", "delta", ES, ES, ES, ES, -0.1, -0.58, -0.6, -0.19, 0.1652, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.25, 0.4, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0,  -10.0,  12.0, 2, 3, 1, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPendulum, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Pendulum-Delayed Chaotic Oscillator", "c", "e", "f", "d", "g", "gamma", "delta", ES, ES, ES, -0.1, -0.6, 0.19, -0.8, 1.0, 0.29, 0.1, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5,  -0.25,  0.5, 1, 2, 3, 7, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoPendulumDelayed, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Pendulum Diverse 4D Chaotic Oscillator", "t1", "t2", "l1", "l2", ES, ES, ES, ES, ES, ES, 1.2, 1.0, 100.0, 60.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.7,  0.72,  0.4, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoPendulumDiverse4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Pendulum-Modified Chaotic Oscillator", "f", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.114, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -10.0, 20.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPendulumModified, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Pendulum Systems with Limited Excitation", "c", "d", "e", "f", "g", "delta", ES, ES, ES, ES, -0.1, -0.6, -0.44, 0.3, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -650.0, -450.0, 750.0, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 80000.0,
	    NullSetup, DoPendulumSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.2, 1.0
	},
	{
	"Performance-Enhancing of RSA Public Key via 3D Hyperchaotic System-added 2 COS functions", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 20.0, 12.2, 15.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -98115.0, -5423.0, 120000.0, 1, 2, 3, 3, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoPerformanceEnhancingRSAPublicKey, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 0.01, 0.01
	},
	{
	"Period-Doubling Route to Chaos, Bistability and Antimononicity in a Jerk Circuit with Quintic Nonlinearity", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.54, 0.55, 0.0, 0.0, 0.0, 0., 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.51, -0.855, 1.73, 3, 6, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPeriodDoublingRoute2Chaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.8, 1.0
	},
	{
	"Periodic Offset Boosting for Attractor Self-Reproducing", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.96, 7.58, 0.0, 0.0, 0.0, 0., 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.3, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.26, -1.13, 2.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPeriodicOffsetBoostingAttractorSelfReproducing, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Periodic Solution of a Higher Dimensional Ecological System", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.6, 3.2, 5, 6, 7, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPeriodicSolutionHigherDimensionalEcologicalSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.1, 1.0
	},
	{
	"Periodically Forced Chaotic System with Signum Nonlinearity", "a", "b", "f", "w", ES, ES, ES, ES, ES, ES, 1.05, 1.0, 1.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, -0.7, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -9.0, 18.0, 1, 5, 2, 6, 3, 3, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoPeriodicallyForcedChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.0, 1.0
	},
	{
	"Periodically Switched Memristor Initial Boosting Behaviours in Memristive Hypogenetic Jerk System", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -3.0, 0.01, 0.01, -0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.7, -2.4, 6.0, 1, 4, 2, 0, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoPeriodicallySwitchedMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Perpetual Points and Periodic Perpetual Loci in Maps-in Cylindrical Coordinates", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.167, 0.267, 0.167, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -7.0, 14.0, 1, 3, 2, 0, 3, 3, NULL, 0, YAXIS, 300000.0,
	    NullSetup, DoPerpetualPointsPeriodicPerpetualLoci, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0006, 1.0, 1.0
	},
	{
	"Phase Coherence and Attractor Geometry of Hyperchaotic Electrochemical Oscillators I", 
	    "r", "c", "a", "b", "q", "g1", "g2", "alpha", "v", ES, 50.0, 1200.0, 0.3, 6.0e-5, 0.001, 0.01, 0.8, 1.03, 60.8, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, -1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -1.5, 9.0, 1, 2, 4, 9, 4, 4, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoPhaseCoherence, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 8.0, 1.0
	},
	{
	"Phase Coherence and Attractor Geometry of Hyperchaotic Electrochemical Oscillators II", 
	    "r", "c", "a", "b", "q", "g1", "g2", "alpha", "v", ES, 50.0, 1200.0, 0.3, 6.0e-5, 0.001, 0.01, 0.8, 1.03, 60.8, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, -1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -1.0, 6.0, 1, 3, 4, 9, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPhaseCoherence, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 28.0, 1.0
	},
	{
	"Phase Coherence and Attractor Geometry of Hyperchaotic Electrochemical Oscillators III", 
	    "r", "c", "a", "b", "q", "g1", "g2", "alpha", "v", ES, 50.0, 1200.0, 0.3, 6.0e-5, 0.001, 0.01, 0.8, 1.03, 60.8, 0.0,  
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, -1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.11, -0.93, 0.2, 3, 4, 1, 9, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPhaseCoherence, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.6, 1.0
	},
	{
	"Pickover Chaotic Oscillator", "a (try between -42 to +42)", "b (try between -1 and +1)", "c", "d", ES, ES, ES, ES, ES, ES, 
	    42.24, 0.33, -0.55, -1.43, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -5.0, 15.0, 3, 1, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoPickoverOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Pickover Chaotic Oscillator (different value of b)", "a (try between -42 to +42)", "b (try between -1 and +1)", "c", "d", ES, ES, ES, ES, ES, ES, 
	    42.24, -0.5, -0.55, -1.43, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -16.0, 20.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoPickoverOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Piecewise Linear Transformation Fractal Map Implemented as an Oscillator", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.001, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.1, 1.2, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS+EVOLVE, 1000.0,
	    NullSetup, DoPiecewiseLinear, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Piecewise Linear Hyperchaotic Circuit", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.13, 0.5, 0.4, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -5.0, 10.0, 3, 6, 2, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPiecewiseLinearHyperchaoticI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Piecewise Linear Hyperchaotic Circuit - Marcus Modifications", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.44, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.13, 0.5, 0.4, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -4.6, 10.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPiecewiseLinearHyperchaoticII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Pinning Hybrid Synchronisation of Time-Delay Hyperchaotic Lu Systems via Single Linear Control", "a", "b", "c", "alpha1", "alpha2", "tau", ES, ES, ES, ES, 
	    35.0, -3.0, -20.0, 2.0, 2.0, 100.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -15.0, 50.0, 1, 7, 2, 6, 4, 4, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoPinningHybridSynchronization, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 0.01, 1.0
	},
	{
	"PolyWings Hyper-Chaotic Attractor in 4 Dimensions", "a", "d", "e", "Number of Wings (1 - 4)", ES, ES, ES, ES, ES, ES, 
	    10.0, 2.0, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 2.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0, -160.0, 320.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoPolyWings4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Predator-Prey System with Impulsive Perturbations", "r", "a", "b", "k", "m", "d", "v1", "v2", ES, ES, 0.2, 0.9, 4.0, 2.0, 1.1, 0.3, 0.2, 0.2, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.5, -3.8, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.06, -0.04, 0.2, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPredatorPrey, "SelectOscParams", (DLGPROC)SelectOscParams, 0.009, 1.0, 1.0
	},
	{
	"Predictability of Threshold Exceedances in Dynamical Systems", "f", ES, ES, ES, ES, ES, ES, ES, ES, ES, 23.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.6, 0.24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -1.25, 2.5, 1, 2, 3, 1, 3, 3, NULL, 0, YAXIS, 300000.0,
	    NullSetup, DoPredictabilityThresholdExceedance, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Prey-Predator-Top Predator Model", "a1", "b1", "w", "i", "a", "a2", "w1", "w2", "c", "w3", 1.0, 1.0, 1.95, 0.3, 1.0, 0.2, 1.38, 2.85, 0.25, 1.6,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.15, 1.6, 1, 3, 2, 10, 3, 3, NULL, 0, YAXIS, 5000000.0,
	    NullSetup, DoPreyPredatorTop, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 6.0, 1.0
	},
	{
	"Probabilistic Concepts in a Changing Climate: A Snapshot Attractor Picture I", "a", "b", "g", "f0", "azao", 
	    ES, ES, ES, ES, ES, 0.25, 4.0, 1.0, 5.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 3.4, -1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoProbabilisticConceptsI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Probabilistic Concepts in a Changing Climate: A Snapshot Attractor Picture II", "a", "b", "g", "tst", 
	    ES, ES, ES, ES, ES, ES, 0.25, 4.0, 1.0, 73000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 6.0, 12.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -12.0, 24.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoProbabilisticConceptsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.6, 1.0
	},
	{
	"Proto-Lorenz System", "r", "sigma", "b", ES, ES, ES, ES, ES, ES, ES, 28.0, 1.0, 2.66666667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "s", "t", "z", ES, ES, ES, ES, ES, ES, ES, 25.0, -1.5, -2.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -30.0, 60.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoMirandaStone4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Prototypes of Attractors in Four Dimensions", "k1", "k2", "b", "c1", "c2", ES, ES, ES, ES, ES, -0.04, 0.02, 34.0, -0.11, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -17.0, -10.0, 20.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoPrototypesAttractors4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.2, 1.0
	},
	{
	"Pseudo-Random Number Generator based on a Generalised Conservative Sprott-A System", "k", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.75,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.1, -2.1, 2.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.1, -3.5, 7.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPseudoRandomNumberGenerator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Pulse Excited Chaotic Oscillator I", "k1", "k2", "kp", "kf", "fi", "n", "Multiply time? 1 for yes, 0 otherwise", ES, ES, ES, 0.0, 1.0, 1.0, 2.0, 0.85, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.75, -0.5,  1.0, 1, 2, 3, 7, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoPulseExcited, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 10.0, 1.0
	},
	{
	"Pulse Excited Chaotic Oscillator II", "k1", "k2", "kp", "kf", "fi", "n", "Multiply time? 1 for yes, 0 otherwise", ES, ES, ES, 0.85, 1.0, 1.0, 2.0, 0.85, 0.1, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoPulseExcited, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 2.5, 1.0
	},
	{
	"Pulse Excited Chaotic Oscillator III", "k1", "k2", "kp", "kf", "fi", "n", "Multiply time? 1 for yes, 0 otherwise", ES, ES, ES, -0.1, 1.0, 1.0, 2.0, 0.85, 0.1, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.04, -0.01, 0.04, 1, 2, 3, 7, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoPulseExcited, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 10.0, 1.0
	},
	{
	"Pulse Excited Chaotic Oscillator IV", "k1", "k2", "kp", "kf", "fi", "n", "Multiply time? 1 for yes, 0 otherwise", ES, ES, ES, 0.85, 1.0, 1.0, 2.0, 0.85, 0.1, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.5, 2.0, 1, 3, 2, 7, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoPulseExcited, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.01, 1.0
	},
	{
	"Qi-3D Chaotic Attractor", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 16.0, 43.0, -16.0, 16.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, -4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -300.0,  -250.0,  500.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoQi3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Qi-Chen Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 38.0, 2.666667, 80.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0,  -100.0,  200.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoQiChen, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Qi-Slave Chaotic Attractor", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 50.0, 24.0, 13.0, 8.0, 33.0, 30.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 6.0, 24.0, -7.0, 43.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -800.0,  -600.0,  1200.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoQiSlave, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Qi-Slave-Marcus Chaotic Attractor", "a", "b", "c", "d", "e", "f", "k", "r", ES, ES, 48.2, 16.0, 49.0, 9.0, 20.0, 40.0, 8.0, -17.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 21.0, 5.0, 50.0, -910.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -720.0,  -400.0,  800.0, 1, 2, 3, 8, 4, 4, NULL, 1, ZAXIS+EVOLVE, 50000.0,
	    NullSetup, DoQiSlaveMarcus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 10.0, 1.0
	},
	{
	"Quadratic Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.017, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.9, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -4.0, 7.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoQuadratic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Quadratic Equations", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 0.1, -2.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -750.0, -50.0, 600.0, 2, 3, 1, 0, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoQuadraticEquations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 2.0, 1.0
	},
	{
	"Quantum Cellular Neural Network Model", "beta1", "beta1", "teta1", "teta2", ES, ES, ES, ES, ES, ES, 0.8, 0.5, 0.5, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.08, 0.05, -0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -13.0, 16.0, 3, 4, 1, 4, 4, 4, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoQuantumCellular, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.4, 1.0
	},
	{
	"Quasiperiodicity and Suppression of Multistability in Nonlinear Dynamical Systems", "k", "v", "p", "w1", "w2", ES, ES, ES, ES, ES, 
	    0.8, 0.55, 0.9702, (sqrt(5.0) - 1.0) / 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, PI, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.2, 2.4, 2, 5, 3, 5, 3, 3, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoQuasiperiodicitySuppressionMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Rabbits and Foxes Predator-Prey Modeling", "Rabbit Birthrate", "Fox Birthrate", "Rabbit Deathrate", "Fox Deathrate", 
	    ES, ES, ES, ES, ES, ES, 0.2, 0.001, 0.001, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "Number Rabbits (100 - 1000)", "Number Foxes (100 - 300)", "z", ES, ES, ES, ES, ES, ES, ES, 500.0, 190.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1800.0, -400.0, 4000.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoRabbitsAndFoxes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
/*
	{
	"Rabinovich-Fabrikant Limited Cycle Chaotic Oscillator",  "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.84, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -2.5, 5.0, 1, 2, 3, 2, 3, NULL, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoRabinovich, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
*/
	{
	"Rabinovich-Fabrikant Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.84, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, -1.1881, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoRabinovich, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Rabinovich-Fabrikant System-IV", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 4, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoRabinovichSystemIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.000725, 0.5, 1.0
	},
	{
	"Rayleigh-Bénard Chemical Oscillator", "a", "r", "b", ES, ES, ES, ES, ES, ES, ES, 9.0, 12.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -16.0, 30.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoRayleighBenard, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.8, 1.0
	},
	{
	"Rayleigh-Bénard Convection Model in 9 Dimensions", "r", "a", "s", ES, ES, ES, ES, ES, ES, ES, 14.22, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "y1", "z1", "x2", "y2", "z2", "x3", "y3", "z3", ES, 0.01, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.01, 0.0,
	    -1.0, -1.8, 2.5, 1, 2, 3, 3, 9, 9, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoRayleighBenard9D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Recent New Examples of Hidden Attractors-Case A", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.6, 0.82, 1.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoRecentNewHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.7, 1.0
	},
	{
	"Research on a New 3D Autonomous Chaotic System with Coexisting Attractors", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 
	    7.0, 4.5, 8.2, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 5, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoResearchNew3DAutonomousChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.05, 1.0
	},
	{
	"Research on a 3D Predator-Prey Evolutionary System in Real Estate Market I", "a", "b", "c", "d", "e", "f", "g", "h", "k", ES, 
	    0.44, 1.14, 2.93, 0.54, 0.21, 0.1, 0.35, 0.2, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.725, 0.321, 0.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.3, -0.125, 0.25, 1, 4, 2, 9, 3, 3, NULL, 1+AVERAGE, YAXIS, 1000000.0,
	    NullSetup, DoResearch3DPredatorPreyEvolutionarySystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.6, 1.0
	},
	{
	"Research on a 3D Predator-Prey Evolutionary System in Real Estate Market II", "a", "b", "c", "d", "e", "f", "g", "h", "k", ES, 
	    0.44, 1.14, 2.93, 0.54, 0.21, 0.1, 0.35, 0.2, 0.25, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.15, -0.1, 0.2, 5, 6, 4, 9, 3, 3, NULL, 1+AVERAGE, YAXIS, 200000.0,
	    NullSetup, DoResearch3DPredatorPreyEvolutionarySystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.007, 16.0, 1.0
	},
	{
	"Research on Grid Scroll Chaotic Extended Sequence Algorithm & Cct Implementation for F-OFDM System", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES,
	    1.4, 1.4, 1.5, 0.8, 3.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.7, -3.8, 7.5, 1, 2, 3, 5, 3, 3, NULL, 1 + AVERAGE, YAXIS, 100000.0,
	    NullSetup, DoResearchGridScrollChaoticExtendedSequenceAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0058, 1.0, 0.6
	},
	{
	"Resonance and Chaotic Trajectories in Magnetic Field Reversed Configuration", "p", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.15, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "r", "z", "m", "w", ES, ES, ES, ES, ES, ES, 0.6, 0.1, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -5.0, 10.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoResonance, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Retailers Dynamics System - the Study of the Chaotic Behavior", "q", "b1", "b2", "d1", "d2", "e", ES, ES, ES, ES, 
	    0.008, 0.3, 0.1, 0.2, 0.65, 0.7, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.2, 1.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 40000.0,
	    NullSetup, DoRetailersDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.0, 1.0
	},
	{
	"Riddled Attraction Basin and Multistability in Three-Element-Based Memristive Circuit", "a", "b", "c", "d", "f", ES, ES, ES, ES, ES,
	    0.8, 9.409 * 0.0001, 2.4082 * 0.0001, 0.05, 1.88, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.005, -0.005, exp(-8.0), exp(-8.0), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.5, -5.0, 10.0, 5, 7, 6, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoRiddledAttractionBasinMultistability, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.6, 1.0
	},
	{
	"Rikitake Chaotic Attractor", "a", "s", "u", ES, ES, ES, ES, ES, ES, ES, 2.0, 10.0, 2.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoRikitake, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Rikitake Chaotic Attractor in 4 Dimensions and its Sync Application for Secure Communication Systems", 
	    "mu", "a", "k", ES, ES, ES, ES, ES, ES, ES, 2.0, 5.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.1, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -11.0, 12.0, 3, 4, 2, 2, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoRikitake4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.005, 1.0
	},
	{
	"Robust Adaptive Synchronisation of the Energy Resource System with Constraint", 
	    "m", "n", "a1", "a2", "b1", "b2", "b3", "c1", "c2", "c3", 1.8, 1.0, 0.09, 0.15, 0.06, 0.082, 0.07, 0.2, 0.5, 0.4,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.82, 0.29, 0.58, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -2.0, 24.0, 1, 3, 2, 10, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoRobustAdaptive, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"Robust Multiple-Scroll Dynamics in Memristive-Based Generator System", "a", "b", "c", "e", "alpha", "beta", "epsilon", ES, ES, ES, 0.06, 0.1, 0.01, 1.0, 25.0, -6 * exp(-4.0), 5 * exp(-4.0), 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.2875, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.64, -0.92, 1.9, 1, 3, 2, 7, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoRobustMultipleScrollDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 0.3
	},
	{
	"Robust Synchronisation of Hyperchaotic Systems", "a", "b1", "b2", "b3", "c", ES, ES, ES, ES, ES, 0.56, 1.0, 1.0, 6.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoRobustSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Rössler Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 5.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -16.0, 30.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoRossler, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Rössler-Like System - II", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7,  -1.5,  2.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoRosslerLike, "SelectOscParams", (DLGPROC)SelectOscParams, 0.04, 1.0, 1.0
	},
	{
	"Rössler Prototype-4 System Based on Chua’s Diode Nonlinearity: Dynamics and FPGA Implementation", "a", "b", "c", "m0", "m1", ES, ES, ES, ES, ES, 4.0, 3.0, 1.0, -1.5, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.4,  -4.8,  10.0, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoRosslerPrototype4System, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 3.0, 1.0
	},
	{
	"Rucklidge Chaotic Attractor", "k", "a", ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 6.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0,  -8.0,  18.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoRucklidge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 0.3, 1.0
	},
	{
	"Russian Pendulum", "c", "e", "f", "d", ES, ES, ES, ES, ES, ES, -0.1, -0.59, 0.19, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 9.0, 5.0, 1.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0,  -10.0,  20.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoRussianPendulum, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 5.0, 1.0
	},
	{
	"Safieddine Chaotic Oscillator Type 1", "a", "b", "m", "g", ES, ES, ES, ES, ES, ES, 3.0, 2.2, 0.001, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0,  -0.5,  5.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSafieddine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Safieddine Chaotic Oscillator Type 2", "a", "b", "m", "g", ES, ES, ES, ES, ES, ES, 3.0, 2.2, 1.51, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.5, 5.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSafieddine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Safieddine Chaotic Oscillator Type 3", "a", "b", "m", "g", ES, ES, ES, ES, ES, ES, 3.0, 2.2, 0.001, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.2, 1.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -0.5, 6.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSafieddine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Saito Chaotic Oscillator", "alpha", "alpha1", "alpha2", "beta", "mu", "m", ES, ES, ES, ES, 2.5, 10.0, -7.5, 5.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 44.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSaito, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sakarya Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -20.0, 42.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSakarya, "SelectOscParams", (DLGPROC)SelectOscParams, 0.003, 1.1, 1.0
	},
	{
	"Scenario of the Birth of Hidden Attractors in the Chua Circuit", "alpha", "beta", "gamma", "m0", "m1", ES, ES, ES, ES, ES, 8.4, 12.0, -0.005, -0.121, -1.143, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.00011, 0.00012, 0.00013, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 4, 6, 5, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoScenarioBirthHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.6, 1.0
	},
	{
	"Second Order Adaptive Time Varying Sliding Mode Control for Sync of Hidden Orbits in a 4D Chaotic System", 
	    "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 2.79, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.9, 1.8, 1, 6, 5, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSecondOrderAdaptiveTimeVaryingSlidingModeControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 0.5
	},
	{
	"Secure Communication Scheme Based Generalised Function Synchronisation of a New 5D System", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 72.0, 3.0, 20.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "u", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -7.0, 56.0, 2, 3, 4, 4, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSecureCommunicationScheme, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.1, 1.0
	},
	{
	"Secure Communication Scheme using 5D Multistable 4-Wing Memristive Hyperchaotic Disturbance Inputs I", "a", "b", "c", "d", "e", "beta", ES, ES, ES, ES, 0.2, 12.0, 30.0, 2.0, 30.0, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "u", ES, ES, ES, ES, ES, -0.1, 0.1, -0.1, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -136.0, -111.0, 146.0, 3, 5, 1, 6, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSecureCommunicationScheme5DMultistableFourWing, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 10.0, 1.0
	},
	{
	"Secure Communication Scheme using 5D Multistable 4-Wing Memristive Hyperchaotic Disturbance Inputs II", "a", "b", "c", "d", "e", "beta", ES, ES, ES, ES, 10.0, 12.0, 30.0, 2.0, 3.0, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "u", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -340.0, -208.0, 420.0, 1, 2, 3, 6, 5, 5, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSecureCommunicationScheme5DMultistableFourWing, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00014, 1.0, 1.0
	},
	{
	"Secure Communication Using a New Hyperchaotic System with Hidden Attractors", "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 35.0, 30.0, 17.0, 0.78, 14.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 12.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    8.5, -1.6, 3.2, 3, 7, 4, 5, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSecureCommunicationUsingNewHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00001, 0.003, 1.0
	},
	{
	"Self-Excited and Hidden Attractors in a Novel Chaotic System with Complicated Multistability", "a", "s", "b", "d", "m", ES, ES, ES, ES, ES, 0.2, 17.0, 0.1, 0.63, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -16.0, 22.0, 1, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSelfExcitedHiddenAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 0.4
	},
	{
	"Self-reproducing Chaos and Bursting Oscillation Analysis in a meminductor-Based Conservative System", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 5.0, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.001, 0.1, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -5.6, 11.0, 2, 3, 1, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSelfReproducingChaosBurstingOscillationAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Seven-Dimensional Hyperchaotic System", "a", "b", "c", "d", "e", "f", "g", "h", "i", ES, 15.0, 20.0, 0.785, 5.0, 10.0, 30.0, 19.0, 3.9, 1.5, 0.0,
	    "x1", "x2", "x3", "x4", "x5", "x6", "x7", ES, ES, ES, 0.1, 0.3, 0.8, 0.3, 0.5, -0.9, -0.1, 0.0, 0.0, 0.0,
	    -5.0, -1.5, 4.0, 1, 2, 3, 9, 7, 7, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSevenDimensions, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Shaw-Pol Chaotic Oscillator", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0,  -4.0,  8.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoShawPol, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 0.6, 1.0
	},
	{
	"Shell Chaotic Oscillator", "a1", "a2", "b1", "b2", "d1", "d2", "e", ES, ES, ES, 5.0, 0.1, 3.0, 2.0, 0.4, 0.01, 0.3, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5,  -0.3,  1.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoShell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Shilnikov Chaotic Attractor", "a", "x", "y", "z", ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4,  -1.5,  3.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoShilnikov, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Shimizu-Morioka Chaotic Attractor Type 1", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.605, 0.549, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4,  -2.5,  4.0, 3, 2, 1, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoShimizu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.9, 1.0
	},
	{
	"Shimizu-Morioka Chaotic Attractor Type 2", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.75, 0.45, 00.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.15, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.8, 3.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoShimizu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.9, 1.0
	},
	{
	"Silnikov Homoclinic Orbits in Two Classes of 3D Autonomous Nonlinear Systems", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.8, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -6.0, 15.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSilnikov, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Silva-Young Anharmonic Chaotic Oscillator", "a", "b", "c", "d", "e", "k", "mu", ES, ES, ES, 0.6, 0.1, 0.15, 0.15, 1.2, 2.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSilvaYoung, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Simple Chaotic Circuit", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 15.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSimple, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Simple Chaotic Circuit with a Light-Emitting Diode", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.9231E-4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -25.0, 50.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticCircuitLightEmittingDiode, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0012, 0.8, 1.0
	},
	{
	"Simple Chaotic Flow", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 31.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlow, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.25, 1.0
	},
	{
	"Simple Chaotic Flow (With an Error)", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10000.0, -4500.0, 11111.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoSimpleChaoticFlowError, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Simple Chaotic Flows with a Curve of Equilibria-CE1", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.8, 0.61, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.54, 2.0, 5, 6, 4, 1, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSimpleChaoticFlowsCurveEquilibriaCE1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Simple Chaotic Flows with a Curve of Equilibria-CE2", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.75, -0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.7, 2.0, 5, 6, 4, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowsCurveEquilibriaCE2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Simple Chaotic Flows with a Curve of Equilibria-CE3", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.3, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.75, 0.0, 0.7, -1.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 2.5, 2, 5, 4, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowsCurveEquilibriaCE3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Simple Chaotic Flows with a Curve of Equilibria-CE4", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.75, 3.89, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 7.0, 1, 6, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowsCurveEquilibriaCE4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.02, 1.0
	},
	{
	"Simple Chaotic Flow With A Continuously Adjustable Attractor Dimension I", 
	    "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -8.0, 16.0, 1, 3, 2, 1, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlowAdjustable, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Simple Chaotic Flow With A Continuously Adjustable Attractor Dimension II", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    5.0, 0.64, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -18.0, 36.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowAdjustableAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.0, 1.0
	},
	{
	"Simple Chaotic Flow With A Continuously Adjustable Attractor Dimension IIIa", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowAdjustableAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Simple Chaotic Flow With A Continuously Adjustable Attractor Dimension IIIb", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowAdjustableAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.5, 1.0
	},
	{
	"Simple Chaotic Flow With A Continuously Adjustable Attractor Dimension IIIc", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 
	    1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleChaoticFlowAdjustableAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.5, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium I", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 4.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0,  -5.0,  10.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 1.0, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium VII", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -12.0, 16.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows7, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 0.5, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium VIII", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.2, 2.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows8, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0009, 0.5, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium X", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.9, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -110.0, -25.0, 100.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows10, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 10.0, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium XI", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows11, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 3.0, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium XII", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -10.0, 12.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows12, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"Simple Chaotic Flows with One Stable Equilibrium XIII", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, -2.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.5, 15.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticFlows13, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 3.0, 1.0
	},
	{
	"Simple Chaotic Hyperjerk System", "a", "b", "c", "kx", "ky", "kz", "kw", ES, ES, ES, 
	    1.0, 3.0, 1.0, 16.0, 7.0, 6.0, 15.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -7.4, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -7.0, 8.0, 1, 8, 2, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimpleChaoticHyperjerk, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.07, 1.0
	},
	{
	"Simple Chaotic Lorenz-Type System and Its Digital Realization Using a TFT Display Embedded System", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.0, 2.0, 0.5, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 2, 4, 1, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSimpleChaoticLorenzTypeSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.08, 1.0
	},
	{
	"Simple Chaotic Oscillator For Educational Purposes I", 
	    "a", "b", "epsilon", "c", ES, ES, ES, ES, ES, ES, 0.4, 30.0, 0.13, 0.004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -60.0, 120.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSimpleChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0079770, 1.0, 1.0
	},
	{
	"Simple Chaotic Oscillator For Educational Purposes II", 
	    "a", "b", "epsilon", "c", ES, ES, ES, ES, ES, ES, 0.4, 30.0, 0.13, 0.004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -140.0, 160.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSimpleChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0079770, 1.0, 1.0
	},
	{
	"Simple Chaotic Oscillator For Educational Purposes III", 
	    "a", "b", "epsilon", "c", ES, ES, ES, ES, ES, ES, 0.4, 30.0, 0.13, 0.004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -120.0, 150.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSimpleChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0079770, 0.8, 1.0
	},
	{
	"Simple Chaotic Oscillator For Educational Purposes IV", 
	    "a", "b", "epsilon", "c", ES, ES, ES, ES, ES, ES, 0.4, 30.0, 0.13, 0.004, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -140.0, 160.0, 1, -3, 2, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSimpleChaoticOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0079770, 1.0, 1.0
	},
	{
	"Simple Memristor Chaotic Circuit I", "alpha", "beta", "gamma", "a", "b", ES, ES, ES, ES, ES, 
	    21.0, 48.0, 0.6, 1.0/7.0, 2.0/7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.05, 1.0
	},
	{
	"Simple Memristor Chaotic Circuit II", "alpha", "beta", "gamma", "a", "b", ES, ES, ES, ES, ES, 
	    21.0, 48.0, 0.6, 1.0/7.0, 2.0/7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.4, 0.0, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.0, 4.0, 1, 3, 2, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.1, 1.0
	},
	{
	"Simple Memristor Chaotic Circuit III", "alpha", "beta", "gamma", "a", "b", ES, ES, ES, ES, ES, 
	    21.0, 48.0, 0.6, 1.0/7.0, 2.0/7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.24, 0.0, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0,  -37.0,  75.0, 2, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimpleMemristor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Simplest Chaotic Circuit", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.33, 0.5, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.0, 5.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSimplest, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Simplest 2 Chaotic Circuit", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.3, 1.7, 0.01, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.6, 4.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSimplest2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 0.9, 1.0
	},
	{
	"Simplest 3D Continuous-Time Quadratic System", "c8", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.23, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.9, 0.1, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -13.0, 26.0, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSimplest3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Simplest Circulant System from the Elegant Chaos Book", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.9, 2.0, 1, 5, 2, 0, 3, 3, NULL, 1, YAXIS, 150000.0,
	    NullSetup, DoSimplestCirculantSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Simplest Hyperchaotic System with Only One Piecewise Linear Term", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.4, 0.2002, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0001, 0.0001, 0.01, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 2, 3, 4, 3, 4, 4, NULL, 1, YAXIS, 800000.0,
	    NullSetup, DoSimplestHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0003, 1.5, 1.0
	},
	{
	"Simulation and Implementation of Memristive Chaotic System and its Application for Communication Systems", "c", "l", "alpha", "beta", ES, ES, ES, ES, ES, ES, 1.0, 3.0, 0.6, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.12, -0.82, 4.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSimulationImplementationMemristiveChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sine-Cosine Optimisation-Based Bijective Substitution-Boxes Construction Using Dynamics of Chaotic Map", 
	    "c", "r", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.0019, -0.0012, 0.0024, 1, 2, 2, 2, 1, 1, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSineCosineOptimisationBasedBijectiveSubstitution, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.001, 0.001
	},
	{
	"Sine-Cosine Shock", "a", "b", "c", "d", "m", "k", "w", ES, ES, ES, 
	    14.0, 43.0, -1.0, 16.0, 9.0, 10.0, PI / 25.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -6.0, 64.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSineCosineShock, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Single Amplifier Biquad Based Inductor-free Chua’s Circuit", "k", "m0", "m1", "alpha", "r1", "r2", ES, ES, ES, ES, 
	    0.04, -1.0/7.0, 2.0/7.0, 20.0, 17.59, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.2, -2.5, 5.0, 1, 4, 3, 6, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSingleAmplifierBiquadBasedInductorFreeChuasCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.3, 1.0
	},
	{
	"Sinusoidally Driven Lorenz System and Circuit Implementation", 
	    "azao", "b", "d", "a", "w", ES, ES, ES, ES, ES, 10.0, 35.0, 8.0/3.0, 17.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -64.0, -32.0, 70.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSinusoidallyDrivenLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Sinusoidally Forced Simplified Lorenz System", "c0", "q", "k", ES, ES, ES, ES, ES, ES, ES, 1.0, 4.5, 7.052, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.2, -0.6, 18.8, 5.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -600.0, -160.0, 400.0, 1, 3, 2, 3, 4, 4, NULL, 1, XAXIS, 1000000.0,
	    NullSetup, DoSinusoidallyForcedLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 3.0, 1.0
	},
	{
	"Six-Dimensional Hyperchaotic System Selection and Its Application in DS-CDMA System I", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 1, 4, 2, 0, 6, 6, NULL, 0, YAXIS, 5000000.0,
	    NullSetup, DoSixDHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.6, 1.0
	},
	{
	"Six-Dimensional Hyperchaotic System Selection and Its Application in DS-CDMA System II", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 3, 4, 1, 0, 6, 6, NULL, 0, YAXIS+EVOLVE, 5000000.0,
	    NullSetup, DoSixDHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.5, 1.0
	},
	{
	"Six-Dimensional Hyperchaotic System Selection and Its Application in DS-CDMA System III", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 4, 5, 1, 0, 6, 6, NULL, 0, YAXIS, 5000000.0,
	    NullSetup, DoSixDHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.3, 1.0
	},
	{
	"Six-Term Novel Chaotic System with Hidden Attractor and Its Circuit Design", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.8, 0.5, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.1, 0.0, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 3, 2, 4, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoSixTermNovelChaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sliding Mode Disturbance Control Based on Adaptive Synchronisation in a Fractional-Order Chaotic System",
	    "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 3.0, 2.7, 4.7, 2.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 6.0, -1.0, 3.0, 0.0, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -73.0, 190.0, 5, 6, 4, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSlidingModeDisturbanceObserverControlBasedAdaptiveSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Smooth N-Layer Attractor", "a1", "a2", "a3", "c1", "c2", "c3", "d", "b", "c", ES, -4.1, 1.2, 13.45, 2.76, 0.6, 13.13, 1.6, 0.4, 0.2, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -15.0, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -18.0, 4.0, 1, 2, 3, 9, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSmoothNLayer, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Snap Hyperchaos (from Elegant Chaos book)", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 3.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1310, 0.0358, 0.0, -1.2550, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 4, 2, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSnapHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.2, 1.0
	},
	{
	"Snap-Hyperchaos Modified from Elegant Chaos Book I", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.6, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 1, 3, 2, 2, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSnapHyperchaosModified, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Snap-Hyperchaos Modified from Elegant Chaos Book II", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.6, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 1, 4, 2, 2, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSnapHyperchaosModified, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Snap-Hyperchaos Modified from Elegant Chaos Book III", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.6, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 2, 4, 3, 2, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSnapHyperchaosModified, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.4, 1.0
	},
	{
	"Snap-Hyperchaos Modified from Elegant Chaos Book IV", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.6, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.45, -0.25, 0.5, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSnapHyperchaosModified, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Solution and Dynamics of a Fractional-Order 5-D Hyperchaotic System with Four Wings", 
	    "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, 2.0, 6.0, 1.0, -2.2, 1.0, 1.0, 10.0, 1.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 2, 4, 3, 8, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSolutionDynamicsFractionalOrder5DHyperchaoticSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 3.0, 1.0
	},
	{
	"Solitary States for Coupled Oscillators", "mi", "alpha", "n", "epsilon", "m", ES, ES, ES, ES, ES, 0.006, 1.0, 100.0, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.167, 0.267, 0.167, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -12.5, 25.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 330000.0,
	    NullSetup, DoSolitaryStatesForCoupledOscillators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0015, 4.0, 1.0
	},
	{
	"Some Attractors in the Extended Complex Lorenz Model", "s", "r", "b", ES, ES, ES, ES, ES, ES, ES, 10.0, 28.0, 8.0/3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.7, 0.5, 0.4, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -185.0, 360.0, 1, 2, 3, 3, 6, 6, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSomeAttractorsComplexLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Spaciotemporal Chaos Due to Spiral Waves in 4D", "k: try -1 to 10", "a", "b1", "b2", "b4", "b6", ES, ES, ES, ES, 
	    1.0, 0.01, 0.1, 0.1, -1.2, 1.12, 0.0, 0.0, 0.0, 0.0,
	    "p1", "q1", "p2", "q2", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.0, 2.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaosExcitedContinuous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Spatial Feedback Control on Multistability in Hidden Attractors - II", "alpha", "m", "beta", "w", ES, ES, ES, ES, ES, ES,
	    0.25, 0.9, 0.5, 2.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0001, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.95, -2.54, 4.375, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSpatialFeedbackControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 0.3
	},
	{
	"Spatio-Temporal Chaos due to Spiral Waves Core Expansion", "a", "b", "e", "c", ES, ES, ES, ES, ES, ES, 
	    0.95, 0.38, 0.01724137, 1.07846, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 100.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, 0.0, 1.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSpatiotemporal, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 1.0
	},
	{
	"Spatio-Temporal Patterns with Hyperchaotic Dynamics in Diffusively Coupled Biochemical Oscillators", "a1", "a2", "b1", "b2", ES, ES, ES, ES, ES, ES, 
	    0.0, 0.3, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.1, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.0, 7.0, 3, 4, 1, 4, 4, 4, NULL, 1, YAXIS, 900000.0,
	    NullSetup, DoSpatiotemporalHyperchaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Spherically Skrew Oscillator-2", "a1", "b1", "c1", "a2", "b2", "c2", "a3", "c3", ES, ES, 
	    0.0, 1.0, 0.0, -1.0, 1.0, 0.0, -1.0, 1.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.5, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 3, 2, 8, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSphericallyScrew, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"SPICE Implementation of a Novel 4-D Hyperchaotic Rikitake Dynamo System",
	"a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.7, 1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.8, 0.2, 0.4, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-8.0, -5.0, 10.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoSPICERikitakeDynamo, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
		{
	"Spiral Waves in Excited Neuronal Network: Solvable Model with a Differentiable Magnetic Flux",
	"a", "b", "k", "c", "d", "r", "s", "k1", "k2", "alpha", 1.0, 0.001, 1.0, 1.0, 5.0, 0.006, 4.0, 1.0, 0.5, 0.1,
	"x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, -2.0, 4.0, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-108.0, -25.0, 50.0, 2, 6, 3, 10, 4, 4, NULL, 0, YAXIS, 1000000.0,
	NullSetup, DoSpiralWavesExternallyExcitedNeuronalNetwork, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.2, 0.5
	},
	{
	"Spontaneous Synchronisation in Two Mutually Coupled Memristor-Based Chua’s Circuits - Numerical", 
	    "omega", "a", "epsilon", "alpha", "gamma", "omega1", "omega2", "s1", "s2", ES, 1.0, 200.0, 1.0E-9, 0.04, 0.001, 1.0, 1.05, 0.061, 0.025, 0.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", ES, ES, 100.0, -1.0, 1.0, 30.0, -40.0, 2.0, 7.0, -0.1, 0.0, 0.0,
	    -115.0, -66.0, 140.0, 6, 8, 3, 9, 8, 8, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSpontaneousSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0092, 0.4, 0.4
	},
	{
	"Sprott on Wind Energy Conversion", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 15.0, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 15.0, 9.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.0, 50.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSprottWind, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 2.0, 1.0
	},
	{
	"Sprott-302-B Chaotic Attractor", "b",  ES, ES, ES,  ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -8.0, 16.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSprott302, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-302-C Chaotic Attractor", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.0, 8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSprott302, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-330-A Chaotic Attractor", "alpha", "beta", "a", "m0", "m1", ES, ES, ES, ES, ES, 7.0, 14.0, 1.0, -0.43, 0.41, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.5, 1.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott330, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-330-B Chaotic Attractor", "alpha", "beta", "a", "m0", "m1", ES, ES, ES, ES, ES, 10.0, 14.0, 1.0, -0.43, 0.41, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott330, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-374 'Easter Island' Chaotic Attractor", "gp", "gr", "np", "nr", ES, ES, ES, ES, ES, ES, 0.1, 0.3, 0.47, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.01, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.1, 1.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 3000000.0,
	    NullSetup, DoSprott374, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Sprott-390 Chaotic Attractor", "a", "b", "c", "r", ES, ES, ES, ES, ES, ES, 10.0, 2.666667, 28.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 3.0, 19.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0,  -35.0,  70.0, 1, 2, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott390, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-391-A Chaotic Attractor", "c8", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -145.0,  -150.0,  300.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott391, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-391-B Chaotic Attractor", "c8", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.07, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2500.0,  -1600.0,  3000.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott391, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-392 Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0,  -1.5,  5.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott392, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Sprott-395-A Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0,  -2.0,  8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott395, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-395-B Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0,  -2.0,  8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott395, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-397 Chaotic Attractor", "z", "ro", "i", ES, ES, ES, ES, ES, ES, ES, 0.007, 3.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.9696, -3.70669, 3.3333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0,  30.0,  5.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott397, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 10.0, 1.0
	},
	{
	"Sprott-406-A Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.6, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, -0.1, 0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0,  -5.0,  10.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott406, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-406-B Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.2, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, -0.0034, -0.9966, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0,  -5.0,  10.0, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSprott406, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-406-C Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.2, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, -0.0074, -0.9926, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.5,  -4.2,  10.0, 2, 3, 4, 2, 4, 4, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoSprott406, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 0.4, 1.0
	},
	{
	"Sprott-407-I Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0,  -5.0,  12.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSprott407a, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.3, 1.0
	},
	{
	"Sprott-407-II Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4,  -1.5,  3.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSprott407b, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Sprott-407-III Chaotic Attractor", "r", ES, ES, ES, ES, ES, ES, ES, ES, ES, 4.418, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0,  -10.0,  20.0, 2, 3, 1, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott407c, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-407-IV Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -0.5, -0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0,  -2.0,  4.0, 1, 3, 2, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott407d, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.5, 1.0
	},
	{
	"Sprott-408 Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.6, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0,  -15.0,  30.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott408, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-409-I Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 15.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4,  -1.0,  2.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott409, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-409-II Chaotic Attractor", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 4.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0,  -6.0,  12.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott409II, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.2, 1.0
	},
	{
	"Sprott-411 Chaotic Attractor", "alpha", "beta", "m0", "m1", "a", ES, ES, ES, ES, ES, 11.0, 15.0, -0.43, 0.41, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0,  -2.0,  4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott411, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-412 Chaotic Attractor", "u", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, -0.1, -0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0,  -12.5,  25.0, 1, 2, 3, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott412, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-416 Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 2.7, 0.28, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.0, 6.0, 1, 2, 3, 3, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott416, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-421 Chaotic Attractor", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "p", "q", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0,  -3.0,  6.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSprott421, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Sprott-423-A Chaotic Attractor", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.0, 2.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprott423, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-423-B Chaotic Attractor", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -5.0, 8.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoSprott423, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Sprott-423-C Chaotic Attractor", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.5, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5,  -1.6,  3.0, 2, 3, 1, 0, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSprott423, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type A", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0,  -3.0,  6.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzA, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type B", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0,  -3.0,  6.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzB, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 0.3, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type C", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0,  -3.0,  6.0, 1, 2, 3, 0, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzC, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 0.5, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type D", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0,  -5.0,  10.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzD, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type E", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5,  -0.8,  4.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type F", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0,  -6.0,  8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 5000.0,
	    NullSetup, DoSprottLinzF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type G", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0,  -3.2,  6.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type H", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0,  -2.5,  8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type I", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0,  -0.5,  1.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type J", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0,  -20.0,  35.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.5, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type K", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0,  -3.6,  7.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzK, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type L", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.9, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0,  -10.0,  36.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzL, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.3, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type M", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0,  -5.0,  8.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type Q", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 3.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0,  -10.0,  16.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSprottLinzQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.6, 1.0
	},
	{
	"Sprott-Linz Chaotic Attractor type S", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.9, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0,  -6.0,  16.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSprottLinzS, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0
	},
	{
	"Sprott-Marcus Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -10.0, 40.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSprottMarcus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Stabilising Near-Nonhyperbolic Chaotic Systems and its Potential Applications in Neuroscience", 
	    "r", "i", ES, ES, ES, ES, ES, ES, ES, ES, 0.00125, 3.281, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.6835, -1.3359, -3.666, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -7.0, 16.0, 2, 4, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoStabilizingNearNonhyperbolicChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Stability Analysis and Control Chaos for Fractional 5D Maxwell-Bloch Model", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.05, -0.5, 0.1, -1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -2.0, 2.8, 3, 5, 1, 0, 5, 5, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoStabilityAnalysisControlChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.2, 1.0
	},
	{
	"Stabilisation and Synchronisation of Uncertain Zhang System by Means of Robust Adaptive Control I", 
	    "a", "b", "c", "d", "f", "g", ES, ES, ES, ES, 2.0, 10.0, 6.1, 3.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -12.5, 30.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoStabilisationSynchronisationUncertainZhangSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 2.0, 2.0
	},
	{
	"Stabilisation and Synchronisation of Uncertain Zhang System by Means of Robust Adaptive Control II", 
	    "a", "b", "c", "d", "f", "g", ES, ES, ES, ES, 2.0, 10.0, 6.1, 3.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.11, 0.52, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 36.0, 1, 6, 2, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoStabilisationSynchronisationUncertainZhangSystem, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0045, 0.3, 2.0
	},
	{
	"Steady Periodic Memristor Oscillator with Transient Chaotic Behaviours", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.65, 0.65, 0.2, 1.0, 100.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.05, 1E-10, 0.1, 0.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.6, 1.2, 3, 5, 1, 6, 4, 4, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoSteadyPeriodicMemristorOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.01, 1.0
	},
	{
	"Stimulus-Induced Chaotic Synchronisation of Chua's Oscillators", 
	    "alpha", "beta", "gamma", "d", "e", "ma", "mb", "mc", ES, ES, 4.0, 4.0, 0.1, 4.0, 1.005, -1.5, -0.5, 3.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 6.0, 1, 4, 6, 8, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoStimulusInducedChaoticSync, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"Strange Double-Deck Butterfly Chaotic Attractor", "a", "w", "ab", "g", ES, ES, ES, ES, ES, ES, 0.1, 1.6, 0.5, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.8, 23.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.0, 2.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoStrangeButterfly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.03, 1.0
	},
	{
	"Strange Non-chaotic Attractor", "gamma", "beta", "f1", "f2", "k", "w1", "w2", ES, ES, ES, 2.0, -1.1, 3.5, 5.0, 3.9, (sqrt(5.0) + 1.0) / 5.0, 2.5, 0.0, 0.0, 0.0,
	    "x", "y", "z", "q", ES, ES, ES, ES, ES, ES, 0.5, 0.01, -0.2, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 2, 3, 7, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoStrangeNonChaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 1.0, 1.0
	},
	{
	"Strizhak-Kawczynski Chaotic Attractor", "a", "b", "b1", "b2", "q", "r", "ax1", "ax1", "ax1", ES, 150.0, 436.6, 3.714, 21.7, 0.07, 0.101115, 10.0, 11.0, 20.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -6.0, 16.0, 1, 2, 3, 9, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoStrizhak, "SelectOscParams", (DLGPROC)SelectOscParams, 0.08, 0.044, 1.0
	},
	{
	"Study of a Two-Memcapacitor Circuit Model with Semi-Explicit ODE Solver", "a1", "b1", "a2", "b2", "c", "d", "ee", ES, ES, ES, 0.25, 0.6, -0.17, 10.0, 8.96, 4.2, 7.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0001, 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.15, -0.072, 0.16, 2, 3, 1, 7, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoStudyTwoMemcapacitorCircuitModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.4, 0.4
	},
	{
	"Study of the Coexistence of Three Types of Attractors in an Autonomous System", 
	    "a", "c", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.14, 0.109, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -8.0, 28.0, 1, 3, 2, 2, 3, 3, NULL, 1, XAXIS, 5000000.0,
	    NullSetup, DoStudyOfOCoexistence, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.2, 1.0
	},
	{
	"Suppression of Chaos in Porous Media Convection under Multifrequency Gravitational Modulation", 
	    "d", "l1", "l2", "alpha", "r", "s1", "s2", ES, ES, ES, 0.5, 1.0, 1.0, 5.0, 25.0, 4.5, 2.5, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.9, 0.9, 0.9, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.54, 2.0, 2, 3, 4, 7, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSuppressionChaosPorousMedia, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 0.01
	},
	{
	"Switching Synchronisation Control between Integer-order & Fractional-order Dynamics of a Chaotic System", 
	    "a", "b", "c", "d", "l", "k", ES, ES, ES, ES, 10.0, 1.0, 5.0, -1.0, -5.0, -6.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.365, -0.91, -2.236, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -5.8, 6.4, 1, 3, 2, 6, 3, 3, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoSwitchingSynchronisationControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Symmetric Flow Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -13.0, 20.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100.0,
	    NullSetup, DoSymmetricFlow, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.7, 1.0
	},
	{
	"Synchronisation and an Application of a Novel Fractional Order King Cobra Chaotic System", "a", "b", "c", "d", "h", "k", ES, ES, ES, ES, 7.5, 1.0, 5.0, -1.0, -5.0, -6.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -22.0, 52.0, 4, 6, 5, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSynchronisationApplicationNovelFractionalOrderKingCobra, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 1.4, 1.0
	},
	{
	"Synchronisation Behaviors of Coupled Systems Composed of Hidden Attractors", "a",  "b", "c", "d", "r", ES, ES, ES, ES, ES, -0.1, 1.0, -1.2, -0.1, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -580.0, -240.0, 700.0, 5, 6, 2, 5, 3, 3, NULL, 1, YAXIS, 12.0,
	    NullSetup, DoSynchronisationBehaviorsCoupledSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.5, 1.0
	},
	{
	"Synchronisation and Anti-Synchronisation of Chaos in an Extended Bonhoffer - van der Pol Oscillator", 
	    "a",  "b", "d", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -5.0, 10.0, 3, 4, 2, 3, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSynchronisationAntiSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.15, 1.0, 1.0
	},
	{
	"Synchronisation Between Fractional-Order and Integer-Order", "a", "b", "c", "d", "gamma", ES, ES, ES, ES, ES, 35.0, 3.0, 28.0, 1.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.9, 0.1, 0.9, 1.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -80.0, 160.0, 1, 4, 3, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSyncFractional, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.4, 1.0
	},
	{
	"Synchronisation Between Two Non-Autonomous Chaotic Systems Via Intermittent Event-Triggered Control", "Starting Value", ES, ES, ES, ES, ES, ES, ES, ES, ES, 6000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, -2.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -80.0, 160.0, 1, 2, 3, 1, 3, 3, NULL, 0, YAXIS, 15000.0,
	    NullSetup, DoSyncBetween2NonAutonomousChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Synchronisation Dependence on Initial Setting of Chaotic Systems Without Equilibria - 3D", "k0", "a", "l", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.8, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.92, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSynchronisationDependenceInitialSetting3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Synchronisation Dependence on Initial Setting of Chaotic Systems Without Equilibria - 6D", "k0", "a", "l", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.8, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.92, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.0, 5.0, 4, 5, 6, 3, 6, 6, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSynchronisationDependenceInitialSetting6D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 0.04
	},
	{
	"Synchronisation of Chaotic Systems using Feedback Controller: (Diffie–Hellman Key and ElGamal Key Crypt)", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -9.0, 12.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSynchronisationChaoticSystemsFeedbackController, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Synchronisation Chaotic Attractor", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "y1", "y2", "y3", ES, ES, ES, ES, 0.11, 0.96, 18.98, -0.69, -11.09, 1.96, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -8.0, 16.0, 6, 2, 3, 0, 6, 6, NULL, 1, XAXIS, 200000.0,
	    NullSetup, DoSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.25, 1.0
	},
	{
	"Synchronisation Control of a Novel Fractional-order Chaotic System with Hidden Attractor", "a", "b", "m", "c", ES, ES, ES, ES, ES, ES, 0.8696, 1.0, 0.756144, 10.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.08, -1.02, -0.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.4, 2.8, 1, 2, 4, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSynchronisationControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00065, 0.2, 1.0
	},
	{
	"Synchronisation in Coupled Multistable Systems with Hidden Attractors", "a", "k", ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.11, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", "p", "q", "r", ES, 0.1, 0.1, 0.1, 0.2, 0.001, 0.5, 2.0, -1.0, 1.0, 0.0,
	    -0.3, -0.2, 0.3, 14, 15, 3, 2, 9, 9, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSynchronizationCoupledMultistableSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0007, 0.1, 1.0
	},
	{
	"Synchronisation of a Novel Fractional Order Stretch-Twist-Fold (STF) Flow Chaotic System", 
	    "alpha", "beta", ES, ES, ES, ES, ES, ES, ES, ES, 0.68, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8593, 0.1632, 0.1678, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.0, 2.0, 2, 4, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSynchronisationNovelFractionalOrderStretchTwistFold, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 0.2, 1.0
	},
	{
	"Synchronisation of Chaotic Cellular Neural Networks in Small-World Topology - Original", "Start Value", "0 = orig, 1 = no trig, 2 = tan", ES, ES, ES, ES, ES, ES, ES, ES, 
	    11100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, -0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 4, 2, 2, 2, 2, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoSynchronisationNeuralNetworks, "SelectOscParams", (DLGPROC)SelectOscParams, 1.5, 1.0, 1.0
	},
	{
	"Synchronisation of Chaotic Cellular Neural Networks in Small-World Topology - No Trig Function", 
	    "Start Value", "0 = orig, 1 = no trig, 2 = tan", ES, ES, ES, ES, ES, ES, ES, ES, 11100.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, -0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.3, -3.0, 6.0, 1, 4, 2, 2, 2, 2, NULL, 0, YAXIS, 100000.0,
	    NullSetup, DoSynchronisationNeuralNetworks, "SelectOscParams", (DLGPROC)SelectOscParams, 1.5, 1.0, 1.0
	},
	{
	"Synchronisation of Chaotic Cellular Neural Networks in Small-World Topology - Using Tangent", "Start Value", "0 = orig, 1 = no trig, 2 = tan", ES, ES, ES, ES, ES, ES, ES, ES, 
	    11100.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", ES, ES, ES, ES, ES, ES, ES, ES, -0.3, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 2, 4, 2, 2, 2, 2, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSynchronisationNeuralNetworks, "SelectOscParams", (DLGPROC)SelectOscParams, 1.5, 1.0, 1.0
	},
	{
	"Synchronisation of Chaotic Cellular Neural Networks in Small-World Topology - An Experiment with Pi", "Anomalous Pi", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.4152, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.2, 0.1, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 2, 6, 3, 1, 4, 4, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoSynchronisationNeuralNetworksPi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Synchronisation of Chaotic Systems via Adaptive Control of Symmetry Coefficient in Semi-Implicit Models", "h", "s", ES, ES, ES, ES, ES, ES, ES, ES,
	    0.08, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.5, -10.0, 15.0, 4, 5, 6, 2, 3, 3, NULL, 0, YAXIS, 500000.0,
	    NullSetup, DoSynchronisationChaoticSystemsAdaptiveControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00014, 0.3, 0.1
	},
	{
	"Synchronisation of Chaotic Fractional-Order WINDMI Systems", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.707, 2.68, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.8, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -50.0, 80.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSyncWindmi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Synchronisation of Coupled Bistable Chaotic Systems: Experimental Study I", 
	    "a", "b", "mu", "g", ES, ES, ES, ES, ES, ES, 0.05, 0.5, 15.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -1.0, 8.0, 1, 3, 2, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSyncCoupledBistable, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Synchronisation of Coupled Bistable Chaotic Systems: Experimental Study II", 
	    "a", "b", "mu", "g", ES, ES, ES, ES, ES, ES, 0.05, 0.5, 15.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -1.0, 8.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSyncCoupledBistable, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.8, 1.0
	},
	{
	"Synchronisation of Cyclic Coupled Hyperchaotic Systems and its Circuit Implementation",
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.98, 9.0, 50.0, 0.06, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -33.0, -19.0, 38.0, 3, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSynchronisationCyclicCoupledHyperchaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.004, 0.4, 1.0
	},
	{
	"Synchronisation Phenomena in Coupled Birkhoff-Shaw Chaotic Systems Using Nonlinear Controllers", 
	    "a", "b", "c", "d", "real", "imag", ES, ES, ES, ES, 1.0, 0.71, 0.2, 3.0, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.13, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 2, 5, 6, 6, 3, 2, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoSyncPhenomenaCoupledBirkhoffShawChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.8, 1.0
	},
	{
	"Synchronisation of Hyperchaotic Oscillators Using a Scalar Signal", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1400000.0, -1600000.0, 2000000.0, 3, 4, 1, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSyncScalarSignal, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 2.0, 1.0
	},
	{
	"Synchronisation of Irregular Complex Networks with Chaotic Oscillators: Hamiltonian Systems Approach", 
	    "gamma", "g", "delta", "s", "beta", "e", ES, ES, ES, ES, 0.2, 2.0, 0.001, 1.667, 0.001, 0.3, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.4, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 3, 2, 6, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSyncIrregularComplexNetworks, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 7.0, 1.0
	},
	{
	"Synchronisation of Two coupled UNFractional-Order Chaotic Oscillators", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 31.21, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -15.0, 30.0, 1, 3, 2, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSyncUNFractional, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Synchronisation of Two, Four Dimensional Chaotic Systems with Cubic Nonlinearities", 
	    "b", "c", "d", "Coherence (1) or other (n) - try'0'", "Master Colour", "Slave Colour", ES, ES, ES, ES, 10.0, 1.0, 10.0, 0.0, 160.0, 123.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "y1", "z1", "t1", "x2", "y2", "z2", "t2", ES, ES, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0,
	    -80.0, -37.0, 75.0, 4, 2, 1, 6, 4, 8, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoSynchronisation2by4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Synchronisation Phenomena of Two Simple RC Chaotic Circuits by a Capacitor", 
	    "g", "va", ES, ES, ES, ES, ES, ES, ES, ES, 0.75, 0.06, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.2, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.5, 1, 2, 3, 2, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSyncPhenomena, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Synchronisation Phenomena in Coupled Non-Identical Chaotic Circuits", 
	    "c", "d", ES, ES, ES, ES, ES, ES, ES, ES, 1.7, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -5.0, 8.5, 3, 4, 2, 2, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSynchronisationPhenomena, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Synchronisation Phenomena of Two Chaotic Oscillators with Shifting Input Wave", 
	    "g", "va", "d 0.005-1.0", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.06, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", ES, ES, ES, ES, ES, ES, 0.1, -0.1, 0.1, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 1, 2, 3, 3, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSyncPhenomena1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Synchronising Nonautonomous Chaotic Circuits",
	    "a", "b", "gamay", "gamaz", "fd", ES, ES, ES, ES, ES, 2.0, 10000.0, 0.2, 0.1, 769.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.102, 0.102, 1.02, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.8, -4.0, 5.7, 2, 3, 1, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoSynchronisingNonautonomousChaoticCircuits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00000151, 1.0, 1.0
	},
	{
	"System with an Infinite Number of Equilibrium Points", 
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 10.0, 15.0, -2.5, -10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, 20.0, 140.0, 2, 3, 4, 5, 4, 4, NULL, 0, YAXIS, 30000.0,
	    NullSetup, DoSystemInfinite, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 4.0, 1.0
	},
	{
	"System with Multiple Attractors-Dynamic Analysis, Circuit Realisation and S-Box Designs", 
	    "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 4.0, 9.0, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -63.0, 126.0, 3, 6, 4, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoSystemMultipleAttractorsDynamicAnalysis, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.01, 0.05
	},
	{
	"Takagi-Sugeno Chaotic Oscillators", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 63.0, 3.0, 28.0, -16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -2.0, -4.0, -15.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -25.0, 50.0, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoTakagiSugeno, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Tamari Chaotic Attractor", "a", "b", "c", "d", "e", "f", "g", "v", "i", ES, 1.013, -0.011, 0.02, 0.96, -1.0, 0.01, 1.0, 0.05, 0.05, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -2.0, 16.0, 1, 2, 3, 9, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoTamari, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Taming Chaos by Linear Regulation with Bound Estimation", "a", "b", "c", "d", "h", ES, ES, ES, ES, ES, 3.0, 2.7, 4.7, 3.0, 9.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.3, 0.4, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -10.0, 20.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTamingChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Tent + Logistic Maps-Complex-8 (Try varying iterations)", 
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.1, 1.2, 3, 1, 2, 0, 4, 4, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoTentPlusLogistic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Test for Chaos in a Fractional Order Financial System with Investment Incentive", 
	    "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 3.0, 0.4, 0.4, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 4.0, 0.3, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 1, 3, 4, 4, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoTestForChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 10.0, 1.0
	},
/* 
	// cancelled by Marcus 2019-03-09
	{
	"The Dynamics of Coupled Oscillators", 
	    "r", "f", "e", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.6, 700.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.54, -0.3, 0.6, 4, 5, 6, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoTheDynamicsCoupledOscillators, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0008, 1.0, 0.2
	},
*/
	{
	"Theoretical Analysis and Adaptive Synchronisation of a 4D Hyperchaotic Oscillator I", 
	    "e1", "e2", "alpha", "sigma", "phi", ES, ES, ES, ES, ES, 3.0, 3.226, 5.249 * 0.00001, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 12.0, 17.0, 29.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -72.0, -42.0, 84.0, 1, 4, 2, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTheoreticalHyperchaoticI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.6, 1.0	
	},
	{
	"Theoretical Analysis and Adaptive Synchronisation of a 4D Hyperchaotic Oscillator II", 
	    "e1", "e2", "alpha", "sigma", "phi", ES, ES, ES, ES, ES, 3.0, 3.226, 5.249 * 0.00001, 0.25, 0.0, 3.226, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -0.9, -0.9, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 2, 3, 4, 5, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTheoreticalHyperchaoticII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0	
	},
	{
	"Theoretical Analysis and Adaptive Synchronisation of a 4D Hyperchaotic Oscillator III", 
	    "e1", "e2", "alpha", "sigma", ES, ES, ES, ES, ES, ES, 3.0, 3.226, 5.249 * 0.00001, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, .1, -0.6, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -52.0, -35.0, 52.0, 1, 4, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTheoreticalHyperchaoticIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Theoretical Analysis and Circuit Verification for Fractional Chaotic Behavior in a New Hyperchaotic System I", 
	    "b1", "b2", "b3", "b4", ES, ES, ES, ES, ES, ES, 8.25, 0.8, 12.25, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.05, 0.08, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.4, -0.25, 0.5, 6, 8, 3, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTheoreticalCircuitVerificationI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.5, 1.0
	},
	{
	"Theoretical Analysis and Circuit Verification for Fractional Chaotic Behavior in a New Hyperchaotic System II", 
	    "b1", "b2", "b3", "b4", ES, ES, ES, ES, ES, ES, 10.0, 1.0, 12.5, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.2, 0.05, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -8.0, 16.0, 2, 5, 7, 4, 4, 4, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoTheoreticalCircuitVerificationII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.018, 0.5, 1.0
	},
	{
	"Third order Chaotic Oscillator in 4 Dimensions",  "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 0.06, 2.8, -2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 2, 3, 1, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoThirdOrder4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Third order Chaotic Oscillator in 7 Dimensions",  "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, 15.0, 5.0, 0.5, 25.0, 10.0, 4.0, 0.1, 1.5, 0.0, 0.0,
	    "x", "y", "z", "w", "q", "r", "s", ES, ES, ES, 1.0, 3.0, 8.0, 0.0, 5.0, -0.9, -0.1, 0.0, 0.0, 0.0,
	    -30.0, -18.0, 36.0, 1, 2, 3, 8, 7, 7, NULL, 1, YAXIS, 100.0,
	    NullSetup, DoThirdOrder7D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Third-Order Generalised Memristor-Based Chaotic Circuit and its Complex Dynamics",  "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    1.125, 0.1, 1.8062 * 0.0001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.021, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -20.0, 40.0, 1, 6, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoThirdOrderGeneralisedMemristorBasedChaoticCircuit, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Third-Order RLCM-Four-Elements-based Chaotic Circuit and its Coexisting Bubbles",  
	    "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 1.125, 0.5625, 1.8062 / 10000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -12.5, 25.0, 1, 3, 2, 3, 3, 3, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoThirdOrderRLCMFourElements, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00015, 0.5, 1.0
	},
	{
	"Thomas Cyclically Symmetric Attractor", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -6.0, 12.0, 1, 2, 3, 1, 3, 3, "SSS", 1, YAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
	{
	"Thomas Cyclically Symmetric Attractor Variation 1", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 1, 3, 3, "SCT", 1, YAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
/*
	{
	"Thomas Cyclically Symmetric Attractor Variation 2", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 1, 3, 3, "TSC", 1, YAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
*/
	{
	"Thomas Cyclically Symmetric Attractor Variation 3", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 1, 3, 3, "CTS", 1, YAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.075, 1.0
	},
/*
	{
	"Thomas Cyclically Symmetric Attractor Variation 4", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 1, 3, 3", "STT", 1, YAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
*/
	{
	"Thomas Cyclically Symmetric Attractor Variation 5", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 1, 3, 3, "SST", 1, XAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
	{
	"Thomas Cyclically Symmetric Attractor Variation 6", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 1, 3, 3, "TST", 1, YAXIS, 20000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
	{
	"Thomas Cyclically Symmetric Attractor Variation 6A", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -24.0, 48.0, 1, 3, 2, 1, 3, 3, "TST", 1, YAXIS, 200000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.075, 1.0
	},
	{
	"Thomas Mod. Symmetric Attractor", "b", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.11, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -5.5, 10.0, 1, 2, 3, 1, 3, 3, "SSS", 1, YAXIS, 50000.0,
	    NullSetup, DoThomas, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 0.75, 1.0
	},
	{
	"Three-Dimensional Memristive Hindmarsh–Rose Neuron Model with Hidden Coexisting Asymmetric Behaviors", 
	    "a", "b", "c", "d", "k", "i", ES, ES, ES, ES, 1.0, 3.0, 1.0, 5.0, 0.9, 1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -10.0, 16.0, 2, 5, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoThreeDimensionalMemristiveHindmarshRoseNeuronModel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 3.0
	},
	{
	"Three Dimensional Spherical Chaotic Attractor", "a1", "a2", "a3", "b", "c1", "c2", "c3", "c4", "d", ES, -4.1, 1.2, 13.45, 0.161, 2.76, 0.6, 13.13, 3.5031, 1.6, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -22.0, 45.0, 2, 3, 1, 9, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoThreeDimensional, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.6, 1.0
	},
	{
	"Three Layer Chaotic Oscillator", "a1", "a2", "a3", "c1", "c2", "c3", "d", ES, ES, ES, -4.1, 1.2, 13.45, 2.76, 0.6, 13.13, 1.8, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -37.0, -36.0, 40.0, 3, 2, 1, 7, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoThreeLayer, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
	{
	"Time-Reversal Sym & Covariant Lyapunov Vectors for Simple Particle Models In & Out of Thermal Equilibrium", "epsilon", ES, ES, ES, ES, ES, ES, ES, ES, ES, 
	    0.09, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, -1.0, 0.1, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.9, -3.3, 6.0, 1, 2, 3, 1, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTimeReversalSymmetry, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 1.0, 1.0
	},
	{
	"Tokamaks Boundaries Fractal", "r", "teta", "a1", "gamma", "a", "q", ES, ES, ES, ES, 1.0, 0.1, -0.4, 4.0, 4.0, 2.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTokamaks, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Torus and Fixed Point Attractors of a New Hyperchaotic 4D System", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 5.0, 10.0, 5.0, 5.0, 1.0, 0.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 0.1, 0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -10.0, 20.0, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoTorusFixedPointAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00075, 1.0, 1.0
	},
	{
	"Transient Chaos in a Closed Chemical System", "sigma", "k", "delta", "ro", ES, ES, ES, ES, ES, ES, 5E-3, 65.0, 0.02, 1E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "mu", "alpha", "beta", "gamma", ES, ES, ES, ES, ES, ES, 0.155, 0.1, 0.8, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.1, 1.0, 2, 3, 4, 4, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTransientChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.005, 1.0
	},
	{
	"Transient Chaos in Fractional Bloch Equations","lamda", "psi", "t1", "t2", "delta", ES, ES, ES, ES, ES, 30.0, 0.173, 5.0, 2.5, -PI*0.4, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.42, -0.31, 0.59, 3, 4, 2, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTransientChaosFractionalBlochEquations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00015, 0.1, 1.0
	},
	{
	"Transport Properties in the Standard Map", "k", "z amplitude", ES, ES, ES, ES, ES, ES, ES, ES, -100.0, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, 0.0, 12.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTransport, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 12.0, 1.0
	},
	{
	"Trial and Error Chaotic Oscillator", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.5, 1.91, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, 14.0, 8.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 78400.0,
	    NullSetup, DoTrialAndError, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00002, 10.0, 1.0
	},
	{
	"Trilobyte (Dynamics of a New Hyperchaotic System with Only One Equilibrium Point)", 
	    "a", "b", "c", "d", "e", "v", ES, ES, ES, ES, 10.0, 8.0/3.0, 28.0, 2.0, 12.0, -1.2, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 1.0, 6.0, -3.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -50.0, 100.0, 3, 4, 2, 6, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoTrilobyte, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 10.0, 1.0	
	},
	{
	"Triternion", "z min", "z max", ES, ES, ES, ES, ES, ES, ES, ES, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 2, 3, 2, 3, 3, NULL, 0, YAXIS, 100.0,
	    NullSetup, DoTriternion, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tritrophic Food Chains I", "a2", "b2", "e2", "d2", "a3", "b3", "e3", "d3", "r", "k", 5.0/3.0, 1.0/3.0, 1.0, 0.4, 0.05, 0.5, 1.0, 0.01, 1.15, 1.07,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.1, 1.2, 1, 2, 3, 10, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoTritrophicFoodChains, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 1.0, 1.0
	},
	{
	"Tritrophic Food Chains II", "a2", "b2", "e2", "d2", "a3", "b3", "e3", "d3", "r", "k", 5.0/3.0, 1.0/3.0, 1.0, 0.4, 0.05, 0.5, 1.0, 0.01, 1.15, 1.07,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.1, 1.5, 1, 3, 2, 10, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoTritrophicFoodChains, "SelectOscParams", (DLGPROC)SelectOscParams, 0.1, 0.1, 1.0
	},
	{
	"Trumpets Chaotic Attractor", "a", "b", "c", "d", "e", "f", "g", "h", ES, ES, 0.5, 1.0, 1.0, 4.0, 1.2, 1.0, 6.0, 0.1, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -40.0, 80.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTrumpets, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Tsucs Chaotic Attractor", "a", "c", "d", "e", "f", ES, ES, ES, ES, ES, 40.0, 0.8333333, 0.5, 0.65, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.0, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -200.0, -100.0, 240.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoTsucs, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Tsucs Like Chaotic Attractor", "alpha", "gamma", "m", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.2, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 1.0, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -480.0, -200.0, 540.0, 2, 3, 1, 3, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoTsucsMarcus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Tumor Growth Model I", "a12", "a21", "a13", "a31", "r2", "r3", "d2", "k2", ES, ES, 1.0, 1.5, 5.0, 0.9435, 0.6, 4.5, 0.5, 1.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5,  -0.1,  1.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoTumorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.008, 1.0, 1.0
	},
	{
	"Tumor Growth Model II", "a12", "a21", "a13", "a31", "r2", "r3", "d2", "k2", ES, ES, 1.0, 1.5, 1.0, 1.0, 0.6, 4.5, 0.5, 1.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.1, 1.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoTumorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.6, 1.0, 1.0
	},
	{
	"Two Hyperchaotic Systems with Four-wing Attractors and Their Synchronisation I", "a", "b", "c", "k1", "k2", ES, ES, ES, ES, ES, 
	    36.0, 3.0, 20.0, 0.085, 0.017, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 1.0, 1.0, 1.0, -6.0, -6.0, -6.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, 8.0, 28.0, 1, 6, 4, 5, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoTwoHyperchaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Two Hyperchaotic Systems with Four-wing Attractors and Their Synchronisation II", "a", "b", "c", "k1", "k2", ES, ES, ES, ES, ES, 
	    36.0, 3.0, 20.0, 0.06, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 1.0, 1.0, 1.0, -5.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0,
	    -45.0, -30.0, 50.0, 1, 4, 6, 5, 6, 6, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoTwoHyperchaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Two Hyperchaotic Systems with Four-wing Attractors and Their Synchronisation III", "a", "b", "c", "k1", "k2", ES, ES, ES, ES, ES, 
	    36.0, 3.0, 20.0, 0.06, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 1.0, 1.0, 1.0, -5.0, -5.0, -5.0, 0.0, 0.0, 0.0, 0.0,
	    -48.0, -10.0, 60.0, 1, 6, 5, 5, 6, 6, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoTwoHyperchaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Two-Time Synchronism and Induced Synchronisation", "omega", "a", "epsilon", "alpha", "gamma", "omega1", "omega2", "s1", "s2", ES, 
	    1.0, 200.0, 10.0E-9, 0.04, 0.001, 1.0, 1.05, 0.061, 0.025, 0.0, 
	    "x", "y", "z", "u", "v", "w", "p", "q", ES, ES, -100.0, -1.0, 1.0, 30.0, -40.0, 2.0, 7.0, -0.1, 0.0, 0.0,
	    -4800.0, -3000.0, 6000.0, 1, 4, 2, 9, 8, 8, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTwoTimeSynchronism, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Ueda Chaotic Oscillator", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 9.5, 0.05, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.0, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, XAXIS, 10000.0,
	    NullSetup, DoUeda, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.25, 1.0
	},
	{
	"Ultimate Bound of a 3D Chaotic System and Its Application in Chaos Synchronisation", 
	    "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 24.0, 16.0, 40.0, 4.0, 5.0, 5.0, 1.7, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -35.0, 70.0, 1, 2, 3, 7, 3, 3, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoUltimateBound, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Uncertain Destination Dynamics of a Novel Memristive 4D Autonomous System", 
	    "sigma", "gamma", "alpha", "beta", "eta", ES, ES, ES, ES, ES, 2.5, 3.0, 2.0, 4.03, 6.667, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -80.0, 170.0, 7, 8, 5, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoUncertainDestinationDynamics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00075, 4.0, 4.0
	},
	{
	"Unilateral Coupling of Two X-Band Gunn Oscillators I", 
	    "a1", "a2", "b1", "b2", "c1", "c2", "d1", "d2", "qs", "omega", 1.0, 1.0, 0.98, 1.0, -0.002, -0.002, 0.015, 0.015, 0.05, 1.27,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 4.0, 0.4, -0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 2, 3, 4, 10, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoUnilateralCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Unilateral Coupling of Two X-Band Gunn Oscillators II", 
	    "a1", "a2", "b1", "b2", "c1", "c2", "d1", "d2", "qs", "omega", 1.0, 1.0, 0.98, 1.0, -0.002, -0.002, 0.015, 0.015, 0.05, 1.27,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 4.0, 0.4, -0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 2, 4, 3, 10, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoUnilateralCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Unimodal Chaotic Attractor", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k", 33.0, 150.0, 0.01, 3.5, 4815.0, 410.0, 0.59, 4.0, 2.5, 2.5,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 120.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -80.0, 320.0, 1, 2, 3, 10, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoUnimodal, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 2.0, 1.0
	},
	{
	"Unique Signum Switch for Chaos and Hyperchaos", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -28.0, -10.0, 32.0, 2, 3, 4, 2, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoUniqueSignum, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Unknown Author", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 
	    1.0, 8.0/3.0, 25.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -390.0, -60.0, 420.0, 2, 3, 1, 4, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoUnknownAuthor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.3, 1.0
	},
	{
	"Unknown Oscillator", "k1", "k2", "alpha", "beta", "a", "b", "c", "d", "s", "r", 
	    1.0, 0.5, 0.5, 0.02, 1.0, 3.0, 1.0, 5.0, 4.0, 0.006,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.003, 1.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, -5.0, 7.0, 1, 2, 3, 10, 4, 4, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoUnknownOscillator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.5, 1.0
	},
	{
	"Unknown Fountain Chaotic Attractor", "a", "c", ES, ES, ES, ES, ES, ES, ES, ES, 0.005, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.2, 1.1, 0.29, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -24.0, 48.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS+EVOLVE, /*16960.0*/20000.0,
	    NullSetup, DoUnknownFountain, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0005, 3.0, 1.0
	},
	{
	"Unknown Oscillator in 4 Dimensions", "te", "ti", "ke", "ki", "c1", "c2", "c3", "c4", "alpha", "beta", 1.0, 1.0, 1.0, 1.0, 16.0, 12.0, 15.0, 3.0, 5.3, -2.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.105, 0.1, 0.1, 0.145, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.36, -0.25, 0.4, 1, 3, 2, 10, 4, 4, NULL, 1, YAXIS, 5000000.0,
	    NullSetup, DoUnknownOscillator4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 0.3, 1.0
	},
	{
	"Unscrew or Back to the Future", "a", "p", "c", "w", ES, ES, ES, ES, ES, ES, 0.15, 0.2, 10.0, 0.91, 0.0, 0.0, 0.0, 0.0, 2.5, 2.5,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, -4.0, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -150.0, 150.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS,206000.0,
	    NullSetup, DoUnscrew, "SelectOscParams", (DLGPROC)SelectOscParams, -0.0005, 2.0, 1.0
	},
	{
	"Using Recurrences To Characterize The Hyperchaos-Chaos Transition", 
	    "a", "b", "alpha", "beta", "k", "m", ES, ES, ES, ES, 10.0, 14.87, -1.27, -0.68, 1.17, 0.0, 0.0, 0.0, 0.0, 0.0, 
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.01, 0.0, 0.0, 0.011, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, -3.0, 6.0, 1, 4, 3, 6, 6, 6, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoUsingRecurrences, "SelectOscParams", (DLGPROC)SelectOscParams, 0.006, 0.8, 1.0
	},
	{
	"Van der Pol Chaotic Oscillator", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 5.0, 2.0, 1.788, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -6.0, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoVanderpol, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Various Types of Coexisting Attractors in a New 4D Autonomous Chaotic System", "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 16.0, 1.0, 9.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -9.0, 18.0, 3, 5, 2, 4, 4, 4, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoVariousTypesCoexistingAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.3, 1.0
	},
	{
	"Wang Chaotic Oscillator", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 2.0, 10.0, 4.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -46.0, -36.0, 60.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS+EVOLVE, 500000.0,
	    NullSetup, DoWang, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Wang-Sun Chaotic Oscillator", "a", "b", "c", "d", "e", "f", ES, ES, ES, ES, 0.2, -0.01, 1.0, -0.4, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.0, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoWangSun, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.2, 1.0
	},
	{
	"Wavelet Characterisation of Hyper-chaotic Time Series", "a", "b", "c", "d", "e", "f", "g", ES, ES, ES, 30.0, 10.0, 15.7, 5.0, 2.5, 4.45, 38.5, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.8, 0.9, 0.85, 1.9, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -27.0, -16.0, 30.0, 4, 5, 3, 7, 5, 5, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoWaveletCharacterisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 0.2, 1.0
	},
	{
	"Waves Chaotic Oscillator type 1", "a", "b", "mu1", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.5, -1.0, -0.6295, -0.3, 0.0, 0.0, 0.0, 0.0,
	    "p1", "p2", "beta", "q1", "q2", ES, ES, ES, ES, ES, 0.0, 0.0, 0.13, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.4, 4.8, 5, 1, 3, 6, 5, 5, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoWaves1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.015, 1.0, 1.0
	},
	{
	"Waves Chaotic Oscillator type 3", "a", "b", "mu1", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.5, -1.0, -0.6295, -0.5, 0.0, 0.0, 0.0, 0.0,
	    "p1", "p2", "beta", "q1", "q2", ES, ES, ES, ES, ES, 0.9, 2.1, 0.1, 0.07, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.4, 4.8, 5, 1, 3, 6, 5, 5, NULL, 1, YAXIS, 500.0,
	    NullSetup, DoWaves3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	},
	{
	"Waves Chaotic Oscillator type 19", "a", "b", "mu1", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.8, -0.1, -0.6295, -0.3, 0.0, 0.0, 0.0, 0.0,
	    "p1", "p2", "beta", "q1", "q2", ES, ES, ES, ES, ES, 0.0, 0.0, 0.13, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0,  -2.0,  4.0, 5, 1, 3, 6, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoWaves19, "SelectOscParams", (DLGPROC)SelectOscParams, 0.017, 1.0, 1.0
	},
	{
	"Waves Chaotic Oscillator type 19a", "a", "b", "mu1", "n1", "n3", "alpha", ES, ES, ES, ES, 1.12, -1.531, 0.8, -0.1, -0.6295, -0.3, 0.0, 0.0, 0.0, 0.0,
	    "p1", "p2", "beta", "q1", "q2", ES, ES, ES, ES, ES, 0.0, 0.0, 0.13, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0,  -1.5,  3.0, 5, 2, 4, 6, 5, 5, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoWaves19, "SelectOscParams", (DLGPROC)SelectOscParams, 0.017, 1.0, 1.0
	},
	{
	"Wien-1 (ELWAKIL and SOLIMAN) Chaotic Oscillator", "alpha", "beta", "wn", "k", ES, ES, ES, ES, ES, ES, 0.6666667, 0.003333333, 1.95, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -18.0, 36.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoWien, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Wien-2 (ELWAKIL and SOLIMAN) Chaotic Oscillator",  "a", "b", "c", "k", ES, ES, ES, ES, ES, ES, 0.8666667, 0.0043333333333333, 19.0, 2.857, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoWien, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Wien and Ressonator Chaotic Oscillator",  "alpha", "beta", "gamma", "delta", "epsilon", ES, ES, ES, ES, ES, 1.0, 3.0, 5.0, 20.0, 40.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoWienRessonator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Willamowski-Rössler Model", "k1a", "k1b", "k2a", "k2b", "k3a", "k3b", "k4a", "k4b", "k5a", "k5b", 30.0, 0.25, 1.0, 0.0001, 10.0, 0.001, 1.0, 0.001, 16.5, 0.5,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -10.0, 110.0, 3, 2, 1, 10, 3, 3, NULL, 1, YAXIS, 30000.0,
	    NullSetup, DoWillamowski, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.9, 1.0
	},
	{
	"Wimol-Banlue Chaotic Attractor", "a", ES, ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -8.0, 16.0, 3, 2, 1, 1, 3, 3, NULL, 1, XAXIS+YAXIS, 50000.0,
	    NullSetup, DoWimolBanlue, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 0.7, 1.0
	},
	{
	"Windmi Chaotic Attractor I", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.7, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -28.0, 36.0, 2, 1, 3, 2, 3, 3, NULL, 1, XAXIS, 100000.0,
	    NullSetup, DoWindmi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Windmi Chaotic Attractor III", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.7, 3.68, -0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -20.0, -30.0, -50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -600.0, -375.0, 750.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoWindmi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.5, 1.0, 1.0
	},
	{
	"Windmi Chaotic Attractor IV", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.7, 2.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.984, 0.345, 0.789, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.0, 40.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoWindmi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Windmi Pulsating Chaotic Attractor Type I", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.6, 3.68, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.8, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26000.0, -15200.0, 28750.0, 2, 3, 1, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSyncWindmi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.3, 1.0, 1.0
	},
	{
	"Windmi Pulsating Chaotic Attractor Type II", "a", "b", ES, ES, ES, ES, ES, ES, ES, ES, 0.707, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.8, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -200.0, -100.0, 200.0, 2, 3, 1, 2, 3, 3, NULL, 0, YAXIS, 200000.0,
	    NullSetup, DoSyncWindmi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.2, 1.0, 1.0
	},
/*
	{
	"Windmi6D Chaotic Attractor", "a1", "a2", "b1", "b3", "d1", ES, 0.247, 0.391, 10.8, 1.06, 2200.0, 0.02,
	    ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0,  -20.0,  32.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoWindmi6D, "SelectOscParams", (DLGPROC)SelectOscParams, 1.0, 1.0, 1.0
	},
*/
	{
	"World Without War Chaotic Attractor", "re", ES, ES, ES, ES, ES, ES, ES, ES, ES, 32.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x1", "x2", "x3", "x4", "x5", ES, ES, ES, ES, ES, -0.1, -0.1, -0.1, -0.5, -0.4, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -1.8, 8.5, 1, 3, 2, 1, 5, 5, NULL, 1, YAXIS, 1000.0,
	    NullSetup, DoWorldWithoutWar, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 1.0, 1.0
	},
	{
	"Wrong Mixup Oscillator", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.4, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 3.0, 1, 3, 2, 0, 3, 3, NULL, 1, YAXIS, 2000.0,
	    NullSetup, DoWrongMixup, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 2.0, 1.0
	},
	{
	"Xing-Yun Chaotic Oscillator", "a", "b", "c", "e", ES, ES, ES, ES, ES, ES, 50.0, 10.0, 13.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 50.0, 10.0, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -25.0, 50.0, 1, 2, 3, 4, 3, 3, NULL, 1, XAXIS, 500000.0,
	    NullSetup, DoXingYun, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Xu Hyper-Chaotic Oscillator in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 35.0, 4.0, 26.0, 38.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 8.0, 12.0, 21.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 40.0, 3, 4, 2, 4, 4, 4, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoXu4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0002, 1.0, 1.0
	},
	{
	"Yang-Chen Chaotic Oscillator in 4 Dimensions", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 37.0, 3.0, 26.0, 38.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 5.0, 1.0, 30.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 1, 2, 3, 4, 4, 4, NULL, 1, XAXIS, 50000.0,
	    NullSetup, DoYangChen4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.9, 1.0
	},
	{
	"Yet Another Chaotic Attractor", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 0.2, 5.7, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -15.0, 30.0, 4, 6, 5, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoYetAnotherChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.005, 0.2, 1.0
	},
	{
	"Yet Another Unknown Chaotic Attractor", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, -0.888, 1.1, 1, 2, 3, 0, 4, 4, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoYetAnotherUnknownChaoticAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.00005, 1.0, 1.0
	},
	{
	"Yu-Wang Chaotic Oscillator", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 10.0, 40.0, 2.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, 1.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 4, 3, 3, NULL, 0, YAXIS+EVOLVE, 50000.0,
	    NullSetup, DoYuWang, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 0.5, 1.0
	},
	{
	"Zaidan, Radwan and Salama 2 Chaotic Systems", "s0", "s1", "s2", "d1", "d2", "d3", ES, ES, ES, ES, 1.555, 3.25, 0.0, 1.4, 2.8, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.0, 6.0, 3, 2, 1, 6, 3, 3, NULL, 0, YAXIS, 50000.0,
	    NullSetup, DoZaidan2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Zaidan, Radwan and Salama 3 Chaotic Systems", "s0", "s1", "s2", "d1", "d2", "d3", ES, ES, ES, ES, 1.56, 2.7115, 3.863, 1.15, 2.3, 3.45, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoZaidan3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Zaidan, Radwan and Salama 4 Chaotic Systems", "s0", "s1", "s2", "d1", "d2", "d3", ES, ES, ES, ES, 0.865, 2.53, 0.0, 1.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.1, -0.1, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.6, 4.0, 2, 3, 1, 6, 3, 3, NULL, 1, YAXIS, 20000.0,
	    NullSetup, DoZaidan4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"Zang Chaotic Oscillator in 5 Dimensions", "a", "b", "c", "Omega", ES, ES, ES, ES, ES, ES, 4.0, 3.0, 2.0, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", "q", ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -15.0, 30.0, 3, 4, 2, 1, 5, 5, NULL, 1, XAXIS, 50000.0,
	    NullSetup, DoZang5D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Zhou Chaotic Oscillator (Showing Construction of the Nucleus)", "a", "b", "c", ES, ES, ES, ES, ES, ES, ES, 10.0, 16.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, 1.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -29.0, -15.0, 30.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS+EVOLVE, 100000.0,
	    NullSetup, DoZhou, "SelectOscParams", (DLGPROC)SelectOscParams, 0.002, 1.0, 1.0
	},
// Knot Oscillators start here
	{
	"~Eight Knot as an Oscillator", "dt2", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -16.0, 32.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoEightKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Granny Knot as an Oscillator", "dt2", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -205.0, -115.0, 230.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoGrannyKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Knot Universes in Bianchi Type I Cosmology - Tempestuous as an Oscillator", "dt2", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -130.0, -95.0, 160.0, 7, 8, 5, 1, 4, 4, NULL, 1, YAXIS, 1500.0,
	    NullSetup, DoKnotUniversesITempestuousOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.2, 1.0, 1.0
	},
	{
	"~Marcus Torus Knot 15, 8 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.001, 2.0, 5.0, 15.0, 8.0, 10.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -9.0, 18.0, 1, 5, 3, 5, 3, 3, NULL, 1, YAXIS, 100.0,
	    NullSetup, DoMarcusTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"~Torus Knot 3, 2 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 4, 3 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 5, 2 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 5.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 5, 3 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 5, 4 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 5.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 6, 5 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 7, 2 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 7.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 7, 3 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 7.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 7, 4 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 7.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 7, 5 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 7.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 7, 6 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 7.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 15, 2 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 15.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Torus Knot 2, 15 as an Oscillator", "dt2", "R", "r", "p", "q", ES, ES, ES, ES, ES, 0.5, 100.0, 25.0, 2.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTorusKnotOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	{
	"~Trefoil Knot A as an Oscillator", "dt2", ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.6, -3.5, 7.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoTrefoilKnotAOsc, "SelectOscParams", (DLGPROC)SelectOscParams, 0.05, 1.0, 1.0
	},
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

