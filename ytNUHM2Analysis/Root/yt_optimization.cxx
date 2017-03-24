#define YT_OPTIMIZATION_CXX
#include "ytNUHM2Analysis/yt_optimization.h"



const float yt_optimization::N_lept_cuts[5] = {2, 3, 4, 5, 6};
const float yt_optimization::N_bjet_cuts[6] = {0, 1, 2, 3, 4, 5};
const float yt_optimization::N_jets_cuts[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const float yt_optimization::bjet_pt_cuts[9] = {20., 25., 30., 35., 40., 50., 70., 100., 150.};
const float yt_optimization::jets_pt_cuts[9] = {20., 25., 30., 35., 40., 50., 70., 100., 150.};
// const float yt_optimization::met_cuts[10] = {0., 50., 100., 150., 200., 250., 300., 350., 400., 500.};
// const float yt_optimization::meff_cuts[21] = {
//     0., 100., 200., 300., 400., 500., 600., 700., 800., 900., 1000.,
//     1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.
// };
const float yt_optimization::met_cuts[12] = {50., 75., 100., 125., 150., 175., 200., 250., 300., 350., 400., 500.};
const float yt_optimization::meff_cuts[12] = {0., 500., 700., 900., 1100., 1300., 1500., 1600., 1700., 1800., 1900., 2100.};


#define N_lepts 3
#define N_bjets 0
#define Bjet_pT 0
#define N_jets 4



yt_optimization::yt_optimization()
{
    // cout << "Call yt_optimization::yt_optimization()" << endl;

    isMC = false;
    isData = false;

    process = "";

    run_number = 0;
    event_number = 0;

    derivation_stat_weights = 0.;

    luminosity = 0;

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
    Ht = 0;
    lepton1_pT = 0;
    lepton2_pT = 0;
    jet1_pT = 0;
    jet2_pT = 0;

    n_leptons = 0.;
    n_bjets = 0.;
    bjet_pT.clear();
    n_jets = 0.;
    jet_pT.clear();
    met = 0.;
    meff = 0.;
    met_over_meff = 0.;

    n_bjet_pTX = 0;
    n_jets_pTX = 0;

    // Optimization cut bins
    n_N_lept_cuts_bins = 0;
    n_N_bjet_cuts_bins = 0;
    n_N_jets_cuts_bins = 0;
    n_bjet_pt_cuts_bins = 0;
    n_jets_pt_cuts_bins = 0;
    n_met_cuts_bins = 0;
    n_meff_cuts_bins = 0;

    events_survived = 0;
    events_survived_weighted = 0;
}



yt_optimization::~yt_optimization()
{
    // Do nothing at here
}



void yt_optimization::initialize()
{
    // cout << "Call yt_optimization::initialize()" << endl;

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
    output_tree->Branch("Ht",           &Ht);
    output_tree->Branch("lepton1_pT",   &lepton1_pT);
    output_tree->Branch("lepton2_pT",   &lepton2_pT);
    output_tree->Branch("jet1_pT",      &jet1_pT);
    output_tree->Branch("jet2_pT",      &jet2_pT);

    output_tree->Branch("n_leptons",    &n_leptons);
    output_tree->Branch("n_bjets",      &n_bjets);
    output_tree->Branch("bjet_pT",      &bjet_pT);
    output_tree->Branch("n_jets",       &n_jets);
    output_tree->Branch("jet_pT",       &jet_pT);
    output_tree->Branch("met",          &met);
    output_tree->Branch("meff",         &meff);
    output_tree->Branch("met_over_meff",&met_over_meff);

    output_tree->Branch("n_bjet_pTX",   &n_bjet_pTX);
    output_tree->Branch("n_jets_pTX",   &n_jets_pTX);

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

    h_derivation_stat_weights->SetBinContent(1, derivation_stat_weights);

    h_met       = new TH1F("h_met", "MET;MET [GeV];Events", 100, 0, 1000);
    h_Ht        = new TH1F("h_Ht", "Ht;Ht [GeV];Events", 300, 0, 3000);
    h_meff      = new TH1F("h_meff", "Meff;M_{eff} [GeV];Events", 300, 0, 3000);
    h_met_over_meff = new TH1F("h_met_over_meff", "MET/M_{eff};MET/M_{eff};Events", 100, 0, 1);
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
    int nbins = sizeof(N_lept_cuts) / sizeof(N_lept_cuts[0]) *
                sizeof(N_bjet_cuts) / sizeof(N_bjet_cuts[0]) *
                sizeof(N_jets_cuts) / sizeof(N_jets_cuts[0]) *
                sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0]) *
                sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0]) *
                sizeof(met_cuts) / sizeof(met_cuts[0]) *
                sizeof(meff_cuts) / sizeof(meff_cuts[0]);

    h_yields = new TH1F("h_yields", "yields", nbins, 0, nbins+1);
    h_yields_weighted = new TH1F("h_yields_weighted", "weighted yields", nbins, 0, nbins+1);

    h_yields->Sumw2();
    h_yields_weighted->Sumw2();

    this->set_binning_default();
    n_N_lept_cuts_bins = m_N_lept_cuts_bins.size() - 1;
    n_N_bjet_cuts_bins = m_N_bjet_cuts_bins.size() - 1;
    n_N_jets_cuts_bins = m_N_jets_cuts_bins.size() - 1;
    n_bjet_pt_cuts_bins = m_bjet_pt_cuts_bins.size() - 1;
    n_jets_pt_cuts_bins = m_jets_pt_cuts_bins.size() - 1;
    n_met_cuts_bins = m_met_cuts_bins.size() - 1;
    n_meff_cuts_bins = m_meff_cuts_bins.size() - 1;

    h_method2_yields = new TH3F("h_method2_yields", "yields;p_{T} [GeV];E_{T}^{miss} [GeV]; M_{eff} [GeV]",
                                n_jets_pt_cuts_bins, &m_jets_pt_cuts_bins[0],
                                n_met_cuts_bins, &m_met_cuts_bins[0],
                                n_meff_cuts_bins, &m_meff_cuts_bins[0]);
    h_method2_yields_weighted = new TH3F("h_method2_yields_weighted", "weighted yields;p_{T} [GeV];E_{T}^{miss} [GeV]; M_{eff} [GeV]",
                                n_jets_pt_cuts_bins, &m_jets_pt_cuts_bins[0],
                                n_met_cuts_bins, &m_met_cuts_bins[0],
                                n_meff_cuts_bins, &m_meff_cuts_bins[0]);
    h_method2_yields->Sumw2();
    h_method2_yields_weighted->Sumw2();

    h_events_survived = new TH1F("h_events_survived", "events_survived", 2, 0, 2);
    h_events_survived_weighted = new TH1F("h_events_survived_weighted", "events_survived_weighted", 2, 0, 2);
}



