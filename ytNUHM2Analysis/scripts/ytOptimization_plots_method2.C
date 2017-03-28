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

#define N_lepts 2
#define N_bjets 1
#define Bjet_pT 20
#define N_jets 6

// const float n_lept_cuts[5] = {2, 3, 4, 5, 6};
// const float n_bjet_cuts[6] = {0, 1, 2, 3, 4, 5};
// const float n_jets_cuts[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
// const float bjet_pt_cuts[9] = {20., 25., 30., 35., 40., 50., 70., 100., 150.};
// const float jets_pt_cuts[9] = {20., 25., 30., 35., 40., 50., 70., 100., 150.};
const float jets_pt_cuts[1] = {25.};
// const float met_cuts[10] = {0., 50., 100., 150., 200., 250., 300., 350., 400., 500.};
// const float meff_cuts[21] = {
//     0., 100., 200., 300., 400., 500., 600., 700., 800., 900., 1000., 
//     1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.
// };
const float met_cuts[12] = {50., 75., 100., 125., 150., 175., 200., 250., 300., 350., 400., 500.};
const float meff_cuts[12] = {0., 500., 600., 900., 1200., 1300., 1500., 1600., 1700., 1800., 1900., 2100.};
// const float met_cuts[1] = {150.};
// const float meff_cuts[1] = {600.};

// string path = "/raid05/users/shen/Ximo_ntuples/v44/NUHM2/Results/20170327_SR1b1/";
string path = "/raid05/users/shen/Ximo_ntuples/v44/NUHM2/";

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

