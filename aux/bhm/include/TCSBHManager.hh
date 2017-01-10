#ifndef TCSBHManager_h
#define TCSBHManager_h 1

#include <TFile.h>
//#include <TTree.h>
#include <TChain.h>

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh"    
#include <vector>

#include <TLorentzVector.h>
//#include <G4LorentzVector.hh>

using namespace std;

//class TFile;
//class TTree;

enum tcs_particle { em, ep, p };

class TCSBHManager {

public:

  TCSBHManager();
  TCSBHManager(string root_file_name);
  ~TCSBHManager();
   
  void Init();

  struct tcs_vars {
    double psf;
    double Q2;
    double t;
    double s;
    double xi;
    double tau, eta;
    double phi_cm, the_cm;
    double Eg;
    double flux_factor;
    double crs_BH;
  };

  tcs_vars GetTCSVars();

  //  enum class tcs_particle { em, ep, p };

  TLorentzVector GetLV(tcs_particle);

  void GetEntry(long ientry);

//  void FillTrees();

//  void PrintStatistic();
        
private:

  string   fRootFileName;
  TFile*   fRootFile;

  TChain*   fChain;

  TLorentzVector* flvEm;
  TLorentzVector* flvEp;
  TLorentzVector* flvPro;

  tcs_vars fTCSVars;

  long fNEntries;

};

#endif
