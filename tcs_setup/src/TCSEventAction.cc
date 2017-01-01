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

#include "TCSEventAction.hh"
#include "TCSRun.hh"
#include "TCSRunAction.hh"
#include "TCSHistoManager.hh"
#include "TCSCalorimeterHit.hh"
#include "TCSHodoXHit.hh"
#include "TCSHodoYHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4SDManager.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSEventAction::TCSEventAction(TCSHistoManager *histo)
  : G4UserEventAction(), fHistoManager(histo), fPrintModulo(0),
    fEdep(0.), fCalorimeterCollID(-1), fHodoXCollID(-1), fHodoYCollID(-1),
    fEvtNo(-1)
{
  //  fPrintModulo = 100000;
  fPrintModulo = 100;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSEventAction::~TCSEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSEventAction::BeginOfEventAction(const G4Event* evt)
{
  fEvtNo = evt->GetEventID();
  if (fEvtNo%fPrintModulo == 0)     G4cout << "event: " << fEvtNo << G4endl;

  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(fCalorimeterCollID<0)
  {
    //    G4String colNam;
    //    fCalorimeterCollID = 
    //      SDman->GetCollectionID(colNam="CalorimeterHitsCollection");
    fCalorimeterCollID = SDman->GetCollectionID("CalorimeterHitsCollection");
  }

  if(fHodoXCollID<0)
  {
    fHodoXCollID = SDman->GetCollectionID("HodoXHitsCollection");
  }

  if(fHodoYCollID<0)
  {
    fHodoYCollID = SDman->GetCollectionID("HodoYHitsCollection");
  }

  // initialization of per event quantities

  fEdep = 0.;   //Per event total energy deposition in the calorimeter.

  fHistoManager->Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSEventAction::EndOfEventAction(const G4Event* event)
{   
  // Accumulate statistics in TCSRun.
  TCSRun* run = static_cast<TCSRun*>(
		       G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
  run->AddEdep(fEdep);

  // Fill histogram with event's energy deposition.
  fHistoManager->FillHisto(1, fEdep/MeV);

  // get number of stored trajectories

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  //  G4cout << "TCSEventAction::EndOfEventAction: n_trajectories = " 
  //	 << n_trajectories << G4endl;
  //  getchar();

  //  G4int eventID = event->GetEventID();

  // Hit collection for this event.

  G4HCofThisEvent * HCE = event->GetHCofThisEvent();

  // Calorimeter hits.

  TCSCalorimeterHitsCollection* CC = 0;
  if(HCE) {
    CC = (TCSCalorimeterHitsCollection*)(HCE->GetHC(fCalorimeterCollID));
    //    G4cout << "  Found calorimeter hit collection." << G4endl;
  }

  if(CC) {
    int n_hit = CC->entries();
    //    G4cout << "  n_hit = " << n_hit << G4endl;

    for(int i=0;i<n_hit;i++) {
      G4int boundary_flag=(*CC)[i]->GetBoundaryFlag();
      //Fill Tree if track is within the calorimeter.
      if (boundary_flag == 0) {
	G4ThreeVector pos=(*CC)[i]->GetPos();
	G4int detpos = pos.getY() > 0. ? 1 : -1;
	G4int col =(*CC)[i]->GetCol();
	G4int row =(*CC)[i]->GetRow();
	//	G4int pid =(*CC)[i]->GetPID();
	G4double energy=(*CC)[i]->GetEnergy();
	fHistoManager->AddHit(detpos, col, row, energy/MeV);
      }
    }

    //Check hit container's consistency first.
    if (!fHistoManager->CheckCaloHitCont())
      cout <<"*** TCSEventAction::EndOfEventAction: "
	   << "calorimeter hit container inconsistent! ***" << endl;

    //    getchar();
  }

  // HodoX hits.

  TCSHodoXHitsCollection* HXC = 0;
  if(HCE) {
    HXC = (TCSHodoXHitsCollection*)(HCE->GetHC(fHodoXCollID));
    //    G4cout << "  Found hodoscope X hit collection." << G4endl;
  }

  if(HXC) {
    int n_hit = HXC->entries();
    //    G4cout << "  HX n_hit = " << n_hit << G4endl;

    for(int i=0;i<n_hit;i++) {
      G4int boundary_flag=(*HXC)[i]->GetBoundaryFlag();
      //Fill Tree if track is within the hodoscope.
      if (boundary_flag == 0) {
	G4ThreeVector pos=(*HXC)[i]->GetPos();
	G4int detpos = pos.getY() > 0. ? 1 : -1;
	G4int chan =(*HXC)[i]->GetChannel();
	//	G4int pid =(*HXC)[i]->GetPID();
	G4double energy=(*HXC)[i]->GetEnergy();
	fHistoManager->AddHit(detpos, chan, energy/MeV,
			      fHistoManager->fHodoXHitCont);
      }
    }

    //Check hit container's consistency first.
    if (!fHistoManager->CheckHodoXHitCont())
      cout <<"*** TCSEventAction::EndOfEventAction: "
    	   << "hodoscope X hit container inconsistent! ***" << endl;
  }

  // HodoY hits.

  TCSHodoYHitsCollection* HYC = 0;
  if(HCE) {
    HYC = (TCSHodoYHitsCollection*)(HCE->GetHC(fHodoYCollID));
    //    G4cout << "  Found hodoscope Y hit collection." << G4endl;
  }

  if(HYC) {
    int n_hit = HYC->entries();
    //    G4cout << "  HY n_hit = " << n_hit << G4endl;

    for(int i=0;i<n_hit;i++) {
      G4int boundary_flag=(*HYC)[i]->GetBoundaryFlag();
      //Fill Tree if track is within the hodoscope.
      if (boundary_flag == 0) {
	G4ThreeVector pos=(*HYC)[i]->GetPos();
	G4int detpos = pos.getY() > 0. ? 1 : -1;
	G4int chan =(*HYC)[i]->GetChannel();
	//	G4int pid =(*HYC)[i]->GetPID();
	G4double energy=(*HYC)[i]->GetEnergy();
	fHistoManager->AddHit(detpos, chan, energy/MeV,
			      fHistoManager->fHodoYHitCont);
      }
    }

    //Check hit container's consistency first.
    if (!fHistoManager->CheckHodoYHitCont())
      cout <<"*** TCSEventAction::EndOfEventAction: "
    	   << "hodoscope Y hit container inconsistent! ***" << endl;
  }

  if (CC || HXC || HYC) {
    fHistoManager->FillTrees();
    //    getchar();
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
