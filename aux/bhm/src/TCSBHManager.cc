#include <CLHEP/Units/SystemOfUnits.h>

#include "TCSBHManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSBHManager::TCSBHManager() : fRootFile(0), fChain(0)
{
  fRootFileName="tcs_gen.root";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSBHManager::TCSBHManager(string aname) : fRootFile(0), fChain(0)
{
  fRootFileName=aname;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSBHManager::~TCSBHManager()
{
  if ( fRootFile ) delete fRootFile;
  if ( fChain ) delete fChain;
  if ( flvEm) delete flvEm;
  if ( flvEp) delete flvEp;
  if ( flvPro) delete flvPro;
  //  if ( fTCSVars) delete fTCSVars;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSBHManager::Init()
{
  fChain = new TChain();
  fChain->Add((fRootFileName+"/tr1").c_str());

  flvEm = new TLorentzVector();    // Lorentz Vector for e-
  flvEp = new TLorentzVector();    // Lorentz Vector for e+
  flvPro = new TLorentzVector();   // Lorentz Vector for proton

  fChain->SetBranchAddress("psf", &(fTCSVars.psf));
  fChain->SetBranchAddress("Q2", &(fTCSVars.Q2));
  fChain->SetBranchAddress("t", &(fTCSVars.t));
  fChain->SetBranchAddress("s", &(fTCSVars.s));
  fChain->SetBranchAddress("xi", &(fTCSVars.xi));
  fChain->SetBranchAddress("tau", &(fTCSVars.tau));
  fChain->SetBranchAddress("eta", &(fTCSVars.eta));
  fChain->SetBranchAddress("the_cm", &(fTCSVars.the_cm));
  fChain->SetBranchAddress("phi_cm", &(fTCSVars.phi_cm));
  fChain->SetBranchAddress("Eg", &(fTCSVars.Eg));
  fChain->SetBranchAddress("flux_factor", &(fTCSVars.flux_factor));
  fChain->SetBranchAddress("crs_BH", &(fTCSVars.crs_BH));

  fChain->SetBranchAddress("L_em", &flvEm);
  fChain->SetBranchAddress("L_ep", &flvEp);
  fChain->SetBranchAddress("L_prot", &flvPro);

  fNEntries = fChain->GetEntries();

  cout << "TCSBHManager::Init: fNEntires = " << fNEntries << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSBHManager::GetEntry(long ientry) {
  if (ientry < fNEntries)
    fChain->GetEntry(ientry);
  else {
    cout << "*** TCSBHManager::GetEntry: ientry > fNEntries! ***" << endl;
    cout << "                            GetEntry " << fNEntries-1 << endl;
    fChain->GetEntry(fNEntries-1);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSBHManager::tcs_vars TCSBHManager::GetTCSVars() {
  //  return *fTCSVars;
  return fTCSVars;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TLorentzVector TCSBHManager::GetLV(tcs_particle particle) {

  //TCS particles: em, ep, p.

  switch (particle) {
  case em : return *flvEm;
    break;
  case ep : return *flvEp;
    break;
  case p  : return *flvPro;
    break;
  default : cout << "*** TCSBHManager::GetLV: wrong particle! ***" << endl;
    return TLorentzVector(0.,0.,0.,0.);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
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

 fTrackerXTree = new TTree("trackerx",
			   "TCS X trackers' per event hit collections");
 fTrackerXTree->Branch("detcont", &(fTrackerXHitCont.Det));
 fTrackerXTree->Branch("chancont", &(fTrackerXHitCont.Chan));
 fTrackerXTree->Branch("edepcont", &(fTrackerXHitCont.Edep));

 fTrackerYTree = new TTree("trackery",
			   "TCS Y trackers' per event hit collections");
 fTrackerYTree->Branch("detcont", &(fTrackerYHitCont.Det));
 fTrackerYTree->Branch("chancont", &(fTrackerYHitCont.Chan));
 fTrackerYTree->Branch("edepcont", &(fTrackerYHitCont.Edep));

 G4cout << "\n----> Root file is opened in " << fRootFileName << G4endl;

}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void TCSHistoManager::PrintStatistic()
{
  if(fHisto[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;
    
//    G4cout 
//    << " EAbs : mean = " << G4BestUnit(fHisto[1]->GetMean(), "Energy") 
//    << " rms = " << G4BestUnit(fHisto[1]->GetRMS(),  "Energy") << G4endl;

  }
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
