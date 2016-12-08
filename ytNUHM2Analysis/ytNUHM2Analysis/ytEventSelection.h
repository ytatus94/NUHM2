#ifndef ytEventSelection_H
#define ytEventSelection_H

#include <EventLoop/Algorithm.h>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>

#include "SUSYTools/SUSYCrossSection.h"
#include "PathResolver/PathResolver.h"

#include "ytNUHM2Analysis/Leptons.h"
#include "ytNUHM2Analysis/Jet.h"
#include "ytNUHM2Analysis/yt_cutflows.h"
#include "ytNUHM2Analysis/yt_optimization.h"
//#include "ytNUHM2Analysis/yt_useful_functions.h"
#include "ytNUHM2Analysis/DileptonTriggerWeight.h"

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <typeinfo>
using namespace std;

#define Mu_Mass 105.6583715

template<typename T> bool sort_descending_Pt(T obj1, T obj2); // NOTE: cannot use const ref for arguments.
template<> bool sort_descending_Pt(TLorentzVector tlv1, TLorentzVector tlv2); // explicit specialization
/*
double calculate_Ht(vector<Lepton> signal_leptons, vector<Jet> signal_jets);
double calculate_Meff(double Ht, double Etmiss);
*/

class ytEventSelection : public EL::Algorithm
{
    // put your configuration variables here as public variables.
    // that way they can be set directly from CINT and python.
public:
    // flags
    bool isMC;
    bool isData;
    bool isSkim;
    bool isOptimization;
    bool isFullSim;
    bool isAF2Sim;

    // user defined variables
    string process;

    double derivation_stat_weights; //!

    float luminosity;
    float cross_section; //!
    float k_factor; //!
    float filter_efficiency; //!
    float cross_section_kfactor_efficiency; //!

    //vector<double>        sum_of_weight_at_cut; //!

    yt_cutflows         *m_cutflow; //!
    yt_optimization     *m_optimization; //!

    SUSY::CrossSectionDB    *m_XsecDB; //!
    DileptonTriggerWeight   *m_dtwTool;

    vector<Electron>    vec_elec; //!
    vector<Muon>        vec_muon; //!
    vector<Lepton>      vec_lept; //!
    vector<Jet>         vec_jets; //!

    vector<Electron>    vec_baseline_elec; //!
    vector<Muon>        vec_baseline_muon; //!
    vector<Lepton>      vec_baseline_lept; //!
    vector<Jet>         vec_baseline_jets; //!

    vector<Electron>    vec_OR_elec; //!
    vector<Muon>        vec_OR_muon; //!
    vector<Lepton>      vec_OR_lept; //!
    vector<Jet>         vec_OR_jets; //!

    vector<Jet>         vec_JVT_jets; //!

    vector<Electron>    vec_signal_elec; //!
    vector<Muon>        vec_signal_muon; //!
    vector<Lepton>      vec_signal_lept; //!
    vector<Jet>         vec_signal_jets; //!


    // variables that don't get filled at submission time should be
    // protected from being send from the submission node to the worker
    // node (done by the //!)
public:
    // Tree *myTree; //!
    // TH1 *myHist; //!
    //
    // Tree and branches copy from TSelector
    //
    TTree           *fChain; //! pointer to the analyzed TTree or TChain

