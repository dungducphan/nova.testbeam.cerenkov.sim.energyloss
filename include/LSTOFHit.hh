//
// Created by Dung Phan on 5/11/18.
//

#ifndef LSSIM_LSTOFHIT_H
#define LSSIM_LSTOFHIT_H

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

class LSTOFHit : public G4VHit {
public:
  LSTOFHit();
  LSTOFHit(G4int pTOFID, G4double pEnergy);
  virtual ~LSTOFHit();

  LSTOFHit(const LSTOFHit &right);
  const LSTOFHit& operator=(const LSTOFHit& right);

  G4int operator==(const LSTOFHit& right) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  inline void SetEnergy(G4double e) { fEnergy = e; }
  inline void SetTOFID(G4int tofid) { fTOFID = tofid; }

  inline G4double GetEnergy() { return fEnergy; }
  inline G4int    GetTOFID() { return fTOFID; }

private:
  G4double      fEnergy;
  G4int         fTOFID;
};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<LSTOFHit> LSTOFHitsCollection;
extern G4ThreadLocal G4Allocator<LSTOFHit>* LSTOFHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* LSTOFHit::operator new(size_t) {
  if(!LSTOFHitAllocator)
    LSTOFHitAllocator = new G4Allocator<LSTOFHit>;
  return (void *) LSTOFHitAllocator->MallocSingle();
}

inline void LSTOFHit::operator delete(void *hit) {
  LSTOFHitAllocator->FreeSingle((LSTOFHit*) hit);
}


#endif //LSSIM_LSTOFHIT_H
