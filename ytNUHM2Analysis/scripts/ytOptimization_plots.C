#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TString.h>
#include <TH1.h>
#include <RooStats/NumberCountingUtils.h>

#include <iostream>
#include <sstream>
using namespace std;

const int n_lept_cuts[5] = {2, 3, 4, 5, 6};
const int n_bjet_cuts[6] = {0, 1, 2, 3, 4, 5};
const int n_jets_cuts[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int bjet_pt_cuts[9] = {20, 25, 30, 35, 40, 50, 70, 100, 150};
const int jets_pt_cuts[9] = {20, 25, 30, 35, 40, 50, 70, 100, 150};
const double met_cuts[10] = {0., 50., 100., 150., 200., 250., 300., 350., 400., 500.};
const double meff_cuts[21] = {
    0., 100., 200., 300., 400., 500., 600., 700., 800., 900., 1000., 
    1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.
};

string path = "/raid05/users/shen/Ximo_ntuples/v44/NUHM2/Results/20170319/";

string signal_files[7] = {
    "optimization_MC_NUHM2_m12_300_strong.root",
    "optimization_MC_NUHM2_m12_350_strong.root",
    "optimization_MC_NUHM2_m12_400_strong.root",
    "optimization_MC_NUHM2_m12_500_strong.root",
    "optimization_MC_NUHM2_m12_600_strong.root",
    "optimization_MC_NUHM2_m12_700_strong.root",
    "optimization_MC_NUHM2_m12_800_strong.root"
};

#define Xsec_NUMH2_m12_300 0.6089026860
#define Xsec_NUMH2_m12_350 0.2512005769
#define Xsec_NUMH2_m12_400 0.1111911838
#define Xsec_NUMH2_m12_500 0.0257765957
#define Xsec_NUMH2_m12_600 0.0069446138
#define Xsec_NUMH2_m12_700 0.0020592510
#define Xsec_NUMH2_m12_800 0.0006553931

string background_files[48] = {
    "optimization_MC_Zee.root",
    "optimization_MC_Zmumu.root",
    "optimization_MC_Ztautau.root",
    "optimization_MC_ttbar.root",
    "optimization_MC_ttbarHT6c_1k_hdamp172p5.root",
    "optimization_MC_ttbarHT1k_1k5_hdamp172p5.root",
    "optimization_MC_ttbarHT1k5_hdamp172p5_no.root",
    "optimization_MC_ttW_Np0.root",
    "optimization_MC_ttW_Np1.root",
    "optimization_MC_ttW_Np2.root",
    "optimization_MC_ttee_Np0.root",
    "optimization_MC_ttee_Np1.root",
    "optimization_MC_ttmumu_Np0.root",
    "optimization_MC_ttmumu_Np1.root",
    "optimization_MC_tttautau_Np0.root",
    "optimization_MC_tttautau_Np1.root",
    "optimization_MC_ttbarWW.root",
    "optimization_MC_3top_SM.root",
    "optimization_MC_4topSM.root",
    "optimization_MC_singletop_tchan_lept_top.root",
    "optimization_MC_singletop_tchan_lept_antitop.root",
    "optimization_MC_Wminusenu.root",
    "optimization_MC_Wminusmunu.root",
    "optimization_MC_Wminustaunu.root",
    "optimization_MC_Wplusenu.root",
    "optimization_MC_Wplusmunu.root",
    "optimization_MC_Wplustaunu.root",
    "optimization_MC_Wt_inclusive_top.root",
    "optimization_MC_Wt_inclusive_antitop.root",
    "optimization_MC_Wt_dilepton_top.root",
    "optimization_MC_Wt_dilepton_antitop.root",
    "optimization_MC_ZZZ_6l0v.root",
    "optimization_MC_ZZZ_4l2v.root",
    "optimization_MC_ZZZ_2l4v.root",
    "optimization_MC_ZqqZll.root",
    "optimization_MC_WqqZll.root",
    "optimization_MC_llll.root",
    "optimization_MC_lllvSFMinus.root",
    "optimization_MC_lllvOFMinus.root",
    "optimization_MC_lllvSFPlus.root",
    "optimization_MC_lllvOFPlus.root",
    "optimization_MC_ggllll.root",
    "optimization_MC_ggllvv.root",
    "optimization_MC_llvv.root",
    "optimization_MC_llvvjj_ss_EW4.root",
    "optimization_MC_llvvjj_ss_EW6.root",
    "optimization_MC_lllvjj_EW6.root",
    "optimization_MC_lllljj_EW6.root"
};

void ytOptimization_plots(string signal_file = "optimization_MC_NUHM2_m12_400_strong.root", int luminosity = 36.5, float bkg_uncertainty = 0.3)
{
    TFile *f_signal = TFile::Open((path + signal_file).c_str());
    // TH1F *h_signal_derivation_stat_weights = (TH1F *)f_signal->Get("h_derivation_stat_weights");
    // TH1F *h_signal_cross_section_kfactor_efficiency = (TH1F *)f_signal->Get("h_cross_section_kfactor_efficiency");
    TH1F *h_signal_yield_weighted = (TH1F *)f_signal->Get("h_yields_weighted");
    double signal_derivation_stat_weights = ((TH1F *)f_signal->Get("h_derivation_stat_weights"))->GetBinContent(1);
    double signal_cross_section_kfactor_efficiency = ((TH1F *)f_signal->Get("h_cross_section_kfactor_efficiency"))->GetBinContent(1);

    // double signal_cross_section_kfactor_efficiency = 0;
    // if (signal_file == "optimization_MC_NUHM2_m12_300_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_300;
    // else if (signal_file == "optimization_MC_NUHM2_m12_350_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_350;
    // else if (signal_file == "optimization_MC_NUHM2_m12_400_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_400;
    // else if (signal_file == "optimization_MC_NUHM2_m12_500_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_500;
    // else if (signal_file == "optimization_MC_NUHM2_m12_600_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_600;
    // else if (signal_file == "optimization_MC_NUHM2_m12_700_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_700;
    // else if (signal_file == "optimization_MC_NUHM2_m12_800_strong.root")
    //     signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_800;

    int n_background_files = sizeof(background_files) / sizeof(background_files[0]);
    // cout << "n_background_files=" << n_background_files << endl;

    TFile *f_background[n_background_files];
    // TH1F *h_background_derivation_stat_weights[n_background_files];
    // TH1F *h_background_cross_section_kfactor_efficiency[n_background_files];
    TH1F *h_background_yield_weighted[n_background_files];
    double background_derivation_stat_weights[n_background_files];
    double background_cross_section_kfactor_efficiency[n_background_files];
    for (int i = 0; i < n_background_files; i++) {
        f_background[i] = TFile::Open((path + background_files[i]).c_str());
        h_background_yield_weighted[i] = (TH1F *)f_background[i]->Get("h_yields_weighted");
        // TH1F *h_background_derivation_stat_weights = (TH1F *)f_background[i]->Get("h_derivation_stat_weights");
        // TH1F *h_background_cross_section_kfactor_efficiency = (TH1F *)f_background[i]->Get("h_cross_section_kfactor_efficiency");
        background_derivation_stat_weights[i] = ((TH1F *)f_background[i]->Get("h_derivation_stat_weights"))->GetBinContent(1);
        background_cross_section_kfactor_efficiency[i] = ((TH1F *)f_background[i]->Get("h_cross_section_kfactor_efficiency"))->GetBinContent(1);
    }

    
    float max_significance = 0;
    int best_n_lept_cut = 0;
    int best_bjet_pt_cut = 0;
    int best_n_bjet_cut = 0;
    int best_jets_pt_cut = 0;
    int best_n_jets_cut = 0;
    int best_met_cut = 0;
    int best_meff_cut = 0;
    int best_cut_bin = 0;

    int bin = 0;
    // Number of lepton requirement
    for (int i_lept = 0; i_lept < sizeof(n_lept_cuts) / sizeof(n_lept_cuts[0]); i_lept++) {
        // b-jet pT requirement
        for (int i_bjet_pt = 0; i_bjet_pt < sizeof(bjet_pt_cuts) / sizeof(bjet_pt_cuts[0]); i_bjet_pt++) {
            // Number of b-jet requirement
            for (int i_bjet = 0; i_bjet < sizeof(n_bjet_cuts) / sizeof(n_bjet_cuts[0]); i_bjet++) {
                // jet pT requirement
                for (int i_jets_pt = 0; i_jets_pt < sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0]); i_jets_pt++) {
                    // Number of jet requirement
                    for (int i_jets = 0; i_jets < sizeof(n_jets_cuts) / sizeof(n_jets_cuts[0]); i_jets++) {
                        cout << "bin=" << bin 
                            << ", n_lept_cuts=" << n_lept_cuts[i_lept]
                            << ", bjet_pt_cuts=" << bjet_pt_cuts[i_bjet_pt]
                            << ", n_bjet_cuts=" << n_bjet_cuts[i_bjet]
                            << ", jets_pt_cuts=" << jets_pt_cuts[i_jets_pt]
                            << ", n_jets_cuts=" << n_jets_cuts[i_jets]
                            << endl;

                        stringstream bin_counter;
                        bin_counter << bin;
                        string hist_name = "hist_" + bin_counter.str();

                        stringstream ss_n_lept_cuts, ss_bjet_pt_cuts, ss_n_bjet_cuts, ss_jets_pt_cuts, ss_n_jets_cuts;
                        ss_n_lept_cuts << n_lept_cuts[i_lept];
                        ss_bjet_pt_cuts << bjet_pt_cuts[i_bjet_pt];
                        ss_n_bjet_cuts << n_bjet_cuts[i_bjet];
                        ss_jets_pt_cuts << jets_pt_cuts[i_jets_pt];
                        ss_n_jets_cuts << n_jets_cuts[i_jets];
                        string hist_title = "N_{l}=" + ss_n_lept_cuts.str()
                                        + ", b-jet p_{T}=" + ss_bjet_pt_cuts.str()
                                        + ", N_{b-jet}=" + ss_n_bjet_cuts.str()
                                        + ", jet p_{T}=" + ss_jets_pt_cuts.str()
                                        + ", N_{jet}=" + ss_n_jets_cuts.str();

                        int n_xbins = sizeof(met_cuts) / sizeof(met_cuts[0]) - 1;
                        int n_ybins = sizeof(meff_cuts) / sizeof(meff_cuts[0]) - 1;
                        // cout << "n_xbins=" << n_xbins << endl;
                        // cout << "n_ybins=" << n_ybins << endl;
                        TH2F *hist = new TH2F(hist_name.c_str(), (hist_title + ";E_{T}^{miss} [GeV];M_{eff} [GeV]").c_str(), n_xbins, &met_cuts[0], n_ybins, &meff_cuts[0]);

                        // MET requirement
                        for (unsigned int i_met = 0; i_met < sizeof(met_cuts) / sizeof(met_cuts[0]); i_met++) {
                            // Meff requirement
                            for (unsigned int i_meff = 0; i_meff < sizeof(meff_cuts) / sizeof(meff_cuts[0]); i_meff++) {
                                // int n_signal = h_signal_yield_weighted->GetBinContent(bin) 
                                //              * luminosity * signal_cross_section_kfactor_efficiency * 1000. / signal_derivation_stat_weights;
                                int n_signal = h_signal_yield_weighted->GetBinContent(bin);
                                // Loop over background
                                int n_background = 0;
                                for (int i = 0; i < n_background_files; i++) {
                                    // n_background += h_background_yield_weighted[i]->GetBinContent(bin)
                                    //               * luminosity * background_cross_section_kfactor_efficiency[i] * 1000. / background_derivation_stat_weights[i];
                                    n_background += h_background_yield_weighted[i]->GetBinContent(bin);
                                }
                                float significance = 0;
                                cout << "n_signal=" << n_signal << ", n_background=" << n_background << endl;
                                if (n_signal >= 2 &&
                                    n_background >= 1) {
                                    significance = RooStats::NumberCountingUtils::BinomialExpZ(n_signal, n_background, bkg_uncertainty);
                                }
                                else if (n_signal < 2)
                                    significance = -2;
                                else if (n_background < 1)
                                    significance = -1;

                                cout << "max_significance=" << max_significance << endl;

                                if (significance > max_significance) {
                                    max_significance = significance;
                                    best_n_lept_cut = n_lept_cuts[i_lept];
                                    best_bjet_pt_cut = bjet_pt_cuts[i_bjet_pt];
                                    best_n_bjet_cut = n_bjet_cuts[i_bjet];
                                    best_jets_pt_cut = jets_pt_cuts[i_jets_pt];
                                    best_n_jets_cut = n_jets_cuts[i_jets];
                                    best_met_cut = met_cuts[i_met];
                                    best_meff_cut = meff_cuts[i_meff];
                                    best_cut_bin = bin;
                                }

                                cout << "met_cuts=" << met_cuts[i_met]
                                    << ", meff_cuts=" << meff_cuts[i_meff]
                                    << ", significance=" << significance
                                    <<endl;
                                // cout << "bin=" << bin
                                //     << ", n_signal=" << n_signal
                                //     << ", n_background=" << n_background
                                //     << ", significance=" << significance
                                //     << ", n_lept_cuts=" << n_lept_cuts[i_lept]
                                //     << ", bjet_pt_cuts=" << bjet_pt_cuts[i_bjet_pt]
                                //     << ", n_bjet_cuts=" << n_bjet_cuts[i_bjet]
                                //     << ", jets_pt_cuts=" << jets_pt_cuts[i_jets_pt]
                                //     << ", n_jets_cuts=" << n_jets_cuts[i_jets]
                                //     << ", met_cuts=" << met_cuts[i_met]
                                //     << ", meff_cuts=" << meff_cuts[i_meff]
                                //     <<endl;
                                
                                hist->SetBinContent(i_met + 1, i_meff + 1, significance);
                                
                                bin++;
                            }
                        }
                        TCanvas *c1 = new TCanvas("c1", "c1");
                        hist->SetStats(kFALSE);
                        hist->Draw("colz,text");
                        c1->SaveAs((hist_name + ".pdf").c_str());
                        c1->Close();
                    }
                }
            }
        }
    }

    cout << "***** best cut *****" << endl;
    cout << "significance=" << max_significance
        << ", n_lept_cuts=" << best_n_lept_cut
        << ", bjet_pt_cuts=" << best_bjet_pt_cut
        << ", n_bjet_cuts=" << best_n_bjet_cut
        << ", jets_pt_cuts=" << best_jets_pt_cut
        << ", n_jets_cuts=" << best_n_jets_cut
        << ", met_cuts=" << best_met_cut
        << ", meff_cuts=" << best_meff_cut
        << ", at bin=" << best_cut_bin
        << endl;
}