    // Declaration of leaf types
    Bool_t          HLT_e24_lhmedium_nod0_ivarloose; //!
    Bool_t          HLT_e24_lhtight_nod0_ivarloose; //!
    Bool_t          HLT_e24_lhmedium_nod0_L1EM20VH; //!
    Bool_t          HLT_e26_lhtight_iloose; //!
    Bool_t          HLT_e26_lhtight_ivarloose; //!
    Bool_t          HLT_e26_lhtight_nod0_iloose; //!
    Bool_t          HLT_e26_lhtight_nod0_ivarloose; //!
    Bool_t          HLT_e60_lhmedium; //!
    Bool_t          HLT_e60_lhmedium_nod0; //!
    Bool_t          HLT_e120_lhloose_nod0; //!
    Bool_t          HLT_e140_lhloose_nod0; //!
    Bool_t          HLT_2e17_lhvloose_nod0; //!
    Bool_t          HLT_2e15_lhvloose_nod0_L12EM13VH; //!
    Bool_t          HLT_e24_lhmedium_e9_lhmedium; //!
    Bool_t          HLT_e24_lhmedium_L1EM20VH; //!
    Bool_t          HLT_e12_lhvloose_L12EM10VH; //!
    Bool_t          HLT_e17_lhloose_2e9_lhloose; //!
    Bool_t          HLT_mu24_ivarmedium; //!
    Bool_t          HLT_mu24_imedium; //!
    Bool_t          HLT_mu24_ivarloose; //!
    Bool_t          HLT_mu24_iloose; //!
    Bool_t          HLT_mu26_ivarmedium; //!
    Bool_t          HLT_mu26_imedium; //!
    Bool_t          HLT_mu20_ivarmedium_L1MU15; //!
    Bool_t          HLT_mu20_imedium_L1MU15; //!
    Bool_t          HLT_mu20_ivarloose_L1MU15; //!
    Bool_t          HLT_mu20_iloose_L1MU15; //!
    Bool_t          HLT_mu20_L1MU15; //!
    Bool_t          HLT_mu20_mu8noL1; //!
    Bool_t          HLT_mu22_mu8noL1; //!
    Bool_t          HLT_mu20_2mu4noL1; //!
    Bool_t          HLT_mu22_2mu4noL1; //!
    Bool_t          HLT_mu40; //!
    Bool_t          HLT_mu50; //!
    Bool_t          HLT_2mu10; //!
    Bool_t          HLT_2mu10_nomucomb; //!
    Bool_t          HLT_2mu14; //!
    Bool_t          HLT_2mu14_nomucomb; //!
    Bool_t          HLT_3mu6; //!
    Bool_t          HLT_3mu6_msonly; //!
    Bool_t          HLT_xe100_L1XE50; //!
    Bool_t          HLT_xe80_mht_L1XE50; //!
    Bool_t          HLT_xe90_mht_L1XE50; //!
    Bool_t          HLT_xe100_mht_L1XE50; //!
    Bool_t          HLT_xe110_pueta_L1XE50; //!
    Bool_t          HLT_xe110_pufit_L1XE50; //!
    Bool_t          HLT_xe100_tc_em_L1XE50; //!
    Bool_t          HLT_xe80_tc_lcw_L1XE50; //!
    Bool_t          HLT_xe90_tc_lcw_L1XE50; //!
    Bool_t          HLT_xe80_tc_lcw_wEFMu_L1XE50; //!
    Bool_t          HLT_e7_lhmedium_mu24; //!
    Bool_t          HLT_e17_lhloose_nod0_mu14; //!
    Bool_t          HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1; //!
    Bool_t          HLT_e24_lhmedium_nod0_L1EM22VHI_mu8noL1; //!
    Bool_t          HLT_2e12_lhloose_L12EM10VH; //!
    Bool_t          HLT_e17_lhloose_mu14; //!
    Bool_t          HLT_mu18_mu8noL1; //!
    Bool_t          HLT_xe70; //!
    ULong64_t       EventNumber; //!
    Int_t           ChannelNumber; //!
    Double_t        AvgMu; //!
    Double_t        EventWeight; //!
    Double_t        PRWWeight; //!
    Int_t           PRWrandomRunNumber; //!
    Int_t           bcid; //!
    Int_t           LB; //!
    Int_t           RunNb; //!
    Int_t           DetError; //!
    Int_t           NMu; //!
    vector<double>  *Mu_eta; //!
    vector<double>  *Mu_phi; //!
    vector<double>  *Mu_pT; //!
    vector<double>  *Mu_SFw; //!
    vector<float>   *Mu_IsoSFw; //!
    vector<int>     *Mu_charge; //!
    vector<double>  *Mu_d0pvtx; //!
    vector<double>  *Mu_sigd0; //!
    vector<double>  *Mu_z0sinTheta; //!
    vector<bool>    *Mu_isBad; //!
    vector<bool>    *Mu_passOR; //!
    vector<bool>    *Mu_isTight; //!
    vector<bool>    *Mu_isCosmic; //!
    //#ifdef _IS_MC_
    vector<int>     *Mu_type; //!
    vector<int>     *Mu_origin; //!
    //#endif // #ifdef _IS_MC_
    vector<double>  *Mu_ptcone20; //!
    vector<double>  *Mu_ptcone30; //!
    vector<double>  *Mu_ptcone40; //!
    vector<double>  *Mu_ptvarcone20; //!
    vector<double>  *Mu_ptvarcone30; //!
    vector<double>  *Mu_ptvarcone40; //!
    vector<double>  *Mu_topoetcone20; //!
    vector<double>  *Mu_topoetcone30; //!
    vector<double>  *Mu_topoetcone40; //!
    vector<bool>    *Mu_passIsoLooseTO; //!
    vector<bool>    *Mu_passIsoLoose; //!
    vector<bool>    *Mu_passIsoTight; //!
    vector<bool>    *Mu_passIsoGrad; //!
    vector<bool>    *Mu_passIsoGradCustomTight; //!
    vector<bool>    *Mu_passIsoGradCustom; //!
    vector<bool>    *Mu_passIsoGradLoose; //!
    Float_t         MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50; //!
    vector<bool>    *Mu_trigMatch_mu26_imedium; //!
    vector<bool>    *Mu_trigMatch_mu50; //!
    vector<bool>    *Mu_trigMatch_mu8noL1; //!
    vector<bool>    *Mu_trigMatch_mu14; //!
    vector<bool>    *Mu_trigMatch_mu18; //!
    vector<bool>    *Mu_trigMatch_mu18_mu8noL1; //!
    vector<bool>    *Mu_trigMatch_e17_lhloose_mu14; //!
    vector<bool>    *Mu_trigMatch_e17_lhloose_nod0_mu14; //!
    vector<bool>    *Mu_trigMatch_mu20_mu8noL1; //!
    vector<bool>    *Mu_trigMatch_mu22_mu8noL1; //!
    vector<bool>    *Mu_TrigMatch_mu24_iloose; //!
    vector<bool>    *Mu_TrigMatch_mu24_ivarloose; //!
    vector<bool>    *Mu_TrigMatch_mu24_iloose_L1MU15; //!
    vector<bool>    *Mu_TrigMatch_mu24_ivarloose_L1MU15; //!
    vector<vector<bool> > *Mu_trigMatchPair_mu18_mu8noL1; //!
    vector<vector<bool> > *Mu_trigMatchPair_mu20_mu8noL1; //!
    vector<vector<bool> > *Mu_trigMatchPair_mu22_mu8noL1; //!
    Int_t           NEl; //!
    vector<double>  *El_eta; //!
    vector<double>  *El_etaclus; //!
    vector<double>  *El_phi; //!
    vector<double>  *El_pT; //!
    vector<double>  *El_E; //!
    vector<int>     *El_charge; //!
    vector<double>  *El_sigd0; //!
    vector<double>  *El_z0sinTheta; //!
    vector<double>  *El_d0pvtx; //!
    vector<bool>    *El_passOR; //!
    vector<double>  *El_SFwMediumLH; //!
    vector<float>   *El_IsoSFwMediumLH; //!
    vector<double>  *El_SFwTightLH; //!
    vector<double>  *El_SFwLooseAndBLayerLH; //!
    vector<double>  *El_SFwTrigMediumLH_e12_lhloose_L1EM10VH; //!
    vector<double>  *El_SFwTrigMediumLH_e17_lhloose; //!
    vector<double>  *El_SFwTrigMediumLH_single; //!
    vector<double>  *El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH; //!
    vector<bool>    *El_isLooseAndBLayerLH; //!
    vector<bool>    *El_isMediumLH; //!
    vector<bool>    *El_isTightLH; //!
    vector<int>     *El_nBLayerHits; //!
    vector<int>     *El_expectBLayerHit; //!
    //#ifdef _IS_MC_
    vector<int>     *El_type; //!
    vector<int>     *El_origin; //!
    vector<int>     *El_bkgMotherPdgId; //!
    vector<int>     *El_bkgOrigin; //!
    vector<int>     *El_chFlip; //!
    //#endif // #ifdef _IS_MC_
    vector<double>  *El_ptcone20; //!
    vector<double>  *El_ptcone30; //!
    vector<double>  *El_ptcone40; //!
    vector<double>  *El_ptvarcone20; //!
    vector<double>  *El_ptvarcone30; //!
    vector<double>  *El_ptvarcone40; //!
    vector<double>  *El_topoetcone20; //!
    vector<double>  *El_topoetcone30; //!
    vector<double>  *El_topoetcone40; //!
    vector<bool>    *El_passIsoLooseTO; //!
    vector<bool>    *El_passIsoLoose; //!
    vector<bool>    *El_passIsoTight; //!
    vector<bool>    *El_passIsoGrad; //!
    vector<bool>    *El_passIsoGradCustomTight; //!
    vector<bool>    *El_passIsoGradCustom; //!
    vector<bool>    *El_passIsoGradLoose; //!
    vector<bool>    *El_trigMatch_e12_lhloose_L1EM10VH; //!
    vector<bool>    *El_trigMatch_e17_lhloose; //!
    vector<bool>    *El_trigMatch_e60_lhmedium; //!
    vector<bool>    *El_trigMatch_e24_lhmedium_iloose_L1EM20VH; //!
    vector<bool>    *El_trigMatch_2e12_lhloose_L12EM10VH; //!
    vector<bool>    *El_trigMatch_2e15_lhloose_L12EM10VH; //!
    vector<bool>    *El_trigMatch_2e15_lhvloose_L12EM13VH; //!
    vector<bool>    *El_trigMatch_2e15_lhvloose_nod0_L12EM13VH; //!
    vector<bool>    *El_trigMatch_2e17_lhvloose_nod0; //!
    vector<bool>    *El_trigMatch_e17_lhloose_mu14; //!
    vector<bool>    *El_trigMatch_e17_lhloose_nod0_mu14; //!
    vector<bool>    *El_TrigMatch_e24_lhmedium_nod0_ivarloose; //!
    vector<bool>    *El_TrigMatch_e24_lhtight_nod0_ivarloose; //!
    vector<bool>    *El_TrigMatch_e60_lhmedium_nod0; //!
    Int_t           NJet; //!
    vector<double>  *Jet_eta; //!
    vector<double>  *Jet_phi; //!
    vector<double>  *Jet_pT; //!
    vector<double>  *Jet_E; //!
    vector<double>  *Jet_quality; //!
    vector<double>  *Jet_JVT; //!
    vector<double>  *Jet_JVTsf; //!
    vector<double>  *Jet_MV2c20; //!
    vector<double>  *Jet_MV2c10; //!
    vector<double>  *Jet_SFw; //!
    //#ifdef _IS_MC_
    vector<int>     *Jet_ConeTruthLabel; //!
    vector<int>     *Jet_PartonTruthLabel; //!
    vector<int>     *Jet_HadronConeExclTruthLabel; //!
    vector<double>  *Jet_deltaR; //!
    //#endif // #ifdef _IS_MC_
    vector<int>     *Jet_nTrk; //!
    vector<bool>    *Jet_passOR; //!
    Float_t         Etmiss_CST_Etx; //!
    Float_t         Etmiss_CST_Ety; //!
    Float_t         Etmiss_CST_Et; //!
    Float_t         Etmiss_TST_Etx; //!
    Float_t         Etmiss_TST_Ety; //!
    Float_t         Etmiss_TST_Et; //!
    Float_t         Etmiss_TSTterm_Etx; //!
    Float_t         Etmiss_TSTterm_Ety; //!
    Float_t         Etmiss_TSTterm_Et; //!
    Float_t         Etmiss_Truth_Etx; //!
    Float_t         Etmiss_Truth_Ety; //!
    Float_t         Etmiss_Truth_Et; //!
    //#ifdef _IS_MC_
    Int_t           NTruthJet; //!
    vector<double>  *TruthJet_eta; //!
    vector<double>  *TruthJet_phi; //!
    vector<double>  *TruthJet_pT; //!
    vector<double>  *TruthJet_E; //!
    vector<int>     *TruthJet_ConeTruthLabel; //!
    vector<int>     *TruthJet_PartonTruthLabel; //!
    Int_t           NTruthL; //!
    vector<double>  *TruthL_eta; //!
    vector<double>  *TruthL_phi; //!
    vector<double>  *TruthL_pT; //!
    vector<int>     *TruthL_id; //!
    Int_t           SUSY_Spart_pdgId1; //!
    Int_t           SUSY_Spart_pdgId2; //!
    Int_t           SUSY_Gluino_decay1; //!
    Int_t           SUSY_Gluino_decay2; //!
    Float_t         GenFiltHT; //!
    Float_t         GenFiltMET; //!
    //#endif // #ifdef _IS_MC_
    Float_t         PV_z; //!
    Int_t           Nvtx; //!
    Float_t         TruthX1; //!
    Float_t         TruthX2; //!
    Float_t         TruthQ; //!
    Float_t         SherpaNjetWeight; //!
    Int_t           TruthPDGID1; //!
    Int_t           TruthPDGID2; //!

