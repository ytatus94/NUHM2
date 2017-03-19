#define YT_OPTIMIZATION_CXX
#include "ytNUHM2Analysis/yt_optimization.h"



const int yt_optimization::n_lept_cuts[5] = {2, 3, 4, 5, 6};
const int yt_optimization::n_bjet_cuts[6] = {0, 1, 2, 3, 4, 5};
const int yt_optimization::n_jets_cuts[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int yt_optimization::bjet_pt_cuts[9] = {20, 25, 30, 35, 40, 50, 70, 100, 150};
const int yt_optimization::jets_pt_cuts[9] = {20, 25, 30, 35, 40, 50, 70, 100, 150};
const int yt_optimization::met_cuts[10] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 500};
const int yt_optimization::meff_cuts[21] = {
    0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 
    1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000
};



yt_optimization::yt_optimization()
{
    isMC = false;
    isData = false;

    process = "";
    
    derivation_stat_weights = 0.;
    
    cross_section = 1.;
    k_factor = 1.;
    filter_efficiency = 1.;
    cross_section_kfactor_efficiency = 1.;

    event_weight = 1.;
    lepton_weight = 1.;
    jet_weight = 1.;
    pileup_weight = 1.;

    // leafs in tree
    // For distributions
    met = 0;
    Ht = 0;
    meff = 0;
    lepton1_pT = 0;
    lepton2_pT = 0;
    jet1_pT = 0;
    jet2_pT = 0;
    // Optimization cut bins
    n_leptons_bin = 0;
    n_jets_bin = 0;
    n_bjets_bin = 0;
    jet_pt_bin = 0;
    bjet_pt_bin = 0;
    met_bin = 0;
    meff_bin = 0;
    events_survived = 0;
    events_survived_weighted = 0;
}



yt_optimization::~yt_optimization()
{
    // Do nothing at here
}