void yt_optimization::execute(vector<Electron> elec, vector<Muon> muon, vector<Lepton> lept, vector<Jet> jets)
{
    // cout << "Call yt_optimization::execute()" << endl;

    this->reset_vectors();
    this->copy_vectors(elec, muon, lept, jets);
    this->fill_signal_bjets(jets);

    // Set bin contents
    h_derivation_stat_weights->Fill(1, derivation_stat_weights);
    h_cross_section->SetBinContent(1, cross_section);
    h_k_factor->SetBinContent(1, k_factor);
    h_filter_efficiency->SetBinContent(1, filter_efficiency);
    h_cross_section_kfactor_efficiency->SetBinContent(1, cross_section_kfactor_efficiency);

    // Fill the histogram for distributions after pre-selection
    // double weight = event_weight * lepton_weight * jet_weight * pileup_weight;
    weight = luminosity * cross_section_kfactor_efficiency * 1000. * event_weight * lepton_weight * jet_weight * pileup_weight / derivation_stat_weights;
    this->debug_print("weight");

    Ht = calculate_Ht(vec_signal_lept, vec_signal_jets);
    meff = calculate_Meff(Ht, met);

    h_met->Fill(met / 1000., weight);
    h_Ht->Fill(Ht / 1000., weight);
    h_meff->Fill(meff / 1000., weight);

    if (vec_signal_lept.size() > 0) {
        h_NLeptons->Fill(vec_signal_lept.at(0).get_number(), weight);
        h_lep1_pT->Fill(vec_signal_lept.at(0).get_pt(), weight);
        lepton1_pT = vec_signal_lept.at(0).get_pt() / 1000.;
        if (vec_signal_lept.size() > 1) {
            h_lep2_pT->Fill(vec_signal_lept.at(1).get_pt(), weight);
            lepton2_pT = vec_signal_lept.at(1).get_pt() / 1000.;
        }
    }

    if (vec_signal_jets.size() > 0) {
        h_NJets->Fill(vec_signal_jets.at(0).get_number(), weight);
        h_jet1_pT->Fill(vec_signal_jets.at(0).get_pt(), weight);
        jet1_pT = vec_signal_jets.at(0).get_pt() / 1000.;
        if (vec_signal_jets.size() > 1) {
            h_jet2_pT->Fill(vec_signal_jets.at(1).get_pt(), weight);
            jet2_pT = vec_signal_jets.at(1).get_pt() / 1000.;
        }
    }

    if (vec_signal_bjet.size() > 0)
        h_Nbjets->Fill(vec_signal_bjet.at(0).get_number(), weight);

    // Get values for leafs
    n_leptons = vec_signal_lept.size();
    n_bjets = vec_signal_bjet.size();
    for (auto & bjet_itr : vec_signal_bjet) {
        bjet_pT.push_back(bjet_itr.get_pt() / 1000.);
    }
    n_jets = vec_signal_jets.size();
    for (auto & jet_itr : vec_signal_jets) {
        jet_pT.push_back(jet_itr.get_pt() / 1000.);
    }
    // met is a data member
    // meff calculation is at above
    met_over_meff = met / meff;

    for (auto & jet_itr : vec_signal_bjet) {
        if (jet_itr.get_pt() / 1000. > 20)
            n_bjet_pTX++;
    }

    for (auto & jet_itr : vec_signal_jets) {
        if (jet_itr.get_pt() / 1000. > 25)
            n_jets_pTX++;
    }

/*
    // Method 1
    // Number of lepton requirement
    for (unsigned int i_lept = 0; i_lept < sizeof(N_lept_cuts) / sizeof(N_lept_cuts[0]); i_lept++) {
        //cout << "N_lept_cuts=" << N_lept_cuts[i_lept] << endl;
        if (static_cast<int>(vec_signal_lept.size()) < N_lept_cuts[i_lept])
            continue;
        // b-jet pT requirement
        for (unsigned int i_bjet_pt = 0; i_bjet_pt < sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0]); i_bjet_pt++) {
            //cout << "*bjet_pt_cuts=" << bjet_pt_cuts[i_bjet_pt] << endl;
            int nbjets = 0;
            for (auto & bjet_itr : vec_signal_bjet) {
                if (bjet_itr.get_pt() / 1000. > bjet_pt_cuts[i_bjet_pt]) // in GeV
                    nbjets++;
            }
            // Number of b-jet requirement
            for (unsigned int i_bjet = 0; i_bjet < sizeof(N_bjet_cuts) / sizeof(N_bjet_cuts[0]); i_bjet++) {
                //cout << "**N_bjet_cuts=" << N_bjet_cuts[i_bjet] << endl;
                if (N_bjet_cuts[i_bjet] > 0 &&
                    nbjets < N_bjet_cuts[i_bjet])
                    continue;
                else if (N_bjet_cuts[i_bjet] == 0 &&
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
                    for (unsigned int i_jets = 0; i_jets < sizeof(N_jets_cuts) / sizeof(N_jets_cuts[0]); i_jets++) {
                        //cout << "****N_jets_cuts=" << N_jets_cuts[i_jets] << endl;
                        if (njets < N_jets_cuts[i_jets])
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
                                bin += i_jets_pt * sizeof(N_jets_cuts) / sizeof(N_jets_cuts[0])
                                                 * sizeof(met_cuts) / sizeof(met_cuts[0])
                                                 * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_bjet * sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0])
                                              * sizeof(N_jets_cuts) / sizeof(N_jets_cuts[0])
                                              * sizeof(met_cuts) / sizeof(met_cuts[0])
                                              * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_bjet_pt * sizeof(N_bjet_cuts) / sizeof(N_bjet_cuts[0])
                                                 * sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0])
                                                 * sizeof(N_jets_cuts) / sizeof(N_jets_cuts[0])
                                                 * sizeof(met_cuts) / sizeof(met_cuts[0])
                                                 * sizeof(meff_cuts) / sizeof(meff_cuts[0]);
                                bin += i_lept * sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0])
                                              * sizeof(N_bjet_cuts) / sizeof(N_bjet_cuts[0])
                                              * sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0])
                                              * sizeof(N_jets_cuts) / sizeof(N_jets_cuts[0])
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
*/
    // Method 2
    bool n_lept_cut_flag = false,
         n_bjet_cut_flag = false;

    // Number of leptons requirement
    if (vec_signal_lept.size() >= N_lepts)
        n_lept_cut_flag = true;

    // Number of b-jets requirement with pT greater than some values
    if (N_bjets == 0) {
        if (vec_signal_bjet.size() == 0)
            n_bjet_cut_flag =  true;
    }
    else { // N_bjets > 0
        this->fill_vec_jets_with_pT_cut("bjets", Bjet_pT);
        if (vec_signal_bjet_with_pt_cut.size() >= N_bjets)
            n_bjet_cut_flag =  true;
    }

    if (n_lept_cut_flag &&
        n_bjet_cut_flag) {
        // Number of jets requirement with pT greater than some values
        for (unsigned int i_jets_pt = 0; i_jets_pt < sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0]); i_jets_pt++) {
            bool n_jets_cut_flag = false;
            this->fill_vec_jets_with_pT_cut("jets", jets_pt_cuts[i_jets_pt]);
            if (vec_signal_jets_with_pt_cut.size() >= N_jets)
                n_jets_cut_flag = true;
            if (n_jets_cut_flag) {
                for (unsigned int i_met = 0; i_met < sizeof(met_cuts) / sizeof(met_cuts[0]); i_met++) {
                    bool met_cut_flag = false;
                    if (met / 1000. > met_cuts[i_met])
                        met_cut_flag = true;
                    if (met_cut_flag) {
                        for (unsigned int i_meff = 0; i_meff < sizeof(meff_cuts) / sizeof(meff_cuts[0]); i_meff++) {
                            bool meff_cut_flag = false;
                            if (meff / 1000. > meff_cuts[i_meff])
                                meff_cut_flag = true;
                            if (meff_cut_flag) {
                                cout << "pass n_lept >= " << N_lepts
                                    << ", n_bjet >= " << N_bjets << " (with b-jets pT >" << Bjet_pT << " GeV)"
                                    << ", n_jets >=" << N_jets << " (with jets pT >" << jets_pt_cuts[i_jets_pt] << " GeV)"
                                    << ", met > " << met_cuts[i_met] << " GeV"
                                    << ", meff > " << meff_cuts[i_meff] << " GeV"
                                    << endl;
                                float old_yields = h_method2_yields->GetBinContent(i_jets_pt + 1,
                                                                                   i_met + 1,
                                                                                   i_meff + 1);
                                cout << "At bin=(" << i_jets_pt + 1 << ", " << i_met + 1 << ", " << i_meff + 1
                                    << "), old_yields=" << old_yields << endl;
                                h_method2_yields->SetBinContent(i_jets_pt + 1,
                                                                i_met + 1,
                                                                i_meff + 1,
                                                                old_yields + 1);
                                float old_weighted_yields = h_method2_yields_weighted->GetBinContent(i_jets_pt + 1,
                                                                                                     i_met + 1,
                                                                                                     i_meff + 1);
                                cout << "At bin=(" << i_jets_pt + 1 << ", " << i_met + 1 << ", " << i_meff + 1
                                    << "), old_weighted_yields=" << old_weighted_yields << endl;
                                h_method2_yields_weighted->SetBinContent(i_jets_pt + 1,
                                                                         i_met + 1,
                                                                         i_meff + 1,
                                                                         old_weighted_yields + 1 * weight);
                                cout << "weight=" << weight << endl;
                            }
                        }
                    }
                }
            }
        }
    }

    // Compare events
    this->apply_signal_region_cuts(2, 20, 1, 25, 6, 150, 600, 0.25, weight); // SR1b1
    // this->apply_signal_region_cuts(2, 20, 1, 25, 6, 250, 0, 0.2, weight); // SR1b2
    // this->apply_signal_region_cuts(2, 20, 3, 25, 6, 150, 0, 0.2, weight); // SR3b1
    // this->apply_signal_region_cuts(2, 20, 3, 25, 6, 250, 1200, 0., weight); // SR3b2

    // fill all new branches
    output_tree->Fill();
}