    // List of branches
    TBranch         *b_HLT_e24_lhmedium_nod0_ivarloose; //!
    TBranch         *b_HLT_e24_lhtight_nod0_ivarloose; //!
    TBranch         *b_HLT_e24_lhmedium_nod0_L1EM20VH; //!
    TBranch         *b_HLT_e26_lhtight_iloose; //!
    TBranch         *b_HLT_e26_lhtight_ivarloose; //!
    TBranch         *b_HLT_e26_lhtight_nod0_iloose; //!
    TBranch         *b_HLT_e26_lhtight_nod0_ivarloose; //!
    TBranch         *b_HLT_e60_lhmedium; //!
    TBranch         *b_HLT_e60_lhmedium_nod0; //!
    TBranch         *b_HLT_e120_lhloose_nod0; //!
    TBranch         *b_HLT_e140_lhloose_nod0; //!
    TBranch         *b_HLT_2e17_lhvloose_nod0; //!
    TBranch         *b_HLT_2e15_lhvloose_nod0_L12EM13VH; //!
    TBranch         *b_HLT_e24_lhmedium_e9_lhmedium; //!
    TBranch         *b_HLT_e24_lhmedium_L1EM20VH; //!
    TBranch         *b_HLT_e12_lhvloose_L12EM10VH; //!
    TBranch         *b_HLT_e17_lhloose_2e9_lhloose; //!
    TBranch         *b_HLT_mu24_ivarmedium; //!
    TBranch         *b_HLT_mu24_imedium; //!
    TBranch         *b_HLT_mu24_ivarloose; //!
    TBranch         *b_HLT_mu24_iloose; //!
    TBranch         *b_HLT_mu26_ivarmedium; //!
    TBranch         *b_HLT_mu26_imedium; //!
    TBranch         *b_HLT_mu20_ivarmedium_L1MU15; //!
    TBranch         *b_HLT_mu20_imedium_L1MU15; //!
    TBranch         *b_HLT_mu20_ivarloose_L1MU15; //!
    TBranch         *b_HLT_mu20_iloose_L1MU15; //!
    TBranch         *b_HLT_mu20_L1MU15; //!
    TBranch         *b_HLT_mu20_mu8noL1; //!
    TBranch         *b_HLT_mu22_mu8noL1; //!
    TBranch         *b_HLT_mu20_2mu4noL1; //!
    TBranch         *b_HLT_mu22_2mu4noL1; //!
    TBranch         *b_HLT_mu40; //!
    TBranch         *b_HLT_mu50; //!
    TBranch         *b_HLT_2mu10; //!
    TBranch         *b_HLT_2mu10_nomucomb; //!
    TBranch         *b_HLT_2mu14; //!
    TBranch         *b_HLT_2mu14_nomucomb; //!
    TBranch         *b_HLT_3mu6; //!
    TBranch         *b_HLT_3mu6_msonly; //!
    TBranch         *b_HLT_xe100_L1XE50; //!
    TBranch         *b_HLT_xe80_mht_L1XE50; //!
    TBranch         *b_HLT_xe90_mht_L1XE50; //!
    TBranch         *b_HLT_xe100_mht_L1XE50; //!
    TBranch         *b_HLT_xe110_pueta_L1XE50; //!
    TBranch         *b_HLT_xe110_pufit_L1XE50; //!
    TBranch         *b_HLT_xe100_tc_em_L1XE50; //!
    TBranch         *b_HLT_xe80_tc_lcw_L1XE50; //!
    TBranch         *b_HLT_xe90_tc_lcw_L1XE50; //!
    TBranch         *b_HLT_xe80_tc_lcw_wEFMu_L1XE50; //!
    TBranch         *b_HLT_e7_lhmedium_mu24; //!
    TBranch         *b_HLT_e17_lhloose_nod0_mu14; //!
    TBranch         *b_HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1; //!
    TBranch         *b_HLT_e24_lhmedium_nod0_L1EM22VHI_mu8noL1; //!
    TBranch         *b_HLT_2e12_lhloose_L12EM10VH; //!
    TBranch         *b_HLT_e17_lhloose_mu14; //!
    TBranch         *b_HLT_mu18_mu8noL1; //!
    TBranch         *b_HLT_xe70; //!
    TBranch         *b_EventNumber; //!
    TBranch         *b_ChannelNumber; //!
    TBranch         *b_AvgMu; //!
    TBranch         *b_EventWeight; //!
    TBranch         *b_PRWWeight; //!
    TBranch         *b_PRWrandomRunNumber; //!
    TBranch         *b_bcid; //!
    TBranch         *b_LB; //!
    TBranch         *b_RunNb; //!
    TBranch         *b_DetError; //!
    TBranch         *b_NMu; //!
    TBranch         *b_Mu_eta; //!
    TBranch         *b_Mu_phi; //!
    TBranch         *b_Mu_pT; //!
    TBranch         *b_Mu_SFw; //!
    TBranch         *b_Mu_IsoSFw; //!
    TBranch         *b_Mu_charge; //!
    TBranch         *b_Mu_d0pvtx; //!
    TBranch         *b_Mu_sigd0; //!
    TBranch         *b_Mu_z0sinTheta; //!
    TBranch         *b_Mu_isBad; //!
    TBranch         *b_Mu_passOR; //!
    TBranch         *b_Mu_isTight; //!
    TBranch         *b_Mu_isCosmic; //!
    //#ifdef _IS_MC_
    TBranch         *b_Mu_type; //!
    TBranch         *b_Mu_origin; //!
    //#endif // #ifdef _IS_MC_
    TBranch         *b_Mu_ptcone20; //!
    TBranch         *b_Mu_ptcone30; //!
    TBranch         *b_Mu_ptcone40; //!
    TBranch         *b_Mu_ptvarcone20; //!
    TBranch         *b_Mu_ptvarcone30; //!
    TBranch         *b_Mu_ptvarcone40; //!
    TBranch         *b_Mu_topoetcone20; //!
    TBranch         *b_Mu_topoetcone30; //!
    TBranch         *b_Mu_topoetcone40; //!
    TBranch         *b_Mu_passIsoLooseTO; //!
    TBranch         *b_Mu_passIsoLoose; //!
    TBranch         *b_Mu_passIsoTight; //!
    TBranch         *b_Mu_passIsoGrad; //!
    TBranch         *b_Mu_passIsoGradCustomTight; //!
    TBranch         *b_Mu_passIsoGradCustom; //!
    TBranch         *b_Mu_passIsoGradLoose; //!
    TBranch         *b_MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50; //!
    TBranch         *b_Mu_trigMatch_mu26_imedium; //!
    TBranch         *b_Mu_trigMatch_mu50; //!
    TBranch         *b_Mu_trigMatch_mu8noL1; //!
    TBranch         *b_Mu_trigMatch_mu14; //!
    TBranch         *b_Mu_trigMatch_mu18; //!
    TBranch         *b_Mu_trigMatch_mu18_mu8noL1; //!
    TBranch         *b_Mu_trigMatch_e17_lhloose_mu14; //!
    TBranch         *b_Mu_trigMatch_e17_lhloose_nod0_mu14; //!
    TBranch         *b_Mu_trigMatch_mu20_mu8noL1; //!
    TBranch         *b_Mu_trigMatch_mu22_mu8noL1; //!
    TBranch         *b_Mu_TrigMatch_mu24_iloose; //!
    TBranch         *b_Mu_TrigMatch_mu24_ivarloose; //!
    TBranch         *b_Mu_TrigMatch_mu24_iloose_L1MU15; //!
    TBranch         *b_Mu_TrigMatch_mu24_ivarloose_L1MU15; //!
    TBranch         *b_Mu_trigMatchPair_mu18_mu8noL1; //!
    TBranch         *b_Mu_trigMatchPair_mu20_mu8noL1; //!
    TBranch         *b_Mu_trigMatchPair_mu22_mu8noL1; //!
    TBranch         *b_NEl; //!
    TBranch         *b_El_eta; //!
    TBranch         *b_El_etaclus; //!
    TBranch         *b_El_phi; //!
    TBranch         *b_El_pT; //!
    TBranch         *b_El_E; //!
    TBranch         *b_El_charge; //!
    TBranch         *b_El_sigd0; //!
    TBranch         *b_El_z0sinTheta; //!
    TBranch         *b_El_d0pvtx; //!
    TBranch         *b_El_passOR; //!
    TBranch         *b_El_SFwMediumLH; //!
    TBranch         *b_El_IsoSFwMediumLH; //!
    TBranch         *b_El_SFwTightLH; //!
    TBranch         *b_El_SFwLooseAndBLayerLH; //!
    TBranch         *b_El_SFwTrigMediumLH_e12_lhloose_L1EM10VH; //!
    TBranch         *b_El_SFwTrigMediumLH_e17_lhloose; //!
    TBranch         *b_El_SFwTrigMediumLH_single; //!
    TBranch         *b_El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH; //!
    TBranch         *b_El_isLooseAndBLayerLH; //!
    TBranch         *b_El_isMediumLH; //!
    TBranch         *b_El_isTightLH; //!
    TBranch         *b_El_nBLayerHits; //!
    TBranch         *b_El_expectBLayerHit; //!
    //#ifdef _IS_MC_
    TBranch         *b_El_type; //!
    TBranch         *b_El_origin; //!
    TBranch         *b_El_bkgMotherPdgId; //!
    TBranch         *b_El_bkgOrigin; //!
    TBranch         *b_El_chFlip; //!
    //#endif // #ifdef _IS_MC_
    TBranch         *b_El_ptcone20; //!
    TBranch         *b_El_ptcone30; //!
    TBranch         *b_El_ptcone40; //!
    TBranch         *b_El_ptvarcone20; //!
    TBranch         *b_El_ptvarcone30; //!
    TBranch         *b_El_ptvarcone40; //!
    TBranch         *b_El_topoetcone20; //!
    TBranch         *b_El_topoetcone30; //!
    TBranch         *b_El_topoetcone40; //!
    TBranch         *b_El_passIsoLooseTO; //!
    TBranch         *b_El_passIsoLoose; //!
    TBranch         *b_El_passIsoTight; //!
    TBranch         *b_El_passIsoGrad; //!
    TBranch         *b_El_passIsoGradCustomTight; //!
    TBranch         *b_El_passIsoGradCustom; //!
    TBranch         *b_El_passIsoGradLoose; //!
    TBranch         *b_El_trigMatch_e12_lhloose_L1EM10VH; //!
    TBranch         *b_El_trigMatch_e17_lhloose; //!
    TBranch         *b_El_trigMatch_e60_lhmedium; //!
    TBranch         *b_El_trigMatch_e24_lhmedium_iloose_L1EM20VH; //!
    TBranch         *b_El_trigMatch_2e12_lhloose_L12EM10VH; //!
    TBranch         *b_El_trigMatch_2e15_lhloose_L12EM10VH; //!
    TBranch         *b_El_trigMatch_2e15_lhvloose_L12EM13VH; //!
    TBranch         *b_El_trigMatch_2e15_lhvloose_nod0_L12EM13VH; //!
    TBranch         *b_El_trigMatch_2e17_lhvloose_nod0; //!
    TBranch         *b_El_trigMatch_e17_lhloose_mu14; //!
    TBranch         *b_El_trigMatch_e17_lhloose_nod0_mu14; //!
    TBranch         *b_El_TrigMatch_e24_lhmedium_nod0_ivarloose; //!
    TBranch         *b_El_TrigMatch_e24_lhtight_nod0_ivarloose; //!
    TBranch         *b_El_TrigMatch_e60_lhmedium_nod0; //!
    TBranch         *b_NJet; //!
    TBranch         *b_Jet_eta; //!
    TBranch         *b_Jet_phi; //!
    TBranch         *b_Jet_pT; //!
    TBranch         *b_Jet_E; //!
    TBranch         *b_Jet_quality; //!
    TBranch         *b_Jet_JVT; //!
    TBranch         *b_Jet_JVTsf; //!
    TBranch         *b_Jet_MV2c20; //!
    TBranch         *b_Jet_MV2c10; //!
    TBranch         *b_Jet_SFw; //!
    //#ifdef _IS_MC_
    TBranch         *b_Jet_ConeTruthLabel; //!
    TBranch         *b_Jet_PartonTruthLabel; //!
    TBranch         *b_Jet_HadronConeExclTruthLabel; //!
    TBranch         *b_Jet_deltaR; //!
    //#endif // #ifdef _IS_MC_
    TBranch         *b_Jet_nTrk; //!
    TBranch         *b_Jet_passOR; //!
    TBranch         *b_Etmiss_CST_Etx; //!
    TBranch         *b_Etmiss_CST_Ety; //!
    TBranch         *b_Etmiss_CST_Et; //!
    TBranch         *b_Etmiss_TST_Etx; //!
    TBranch         *b_Etmiss_TST_Ety; //!
    TBranch         *b_Etmiss_TST_Et; //!
    TBranch         *b_Etmiss_TSTterm_Etx; //!
    TBranch         *b_Etmiss_TSTterm_Ety; //!
    TBranch         *b_Etmiss_TSTterm_Et; //!
    TBranch         *b_Etmiss_Truth_Etx; //!
    TBranch         *b_Etmiss_Truth_Ety; //!
    TBranch         *b_Etmiss_Truth_Et; //!
    //#ifdef _IS_MC_
    TBranch         *b_NTruthJet; //!
    TBranch         *b_TruthJet_eta; //!
    TBranch         *b_TruthJet_phi; //!
    TBranch         *b_TruthJet_pT; //!
    TBranch         *b_TruthJet_E; //!
    TBranch         *b_TruthJet_ConeTruthLabel; //!
    TBranch         *b_TruthJet_PartonTruthLabel; //!
    TBranch         *b_NTruthL; //!
    TBranch         *b_TruthL_eta; //!
    TBranch         *b_TruthL_phi; //!
    TBranch         *b_TruthL_pT; //!
    TBranch         *b_TruthL_id; //!
    TBranch         *b_SUSY_Spart_pdgId1; //!
    TBranch         *b_SUSY_Spart_pdgId2; //!
    TBranch         *b_SUSY_Gluino_decay1; //!
    TBranch         *b_SUSY_Gluino_decay2; //!
    TBranch         *b_GenFiltHT; //!
    TBranch         *b_GenFiltMET; //!
    //#endif // #ifdef _IS_MC_
    TBranch         *b_PV_z; //!
    TBranch         *b_Nvtx; //!
    TBranch         *b_TruthX1; //!
    TBranch         *b_TruthX2; //!
    TBranch         *b_TruthQ; //!
    TBranch         *b_SherpaNjetWeight; //!
    TBranch         *b_TruthPDGID1; //!
    TBranch         *b_TruthPDGID2; //!

