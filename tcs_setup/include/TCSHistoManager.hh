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

using namespace std;

class TFile;
class TTree;
class TH1D;

struct HodoHitContainer {
  vector<int> Det;
  vector<uint> Chan;
  vector<double> Edep;
};

const G4int MaxHisto = 5;

class TCSHistoManager {

public:
  
  TCSHistoManager();
  TCSHistoManager(char*);
  ~TCSHistoManager();
   
  void book();
  void save();

  void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
  void Normalize(G4int id, G4double fac);    

  void AddHit(int det, uint col, uint row, double edep);
  void AddHit(int det, uint chan, double edep, HodoHitContainer &HodoHitCont);

  bool CheckCaloHitCont() {
    uint sz = fCaloHitCont.Det.size();
    return (fCaloHitCont.Col.size() != sz || fCaloHitCont.Row.size() != sz ||
	    fCaloHitCont.Edep.size() != sz ? false : true);
  }

  //Generalize later on.
  bool CheckHodoXHitCont() {
    uint sz = fHodoXHitCont.Det.size();
    return (fHodoXHitCont.Chan.size() != sz || fHodoXHitCont.Edep.size() != sz
	    ? false : true);
  }

  bool CheckHodoYHitCont() {
    uint sz = fHodoYHitCont.Det.size();
    return (fHodoYHitCont.Chan.size() != sz || fHodoYHitCont.Edep.size() != sz
	    ? false : true);
  }

  void ResetCalo() {
    fCaloHitCont.Det.clear();
    fCaloHitCont.Col.clear();
    fCaloHitCont.Row.clear();
    fCaloHitCont.Edep.clear();
  };

  //Generalize later on.
  void ResetHodoX() {
    fHodoXHitCont.Det.clear();
    fHodoXHitCont.Chan.clear();
    fHodoXHitCont.Edep.clear();
  };

  void ResetHodoY() {
    fHodoYHitCont.Det.clear();
    fHodoYHitCont.Chan.clear();
    fHodoYHitCont.Edep.clear();
  };

  void Reset() {
    ResetCalo();
    ResetHodoX();
    ResetHodoY();
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

  struct CaloHitContainer {
    vector<int> Det;
    vector<uint> Col;
    vector<uint> Row;
    vector<double> Edep;
  } fCaloHitCont;

  HodoHitContainer fHodoXHitCont;
  HodoHitContainer fHodoYHitCont;

  friend class TCSEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
