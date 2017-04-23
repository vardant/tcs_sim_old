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

TCSHistoManager::TCSHistoManager() : fKinFile(0), fRootFile(0), fCaloTree(0),
				     fHodoXTree(0), fHodoYTree(0),
				     fTrackerXTree(0), fTrackerYTree(0)
{
  fKinFileName ="tcs_gen.kin_data";
  fRootFileName="tcs_setup.root";
  // histogram(s)
  for (G4int k=0; k<MaxHisto; k++) fHisto[k] = 0;
    
  // Trees
  //  fCaloTree = 0;
  //  fHodoXTree = 0;
  //  fHodoYTree = 0;
  //  fTrackerXTree = 0;
  //  fTrackerYTree = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSHistoManager::TCSHistoManager(char *kname, char *rname) :
  fKinFile(0), fRootFile(0), fCaloTree(0), fHodoXTree(0), fHodoYTree(0),
  fTrackerXTree(0), fTrackerYTree(0)
{
  fKinFileName = kname;  
  fRootFileName= rname;  

  // histogram(s)
  for (G4int k=0; k<MaxHisto; k++) fHisto[k] = 0;
    
  // Trees
  //  fCaloTree = 0;
  //  fHodoXTree = 0;
  //  fHodoYTree = 0;
  //  fTrackerXTree = 0;
  //  fTrackerYTree = 0;
  //  fKinTree = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSHistoManager::~TCSHistoManager()
{
  if ( fRootFile ) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::book()
{ 

  fKinFile.open(fKinFileName);   //to read per event kinematic quantities
  if(!fKinFile) {
   G4cout << " ***** HistoManager::book :" 
          << " problem openiing Kin file *****"
          << G4endl;
   return;
  }

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
 fCaloTree->Branch("pidcont", &(fCaloHitCont.PID));

 fHodoXTree = new TTree("hodox", "TCS X hodoscopes' per event hit collections");
 fHodoXTree->Branch("detcont", &(fHodoXHitCont.Det));
 fHodoXTree->Branch("chancont", &(fHodoXHitCont.Chan));
 fHodoXTree->Branch("edepcont", &(fHodoXHitCont.Edep));
 fHodoXTree->Branch("pidcont", &(fHodoXHitCont.PID));

 fHodoYTree = new TTree("hodoy", "TCS Y hodoscopes' per event hit collections");
 fHodoYTree->Branch("detcont", &(fHodoYHitCont.Det));
 fHodoYTree->Branch("chancont", &(fHodoYHitCont.Chan));
 fHodoYTree->Branch("edepcont", &(fHodoYHitCont.Edep));
 fHodoYTree->Branch("pidcont", &(fHodoYHitCont.PID));

 fTrackerXTree = new TTree("trackerx",
			   "TCS X trackers' per event hit collections");
 fTrackerXTree->Branch("detcont", &(fTrackerXHitCont.Det));
 fTrackerXTree->Branch("chancont", &(fTrackerXHitCont.Chan));
 fTrackerXTree->Branch("edepcont", &(fTrackerXHitCont.Edep));
 fTrackerXTree->Branch("pidcont", &(fTrackerXHitCont.PID));

 fTrackerYTree = new TTree("trackery",
			   "TCS Y trackers' per event hit collections");
 fTrackerYTree->Branch("detcont", &(fTrackerYHitCont.Det));
 fTrackerYTree->Branch("chancont", &(fTrackerYHitCont.Chan));
 fTrackerYTree->Branch("edepcont", &(fTrackerYHitCont.Edep));
 fTrackerYTree->Branch("pidcont", &(fTrackerYHitCont.PID));

 fKinTree = new TTree("kin","TCS kinematics");
 fKinTree->Branch("Q2",&fKinVar.Q2);
 fKinTree->Branch("t",&fKinVar.t);
 fKinTree->Branch("s",&fKinVar.s);
 fKinTree->Branch("xi",&fKinVar.xi);
 fKinTree->Branch("tau",&fKinVar.tau);
 fKinTree->Branch("eta",&fKinVar.eta);
 fKinTree->Branch("phi_cm",&fKinVar.phi_cm);
 fKinTree->Branch("the_cm",&fKinVar.the_cm);
 fKinTree->Branch("psf",&fKinVar.psf);
 fKinTree->Branch("flux_factor",&fKinVar.flux_factor);
 fKinTree->Branch("crs_BH",&fKinVar.crs_BH);
 fKinTree->Branch("Eg",&fKinVar.Eg);

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
  fKinFile.close();
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

  if (fTrackerXTree) {
    //G4cout <<"Filling Tree right now! fHitList size = " << fHitList.size()
    //<< "\n";
    fTrackerXTree->Fill();
  }

  if (fTrackerYTree) {
    //G4cout <<"Filling Tree right now! fHitList size = " << fHitList.size()
    //<< "\n";
    fTrackerYTree->Fill();
  }

  fKinFile >> fKinVar.Q2 >> fKinVar.t >> fKinVar.s >> fKinVar.xi >> fKinVar.tau
	   >> fKinVar.eta >> fKinVar.phi_cm >> fKinVar.the_cm
	   >> fKinVar.psf >> fKinVar.flux_factor >> fKinVar.crs_BH
	   >> fKinVar.Eg;

  if (fKinTree) {
    fKinTree->Fill();
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::AddHit(int det, uint col,uint row, double edep, int pid) {

  bool found = false;

  vector<uint>::iterator ic = fCaloHitCont.Col.begin();
  vector<uint>::iterator ir = fCaloHitCont.Row.begin();
  vector<double>::iterator ie = fCaloHitCont.Edep.begin();
  vector<int>::iterator ip = fCaloHitCont.PID.begin();

  for (vector<int>::iterator id=fCaloHitCont.Det.begin();
       id != fCaloHitCont.Det.end(); id++) {

    if (*id == det && *ic == col && *ir == row && *ip == pid) {
      //      cout << "AddHit: *ie = " << *ie << "  edep = " << edep << endl;
      *ie += edep;
      //      cout << "AddHit: *ie = " << *ie << endl;
      //      getchar();
      found = true;
      break;
    }

    ic++; ir++; ie++; ip++;
  }

  if (!found) {
    fCaloHitCont.Det.push_back(det);
    fCaloHitCont.Col.push_back(col);
    fCaloHitCont.Row.push_back(row);
    fCaloHitCont.Edep.push_back(edep);
    fCaloHitCont.PID.push_back(pid);
    //cout << "Pushed back cal hit " << det << " " << col << " " << row << " "
    //<< edep << endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::AddHit(int det, uint chan, double edep, int pid,
                             HodoHitContainer& HodoHitCont) {

  // Add hit to a hodoscope/tracker hit container.

  bool found = false;

  vector<uint>::iterator ic = HodoHitCont.Chan.begin();
  vector<double>::iterator ie = HodoHitCont.Edep.begin();
  vector<int>::iterator ip = HodoHitCont.PID.begin();

  for (vector<int>::iterator id=HodoHitCont.Det.begin();
       id != HodoHitCont.Det.end(); id++) {

    if (*id == det && *ic == chan && *ip == pid) {
      //      cout << "          TCSHistoManager::AddHit: *ie = " << *ie
      //           << "  edep = " << edep << endl;
      *ie += edep;
      //    cout << "          TCSHistoManager::AddHit: *ie = " << *ie << endl;
      //      getchar();
      found = true;
      break;
    }

    ic++; ie++; ip++;
  }

  if (!found) {
    HodoHitCont.Det.push_back(det);
    HodoHitCont.Chan.push_back(chan);
    HodoHitCont.Edep.push_back(edep);
    HodoHitCont.PID.push_back(pid);
    //G4cout << "          TCSHistoManager::AddHit: hit pushed back" << G4endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSHistoManager::AddHit(int det, uint chan, double edep, int pid,
                             TrackerHitContainer& TrackerHitCont) {

  // Add hit to a hodoscope/tracker hit conatainer.

  bool found = false;

  vector<uint>::iterator ic = TrackerHitCont.Chan.begin();
  vector<double>::iterator ie = TrackerHitCont.Edep.begin();
  vector<int>::iterator ip = TrackerHitCont.PID.begin();

  for (vector<int>::iterator id=TrackerHitCont.Det.begin();
       id != TrackerHitCont.Det.end(); id++) {

    if (*id == det && *ic == chan && *ip == pid) {
      //      cout << "          TCSHistoManager::AddHit: *ie = " << *ie
      //           << "  edep = " << edep << endl;
      *ie += edep;
      //    cout << "          TCSHistoManager::AddHit: *ie = " << *ie << endl;
      //      getchar();
      found = true;
      break;
    }

    ic++; ie++; ip++;
  }

  if (!found) {
    TrackerHitCont.Det.push_back(det);
    TrackerHitCont.Chan.push_back(chan);
    TrackerHitCont.Edep.push_back(edep);
    TrackerHitCont.PID.push_back(pid);
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