    // Histograms
    TH1F *h_Nvtx; //!
    TH1F *h_AvgMu; //!
    TH1F *h_NLepts; //!
    TH1F *h_NJets; //!

    TH1F *h_Nvtx_weighted; //!
    TH1F *h_AvgMu_weighted; //!
    TH1F *h_NLepts_weighted; //!
    TH1F *h_NJets_weighted; //!

    // Canvas
    TCanvas *c_Nvtx; //!
    TCanvas *c_AvgMu; //!
    TCanvas *c_NLepts; //!
    TCanvas *c_NJets; //!

    // this is a standard constructor
    ytEventSelection ();

    // these are the functions inherited from Algorithm
    virtual EL::StatusCode setupJob (EL::Job& job);
    virtual EL::StatusCode fileExecute ();
    virtual EL::StatusCode histInitialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode postExecute ();
    virtual EL::StatusCode finalize ();
    virtual EL::StatusCode histFinalize ();

    // user defined functions
    //#ifdef _IS_MC_
    void fill_electrons(
        Int_t           NEl,
        int             flavor,
        vector<double>  *El_eta,
        vector<double>  *El_etaclus,
        vector<double>  *El_phi,
        vector<double>  *El_pT,
        vector<double>  *El_E,
        vector<int>     *El_charge,
        vector<double>  *El_sigd0,
        vector<double>  *El_z0sinTheta,
        vector<double>  *El_d0pvtx,
        vector<bool>    *El_passOR,
        vector<double>  *El_SFwMediumLH,
        vector<float>   *El_IsoSFwMediumLH,
        vector<double>  *El_SFwTightLH,
        vector<double>  *El_SFwLooseAndBLayerLH,
        vector<double>  *El_SFwTrigMediumLH_e12_lhloose_L1EM10VH,
        vector<double>  *El_SFwTrigMediumLH_e17_lhloose,
        vector<double>  *El_SFwTrigMediumLH_single,
        vector<double>  *El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH,
        vector<bool>    *El_isLooseAndBLayerLH,
        vector<bool>    *El_isMediumLH,
        vector<bool>    *El_isTightLH,
        vector<int>     *El_nBLayerHits,
        vector<int>     *El_expectBLayerHit,
        vector<int>     *El_type,
        vector<int>     *El_origin,
        vector<int>     *El_bkgMotherPdgId,
        vector<int>     *El_bkgOrigin,
        vector<int>     *El_chFlip,
        vector<double>  *El_ptcone20,
        vector<double>  *El_ptcone30,
        vector<double>  *El_ptcone40,
        vector<double>  *El_ptvarcone20,
        vector<double>  *El_ptvarcone30,
        vector<double>  *El_ptvarcone40,
        vector<double>  *El_topoetcone20,
        vector<double>  *El_topoetcone30,
        vector<double>  *El_topoetcone40,
        vector<bool>    *El_passIsoLooseTO,
        vector<bool>    *El_passIsoLoose,
        vector<bool>    *El_passIsoTight,
        vector<bool>    *El_passIsoGrad,
        vector<bool>    *El_passIsoGradCustomTight,
        vector<bool>    *El_passIsoGradCustom,
        vector<bool>    *El_passIsoGradLoose,
        vector<bool>    *El_trigMatch_e12_lhloose_L1EM10VH,
        vector<bool>    *El_trigMatch_e17_lhloose,
        vector<bool>    *El_trigMatch_e60_lhmedium,
        vector<bool>    *El_trigMatch_e24_lhmedium_iloose_L1EM20VH,
        vector<bool>    *El_trigMatch_2e12_lhloose_L12EM10VH,
        vector<bool>    *El_trigMatch_2e15_lhloose_L12EM10VH,
        vector<bool>    *El_trigMatch_2e15_lhvloose_L12EM13VH,
        vector<bool>    *El_trigMatch_2e15_lhvloose_nod0_L12EM13VH,
        vector<bool>    *El_trigMatch_2e17_lhvloose_nod0,
        vector<bool>    *El_trigMatch_e17_lhloose_mu14,
        vector<bool>    *El_trigMatch_e17_lhloose_nod0_mu14,
        vector<bool>    *El_TrigMatch_e24_lhmedium_nod0_ivarloose,
        vector<bool>    *El_TrigMatch_e24_lhtight_nod0_ivarloose,
        vector<bool>    *El_TrigMatch_e60_lhmedium_nod0);

