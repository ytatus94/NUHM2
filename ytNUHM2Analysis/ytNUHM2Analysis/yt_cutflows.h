#ifndef YT_CUTFLOWS_H
#define YT_CUTFLOWS_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "PileupReweighting/PileupReweightingTool.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODEventInfo/EventInfoContainer.h"
#include "xAODEventInfo/EventInfoAuxContainer.h"
#include "ytNUHM2Analysis/Leptons.h"
#include "ytNUHM2Analysis/Jet.h"

enum cuts {
	DerivationStat_Weights,
	All_events,
	GRL,
	Primary_vertex,
	Trigger,
	Global_flags,
	Bad_muon,
	At_least_one_jet_passes_jet_OR,
	Bad_jet,
	At_least_one_signal_jet,
	Cosmic_muons_veto,
	At_least_two_baseline_leptons_greater_than_10GeV,
	At_least_two_signal_leptons_greater_than_20GeV,
	Same_sign,
	ee_channel_separation,
	ee_trigger_matching,
	ee_at_least_one_bjet_greater_than_20GeV,
	ee_four_jets_greater_than_50GeV,
	ee_MET_greater_than_125GeV,
	emu_channel_separation,
	emu_trigger_matching,
	emu_at_least_one_bjet_greater_than_20GeV,
	emu_four_jets_greater_than_50GeV,
	emu_MET_greater_than_125GeV,
	mumu_channel_separation,
	mumu_trigger_matching,
	mumu_at_least_one_bjet_greater_than_20GeV,
	mumu_four_jets_greater_than_50GeV,
	mumu_MET_greater_than_125GeV,
	Ncuts
};

class yt_cutflows
{
public:
	// user defined variables
	GoodRunsListSelectionTool *m_GRL;

	CP::PileupReweightingTool* m_Pileup;
	xAOD::EventInfo* eventInfo;
	xAOD::EventInfoContainer* eInfos;
	vector<string> confFiles;
	vector<string> lcalcFiles;

	vector<double> events_pass_cutflow;

	//float pileupwgh;

public:
	yt_cutflows();
	virtual ~yt_cutflows();
	void update(int cut, bool passed);
	void print();

	// user defined cutflow methods
	bool pass_all_events();
	bool pass_GRL(bool isData, int RunNb, int LB);
	bool pass_primary_vertex(float PV_z);
	bool pass_2015_trigger(bool HLT_2e12_lhloose_L12EM10VH, bool HLT_e17_lhloose_mu14, bool HLT_mu18_mu8noL1, bool HLT_xe70, float Etmiss_TST_Et);
	bool pass_2016_trigger(bool HLT_2e17_lhvloose_nod0, bool HLT_e17_lhloose_nod0_mu14, bool HLT_mu20_mu8noL1, bool HLT_xe100_mht_L1XE50, float Etmiss_TST_Et);
	bool pass_trigger(bool isData, bool isMC, int run_number, int random_run_number,
					  bool HLT_2e12_lhloose_L12EM10VH, bool HLT_e17_lhloose_mu14, bool HLT_mu18_mu8noL1, bool HLT_xe70,
					  bool HLT_2e17_lhvloose_nod0, bool HLT_e17_lhloose_nod0_mu14, bool HLT_mu20_mu8noL1, bool HLT_xe100_mht_L1XE50,
					  float Etmiss_TST_Et);
	bool pass_global_flags(bool isData, bool isMC, int DetError);
	bool pass_bad_muon(vector<Muon> muon);
	bool pass_at_least_one_jet_passes_jet_OR(vector<Jet> jets);
	bool pass_bad_jet(vector<Jet> jets);
	bool pass_at_least_one_signal_jet(vector<Jet> jets);
	bool pass_cosmic_muon_veto(vector<Muon> muons);
	bool pass_at_least_two_baseline_leptons_greater_than_10GeV(vector<Lepton> lept);
	bool pass_at_least_two_signal_leptons_greater_than_20GeV(vector<Lepton> lept);
	bool pass_same_sign(vector<Lepton> lept);
	// same-sign
	int  pass_channel_separation(vector<Lepton> lept);
	bool pass_ee_2015_trigger_matching(vector<Electron> elec, bool HLT_2e12_lhloose_L12EM10VH, bool HLT_xe70, double Etmiss_TST_Et);
	bool pass_ee_2016_trigger_matching(vector<Electron> elec, bool HLT_2e17_lhvloose_nod0, bool HLT_xe100_mht_L1XE50, double Etmiss_TST_Et);
	bool pass_emu_2015_trigger_matching(vector<Electron> elec, vector<Muon> muon, bool HLT_e17_lhloose_mu14, bool HLT_xe70, double Etmiss_TST_Et);
	bool pass_emu_2016_trigger_matching(vector<Electron> elec, vector<Muon> muon, bool HLT_e17_lhloose_nod0_mu14, bool HLT_xe100_mht_L1XE50, double Etmiss_TST_Et);
	bool pass_mumu_2015_trigger_matching(vector<Muon> muon, bool HLT_mu18_mu8noL1, bool HLT_xe70, double Etmiss_TST_Et);
	bool pass_mumu_2016_trigger_matching(vector<Muon> muon, bool HLT_mu20_mu8noL1, bool HLT_xe100_mht_L1XE50, double Etmiss_TST_Et);
	bool pass_trigger_matching(bool isData, bool isMC, int run_number, int random_run_number, vector<Electron> elec, vector<Muon> muon,
							   bool HLT_2e12_lhloose_L12EM10VH, bool HLT_e17_lhloose_mu14, bool HLT_mu18_mu8noL1, bool HLT_xe70,
							   bool HLT_2e17_lhvloose_nod0, bool HLT_e17_lhloose_nod0_mu14, bool HLT_mu20_mu8noL1, bool HLT_xe100_mht_L1XE50,
					 		   float Etmiss_TST_Et);
	bool pass_at_least_one_bjet_greater_than_20GeV(vector<Jet> jets);
	bool pass_four_jets_greater_than_50GeV(vector<Jet> jets);
	bool pass_MET_greater_than_125GeV(float MET);

	ClassDef(yt_cutflows, 0);
};

#endif // #ifndef YT_CUTFLOWS_H
