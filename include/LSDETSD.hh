//
// Created by Dung Phan on 5/11/18.
//

#ifndef LSSIM_LSDETSD_H
#define LSSIM_LSDETSD_H

#include "G4VSensitiveDetector.hh"
#include "LSDETHit.hh"

class G4Step;
class G4HCofThisEvent;

class LSDETSD : public G4VSensitiveDetector {
public:
  LSDETSD(const G4String);
  virtual ~LSDETSD();

  // methods from base class
  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

  virtual LSDETHitsCollection* GetHitCollection() { return fDETHitCollection; }

private:
  LSDETHitsCollection* fDETHitCollection;
};


#endif //LSSIM_LSDETSD_H