    void fill_muons(
        Int_t           NMu,
        int             flavor,
        vector<double>  *Mu_eta,
        vector<double>  *Mu_phi,
        vector<double>  *Mu_pT,
        vector<double>  *Mu_SFw,
        vector<float>   *Mu_IsoSFw,
        vector<int>     *Mu_charge,
        vector<double>  *Mu_d0pvtx,
        vector<double>  *Mu_sigd0,
        vector<double>  *Mu_z0sinTheta,
        vector<bool>    *Mu_isBad,
        vector<bool>    *Mu_passOR,
        vector<bool>    *Mu_isTight,
        vector<bool>    *Mu_isCosmic,
        vector<int>     *Mu_type,
        vector<int>     *Mu_origin,
        vector<double>  *Mu_ptcone20,
        vector<double>  *Mu_ptcone30,
        vector<double>  *Mu_ptcone40,
        vector<double>  *Mu_ptvarcone20,
        vector<double>  *Mu_ptvarcone30,
        vector<double>  *Mu_ptvarcone40,
        vector<double>  *Mu_topoetcone20,
        vector<double>  *Mu_topoetcone30,
        vector<double>  *Mu_topoetcone40,
        vector<bool>    *Mu_passIsoLooseTO,
        vector<bool>    *Mu_passIsoLoose,
        vector<bool>    *Mu_passIsoTight,
        vector<bool>    *Mu_passIsoGrad,
        vector<bool>    *Mu_passIsoGradCustomTight,
        vector<bool>    *Mu_passIsoGradCustom,
        vector<bool>    *Mu_passIsoGradLoose,
        Float_t         MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50,
        vector<bool>    *Mu_trigMatch_mu26_imedium,
        vector<bool>    *Mu_trigMatch_mu50,
        vector<bool>    *Mu_trigMatch_mu8noL1,
        vector<bool>    *Mu_trigMatch_mu14,
        vector<bool>    *Mu_trigMatch_mu18,
        vector<bool>    *Mu_trigMatch_mu18_mu8noL1,
        vector<bool>    *Mu_trigMatch_e17_lhloose_mu14,
        vector<bool>    *Mu_trigMatch_e17_lhloose_nod0_mu14,
        vector<bool>    *Mu_trigMatch_mu20_mu8noL1,
        vector<bool>    *Mu_trigMatch_mu22_mu8noL1,
        vector<bool>    *Mu_TrigMatch_mu24_iloose,
        vector<bool>    *Mu_TrigMatch_mu24_ivarloose,
        vector<bool>    *Mu_TrigMatch_mu24_iloose_L1MU15,
        vector<bool>    *Mu_TrigMatch_mu24_ivarloose_L1MU15,
        vector<vector<bool> > *Mu_trigMatchPair_mu18_mu8noL1,
        vector<vector<bool> > *Mu_trigMatchPair_mu20_mu8noL1,
        vector<vector<bool> > *Mu_trigMatchPair_mu22_mu8noL1);

