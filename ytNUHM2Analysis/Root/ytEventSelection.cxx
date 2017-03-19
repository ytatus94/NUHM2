#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <ytNUHM2Analysis/ytEventSelection.h>

// this is needed to distribute the algorithm to the workers
ClassImp(ytEventSelection)



ytEventSelection::ytEventSelection ()
{
    // Here you put any code for the base initialization of variables,
    // e.g. initialize all pointers to 0.  Note that you should only put
    // the most basic initialization here, since this method will be
    // called on both the submission and the worker node.  Most of your
    // initialization code will go into histInitialize() and
    // initialize().

    //const char * function_name = "ytEventSelection()";
    //Info(function_name, "Function calls");

    m_cutflow       = new yt_cutflows;
    // m_skim          = new yt_skim;
    m_optimization  = new yt_optimization;
    m_XsecDB        = new SUSY::CrossSectionDB(PathResolverFindCalibDirectory("SUSYTools/mc15_13TeV/"));
    m_dtwTool       = new DileptonTriggerWeight;

    // initialize event weight sum to zero
    //sum_of_weight_at_cut = vector<double>(Ncuts, 0); // initialize Ncuts elements to zero.
}



EL::StatusCode ytEventSelection::setupJob (EL::Job& job)
{
    // Here you put code that sets up the job on the submission object
    // so that it is ready to work with your algorithm, e.g. you can
    // request the D3PDReader service or add output files.  Any code you
    // put here could instead also go into the submission script.  The
    // sole advantage of putting it here is that it gets automatically
    // activated/deactivated when you add/remove the algorithm from your
    // job, which may or may not be of value to you.

    //const char * function_name = "setupJob()";
    //Info(function_name, "Function calls");

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::histInitialize ()
{
    // Here you do everything that needs to be done at the very
    // beginning on each worker node, e.g. create histograms and output
    // trees.  This method gets called before any input files are
    // connected.

    //const char * function_name = "histInitialize()";
    //Info(function_name, "Function calls");

    h_Nvtx      = new TH1F("h_Nvtx", "Nvtx;Nvtx;Events", 50, 0 , 50);
    h_AvgMu     = new TH1F("h_AvgMu", "AvgMu;<#mu>;Events", 50, 0 , 50);
    h_NLepts    = new TH1F("h_NLepts", "Number of leptons;N_{leptons};Events",10, 0, 10);
    h_NJets     = new TH1F("h_NJets", "Number of jets;N_{jets};Events", 40, 0, 40);

    h_Nvtx->Sumw2();
    h_AvgMu->Sumw2();
    h_NLepts->Sumw2();
    h_NJets->Sumw2();

    wk()->addOutput(h_Nvtx);
    wk()->addOutput(h_AvgMu);
    wk()->addOutput(h_NLepts);
    wk()->addOutput(h_NJets);

    // h_Nvtx_weighted      = new TH1F("h_Nvtx_weighted", "Nvtx (weighted);Nvtx;Events", 50, 0 , 50);
    // h_AvgMu_weighted     = new TH1F("h_AvgMu_weighted", "AvgMu (weighted);<#mu>;Events", 50, 0 , 50);
    // h_NLepts_weighted    = new TH1F("h_NLepts_weighted", "Number of leptons (weighted);N_{leptons};Events",10, 0, 10);
    // h_NJets_weighted     = new TH1F("h_NJets_weighted", "Number of jets (weighted);N_{jets};Events", 40, 0, 40);
    
    // h_Nvtx_weighted->Sumw2();
    // h_AvgMu_weighted->Sumw2();
    // h_NLepts_weighted->Sumw2();
    // h_NJets_weighted->Sumw2();

    // wk()->addOutput(h_Nvtx_weighted);
    // wk()->addOutput(h_AvgMu_weighted);
    // wk()->addOutput(h_NLepts_weighted);
    // wk()->addOutput(h_NJets_weighted);

    h_AvgMu_OSee            = new TH1F("h_AvgMu_OSee", "AvgMu;<#mu>;Events", 50, 0 , 50);
    h_AvgMu_OSmumu          = new TH1F("h_AvgMu_OSmumu", "AvgMu;<#mu>;Events", 50, 0 , 50);
    // h_AvgMu_OSee_weighted   = new TH1F("h_AvgMu_OSee_weighted", "AvgMu (weighted);<#mu>;Events", 50, 0 , 50);
    // h_AvgMu_OSmumu_weighted = new TH1F("h_AvgMu_OSmumu_weighted", "AvgMu (weighted);<#mu>;Events", 50, 0 , 50);

    h_AvgMu_OSee->Sumw2();
    h_AvgMu_OSmumu->Sumw2();
    // h_AvgMu_OSee_weighted->Sumw2();
    // h_AvgMu_OSmumu_weighted->Sumw2();

    wk()->addOutput(h_AvgMu_OSee);
    wk()->addOutput(h_AvgMu_OSmumu);
    // wk()->addOutput(h_AvgMu_OSee_weighted);
    // wk()->addOutput(h_AvgMu_OSmumu_weighted);

    h_baseline_OSee_mll = new TH1F("h_baseline_OSee_mll", "mll;M_{ll} [GeV];Events", 90, 60, 150);
    h_baseline_OSmumu_mll = new TH1F("h_baseline_OSmumu_mll", "mll;M_{ll} [GeV];Events", 90, 60, 150);
    // h_baseline_OSee_mll_weighted = new TH1F("h_baseline_OSee_mll_weighted", "mll;M_{ll} [GeV];Events", 90, 60, 150);
    // h_baseline_OSmumu_mll_weighted = new TH1F("h_baseline_OSmumu_mll_weighted", "mll;M_{ll} [GeV];Events", 90, 60, 150);

    h_baseline_OSee_mll->Sumw2();
    h_baseline_OSmumu_mll->Sumw2();
    // h_baseline_OSee_mll_weighted->Sumw2();
    // h_baseline_OSmumu_mll_weighted->Sumw2();

    wk()->addOutput(h_baseline_OSee_mll);
    wk()->addOutput(h_baseline_OSmumu_mll);
    // wk()->addOutput(h_baseline_OSee_mll_weighted);
    // wk()->addOutput(h_baseline_OSmumu_mll_weighted);

    h_signal_OSee_mll = new TH1F("h_signal_OSee_mll", "mll;M_{ll} [GeV];Events", 90, 60, 150);
    h_signal_OSmumu_mll = new TH1F("h_signal_OSmumu_mll", "mll;M_{ll} [GeV];Events", 90, 60, 150);
    // h_signal_OSee_mll_weighted = new TH1F("h_signal_OSee_mll_weighted", "mll;M_{ll} [GeV];Events", 90, 60, 150);
    // h_signal_OSmumu_mll_weighted = new TH1F("h_signal_OSmumu_mll_weighted", "mll;M_{ll} [GeV];Events", 90, 60, 150);

    h_signal_OSee_mll->Sumw2();
    h_signal_OSmumu_mll->Sumw2();
    // h_signal_OSee_mll_weighted->Sumw2();
    // h_signal_OSmumu_mll_weighted->Sumw2();

    wk()->addOutput(h_signal_OSee_mll);
    wk()->addOutput(h_signal_OSmumu_mll);
    // wk()->addOutput(h_signal_OSee_mll_weighted);
    // wk()->addOutput(h_signal_OSmumu_mll_weighted);

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::fileExecute ()
{
    // Here you do everything that needs to be done exactly once for every
    // single file, e.g. collect a list of all lumi-blocks processed

    //const char * function_name = "fileExecute()";
    //Info(function_name, "Function calls");

    // TH1F *h1 = (TH1F *)wk()->inputFile()->Get("DerivationStat_Weights");
    // derivation_stat_weights += h1->GetBinContent(1);
    //cout <<   "derivation_stat_weights=" << h1->GetBinContent(1) << endl;

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::changeInput (bool firstFile)
{
    // Here you do everything you need to do when we change input files,
    // e.g. resetting branch addresses on trees.  If you are using
    // D3PDReader or a similar service this method is not needed.

    //const char * function_name = "changeInput()";
    //Info(function_name, "Function calls");

    // Set object pointer
    Mu_eta = 0;
    Mu_phi = 0;
    Mu_pT = 0;
    Mu_SFw = 0;
    Mu_IsoSFw = 0;
    Mu_charge = 0;
    Mu_d0pvtx = 0;
    Mu_sigd0 = 0;
    Mu_z0sinTheta = 0;
    Mu_isBad = 0;
    Mu_passOR = 0;
    Mu_isTight = 0;
    Mu_isCosmic = 0;
    //#ifdef _IS_MC_
    Mu_type = 0;
    Mu_origin = 0;
    //#endif // #ifdef _IS_MC_
    Mu_ptcone20 = 0;
    Mu_ptcone30 = 0;
    Mu_ptcone40 = 0;
    Mu_ptvarcone20 = 0;
    Mu_ptvarcone30 = 0;
    Mu_ptvarcone40 = 0;
    Mu_topoetcone20 = 0;
    Mu_topoetcone30 = 0;
    Mu_topoetcone40 = 0;
    Mu_passIsoLooseTO = 0;
    Mu_passIsoLoose = 0;
    Mu_passIsoTight = 0;
    Mu_passIsoGrad = 0;
    Mu_passIsoGradCustomTight = 0;
    Mu_passIsoGradCustom = 0;
    Mu_passIsoGradLoose = 0;
    Mu_trigMatch_mu26_imedium = 0;
    Mu_trigMatch_mu50 = 0;
    Mu_trigMatch_mu8noL1 = 0;
    Mu_trigMatch_mu14 = 0;
    Mu_trigMatch_mu18 = 0;
    Mu_trigMatch_mu18_mu8noL1 = 0;
    Mu_trigMatch_e17_lhloose_mu14 = 0;
    Mu_trigMatch_e17_lhloose_nod0_mu14 = 0;
    Mu_trigMatch_mu20_mu8noL1 = 0;
    Mu_trigMatch_mu22_mu8noL1 = 0;
    Mu_TrigMatch_mu24_iloose = 0;
    Mu_TrigMatch_mu24_ivarloose = 0;
    Mu_TrigMatch_mu24_iloose_L1MU15 = 0;
    Mu_TrigMatch_mu24_ivarloose_L1MU15 = 0;
    Mu_trigMatchPair_mu18_mu8noL1 = 0;
    Mu_trigMatchPair_mu20_mu8noL1 = 0;
    Mu_trigMatchPair_mu22_mu8noL1 = 0;
    El_eta = 0;
    El_etaclus = 0;
    El_phi = 0;
    El_pT = 0;
    El_E = 0;
    El_charge = 0;
    El_sigd0 = 0;
    El_z0sinTheta = 0;
    El_d0pvtx = 0;
    El_passOR = 0;
    El_SFwMediumLH = 0;
    El_IsoSFwMediumLH = 0;
    El_SFwTightLH = 0;
    El_SFwLooseAndBLayerLH = 0;
    El_SFwTrigMediumLH_e12_lhloose_L1EM10VH = 0;
    El_SFwTrigMediumLH_e17_lhloose = 0;
    El_SFwTrigMediumLH_single = 0;
    El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH = 0;
    El_isLooseAndBLayerLH = 0;
    El_isMediumLH = 0;
    El_isTightLH = 0;
    El_nBLayerHits = 0;
    El_expectBLayerHit = 0;
    //#ifdef _IS_MC_
    El_type = 0;
    El_origin = 0;
    El_bkgMotherPdgId = 0;
    El_bkgOrigin = 0;
    El_chFlip = 0;
    //#endif // #ifdef _IS_MC_
    El_ptcone20 = 0;
    El_ptcone30 = 0;
    El_ptcone40 = 0;
    El_ptvarcone20 = 0;
    El_ptvarcone30 = 0;
    El_ptvarcone40 = 0;
    El_topoetcone20 = 0;
    El_topoetcone30 = 0;
    El_topoetcone40 = 0;
    El_passIsoLooseTO = 0;
    El_passIsoLoose = 0;
    El_passIsoTight = 0;
    El_passIsoGrad = 0;
    El_passIsoGradCustomTight = 0;
    El_passIsoGradCustom = 0;
    El_passIsoGradLoose = 0;
    El_trigMatch_e12_lhloose_L1EM10VH = 0;
    El_trigMatch_e17_lhloose = 0;
    El_trigMatch_e60_lhmedium = 0;
    El_trigMatch_e24_lhmedium_iloose_L1EM20VH = 0;
    El_trigMatch_2e12_lhloose_L12EM10VH = 0;
    El_trigMatch_2e15_lhloose_L12EM10VH = 0;
    El_trigMatch_2e15_lhvloose_L12EM13VH = 0;
    El_trigMatch_2e15_lhvloose_nod0_L12EM13VH = 0;
    El_trigMatch_2e17_lhvloose_nod0 = 0;
    El_trigMatch_e17_lhloose_mu14 = 0;
    El_trigMatch_e17_lhloose_nod0_mu14 = 0;
    El_TrigMatch_e24_lhmedium_nod0_ivarloose = 0;
    El_TrigMatch_e24_lhtight_nod0_ivarloose = 0;
    El_TrigMatch_e60_lhmedium_nod0 = 0;
    Jet_eta = 0;
    Jet_phi = 0;
    Jet_pT = 0;
    Jet_E = 0;
    Jet_quality = 0;
    Jet_JVT = 0;
    Jet_JVTsf = 0;
    Jet_MV2c20 = 0;
    Jet_MV2c10 = 0;
    Jet_SFw = 0;
    //#ifdef _IS_MC_
    Jet_ConeTruthLabel = 0;
    Jet_PartonTruthLabel = 0;
    Jet_HadronConeExclTruthLabel = 0;
    Jet_deltaR = 0;
    //#endif // #ifdef _IS_MC_
    Jet_nTrk = 0;
    Jet_passOR = 0;
    //#ifdef _IS_MC_
    TruthJet_eta = 0;
    TruthJet_phi = 0;
    TruthJet_pT = 0;
    TruthJet_E = 0;
    TruthJet_ConeTruthLabel = 0;
    TruthJet_PartonTruthLabel = 0;
    TruthL_eta = 0;
    TruthL_phi = 0;
    TruthL_pT = 0;
    TruthL_id = 0;
    //#endif // #ifdef _IS_MC_

    // Set branch addresses and branch pointers
    fChain = wk()->tree();
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("HLT_e24_lhmedium_nod0_ivarloose", &HLT_e24_lhmedium_nod0_ivarloose, &b_HLT_e24_lhmedium_nod0_ivarloose);
    fChain->SetBranchAddress("HLT_e24_lhtight_nod0_ivarloose", &HLT_e24_lhtight_nod0_ivarloose, &b_HLT_e24_lhtight_nod0_ivarloose);
    fChain->SetBranchAddress("HLT_e24_lhmedium_nod0_L1EM20VH", &HLT_e24_lhmedium_nod0_L1EM20VH, &b_HLT_e24_lhmedium_nod0_L1EM20VH);
    fChain->SetBranchAddress("HLT_e26_lhtight_iloose", &HLT_e26_lhtight_iloose, &b_HLT_e26_lhtight_iloose);
    fChain->SetBranchAddress("HLT_e26_lhtight_ivarloose", &HLT_e26_lhtight_ivarloose, &b_HLT_e26_lhtight_ivarloose);
    fChain->SetBranchAddress("HLT_e26_lhtight_nod0_iloose", &HLT_e26_lhtight_nod0_iloose, &b_HLT_e26_lhtight_nod0_iloose);
    fChain->SetBranchAddress("HLT_e26_lhtight_nod0_ivarloose", &HLT_e26_lhtight_nod0_ivarloose, &b_HLT_e26_lhtight_nod0_ivarloose);
    fChain->SetBranchAddress("HLT_e60_lhmedium", &HLT_e60_lhmedium, &b_HLT_e60_lhmedium);
    fChain->SetBranchAddress("HLT_e60_lhmedium_nod0", &HLT_e60_lhmedium_nod0, &b_HLT_e60_lhmedium_nod0);
    fChain->SetBranchAddress("HLT_e120_lhloose_nod0", &HLT_e120_lhloose_nod0, &b_HLT_e120_lhloose_nod0);
    fChain->SetBranchAddress("HLT_e140_lhloose_nod0", &HLT_e140_lhloose_nod0, &b_HLT_e140_lhloose_nod0);
    fChain->SetBranchAddress("HLT_2e17_lhvloose_nod0", &HLT_2e17_lhvloose_nod0, &b_HLT_2e17_lhvloose_nod0);
    fChain->SetBranchAddress("HLT_2e15_lhvloose_nod0_L12EM13VH", &HLT_2e15_lhvloose_nod0_L12EM13VH, &b_HLT_2e15_lhvloose_nod0_L12EM13VH);
    fChain->SetBranchAddress("HLT_e24_lhmedium_e9_lhmedium", &HLT_e24_lhmedium_e9_lhmedium, &b_HLT_e24_lhmedium_e9_lhmedium);
    fChain->SetBranchAddress("HLT_e24_lhmedium_L1EM20VH", &HLT_e24_lhmedium_L1EM20VH, &b_HLT_e24_lhmedium_L1EM20VH);
    fChain->SetBranchAddress("HLT_e12_lhvloose_L12EM10VH", &HLT_e12_lhvloose_L12EM10VH, &b_HLT_e12_lhvloose_L12EM10VH);
    fChain->SetBranchAddress("HLT_e17_lhloose_2e9_lhloose", &HLT_e17_lhloose_2e9_lhloose, &b_HLT_e17_lhloose_2e9_lhloose);
    fChain->SetBranchAddress("HLT_mu24_ivarmedium", &HLT_mu24_ivarmedium, &b_HLT_mu24_ivarmedium);
    fChain->SetBranchAddress("HLT_mu24_imedium", &HLT_mu24_imedium, &b_HLT_mu24_imedium);
    fChain->SetBranchAddress("HLT_mu24_ivarloose", &HLT_mu24_ivarloose, &b_HLT_mu24_ivarloose);
    fChain->SetBranchAddress("HLT_mu24_iloose", &HLT_mu24_iloose, &b_HLT_mu24_iloose);
    fChain->SetBranchAddress("HLT_mu26_ivarmedium", &HLT_mu26_ivarmedium, &b_HLT_mu26_ivarmedium);
    fChain->SetBranchAddress("HLT_mu26_imedium", &HLT_mu26_imedium, &b_HLT_mu26_imedium);
    fChain->SetBranchAddress("HLT_mu20_ivarmedium_L1MU15", &HLT_mu20_ivarmedium_L1MU15, &b_HLT_mu20_ivarmedium_L1MU15);
    fChain->SetBranchAddress("HLT_mu20_imedium_L1MU15", &HLT_mu20_imedium_L1MU15, &b_HLT_mu20_imedium_L1MU15);
    fChain->SetBranchAddress("HLT_mu20_ivarloose_L1MU15", &HLT_mu20_ivarloose_L1MU15, &b_HLT_mu20_ivarloose_L1MU15);
    fChain->SetBranchAddress("HLT_mu20_iloose_L1MU15", &HLT_mu20_iloose_L1MU15, &b_HLT_mu20_iloose_L1MU15);
    fChain->SetBranchAddress("HLT_mu20_L1MU15", &HLT_mu20_L1MU15, &b_HLT_mu20_L1MU15);
    fChain->SetBranchAddress("HLT_mu20_mu8noL1", &HLT_mu20_mu8noL1, &b_HLT_mu20_mu8noL1);
    fChain->SetBranchAddress("HLT_mu22_mu8noL1", &HLT_mu22_mu8noL1, &b_HLT_mu22_mu8noL1);
    fChain->SetBranchAddress("HLT_mu20_2mu4noL1", &HLT_mu20_2mu4noL1, &b_HLT_mu20_2mu4noL1);
    fChain->SetBranchAddress("HLT_mu22_2mu4noL1", &HLT_mu22_2mu4noL1, &b_HLT_mu22_2mu4noL1);
    fChain->SetBranchAddress("HLT_mu40", &HLT_mu40, &b_HLT_mu40);
    fChain->SetBranchAddress("HLT_mu50", &HLT_mu50, &b_HLT_mu50);
    fChain->SetBranchAddress("HLT_2mu10", &HLT_2mu10, &b_HLT_2mu10);
    fChain->SetBranchAddress("HLT_2mu10_nomucomb", &HLT_2mu10_nomucomb, &b_HLT_2mu10_nomucomb);
    fChain->SetBranchAddress("HLT_2mu14", &HLT_2mu14, &b_HLT_2mu14);
    fChain->SetBranchAddress("HLT_2mu14_nomucomb", &HLT_2mu14_nomucomb, &b_HLT_2mu14_nomucomb);
    fChain->SetBranchAddress("HLT_3mu6", &HLT_3mu6, &b_HLT_3mu6);
    fChain->SetBranchAddress("HLT_3mu6_msonly", &HLT_3mu6_msonly, &b_HLT_3mu6_msonly);
    fChain->SetBranchAddress("HLT_xe100_L1XE50", &HLT_xe100_L1XE50, &b_HLT_xe100_L1XE50);
    fChain->SetBranchAddress("HLT_xe80_mht_L1XE50", &HLT_xe80_mht_L1XE50, &b_HLT_xe80_mht_L1XE50);
    fChain->SetBranchAddress("HLT_xe90_mht_L1XE50", &HLT_xe90_mht_L1XE50, &b_HLT_xe90_mht_L1XE50);
    fChain->SetBranchAddress("HLT_xe100_mht_L1XE50", &HLT_xe100_mht_L1XE50, &b_HLT_xe100_mht_L1XE50);
    fChain->SetBranchAddress("HLT_xe110_pueta_L1XE50", &HLT_xe110_pueta_L1XE50, &b_HLT_xe110_pueta_L1XE50);
    fChain->SetBranchAddress("HLT_xe110_pufit_L1XE50", &HLT_xe110_pufit_L1XE50, &b_HLT_xe110_pufit_L1XE50);
    fChain->SetBranchAddress("HLT_xe100_tc_em_L1XE50", &HLT_xe100_tc_em_L1XE50, &b_HLT_xe100_tc_em_L1XE50);
    fChain->SetBranchAddress("HLT_xe80_tc_lcw_L1XE50", &HLT_xe80_tc_lcw_L1XE50, &b_HLT_xe80_tc_lcw_L1XE50);
    fChain->SetBranchAddress("HLT_xe90_tc_lcw_L1XE50", &HLT_xe90_tc_lcw_L1XE50, &b_HLT_xe90_tc_lcw_L1XE50);
    fChain->SetBranchAddress("HLT_xe80_tc_lcw_wEFMu_L1XE50", &HLT_xe80_tc_lcw_wEFMu_L1XE50, &b_HLT_xe80_tc_lcw_wEFMu_L1XE50);
    fChain->SetBranchAddress("HLT_e7_lhmedium_mu24", &HLT_e7_lhmedium_mu24, &b_HLT_e7_lhmedium_mu24);
    fChain->SetBranchAddress("HLT_e17_lhloose_nod0_mu14", &HLT_e17_lhloose_nod0_mu14, &b_HLT_e17_lhloose_nod0_mu14);
    fChain->SetBranchAddress("HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1", &HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1, &b_HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1);
    fChain->SetBranchAddress("HLT_e24_lhmedium_nod0_L1EM22VHI_mu8noL1", &HLT_e24_lhmedium_nod0_L1EM22VHI_mu8noL1, &b_HLT_e24_lhmedium_nod0_L1EM22VHI_mu8noL1);
    fChain->SetBranchAddress("HLT_2e12_lhloose_L12EM10VH", &HLT_2e12_lhloose_L12EM10VH, &b_HLT_2e12_lhloose_L12EM10VH);
    fChain->SetBranchAddress("HLT_e17_lhloose_mu14", &HLT_e17_lhloose_mu14, &b_HLT_e17_lhloose_mu14);
    fChain->SetBranchAddress("HLT_mu18_mu8noL1", &HLT_mu18_mu8noL1, &b_HLT_mu18_mu8noL1);
    fChain->SetBranchAddress("HLT_xe70", &HLT_xe70, &b_HLT_xe70);
    fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
    fChain->SetBranchAddress("ChannelNumber", &ChannelNumber, &b_ChannelNumber);
    fChain->SetBranchAddress("AvgMu", &AvgMu, &b_AvgMu);
    fChain->SetBranchAddress("EventWeight", &EventWeight, &b_EventWeight);
    fChain->SetBranchAddress("PRWWeight", &PRWWeight, &b_PRWWeight);
    fChain->SetBranchAddress("PRWrandomRunNumber", &PRWrandomRunNumber, &b_PRWrandomRunNumber);
    fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
    fChain->SetBranchAddress("LB", &LB, &b_LB);
    fChain->SetBranchAddress("RunNb", &RunNb, &b_RunNb);
    fChain->SetBranchAddress("DetError", &DetError, &b_DetError);
    fChain->SetBranchAddress("NMu", &NMu, &b_NMu);
    fChain->SetBranchAddress("Mu_eta", &Mu_eta, &b_Mu_eta);
    fChain->SetBranchAddress("Mu_phi", &Mu_phi, &b_Mu_phi);
    fChain->SetBranchAddress("Mu_pT", &Mu_pT, &b_Mu_pT);
    fChain->SetBranchAddress("Mu_SFw", &Mu_SFw, &b_Mu_SFw);
    fChain->SetBranchAddress("Mu_IsoSFw", &Mu_IsoSFw, &b_Mu_IsoSFw);
    fChain->SetBranchAddress("Mu_charge", &Mu_charge, &b_Mu_charge);
    fChain->SetBranchAddress("Mu_d0pvtx", &Mu_d0pvtx, &b_Mu_d0pvtx);
    fChain->SetBranchAddress("Mu_sigd0", &Mu_sigd0, &b_Mu_sigd0);
    fChain->SetBranchAddress("Mu_z0sinTheta", &Mu_z0sinTheta, &b_Mu_z0sinTheta);
    fChain->SetBranchAddress("Mu_isBad", &Mu_isBad, &b_Mu_isBad);
    fChain->SetBranchAddress("Mu_passOR", &Mu_passOR, &b_Mu_passOR);
    fChain->SetBranchAddress("Mu_isTight", &Mu_isTight, &b_Mu_isTight);
    fChain->SetBranchAddress("Mu_isCosmic", &Mu_isCosmic, &b_Mu_isCosmic);
    //#ifdef _IS_MC_
    fChain->SetBranchAddress("Mu_type", &Mu_type, &b_Mu_type);
    fChain->SetBranchAddress("Mu_origin", &Mu_origin, &b_Mu_origin);
    //#endif // #ifdef _IS_MC_
    fChain->SetBranchAddress("Mu_ptcone20", &Mu_ptcone20, &b_Mu_ptcone20);
    fChain->SetBranchAddress("Mu_ptcone30", &Mu_ptcone30, &b_Mu_ptcone30);
    fChain->SetBranchAddress("Mu_ptcone40", &Mu_ptcone40, &b_Mu_ptcone40);
    fChain->SetBranchAddress("Mu_ptvarcone20", &Mu_ptvarcone20, &b_Mu_ptvarcone20);
    fChain->SetBranchAddress("Mu_ptvarcone30", &Mu_ptvarcone30, &b_Mu_ptvarcone30);
    fChain->SetBranchAddress("Mu_ptvarcone40", &Mu_ptvarcone40, &b_Mu_ptvarcone40);
    fChain->SetBranchAddress("Mu_topoetcone20", &Mu_topoetcone20, &b_Mu_topoetcone20);
    fChain->SetBranchAddress("Mu_topoetcone30", &Mu_topoetcone30, &b_Mu_topoetcone30);
    fChain->SetBranchAddress("Mu_topoetcone40", &Mu_topoetcone40, &b_Mu_topoetcone40);
    fChain->SetBranchAddress("Mu_passIsoLooseTO", &Mu_passIsoLooseTO, &b_Mu_passIsoLooseTO);
    fChain->SetBranchAddress("Mu_passIsoLoose", &Mu_passIsoLoose, &b_Mu_passIsoLoose);
    fChain->SetBranchAddress("Mu_passIsoTight", &Mu_passIsoTight, &b_Mu_passIsoTight);
    fChain->SetBranchAddress("Mu_passIsoGrad", &Mu_passIsoGrad, &b_Mu_passIsoGrad);
    fChain->SetBranchAddress("Mu_passIsoGradCustomTight", &Mu_passIsoGradCustomTight, &b_Mu_passIsoGradCustomTight);
    fChain->SetBranchAddress("Mu_passIsoGradCustom", &Mu_passIsoGradCustom, &b_Mu_passIsoGradCustom);
    fChain->SetBranchAddress("Mu_passIsoGradLoose", &Mu_passIsoGradLoose, &b_Mu_passIsoGradLoose);
    fChain->SetBranchAddress("MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50", &MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50, &b_MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50);
    fChain->SetBranchAddress("Mu_trigMatch_mu26_imedium", &Mu_trigMatch_mu26_imedium, &b_Mu_trigMatch_mu26_imedium);
    fChain->SetBranchAddress("Mu_trigMatch_mu50", &Mu_trigMatch_mu50, &b_Mu_trigMatch_mu50);
    fChain->SetBranchAddress("Mu_trigMatch_mu8noL1", &Mu_trigMatch_mu8noL1, &b_Mu_trigMatch_mu8noL1);
    fChain->SetBranchAddress("Mu_trigMatch_mu14", &Mu_trigMatch_mu14, &b_Mu_trigMatch_mu14);
    fChain->SetBranchAddress("Mu_trigMatch_mu18", &Mu_trigMatch_mu18, &b_Mu_trigMatch_mu18);
    fChain->SetBranchAddress("Mu_trigMatch_mu18_mu8noL1", &Mu_trigMatch_mu18_mu8noL1, &b_Mu_trigMatch_mu18_mu8noL1);
    fChain->SetBranchAddress("Mu_trigMatch_e17_lhloose_mu14", &Mu_trigMatch_e17_lhloose_mu14, &b_Mu_trigMatch_e17_lhloose_mu14);
    fChain->SetBranchAddress("Mu_trigMatch_e17_lhloose_nod0_mu14", &Mu_trigMatch_e17_lhloose_nod0_mu14, &b_Mu_trigMatch_e17_lhloose_nod0_mu14);
    fChain->SetBranchAddress("Mu_trigMatch_mu20_mu8noL1", &Mu_trigMatch_mu20_mu8noL1, &b_Mu_trigMatch_mu20_mu8noL1);
    fChain->SetBranchAddress("Mu_trigMatch_mu22_mu8noL1", &Mu_trigMatch_mu22_mu8noL1, &b_Mu_trigMatch_mu22_mu8noL1);
    fChain->SetBranchAddress("Mu_TrigMatch_mu24_iloose", &Mu_TrigMatch_mu24_iloose, &b_Mu_TrigMatch_mu24_iloose);
    fChain->SetBranchAddress("Mu_TrigMatch_mu24_ivarloose", &Mu_TrigMatch_mu24_ivarloose, &b_Mu_TrigMatch_mu24_ivarloose);
    fChain->SetBranchAddress("Mu_TrigMatch_mu24_iloose_L1MU15", &Mu_TrigMatch_mu24_iloose_L1MU15, &b_Mu_TrigMatch_mu24_iloose_L1MU15);
    fChain->SetBranchAddress("Mu_TrigMatch_mu24_ivarloose_L1MU15", &Mu_TrigMatch_mu24_ivarloose_L1MU15, &b_Mu_TrigMatch_mu24_ivarloose_L1MU15);
    fChain->SetBranchAddress("Mu_trigMatchPair_mu18_mu8noL1", &Mu_trigMatchPair_mu18_mu8noL1, &b_Mu_trigMatchPair_mu18_mu8noL1);
    fChain->SetBranchAddress("Mu_trigMatchPair_mu20_mu8noL1", &Mu_trigMatchPair_mu20_mu8noL1, &b_Mu_trigMatchPair_mu20_mu8noL1);
    fChain->SetBranchAddress("Mu_trigMatchPair_mu22_mu8noL1", &Mu_trigMatchPair_mu22_mu8noL1, &b_Mu_trigMatchPair_mu22_mu8noL1);
    fChain->SetBranchAddress("NEl", &NEl, &b_NEl);
    fChain->SetBranchAddress("El_eta", &El_eta, &b_El_eta);
    fChain->SetBranchAddress("El_etaclus", &El_etaclus, &b_El_etaclus);
    fChain->SetBranchAddress("El_phi", &El_phi, &b_El_phi);
    fChain->SetBranchAddress("El_pT", &El_pT, &b_El_pT);
    fChain->SetBranchAddress("El_E", &El_E, &b_El_E);
    fChain->SetBranchAddress("El_charge", &El_charge, &b_El_charge);
    fChain->SetBranchAddress("El_sigd0", &El_sigd0, &b_El_sigd0);
    fChain->SetBranchAddress("El_z0sinTheta", &El_z0sinTheta, &b_El_z0sinTheta);
    fChain->SetBranchAddress("El_d0pvtx", &El_d0pvtx, &b_El_d0pvtx);
    fChain->SetBranchAddress("El_passOR", &El_passOR, &b_El_passOR);
    fChain->SetBranchAddress("El_SFwMediumLH", &El_SFwMediumLH, &b_El_SFwMediumLH);
    fChain->SetBranchAddress("El_IsoSFwMediumLH", &El_IsoSFwMediumLH, &b_El_IsoSFwMediumLH);
    fChain->SetBranchAddress("El_SFwTightLH", &El_SFwTightLH, &b_El_SFwTightLH);
    fChain->SetBranchAddress("El_SFwLooseAndBLayerLH", &El_SFwLooseAndBLayerLH, &b_El_SFwLooseAndBLayerLH);
    fChain->SetBranchAddress("El_SFwTrigMediumLH_e12_lhloose_L1EM10VH", &El_SFwTrigMediumLH_e12_lhloose_L1EM10VH, &b_El_SFwTrigMediumLH_e12_lhloose_L1EM10VH);
    fChain->SetBranchAddress("El_SFwTrigMediumLH_e17_lhloose", &El_SFwTrigMediumLH_e17_lhloose, &b_El_SFwTrigMediumLH_e17_lhloose);
    fChain->SetBranchAddress("El_SFwTrigMediumLH_single", &El_SFwTrigMediumLH_single, &b_El_SFwTrigMediumLH_single);
    fChain->SetBranchAddress("El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH", &El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH, &b_El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH);
    fChain->SetBranchAddress("El_isLooseAndBLayerLH", &El_isLooseAndBLayerLH, &b_El_isLooseAndBLayerLH);
    fChain->SetBranchAddress("El_isMediumLH", &El_isMediumLH, &b_El_isMediumLH);
    fChain->SetBranchAddress("El_isTightLH", &El_isTightLH, &b_El_isTightLH);
    fChain->SetBranchAddress("El_nBLayerHits", &El_nBLayerHits, &b_El_nBLayerHits);
    fChain->SetBranchAddress("El_expectBLayerHit", &El_expectBLayerHit, &b_El_expectBLayerHit);
    //#ifdef _IS_MC_
    fChain->SetBranchAddress("El_type", &El_type, &b_El_type);
    fChain->SetBranchAddress("El_origin", &El_origin, &b_El_origin);
    fChain->SetBranchAddress("El_bkgMotherPdgId", &El_bkgMotherPdgId, &b_El_bkgMotherPdgId);
    fChain->SetBranchAddress("El_bkgOrigin", &El_bkgOrigin, &b_El_bkgOrigin);
    fChain->SetBranchAddress("El_chFlip", &El_chFlip, &b_El_chFlip);
    //#endif // #ifdef _IS_MC_
    fChain->SetBranchAddress("El_ptcone20", &El_ptcone20, &b_El_ptcone20);
    fChain->SetBranchAddress("El_ptcone30", &El_ptcone30, &b_El_ptcone30);
    fChain->SetBranchAddress("El_ptcone40", &El_ptcone40, &b_El_ptcone40);
    fChain->SetBranchAddress("El_ptvarcone20", &El_ptvarcone20, &b_El_ptvarcone20);
    fChain->SetBranchAddress("El_ptvarcone30", &El_ptvarcone30, &b_El_ptvarcone30);
    fChain->SetBranchAddress("El_ptvarcone40", &El_ptvarcone40, &b_El_ptvarcone40);
    fChain->SetBranchAddress("El_topoetcone20", &El_topoetcone20, &b_El_topoetcone20);
    fChain->SetBranchAddress("El_topoetcone30", &El_topoetcone30, &b_El_topoetcone30);
    fChain->SetBranchAddress("El_topoetcone40", &El_topoetcone40, &b_El_topoetcone40);
    fChain->SetBranchAddress("El_passIsoLooseTO", &El_passIsoLooseTO, &b_El_passIsoLooseTO);
    fChain->SetBranchAddress("El_passIsoLoose", &El_passIsoLoose, &b_El_passIsoLoose);
    fChain->SetBranchAddress("El_passIsoTight", &El_passIsoTight, &b_El_passIsoTight);
    fChain->SetBranchAddress("El_passIsoGrad", &El_passIsoGrad, &b_El_passIsoGrad);
    fChain->SetBranchAddress("El_passIsoGradCustomTight", &El_passIsoGradCustomTight, &b_El_passIsoGradCustomTight);
    fChain->SetBranchAddress("El_passIsoGradCustom", &El_passIsoGradCustom, &b_El_passIsoGradCustom);
    fChain->SetBranchAddress("El_passIsoGradLoose", &El_passIsoGradLoose, &b_El_passIsoGradLoose);
    fChain->SetBranchAddress("El_trigMatch_e12_lhloose_L1EM10VH", &El_trigMatch_e12_lhloose_L1EM10VH, &b_El_trigMatch_e12_lhloose_L1EM10VH);
    fChain->SetBranchAddress("El_trigMatch_e17_lhloose", &El_trigMatch_e17_lhloose, &b_El_trigMatch_e17_lhloose);
    fChain->SetBranchAddress("El_trigMatch_e60_lhmedium", &El_trigMatch_e60_lhmedium, &b_El_trigMatch_e60_lhmedium);
    fChain->SetBranchAddress("El_trigMatch_e24_lhmedium_iloose_L1EM20VH", &El_trigMatch_e24_lhmedium_iloose_L1EM20VH, &b_El_trigMatch_e24_lhmedium_iloose_L1EM20VH);
    fChain->SetBranchAddress("El_trigMatch_2e12_lhloose_L12EM10VH", &El_trigMatch_2e12_lhloose_L12EM10VH, &b_El_trigMatch_2e12_lhloose_L12EM10VH);
    fChain->SetBranchAddress("El_trigMatch_2e15_lhloose_L12EM10VH", &El_trigMatch_2e15_lhloose_L12EM10VH, &b_El_trigMatch_2e15_lhloose_L12EM10VH);
    fChain->SetBranchAddress("El_trigMatch_2e15_lhvloose_L12EM13VH", &El_trigMatch_2e15_lhvloose_L12EM13VH, &b_El_trigMatch_2e15_lhvloose_L12EM13VH);
    fChain->SetBranchAddress("El_trigMatch_2e15_lhvloose_nod0_L12EM13VH", &El_trigMatch_2e15_lhvloose_nod0_L12EM13VH, &b_El_trigMatch_2e15_lhvloose_nod0_L12EM13VH);
    fChain->SetBranchAddress("El_trigMatch_2e17_lhvloose_nod0", &El_trigMatch_2e17_lhvloose_nod0, &b_El_trigMatch_2e17_lhvloose_nod0);
    fChain->SetBranchAddress("El_trigMatch_e17_lhloose_mu14", &El_trigMatch_e17_lhloose_mu14, &b_El_trigMatch_e17_lhloose_mu14);
    fChain->SetBranchAddress("El_trigMatch_e17_lhloose_nod0_mu14", &El_trigMatch_e17_lhloose_nod0_mu14, &b_El_trigMatch_e17_lhloose_nod0_mu14);
    fChain->SetBranchAddress("El_TrigMatch_e24_lhmedium_nod0_ivarloose", &El_TrigMatch_e24_lhmedium_nod0_ivarloose, &b_El_TrigMatch_e24_lhmedium_nod0_ivarloose);
    fChain->SetBranchAddress("El_TrigMatch_e24_lhtight_nod0_ivarloose", &El_TrigMatch_e24_lhtight_nod0_ivarloose, &b_El_TrigMatch_e24_lhtight_nod0_ivarloose);
    fChain->SetBranchAddress("El_TrigMatch_e60_lhmedium_nod0", &El_TrigMatch_e60_lhmedium_nod0, &b_El_TrigMatch_e60_lhmedium_nod0);
    fChain->SetBranchAddress("NJet", &NJet, &b_NJet);
    fChain->SetBranchAddress("Jet_eta", &Jet_eta, &b_Jet_eta);
    fChain->SetBranchAddress("Jet_phi", &Jet_phi, &b_Jet_phi);
    fChain->SetBranchAddress("Jet_pT", &Jet_pT, &b_Jet_pT);
    fChain->SetBranchAddress("Jet_E", &Jet_E, &b_Jet_E);
    fChain->SetBranchAddress("Jet_quality", &Jet_quality, &b_Jet_quality);
    fChain->SetBranchAddress("Jet_JVT", &Jet_JVT, &b_Jet_JVT);
    fChain->SetBranchAddress("Jet_JVTsf", &Jet_JVTsf, &b_Jet_JVTsf);
    fChain->SetBranchAddress("Jet_MV2c20", &Jet_MV2c20, &b_Jet_MV2c20);
    fChain->SetBranchAddress("Jet_MV2c10", &Jet_MV2c10, &b_Jet_MV2c10);
    fChain->SetBranchAddress("Jet_SFw", &Jet_SFw, &b_Jet_SFw);
    //#ifdef _IS_MC_
    fChain->SetBranchAddress("Jet_ConeTruthLabel", &Jet_ConeTruthLabel, &b_Jet_ConeTruthLabel);
    fChain->SetBranchAddress("Jet_PartonTruthLabel", &Jet_PartonTruthLabel, &b_Jet_PartonTruthLabel);
    fChain->SetBranchAddress("Jet_HadronConeExclTruthLabel", &Jet_HadronConeExclTruthLabel, &b_Jet_HadronConeExclTruthLabel);
    fChain->SetBranchAddress("Jet_deltaR", &Jet_deltaR, &b_Jet_deltaR);
    //#endif // #ifdef _IS_MC_
    fChain->SetBranchAddress("Jet_nTrk", &Jet_nTrk, &b_Jet_nTrk);
    fChain->SetBranchAddress("Jet_passOR", &Jet_passOR, &b_Jet_passOR);
    fChain->SetBranchAddress("Etmiss_CST_Etx", &Etmiss_CST_Etx, &b_Etmiss_CST_Etx);
    fChain->SetBranchAddress("Etmiss_CST_Ety", &Etmiss_CST_Ety, &b_Etmiss_CST_Ety);
    fChain->SetBranchAddress("Etmiss_CST_Et", &Etmiss_CST_Et, &b_Etmiss_CST_Et);
    fChain->SetBranchAddress("Etmiss_TST_Etx", &Etmiss_TST_Etx, &b_Etmiss_TST_Etx);
    fChain->SetBranchAddress("Etmiss_TST_Ety", &Etmiss_TST_Ety, &b_Etmiss_TST_Ety);
    fChain->SetBranchAddress("Etmiss_TST_Et", &Etmiss_TST_Et, &b_Etmiss_TST_Et);
    fChain->SetBranchAddress("Etmiss_TSTterm_Etx", &Etmiss_TSTterm_Etx, &b_Etmiss_TSTterm_Etx);
    fChain->SetBranchAddress("Etmiss_TSTterm_Ety", &Etmiss_TSTterm_Ety, &b_Etmiss_TSTterm_Ety);
    fChain->SetBranchAddress("Etmiss_TSTterm_Et", &Etmiss_TSTterm_Et, &b_Etmiss_TSTterm_Et);
    fChain->SetBranchAddress("Etmiss_Truth_Etx", &Etmiss_Truth_Etx, &b_Etmiss_Truth_Etx);
    fChain->SetBranchAddress("Etmiss_Truth_Ety", &Etmiss_Truth_Ety, &b_Etmiss_Truth_Ety);
    fChain->SetBranchAddress("Etmiss_Truth_Et", &Etmiss_Truth_Et, &b_Etmiss_Truth_Et);
    //#ifdef _IS_MC_
    fChain->SetBranchAddress("NTruthJet", &NTruthJet, &b_NTruthJet);
    fChain->SetBranchAddress("TruthJet_eta", &TruthJet_eta, &b_TruthJet_eta);
    fChain->SetBranchAddress("TruthJet_phi", &TruthJet_phi, &b_TruthJet_phi);
    fChain->SetBranchAddress("TruthJet_pT", &TruthJet_pT, &b_TruthJet_pT);
    fChain->SetBranchAddress("TruthJet_E", &TruthJet_E, &b_TruthJet_E);
    fChain->SetBranchAddress("TruthJet_ConeTruthLabel", &TruthJet_ConeTruthLabel, &b_TruthJet_ConeTruthLabel);
    fChain->SetBranchAddress("TruthJet_PartonTruthLabel", &TruthJet_PartonTruthLabel, &b_TruthJet_PartonTruthLabel);
    fChain->SetBranchAddress("NTruthL", &NTruthL, &b_NTruthL);
    fChain->SetBranchAddress("TruthL_eta", &TruthL_eta, &b_TruthL_eta);
    fChain->SetBranchAddress("TruthL_phi", &TruthL_phi, &b_TruthL_phi);
    fChain->SetBranchAddress("TruthL_pT", &TruthL_pT, &b_TruthL_pT);
    fChain->SetBranchAddress("TruthL_id", &TruthL_id, &b_TruthL_id);
    fChain->SetBranchAddress("SUSY_Spart_pdgId1", &SUSY_Spart_pdgId1, &b_SUSY_Spart_pdgId1);
    fChain->SetBranchAddress("SUSY_Spart_pdgId2", &SUSY_Spart_pdgId2, &b_SUSY_Spart_pdgId2);
    fChain->SetBranchAddress("SUSY_Gluino_decay1", &SUSY_Gluino_decay1, &b_SUSY_Gluino_decay1);
    fChain->SetBranchAddress("SUSY_Gluino_decay2", &SUSY_Gluino_decay2, &b_SUSY_Gluino_decay2);
    fChain->SetBranchAddress("GenFiltHT", &GenFiltHT, &b_GenFiltHT);
    fChain->SetBranchAddress("GenFiltMET", &GenFiltMET, &b_GenFiltMET);
    //#endif // #ifdef _IS_MC_
    fChain->SetBranchAddress("PV_z", &PV_z, &b_PV_z);
    fChain->SetBranchAddress("Nvtx", &Nvtx, &b_Nvtx);
    fChain->SetBranchAddress("TruthX1", &TruthX1, &b_TruthX1);
    fChain->SetBranchAddress("TruthX2", &TruthX2, &b_TruthX2);
    fChain->SetBranchAddress("TruthQ", &TruthQ, &b_TruthQ);
    fChain->SetBranchAddress("SherpaNjetWeight", &SherpaNjetWeight, &b_SherpaNjetWeight);
    fChain->SetBranchAddress("TruthPDGID1", &TruthPDGID1, &b_TruthPDGID1);
    fChain->SetBranchAddress("TruthPDGID2", &TruthPDGID2, &b_TruthPDGID2);

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::initialize ()
{
    // Here you do everything that you need to do after the first input
    // file has been connected and before the first event is processed,
    // e.g. create additional histograms based on which variables are
    // available in the input files.  You can also create all of your
    // histograms and trees in here, but be aware that this method
    // doesn't get called if no events are processed.  So any objects
    // you create here won't be available in the output if you have no
    // input events.

    //const char * function_name = "initialize()";
    //Info(function_name, "Function calls");

    //if (isSkim) {
        //m_skim->initialize(wk()->tree(), process);
    //}
    if (isOptimization) {
        m_optimization->set_isMC(isMC);
        m_optimization->set_isData(isData);
        m_optimization->set_process(process);
        m_optimization->initialize();
    }

    //cout << "ChannelNumber=" << ChannelNumber << endl;
    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::execute ()
{
    // Here you do everything that needs to be done on every single
    // events, e.g. read input variables, apply cuts, and fill
    // histograms and trees.  This is where most of your actual analysis
    // code will go.

    //const char * function_name = "execute()";
    //Info(function_name, "Function calls");

    wk()->tree()->GetEntry(wk()->treeEntry());

    // Extend ttbar samples using HT samples
    // Use 410000 only when GenFiltHT < 600
    // And use 407009, 407010, and 407011 for GenFiltHT > 600 GeV.
    if (ChannelNumber == 410000 && GenFiltHT > 600000)
        return EL::StatusCode::SUCCESS;

    if (isMC) {
        if (process.find("NUHM2") != string::npos) {
            // NUHM2 samples
            cross_section = m_XsecDB->rawxsect(ChannelNumber, 2);
            k_factor = m_XsecDB->kfactor(ChannelNumber, 2);
            filter_efficiency = m_XsecDB->efficiency(ChannelNumber, 2);
            cross_section_kfactor_efficiency = m_XsecDB->xsectTimesEff(ChannelNumber, 2);
        }
        else {
            // bkg samples
            cross_section = m_XsecDB->rawxsect(ChannelNumber);
            k_factor = m_XsecDB->kfactor(ChannelNumber);
            filter_efficiency = m_XsecDB->efficiency(ChannelNumber);
            cross_section_kfactor_efficiency = m_XsecDB->xsectTimesEff(ChannelNumber);
        }
        // If SUSYTools can't find the process, then it return -1
        // We need to convert the results to +1
        // For example, the merged root file for Gtt
        if (cross_section < 0) cross_section = fabs(cross_section);
        if (k_factor < 0) k_factor = fabs(k_factor);
        if (filter_efficiency < 0) filter_efficiency = fabs(filter_efficiency);
        if (cross_section_kfactor_efficiency < 0) cross_section_kfactor_efficiency = fabs(cross_section_kfactor_efficiency);
    }
    else if (isData) {
        cross_section = 1.;
        k_factor = 1.;
        filter_efficiency = 1.;
        cross_section_kfactor_efficiency = 1.;
    }

    //cout << "cross_section=" << cross_section << endl;
    //cout << "k_factor=" << k_factor << endl;
    //cout << "filter_efficiency=" << filter_efficiency << endl;
    //cout << "cross_section_kfactor_efficiency=" << cross_section_kfactor_efficiency << endl;

    // Reset vectors
    vec_elec.clear();
    vec_muon.clear();
    vec_lept.clear();
    vec_jets.clear();

    vec_baseline_elec.clear();
    vec_baseline_muon.clear();
    vec_baseline_lept.clear();
    vec_baseline_jets.clear();

    vec_OR_elec.clear();
    vec_OR_muon.clear();
    vec_OR_lept.clear();
    vec_OR_jets.clear();

    vec_JVT_jets.clear();

    vec_signal_elec.clear();
    vec_signal_muon.clear();
    vec_signal_jets.clear();
    vec_signal_lept.clear();

    if (isMC) {
        //#ifdef _IS_MC_
        fill_electrons(
            NEl,
            11, // particle id e- = 11
            El_eta,
            El_etaclus,
            El_phi,
            El_pT,
            El_E,
            El_charge,
            El_sigd0,
            El_z0sinTheta,
            El_d0pvtx,
            El_passOR,
            El_SFwMediumLH,
            El_IsoSFwMediumLH,
            El_SFwTightLH,
            El_SFwLooseAndBLayerLH,
            El_SFwTrigMediumLH_e12_lhloose_L1EM10VH,
            El_SFwTrigMediumLH_e17_lhloose,
            El_SFwTrigMediumLH_single,
            El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH,
            El_isLooseAndBLayerLH,
            El_isMediumLH,
            El_isTightLH,
            El_nBLayerHits,
            El_expectBLayerHit,
            El_type,
            El_origin,
            El_bkgMotherPdgId,
            El_bkgOrigin,
            El_chFlip,
            El_ptcone20,
            El_ptcone30,
            El_ptcone40,
            El_ptvarcone20,
            El_ptvarcone30,
            El_ptvarcone40,
            El_topoetcone20,
            El_topoetcone30,
            El_topoetcone40,
            El_passIsoLooseTO,
            El_passIsoLoose,
            El_passIsoTight,
            El_passIsoGrad,
            El_passIsoGradCustomTight,
            El_passIsoGradCustom,
            El_passIsoGradLoose,
            El_trigMatch_e12_lhloose_L1EM10VH,
            El_trigMatch_e17_lhloose,
            El_trigMatch_e60_lhmedium,
            El_trigMatch_e24_lhmedium_iloose_L1EM20VH,
            El_trigMatch_2e12_lhloose_L12EM10VH,
            El_trigMatch_2e15_lhloose_L12EM10VH,
            El_trigMatch_2e15_lhvloose_L12EM13VH,
            El_trigMatch_2e15_lhvloose_nod0_L12EM13VH,
            El_trigMatch_2e17_lhvloose_nod0,
            El_trigMatch_e17_lhloose_mu14,
            El_trigMatch_e17_lhloose_nod0_mu14,
            El_TrigMatch_e24_lhmedium_nod0_ivarloose,
            El_TrigMatch_e24_lhtight_nod0_ivarloose,
            El_TrigMatch_e60_lhmedium_nod0);

        fill_muons(
            NMu,
            13, // particle id mu- = 13
            Mu_eta,
            Mu_phi,
            Mu_pT,
            Mu_SFw,
            Mu_IsoSFw,
            Mu_charge,
            Mu_d0pvtx,
            Mu_sigd0,
            Mu_z0sinTheta,
            Mu_isBad,
            Mu_passOR,
            Mu_isTight,
            Mu_isCosmic,
            Mu_type,
            Mu_origin,
            Mu_ptcone20,
            Mu_ptcone30,
            Mu_ptcone40,
            Mu_ptvarcone20,
            Mu_ptvarcone30,
            Mu_ptvarcone40,
            Mu_topoetcone20,
            Mu_topoetcone30,
            Mu_topoetcone40,
            Mu_passIsoLooseTO,
            Mu_passIsoLoose,
            Mu_passIsoTight,
            Mu_passIsoGrad,
            Mu_passIsoGradCustomTight,
            Mu_passIsoGradCustom,
            Mu_passIsoGradLoose,
            MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50,
            Mu_trigMatch_mu26_imedium,
            Mu_trigMatch_mu50,
            Mu_trigMatch_mu8noL1,
            Mu_trigMatch_mu14,
            Mu_trigMatch_mu18,
            Mu_trigMatch_mu18_mu8noL1,
            Mu_trigMatch_e17_lhloose_mu14,
            Mu_trigMatch_e17_lhloose_nod0_mu14,
            Mu_trigMatch_mu20_mu8noL1,
            Mu_trigMatch_mu22_mu8noL1,
            Mu_TrigMatch_mu24_iloose,
            Mu_TrigMatch_mu24_ivarloose,
            Mu_TrigMatch_mu24_iloose_L1MU15,
            Mu_TrigMatch_mu24_ivarloose_L1MU15,
            Mu_trigMatchPair_mu18_mu8noL1,
            Mu_trigMatchPair_mu20_mu8noL1,
            Mu_trigMatchPair_mu22_mu8noL1);

        fill_jets(
            NJet,
            Jet_eta,
            Jet_phi,
            Jet_pT,
            Jet_E,
            Jet_quality,
            Jet_JVT,
            Jet_JVTsf,
            Jet_MV2c20,
            Jet_MV2c10,
            Jet_SFw,
            Jet_ConeTruthLabel,
            Jet_PartonTruthLabel,
            Jet_HadronConeExclTruthLabel,
            Jet_deltaR,
            Jet_nTrk,
            Jet_passOR);
        //#endif // #ifdef _IS_MC_
    }

    if (isData) {
        //#ifdef _IS_DATA_
        fill_electrons(
            NEl,
            11, // particle id e- = 11
            El_eta,
            El_etaclus,
            El_phi,
            El_pT,
            El_E,
            El_charge,
            El_sigd0,
            El_z0sinTheta,
            El_d0pvtx,
            El_passOR,
            El_SFwMediumLH,
            El_IsoSFwMediumLH,
            El_SFwTightLH,
            El_SFwLooseAndBLayerLH,
            El_SFwTrigMediumLH_e12_lhloose_L1EM10VH,
            El_SFwTrigMediumLH_e17_lhloose,
            El_SFwTrigMediumLH_single,
            El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH,
            El_isLooseAndBLayerLH,
            El_isMediumLH,
            El_isTightLH,
            El_nBLayerHits,
            El_expectBLayerHit,
            /*
            El_type, // MC only
            El_origin, // MC only
            El_bkgMotherPdgId, // MC only
            El_bkgOrigin, // MC only
            El_chFlip, // MC only
            */
            El_ptcone20,
            El_ptcone30,
            El_ptcone40,
            El_ptvarcone20,
            El_ptvarcone30,
            El_ptvarcone40,
            El_topoetcone20,
            El_topoetcone30,
            El_topoetcone40,
            El_passIsoLooseTO,
            El_passIsoLoose,
            El_passIsoTight,
            El_passIsoGrad,
            El_passIsoGradCustomTight,
            El_passIsoGradCustom,
            El_passIsoGradLoose,
            El_trigMatch_e12_lhloose_L1EM10VH,
            El_trigMatch_e17_lhloose,
            El_trigMatch_e60_lhmedium,
            El_trigMatch_e24_lhmedium_iloose_L1EM20VH,
            El_trigMatch_2e12_lhloose_L12EM10VH,
            El_trigMatch_2e15_lhloose_L12EM10VH,
            El_trigMatch_2e15_lhvloose_L12EM13VH,
            El_trigMatch_2e15_lhvloose_nod0_L12EM13VH,
            El_trigMatch_2e17_lhvloose_nod0,
            El_trigMatch_e17_lhloose_mu14,
            El_trigMatch_e17_lhloose_nod0_mu14,
            El_TrigMatch_e24_lhmedium_nod0_ivarloose,
            El_TrigMatch_e24_lhtight_nod0_ivarloose,
            El_TrigMatch_e60_lhmedium_nod0);

        fill_muons(
            NMu,
            13, // particle id mu- = 13
            Mu_eta,
            Mu_phi,
            Mu_pT,
            Mu_SFw,
            Mu_IsoSFw,
            Mu_charge,
            Mu_d0pvtx,
            Mu_sigd0,
            Mu_z0sinTheta,
            Mu_isBad,
            Mu_passOR,
            Mu_isTight,
            Mu_isCosmic,
            /*
            Mu_type, // MC only
            Mu_origin, // MC only
            */
            Mu_ptcone20,
            Mu_ptcone30,
            Mu_ptcone40,
            Mu_ptvarcone20,
            Mu_ptvarcone30,
            Mu_ptvarcone40,
            Mu_topoetcone20,
            Mu_topoetcone30,
            Mu_topoetcone40,
            Mu_passIsoLooseTO,
            Mu_passIsoLoose,
            Mu_passIsoTight,
            Mu_passIsoGrad,
            Mu_passIsoGradCustomTight,
            Mu_passIsoGradCustom,
            Mu_passIsoGradLoose,
            MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50,
            Mu_trigMatch_mu26_imedium,
            Mu_trigMatch_mu50,
            Mu_trigMatch_mu8noL1,
            Mu_trigMatch_mu14,
            Mu_trigMatch_mu18,
            Mu_trigMatch_mu18_mu8noL1,
            Mu_trigMatch_e17_lhloose_mu14,
            Mu_trigMatch_e17_lhloose_nod0_mu14,
            Mu_trigMatch_mu20_mu8noL1,
            Mu_trigMatch_mu22_mu8noL1,
            Mu_TrigMatch_mu24_iloose,
            Mu_TrigMatch_mu24_ivarloose,
            Mu_TrigMatch_mu24_iloose_L1MU15,
            Mu_TrigMatch_mu24_ivarloose_L1MU15,
            Mu_trigMatchPair_mu18_mu8noL1,
            Mu_trigMatchPair_mu20_mu8noL1,
            Mu_trigMatchPair_mu22_mu8noL1);

        fill_jets(
            NJet,
            Jet_eta,
            Jet_phi,
            Jet_pT,
            Jet_E,
            Jet_quality,
            Jet_JVT,
            Jet_JVTsf,
            Jet_MV2c20,
            Jet_MV2c10,
            Jet_SFw,
            /*
            Jet_ConeTruthLabel, // MC only
            Jet_PartonTruthLabel, // MC only
            Jet_HadronConeExclTruthLabel, // MC only
            Jet_deltaR, // MC only
            */
            Jet_nTrk,
            Jet_passOR);
    //#endif // #ifdef _IS_DATA_
    }

    // sort by descending Pt
    sort(vec_elec.begin(), vec_elec.end(), sort_descending_Pt<Electron>);
    sort(vec_muon.begin(), vec_muon.end(), sort_descending_Pt<Muon>);
    sort(vec_jets.begin(), vec_jets.end(), sort_descending_Pt<Jet>);

    // Set the baseline for electrons, muons, and jets.
    // Set the isSignal property for electrons and muons and set the isBjet property for jets.
    set_baseline_and_signal_electrons();
    set_baseline_and_signal_muons();
    set_jets_and_bjets();

    // Fill leptons into vector. Put the fill_leptons() function at here then the lepton in the
    // vector has correct baseline, flavor, and isSignal information.
    fill_leptons(vec_elec, vec_muon);
    // Now sort leptons by descending Pt
    sort(vec_lept.begin(), vec_lept.end(), sort_descending_Pt<Lepton>);

    // Fill baseline electrons, baseline muons, baseline jets, and baseline leptons into vectors.
    fill_baseline_electrons(vec_elec);
    fill_baseline_muons(vec_muon);
    fill_baseline_leptons(vec_elec, vec_muon);
    fill_baseline_jets(vec_jets);
    // Now sort leptons by descending Pt
    sort(vec_baseline_lept.begin(), vec_baseline_lept.end(), sort_descending_Pt<Lepton>);

    int random_run_number = -1;
    random_run_number = PRWrandomRunNumber;
/*
    random_run_number = m_cutflow->get_mc_random_event_number(isData, isMC,
                                                              EventNumber, ChannelNumber,
                                                              AvgMu, EventWeight, //PRWWeight,
                                                              LB, RunNb);
*/
    //cout << "*** EventNumber=" << EventNumber << endl;
    //cout << "ytEventSelection AvgMu=" << AvgMu << endl;
    //cout << "ytEventSelection PRWWeight=" << PRWWeight << endl;
    //cout << "yt_cutflows AvgMu=" << m_cutflow->get_AvgMu() << endl;
    //cout << "yt_cutflows pileup_weight=" << m_cutflow->get_pileup_weight() << endl;

    float pileup_weight = 1.;
    pileup_weight = m_cutflow->get_pileup_weight();

    double weight = 1.;
    if (isMC)
        weight = luminosity * cross_section_kfactor_efficiency * 1000. * EventWeight * pileup_weight / derivation_stat_weights;

    // Dump histograms
    h_Nvtx->Fill(Nvtx, weight);

    if (isData)
        h_AvgMu->Fill(m_cutflow->get_AvgMu() * 1.0 / 1.16);
    else if (isMC)
        h_AvgMu->Fill(m_cutflow->get_AvgMu(), weight);

    if (vec_lept.size() > 0)
        h_NLepts->Fill(vec_lept.at(0).get_number(), weight);
    else
        h_NLepts->Fill(0., weight);

    if (vec_jets.size() > 0)
        h_NJets->Fill(vec_jets.at(0).get_number(), weight);
    else
        h_NJets->Fill(0., weight);

    //----------------------------------//
    // Apply cuts
    //----------------------------------//

    m_cutflow->events_pass_cutflow[DerivationStat_Weights] = derivation_stat_weights; // read the first bin from root file.

    bool cut1  = m_cutflow->pass_all_events();
    m_cutflow->update(All_events, cut1);
    if (!cut1) return EL::StatusCode::SUCCESS;

    bool cut2  = m_cutflow->pass_GRL(isData, RunNb, LB);
    m_cutflow->update(GRL, cut2);
    if (!cut2) return EL::StatusCode::SUCCESS;

    bool cut3  = m_cutflow->pass_primary_vertex(PV_z);
    m_cutflow->update(Primary_vertex, cut3);
    if (!cut3) return EL::StatusCode::SUCCESS;

    bool cut4  = m_cutflow->pass_trigger(isData, isMC, RunNb, random_run_number /*PRWrandomRunNumber*/,
                                         HLT_2e12_lhloose_L12EM10VH, HLT_e17_lhloose_mu14, HLT_mu18_mu8noL1, HLT_xe70,
                                         HLT_2e17_lhvloose_nod0, HLT_e17_lhloose_nod0_mu14, HLT_mu20_mu8noL1, HLT_xe100_mht_L1XE50,
                                         Etmiss_TST_Et);
    m_cutflow->update(Trigger, cut4);
    if (!cut4) return EL::StatusCode::SUCCESS;

    bool cut5  = m_cutflow->pass_global_flags(isData, isMC, DetError);
    m_cutflow->update(Global_flags, cut5);
    if (!cut5) return EL::StatusCode::SUCCESS;

    bool cut6  = m_cutflow->pass_bad_muon(vec_baseline_muon); // use baseline muons
    m_cutflow->update(Bad_muon, cut6);
    if (!cut6) return EL::StatusCode::SUCCESS;

    if (!isSkim && !isOptimization) {
        bool cut7  = m_cutflow->pass_at_least_one_jet_passes_jet_OR(vec_baseline_jets); // use baseline jets
        m_cutflow->update(At_least_one_jet_passes_jet_OR, cut7);
        if (!cut7) return EL::StatusCode::SUCCESS;
    }

    bool cut8  = m_cutflow->pass_bad_jet(vec_jets); // we have to use the raw jet objects (vec_jets) at this step.
    m_cutflow->update(Bad_jet, cut8);
    if (!cut8) return EL::StatusCode::SUCCESS;

    // Fill OR electrons, OR muons, OR jets, and OR leptons into vectors.
    fill_OR_electrons(vec_baseline_elec);
    fill_OR_muons(vec_baseline_muon);
    fill_OR_leptons(vec_baseline_elec, vec_baseline_muon);
    fill_OR_jets(vec_baseline_jets); // Apply jet OR before jet quality
    // Now sort leptons by descending Pt
    sort(vec_OR_lept.begin(), vec_OR_lept.end(), sort_descending_Pt<Lepton>);

    // JVT cut applied after OR and jet quality
    fill_JVT_jets(vec_OR_jets);

    if (!isSkim && isOptimization) {
        bool cut9  = m_cutflow->pass_at_least_one_signal_jet(vec_JVT_jets);
        m_cutflow->update(At_least_one_signal_jet, cut9);
        if (!cut9) return EL::StatusCode::SUCCESS;
    }

    bool cut10 = m_cutflow->pass_cosmic_muon_veto(vec_OR_muon);
    m_cutflow->update(Cosmic_muons_veto, cut10);
    if (!cut10) return EL::StatusCode::SUCCESS;

    bool cut11 = m_cutflow->pass_at_least_two_baseline_leptons_greater_than_10GeV(vec_OR_lept);
    m_cutflow->update(At_least_two_baseline_leptons_greater_than_10GeV, cut11);
    if (!cut11) return EL::StatusCode::SUCCESS;

    // Fill signal electrons, signal muons, signal jets, and signal leptons into vectors.
    fill_signal_electrons(vec_OR_elec);
    fill_signal_muons(vec_OR_muon);
    fill_signal_leptons(vec_signal_elec, vec_signal_muon);
    fill_signal_jets(vec_JVT_jets);
    // Now sort leptons by descending Pt
    sort(vec_signal_lept.begin(), vec_signal_lept.end(), sort_descending_Pt<Lepton>);

    // Dump the AvgMu histograms of the opposit sign baseline leptons.
    if (vec_baseline_lept.size() == 2) {
        int sign = vec_baseline_lept[0].get_charge() * vec_baseline_lept[1].get_charge();
        int channel = vec_baseline_lept[0].get_flavor() * vec_baseline_lept[1].get_flavor();

        TLorentzVector tlv_lept0, tlv_lept1;
        if (vec_baseline_lept[0].get_flavor() == 11) // electron
            tlv_lept0.SetPtEtaPhiE(vec_baseline_lept[0].get_pt(), vec_baseline_lept[0].get_eta(), vec_baseline_lept[0].get_phi(), vec_baseline_lept[0].get_E());
        else if (vec_baseline_lept[0].get_flavor() == 13) // muon
            tlv_lept0.SetPtEtaPhiM(vec_baseline_lept[0].get_pt(), vec_baseline_lept[0].get_eta(), vec_baseline_lept[0].get_phi(), Mu_Mass);

        if (vec_baseline_lept[1].get_flavor() == 11) // electron
            tlv_lept1.SetPtEtaPhiE(vec_baseline_lept[1].get_pt(), vec_baseline_lept[1].get_eta(), vec_baseline_lept[1].get_phi(), vec_baseline_lept[1].get_E());
        else if (vec_baseline_lept[1].get_flavor() == 13) // muon
            tlv_lept1.SetPtEtaPhiM(vec_baseline_lept[1].get_pt(), vec_baseline_lept[1].get_eta(), vec_baseline_lept[1].get_phi(), Mu_Mass);

        double mll = (tlv_lept0 + tlv_lept1).M();

        if (sign == -1 && channel == 121) {
            // OS ee
            if (isData)
                h_AvgMu_OSee->Fill(m_cutflow->get_AvgMu() * 1.0 / 1.16);
            else if (isMC)
                h_AvgMu_OSee->Fill(m_cutflow->get_AvgMu(), weight);
            h_baseline_OSee_mll->Fill(mll / 1000., weight);
        }
        else if (sign == -1 && channel == 169) {
            // OS mumu
            if (isData)
                h_AvgMu_OSmumu->Fill(m_cutflow->get_AvgMu() * 1.0 / 1.16);
            else if (isMC)
                h_AvgMu_OSmumu->Fill(m_cutflow->get_AvgMu(), weight);
            h_baseline_OSmumu_mll->Fill(mll / 1000.);
        }
    }

    // Calculate weight
    double baseline_weight = 1., signal_weight = 1.;
    baseline_weight = ID_weight(vec_baseline_elec, false) * ID_weight(vec_baseline_muon) * jets_weight(vec_signal_jets);
    signal_weight = ID_weight(vec_signal_elec, true) * Iso_weight(vec_signal_elec) * ID_weight(vec_signal_muon) * Iso_weight(vec_signal_muon) * jets_weight(vec_signal_jets);

    // if (isSkim) {
    //     cout << "Doing skim at here..." << endl;
    //     Setting
    //     m_skim->set_isMC(isMC);
    //     m_skim->set_isData(isData);
    //     m_skim->set_process(process);
    //     m_skim->set_luminosity(luminosity);
    //     m_skim->set_cross_section(cross_section);
    //     m_skim->set_k_factor(k_factor);
    //     m_skim->set_filter_efficiency(filter_efficiency);
    //     m_skim->set_cross_section_kfactor_efficiency(cross_section_kfactor_efficiency);
    //     m_skim->set_event_weight_sum(derivation_stat_weights);
    //     m_skim->set_event_weight(EventWeight);
    //     m_skim->set_pileup_weight(pileup_weight /*PRWWeight*/);
    //     m_skim->set_baseline_weight(baseline_weight);
    //     m_skim->set_signal_weight(signal_weight);
    //     if (isData)
    //         m_skim->set_run_number(RunNb);
    //     else if (isMC)
    //         m_skim->set_run_number(random_run_number /*PRWrandomRunNumber*/);
    //     m_skim_set_new_AvgMu(m_cutflow->get_AvgMu());
    //     m_skim->set_tag_pt_threshold(tag_pt_threshold);
    //     Do skim
    //     m_skim->execute(vec_elec, vec_muon, vec_lept, vec_jets,
    //                    vec_baseline_elec, vec_baseline_muon, vec_baseline_lept, vec_baseline_jets,
    //                    vec_signal_elec, vec_signal_muon, vec_signal_lept, vec_signal_jets);
    // }

    bool cut12 = m_cutflow->pass_at_least_two_signal_leptons_greater_than_20GeV(vec_signal_lept);
    m_cutflow->update(At_least_two_signal_leptons_greater_than_20GeV, cut12);
    if (!cut12) return EL::StatusCode::SUCCESS;

    // Dump the AvgMu histograms of the opposit sign baseline leptons.
    if (vec_signal_lept.size() == 2) {
        int sign = vec_signal_lept[0].get_charge() * vec_signal_lept[1].get_charge();
        int channel = vec_baseline_lept[0].get_flavor() * vec_baseline_lept[1].get_flavor();

        TLorentzVector tlv_lept0, tlv_lept1;
        if (vec_signal_lept[0].get_flavor() == 11) // electron
            tlv_lept0.SetPtEtaPhiE(vec_signal_lept[0].get_pt(), vec_signal_lept[0].get_eta(), vec_signal_lept[0].get_phi(), vec_signal_lept[0].get_E());
        else if (vec_signal_lept[0].get_flavor() == 13) // muon
            tlv_lept0.SetPtEtaPhiM(vec_signal_lept[0].get_pt(), vec_signal_lept[0].get_eta(), vec_signal_lept[0].get_phi(), Mu_Mass);

        if (vec_signal_lept[1].get_flavor() == 11) // electron
            tlv_lept1.SetPtEtaPhiE(vec_signal_lept[1].get_pt(), vec_signal_lept[1].get_eta(), vec_signal_lept[1].get_phi(), vec_signal_lept[1].get_E());
        else if (vec_signal_lept[1].get_flavor() == 13) // muon
            tlv_lept1.SetPtEtaPhiM(vec_signal_lept[1].get_pt(), vec_signal_lept[1].get_eta(), vec_signal_lept[1].get_phi(), Mu_Mass);

        double mll = (tlv_lept0 + tlv_lept1).M();

        if (sign == -1 && channel == 121) {
            // OS ee
            h_signal_OSee_mll->Fill(mll / 1000., weight);
        }
        else if (sign == -1 && channel == 169) {
            // OS mumu
            h_signal_OSmumu_mll->Fill(mll / 1000., weight);
        }
    }

    bool cut13 = m_cutflow->pass_same_sign(vec_signal_lept);
    m_cutflow->update(Same_sign, cut13);
    if (!cut13) return EL::StatusCode::SUCCESS;

    // same-sign
    // e-e
    int ee_cut1 = m_cutflow->pass_channel_separation(vec_signal_lept);
    if (ee_cut1 == 1) {
        m_cutflow->update(ee_channel_separation, true);
    }

    bool ee_cut2 = m_cutflow->pass_trigger_matching(isData, isMC, RunNb, random_run_number /*PRWrandomRunNumber*/, vec_signal_elec, vec_signal_muon,
                                                    HLT_2e12_lhloose_L12EM10VH, HLT_e17_lhloose_mu14, HLT_mu18_mu8noL1, HLT_xe70,
                                                    HLT_2e17_lhvloose_nod0, HLT_e17_lhloose_nod0_mu14, HLT_mu20_mu8noL1, HLT_xe100_mht_L1XE50,
                                                    Etmiss_TST_Et);
    if (ee_cut1 == 1 && ee_cut2) {
        m_cutflow->update(ee_trigger_matching, ee_cut2);
    }

    bool ee_cut3 = m_cutflow->pass_at_least_one_bjet_greater_than_20GeV(vec_signal_jets);
    if (ee_cut1 == 1 && ee_cut2 && ee_cut3) {
        m_cutflow->update(ee_at_least_one_bjet_greater_than_20GeV, ee_cut3);
    }

    bool ee_cut4 = m_cutflow->pass_four_jets_greater_than_50GeV(vec_signal_jets);
    if (ee_cut1 == 1 && ee_cut2 && ee_cut3 && ee_cut4) {
        m_cutflow->update(ee_four_jets_greater_than_50GeV, ee_cut4);
    }

    bool ee_cut5 = m_cutflow->pass_MET_greater_than_125GeV(Etmiss_TST_Et);
    if (ee_cut1 == 1 && ee_cut2 && ee_cut3 && ee_cut4 && ee_cut5) {
        m_cutflow->update(ee_MET_greater_than_125GeV, ee_cut5);
    }

    // e-mu
    int emu_cut1 = m_cutflow->pass_channel_separation(vec_signal_lept);
    if (emu_cut1 == 2) {
        m_cutflow->update(emu_channel_separation, true);
    }

    bool emu_cut2 = m_cutflow->pass_trigger_matching(isData, isMC, RunNb, random_run_number /*PRWrandomRunNumber*/, vec_signal_elec, vec_signal_muon,
                                                     HLT_2e12_lhloose_L12EM10VH, HLT_e17_lhloose_mu14, HLT_mu18_mu8noL1, HLT_xe70,
                                                     HLT_2e17_lhvloose_nod0, HLT_e17_lhloose_nod0_mu14, HLT_mu20_mu8noL1, HLT_xe100_mht_L1XE50,
                                                     Etmiss_TST_Et);
    if (emu_cut1 == 2 && emu_cut2) {
        m_cutflow->update(emu_trigger_matching, emu_cut2);
    }

    bool emu_cut3 = m_cutflow->pass_at_least_one_bjet_greater_than_20GeV(vec_signal_jets);
    if (emu_cut1 == 2 && emu_cut2 && emu_cut3) {
        m_cutflow->update(emu_at_least_one_bjet_greater_than_20GeV, emu_cut3);
    }

    bool emu_cut4 = m_cutflow->pass_four_jets_greater_than_50GeV(vec_signal_jets);
    if (emu_cut1 == 2 && emu_cut2 && emu_cut3 && emu_cut4) {
        m_cutflow->update(emu_four_jets_greater_than_50GeV, emu_cut4);
    }

    bool emu_cut5 = m_cutflow->pass_MET_greater_than_125GeV(Etmiss_TST_Et);
    if (emu_cut1 == 2 && emu_cut2 && emu_cut3 && emu_cut4 && emu_cut5) {
        m_cutflow->update(emu_MET_greater_than_125GeV, emu_cut5);
    }

    // mu-mu
    int mumu_cut1 = m_cutflow->pass_channel_separation(vec_signal_lept);
    if (mumu_cut1 == 3) {
        m_cutflow->update(mumu_channel_separation, true);
    }

    bool mumu_cut2 = m_cutflow->pass_trigger_matching(isData, isMC, RunNb, random_run_number /*PRWrandomRunNumber*/, vec_signal_elec, vec_signal_muon,
                                                      HLT_2e12_lhloose_L12EM10VH, HLT_e17_lhloose_mu14, HLT_mu18_mu8noL1, HLT_xe70,
                                                      HLT_2e17_lhvloose_nod0, HLT_e17_lhloose_nod0_mu14, HLT_mu20_mu8noL1, HLT_xe100_mht_L1XE50,
                                                      Etmiss_TST_Et);
    if (mumu_cut1 == 3 && mumu_cut2) {
        m_cutflow->update(mumu_trigger_matching, mumu_cut2);
    }

    bool mumu_cut3 = m_cutflow->pass_at_least_one_bjet_greater_than_20GeV(vec_signal_jets);
    if (mumu_cut1 == 3 && mumu_cut2 && mumu_cut3) {
        m_cutflow->update(mumu_at_least_one_bjet_greater_than_20GeV, mumu_cut3);
    }

    bool mumu_cut4 = m_cutflow->pass_four_jets_greater_than_50GeV(vec_signal_jets);
    if (mumu_cut1 == 3 && mumu_cut2 && mumu_cut3 && mumu_cut4) {
        m_cutflow->update(mumu_four_jets_greater_than_50GeV, mumu_cut4);
    } 

    bool mumu_cut5 = m_cutflow->pass_MET_greater_than_125GeV(Etmiss_TST_Et);
    if (mumu_cut1 == 3 && mumu_cut2 && mumu_cut3 && mumu_cut4 && mumu_cut5) {
        m_cutflow->update(mumu_MET_greater_than_125GeV, mumu_cut5);
    }

    if (isOptimization) {
        // require trigger matching
        if (!(ee_cut2 || emu_cut2 || mumu_cut2))
            return EL::StatusCode::SUCCESS;

        double elec_ID_weight = 1., elec_iso_weight = 1.,
               muon_ID_weight = 1., muon_iso_weight = 1.,
               lepton_weight = 1., jet_weight = 1.,
               dilepton_trigger_weight = 1.;

        if (isMC) {
            elec_ID_weight = ID_weight(vec_signal_elec, true);
            elec_iso_weight = Iso_weight(vec_signal_elec);
            muon_ID_weight = ID_weight(vec_signal_muon);
            muon_iso_weight = Iso_weight(vec_signal_muon);
            lepton_weight = lept_weight(elec_ID_weight, elec_iso_weight, muon_ID_weight, muon_iso_weight);
            jet_weight = jets_weight(vec_signal_jets);

            // DileptonTriggerWeight
            if(Etmiss_TST_Et < 250000 || !HLT_xe70 || !HLT_xe100_mht_L1XE50) {
                // lepton scale factors
                for (auto & el_itr : vec_signal_elec) {
                    m_dtwTool->AddElectron(el_itr.get_pt(), el_itr.get_eta(), RunNb, isFullSim);
                }
                for (auto & mu_itr : vec_signal_muon) {
                    m_dtwTool->AddMuon(mu_itr.get_pt(), mu_itr.get_eta(), mu_itr.get_phi(), RunNb);
                }
                double uncertainty;
                dilepton_trigger_weight = m_dtwTool->GetScaleFactor(uncertainty);
            }
        }
        /*
        cout << "***In ytEventSelection***" << endl;
        cout << "elec_ID_weight=" << elec_ID_weight << endl;
        cout << "elec_iso_weight=" << elec_iso_weight << endl;
        cout << "muon_ID_weight=" << muon_ID_weight << endl;
        cout << "muon_iso_weight=" << muon_iso_weight << endl;
        cout << "lepton_weight=" << lepton_weight << endl;
        cout << "jet_weight=" << jet_weight << endl;
        cout << "dilepton_trigger_weight=" << dilepton_trigger_weight << endl;
        */

        m_optimization->set_cross_section(cross_section);
        m_optimization->set_k_factor(k_factor);
        m_optimization->set_filter_efficiency(filter_efficiency);
        m_optimization->set_cross_section_kfactor_efficiency(cross_section_kfactor_efficiency);

        //m_optimization->set_derivation_stat_weights(derivation_stat_weights);

        m_optimization->set_event_weight(EventWeight);
        m_optimization->set_lepton_weight(lepton_weight * dilepton_trigger_weight);
        m_optimization->set_jet_weight(jet_weight);
        m_optimization->set_pileup_weight(PRWWeight);

        m_optimization->set_met(Etmiss_TST_Et);

        m_optimization->execute(vec_signal_elec, vec_signal_muon, vec_signal_lept, vec_signal_jets);
    }

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::postExecute ()
{
    // Here you do everything that needs to be done after the main event
    // processing.  This is typically very rare, particularly in user
    // code.  It is mainly used in implementing the NTupleSvc.

    //const char * function_name = "postExecute()";
    //Info(function_name, "Function calls");

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::finalize ()
{
    // This method is the mirror image of initialize(), meaning it gets
    // called after the last event has been processed on the worker node
    // and allows you to finish up any objects you created in
    // initialize() before they are written to disk.  This is actually
    // fairly rare, since this happens separately for each worker node.
    // Most of the time you want to do your post-processing on the
    // submission node after all your histogram outputs have been
    // merged.  This is different from histFinalize() in that it only
    // gets called on worker nodes that processed input events.

    //const char * function_name = "finalize()";
    //Info(function_name, "Function calls");

    m_cutflow->print();

    //if (isSkim) {
        //m_skim->finalize();
        //m_skim->debug_print();
    //}
    if (isOptimization) {
        m_optimization->set_derivation_stat_weights(derivation_stat_weights);
        m_optimization->finalize();
    }

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode ytEventSelection::histFinalize ()
{
    // This method is the mirror image of histInitialize(), meaning it
    // gets called after the last event has been processed on the worker
    // node and allows you to finish up any objects you created in
    // histInitialize() before they are written to disk.  This is
    // actually fairly rare, since this happens separately for each
    // worker node.  Most of the time you want to do your
    // post-processing on the submission node after all your histogram
    // outputs have been merged.  This is different from finalize() in
    // that it gets called on all worker nodes regardless of whether
    // they processed input events.

    //const char * function_name = "histFinalize()";
    //Info(function_name, "Function calls");
/*
    this->make_plots_Nvtx();
    this->make_plots_AvgMu();
    this->make_plots_NLepts();
    this->make_plots_NJets();
*/
    return EL::StatusCode::SUCCESS;
}



//
// user defined functions
//

//----------------------------------//
// debug print functions
//----------------------------------//
void ytEventSelection::debug_lept_print(vector<Lepton> vec_lept)
{
    cout << "*** Event Number=" << EventNumber << endl;
    cout << "NEl+NMu=" << vec_lept.size() << endl;
    int i = 0;
    for (auto & lep_itr : vec_lept) {
        //cout << "NEl+NMu=" << lep_itr.get_number() << endl;
        cout << i + 1
            << ": pt=" << lep_itr.get_pt()
            << ", eta=" << lep_itr.get_eta()
            << ", phi=" << lep_itr.get_phi()
            << ", baseline=" << lep_itr.get_baseline()
            << ", passOR=" << lep_itr.get_passOR()
            << ", isSignal=" << lep_itr.get_isSignal()
            << ", flavor=" << lep_itr.get_flavor()
            << ", charge=" << lep_itr.get_charge()
            << endl;
        i++;
    }
}



void ytEventSelection::debug_elec_print(vector<Electron> vec_elec)
{
    cout << "*** Event Number=" << EventNumber << endl;
    cout << "NEl=" << vec_elec.size() << endl;
    int i = 0;
    for (auto & el_itr : vec_elec) {
        //cout << "NEl=" << el_itr.get_number() << endl;
        cout << i + 1
            << ": pt=" << el_itr.get_pt()
            << ", eta=" << el_itr.get_eta() << ", etaclus=" << el_itr.get_etaclus() 
            << ", phi=" << el_itr.get_phi()
            << ", baseline=" << el_itr.get_baseline()
            << ", passOR=" << el_itr.get_passOR()
            << ", isSignal=" << el_itr.get_isSignal()
            << ", flavor=" << el_itr.get_flavor()
            << ", charge=" << el_itr.get_charge()
            << ", d0pvtx=" << el_itr.get_d0pvtx()
            << ", |d0sig|=" << fabs(el_itr.get_sigd0())
            << ", |z0sinTheta|=" << fabs(el_itr.get_z0sinTheta())
            << ", ptvarcone20/pt=" << el_itr.get_ptvarcone20() / el_itr.get_pt()
            << ", topoetcone20/pt=" << el_itr.get_topoetcone20() / el_itr.get_pt()
            << ", SFwTightLH=" << el_itr.get_SFwTightLH()
            << ", SFwMediumLH=" << el_itr.get_SFwMediumLH()
            << ", SFwLooseAndBLayerLH=" << el_itr.get_SFwLooseAndBLayerLH()
            << ", IsoSFwMediumLH=" << el_itr.get_IsoSFwMediumLH()
            << ", SFwTrigMediumLH_single=" << el_itr.get_SFwTrigMediumLH_single()
            << ", SFwTrigMediumLH_e12_lhloose_L1EM10VH=" << el_itr.get_SFwTrigMediumLH_e12_lhloose_L1EM10VH()
            << ", SFwTrigMediumLH_e17_lhloose=" << el_itr.get_SFwTrigMediumLH_e17_lhloose()
            << ", SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH=" << el_itr.get_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH()
            << ", trigMatch_e12_lhloose_L1EM10VH=" << el_itr.get_trigMatch_e12_lhloose_L1EM10VH()
            << ", trigMatch_e17_lhloose=" << el_itr.get_trigMatch_e17_lhloose()
            << ", trigMatch_e17_lhloose_mu14=" << el_itr.get_trigMatch_e17_lhloose_mu14()
            << ", trigMatch_e17_lhloose_nod0_mu14=" << el_itr.get_trigMatch_e17_lhloose_nod0_mu14()
            << ", trigMatch_e24_lhmedium_iloose_L1EM20VH=" << el_itr.get_trigMatch_e24_lhmedium_iloose_L1EM20VH()
            << ", trigMatch_e60_lhmedium=" << el_itr.get_trigMatch_e60_lhmedium()
            << ", trigMatch_2e12_lhloose_L12EM10VH=" << el_itr.get_trigMatch_2e12_lhloose_L12EM10VH()
            << ", trigMatch_2e15_lhloose_L12EM10VH=" << el_itr.get_trigMatch_2e15_lhloose_L12EM10VH()
            << ", trigMatch_2e15_lhvloose_L12EM13VH=" << el_itr.get_trigMatch_2e15_lhvloose_L12EM13VH()
            << ", trigMatch_2e15_lhvloose_nod0_L12EM13VH=" << el_itr.get_trigMatch_2e15_lhvloose_nod0_L12EM13VH()
            << ", trigMatch_2e17_lhvloose_nod0=" << el_itr.get_trigMatch_2e17_lhvloose_nod0()
            << ", trigMatch_e24_lhmedium_nod0_ivarloose=" << el_itr.get_trigMatch_e24_lhmedium_nod0_ivarloose()
            << ", trigMatch_e24_lhtight_nod0_ivarloose=" << el_itr.get_trigMatch_e24_lhtight_nod0_ivarloose()
            << ", trigMatch_e60_lhmedium_nod0=" << el_itr.get_trigMatch_e60_lhmedium_nod0()
            << endl;
        i++;
    }
}



void ytEventSelection::debug_muon_print(vector<Muon> vec_muon)
{
    cout << "*** Event Number=" << EventNumber << endl;
    cout << "NMu=" << vec_muon.size() << endl;
    int i = 0;
    for (auto & mu_itr : vec_muon) {
        //cout << "NMu=" << mu_itr.get_number() << endl;
        cout << i + 1
            << ": pt=" << mu_itr.get_pt()
            << ", eta=" << mu_itr.get_eta()
            << ", phi=" << mu_itr.get_phi()
            << ", baseline=" << mu_itr.get_baseline()
            << ", passOR=" << mu_itr.get_passOR()
            << ", isSignal=" << mu_itr.get_isSignal()
            << ", flavor=" << mu_itr.get_flavor()
            << ", charge=" << mu_itr.get_charge()
            << ", isBad=" << mu_itr.get_isBad()
            << ", isCosmic=" << mu_itr.get_isCosmic()
            << ", d0pvtx=" << mu_itr.get_d0pvtx()
            << ", |d0sig|=" << fabs(mu_itr.get_sigd0())
            << ", |z0sinTheta|=" << fabs(mu_itr.get_z0sinTheta())
            << ", ptvarcone30/pt=" << mu_itr.get_ptvarcone30() / mu_itr.get_pt()
            << ", SFw=" << mu_itr.get_SFw()
            << ", IsoSFw=" << mu_itr.get_IsoSFw()
            << ", MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50=" << mu_itr.get_MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50()
            << ", trigMatch_mu8noL1=" << mu_itr.get_trigMatch_mu8noL1()
            << ", trigMatch_mu14=" << mu_itr.get_trigMatch_mu14()
            << ", trigMatch_mu18=" << mu_itr.get_trigMatch_mu18()
            << ", trigMatch_mu18_mu8noL1=" << mu_itr.get_trigMatch_mu18_mu8noL1()
            << ", trigMatch_mu20_mu8noL1=" << mu_itr.get_trigMatch_mu20_mu8noL1()
            << ", trigMatch_mu22_mu8noL1=" << mu_itr.get_trigMatch_mu22_mu8noL1()
            << ", trigMatch_mu26_imedium=" << mu_itr.get_trigMatch_mu26_imedium()
            << ", trigMatch_mu50=" << mu_itr.get_trigMatch_mu50()
            << ", trigMatch_e17_lhloose_mu14=" << mu_itr.get_trigMatch_e17_lhloose_mu14()
            << ", trigMatch_e17_lhloose_nod0_mu14=" << mu_itr.get_trigMatch_e17_lhloose_nod0_mu14()
            << ", trigMatch_mu24_iloose=" << mu_itr.get_trigMatch_mu24_iloose()
            << ", trigMatch_mu24_ivarloose=" << mu_itr.get_trigMatch_mu24_ivarloose()
            << ", trigMatch_mu24_iloose_L1MU15=" << mu_itr.get_trigMatch_mu24_iloose_L1MU15()
            << ", trigMatch_mu24_ivarloose_L1MU15=" << mu_itr.get_trigMatch_mu24_ivarloose_L1MU15()
            << endl;
        i++;
    }
}



void ytEventSelection::debug_jets_print(vector<Jet> vec_jets)
{
    cout << "*** Event Number=" << EventNumber << endl;
    cout << "NJet=" << vec_jets.size() << endl;
    int i = 0;
    for (auto & jet_itr : vec_jets) {
        //cout << "NJet=" << jet_itr.get_number() << endl;
        cout << i + 1
            << ": pt=" << jet_itr.get_pt()
            << ", eta=" << jet_itr.get_eta()
            << ", phi=" << jet_itr.get_phi()
            << ", baseline=" << jet_itr.get_baseline()
            << ", passOR=" << jet_itr.get_passOR()
            << ", isBjet=" << jet_itr.get_isBjet()
            << ", quality=" << jet_itr.get_quality()
            << ", JVT=" << jet_itr.get_JVT()
            << ", JVTsf=" << jet_itr.get_JVTsf()
            << ", MV2c20=" << jet_itr.get_MV2c20()
            << ", MV2c10=" << jet_itr.get_MV2c10()
            << ", SFw=" << jet_itr.get_SFw()
            << ", nTrk=" << jet_itr.get_nTrk()
            << endl;
        i++;
    }
}


/*
//----------------------------------//
// Calculate weight functions
//----------------------------------//
double ytEventSelection::ID_weight(vector<Electron> elec, bool signal)
{
    double total = 1.;
    if (!signal) {// baseline electrons
        for (auto & el_itr : elec) {
            total *= el_itr.get_SFwLooseAndBLayerLH();
        }
    }
    else {// signal electrons
        for (auto & el_itr : elec) {
            total *= el_itr.get_SFwMediumLH();
        }
    }
    return total;
}



double ytEventSelection::ID_weight(vector<Muon> muon)
{
    double total = 1.;
    for (auto & mu_itr : muon) {
        total *= mu_itr.get_SFw();
    }
    return total;
}



double ytEventSelection::Iso_weight(vector<Electron> elec)
{
    double total = 1.;
    for (auto & el_itr : elec) {
        total *= el_itr.get_IsoSFwMediumLH();
    }
    return total;
}



double ytEventSelection::Iso_weight(vector<Muon> muon)
{
    double total = 1.;
    for (auto & mu_itr : muon) {
        total *= mu_itr.get_IsoSFw();
    }
    return total;
}



double ytEventSelection::lept_weight(double elec_ID_weight, double elec_iso_weight, double muon_ID_weight, double muon_iso_weight)
{
    return elec_ID_weight * elec_iso_weight * muon_ID_weight * muon_iso_weight;
}



double ytEventSelection::jets_weight(vector<Jet> jets)
{
    double total = 1.;
    for (auto & jet_itr : jets) {
        total *= jet_itr.get_SFw() * jet_itr.get_JVTsf();
    }
    return total;
}



double ytEventSelection::calculate_weight(double event_weight, double PRW_weight, double lept_weight, double jets_weight)
{
    return event_weight * PRW_weight * lept_weight * jets_weight;
}



double ytEventSelection::calculate_weight(double event_weight, double PRW_weight, vector<Electron> elec, vector<Muon> muon, vector<Jet> jets)
{
    double total = event_weight * PRW_weight;
    for (auto & el_itr : elec) {
        total *= el_itr.get_SFwMediumLH() * el_itr.get_IsoSFwMediumLH();
    }
    for (auto & mu_itr : muon) {
        total *= mu_itr.get_SFw() * mu_itr.get_IsoSFw();
    }
    for (auto & jet_itr : jets) {
        total *= jet_itr.get_SFw() * jet_itr.get_JVTsf();
    }
    return total;
}



void ytEventSelection::update(int cut, bool passed, double weight)
{
    if (passed)
        sum_of_weight_at_cut[cut] += weight;
}



void ytEventSelection::debug_sum_of_weight_print()
{
    extern const char* cut_name[];
    cout << "**************************************************" << endl;
    cout << " Sum of EventWeight for each cut" << endl;
    cout << "**************************************************" << endl;
    for (int cuts = 0; cuts < Ncuts; cuts++) {
        //if (cuts == 14) {
        if (cuts == ee_channel_separation) {
            cout << "***** El-El channel *****" << endl;
            cout << setw(20) << left << cut_name[cuts]  << " = " << setw(10) << sum_of_weight_at_cut[cuts] << endl;
        }
        //else if (cuts == 19) {
        else if (cuts == emu_channel_separation) {
            cout << "***** El-Mu channel *****" << endl;
            cout << setw(20) << left << cut_name[cuts]  << " = " << setw(10) << sum_of_weight_at_cut[cuts] << endl;
        }
        //else if (cuts == 24) {
        else if (cuts == mumu_channel_separation) {
            cout << "***** Mu-Mu channel *****" << endl;
            cout << setw(20) << left << cut_name[cuts]  << " = " << setw(10) << sum_of_weight_at_cut[cuts] << endl;
        }
        else {
            cout << setw(20) << left << cut_name[cuts]  << " = " << setw(10) << sum_of_weight_at_cut[cuts] << endl;
        }
    }
    cout << "**************************************************" << endl;
}
*/


/*
//----------------------------------//
// make plots
//----------------------------------//
void ytEventSelection::make_plots_Nvtx()
{
    c_Nvtx = new TCanvas("c_Nvtx", "Nvtx", 800, 800);

    h_Nvtx->SetLineColor(kBlack);
    h_Nvtx->SetMarkerColor(kBlack);
    h_Nvtx->SetMarkerStyle(kFullCircle);
    h_Nvtx->SetStats(kFALSE);
    h_Nvtx->SetMaximum(max(h_Nvtx->GetMaximum(), h_Nvtx_PRW->GetMaximum()) * 1.2);
    h_Nvtx->SetMinimum(0.);
    h_Nvtx->SetTitle("");
    h_Nvtx->SetXTitle("Nvtx");
    h_Nvtx->SetYTitle("Events");
    h_Nvtx->Draw();

    h_Nvtx_PRW->SetLineColor(kRed);
    h_Nvtx_PRW->SetMarkerColor(kRed);
    h_Nvtx_PRW->SetMarkerStyle(kFullCircle);
    h_Nvtx_PRW->Draw("same");

    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.89);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry("h_Nvtx", "Nvtx");
    leg->AddEntry("h_Nvtx_PRW", "Nvtx (PRW)");
    leg->Draw();

    wk()->addOutput(c_Nvtx);
}



void ytEventSelection::make_plots_AvgMu()
{
    c_AvgMu = new TCanvas("c_AvgMu", "AvgMu", 800, 800);

    h_AvgMu->SetLineColor(kBlack);
    h_AvgMu->SetMarkerColor(kBlack);
    h_AvgMu->SetMarkerStyle(kFullCircle);
    h_AvgMu->SetStats(kFALSE);
    h_AvgMu->SetMaximum(max(h_AvgMu->GetMaximum(), h_AvgMu_PRW->GetMaximum()) * 1.2);
    h_AvgMu->SetMinimum(0.);
    h_AvgMu->SetTitle("");
    h_AvgMu->SetXTitle("AvgMu");
    h_AvgMu->SetYTitle("Events");
    h_AvgMu->Draw();

    h_AvgMu_PRW->SetLineColor(kRed);
    h_AvgMu_PRW->SetMarkerColor(kRed);
    h_AvgMu_PRW->SetMarkerStyle(kFullCircle);
    h_AvgMu_PRW->Draw("same");

    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.89);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry("h_AvgMu", "AvgMu");
    leg->AddEntry("h_AvgMu_PRW", "AvgMu (PRW)");
    leg->Draw();

    wk()->addOutput(c_AvgMu);
}



void ytEventSelection::make_plots_NLepts()
{
    c_NLepts = new TCanvas("c_NLepts", "NLepts", 800, 800);

    h_NLepts->SetLineColor(kBlack);
    h_NLepts->SetMarkerColor(kBlack);
    h_NLepts->SetMarkerStyle(kFullCircle);
    h_NLepts->SetStats(kFALSE);
    h_NLepts->SetMaximum(max(h_NLepts->GetMaximum(), h_NLepts_PRW->GetMaximum()) * 1.2);
    h_NLepts->SetMinimum(0.);
    h_NLepts->SetTitle("");
    h_NLepts->SetXTitle("NLepts");
    h_NLepts->SetYTitle("Events");
    h_NLepts->Draw();

    h_NLepts_PRW->SetLineColor(kRed);
    h_NLepts_PRW->SetMarkerColor(kRed);
    h_NLepts_PRW->SetMarkerStyle(kFullCircle);
    h_NLepts_PRW->Draw("same");

    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.89);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry("h_NLepts", "NLepts");
    leg->AddEntry("h_NLepts_PRW", "NLepts (PRW)");
    leg->Draw();

    wk()->addOutput(c_NLepts);
}



void ytEventSelection::make_plots_NJets()
{
    c_NJets = new TCanvas("c_NJets", "NJets", 800, 800);

    h_NJets->SetLineColor(kBlack);
    h_NJets->SetMarkerColor(kBlack);
    h_NJets->SetMarkerStyle(kFullCircle);
    h_NJets->SetStats(kFALSE);
    h_NJets->SetMaximum(max(h_NJets->GetMaximum(), h_NJets_PRW->GetMaximum()) * 1.2);
    h_NJets->SetMinimum(0.);
    h_NJets->SetTitle("");
    h_NJets->SetXTitle("NJets");
    h_NJets->SetYTitle("Events");
    h_NJets->Draw();

    h_NJets_PRW->SetLineColor(kRed);
    h_NJets_PRW->SetMarkerColor(kRed);
    h_NJets_PRW->SetMarkerStyle(kFullCircle);
    h_NJets_PRW->Draw("same");

    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.89);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry("h_NJets", "NJets");
    leg->AddEntry("h_NJets_PRW", "NJets (PRW)");
    leg->Draw();

    wk()->addOutput(c_NJets);
}
*/


//----------------------------------//
// Fill & set functions
//----------------------------------//
//#ifdef _IS_MC_
void ytEventSelection::fill_electrons(
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
    vector<bool>    *El_TrigMatch_e60_lhmedium_nod0)
{
    for (int i = 0; i < NEl; i++) {
        Electron el;
        el.set_index(i);
        el.set_number(NEl);
        el.set_flavor(flavor);
        el.set_eta( (*El_eta)[i] );
        el.set_etaclus( (*El_etaclus)[i] );
        el.set_phi( (*El_phi)[i] );
        el.set_pt( (*El_pT)[i] );
        el.set_E( (*El_E)[i] );
        el.set_charge( (*El_charge)[i] );
        el.set_sigd0( (*El_sigd0)[i] );
        el.set_z0sinTheta( (*El_z0sinTheta)[i] );
        el.set_d0pvtx( (*El_d0pvtx)[i] );
        el.set_passOR( (*El_passOR)[i] );
        el.set_SFwMediumLH( (*El_SFwMediumLH)[i] );
        el.set_IsoSFwMediumLH( (*El_IsoSFwMediumLH)[i] );
        el.set_SFwTightLH( (*El_SFwTightLH)[i] );
        el.set_SFwLooseAndBLayerLH( (*El_SFwLooseAndBLayerLH)[i] );
        el.set_SFwTrigMediumLH_e12_lhloose_L1EM10VH( (*El_SFwTrigMediumLH_e12_lhloose_L1EM10VH)[i] );
        el.set_SFwTrigMediumLH_e17_lhloose( (*El_SFwTrigMediumLH_e17_lhloose)[i] );
        el.set_SFwTrigMediumLH_single( (*El_SFwTrigMediumLH_single)[i] );
        el.set_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH( (*El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH)[i] );
        el.set_isLooseAndBLayerLH( (*El_isLooseAndBLayerLH)[i] );
        el.set_isMediumLH( (*El_isMediumLH)[i] );
        el.set_isTightLH( (*El_isTightLH)[i] );
        el.set_nBLayerHits( (*El_nBLayerHits)[i] );
        el.set_expectBLayerHit( (*El_expectBLayerHit)[i] );
        el.set_type( (*El_type)[i] );
        el.set_origin( (*El_origin)[i] );
        el.set_bkgMotherPdgId( (*El_bkgMotherPdgId)[i] );
        el.set_bkgOrigin( (*El_bkgOrigin)[i] );
        el.set_chFlip( (*El_chFlip)[i] );
        el.set_ptcone20( (*El_ptcone20)[i] );
        el.set_ptcone30( (*El_ptcone30)[i] );
        el.set_ptcone40( (*El_ptcone40)[i] );
        el.set_ptvarcone20( (*El_ptvarcone20)[i] );
        el.set_ptvarcone30( (*El_ptvarcone30)[i] );
        el.set_ptvarcone40( (*El_ptvarcone40)[i] );
        el.set_topoetcone20( (*El_topoetcone20)[i] );
        el.set_topoetcone30( (*El_topoetcone30)[i] );
        el.set_topoetcone40( (*El_topoetcone40)[i] );
        el.set_passIsoLooseTO( (*El_passIsoLooseTO)[i] );
        el.set_passIsoLoose( (*El_passIsoLoose)[i] );
        el.set_passIsoTight( (*El_passIsoTight)[i] );
        el.set_passIsoGrad( (*El_passIsoGrad)[i] );
        el.set_passIsoGradCustomTight( (*El_passIsoGradCustomTight)[i] );
        el.set_passIsoGradCustom( (*El_passIsoGradCustom)[i] );
        el.set_passIsoGradLoose( (*El_passIsoGradLoose)[i] );
        el.set_trigMatch_e12_lhloose_L1EM10VH( (*El_trigMatch_e12_lhloose_L1EM10VH)[i] );
        el.set_trigMatch_e17_lhloose( (*El_trigMatch_e17_lhloose)[i] );
        el.set_trigMatch_e60_lhmedium( (*El_trigMatch_e60_lhmedium)[i] );
        el.set_trigMatch_e24_lhmedium_iloose_L1EM20VH( (*El_trigMatch_e24_lhmedium_iloose_L1EM20VH)[i] );
        el.set_trigMatch_2e12_lhloose_L12EM10VH( (*El_trigMatch_2e12_lhloose_L12EM10VH)[i] );
        el.set_trigMatch_2e15_lhloose_L12EM10VH( (*El_trigMatch_2e15_lhloose_L12EM10VH)[i] );
        el.set_trigMatch_2e15_lhvloose_L12EM13VH( (*El_trigMatch_2e15_lhvloose_L12EM13VH)[i] );
        el.set_trigMatch_2e15_lhvloose_nod0_L12EM13VH( (*El_trigMatch_2e15_lhvloose_nod0_L12EM13VH)[i] );
        el.set_trigMatch_2e17_lhvloose_nod0( (*El_trigMatch_2e17_lhvloose_nod0)[i] );
        el.set_trigMatch_e17_lhloose_mu14( (*El_trigMatch_e17_lhloose_mu14)[i] );
        el.set_trigMatch_e17_lhloose_nod0_mu14( (*El_trigMatch_e17_lhloose_nod0_mu14)[i] );
        el.set_trigMatch_e24_lhmedium_nod0_ivarloose( (*El_TrigMatch_e24_lhmedium_nod0_ivarloose)[i] );
        el.set_trigMatch_e24_lhtight_nod0_ivarloose( (*El_TrigMatch_e24_lhtight_nod0_ivarloose)[i] );
        el.set_trigMatch_e60_lhmedium_nod0( (*El_TrigMatch_e60_lhmedium_nod0)[i] );
        el.set_TLV_E(el.get_pt(), el.get_eta(), el.get_phi(), el.get_E());
        vec_elec.push_back(el);
    }
}



void ytEventSelection::fill_muons(
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
    vector<vector<bool> > *Mu_trigMatchPair_mu22_mu8noL1)
{
    for (int i = 0; i < NMu; i++) {
        Muon mu;
        mu.set_index(i);
        mu.set_number(NMu);
        mu.set_flavor(flavor);
        mu.set_eta( (*Mu_eta)[i] );
        mu.set_phi( (*Mu_phi)[i] );
        mu.set_pt( (*Mu_pT)[i] );
        mu.set_SFw( (*Mu_SFw)[i] );
        mu.set_IsoSFw( (*Mu_IsoSFw)[i] );
        mu.set_charge( (*Mu_charge)[i] );
        mu.set_d0pvtx( (*Mu_d0pvtx)[i] );
        mu.set_sigd0( (*Mu_sigd0)[i] );
        mu.set_z0sinTheta( (*Mu_z0sinTheta)[i] );
        mu.set_isBad( (*Mu_isBad)[i] );
        mu.set_passOR( (*Mu_passOR)[i] );
        mu.set_isTight( (*Mu_isTight)[i] );
        mu.set_isCosmic( (*Mu_isCosmic)[i] );
        mu.set_type( (*Mu_type)[i] ); // MC only
        mu.set_origin( (*Mu_origin)[i] ); // MC only
        mu.set_ptcone20( (*Mu_ptcone20)[i] );
        mu.set_ptcone30( (*Mu_ptcone30)[i] );
        mu.set_ptcone40( (*Mu_ptcone40)[i] );
        mu.set_ptvarcone20( (*Mu_ptvarcone20)[i] );
        mu.set_ptvarcone30( (*Mu_ptvarcone30)[i] );
        mu.set_ptvarcone40( (*Mu_ptvarcone40)[i] );
        mu.set_topoetcone20( (*Mu_topoetcone20)[i] );
        mu.set_topoetcone30( (*Mu_topoetcone30)[i] );
        mu.set_topoetcone40( (*Mu_topoetcone40)[i] );
        mu.set_passIsoLooseTO( (*Mu_passIsoLooseTO)[i] );
        mu.set_passIsoLoose( (*Mu_passIsoLoose)[i] );
        mu.set_passIsoTight( (*Mu_passIsoTight)[i] );
        mu.set_passIsoGrad( (*Mu_passIsoGrad)[i] );
        mu.set_passIsoGradCustomTight( (*Mu_passIsoGradCustomTight)[i] );
        mu.set_passIsoGradCustom( (*Mu_passIsoGradCustom)[i] );
        mu.set_passIsoGradLoose( (*Mu_passIsoGradLoose)[i] );
        mu.set_MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50( MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50 );
        mu.set_trigMatch_mu26_imedium( (*Mu_trigMatch_mu26_imedium)[i] );
        mu.set_trigMatch_mu50( (*Mu_trigMatch_mu50)[i] );
        mu.set_trigMatch_mu8noL1( (*Mu_trigMatch_mu8noL1)[i] );
        mu.set_trigMatch_mu14( (*Mu_trigMatch_mu14)[i] );
        mu.set_trigMatch_mu18( (*Mu_trigMatch_mu18)[i] );
        mu.set_trigMatch_mu18_mu8noL1( (*Mu_trigMatch_mu18_mu8noL1)[i] );
        mu.set_trigMatch_e17_lhloose_mu14( (*Mu_trigMatch_e17_lhloose_mu14)[i] );
        mu.set_trigMatch_e17_lhloose_nod0_mu14( (*Mu_trigMatch_e17_lhloose_nod0_mu14)[i] );
        mu.set_trigMatch_mu20_mu8noL1( (*Mu_trigMatch_mu20_mu8noL1)[i] );
        mu.set_trigMatch_mu22_mu8noL1( (*Mu_trigMatch_mu22_mu8noL1)[i] );
        mu.set_trigMatch_mu24_iloose( (*Mu_TrigMatch_mu24_iloose)[i] );
        mu.set_trigMatch_mu24_ivarloose( (*Mu_TrigMatch_mu24_ivarloose)[i] );
        mu.set_trigMatch_mu24_iloose_L1MU15( (*Mu_TrigMatch_mu24_iloose_L1MU15)[i] );
        mu.set_trigMatch_mu24_ivarloose_L1MU15( (*Mu_TrigMatch_mu24_ivarloose_L1MU15)[i] );
        for (vector<bool>::iterator itr = (*Mu_trigMatchPair_mu18_mu8noL1)[i].begin();
             itr != (*Mu_trigMatchPair_mu18_mu8noL1)[i].end(); itr++) {
            int index = distance((*Mu_trigMatchPair_mu18_mu8noL1)[i].begin(), itr);
            mu.set_trigMatchPair_mu18_mu8noL1(index, *itr);
        }
        for (vector<bool>::iterator itr = (*Mu_trigMatchPair_mu20_mu8noL1)[i].begin();
             itr != (*Mu_trigMatchPair_mu20_mu8noL1)[i].end(); itr++) {
            int index = distance((*Mu_trigMatchPair_mu20_mu8noL1)[i].begin(), itr);
            mu.set_trigMatchPair_mu20_mu8noL1(index, *itr);
        }
        for (vector<bool>::iterator itr = (*Mu_trigMatchPair_mu22_mu8noL1)[i].begin();
             itr != (*Mu_trigMatchPair_mu22_mu8noL1)[i].end(); itr++) {
            int index = distance((*Mu_trigMatchPair_mu22_mu8noL1)[i].begin(), itr);
            mu.set_trigMatchPair_mu22_mu8noL1(index, *itr);
        }
        mu.set_TLV_M(mu.get_pt(), mu.get_eta(), mu.get_phi(), Mu_Mass);
        vec_muon.push_back(mu);
    }
}



void ytEventSelection::fill_jets(
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
    vector<bool>    *Jet_passOR)
{
    for (int i = 0; i < NJet; i++) {
        Jet je;
        je.set_index(i);
        je.set_number(NJet);
        je.set_eta( (*Jet_eta)[i] );
        je.set_phi( (*Jet_phi)[i] );
        je.set_pt( (*Jet_pT)[i] );
        je.set_E( (*Jet_E)[i] );
        je.set_quality( (*Jet_quality)[i] );
        je.set_JVT( (*Jet_JVT)[i] );
        je.set_JVTsf( (*Jet_JVTsf)[i] );
        je.set_MV2c20( (*Jet_MV2c20)[i] );
        je.set_MV2c10( (*Jet_MV2c10)[i] );
        je.set_SFw( (*Jet_SFw)[i] );
        je.set_ConeTruthLabel( (*Jet_ConeTruthLabel)[i] );
        je.set_PartonTruthLabel( (*Jet_PartonTruthLabel)[i] );
        je.set_HadronConeExclTruthLabel( (*Jet_HadronConeExclTruthLabel)[i] );
        je.set_deltaR( (*Jet_deltaR)[i] );
        je.set_nTrk( (*Jet_nTrk)[i] );
        je.set_passOR( (*Jet_passOR)[i]);
        je.set_TLV_E(je.get_pt(), je.get_eta(), je.get_phi(), je.get_E());
        vec_jets.push_back(je);
    }
}
//#endif // #ifdef _IS_MC_



//#ifdef _IS_DATA_
void ytEventSelection::fill_electrons(
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
    vector<bool>    *El_TrigMatch_e60_lhmedium_nod0)
{
    for (int i = 0; i < NEl; i++) {
        Electron el;
        el.set_index(i);
        el.set_number(NEl);
        el.set_flavor(flavor);
        el.set_eta( (*El_eta)[i] );
        el.set_etaclus( (*El_etaclus)[i] );
        el.set_phi( (*El_phi)[i] );
        el.set_pt( (*El_pT)[i] );
        el.set_E( (*El_E)[i] );
        el.set_charge( (*El_charge)[i] );
        el.set_sigd0( (*El_sigd0)[i] );
        el.set_z0sinTheta( (*El_z0sinTheta)[i] );
        el.set_d0pvtx( (*El_d0pvtx)[i] );
        el.set_passOR( (*El_passOR)[i] );
        el.set_SFwMediumLH( (*El_SFwMediumLH)[i] );
        el.set_IsoSFwMediumLH( (*El_IsoSFwMediumLH)[i] );
        el.set_SFwTightLH( (*El_SFwTightLH)[i] );
        el.set_SFwLooseAndBLayerLH( (*El_SFwLooseAndBLayerLH)[i] );
        el.set_SFwTrigMediumLH_e12_lhloose_L1EM10VH( (*El_SFwTrigMediumLH_e12_lhloose_L1EM10VH)[i] );
        el.set_SFwTrigMediumLH_e17_lhloose( (*El_SFwTrigMediumLH_e17_lhloose)[i] );
        el.set_SFwTrigMediumLH_single( (*El_SFwTrigMediumLH_single)[i] );
        el.set_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH( (*El_SFwTrigLooseAndBLayerLH_e12_lhloose_L1EM10VH)[i] );
        el.set_isLooseAndBLayerLH( (*El_isLooseAndBLayerLH)[i] );
        el.set_isMediumLH( (*El_isMediumLH)[i] );
        el.set_isTightLH( (*El_isTightLH)[i] );
        el.set_nBLayerHits( (*El_nBLayerHits)[i] );
        el.set_expectBLayerHit( (*El_expectBLayerHit)[i] );
        /*
        el.set_type( (*El_type)[i] ); // MC only
        el.set_origin( (*El_origin)[i] ); // MC only
        el.set_bkgMotherPdgId( (*El_bkgMotherPdgId)[i] ); // MC only
        el.set_bkgOrigin( (*El_bkgOrigin)[i] ); // MC only
        el.set_chFlip( (*El_chFlip)[i] ); // MC only
        */
        el.set_ptcone20( (*El_ptcone20)[i] );
        el.set_ptcone30( (*El_ptcone30)[i] );
        el.set_ptcone40( (*El_ptcone40)[i] );
        el.set_ptvarcone20( (*El_ptvarcone20)[i] );
        el.set_ptvarcone30( (*El_ptvarcone30)[i] );
        el.set_ptvarcone40( (*El_ptvarcone40)[i] );
        el.set_topoetcone20( (*El_topoetcone20)[i] );
        el.set_topoetcone30( (*El_topoetcone30)[i] );
        el.set_topoetcone40( (*El_topoetcone40)[i] );
        el.set_passIsoLooseTO( (*El_passIsoLooseTO)[i] );
        el.set_passIsoLoose( (*El_passIsoLoose)[i] );
        el.set_passIsoTight( (*El_passIsoTight)[i] );
        el.set_passIsoGrad( (*El_passIsoGrad)[i] );
        el.set_passIsoGradCustomTight( (*El_passIsoGradCustomTight)[i] );
        el.set_passIsoGradCustom( (*El_passIsoGradCustom)[i] );
        el.set_passIsoGradLoose( (*El_passIsoGradLoose)[i] );
        el.set_trigMatch_e12_lhloose_L1EM10VH( (*El_trigMatch_e12_lhloose_L1EM10VH)[i] );
        el.set_trigMatch_e17_lhloose( (*El_trigMatch_e17_lhloose)[i] );
        el.set_trigMatch_e60_lhmedium( (*El_trigMatch_e60_lhmedium)[i] );
        el.set_trigMatch_e24_lhmedium_iloose_L1EM20VH( (*El_trigMatch_e24_lhmedium_iloose_L1EM20VH)[i] );
        el.set_trigMatch_2e12_lhloose_L12EM10VH( (*El_trigMatch_2e12_lhloose_L12EM10VH)[i] );
        el.set_trigMatch_2e15_lhloose_L12EM10VH( (*El_trigMatch_2e15_lhloose_L12EM10VH)[i] );
        el.set_trigMatch_2e15_lhvloose_L12EM13VH( (*El_trigMatch_2e15_lhvloose_L12EM13VH)[i] );
        el.set_trigMatch_2e15_lhvloose_nod0_L12EM13VH( (*El_trigMatch_2e15_lhvloose_nod0_L12EM13VH)[i] );
        el.set_trigMatch_2e17_lhvloose_nod0( (*El_trigMatch_2e17_lhvloose_nod0)[i] );
        el.set_trigMatch_e17_lhloose_mu14( (*El_trigMatch_e17_lhloose_mu14)[i] );
        el.set_trigMatch_e17_lhloose_nod0_mu14( (*El_trigMatch_e17_lhloose_nod0_mu14)[i] );
        el.set_trigMatch_e24_lhmedium_nod0_ivarloose( (*El_TrigMatch_e24_lhmedium_nod0_ivarloose)[i] );
        el.set_trigMatch_e24_lhtight_nod0_ivarloose( (*El_TrigMatch_e24_lhtight_nod0_ivarloose)[i] );
        el.set_trigMatch_e60_lhmedium_nod0( (*El_TrigMatch_e60_lhmedium_nod0)[i] );
        el.set_TLV_E(el.get_pt(), el.get_eta(), el.get_phi(), el.get_E());
        vec_elec.push_back(el);
    }
}



void ytEventSelection::fill_muons(
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
    vector<vector<bool> > *Mu_trigMatchPair_mu22_mu8noL1)
{
    for (int i = 0; i < NMu; i++) {
        Muon mu;
        mu.set_index(i);
        mu.set_number(NMu);
        mu.set_flavor(flavor);
        mu.set_eta( (*Mu_eta)[i] );
        mu.set_phi( (*Mu_phi)[i] );
        mu.set_pt( (*Mu_pT)[i] );
        mu.set_SFw( (*Mu_SFw)[i] );
        mu.set_IsoSFw( (*Mu_IsoSFw)[i] );
        mu.set_charge( (*Mu_charge)[i] );
        mu.set_d0pvtx( (*Mu_d0pvtx)[i] );
        mu.set_sigd0( (*Mu_sigd0)[i] );
        mu.set_z0sinTheta( (*Mu_z0sinTheta)[i] );
        mu.set_isBad( (*Mu_isBad)[i] );
        mu.set_passOR( (*Mu_passOR)[i] );
        mu.set_isTight( (*Mu_isTight)[i] );
        mu.set_isCosmic( (*Mu_isCosmic)[i] );
        /*
        mu.set_type( (*Mu_type)[i] ); // MC only
        mu.set_origin( (*Mu_origin)[i] ); // MC only
        */
        mu.set_ptcone20( (*Mu_ptcone20)[i] );
        mu.set_ptcone30( (*Mu_ptcone30)[i] );
        mu.set_ptcone40( (*Mu_ptcone40)[i] );
        mu.set_ptvarcone20( (*Mu_ptvarcone20)[i] );
        mu.set_ptvarcone30( (*Mu_ptvarcone30)[i] );
        mu.set_ptvarcone40( (*Mu_ptvarcone40)[i] );
        mu.set_topoetcone20( (*Mu_topoetcone20)[i] );
        mu.set_topoetcone30( (*Mu_topoetcone30)[i] );
        mu.set_topoetcone40( (*Mu_topoetcone40)[i] );
        mu.set_passIsoLooseTO( (*Mu_passIsoLooseTO)[i] );
        mu.set_passIsoLoose( (*Mu_passIsoLoose)[i] );
        mu.set_passIsoTight( (*Mu_passIsoTight)[i] );
        mu.set_passIsoGrad( (*Mu_passIsoGrad)[i] );
        mu.set_passIsoGradCustomTight( (*Mu_passIsoGradCustomTight)[i] );
        mu.set_passIsoGradCustom( (*Mu_passIsoGradCustom)[i] );
        mu.set_passIsoGradLoose( (*Mu_passIsoGradLoose)[i] );
        mu.set_MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50( MuTrigSF_HLT_mu20_iloose_L1MU15_OR_HLT_mu50 );
        mu.set_trigMatch_mu26_imedium( (*Mu_trigMatch_mu26_imedium)[i] );
        mu.set_trigMatch_mu50( (*Mu_trigMatch_mu50)[i] );
        mu.set_trigMatch_mu8noL1( (*Mu_trigMatch_mu8noL1)[i] );
        mu.set_trigMatch_mu14( (*Mu_trigMatch_mu14)[i] );
        mu.set_trigMatch_mu18( (*Mu_trigMatch_mu18)[i] );
        mu.set_trigMatch_mu18_mu8noL1( (*Mu_trigMatch_mu18_mu8noL1)[i] );
        mu.set_trigMatch_e17_lhloose_mu14( (*Mu_trigMatch_e17_lhloose_mu14)[i] );
        mu.set_trigMatch_e17_lhloose_nod0_mu14( (*Mu_trigMatch_e17_lhloose_nod0_mu14)[i] );
        mu.set_trigMatch_mu20_mu8noL1( (*Mu_trigMatch_mu20_mu8noL1)[i] );
        mu.set_trigMatch_mu22_mu8noL1( (*Mu_trigMatch_mu22_mu8noL1)[i] );
        mu.set_trigMatch_mu24_iloose( (*Mu_TrigMatch_mu24_iloose)[i] );
        mu.set_trigMatch_mu24_ivarloose( (*Mu_TrigMatch_mu24_ivarloose)[i] );
        mu.set_trigMatch_mu24_iloose_L1MU15( (*Mu_TrigMatch_mu24_iloose_L1MU15)[i] );
        mu.set_trigMatch_mu24_ivarloose_L1MU15( (*Mu_TrigMatch_mu24_ivarloose_L1MU15)[i] );
        for (vector<bool>::iterator itr = (*Mu_trigMatchPair_mu18_mu8noL1)[i].begin();
             itr != (*Mu_trigMatchPair_mu18_mu8noL1)[i].end(); itr++) {
            int index = distance((*Mu_trigMatchPair_mu18_mu8noL1)[i].begin(), itr);
            mu.set_trigMatchPair_mu18_mu8noL1(index, *itr);
        }
        for (vector<bool>::iterator itr = (*Mu_trigMatchPair_mu20_mu8noL1)[i].begin();
             itr != (*Mu_trigMatchPair_mu20_mu8noL1)[i].end(); itr++) {
            int index = distance((*Mu_trigMatchPair_mu20_mu8noL1)[i].begin(), itr);
            mu.set_trigMatchPair_mu20_mu8noL1(index, *itr);
        }
        for (vector<bool>::iterator itr = (*Mu_trigMatchPair_mu22_mu8noL1)[i].begin();
             itr != (*Mu_trigMatchPair_mu22_mu8noL1)[i].end(); itr++) {
            int index = distance((*Mu_trigMatchPair_mu22_mu8noL1)[i].begin(), itr);
            mu.set_trigMatchPair_mu22_mu8noL1(index, *itr);
        }
        mu.set_TLV_M(mu.get_pt(), mu.get_eta(), mu.get_phi(), Mu_Mass);
        vec_muon.push_back(mu);
    }
}



void ytEventSelection::fill_jets(
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
    vector<bool>    *Jet_passOR)
{
    for (int i = 0; i < NJet; i++) {
        Jet je;
        je.set_index(i);
        je.set_number(NJet);
        je.set_eta( (*Jet_eta)[i] );
        je.set_phi( (*Jet_phi)[i] );
        je.set_pt( (*Jet_pT)[i] );
        je.set_E( (*Jet_E)[i] );
        je.set_quality( (*Jet_quality)[i] );
        je.set_JVT( (*Jet_JVT)[i] );
        je.set_JVTsf( (*Jet_JVTsf)[i] );
        je.set_MV2c20( (*Jet_MV2c20)[i] );
        je.set_MV2c10( (*Jet_MV2c10)[i] );
        je.set_SFw( (*Jet_SFw)[i] );
        /*
        je.set_ConeTruthLabel( (*Jet_ConeTruthLabel)[i] ); // MC only
        je.set_PartonTruthLabel( (*Jet_PartonTruthLabel)[i] ); // MC only
        je.set_HadronConeExclTruthLabel( (*Jet_HadronConeExclTruthLabel)[i] ); // MC only
        je.set_deltaR( (*Jet_deltaR)[i] ); // MC only
        */
        je.set_nTrk( (*Jet_nTrk)[i] );
        je.set_passOR( (*Jet_passOR)[i]);
        je.set_TLV_E(je.get_pt(), je.get_eta(), je.get_phi(), je.get_E());
        vec_jets.push_back(je);
    }
}
//#endif // #ifdef _IS_DATA_



void ytEventSelection::fill_leptons(vector<Electron> vec_elec, vector<Muon> vec_muon)
{
    for (auto & el_itr : vec_elec) {
        vec_lept.push_back(el_itr);
    }
    for (auto & mu_itr : vec_muon) {
        vec_lept.push_back(mu_itr);
    }
}



//
// baseline objects
//
void ytEventSelection::fill_baseline_electrons(vector<Electron> vec_elec)
{
    for (auto & el_itr : vec_elec) {
        if (el_itr.get_baseline() == true)
            vec_baseline_elec.push_back(el_itr);
    }
}



void ytEventSelection::fill_baseline_muons(vector<Muon> vec_muon)
{
    for (auto & mu_itr : vec_muon) {
        if (mu_itr.get_baseline() == true)
            vec_baseline_muon.push_back(mu_itr);
    }
}



void ytEventSelection::fill_baseline_leptons(vector<Electron> vec_elec, vector<Muon> vec_muon)
{
    for (auto & el_itr : vec_elec) {
        if (el_itr.get_baseline() == true)
            vec_baseline_lept.push_back(el_itr);
    }
    for (auto & mu_itr : vec_muon) {
        if (mu_itr.get_baseline() == true)
            vec_baseline_lept.push_back(mu_itr);
    }
}



void ytEventSelection::fill_baseline_jets(vector<Jet> vec_jets)
{
    for (auto & jet_itr : vec_jets) {
        if (jet_itr.get_baseline() == true) {
            vec_baseline_jets.push_back(jet_itr);
        }
    }
}



//
// pass OR objects
//
void ytEventSelection::fill_OR_electrons(vector<Electron> vec_elec)
{
    for (auto & el_itr : vec_elec) {
        if (el_itr.get_baseline() == true &&
            el_itr.get_passOR() == true) {
            vec_OR_elec.push_back(el_itr);
        }
    }
}



void ytEventSelection::fill_OR_muons(vector<Muon> vec_muon)
{
    for (auto & mu_itr : vec_muon) {
        if (mu_itr.get_baseline() == true &&
            mu_itr.get_passOR() == true) {
            vec_OR_muon.push_back(mu_itr);
        }
    }
}



void ytEventSelection::fill_OR_leptons(vector<Electron> vec_elec, vector<Muon> vec_muon)
{
    for (auto & el_itr : vec_elec) {
        if (el_itr.get_baseline() == true &&
            el_itr.get_passOR() == true) {
            vec_OR_lept.push_back(el_itr);
        }
    }
    for (auto & mu_itr : vec_muon) {
        if (mu_itr.get_baseline() == true &&
            mu_itr.get_passOR() == true) {
            vec_OR_lept.push_back(mu_itr);
        }
    }
}



void ytEventSelection::fill_OR_jets(vector<Jet> vec_jets)
{
    for (auto & jet_itr : vec_jets) {
        if (jet_itr.get_baseline() == true &&
            jet_itr.get_passOR() == true) {
            vec_OR_jets.push_back(jet_itr);
        }
    }
}



void ytEventSelection::fill_JVT_jets(vector<Jet> vec_jets)
{
    for (auto & jet_itr : vec_jets) {
        if (jet_itr.get_baseline() == true &&
            jet_itr.get_passOR() == true) {
            if (jet_itr.get_pt() < 60000. &&
                fabs(jet_itr.get_eta()) < 2.4 &&
                jet_itr.get_JVT() < 0.59) {
                jet_itr.set_baseline(0);
                continue;
            }
            else {
                vec_JVT_jets.push_back(jet_itr);
            }
        }
    }
}



//
// signal objects
//
void ytEventSelection::fill_signal_electrons(vector<Electron> vec_elec)
{
    for (auto & el_itr : vec_elec) {
        if (el_itr.get_baseline() == true &&
            el_itr.get_passOR() == true &&
            el_itr.get_isSignal() == true) {
            vec_signal_elec.push_back(el_itr);
        }
    }
}



void ytEventSelection::fill_signal_muons(vector<Muon> vec_muon)
{
    for (auto & mu_itr : vec_muon) {
        if (mu_itr.get_baseline() == true &&
            mu_itr.get_passOR() == true &&
            mu_itr.get_isSignal() == true) {
            vec_signal_muon.push_back(mu_itr);
        }
    }
}



void ytEventSelection::fill_signal_leptons(vector<Electron> signal_elec, vector<Muon> signal_muon)
{
    for (auto & el_itr : signal_elec) {
        vec_signal_lept.push_back(el_itr);
    }
    for (auto & mu_itr : signal_muon) {
        vec_signal_lept.push_back(mu_itr);
    }
}



void ytEventSelection::fill_signal_jets(vector<Jet> vec_jets)
{
    for (auto & jet_itr : vec_jets) {
        if (jet_itr.get_baseline() == true &&
            jet_itr.get_passOR() == true &&
            jet_itr.get_quality() < 0.5) { // 1=bad jet from SUSYToo ls IsGoodJet
            vec_signal_jets.push_back(jet_itr);
        }
    }
}



//
// Set methods
//
void ytEventSelection::set_baseline_and_signal_electrons()
{
    for (auto & el_itr : vec_elec) {
        // baseline electrons
        if (el_itr.get_isLooseAndBLayerLH() == true &&
            el_itr.get_pt() > 10000. &&
            fabs(el_itr.get_etaclus()) < 2.47 &&
            fabs(el_itr.get_sigd0()) < 5.) {
            // Reject electrons in the crack region: 1.37<|eta|<1.52
            // Use the cluster eta (detector level)
            if (fabs(el_itr.get_etaclus()) > 1.37 &&
                fabs(el_itr.get_etaclus()) < 1.52) {
                el_itr.set_baseline(0); // Not a baseline electron
            }
            else {
                el_itr.set_baseline(1);
            }
        }
        else {
            el_itr.set_baseline(0);
        }
        // signal electrons
        if (el_itr.get_isMediumLH() == true &&
            fabs(el_itr.get_eta()) <= 2. && // Use track eta this time
            el_itr.get_ptvarcone20() / el_itr.get_pt() < 0.06 &&
            el_itr.get_topoetcone20() / el_itr.get_pt() < 0.06 &&
            fabs(el_itr.get_z0sinTheta()) < 0.5) {
            el_itr.set_isSignal(1);
        }
        else {
            el_itr.set_isSignal(0);
        }
    }
}



void ytEventSelection::set_baseline_and_signal_muons()
{
    for (auto & mu_itr : vec_muon) {
        // baseline muons
        // Medium is already applied at the ntuple, you can assume all the muon have it applied
        if (mu_itr.get_pt() > 10000. &&
            fabs(mu_itr.get_eta()) < 2.5) {
            mu_itr.set_baseline(1);
        }
        else {
            mu_itr.set_baseline(0);
        }
        // signal muons
        if (mu_itr.get_ptvarcone30() / mu_itr.get_pt() < 0.06 &&
            fabs(mu_itr.get_sigd0()) < 3.0 &&
            fabs(mu_itr.get_z0sinTheta()) < 0.5) {
            mu_itr.set_isSignal(1);
        }
        else {
            mu_itr.set_isSignal(0);
        }
    }
}



void ytEventSelection::set_jets_and_bjets()
{
    // Default jets are AntiKt4EMTopo
    for (auto & jet_itr : vec_jets) {
        // Jets
        if (jet_itr.get_pt() > 20000. &&
            fabs(jet_itr.get_eta()) < 2.8) { // for jets, there is no cluster eta (there are more than 1 cluster per jet)
            jet_itr.set_baseline(1);
        }
        else {
            jet_itr.set_baseline(0);
        }
        // b-jets
        if (jet_itr.get_pt() > 20000. &&
            fabs(jet_itr.get_eta()) < 2.5 &&
            jet_itr.get_MV2c10() > 0.8244) {
            jet_itr.set_isBjet(1);
        }
        else {
            jet_itr.set_isBjet(0);
        }
    }
}



template <typename T>
bool sort_descending_Pt(T obj1, T obj2) // Cannot use bool sort_descending_Pt(const T & obj1, const T & obj2)
{
    return (obj1.get_TLV()).Pt() > (obj2.get_TLV()).Pt();
}



template <>  // explicit specialization
bool sort_descending_Pt(TLorentzVector tlv1, TLorentzVector tlv2)
{
    return tlv1.Pt() > tlv2.Pt();
}


/*
double calculate_Ht(vector<Lepton> signal_leptons, vector<Jet> signal_jets)
{
    double Ht = 0;
    for (auto & lep : signal_leptons) {
        Ht = Ht + lep.get_pt();
    }
    for (auto & jet : signal_jets) {
        Ht = Ht + jet.get_pt();
    }
    return Ht;
}



double calculate_Meff(double Ht, double Etmiss)
{
    // the scalar sum of the signal leptons pT, all signal jets pT, and ETmiss
    return  Ht + Etmiss;
}
*/
