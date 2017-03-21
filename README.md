# NUHM2
This is the framework for NUHM2 SR optimization.


### v17. Modify method 2
1. `yt_optimization.cxx`::
   * Define constant `N_lepts`, `N_bjets`, `Bjet_pT`, and `N_jets`
   * Use the above constant in method 2
2. `ytOptimization_plots.C`: Plot cosmatic
3. Use the sam MET and Meff as what Chris did in `yt_optimization.cxx`, `ytOptimization_plots.C`, 
   and `ytOptimization_plots_method2.C`
4. `yt_optimization.h`: Change the number of elements in MET and Meff arrays definition


### v16. Set unreasonable significance to 0
1. `yt_optimization.cxx`:
   * Comment out the branches to reduce root file size
   * Change `n_lept_cuts` to `N_lept_cuts`
   * Change `n_bjet_cuts` to `N_bjet_cuts`
   * Change `n_jets_cuts` to `N_jets_cuts`
   * Uncomment method 1 and comment method 2
2. `ytOptimization_plots.C`:
   * Use `signal_cross_section_kfactor_efficiency` value from SUSYTools.
   * Modify `hist_title`.
   * Comment unused `signal_derivation_stat_weights`, `signal_cross_section_kfactor_efficiency`,
     `background_derivation_stat_weights`, and `background_cross_section_kfactor_efficiency`.
   * Add some new couts
   * Set significance to 0 if it is less then zero or reater then 100.
   * Plot cosmatic


### v15. Add new script for method2
1. `ytOptimization_plots_method2.C`: New script
2. `ytOptimization_plots.C`: change the type of luminosity to `float`
3. `yt_optimization.cxx`:
   * Remove unused branches
   * Calculate `lepton1_pT`, `lepton2_pT`, `jet1_pT`, and `jet2_pT`
   * Add some couts in method2 for loop


### v14. Add new method 2
1. `ytEventSelection.cxx`: `derivation_stat_weights` becomes an input of optimization
2. `yt_optimization.h`:
   * Add TH1F histogram `h_met_over_meff`
   * Add TH3F histograms `h_method2_yields` and `h_method2_yields_weighted`
   * Add new TTree leafs
   * Change the cuts type form `const int` to `const float` because TH3F uses `float`
   * Add binnings vector.
   * Don't fill histogram in `set_derivation_stat_weights()`
   * Add new method `set_binning_default()`
3. `yt_optimization.cxx`:
   * Change the cuts type form `const int` to `const float`.
   * Initialize new leafs to 0 or empty the vector.
   * Add new branches for new leafs.
   * Fill `h_derivation_stat_weights` after it is created.
   * Initialize new histograms `h_method2_yields`, `h_method2_yields_weighted`
   * Correct the `weight` calculation.
   * Calculate new variable `met_over_meff`
   * Comment out method 1 and add method 2.
   * Implement `set_binning_default()`


### v13. Change the weight calculation
1. `ytEventSelection.cxx`: pass luminosity to optimization
2. `yt_optimization.cxx`: Change the calculation of weight
3. `ytOptimization_plots.C`: Comment the weight calculation because the root file contains correct weight now.


### v12. Fix typo
1. `run_bkg_samples.sh`: Use `lllvjj_EW6` (not `llvvjj_EW6`)
2. `cutflow.cxx`: Use `lllvjj_EW6` (not `llvvjj_EW6`)


### v11. New script to merge the input and modify script to run signal and background.
1. `merge_bkg_samples.py`: New script
2. `run_bkg_samples.sh`: Re-direct output
3. `run_signal_samples.sh`: Re-direct output
4. `cutflow.cxx`: Change the input path


### v10. Use mu20_mu8noL1 trigger
1. `ytEventSelection.cxx`: Use `mu20_mu8noL1` for v44 (not `mu22_mu8noL1`)
2. `yt_cutflows.cxx`: Use `mu20_mu8noL1`
3. The cutflow results agree


### v09. Cutflow agree
1. `ytEventSelection.cxx`: Use `PRWrandomRunNumber` instead of using `random_run_number`
2. Change the comment style about skim part


### v08. Modify the code for optimization.
1. `LinkDef.h`: Add `DileptonTriggerWeight.h`
2. `ytEventSelection.h`:
   * Comment `yt_skim` related part
   * Comment the weighted histograms
   * Comment the canvas
   * Add `set_derivation_stat_weights()`
3. `yt_optimization.h`: Arrange the order of codes.
4. `run_bkg_samples.sh` and `run_signal_samples.sh`: Add optimization in the script.
5. `yt_useful_functions.cxx`: Use `SetPtEtaPhiM()` for the muon and define mu mass
6. `yt_cutflows.cxx`: Comment `pileupwgh`
7. `yt_optimization.cxx`:
   * Uncomment `h_derivation_stat_weights`
   * Use GeV for pT, met, meff
8. `cutflow.cxx`:
   * Add `isCutflow` and `isOptimization` flag
   * Change `submitDir` based on the flag
   * Use `merged_all_data.root`
9. `ytEventSelection.cxx`:
   * Comment the skim related part, weighted histograms, `derivation_stat_weights` and the part to make plots
   * The Xsec relation calculationis are separated into signal and bkg. And correct the value if the sample doesn't be found.
   * The Nvtx, AvgMu, NLeps, Njets, mll histograms are recalculated using the correct weight.


### v07. Add histograms of AvgMu and mll.
1. `yt_cutflows.h`: Add 3 methods:
   * `get_mc_random_event_number()`
   * `get_AvgMu()`
   * `get_pileup_weight()`
2. `ytEventSelection.h:
   * Add `yt_skim`
   * Add weighted and unweighted histograms of OSee and OSmumu AvgMu and mll.
   * Add new method `set_isOptimization()`
3. `yt_cutflows.cxx`:
   * Comment the argument `PRW_weight` and calculate the `pileupwgh` by myself.
   * Implement `get_AvgMu()` and `get_pileup_weight()`
4. `ytEventSelection.cxx`:
   * `initialize()`: put skim and optimization in the if condition
   * Add codes for declaring and filling AvgMu and mll histograms
   * Calculate `random_run_number` and `pileup_weight`
   * Correct the calculation of MC weight
   * Use `random_run_number` instead of using `PRWrandomRunNumber`


### v06. Put cut 7 and cut 9 in `if` condition
1. Put cut 7 and cut 9 in `if(!isSkim)`
2. Add `tag_pt_threshold` in `ytEventSelection`


### v05. Change histogram names
* Change histogram name from h_XXX_PRW to h_XXX_weighted
* histograms are separated into data and mc.


### v04. Add two scripts
* Add `run_bkg_samples.sh` and `run_signal_samples.sh` to run over all samples.


### v03. Add background samples
* The background samples are added in the `cutflow.cxx`


### v02. Pass the correct value of `DerivationStat_Weight`
* If there are more than one root file in the dataset, the new way can pass the correct total `DerivationStat_Weight` to `yt_optimization` class.


### v01. First commit.
The current version should work and was testing using v44 4topSM sample.
But I have to run over all v44 bkg samples and compare with Ximo's results to make sure the results are correct. 