    void fill_jets(
        Int_t           NJet,
        vector<double>  *Jet_eta,
        vector<double>  *Jet_phi,
        vector<double>  *Jet_pT,
        vector<double>  *Jet_E,
        vector<double>  *Jet_quality,
        vector<double>  *Jet_JVT,
        vector<double>  *Jet_JVTsf,
        vector<double>  *Jet_MV2c20,
        vector<double>  *Jet_MV2c10,
        vector<double>  *Jet_SFw,
        vector<int>     *Jet_ConeTruthLabel,
        vector<int>     *Jet_PartonTruthLabel,
        vector<int>     *Jet_HadronConeExclTruthLabel,
        vector<double>  *Jet_deltaR,
        vector<int>     *Jet_nTrk,
        vector<bool>    *Jet_passOR);
    //#endif // #ifdef _IS_MC_

    // Function overload
    //#ifdef _IS_DATA_
    void fill_electrons(
        Int_t           NEl,
        int             flavor,
        vector<double>  *El_eta,
        vector<double>  *El_etaclus,
        vector<double>  *El_phi,
        vector<double>  *El_pT,
        vector<double>  *El_E,
        vector<int>     *El_charge,
        vector<double>  *El_sigd0,
        vector<double>  *El_z0sinTheta,
        vector<double>  *El_d0pvtx,
        vector<bool>    *El_passOR,
        vector<double>  *El_SFwMediumLH,
        vector<float>   *El_IsoSFwMediumLH,
        vector<double>  *El_SFwTightLH,
        vector<double>  *El_SFwLooseAndBLayerLH,
        vector<double>  *El_SFwTrigMediumLH_e12_lhloose_L1EM10VH,
        vector<double>  *El_SFwTrigMediumLH_e17_lhloose,
        vector<double>  *El_SFwTrigMediumLH_single,
        vector<double>  *El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH,
        vector<bool>    *El_isLooseAndBLayerLH,
        vector<bool>    *El_isMediumLH,
        vector<bool>    *El_isTightLH,
        vector<int>     *El_nBLayerHits,
        vector<int>     *El_expectBLayerHit,
/*
        vector<int>     *El_type, // MC only
        vector<int>     *El_origin, // MC only
        vector<int>     *El_bkgMotherPdgId, // MC only
        vector<int>     *El_bkgOrigin, // MC only
        vector<int>     *El_chFlip, // MC only
*/
        vector<double>  *El_ptcone20,
        vector<double>  *El_ptcone30,
        vector<double>  *El_ptcone40,
        vector<double>  *El_ptvarcone20,
        vector<double>  *El_ptvarcone30,
        vector<double>  *El_ptvarcone40,
        vector<double>  *El_topoetcone20,
        vector<double>  *El_topoetcone30,
        vector<double>  *El_topoetcone40,
        vector<bool>    *El_passIsoLooseTO,
        vector<bool>    *El_passIsoLoose,
        vector<bool>    *El_passIsoTight,
        vector<bool>    *El_passIsoGrad,
        vector<bool>    *El_passIsoGradCustomTight,
        vector<bool>    *El_passIsoGradCustom,
        vector<bool>    *El_passIsoGradLoose,
        vector<bool>    *El_trigMatch_e12_lhloose_L1EM10VH,
        vector<bool>    *El_trigMatch_e17_lhloose,
        vector<bool>    *El_trigMatch_e60_lhmedium,
        vector<bool>    *El_trigMatch_e24_lhmedium_iloose_L1EM20VH,
        vector<bool>    *El_trigMatch_2e12_lhloose_L12EM10VH,
        vector<bool>    *El_trigMatch_2e15_lhloose_L12EM10VH,
        vector<bool>    *El_trigMatch_2e15_lhvloose_L12EM13VH,
        vector<bool>    *El_trigMatch_2e15_lhvloose_nod0_L12EM13VH,
        vector<bool>    *El_trigMatch_2e17_lhvloose_nod0,
        vector<bool>    *El_trigMatch_e17_lhloose_mu14,
        vector<bool>    *El_trigMatch_e17_lhloose_nod0_mu14,
        vector<bool>    *El_TrigMatch_e24_lhmedium_nod0_ivarloose,
        vector<bool>    *El_TrigMatch_e24_lhtight_nod0_ivarloose,
        vector<bool>    *El_TrigMatch_e60_lhmedium_nod0);

