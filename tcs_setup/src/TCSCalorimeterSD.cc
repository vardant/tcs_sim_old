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

#include "TCSCalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSCalorimeterSD::TCSCalorimeterSD(const G4String& name,
				   const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(0), lastID(-1)
{
  collectionName.insert(hitsCollectionName);

  //  G4cout << "TCSCalorimeterSD::TCSCalorimeterSD: constructed" << G4endl;
  //  getchar();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSCalorimeterSD::~TCSCalorimeterSD() 
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection 
  = new TCSCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  //  G4cout << "TCSCalorimeterSD::Initialize: initialized" << G4endl;
  //  getchar();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TCSCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4int pid = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  G4ThreeVector pos = step->GetTrack()->GetPosition();

  // Particle entering calorimeter, save kinetic energy for flux calc-s.
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() ==
      "caloWorld_PV" &&
      step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName() ==
      "Block_PV" &&
      step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
    G4int col =step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4int row =step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
    G4double ekin = step->GetTrack()->GetKineticEnergy();
    TCSCalorimeterHit* hit = new TCSCalorimeterHit(col, row, pid, ekin, pos, 1);
    fHitsCollection->insert( hit );
    ////    return true;
  }

  // Particle in the crystals, save energy deposit for dose calc-s.
  if (step->GetTrack()->GetVolume()->GetName() == "Block_PV") {
    G4int col = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4int row = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
    G4double edep = step->GetTotalEnergyDeposit();
    TCSCalorimeterHit* hit = new TCSCalorimeterHit(col, row, pid, edep, pos, 0);
    fHitsCollection->insert( hit );
    //  hit->Print();
    //  getchar();
    ////    return true;
  }

  //  G4cout << "TCSCalorimeterSD::ProcessHits:" << G4endl;
  //  G4cout << " PreStepPoint volume:" <<
  //    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
  //	 << G4endl;
  //  G4cout << " PostStepPoint volume:" <<
  //    step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
  //	 << G4endl;

  /*
  if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
    G4cout << " PreStepPoint status: on Boundary !" << G4endl; 
    G4cout << " Pre  col, row: " << col << "  " << row << G4endl;
    G4int colpos =
      step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4int rowpos =
      step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
    G4cout << " Post col, row: " << colpos << "  " << rowpos << G4endl;
    getchar();
  }

  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
    G4cout << " PostStepPoint status: on Boundary !" << G4endl; 
    G4cout << " Pre  col, row: " << col << "  " << row << G4endl;
    G4int colpos =
      step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber();
    G4int rowpos =
      step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
    G4cout << " Post col, row: " << colpos << "  " << rowpos << G4endl;
    getchar();
  }
  */
  /*
  if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
    G4cout << " PreStepPoint status: on Boundary !" << G4endl;
    G4cout << " PreVol: "
    << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
	   << G4endl;
    G4cout << " PostVol: "
    << step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
	   << G4endl;
    getchar();
  };
  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
    G4cout << " PostStepPoint status: on Boundary !" << G4endl;
    G4cout << " PreVol: "
    << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
	   << G4endl;
    G4cout << " PostVol: "
    << step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
	   << G4endl;
    getchar();
  };
  */

  // Shall never happen.
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  //  if ( verboseLevel>1 ) { 
  //  G4int nofHits = fHitsCollection->entries();
  //   G4cout << G4endl
  //          << "-------->Hits Collection: in this event they are " << nofHits 
  //          << " hits in the tracker chambers: " << G4endl;
  //  for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->delete();
  //}

  //  G4cout << "TCSCalorimeterSD::EndOfEvent: end of event" << G4endl;
  //  getchar();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
