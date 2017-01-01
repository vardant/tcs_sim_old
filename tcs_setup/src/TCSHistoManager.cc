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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "TCSHistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSHistoManager::TCSHistoManager() : fRootFile(0), fCaloTree(0), fHodoXTree(0),
				     fHodoYTree(0)
{
  fRootFileName="tcs_setup.root";
  // histogram(s)
  for (G4int k=0; k<MaxHisto; k++) fHisto[k] = 0;
    
  // Trees
  fCaloTree = 0;
  fHodoXTree = 0;
  fHodoYTree = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSHistoManager::TCSHistoManager(char *aname) : fRootFile(0),
						fCaloTree(0),
						fHodoXTree(0),
						fHodoYTree(0)
{
  fRootFileName=aname;  
  // histogram(s)
  for (G4int k=0; k<MaxHisto; k++) fHisto[k] = 0;
    
  // Trees
  fCaloTree = 0;
  fHodoXTree = 0;
  fHodoYTree = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSHistoManager::~TCSHistoManager()
{
  if ( fRootFile ) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::book()
{ 
 // Creating tree containers to handle histograms and Trees.
 // These trees are associated to an output file.
 //
 fRootFile = new TFile(fRootFileName,"RECREATE");
 if(!fRootFile) {
   G4cout << " ***** HistoManager::book :" 
          << " problem creating the ROOT TFile *****"
          << G4endl;
   return;
 }
   
 fHisto[1] = new TH1D("h1", "Total Edep(MeV)", 100, 0., 5000.);
 if (!fHisto[1]) G4cout << "\n can't create histo 1" << G4endl;

 fCaloTree = new TTree("calo", "TCS Calorimeters' per event hit collections");
 fCaloTree->Branch("detcont", &(fCaloHitCont.Det));
 fCaloTree->Branch("colcont", &(fCaloHitCont.Col));
 fCaloTree->Branch("rowcont", &(fCaloHitCont.Row));
 fCaloTree->Branch("edepcont", &(fCaloHitCont.Edep));

 fHodoXTree = new TTree("hodox", "TCS X hodoscopes' per event hit collections");
 fHodoXTree->Branch("detcont", &(fHodoXHitCont.Det));
 fHodoXTree->Branch("chancont", &(fHodoXHitCont.Chan));
 fHodoXTree->Branch("edepcont", &(fHodoXHitCont.Edep));

 fHodoYTree = new TTree("hodoy", "TCS Y hodoscopes' per event hit collections");
 fHodoYTree->Branch("detcont", &(fHodoYHitCont.Det));
 fHodoYTree->Branch("chancont", &(fHodoYHitCont.Chan));
 fHodoYTree->Branch("edepcont", &(fHodoYHitCont.Edep));

 G4cout << "\n----> Root file is opened in " << fRootFileName << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::save()
{ 
  if (fRootFile) {
    fRootFile->Write();       // Writing the histograms to the file
    fRootFile->Close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
 if  (fHisto[ih]) { fHisto[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (fHisto[ih]) fHisto[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::FillTrees()
{
  if (fCaloTree) {
    //G4cout <<"Filling Tree right now! fHitList size = " << fHitList.size()
    //<< "\n";
    fCaloTree->Fill();
  }

  if (fHodoXTree) {
    //G4cout <<"Filling Tree right now! fHitList size = " << fHitList.size()
    //<< "\n";
    fHodoXTree->Fill();
  }

  if (fHodoYTree) {
    //G4cout <<"Filling Tree right now! fHitList size = " << fHitList.size()
    //<< "\n";
    fHodoYTree->Fill();
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::AddHit(int det, uint col, uint row, double edep) {

  bool found = false;

  vector<uint>::iterator ic = fCaloHitCont.Col.begin();
  vector<uint>::iterator ir = fCaloHitCont.Row.begin();
  vector<double>::iterator ie = fCaloHitCont.Edep.begin();

  for (vector<int>::iterator id=fCaloHitCont.Det.begin();
       id != fCaloHitCont.Det.end(); id++) {

    if (*id == det && *ic == col && *ir == row) {
      //      cout << "AddHit: *ie = " << *ie << "  edep = " << edep << endl;
      *ie += edep;
      //      cout << "AddHit: *ie = " << *ie << endl;
      //      getchar();
      found = true;
      break;
    }

    ic++; ir++; ie++;
  }

  if (!found) {
    fCaloHitCont.Det.push_back(det);
    fCaloHitCont.Col.push_back(col);
    fCaloHitCont.Row.push_back(row);
    fCaloHitCont.Edep.push_back(edep);
    //cout << "Pushed back cal hit " << det << " " << col << " " << row << " "
    //<< edep << endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::AddHit(int det, uint chan, double edep,
                             HodoHitContainer& HodoHitCont) {

  // Add hit to a hodoscope/tracker hit conatainer.

  bool found = false;

  vector<uint>::iterator ic = HodoHitCont.Chan.begin();
  vector<double>::iterator ie = HodoHitCont.Edep.begin();

  for (vector<int>::iterator id=HodoHitCont.Det.begin();
       id != HodoHitCont.Det.end(); id++) {

    if (*id == det && *ic == chan) {
      //      cout << "          TCSHistoManager::AddHit: *ie = " << *ie
      //           << "  edep = " << edep << endl;
      *ie += edep;
      //    cout << "          TCSHistoManager::AddHit: *ie = " << *ie << endl;
      //      getchar();
      found = true;
      break;
    }

    ic++; ie++;
  }

  if (!found) {
    HodoHitCont.Det.push_back(det);
    HodoHitCont.Chan.push_back(chan);
    HodoHitCont.Edep.push_back(edep);
    //G4cout << "          TCSHistoManager::AddHit: hit pushed back" << G4endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::PrintStatistic()
{
  if(fHisto[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;
    
    /*
    G4cout 
    << " EAbs : mean = " << G4BestUnit(fHisto[1]->GetMean(), "Energy") 
    << " rms = " << G4BestUnit(fHisto[1]->GetRMS(),  "Energy") << G4endl;
    */
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