    void fill_muons(
        Int_t           NMu,
        int             flavor,
        vector<double>  *Mu_eta,
        vector<double>  *Mu_phi,
        vector<double>  *Mu_pT,
        vector<double>  *Mu_SFw,
        vector<float>   *Mu_IsoSFw,
        vector<int>     *Mu_charge,
        vector<double>  *Mu_d0pvtx,
        vector<double>  *Mu_sigd0,
        vector<double>  *Mu_z0sinTheta,
        vector<bool>    *Mu_isBad,
        vector<bool>    *Mu_passOR,
        vector<bool>    *Mu_isTight,
        vector<bool>    *Mu_isCosmic,
/*
        vector<int>     *Mu_type, // MC only
        vector<int>     *Mu_origin, // MC only
*/
        vector<double>  *Mu_ptcone20,
        vector<double>  *Mu_ptcone30,
        vector<double>  *Mu_ptcone40,
        vector<double>  *Mu_ptvarcone20,
        vector<double>  *Mu_ptvarcone30,
        vector<double>  *Mu_ptvarcone40,
        vector<double>  *Mu_topoetcone20,
        vector<double>  *Mu_topoetcone30,
        vector<double>  *Mu_topoetcone40,
        vector<bool>    *Mu_passIsoLooseTO,
        vector<bool>    *Mu_passIsoLoose,
        vector<bool>    *Mu_passIsoTight,
        vector<bool>    *Mu_passIsoGrad,
        vector<bool>    *Mu_passIsoGradCustomTight,
        vector<bool>    *Mu_passIsoGradCustom,
        vector<bool>    *Mu_passIsoGradLoose,
        Float_t         MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50,
        vector<bool>    *Mu_trigMatch_mu26_imedium,
        vector<bool>    *Mu_trigMatch_mu50,
        vector<bool>    *Mu_trigMatch_mu8noL1,
        vector<bool>    *Mu_trigMatch_mu14,
        vector<bool>    *Mu_trigMatch_mu18,
        vector<bool>    *Mu_trigMatch_mu18_mu8noL1,
        vector<bool>    *Mu_trigMatch_e17_lhloose_mu14,
        vector<bool>    *Mu_trigMatch_e17_lhloose_nod0_mu14,
        vector<bool>    *Mu_trigMatch_mu20_mu8noL1,
        vector<bool>    *Mu_trigMatch_mu22_mu8noL1,
        vector<bool>    *Mu_TrigMatch_mu24_iloose,
        vector<bool>    *Mu_TrigMatch_mu24_ivarloose,
        vector<bool>    *Mu_TrigMatch_mu24_iloose_L1MU15,
        vector<bool>    *Mu_TrigMatch_mu24_ivarloose_L1MU15,
        vector<vector<bool> > *Mu_trigMatchPair_mu18_mu8noL1,
        vector<vector<bool> > *Mu_trigMatchPair_mu20_mu8noL1,
        vector<vector<bool> > *Mu_trigMatchPair_mu22_mu8noL1);

