//
// Created by Dung Phan on 5/11/18.
//

#ifndef LSSIM_LSTOFHIT_H
#define LSSIM_LSTOFHIT_H

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

class LSDETHit : public G4VHit {
public:
  LSDETHit();
  LSDETHit(G4int pDETID, G4double pEnergy);
  virtual ~LSDETHit();

  LSDETHit(const LSDETHit &right);
  const LSDETHit& operator=(const LSDETHit& right);

  G4int operator==(const LSDETHit& right) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  inline void SetEnergy(G4double e) { fEnergy = e; }
  inline void SetDETID(G4int detid) { fDETID = detid; }

  inline G4double GetEnergy() { return fEnergy; }
  inline G4int    GetDETID() { return fDETID; }

private:
  G4double      fEnergy;
  G4int         fDETID;
};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<LSDETHit> LSDETHitsCollection;
extern G4ThreadLocal G4Allocator<LSDETHit>* LSDETHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* LSDETHit::operator new(size_t) {
  if(!LSDETHitAllocator)
    LSDETHitAllocator = new G4Allocator<LSDETHit>;
  return (void *) LSDETHitAllocator->MallocSingle();
}

inline void LSDETHit::operator delete(void *hit) {
  LSDETHitAllocator->FreeSingle((LSDETHit*) hit);
}


#endif //LSSIM_LSDETHIT_H