void yt_optimization::initialize()
{
    // Declare the output
    string output_path = string(getenv("ROOTCOREBIN"))  + "/../";
    string filename;
    if (isMC)
        filename = output_path + "optimization_MC_" + process + ".root";
    else if (isData)
        filename = output_path +"optimization_Data_" + process + ".root";

    output_file = TFile::Open(filename.c_str(), "RECREATE");
    output_tree = new TTree("output_tree", "Optimization output tree");

    // Building the new branches
    output_tree->Branch("met",          &met);
    output_tree->Branch("Ht",           &Ht);
    output_tree->Branch("meff",         &meff);
    output_tree->Branch("lepton1_pT",   &lepton1_pT);
    output_tree->Branch("lepton2_pT",   &lepton2_pT);
    output_tree->Branch("jet1_pT",      &jet1_pT);
    output_tree->Branch("jet2_pT",      &jet2_pT);
/*
    output_tree->Branch("n_leptons_bin",    &n_leptons_bin);
    output_tree->Branch("n_jets_bin",       &n_jets_bin);
    output_tree->Branch("n_bjets_bin",      &n_bjets_bin);
    output_tree->Branch("jet_pt_bin",       &jet_pt_bin);
    output_tree->Branch("bjet_pt_bin",      &bjet_pt_bin);
    output_tree->Branch("met_bin",          &met_bin);
    output_tree->Branch("meff_bin",         &meff_bin);
    output_tree->Branch("event_survived",   &event_survived);
*/
    h_derivation_stat_weights           = new TH1F("h_derivation_stat_weights", "DerivationStat Weight", 2, 0, 2);
    h_cross_section                     = new TH1F("h_cross_section", "Cross section", 2, 0, 2);
    h_k_factor                          = new TH1F("h_k_factor", "k-factor", 2, 0, 2);
    h_filter_efficiency                 = new TH1F("h_filter_efficiency", "Filter efficiency", 2, 0, 2);
    h_cross_section_kfactor_efficiency  = new TH1F("h_cross_section_kfactor_efficiency", "Cross section#times k-factor#times Filter efficiency", 2, 0, 2);

    h_derivation_stat_weights->Sumw2();
    h_cross_section->Sumw2();
    h_k_factor->Sumw2();
    h_filter_efficiency->Sumw2();
    h_cross_section_kfactor_efficiency->Sumw2();

    h_met       = new TH1F("h_met", "MET;MET [GeV];Events", 100, 0, 1000);
    h_Ht        = new TH1F("h_Ht", "Ht;Ht [GeV];Events", 300, 0, 3000);
    h_meff      = new TH1F("h_meff", "Meff;M_{eff} [GeV];Events", 300, 0, 3000);
    h_NLeptons  = new TH1F("h_NLeptons", "NLeptons;N_{leptons};Events", 10, 0, 10);
    h_NJets     = new TH1F("h_NJets", "NJets;N_{jets};Events", 20, 0, 20);
    h_Nbjets    = new TH1F("h_Nbjets", "Nbjets;N_{b-jets};Events", 20, 0, 20);
    h_lep1_pT   = new TH1F("h_lep1_pT", "Lep1_pT;lepton 1 p_{T} [GeV];Entries", 200, 0, 2000);
    h_lep2_pT   = new TH1F("h_lep2_pT", "Lep2_pT;lepton 2 p_{T} [GeV];Entries", 200, 0, 2000);
    h_jet1_pT   = new TH1F("h_jet1_pT", "Jet1_pT;lepton 1 p_{T} [GeV];Entries", 200, 0, 2000);
    h_jet2_pT   = new TH1F("h_jet2_pT", "Jet2_pT;lepton 2 p_{T} [GeV];Entries", 200, 0, 2000);

    h_met->Sumw2();
    h_Ht->Sumw2();
    h_meff->Sumw2();
    h_NLeptons->Sumw2();
    h_NJets->Sumw2();
    h_Nbjets->Sumw2();
    h_lep1_pT->Sumw2();
    h_lep2_pT->Sumw2();
    h_jet1_pT->Sumw2();
    h_jet2_pT->Sumw2();

    // calculate total bins
    int nbins = sizeof(n_lept_cuts) / sizeof(n_lept_cuts[0]) *
                sizeof(n_bjet_cuts) / sizeof(n_bjet_cuts[0]) *
                sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0]) *
                sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0]) *
                sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0]) *
                sizeof(met_cuts) / sizeof(met_cuts[0]) *
                sizeof(meff_cuts) / sizeof(meff_cuts[0]);

    h_yields = new TH1F("h_yields", "yields", nbins, 0, nbins+1);
    h_yields_weighted = new TH1F("h_yields_weighted", "weighted yields", nbins, 0, nbins+1);

    h_yields->Sumw2();
    h_yields_weighted->Sumw2();
}



