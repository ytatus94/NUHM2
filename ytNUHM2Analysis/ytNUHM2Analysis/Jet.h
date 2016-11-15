#ifndef JET_H
#define JET_H

#include "ytNUHM2Analysis/Particle.h"

class Jet : public Particle {
	// user defined variables

	// ntuple variables
	bool	isBjet;
	double	quality;
	double	JVT;
	double	JVTsf;
	double	MV2c20;
	double	MV2c10;
	double	SFw;
	int		ConeTruthLabel;
	int		PartonTruthLabel;
	int		HadronConeExclTruthLabel;
	double	deltaR;
	int		nTrk;
	bool	passOR;

public:
	Jet() : isBjet(0), quality(0), JVT(0), JVTsf(0), MV2c20(0), MV2c10(0), SFw(0), ConeTruthLabel(0), PartonTruthLabel(0), HadronConeExclTruthLabel(0),  deltaR(0), nTrk(0), passOR(0) {}
	~Jet() {}
	// get methods
	bool	get_isBjet()					{ return isBjet; }
	double	get_quality()					{ return quality; }
	double	get_JVT()						{ return JVT; }
	double	get_JVTsf()						{ return JVTsf; }
	double	get_MV2c20()					{ return MV2c20; }
	double	get_MV2c10()					{ return MV2c10; }
	double	get_SFw()						{ return SFw; }
	int		get_ConeTruthLabel()			{ return ConeTruthLabel; }
	int		get_PartonTruthLabel() 			{ return PartonTruthLabel; }
	int		get_HadronConeExclTruthLabel() 	{ return HadronConeExclTruthLabel; }
	double	get_deltaR()					{ return deltaR; }
	int		get_nTrk()						{ return nTrk; }
	bool	get_passOR()					{ return passOR; }
	// set methods
	void	set_isBjet(bool b)					{ isBjet = b; }
	void	set_quality(double d)				{ quality = d; }
	void	set_JVT(double d)					{ JVT = d; }
	void	set_JVTsf(double d)					{ JVTsf = d; }
	void	set_MV2c20(double d)				{ MV2c20 = d; }
	void	set_MV2c10(double d)				{ MV2c10 = d; }
	void	set_SFw(double d)					{ SFw = d; }
	void	set_ConeTruthLabel(int i) 			{ ConeTruthLabel = i; }
	void	set_PartonTruthLabel(int i)			{ PartonTruthLabel = i; }
	void	set_HadronConeExclTruthLabel(int i)	{ HadronConeExclTruthLabel = i; }
	void	set_deltaR(double d)				{ deltaR = d; }
	void	set_nTrk(int i)						{ nTrk = i; }
	void	set_passOR(int b)					{ passOR = b; }
};

#endif // #define JET_H
