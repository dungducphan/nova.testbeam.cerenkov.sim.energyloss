//
// Created by Dung Phan on 5/11/18.
//

#include "LSTOFHit.hh"

#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<LSTOFHit>* LSTOFHitAllocator=0;

LSTOFHit::LSTOFHit() {
}

LSTOFHit::LSTOFHit(G4double pEnergy) {
  fEnergy      = pEnergy;
}

LSTOFHit::~LSTOFHit() {
}

LSTOFHit::LSTOFHit(const LSTOFHit& right) : G4VHit() {
  *this = right;
}

const LSTOFHit& LSTOFHit::operator=(const LSTOFHit& right) {
  fEnergy      = right.fEnergy;

  return *this;
}

G4int LSTOFHit::operator==(const LSTOFHit& right) const {
  return fEnergy == right.fEnergy;
}