void yt_optimization::execute(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets)
{
    this->reset_vectors();
    this->copy_vectors(elec, muon, lept, jets);
    this->fill_signal_bjets(jets);

    // Set bin contents
    //h_derivation_stat_weights->SetBinContent(1, derivation_stat_weights);
    //cout << "derivation_stat_weights=" << derivation_stat_weights << endl;
    h_derivation_stat_weights->Fill(1, derivation_stat_weights);
    h_cross_section->SetBinContent(1, cross_section);
    h_k_factor->SetBinContent(1, k_factor);
    h_filter_efficiency->SetBinContent(1, filter_efficiency);
    h_cross_section_kfactor_efficiency->SetBinContent(1, cross_section_kfactor_efficiency);

    // Fill the histogram for distributions after pre-selection
    //double weight = event_weight * lepton_weight * jet_weight * pileup_weight;
    double weight = luminosity * cross_section_kfactor_efficiency * 1000. * event_weight * pileup_weight / derivation_stat_weights;

    Ht = calculate_Ht(vec_signal_lept, vec_signal_jets);
    meff = calculate_Meff(Ht, met);

    h_met->Fill(met / 1000., weight);
    h_Ht->Fill(Ht / 1000., weight);
    h_meff->Fill(meff / 1000., weight);

    if (vec_signal_lept.size() > 0) {
        h_NLeptons->Fill(vec_signal_lept.at(0).get_number(), weight);
        h_lep1_pT->Fill(vec_signal_lept.at(0).get_pt(), weight);
        if (vec_signal_lept.size() > 1)
            h_lep2_pT->Fill(vec_signal_lept.at(1).get_pt(), weight);
    }

    if (vec_signal_jets.size() > 0) {
        h_NJets->Fill(vec_signal_jets.at(0).get_number(), weight);
        h_jet1_pT->Fill(vec_signal_jets.at(0).get_pt(), weight);
        if (vec_signal_jets.size() > 1)
            h_jet2_pT->Fill(vec_signal_jets.at(1).get_pt(), weight);
    }

    if (vec_signal_bjet.size() > 0)
    h_Nbjets->Fill(vec_signal_bjet.at(0).get_number(), weight);

    // Number of lepton requirement
    for (unsigned int i_lept = 0; i_lept < sizeof(n_lept_cuts) / sizeof(n_lept_cuts[0]); i_lept++) {
        //cout << "n_lept_cuts=" << n_lept_cuts[i_lept] << endl;
        if (static_cast<int>(vec_signal_lept.size()) < n_lept_cuts[i_lept])
            continue;
        // b-jet pT requirement
        for (unsigned int i_bjet_pt = 0; i_bjet_pt < sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0]); i_bjet_pt++) {
            //cout << "*bjet_pt_cuts=" << bjet_pt_cuts[i_bjet_pt] << endl;
            int nbjets = 0;
            for (auto & bjet_itr : vec_signal_bjet) {
                if (bjet_itr.get_pt() / 1000. >= bjet_pt_cuts[i_bjet_pt]) // in GeV
                    nbjets++;
            }
            // Number of b-jet requirement
            for (unsigned int i_bjet = 0; i_bjet < sizeof(n_bjet_cuts) / sizeof(n_bjet_cuts[0]); i_bjet++) {
                //cout << "**n_bjet_cuts=" << n_bjet_cuts[i_bjet] << endl;
                if (n_bjet_cuts[i_bjet] > 0 &&
                    nbjets < n_bjet_cuts[i_bjet])
                    continue;
                else if (n_bjet_cuts[i_bjet] == 0 &&
                         nbjets > 0)
                    continue;
                // jet pT requirement
                for (unsigned int i_jets_pt = 0; i_jets_pt < sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0]); i_jets_pt++) {
                    //cout << "***jets_pt_cuts=" << jets_pt_cuts[i_jets_pt] << endl;
                    int njets = 0;
                    for (auto & jet_itr : vec_signal_jets) {
                        if (jet_itr.get_pt() / 1000. >= jets_pt_cuts[i_jets_pt]) // in GeV
                            njets++;
                    }
                    // Number of jet requirement
                    for (unsigned int i_jets = 0; i_jets < sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0]); i_jets++) {
                        //cout << "****n_jets_cuts=" << n_jets_cuts[i_jets] << endl;
                        if (njets < n_jets_cuts[i_jets])
                            continue;
                        // MET requirement
                        for (unsigned int i_met = 0; i_met < sizeof(met_cuts) / sizeof(met_cuts[0]); i_met++) {
                            //cout << "*****met_cuts=" << met_cuts[i_met] << endl;
                            if (met / 1000. <= met_cuts[i_met]) // in GeV
                                continue;
                            // Meff requirement
                            for (unsigned int i_meff = 0; i_meff < sizeof(meff_cuts) / sizeof(meff_cuts[0]); i_meff++) {
                                //cout << "******meff_cuts=" << meff_cuts[i_meff] << endl;
                                int bin = 1 + i_meff;
                                bin += i_met * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_jets * sizeof(met_cuts) / sizeof(met_cuts[0])
                                              * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_jets_pt * sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0])
                                                 * sizeof(met_cuts) / sizeof(met_cuts[0])
                                                 * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_bjet * sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0])
                                              * sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0])
                                              * sizeof(met_cuts) / sizeof(met_cuts[0])
                                              * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_bjet_pt * sizeof(n_bjet_cuts) / sizeof(n_bjet_cuts[0])
                                                 * sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0])
                                                 * sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0])
                                                 * sizeof(met_cuts) / sizeof(met_cuts[0])
                                                 * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_lept * sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0])
                                              * sizeof(n_bjet_cuts) / sizeof(n_bjet_cuts[0])
                                              * sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0])
                                              * sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0])
                                              * sizeof(met_cuts) / sizeof(met_cuts[0])
                                              * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                if (meff / 1000. <= meff_cuts[i_meff]) // in GeV
                                    continue;
                                else {
                                    h_yields->AddBinContent(bin);
                                    h_yields_weighted->AddBinContent(bin, weight);
                                }
                                //cout << "bin=" << bin << endl;
                            }
                        }
                    }
                }
            }
        }
    }

    // fill all new branches
    output_tree->Fill();
}



