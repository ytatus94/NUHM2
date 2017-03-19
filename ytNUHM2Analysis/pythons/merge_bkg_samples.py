#!/usr/bin/python
import os

bkg_samples_path = "/raid05/atlas/data/ss3l/ximo-v44/"

bkg_samples_list = [
    "user.jpoveda.t0789_v44.410080.MadGraphPythia8EvtGen_A14NNPDF23_4topSM.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361100.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wplusenu.DAOD_SUSY2.s2576_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361101.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wplusmunu.DAOD_SUSY2.s2576_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361102.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wplustaunu.DAOD_SUSY2.s2576_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361103.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wminusenu.DAOD_SUSY2.s2576_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361104.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wminusmunu.DAOD_SUSY2.s2576_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361105.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Wminustaunu.DAOD_SUSY2.s2576_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410066.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np0.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.410067.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np1.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.410068.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np2.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.410111.MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np0.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410112.MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np1.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410113.MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np0.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410114.MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np1.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410115.MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np0.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410116.MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np1.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.304014.MadGraphPythia8EvtGen_A14NNPDF23_3top_SM.DAOD_SUSY2.a766_r7676_output.root",
    "user.jpoveda.t0789_v44.361063.Sherpa_CT10_llll.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361064.Sherpa_CT10_lllvSFMinus.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361065.Sherpa_CT10_lllvOFMinus.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361066.Sherpa_CT10_lllvSFPlus.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361067.Sherpa_CT10_lllvOFPlus.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361069.Sherpa_CT10_llvvjj_ss_EW4.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.361070.Sherpa_CT10_llvvjj_ss_EW6.DAOD_SUSY2.s2608_r7676_output.root",
    "user.jpoveda.t0789_v44.361071.Sherpa_CT10_lllvjj_EW6.DAOD_SUSY2.s2726_r7676_output.root",
    "user.jpoveda.t0789_v44.361072.Sherpa_CT10_lllljj_EW6.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361073.Sherpa_CT10_ggllll.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361625.Sherpa_CT10_ZZZ_6l0v.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361626.Sherpa_CT10_ZZZ_4l2v.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361627.Sherpa_CT10_ZZZ_2l4v.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361068.Sherpa_CT10_llvv.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.361077.Sherpa_CT10_ggllvv.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361084.Sherpa_CT10_WqqZll.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.361086.Sherpa_CT10_ZqqZll.DAOD_SUSY2.s2608_r7772_p2666_output.root",
    "user.jpoveda.t0789_v44.410011.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top.DAOD_SUSY2.a766_r7676_output.root",
    "user.jpoveda.t0789_v44.410012.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop.DAOD_SUSY2.a766_r7676_output.root",
    "user.jpoveda.t0789_v44.410013.PowhegPythiaEvtGen_P2012_Wt_inclusive_top.DAOD_SUSY2.a766_r7676_output.root",
    "user.jpoveda.t0789_v44.410014.PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop.DAOD_SUSY2.a766_r7676_output.root",
    "user.jpoveda.t0789_v44.410015.PowhegPythiaEvtGen_P2012_Wt_dilepton_top.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.410016.PowhegPythiaEvtGen_P2012_Wt_dilepton_antitop.DAOD_SUSY2.s2608_r7725_p2666_output.root",
    "user.jpoveda.t0789_v44.407009.PowhegPythiaEvtGen_P2012CT10_ttbarHT6c_1k_hdamp172p5_.DAOD_SUSY2.s2608_r7676_output.root",
    "user.jpoveda.t0789_v44.407010.PowhegPythiaEvtGen_P2012CT10_ttbarHT1k_1k5_hdamp172p5.DAOD_SUSY2.s2608_r7676_output.root",
    "user.jpoveda.t0789_v44.407011.PowhegPythiaEvtGen_P2012CT10_ttbarHT1k5_hdamp172p5_no.DAOD_SUSY2.s2608_r7676_output.root"
]