    void fill_jets(
        Int_t           NJet,
        vector<double>  *Jet_eta,
        vector<double>  *Jet_phi,
        vector<double>  *Jet_pT,
        vector<double>  *Jet_E,
        vector<double>  *Jet_quality,
        vector<double>  *Jet_JVT,
        vector<double>  *Jet_JVTsf,
        vector<double>  *Jet_MV2c20,
        vector<double>  *Jet_MV2c10,
        vector<double>  *Jet_SFw,
/*
        vector<int>     *Jet_ConeTruthLabel, // MC only
        vector<int>     *Jet_PartonTruthLabel, // MC only
        vector<int>     *Jet_HadronConeExclTruthLabel, // MC only
        vector<double>  *Jet_deltaR, // MC only
*/
        vector<int>     *Jet_nTrk,
        vector<bool>    *Jet_passOR);
    //#endif // #ifdef _IS_DATA_

    void fill_leptons(vector<Electron> elec, vector<Muon> muon);

    void fill_baseline_electrons(vector<Electron> elec);
    void fill_baseline_muons(vector<Muon> muon);
    void fill_baseline_leptons(vector<Electron> elec, vector<Muon> muon);
    void fill_baseline_jets(vector<Jet> jets);

    void fill_OR_electrons(vector<Electron> elec);
    void fill_OR_muons(vector<Muon> muon);
    void fill_OR_leptons(vector<Electron> elec, vector<Muon> muon);
    void fill_OR_jets(vector<Jet> jets);

    void fill_JVT_jets(vector<Jet> jets);

    void fill_signal_electrons(vector<Electron> elec);
    void fill_signal_muons(vector<Muon> muon);
    void fill_signal_leptons(vector<Electron> elec, vector<Muon> muon);
    void fill_signal_jets(vector<Jet> jets);

    void set_baseline_and_signal_electrons();
    void set_baseline_and_signal_muons();
    void set_jets_and_bjets();

    //template<typename T>
    //void debug_print(vector<T> vec);
    void debug_elec_print(vector<Electron> elec);
    void debug_muon_print(vector<Muon> muon);
    void debug_lept_print(vector<Lepton> lept);
    void debug_jets_print(vector<Jet> jets);

    void set_isMC(bool b) { isMC = b; }
    void set_isData(bool b) { isData = b; }
    void set_isSkim(bool b) { isSkim = b; }
    void set_isFullSim(bool b) { isFullSim = b; }
    void set_isAF2Sim(bool b) { isAF2Sim = b; }

    void set_process(string s) { process = s; }
    void set_luminosity(double d) { luminosity = d; }
/*
    void set_cross_section(double d) { cross_section = d; }
    void set_k_factor(double d) { k_factor = d; }
    void set_filter_efficiency(double d) { filter_efficiency = d; }
    void set_derivation_stat_weights(double d) { derivation_stat_weights = d; }

    // Weight related methods
    double ID_weight(vector<Electron> elec, bool signal = true);
    double ID_weight(vector<Muon> muon);
    double Iso_weight(vector<Electron> elec);
    double Iso_weight(vector<Muon> muon);
    double lept_weight(double elec_ID_weight, double elec_iso_weight, double muon_ID_weight, double muon_iso_weight);
    double jets_weight(vector<Jet> jets);
    double calculate_weight(double event_weight, double PRW_weight, double lept_weight, double jets_weight);
    double calculate_weight(double event_weight, double PRW_weight, vector<Electron> elec, vector<Muon> muon, vector<Jet> jets);
    void update(int cut, bool passed, double weight);
    void debug_sum_of_weight_print();
*/
    // plots
    void make_plots_Nvtx();
    void make_plots_AvgMu();
    void make_plots_NLepts();
    void make_plots_NJets();

    // this is needed to distribute the algorithm to the workers
    ClassDef(ytEventSelection, 1);
};

#endif