string background_files[1] = {
    "optimization_MC_4topSM.root"
};
/*
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
*/
void ytOptimization_plots_method2(string signal_file = "optimization_MC_NUHM2_m12_500_strong.root", float luminosity = 36.5, float bkg_uncertainty = 0.3)
{
    TFile *f_signal = TFile::Open((path + signal_file).c_str());
    TH3F *h_signal_yield = (TH3F *)f_signal->Get("h_method2_yields");
    TH3F *h_signal_yield_weighted = (TH3F *)f_signal->Get("h_method2_yields_weighted");

    // Because the cross_section_kfactor_efficiency = 1 in SUSYTools
    // We need to correct it by ourself
    double signal_cross_section_kfactor_efficiency = 0;
    if (signal_file == "optimization_MC_NUHM2_m12_300_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_300;
    else if (signal_file == "optimization_MC_NUHM2_m12_350_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_350;
    else if (signal_file == "optimization_MC_NUHM2_m12_400_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_400;
    else if (signal_file == "optimization_MC_NUHM2_m12_500_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_500;
    else if (signal_file == "optimization_MC_NUHM2_m12_600_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_600;
    else if (signal_file == "optimization_MC_NUHM2_m12_700_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_700;
    else if (signal_file == "optimization_MC_NUHM2_m12_800_strong.root")
        signal_cross_section_kfactor_efficiency = Xsec_NUMH2_m12_800;

    // Background samples
    int n_background_files = sizeof(background_files) / sizeof(background_files[0]);
    TFile *f_background[n_background_files];
    TH3F *h_background_yield[n_background_files];
    TH3F *h_background_yield_weighted[n_background_files];

    for (int i = 0; i < n_background_files; i++) {
        f_background[i] = TFile::Open((path + background_files[i]).c_str());
        h_background_yield[i] = (TH3F *)f_background[i]->Get("h_method2_yields");
        h_background_yield_weighted[i] = (TH3F *)f_background[i]->Get("h_method2_yields_weighted");
    }

    float max_significance = 0;
    float best_jets_pt_cut = 0;
    float best_met_cut = 0;
    float best_meff_cut = 0;

    int bin = 1;
    for (unsigned int i_jets_pt = 0; i_jets_pt < sizeof(jets_pt_cuts) / sizeof(jets_pt_cuts[0]); i_jets_pt++) {
        stringstream bin_counter;
        bin_counter << bin;
        string h_nsig_name = "h_nsig_" + bin_counter.str();
        string h_nsig_weighted_name = "h_nsig_weighted_" + bin_counter.str();
        string h_nbkg_name = "h_nbkg_" + bin_counter.str();
        string h_nbkg_weighted_name = "h_nbkg_weighted_" + bin_counter.str();
        string hist_name = "hist_" + bin_counter.str();

        stringstream ss_n_lept_cuts, ss_bjet_pt_cuts, ss_n_bjet_cuts, ss_jets_pt_cuts, ss_n_jets_cuts;
        ss_n_lept_cuts << N_lepts;
        ss_bjet_pt_cuts << Bjet_pT;
        ss_n_bjet_cuts << N_bjets;
        ss_n_jets_cuts << N_jets;
        ss_jets_pt_cuts << jets_pt_cuts[i_jets_pt];
        
        string hist_title = "N_{l}>=" + ss_n_lept_cuts.str()
                        + ", b-jet p_{T}=" + ss_bjet_pt_cuts.str()
                        + ", N_{b-jets}=" + ss_n_bjet_cuts.str()
                        + ", N_{jets}>=" + ss_n_jets_cuts.str()
                        + ", p_{T,jets}>" + ss_jets_pt_cuts.str();

        // cout << "hist_title=" << hist_title << endl;

        int n_xbins = sizeof(met_cuts) / sizeof(met_cuts[0]);
        int n_ybins = sizeof(meff_cuts) / sizeof(meff_cuts[0]);

        // cout << "n_xbins=" << n_xbins << endl;
        // cout << "n_ybins=" << n_ybins << endl;

        TH2F *hist_n_signal = new TH2F(h_nsig_name.c_str(),
                                       (hist_title + ";E_{T}^{miss} [GeV];M_{eff} [GeV]").c_str(),
                                       n_xbins, 0, n_xbins + 1, n_ybins, 0, n_ybins + 1);
        TH2F *hist_n_signal_weighted = new TH2F(h_nsig_weighted_name.c_str(),
                                                (hist_title + ";E_{T}^{miss} [GeV];M_{eff} [GeV]").c_str(),
                                                n_xbins, 0, n_xbins + 1, n_ybins, 0, n_ybins + 1);
        TH2F *hist_n_background = new TH2F(h_nbkg_name.c_str(),
                                           (hist_title + ";E_{T}^{miss} [GeV];M_{eff} [GeV]").c_str(),
                                           n_xbins, 0, n_xbins + 1, n_ybins, 0, n_ybins + 1);
        TH2F *hist_n_background_weighted = new TH2F(h_nbkg_weighted_name.c_str(),
                                                    (hist_title + ";E_{T}^{miss} [GeV];M_{eff} [GeV]").c_str(),
                                                    n_xbins, 0, n_xbins + 1, n_ybins, 0, n_ybins + 1);
        TH2F *hist = new TH2F(hist_name.c_str(), (hist_title + ";E_{T}^{miss} [GeV];M_{eff} [GeV]").c_str(),
                              n_xbins, 0, n_xbins + 1, n_ybins, 0, n_ybins + 1);

        for (unsigned int i_met = 0; i_met < sizeof(met_cuts) / sizeof(met_cuts[0]); i_met++) {
            stringstream xbin_value;
            xbin_value << met_cuts[i_met];
            hist_n_signal->GetXaxis()->SetBinLabel(i_met + 1, xbin_value.str().c_str());
            hist_n_signal_weighted->GetXaxis()->SetBinLabel(i_met + 1, xbin_value.str().c_str());
            hist_n_background->GetXaxis()->SetBinLabel(i_met + 1, xbin_value.str().c_str());
            hist_n_background_weighted->GetXaxis()->SetBinLabel(i_met + 1, xbin_value.str().c_str());
            hist->GetXaxis()->SetBinLabel(i_met + 1, xbin_value.str().c_str());
            for (unsigned int i_meff = 0; i_meff < sizeof(meff_cuts) / sizeof(meff_cuts[0]); i_meff++) {
                stringstream ybin_value;
                ybin_value << meff_cuts[i_meff];
                hist_n_signal->GetYaxis()->SetBinLabel(i_meff + 1, ybin_value.str().c_str());
                hist_n_signal_weighted->GetYaxis()->SetBinLabel(i_meff + 1, ybin_value.str().c_str());
                hist_n_background->GetYaxis()->SetBinLabel(i_meff + 1, ybin_value.str().c_str());
                hist_n_background_weighted->GetYaxis()->SetBinLabel(i_meff + 1, ybin_value.str().c_str());
                hist->GetYaxis()->SetBinLabel(i_meff + 1, ybin_value.str().c_str());
                cout << "bin=" << bin
                    << ": i_jets_pt=" << i_jets_pt
                    << ", i_met=" << i_met
                    << ", i_meff=" << i_meff
                    << endl;

                cout << "jets_pt_cuts > " << jets_pt_cuts[i_jets_pt]
                    << ", met_cuts > " << met_cuts[i_met]
                    << ", meff_cuts > " << meff_cuts[i_meff]
                    << endl;

                // Signal
                int n_signal = h_signal_yield->GetBinContent(i_jets_pt + 1, i_met + 1, i_meff + 1);
                float n_signal_weighted = h_signal_yield_weighted->GetBinContent(i_jets_pt + 1, i_met + 1, i_meff + 1)
                                        * signal_cross_section_kfactor_efficiency;

                // Loop over background
                int n_background = 0;
                float n_background_weighted = 0;
                for (int i = 0; i < n_background_files; i++) {
                    n_background += h_background_yield[i]->GetBinContent(i_jets_pt + 1, i_met + 1, i_meff + 1);
                    n_background_weighted += h_background_yield_weighted[i]->GetBinContent(i_jets_pt + 1, i_met + 1, i_meff + 1);
                }

                cout << "n_signal=" << n_signal << ", n_background=" << n_background << endl;
                cout << "n_signal_weighted=" << n_signal_weighted << ", n_background_weighted=" << n_background_weighted << endl;

                hist_n_signal->SetBinContent(i_met + 1, i_meff + 1, n_signal);
                hist_n_signal_weighted->SetBinContent(i_met + 1, i_meff + 1, n_signal_weighted);
                hist_n_background->SetBinContent(i_met + 1, i_meff + 1, n_background);
                hist_n_background_weighted->SetBinContent(i_met + 1, i_meff + 1, n_background_weighted);

                // Calculate significance
                float significance = 0;
                
                if (n_signal_weighted >= 2. &&
                    n_background_weighted >= 1.) {
                    significance = RooStats::NumberCountingUtils::BinomialExpZ(n_signal_weighted, n_background_weighted, bkg_uncertainty);
                }
                else if (n_signal_weighted < 2.)
                    significance = 0.02;
                else if (n_background_weighted < 1.)
                    significance = 0.01;

                cout << "significance=" << significance << endl;

                cout << "max_significance=" << max_significance << endl;

                if (significance > max_significance) {
                    max_significance = significance;
                    best_jets_pt_cut = jets_pt_cuts[i_jets_pt];
                    best_met_cut = met_cuts[i_met];
                    best_meff_cut = meff_cuts[i_meff];
                }

                hist->SetBinContent(i_met + 1, i_meff + 1, significance);

                bin++;
            }
        }
        TCanvas *c1 = new TCanvas("c1", "c1");
        gPad->SetLeftMargin(0.12);
        gStyle->SetPaintTextFormat(".3f");
        hist_n_signal->SetStats(kFALSE);
        // hist_n_signal->GetXaxis()->SetNdivisions(110);
        // hist_n_signal->GetYaxis()->SetNdivisions(110);
        hist_n_signal->GetYaxis()->SetTitleOffset(1.5);
        hist_n_signal->Draw("colz,text");
        c1->SaveAs((h_nsig_name + ".pdf").c_str());
        c1->Close();

        TCanvas *c2 = new TCanvas("c2", "c2");
        gPad->SetLeftMargin(0.12);
        gStyle->SetPaintTextFormat(".3f");
        hist_n_signal_weighted->SetStats(kFALSE);
        // hist_n_signal_weighted->GetXaxis()->SetNdivisions(110);
        // hist_n_signal_weighted->GetYaxis()->SetNdivisions(110);
        hist_n_signal_weighted->GetYaxis()->SetTitleOffset(1.5);
        hist_n_signal_weighted->Draw("colz,text");
        c2->SaveAs((h_nsig_weighted_name + ".pdf").c_str());
        c2->Close();

        TCanvas *c3 = new TCanvas("c3", "c3");
        gPad->SetLeftMargin(0.12);
        gStyle->SetPaintTextFormat(".3f");
        hist_n_background->SetStats(kFALSE);
        // hist_n_background->GetXaxis()->SetNdivisions(110);
        // hist_n_background->GetYaxis()->SetNdivisions(110);
        hist_n_background->GetYaxis()->SetTitleOffset(1.5);
        hist_n_background->Draw("colz,text");
        c3->SaveAs((h_nbkg_name + ".pdf").c_str());
        c3->Close();

        TCanvas *c4 = new TCanvas("c4", "c4");
        gPad->SetLeftMargin(0.12);
        gStyle->SetPaintTextFormat(".3f");
        hist_n_background_weighted->SetStats(kFALSE);
        // hist_n_background_weighted->GetXaxis()->SetNdivisions(110);
        // hist_n_background_weighted->GetYaxis()->SetNdivisions(110);
        hist_n_background_weighted->GetYaxis()->SetTitleOffset(1.5);
        hist_n_background_weighted->Draw("colz,text");
        c4->SaveAs((h_nbkg_weighted_name + ".pdf").c_str());
        c4->Close();

        TCanvas *c5 = new TCanvas("c5", "c5");
        gPad->SetLeftMargin(0.12);
        gStyle->SetPaintTextFormat(".3f");
        hist->SetStats(kFALSE);
        hist->GetYaxis()->SetTitleOffset(1.5);
        hist->Draw("colz,text");
        c5->SaveAs((hist_name + ".pdf").c_str());
        c5->Close();
    }

    cout << "***** best cut *****" << endl;
    cout << "significance=" << max_significance
        << ", n_lept_cuts>=" << N_lepts
        << ", bjet_pt_cuts=" << Bjet_pT
        << ", n_bjet_cuts=" << N_bjets
        << ", n_jets_cuts=" << N_jets
        << ", jets_pt_cuts=" << best_jets_pt_cut
        << ", met_cuts=" << best_met_cut
        << ", meff_cuts=" << best_meff_cut
        << endl;
}