//
// Created by Dung Phan on 5/11/18.
//

#include "LSDETHit.hh"

#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<LSDETHit>* LSDETHitAllocator=0;

LSDETHit::LSDETHit() {
}

LSDETHit::LSDETHit(G4int pDETID, G4double pEnergy) {
  fDETID       = pDETID;
  fEnergy      = pEnergy;
}

LSDETHit::~LSDETHit() {
}

LSDETHit::LSDETHit(const LSDETHit& right) : G4VHit() {
  *this = right;
}

const LSDETHit& LSDETHit::operator=(const LSDETHit& right) {
  fEnergy      = right.fEnergy;
  fDETID      = right.fDETID;

  return *this;
}

G4int LSDETHit::operator==(const LSDETHit& right) const {
  return (fEnergy == right.fEnergy && fDETID == right.fDETID);
}

