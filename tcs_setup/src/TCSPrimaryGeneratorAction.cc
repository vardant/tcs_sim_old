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

#include "TCSPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
//#include "TCSGen.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSPrimaryGeneratorAction::TCSPrimaryGeneratorAction() :
  G4VUserPrimaryGeneratorAction()
{
 // Read in default beam parameters

  ifstream file("beam_definition.txt"); // Open the file for reading.

  string line;
  istringstream iss;

  getline(file, line);  iss.str(line);
  iss >> fParticleName;
  getline(file, line);  iss.str(line);
  iss >> fEnergy;
  getline(file, line);  iss.str(line);
  iss >> fX0 >> fY0 >> fZ0;
  getline(file, line);  iss.str(line);
  iss >> fDX >> fDY >> fDZ;
  getline(file, line);  iss.str(line);
  string mode_flag;
  iss >> mode_flag;

  file.close();

  fEnergy *= GeV;
  fX0 *= cm;
  fY0 *= cm;
  fZ0 *= cm;
  fDX *= mm;
  fDY *= mm;
  fDZ *= mm;

  if (mode_flag == "tcs")
    fMode = tcs;
  else
    fMode = beam;


  G4cout << "TCSPrimaryGeneratorAction: Initial beam definition:" << G4endl;
  G4cout << "  Particle " << fParticleName << G4endl;
  G4cout << "  Energy = " << fEnergy/GeV << " GeV" << G4endl;
  G4cout << "  Position: (" << fX0/cm << ", " << fY0/cm << ", " << fZ0/cm
         << ") cm" << G4endl;
  G4cout << "  Beam sizes: " << fDX/mm << " x " << fDY/mm << " x " << fDZ
	 << " mm^3" << G4endl;
  G4cout << "  Requested mode: " << mode_flag << endl;
  if (fMode == tcs)
    G4cout << "  *** TCS mode: will read TCS events from input root file! ***"
	   << G4endl;
  else
    G4cout << "  Will use Beam source." << G4endl;

  if (fMode == tcs) {
    fTCSEntryNum = 0;
    fTCSPartNum = 0;
  }
  else {
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle=particleTable->FindParticle(fParticleName);

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticleEnergy(fEnergy);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCSPrimaryGeneratorAction::~TCSPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TCSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of each event.

  G4cout << "TCSPrimaryGeneratorAction::GeneratePrimaries: fMode ";
  if (fMode == tcs)
    G4cout << "tcs";
  else
    G4cout << "beam";
  G4cout << G4endl;

  G4double x = 0.;
  G4double y = 0.;
  G4double z = 0.;

  do {
    x = G4UniformRand()-.5;
    y = G4UniformRand()-.5;
  } while (x*x+y*y > .25);

  z = G4UniformRand()-.5;

  x = x*fDX + fX0;
  y = y*fDY + fY0;
  z = z*fDZ + fZ0;

  G4cout << "   xyz: " << x << " " << y << " " << z << G4endl;

  fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

  if (fMode == tcs) {

    if (fTCSPartNum == 3) {
      fTCSPartNum = 0;
      fTCSEntryNum++;
    }

    //    G4cout << "TCSPrimaryGeneratorAction::GeneratePrimaries:" << G4endl;
    G4cout << "   PartNum = " << fTCSPartNum << "  EntryNum = " << fTCSEntryNum
	   << G4endl;

    //    fTCSGen.GetEntry(fTCSEntryNum);
    //    fTCSGen.Show();
    TCSGen t;
    t.GetEntry(12);
    t.Show();
    G4cout << "   Got TCS entry" << G4endl;
    getchar();

    //    TLorentzVector lv = fBHM.GetLV((tcs_particle) fBHMPartNum);
    //    G4cout << "   lv: " << lv(0) << " " << lv(1) << " " << lv(2) << " "
    //	   << lv(3) << G4endl;
    //    getchar();

    /*
    fParticleGun->SetParticleMomentumDirection(
		  G4ThreeVector(lv(0),lv(1),lv(2)));
    fParticleGun->SetParticleEnergy(lv(3)*GeV);

    switch ((tcs_particle) fBHMPartNum) {
    case em: fParticleName = "e-"; break;
    case ep: fParticleName = "e+"; break;
    case p : fParticleName = "p"; break;
    };

    G4cout << "   ParticleName: " << fParticleName << G4endl;

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle=particleTable->FindParticle(fParticleName);

    fBHMPartNum++;

    getchar();
    */
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
