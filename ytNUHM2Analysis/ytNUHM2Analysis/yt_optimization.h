#ifndef YT_OPTIMIZATION_H
#define YT_OPTIMIZATION_H

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

#include "ytNUHM2Analysis/Leptons.h"
#include "ytNUHM2Analysis/Jet.h"
#include "ytNUHM2Analysis/yt_useful_functions.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

class yt_optimization
{
public:
    // flag
    bool isMC;
    bool isData;

    // user defined variables
    string  process;

    // event information
    int  run_number;
    long event_number;

    double derivation_stat_weights;

    float luminosity;

    float cross_section;
    float k_factor;
    float filter_efficiency;
    float cross_section_kfactor_efficiency;

    // Weights
    double weight;
    float  event_weight;
    float  lepton_weight;
    float  jet_weight;
    float  pileup_weight;

    vector<Electron>    vec_signal_elec;
    vector<Muon>        vec_signal_muon;
    vector<Lepton>      vec_signal_lept;
    vector<Jet>         vec_signal_jets;
    vector<Jet>         vec_signal_bjet;

    vector<Jet>         vec_signal_jets_with_pt_cut;
    vector<Jet>         vec_signal_bjet_with_pt_cut;

    // Declare the output
    TFile   *output_file;
    TTree   *output_tree;

    TH1F    *h_derivation_stat_weights;
    TH1F    *h_cross_section;
    TH1F    *h_k_factor;
    TH1F    *h_filter_efficiency;
    TH1F    *h_cross_section_kfactor_efficiency;

    TH1F    *h_met;
    TH1F    *h_Ht;
    TH1F    *h_meff;
    TH1F    *h_met_over_meff;
    TH1F    *h_NLeptons;
    TH1F    *h_NJets;
    TH1F    *h_Nbjets;
    TH1F    *h_lep1_pT;
    TH1F    *h_lep2_pT;
    TH1F    *h_jet1_pT;
    TH1F    *h_jet2_pT;

    TH1F    *h_yields;
    TH1F    *h_yields_weighted;

    TH3F    *h_method2_yields;
    TH3F    *h_method2_yields_weighted;

    TH1F    *h_events_survived;
    TH1F    *h_events_survived_weighted;

    // leafs in tree
    // For distributions
    float   Ht;
    float   lepton1_pT;
    float   lepton2_pT;
    float   jet1_pT;
    float   jet2_pT;

    int             n_leptons;
    int             n_bjets;
    vector<float>   bjet_pT;
    int             n_jets;
    vector<float>   jet_pT;
    float           met;
    float           meff;
    float           met_over_meff;

    int n_bjet_pTX; // Number of b-jets with pT > X
    int n_jets_pTX; // Number of jets with pT > X

    // Cuts
    static const float N_lept_cuts[5];
    static const float N_bjet_cuts[6];
    static const float N_jets_cuts[9];
    static const float bjet_pt_cuts[9];
    static const float jets_pt_cuts[9];
    static const float met_cuts[12];
    static const float meff_cuts[12];

    // Binning
    vector<float> m_N_lept_cuts_bins;
    vector<float> m_N_bjet_cuts_bins;
    vector<float> m_N_jets_cuts_bins;
    vector<float> m_bjet_pt_cuts_bins;
    vector<float> m_jets_pt_cuts_bins;
    vector<float> m_met_cuts_bins;
    vector<float> m_meff_cuts_bins;

    // Optimization cut bins
    int n_N_lept_cuts_bins;
    int n_N_bjet_cuts_bins;
    int n_N_jets_cuts_bins;
    int n_bjet_pt_cuts_bins;
    int n_jets_pt_cuts_bins;
    int n_met_cuts_bins;
    int n_meff_cuts_bins;

    int     events_survived;
    double  events_survived_weighted;

public:
    yt_optimization();
    virtual ~yt_optimization();

    void initialize();
    void execute(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets);
    void finalize();

    void set_isMC(bool b) { isMC = b; }
    void set_isData(bool b) { isData = b; }
    void set_process(string s) { process = s; }
    void set_run_number(int i) { run_number = i; }
    void set_event_number(long l) { event_number = l; }
    void set_luminosity(float f) { luminosity = f; }
    void set_cross_section(float f) { cross_section = f; }
    void set_k_factor(float f) { k_factor = f; }
    void set_filter_efficiency(float f) { filter_efficiency = f; }
    void set_cross_section_kfactor_efficiency(float f) { cross_section_kfactor_efficiency = f; }
    // void set_derivation_stat_weights(double d) { derivation_stat_weights = d; h_derivation_stat_weights->SetBinContent(1, derivation_stat_weights); }
    void set_derivation_stat_weights(double d) { derivation_stat_weights = d; }
    void set_event_weight(double d) { event_weight = d; }
    void set_pileup_weight(double d) { pileup_weight = d; }
    void set_lepton_weight(double d) { lepton_weight = d; }
    void set_jet_weight(double d) { jet_weight = d; }
    void set_met(double d) { met = d; }
    void set_binning_default();

    void reset_vectors();
    void copy_vectors(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets);
    void fill_signal_bjets(vector<Jet> signal_jets);
    void fill_Nbjets_pT();
    void fill_vec_jets_with_pT_cut(string jet, int pt);

    void apply_signal_region_cuts(int cut_n_leptons,
                                  int cut_bjets_pT, int cut_n_bjets,
                                  int cut_jet_pt, int cut_n_jet,
                                  int cut_met, int cut_meff,
                                  float cut_met_over_meff,
                                  float weight);
    void debug_print(string s);

    ClassDef(yt_optimization, 0);
};

#endif // #ifndef YT_OPTIMIZATION_H