void yt_optimization::finalize()
{
    // cout << "Call yt_optimization::finalize()" << endl;

    // cout << "In yt_optimization::finalize(): events_survived=" << events_survived << ", events_survived_weighted=" << events_survived_weighted << endl;
    h_events_survived->SetBinContent(1, events_survived);
    h_events_survived_weighted->SetBinContent(1, events_survived_weighted);

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

    vec_signal_jets_with_pt_cut.clear();
    vec_signal_bjet_with_pt_cut.clear();
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



void yt_optimization::fill_vec_jets_with_pT_cut(string jet, int pt)
{
    if (jet != "bjets") {
        for (auto & jet_itr : vec_signal_jets) {
            if (jet_itr.get_pt() / 1000. > pt)
                vec_signal_jets_with_pt_cut.push_back(jet_itr);
        }
    }
    else { // b-jets
        for (auto & jet_itr : vec_signal_bjet) {
            if (jet_itr.get_pt() / 1000. > pt)
                vec_signal_bjet_with_pt_cut.push_back(jet_itr);
        }
    }
}



void yt_optimization::apply_signal_region_cuts(int cut_n_leptons,
                                               int cut_bjets_pT, int cut_n_bjets,
                                               int cut_jet_pt, int cut_n_jet,
                                               int cut_met, int cut_meff,
                                               float cut_met_over_meff,
                                               float weight)
{
    // cout << "In yt_optimization::apply_signal_region_cuts()"
    //     << ", events_survived=" << events_survived
    //     << ", events_survived_weighted=" << events_survived_weighted
    //     << ", weight=" << weight
    //     << endl;

    bool n_lept_cut_flag = false;
    if (static_cast<int> (vec_signal_lept.size()) >= cut_n_leptons)
        n_lept_cut_flag = true;

    bool n_bjet_cut_flag = false;
    if (cut_n_bjets == 0) {
        if (static_cast<int> (vec_signal_bjet.size()) == 0)
            n_bjet_cut_flag =  true;
    }
    else { // cut_n_bjets > 0
        this->fill_vec_jets_with_pT_cut("bjets", cut_bjets_pT);
        if (static_cast<int> (vec_signal_bjet_with_pt_cut.size()) >= cut_n_bjets)
            n_bjet_cut_flag =  true;
    }

    bool n_jets_cut_flag = false;
    this->fill_vec_jets_with_pT_cut("jets", cut_jet_pt);
    if (static_cast<int> (vec_signal_jets_with_pt_cut.size()) >= cut_n_jet)
        n_jets_cut_flag = true;

    bool met_cut_flag = false;
    if (met / 1000. > cut_met)
        met_cut_flag = true;

    bool meff_cut_flag = false;
    if (meff / 1000. > cut_meff)
        meff_cut_flag = true;

    bool met_over_meff_cut_flag = false;
    if (met_over_meff > cut_met_over_meff)
        met_over_meff_cut_flag = true;

    if (n_lept_cut_flag &&
        n_bjet_cut_flag &&
        n_jets_cut_flag &&
        met_cut_flag &&
        meff_cut_flag &&
        met_over_meff_cut_flag) {
        events_survived++;
        events_survived_weighted += weight;
    }
}



void yt_optimization::debug_print(string s)
{
    cout << "***In yt_optimization***" << endl;
    if (s == "weight") {
        cout << "run_number=" << run_number
            << ", event_number=" << event_number
            << ", weight=" << weight
            << ", luminosity=" << luminosity
            << ", cross_section=" << cross_section
            << ", k_factor=" << k_factor
            << ", filter_efficiency=" << filter_efficiency
            << ", cross_section_kfactor_efficiency=" << cross_section_kfactor_efficiency
            << ", event_weight=" << event_weight
            << ", lepton_weight=" << lepton_weight
            << ", jet_weight=" << jet_weight
            << ", pileup_weight=" << pileup_weight
            << ", derivation_stat_weights=" << derivation_stat_weights
            << endl;
    }
}



void yt_optimization::set_binning_default()
{
    m_N_lept_cuts_bins = vector<float> (N_lept_cuts, N_lept_cuts + sizeof(N_lept_cuts) / sizeof(float));
    m_N_bjet_cuts_bins = vector<float> (N_bjet_cuts, N_bjet_cuts + sizeof(N_bjet_cuts) / sizeof(float));
    m_N_jets_cuts_bins = vector<float> (N_jets_cuts, N_jets_cuts + sizeof(N_jets_cuts) / sizeof(float));
    m_bjet_pt_cuts_bins = vector<float> (bjet_pt_cuts, bjet_pt_cuts + sizeof(bjet_pt_cuts) / sizeof(float));
    m_jets_pt_cuts_bins = vector<float> (jets_pt_cuts, jets_pt_cuts + sizeof(jets_pt_cuts) / sizeof(float));
    m_met_cuts_bins = vector<float> (met_cuts, met_cuts + sizeof(met_cuts) / sizeof(float));
    m_meff_cuts_bins = vector<float> (meff_cuts, meff_cuts + sizeof(meff_cuts) / sizeof(float));
}
