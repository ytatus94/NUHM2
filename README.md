# NUHM2
This is the framework for NUHM2 SR optimization.

### v03. Add background samples
* The background samples are added in the `cutflow.cxx`

### v02. Pass the correct value of `DerivationStat_Weight`
* If there are more than one root file in the dataset, the new way can pass the correct total `DerivationStat_Weight` to `yt_optimization` class.


### v01. First commit.
The current version should work and was testing using v44 4topSM sample.
But I have to run over all v44 bkg samples and compare with Ximo's results to make sure the results are correct. 
