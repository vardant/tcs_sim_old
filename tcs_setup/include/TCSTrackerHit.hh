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

#ifndef TCSTrackerHit_h
#define TCSTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class TCSTrackerHit : public G4VHit {

public:

  TCSTrackerHit();
  TCSTrackerHit(const TCSTrackerHit&);
  TCSTrackerHit(G4int channel, G4int pid, G4double energy, G4ThreeVector pos,
	      G4int boundary_flag);
  virtual ~TCSTrackerHit();
  
  // operators
  const TCSTrackerHit& operator=(const TCSTrackerHit&);
  G4int operator==(const TCSTrackerHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  // methods from base class
  virtual void Draw();
  virtual void Print();
  
  // Get methods
  G4int GetChannel() const { return fChannel; };
  G4int GetPID() const { return fPID; };
  G4double GetEnergy() const { return fEnergy; };
  G4ThreeVector GetPos() const { return fPos; };
  G4int GetBoundaryFlag() const { return fBoundaryFlag; };
  
private:

  // fEnergy is kin.energy for particles entering hodoscope,
  // otherwise deposited energy.
  // fBoundaryFlag is true for particles entering hodoscope,
  // otherwise false.
  
  G4int         fChannel;
  G4int         fPID;
  G4double      fEnergy;
  G4ThreeVector fPos;
  G4int         fBoundaryFlag;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<TCSTrackerHit> TCSTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<TCSTrackerHit>* TCSTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* TCSTrackerHit::operator new(size_t)
{
  if(!TCSTrackerHitAllocator)
    TCSTrackerHitAllocator = new G4Allocator<TCSTrackerHit>;
  return (void *) TCSTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void TCSTrackerHit::operator delete(void *hit)
{
  TCSTrackerHitAllocator->FreeSingle((TCSTrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