void yt_optimization::finalize()
{

    cout << "Write into file" << endl;
    output_file->cd();
    output_file->Write();
/*
    output_tree->Write();

    h_met->Write();
    h_Ht->Write();
    h_meff->Write();
    h_NLeptons->Write();
    h_NJets->Write();
    h_Nbjets->Write();
    h_lep1_pT->Write();
    h_lep2_pT->Write();
    h_jet1_pT->Write();
    h_jet2_pT->Write();
*/
    output_file->Close();
}



void yt_optimization::reset_vectors()
{
    // clear all vectors
    vec_signal_elec.clear();
    vec_signal_muon.clear();
    vec_signal_lept.clear();
    vec_signal_jets.clear();
    vec_signal_bjet.clear();
}



void yt_optimization::copy_vectors(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets)
{
    vec_signal_elec = elec;
    vec_signal_muon = muon;
    vec_signal_lept = lept;
    vec_signal_jets = jets;
}



void yt_optimization::fill_signal_bjets(vector<Jet> signal_jets)
{
    for (auto & signal_jets_itr : signal_jets) {
        if (signal_jets_itr.get_isBjet() == true)
            vec_signal_bjet.push_back(signal_jets_itr); 
    }
}



void yt_optimization::apply_signal_region_cuts(int cut_n_leptons,
                                               int cut_bjets_pT, int cut_n_bjets,
                                               int cut_jet_pt, int cut_n_jet,
                                               int cut_met, int cut_meff,
                                               float weight)
{
    int nlepts = vec_signal_lept.size();
    // Count the number of b-jets with pT > b-jet pT cut
    int nbjets = 0;
    for (auto & bjet_itr : vec_signal_bjet) {
        if (bjet_itr.get_pt() >= cut_bjets_pT)
            nbjets++;
    }
    // Count the number of jets with pT > jet pT cut
    int njets = 0;
    for (auto & jet_itr : vec_signal_jets) {
        if (jet_itr.get_pt() >= cut_jet_pt)
            njets++;
    }

    if (nlepts >= cut_n_leptons &&
        nbjets >= cut_n_bjets &&
        njets >= cut_n_jet &&
        met > cut_met &&
        meff > cut_meff) {
        events_survived++;
        events_survived_weighted = events_survived_weighted + (1 * weight);
    }
}



void yt_optimization::debug_print()
{
    cout << "***In yt_optimization***" << endl;
    cout << "event_weight=" << event_weight << endl;
    cout << "lepton_weight=" << lepton_weight << endl;
    cout << "jet_weight=" << jet_weight << endl;
    cout << "pileup_weight=" << pileup_weight << endl;
    cout << "derivation_stat_weights=" << derivation_stat_weights << endl;
    cout << "cross_section=" << cross_section << endl;
    cout << "k_factor=" << k_factor << endl;
    cout << "filter_efficiency=" << filter_efficiency << endl;
    cout << "cross_section_kfactor_efficiency=" << cross_section_kfactor_efficiency << endl;
    cout << "met=" << met << endl;
}
