#ifdef YT_SELECTOR_CXX
#ifndef USEFUL_FUNCTIONS_CXX
#define USEFUL_FUNCTIONS_CXX

// Functions declarations
//template<typename T> bool sort_descending_Pt(T obj1, T obj2); // NOTE: cannot use const ref for arguments.
//template<> bool sort_descending_Pt(TLorentzVector tlv1, TLorentzVector tlv2); // explicit specialization
double calculate_Ht(vector<Lepton> signal_leptons, vector<Jet> signal_jets);
double calculate_Meff(double Ht, double Etmiss);
double calculate_mll(vector<Lepton> leptons);
double calculate_mjj(vector<Jet> jets);
/*
// Function definitions
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

#endif // #define USEFUL_FUNCTIONS_CXX
#endif // #ifdef YT_SELECTOR_CXX
