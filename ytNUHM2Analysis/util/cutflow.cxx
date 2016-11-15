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
	//std::string submitDir = "submitDir";
	std::string submitDir;
	//if( argc > 1 ) submitDir = argv[ 1 ];

	bool isMC = false;
	bool isData = false;
	string process;
	bool use_Condor = false;
	bool use_Grid = false;
	bool use_PROOF = false;

	const char* bkg_sample;

	for (int i = 1; i < argc; i++) {
		// For running signal
		//const char *key = argv[i];
		// For running bkg:
		const char* key = strtok(argv[i], "=");
		const char* val = strtok(0, " ");
		// Check MC or data.
		if (strcmp(key, "isMC") == 0)
			isMC = true;
		else if (strcmp(key, "isData") == 0)
			isData = true;
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
			
		// Specify the driver to run.
		else if (strcmp(key, "Condor") == 0)
			use_Condor = true;
		else if (strcmp(key, "Grid") == 0)
			use_Grid = true;
		else if (strcmp(key, "PROOF") == 0)
			use_PROOF = true;
	}

	printf("isMC = %s, isData = %s\n", isMC ? "true" : "false", isData ? "true" : "false");

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

	if (isMC && !process.empty()) {
		cout << "process = " << process << endl;
		if (process == "bkg")
			cout << "bkg sample =" << bkg_sample << endl;
	}

	if (isMC) {
		submitDir = "cutflow_MC_" + process;
		if (process == "bkg")
			submitDir += "_" + string(bkg_sample);
	}
	else if (isData)
		submitDir = "cutflow_Data";
	cout << "submitDir = " << submitDir << endl;	

	// Construct the samples to run on:
	SH::SampleHandler sh;

	// use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
	const char* inputFilePath;
	const char* NUHM2_inputFilePath;
	const char* bkg_inputFilePath;

	if (isMC) {
		cout << "Read MC files..." << endl;
		inputFilePath = "/raid05/users/shen/Ximo_ntuples/v44/MC"; // no slash (/) at the end.
		NUHM2_inputFilePath = "/raid05/users/walker/yuting/data/v44"; // no slash (/) at the end.
		bkg_inputFilePath = "/raid05//users/walker/othmane/data/ximo-v44/"; // no slash (/) at the end.

		// For cutflow study
		if (process == "4topSM") {
			//inputFilePath = "/UserDisk2/yushen/Ximo_ntuples/v44/MC/user.jpoveda.t0789_v44.410080.MadGraphPythia8EvtGen_A14NNPDF23_4topSM.DAOD_SUSY2.s2608_r7725_p2666_output.root";
			//SH::ScanDir().filePattern("user.jpoveda.9048853._000001.output.root").scan(sh, inputFilePath);
			SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*4topSM*").scan(sh, inputFilePath); // Get all root files in this dataset
		}
		// For real lepton efficiency study
		else if (process == "Zee") {
			//SH::ScanDir().filePattern("Zee_merged.root").scan(sh, inputFilePath); // Get specific root file
			SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*Zee*").scan(sh, inputFilePath); // Get all root files in this dataset
		}
		else if (process == "Zmumu") {
			//SH::ScanDir().filePattern("Zmumu_merged.root").scan(sh, inputFilePath); // Get specific root file
			SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*Zmumu*").scan(sh, inputFilePath); // Get all root files in this dataset
		}
		else if (process == "ttbar") {
			//SH::ScanDir().filePattern("ttbar_merged.root").scan(sh, inputFilePath); // Get specific root file
			SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*ttbar*nonallhad*").scan(sh, inputFilePath); // Get all root files in this dataset
		}
		else if (process == "GG_ttn1") {
			SH::ScanDir().filePattern("GG_ttn1_merged.root").scan(sh, inputFilePath); // Get specific root file
			//SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*GG_ttn1_*.root").scan(sh, inputFilePath); // Get all root files in this dataset
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
	}
	else if (isData) {
		cout << "Read Data files..." << endl;
		inputFilePath = "/raid05/users/shen/Ximo_ntuples/v44/Data"; // no slash (/) at the end.
		//SH::ScanDir().scan(sh, inputFilePath); // Get all datasets in inputFilePath
		//SH::ScanDir().filePattern("merged_all_data.root").scan(sh, inputFilePath); // Get specific root file
		SH::ScanDir().samplePattern("user.jpoveda.t0789_v44.*.physics_Main.DAOD_SUSY2.*").scan(sh, inputFilePath); // Get all root files in this dataset
	}

	// Set the name of the input TTree.
	sh.setMetaString("nc_tree", "AnaNtup");

	// Print what we found:
	sh.print();

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
	alg->set_isFullSim(isFullSim);
	alg->set_isAF2Sim(isAF2Sim);
	if (isMC && !process.empty())
		alg->set_process(process);
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