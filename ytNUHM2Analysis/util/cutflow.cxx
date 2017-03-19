// Usage:
// cutflow isData [PROOF/Condor]
// cutflow isMC Zee/Zmumu/ttbar/GG_ttn1 [PROOF/Condor]
//

#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/Sample.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoop/ProofDriver.h"
#include "EventLoop/CondorDriver.h"
#include "EventLoopGrid/PrunDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>
#include <TH1.h>

#include "ytNUHM2Analysis/ytEventSelection.h"

#include <iostream>
#include <string>
using namespace std;

int main( int argc, char* argv[] ) {

    // Take the submit directory from the input if provided:
    // std::string submitDir = "submitDir";
    std::string submitDir;
    // if( argc > 1 ) submitDir = argv[ 1 ];

    bool isMC = false;
    bool isData = false;

    bool isCutflow = true;
    bool isOptimization = false;

    string process;

    bool use_Condor = false;
    bool use_Grid = false;
    bool use_PROOF = false;

    const char* bkg_sample;

    for (int i = 1; i < argc; i++) {
        // For running signal
        // const char *key = argv[i];
        // For running bkg:
        const char* key = strtok(argv[i], "=");
        const char* val = strtok(0, " ");
        // Check MC or data.
        if (strcmp(key, "isMC") == 0)
            isMC = true;
        else if (strcmp(key, "isData") == 0)
            isData = true;
        // Check SR optimization
        else if (strcmp(key, "optimization") == 0)
            isOptimization = true;
        // Choose samples to run.
        else if (strcmp(key, "4topSM") == 0)
            process = "4topSM";
        else if (strcmp(key, "Zee") == 0)
            process = "Zee";
        else if (strcmp(key, "Zmumu") == 0)
            process = "Zmumu";
        else if (strcmp(key, "ttbar") == 0)
            process = "ttbar";
        else if (strcmp(key, "GG_ttn1") == 0)
            process = "GG_ttn1";
        // signal NUHM2
        // strong
        else if (strcmp(key, "NUHM2_m12_300_strong") == 0)
            process = "NUHM2_m12_300_strong";
        else if (strcmp(key, "NUHM2_m12_350_strong") == 0)
            process = "NUHM2_m12_350_strong";
        else if (strcmp(key, "NUHM2_m12_400_strong") == 0)
            process = "NUHM2_m12_400_strong";
        else if (strcmp(key, "NUHM2_m12_500_strong") == 0)
            process = "NUHM2_m12_500_strong";
        else if (strcmp(key, "NUHM2_m12_600_strong") == 0)
            process = "NUHM2_m12_600_strong";
        else if (strcmp(key, "NUHM2_m12_700_strong") == 0)
            process = "NUHM2_m12_700_strong";
        else if (strcmp(key, "NUHM2_m12_800_strong") == 0)
            process = "NUHM2_m12_800_strong";
        // weak
        else if (strcmp(key, "NUHM2_m12_300_weak") == 0)
            process = "NUHM2_m12_300_weak";
        else if (strcmp(key, "NUHM2_m12_350_weak") == 0)
            process = "NUHM2_m12_350_weak";
        else if (strcmp(key, "NUHM2_m12_400_weak") == 0)
            process = "NUHM2_m12_400_weak";
        else if (strcmp(key, "NUHM2_m12_500_weak") == 0)
            process = "NUHM2_m12_500_weak";
        else if (strcmp(key, "NUHM2_m12_600_weak") == 0)
            process = "NUHM2_m12_600_weak";
        else if (strcmp(key, "NUHM2_m12_700_weak") == 0)
            process = "NUHM2_m12_700_weak";
        else if (strcmp(key, "NUHM2_m12_800_weak") == 0)
            process = "NUHM2_m12_800_weak";
        // bkg
        else if (strcmp(key, "bkg") == 0) {
            bkg_sample = val;
            process = "bkg";
        }
        else if (strcmp(key, "Wplusenu") == 0)
            process = "Wplusenu";
        else if (strcmp(key, "Wplusmunu") == 0)
            process = "Wplusmunu";
        else if (strcmp(key, "Wplustaunu") == 0)
            process = "Wplustaunu";
        else if (strcmp(key, "Wminusenu") == 0)
            process = "Wminusenu";
        else if (strcmp(key, "Wminusmunu") == 0)
            process = "Wminusmunu";
        else if (strcmp(key, "Wminustaunu") == 0)
            process = "Wminustaunu";
        else if (strcmp(key, "Ztautau") == 0)
            process = "Ztautau";
        else if (strcmp(key, "ttW_Np0") == 0)
            process = "ttW_Np0";
        else if (strcmp(key, "ttW_Np1") == 0)
            process = "ttW_Np1";
        else if (strcmp(key, "ttW_Np2") == 0)
            process = "ttW_Np2";
        else if (strcmp(key, "ttee_Np0") == 0)
            process = "ttee_Np0";
        else if (strcmp(key, "ttee_Np1") == 0)
            process = "ttee_Np1";
        else if (strcmp(key, "ttmumu_Np0") == 0)
            process = "ttmumu_Np0";
        else if (strcmp(key, "ttmumu_Np1") == 0)
            process = "ttmumu_Np1";
        else if (strcmp(key, "tttautau_Np0") == 0)
            process = "tttautau_Np0";
        else if (strcmp(key, "tttautau_Np1") == 0)
            process = "tttautau_Np1";
        else if (strcmp(key, "ttbarWW") == 0)
            process = "ttbarWW";
        else if (strcmp(key, "3top_SM") == 0)
            process = "3top_SM";
        else if (strcmp(key, "llll") == 0)
            process = "llll";
        else if (strcmp(key, "lllvSFMinus") == 0)
            process = "lllvSFMinus";
        else if (strcmp(key, "lllvOFMinus") == 0)
            process = "lllvOFMinus";
        else if (strcmp(key, "lllvSFPlus") == 0)
            process = "lllvSFPlus";
        else if (strcmp(key, "lllvOFPlus") == 0)
            process = "lllvOFPlus";
        else if (strcmp(key, "llvvjj_ss_EW4") == 0)
            process = "llvvjj_ss_EW4";
        else if (strcmp(key, "llvvjj_ss_EW6") == 0)
            process = "llvvjj_ss_EW6";
        else if (strcmp(key, "lllvjj_EW6") == 0)
            process = "llvvjj_EW6";
        else if (strcmp(key, "lllljj_EW6") == 0)
            process = "lllljj_EW6";
        else if (strcmp(key, "ggllll") == 0)
            process = "ggllll";
        else if (strcmp(key, "ZZZ_6l0v") == 0)
            process = "ZZZ_6l0v";
        else if (strcmp(key, "ZZZ_4l2v") == 0)
            process = "ZZZ_4l2v";
        else if (strcmp(key, "ZZZ_2l4v") == 0)
            process = "ZZZ_2l4v";
        else if (strcmp(key, "llvv") == 0)
            process = "llvv";
        else if (strcmp(key, "ggllvv") == 0)
            process = "ggllvv";
        else if (strcmp(key, "WqqZll") == 0)
            process = "WqqZll";
        else if (strcmp(key, "ZqqZll") == 0)
            process = "ZqqZll";
        else if (strcmp(key, "singletop_tchan_lept_top") == 0)
            process = "singletop_tchan_lept_top";
        else if (strcmp(key, "singletop_tchan_lept_antitop") == 0)
            process = "singletop_tchan_lept_antitop";
        else if (strcmp(key, "Wt_inclusive_top") == 0)
            process = "Wt_inclusive_top";
        else if (strcmp(key, "Wt_inclusive_antitop") == 0)
            process = "Wt_inclusive_antitop";
        else if (strcmp(key, "Wt_dilepton_top") == 0)
            process = "Wt_dilepton_top";
        else if (strcmp(key, "Wt_dilepton_antitop") == 0)
            process = "Wt_dilepton_antitop";
        else if (strcmp(key, "ttbarHT6c_1k_hdamp172p5") == 0)
            process = "ttbarHT6c_1k_hdamp172p5";
        else if (strcmp(key, "ttbarHT1k_1k5_hdamp172p5") == 0)
            process = "ttbarHT1k_1k5_hdamp172p5";
        else if (strcmp(key, "ttbarHT1k5_hdamp172p5_no") == 0)
            process = "ttbarHT1k5_hdamp172p5_no";

        // Specify the driver to run.
        else if (strcmp(key, "Condor") == 0)
            use_Condor = true;
        else if (strcmp(key, "Grid") == 0)
            use_Grid = true;
        else if (strcmp(key, "PROOF") == 0)
            use_PROOF = true;
    }

    if (isCutflow && !isOptimization)
        printf("Running cutflow for %s\n", isMC ? process.c_str() : "Data");
    else if (isOptimization)
        printf("Running SR optimization for %s\n", isMC ? process.c_str() : "Data");
    cout << "process = " << process << endl;

    printf("isMC = %s, isData = %s\n", isMC ? "true" : "false", isData ? "true" : "false");

    if (isMC && !process.empty()) {
        if (process == "bkg")
            cout << "process = " << bkg_sample << endl;
        else
            cout << "process = " << process << endl;
    }

    if (isMC) {
        if (isCutflow && !isOptimization)
            submitDir = "cutflow_MC_" + process;
        else if (isOptimization)
            submitDir = "optimization_MC_" + process;
        if (process == "bkg")
            submitDir += "_" + string(bkg_sample);
    }
    else if (isData) {
        if (isCutflow && !isOptimization)
            submitDir = "cutflow_Data";
        else if (isOptimization)
            submitDir = "optimization_Data";
    }
    cout << "submitDir = " << submitDir << endl;

    if (use_Condor) {
        printf("Submit jobs to CondorDriver...\n");
    }
    else if (use_Grid) {
        printf("Submit jobs to PrunDriver...\n");
    }
    else if (use_PROOF) {
        printf("Submit jobs to ProofDriver...\n");
    }
    else {
        printf("Submit jobs to DirectDriver...\n");
    }

    // Construct the samples to run on:
    SH::SampleHandler sh;

    // use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
    const char* inputFilePath;
    const char* NUHM2_inputFilePath;
    const char* bkg_inputFilePath;

    if (isMC) {
        cout << "Read MC files..." << endl;
        // inputFilePath = "/raid05/atlas/data/ss3l/ximo-v44"; // no slash (/) at the end.
        inputFilePath = "/raid05/users/shen/Ximo_ntuples/v44/MC"; // no slash (/) at the end.
        NUHM2_inputFilePath = "/raid05/atlas/data/NUHM2/strongMC/p2666"; // no slash (/) at the end.
        bkg_inputFilePath = "/raid05/atlas/data/ss3l/ximo-v44"; // no slash (/) at the end.

        // For cutflow study
        if (process == "4topSM") {
            // inputFilePath = "/UserDisk2/yushen/Ximo_ntuples/v44/MC/user.jpoveda.t0789_v44.410080.MadGraphPythia8EvtGen_A14NNPDF23_4topSM.DAOD_SUSY2.s2608_r7725_p2666_output.root";
            // SH::ScanDir().filePattern("user.jpoveda.9048853._000001.output.root").scan(sh, inputFilePath);
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410080.*4topSM*").scan(sh, inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("4topSM_merged.root").scan(sh, inputFilePath);
        }
        // For real lepton efficiency study
        else if (process == "Zee") {
            SH::ScanDir().filePattern("Zee_merged.root").scan(sh, inputFilePath); // Get specific root file
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361106.*Zee*").scan(sh, inputFilePath); // Get all root files in this dataset
        }
        else if (process == "Zmumu") {
            SH::ScanDir().filePattern("Zmumu_merged.root").scan(sh, inputFilePath); // Get specific root file
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361107.*Zmumu*").scan(sh, inputFilePath); // Get all root files in this dataset
        }
        else if (process == "ttbar") {
            SH::ScanDir().filePattern("ttbar_merged.root").scan(sh, inputFilePath); // Get specific root file
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410000.*ttbar*nonallhad*").scan(sh, inputFilePath); // Get all root files in this dataset
        }
        else if (process == "GG_ttn1") {
            SH::ScanDir().filePattern("GG_ttn1_merged.root").scan(sh, inputFilePath); // Get specific root file
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*GG_ttn1_*.root").scan(sh, inputFilePath); // Get all root files in this dataset
        }
        // strong
        else if (process == "NUHM2_m12_300_strong") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370600.MGPy8EG_A14N23LO_NUHM2_m12_300_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_350_strong") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370601.MGPy8EG_A14N23LO_NUHM2_m12_350_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_400_strong") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370602.MGPy8EG_A14N23LO_NUHM2_m12_400_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_500_strong") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370603.MGPy8EG_A14N23LO_NUHM2_m12_500_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_600_strong") {
            SH::ScanDir().samplePattern("user.yushen.Jul05.v39.370604.MGPy8EG_A14N23LO_NUHM2_m12_600_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_700_strong") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370605.MGPy8EG_A14N23LO_NUHM2_m12_700_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_800_strong") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370606.MGPy8EG_A14N23LO_NUHM2_m12_800_strong.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        // weak
        else if (process == "NUHM2_m12_300_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370617.MGPy8EG_A14N23LO_NUHM2_m12_300_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_350_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370618.MGPy8EG_A14N23LO_NUHM2_m12_350_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_400_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370619.MGPy8EG_A14N23LO_NUHM2_m12_400_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_500_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370620.MGPy8EG_A14N23LO_NUHM2_m12_500_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_600_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370621.MGPy8EG_A14N23LO_NUHM2_m12_600_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_700_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370622.MGPy8EG_A14N23LO_NUHM2_m12_700_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "NUHM2_m12_800_weak") {
            SH::ScanDir().samplePattern("user.yushen.Aug04.v44.370623.MGPy8EG_A14N23LO_NUHM2_m12_800_weak.DAOD_SUSY2.a766_r7676_output.root").scan(sh, NUHM2_inputFilePath); // Get all root files in this dataset
        }
        // bkg
        else if (process == "bkg") {
            SH::ScanDir().samplePattern(bkg_sample).scan(sh, bkg_inputFilePath); // Get all root files in this dataset
        }
        else if (process == "Wplusenu") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361100.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wplusenu.DAOD_SUSY2.s2576_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wplusenu_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wplusmunu") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361101.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wplusmunu.DAOD_SUSY2.s2576_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wplusmunu_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wplustaunu") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361102.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wplustaunu.DAOD_SUSY2.s2576_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wplustaunu_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wminusenu") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361103.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wminusenu.DAOD_SUSY2.s2576_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wminusenu_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wminusmunu") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361104.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wminusmunu.DAOD_SUSY2.s2576_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wminusmunu_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wminustaunu") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361105.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wminustaunu.DAOD_SUSY2.s2576_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wminustaunu_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Ztautau") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Ztautau_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttW_Np0") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410066.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np0.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttW_Np0_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttW_Np1") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410067.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np1.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttW_Np1_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttW_Np2") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410068.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np2.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttW_Np2_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttee_Np0") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410111.MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np0.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttee_Np0_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttee_Np1") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410112.MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np1.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttee_Np1_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttmumu_Np0") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410113.MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np0.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttmumu_Np0_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttmumu_Np1") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410114.MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np1.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttmumu_Np1_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "tttautau_Np0") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410115.MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np0.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("tttautau_Np0_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "tttautau_Np1") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410116.MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np1.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("tttautau_Np1_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttbarWW") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttbarWW_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "3top_SM") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.304014.MadGraphPythia8EvtGen_A14NNPDF23_3top_SM.DAOD_SUSY2.a766_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("3top_SM_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "llll") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361063.Sherpa_CT10_llll.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("llll_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "lllvSFMinus") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361064.Sherpa_CT10_lllvSFMinus.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("lllvSFMinus_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "lllvOFMinus") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361065.Sherpa_CT10_lllvOFMinus.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("lllvOFMinus_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "lllvSFPlus") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361066.Sherpa_CT10_lllvSFPlus.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("lllvSFPlus_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "lllvOFPlus") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361067.Sherpa_CT10_lllvOFPlus.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("lllvOFPlus_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "llvvjj_ss_EW4") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361069.Sherpa_CT10_llvvjj_ss_EW4.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("llvvjj_ss_EW4_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "llvvjj_ss_EW6") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361070.Sherpa_CT10_llvvjj_ss_EW6.DAOD_SUSY2.s2608_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("llvvjj_ss_EW6_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "lllvjj_EW6") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361071.Sherpa_CT10_lllvjj_EW6.DAOD_SUSY2.s2726_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("lllvjj_EW6_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "lllljj_EW6") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361072.Sherpa_CT10_lllljj_EW6.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("lllljj_EW6_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ggllll") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361073.Sherpa_CT10_ggllll.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ggllll_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ZZZ_6l0v") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361625.Sherpa_CT10_ZZZ_6l0v.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ZZZ_6l0v_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ZZZ_4l2v") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361626.Sherpa_CT10_ZZZ_4l2v.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ZZZ_4l2v_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ZZZ_2l4v") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361627.Sherpa_CT10_ZZZ_2l4v.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ZZZ_2l4v_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "llvv") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361068.Sherpa_CT10_llvv.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("llvv_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ggllvv") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361077.Sherpa_CT10_ggllvv.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ggllvv_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "WqqZll") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361084.Sherpa_CT10_WqqZll.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("WqqZll_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ZqqZll") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.361086.Sherpa_CT10_ZqqZll.DAOD_SUSY2.s2608_r7772_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ZqqZll_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "singletop_tchan_lept_top") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410011.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top.DAOD_SUSY2.a766_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("singletop_tchan_lept_top_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "singletop_tchan_lept_antitop") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410012.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop.DAOD_SUSY2.a766_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("singletop_tchan_lept_antitop_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wt_inclusive_top") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410013.PowhegPythiaEvtGen_P2012_Wt_inclusive_top.DAOD_SUSY2.a766_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wt_inclusive_top_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wt_inclusive_antitop") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410014.PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop.DAOD_SUSY2.a766_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wt_inclusive_antitop_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wt_dilepton_top") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410015.PowhegPythiaEvtGen_P2012_Wt_dilepton_top.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wt_dilepton_top_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "Wt_dilepton_antitop") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.410016.PowhegPythiaEvtGen_P2012_Wt_dilepton_antitop.DAOD_SUSY2.s2608_r7725_p2666_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("Wt_dilepton_antitop_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttbarHT6c_1k_hdamp172p5") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.407009.PowhegPythiaEvtGen_P2012CT10_ttbarHT6c_1k_hdamp172p5_.DAOD_SUSY2.s2608_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttbarHT6c_1k_hdamp172p5_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttbarHT1k_1k5_hdamp172p5") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.407010.PowhegPythiaEvtGen_P2012CT10_ttbarHT1k_1k5_hdamp172p5.DAOD_SUSY2.s2608_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttbarHT1k_1k5_hdamp172p5_merged.root").scan(sh, inputFilePath);
        }
        else if (process == "ttbarHT1k5_hdamp172p5_no") {
            // SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.407011.PowhegPythiaEvtGen_P2012CT10_ttbarHT1k5_hdamp172p5_no.DAOD_SUSY2.s2608_r7676_output.root").scan(sh, bkg_inputFilePath); // Get all root files in this dataset
            SH::ScanDir().filePattern("ttbarHT1k5_hdamp172p5_merged.root").scan(sh, inputFilePath);
        }
    }
    else if (isData) {
        cout << "Read Data files..." << endl;
        inputFilePath = "/raid05/users/shen/Ximo_ntuples/v44/Data"; // no slash (/) at the end.
        //SH::ScanDir().scan(sh, inputFilePath); // Get all datasets in inputFilePath
        SH::ScanDir().filePattern("merged_all_data.root").scan(sh, inputFilePath); // Get specific root file
        //SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*.physics_Main.DAOD_SUSY2.*").scan(sh, inputFilePath); // Get all root files in this dataset
    }

    // Set the name of the input TTree.
    sh.setMetaString("nc_tree", "AnaNtup");

    // Print what we found:
    sh.print();

    //
    // Get DerivationStat_Weights from input files
    //
    double derivation_stat_weights = 0;
    SH::Sample *sample = sh.at(sh.size() - 1); // Because we only have one dataset at here
    // cout << "sample name=" << sample->name() << endl; // dataset name
    // cout << "numFiles()=" << sample->numFiles() << endl; // number of root files in dataset
    for (unsigned int i = 0; i < sample->numFiles() ; i++) {
        string fileName = sample->fileName(i); // this returns file://root file name and path
        string remove = "file://"; // need to remove "file://" part
        string::size_type find_remove_part = fileName.find(remove);
        if (find_remove_part != string::npos)
            fileName.erase(find_remove_part, remove.length()); // now contains root file name and path only
        //cout << "fileName(" << i << ")=" << fileName << endl;
        TFile *file = new TFile(fileName.c_str());
        TH1D *h1 = (TH1D *)file->Get("DerivationStat_Weights");
        derivation_stat_weights += h1->GetBinContent(1);
    }
    // cout << "DerivationStat_Weights=" << derivation_stat_weights << endl;
    // Get the dataset name
    string dataset_name = sh.at(0)->fileName(0);
    //int index_of_DAOD_SUSY2 = dataset_name.find("DAOD_SUSY2.");
    bool isFullSim = false, isAF2Sim = false;
    if (dataset_name.find("_r") != string::npos) {
        isFullSim = true;
        isAF2Sim = false;
    }
    if (dataset_name.find("_a") != string::npos ||
        dataset_name.find(".a") != string::npos) {
        isFullSim = false;
        isAF2Sim = true;
    }
    printf("isFullSim = %s, isAF2Sim = %s\n", isFullSim ? "true" : "false", isAF2Sim ? "true" : "false");

    // Create an EventLoop job:
    EL::Job job;
    job.sampleHandler( sh );
    //job.options()->setDouble (EL::Job::optMaxEvents, 50);

    // Add our analysis to the job:
    ytEventSelection *alg = new ytEventSelection();
    alg->set_isMC(isMC);
    alg->set_isData(isData);
    alg->set_isSkim(false);
    alg->set_isOptimization(isOptimization);
    alg->set_isFullSim(isFullSim);
    alg->set_isAF2Sim(isAF2Sim);
    alg->set_process(process);
    alg->set_tag_pt_threshold(25000.);
    alg->set_derivation_stat_weights(derivation_stat_weights);
    if (isMC && !process.empty()) {
        if (process != "bkg")
            alg->set_process(process);
        else
            alg->set_process(string(bkg_sample));
    }

    const double luminosity = 36.5; // unit: 1/fb 2015+2016
    alg->set_luminosity(luminosity);

    if (isData)
        alg->set_process(process);
    //alg->set_derivation_stat_weights(derivation_stat_weights);
    job.algsAdd( alg );

    if (use_Condor) {
        // Run the jobs using the Condor driver:
        EL::CondorDriver driver;
        // some commands for setting up root on the nodes
        driver.shellInit = "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase ; source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh ; eval rcSetup Base,2.4.19";
        driver.submitOnly( job, submitDir );
    }
    else if (use_Grid) {
        // Run the jobs using the Grid driver:
        EL::PrunDriver driver;
        // Specify how to name the grid output datasets
        // Note that %nickname% is populated when you do voms-proxy init, so this does not have to be replaced by hand
        driver.options()->setString("nc_outputSampleName", "user.%nickname%.%in:name[2]%.%in:name[3]%.%in:name[6]%.");
        driver.submitOnly( job, submitDir );
    }
    else if (use_PROOF) {
        EL::ProofDriver driver;
        driver.submit( job, submitDir );
    }
    else {
        // Run the job using the local/direct driver:
        EL::DirectDriver driver;
        driver.submit( job, submitDir );
    }

    return 0;
}
