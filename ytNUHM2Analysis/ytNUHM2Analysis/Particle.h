#ifndef PARTICLE_H
#define PARTICLE_H

#include <TLorentzVector.h>

class Particle {
    // user defined variables
    TLorentzVector tlv;
    bool    baseline;
    bool    passOR;
    int     index;
    // ntuple variables
    int     number;
    double  pT;
    double  eta;
    double  phi;
    double  E;
    double  M;

public:
    Particle() : baseline(0), passOR(0), number(0), pT(0), eta(0), phi(0), E(0), M(0) {}
    ~Particle() {}
    // get methods
    TLorentzVector get_TLV()    { return tlv; }
    bool    get_baseline()      { return baseline; }
    bool    get_passOR()        { return passOR; }
    int     get_index()         { return index; }
    int     get_number()        { return number; }
    double  get_pt()            { return pT; }
    double  get_eta()           { return eta; }
    double  get_phi()           { return phi; }
    double  get_E()             { return E; }
    double  get_M()             { return M; }
    // set methods
    virtual void set_TLV_E(double Pt, double Eta, double Phi, double Energy) { tlv.SetPtEtaPhiE(Pt, Eta, Phi, Energy); }
    virtual void set_TLV_M(double Pt, double Eta, double Phi, double Mass)   { tlv.SetPtEtaPhiM(Pt, Eta, Phi, Mass); }
    void    set_baseline(bool b)    { baseline = b; }
    void    set_passOR(bool b)      { passOR = b; }
    void    set_index(int i)        { index = i; }
    void    set_number(int i)       { number = i; }
    void    set_pt(double d)        { pT = d; }
    void    set_eta(double d)       { eta = d; }
    void    set_phi(double d)       { phi = d; }
    void    set_E(double d)         { E = d; }
    void    set_M(double d)         { M = d; }
};

#endif // #define PARTICLE_H
