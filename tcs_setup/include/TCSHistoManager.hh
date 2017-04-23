//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#ifndef TCSHistoManager_h
#define TCSHistoManager_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh"    
#include <vector>
#include <fstream>

using namespace std;

class TFile;
class TTree;
class TH1D;

struct HodoHitContainer {
  vector<int> Det;
  vector<uint> Chan;
  vector<double> Edep;
  vector<int> PID;
};

struct TrackerHitContainer {
  vector<int> Det;
  vector<uint> Chan;
  vector<double> Edep;
  vector<int> PID;
};

const G4int MaxHisto = 5;

class TCSHistoManager {

public:
  
  TCSHistoManager();
  TCSHistoManager(char*,char*);
  ~TCSHistoManager();
   
  void book();
  void save();

  void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
  void Normalize(G4int id, G4double fac);    

  void AddHit(int det, uint col, uint row, double edep, int pid);
  void AddHit(int det, uint chan, double edep, int pid,
	      HodoHitContainer &HodoHitCont);
  void AddHit(int det, uint chan, double edep, int pid,
	      TrackerHitContainer &TrackerHitCont);

  bool CheckCaloHitCont() {
    uint sz = fCaloHitCont.Det.size();
    return (fCaloHitCont.Col.size() != sz || fCaloHitCont.Row.size() != sz ||
	    fCaloHitCont.Edep.size() != sz || fCaloHitCont.PID.size() != sz
	    ? false : true);
  }

  bool CheckHodoHitCont(HodoHitContainer &HodoHitCont) {
    uint sz = HodoHitCont.Det.size();
    return (HodoHitCont.Chan.size() != sz || HodoHitCont.Edep.size() != sz
	    || HodoHitCont.PID.size() != sz
            ? false : true);
  }

  bool CheckTrackerHitCont(TrackerHitContainer &TrackerHitCont) {
    uint sz = TrackerHitCont.Det.size();
    return (TrackerHitCont.Chan.size() != sz || TrackerHitCont.Edep.size() != sz
	    || TrackerHitCont.PID.size() != sz
            ? false : true);
  }

  void ResetCalo() {
    fCaloHitCont.Det.clear();
    fCaloHitCont.Col.clear();
    fCaloHitCont.Row.clear();
    fCaloHitCont.Edep.clear();
    fCaloHitCont.PID.clear();
  };

  void ResetHodo(HodoHitContainer &HodoHitCont) {
    HodoHitCont.Det.clear();
    HodoHitCont.Chan.clear();
    HodoHitCont.Edep.clear();
    HodoHitCont.PID.clear();
  };

  void ResetTracker(TrackerHitContainer &TrackerHitCont) {
    TrackerHitCont.Det.clear();
    TrackerHitCont.Chan.clear();
    TrackerHitCont.Edep.clear();
    TrackerHitCont.PID.clear();
  };

  void Reset() {
    ResetCalo();
    ResetHodo(fHodoXHitCont);
    ResetHodo(fHodoYHitCont);
    ResetTracker(fTrackerXHitCont);
    ResetTracker(fTrackerYHitCont);
  };

  void FillTrees();

  void PrintStatistic();
        
private:

  char*    fRootFileName;
  TFile*   fRootFile;
  TH1D*    fHisto[MaxHisto];            
  TTree*   fCaloTree;
  TTree*   fHodoXTree;
  TTree*   fHodoYTree;
  TTree*   fTrackerXTree;
  TTree*   fTrackerYTree;
  TTree*   fKinTree;

  struct CaloHitContainer {
    vector<int> Det;
    vector<uint> Col;
    vector<uint> Row;
    vector<double> Edep;
    vector<int> PID;
  } fCaloHitCont;

  HodoHitContainer fHodoXHitCont;
  HodoHitContainer fHodoYHitCont;

  TrackerHitContainer fTrackerXHitCont;
  TrackerHitContainer fTrackerYHitCont;

  struct KinVar {
    double Q2;
    double t;
    double s;
    double xi;
    double tau;
    double eta;
    double phi_cm;
    double the_cm;
    double psf;
    double flux_factor;
    double crs_BH;
    double Eg;
  } fKinVar;

  char*    fKinFileName;
  ifstream fKinFile;

  friend class TCSEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
