#define YT_USEFUL_FUNCTIONS_CXX
#include "ytNUHM2Analysis/yt_useful_functions.h"

// Function definitions
/*
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
*/


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



double calculate_mll(vector<Lepton> leptons)
{
	// calculate the invariant mass of the 2 leading leptons
	if (leptons.size() < 2)
		return -999.;

	TLorentzVector tlv_lept_0, tlv_lept_1;
	tlv_lept_0.SetPtEtaPhiM(leptons.at(0).get_pt(), leptons.at(0).get_eta(), leptons.at(0).get_phi(), leptons.at(0).get_M());
	tlv_lept_1.SetPtEtaPhiM(leptons.at(1).get_pt(), leptons.at(1).get_eta(), leptons.at(1).get_phi(), leptons.at(1).get_M());
	double mll = (tlv_lept_0 + tlv_lept_1).M();
	return mll;
}



double calculate_mjj(vector<Jet> jets)
{
	// calculate the invariant mass of the 2 leading jets
	if (jets.size() < 2) 
		return -999.;

	TLorentzVector tlv_jets_0, tlv_jets_1;
	tlv_jets_0.SetPtEtaPhiE(jets.at(0).get_pt(), jets.at(0).get_eta(), jets.at(0).get_phi(), jets.at(0).get_E());
	tlv_jets_1.SetPtEtaPhiE(jets.at(1).get_pt(), jets.at(1).get_eta(), jets.at(1).get_phi(), jets.at(1).get_E());
	double mjj = (tlv_jets_0 + tlv_jets_1).M();
	return mjj;
}



//----------------------------------//
// Calculate weight functions
//----------------------------------//
double ID_weight(vector<Electron> elec, bool signal)
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



double ID_weight(vector<Muon> muon)
{
	double total = 1.;
	for (auto & mu_itr : muon) {
		total *= mu_itr.get_SFw();
	}
	return total;
}



double Iso_weight(vector<Electron> elec)
{
	double total = 1.;
	for (auto & el_itr : elec) {
		total *= el_itr.get_IsoSFwMediumLH();
	}
	return total;
}



double Iso_weight(vector<Muon> muon)
{
	double total = 1.;
	for (auto & mu_itr : muon) {
		total *= mu_itr.get_IsoSFw();
	}
	return total;
}



double lept_weight(double elec_ID_weight, double elec_iso_weight, double muon_ID_weight, double muon_iso_weight)
{
	return elec_ID_weight * elec_iso_weight * muon_ID_weight * muon_iso_weight;
}



double jets_weight(vector<Jet> jets)
{
	double total = 1.;
	for (auto & jet_itr : jets) {
		total *= jet_itr.get_SFw() * jet_itr.get_JVTsf();
	}
	return total;
}



double calculate_weight(double event_weight, double PRW_weight, double lept_weight, double jets_weight)
{
	return event_weight * PRW_weight * lept_weight * jets_weight;
}



double calculate_weight(double event_weight, double PRW_weight, vector<Electron> elec, vector<Muon> muon, vector<Jet> jets)
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


/*
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