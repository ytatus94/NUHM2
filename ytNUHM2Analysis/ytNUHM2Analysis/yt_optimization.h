#ifndef YT_OPTIMIZATION_H
#define YT_OPTIMIZATION_H

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

#include "ytNUHM2Analysis/Leptons.h"
#include "ytNUHM2Analysis/Jet.h"
#include "ytNUHM2Analysis/yt_useful_functions.h"

class yt_optimization
{
public:
	// flag
	bool isMC;
	bool isData;

	// user defined variables
	string	process;

	double 	derivation_stat_weights;

	float 	cross_section;
	float 	k_factor;
	float 	filter_efficiency;
	float 	cross_section_kfactor_efficiency;

	// Weights
	float event_weight;
	float lepton_weight;
	float jet_weight;
	float pileup_weight;

	vector<Electron> 	vec_signal_elec;
	vector<Muon>     	vec_signal_muon;
	vector<Lepton>   	vec_signal_lept;
	vector<Jet>      	vec_signal_jets;
	vector<Jet>			vec_signal_bjet;

	// Declare the output
	TFile 	*output_file;
	TTree 	*output_tree;

	TH1F 	*h_derivation_stat_weights;
	TH1F	*h_cross_section;
	TH1F	*h_k_factor;
	TH1F	*h_filter_efficiency;
	TH1F	*h_cross_section_kfactor_efficiency;

	TH1F	*h_met;
	TH1F	*h_Ht;
	TH1F	*h_meff;
	TH1F	*h_NLeptons;
	TH1F	*h_NJets;
	TH1F	*h_Nbjets;
	TH1F	*h_lep1_pT;
	TH1F	*h_lep2_pT;
	TH1F	*h_jet1_pT;
	TH1F	*h_jet2_pT;

	TH1F	*h_yields;
	TH1F	*h_yields_weighted;

	// leafs in tree
	// For distributions
	float	met;
	float	Ht;
	float	meff;
	float	lepton1_pT;
	float	lepton2_pT;
	float	jet1_pT;
	float	jet2_pT;
	// Optimization cut bins
	int		n_leptons_bin;
	int 	n_jets_bin;
	int 	n_bjets_bin;
	int 	jet_pt_bin;
	int 	bjet_pt_bin;
	int 	met_bin;
	int 	meff_bin;
	int 	events_survived;
	int 	events_survived_weighted;
	

	// Cuts
	static const int n_lept_cuts[5];
	static const int n_bjet_cuts[6];
	static const int n_jets_cuts[9];
	static const int bjet_pt_cuts[9];
	static const int jets_pt_cuts[9];
	static const int met_cuts[10];
	static const int meff_cuts[21];

public:
	yt_optimization();
	virtual ~yt_optimization();

	void initialize();
	void execute(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets);
	void finalize();

	void set_isMC(bool b) { isMC = b; }
	void set_isData(bool b) { isData = b; }

	void set_process(string s) { process = s; }

	//void set_luminosity(double d) { luminosity = d; }
	void set_cross_section(double d) { cross_section = d; }
	void set_k_factor(double d) { k_factor = d; }
	void set_filter_efficiency(double d) { filter_efficiency = d; }
	void set_cross_section_kfactor_efficiency(double d) { cross_section_kfactor_efficiency = d; }
	void set_derivation_stat_weights(double d) { derivation_stat_weights = d; }

	void set_event_weight(double d) { event_weight = d; }
	void set_lepton_weight(double d) { lepton_weight = d; }
	void set_jet_weight(double d) { jet_weight = d; }
	void set_pileup_weight(double d) { pileup_weight = d; }

	void set_met(double d) { met = d; }

	void reset_vectors();
	void copy_vectors(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets);
	void fill_signal_bjets(vector<Jet> signal_jets);

	void apply_signal_region_cuts(int cut_n_leptons,
								  int cut_bjets_pT, int cut_n_bjets,
								  int cut_jet_pt, int cut_n_jet,
								  int cut_met, int cut_meff,
								  float weight);
	void get_events_survived();
	void get_events_survived_weighted();

	void debug_print();

	ClassDef(yt_optimization, 0);
};

#endif // #ifndef YT_OPTIMIZATION_H