for dataset in bkg_samples_list:
    if os.path.isdir(bkg_samples_path + dataset):
        command = "hadd "
        if "4topSM" in dataset:
            merged_root = "4topSM"
        elif "Wplusenu" in dataset:
            merged_root = "Wplusenu"
        elif "Wplusmunu" in dataset:
            merged_root = "Wplusmunu"
        elif "Wplustaunu" in dataset:
            merged_root = "Wplustaunu"
        elif "Wminusenu" in dataset:
            merged_root = "Wminusenu"
        elif "Wminusmunu" in dataset:
            merged_root = "Wminusmunu"
        elif "Wminustaunu" in dataset:
            merged_root = "Wminustaunu"
        elif "Ztautau" in dataset:
            merged_root = "Ztautau"
        elif "ttW_Np0" in dataset:
            merged_root = "ttW_Np0"
        elif "ttW_Np1" in dataset:
            merged_root = "ttW_Np1"
        elif "ttW_Np2" in dataset:
            merged_root = "ttW_Np2"
        elif "ttee_Np0" in dataset:
            merged_root = "ttee_Np0"
        elif "ttee_Np1" in dataset:
            merged_root = "ttee_Np1"
        elif "ttmumu_Np0" in dataset:
            merged_root = "ttmumu_Np0"
        elif "ttmumu_Np1" in dataset:
            merged_root = "ttmumu_Np1"
        elif "tttautau_Np0" in dataset:
            merged_root = "tttautau_Np0"
        elif "tttautau_Np1" in dataset:
            merged_root = "tttautau_Np1"
        elif "ttbarWW" in dataset:
            merged_root = "ttbarWW"
        elif "3top_SM" in dataset:
            merged_root = "3top_SM"
        elif "Sherpa_CT10_llll" in dataset:
            merged_root = "llll"
        elif "Sherpa_CT10_lllvSFMinus" in dataset:
            merged_root = "lllvSFMinus"
        elif "Sherpa_CT10_lllvOFMinus" in dataset:
            merged_root = "lllvOFMinus"
        elif "Sherpa_CT10_lllvSFPlus" in dataset:
            merged_root = "lllvSFPlus"
        elif "Sherpa_CT10_lllvOFPlus" in dataset:
            merged_root = "lllvOFPlus"
        elif "Sherpa_CT10_llvvjj_ss_EW4" in dataset:
            merged_root = "llvvjj_ss_EW4"
        elif "Sherpa_CT10_llvvjj_ss_EW6" in dataset:
            merged_root = "llvvjj_ss_EW6"
        elif "Sherpa_CT10_lllvjj_EW6" in dataset:
            merged_root = "lllvjj_EW6"
        elif "Sherpa_CT10_lllljj_EW6" in dataset:
            merged_root = "lllljj_EW6"
        elif "Sherpa_CT10_ggllll" in dataset:
            merged_root = "ggllll"
        elif "Sherpa_CT10_ZZZ_6l0v" in dataset:
            merged_root = "ZZZ_6l0v"
        elif "Sherpa_CT10_ZZZ_4l2v" in dataset:
            merged_root = "ZZZ_4l2v"
        elif "Sherpa_CT10_ZZZ_2l4v" in dataset:
            merged_root = "ZZZ_2l4v"
        elif "Sherpa_CT10_llvv" in dataset:
            merged_root = "llvv"
        elif "Sherpa_CT10_ggllvv" in dataset:
            merged_root = "ggllvv"
        elif "Sherpa_CT10_WqqZll" in dataset:
            merged_root = "WqqZll"
        elif "Sherpa_CT10_ZqqZll" in dataset:
            merged_root = "ZqqZll"
        elif "singletop_tchan_lept_top" in dataset:
            merged_root = "singletop_tchan_lept_top"
        elif "singletop_tchan_lept_antitop" in dataset:
            merged_root = "singletop_tchan_lept_antitop"
        elif "Wt_inclusive_top" in dataset:
            merged_root = "Wt_inclusive_top"
        elif "Wt_inclusive_antitop" in dataset:
            merged_root = "Wt_inclusive_antitop"
        elif "Wt_dilepton_top" in dataset:
            merged_root = "Wt_dilepton_top"
        elif "Wt_dilepton_antitop" in dataset:
            merged_root = "Wt_dilepton_antitop"
        elif "ttbarHT6c_1k_hdamp172p5" in dataset:
            merged_root = "ttbarHT6c_1k_hdamp172p5"
        elif "ttbarHT1k_1k5_hdamp172p5" in dataset:
            merged_root = "ttbarHT1k_1k5_hdamp172p5"
        elif "ttbarHT1k5_hdamp172p5_no" in dataset:
            merged_root = "ttbarHT1k5_hdamp172p5"

        command += merged_root + "_merged.root "
        for root_file in sorted(os.listdir(bkg_samples_path + dataset)):
            if root_file.startswith("user.jpoveda.") and root_file.endswith(".output.root"):
                command += bkg_samples_path + dataset + "/" + root_file + " "
        #print command
        os.system(command)