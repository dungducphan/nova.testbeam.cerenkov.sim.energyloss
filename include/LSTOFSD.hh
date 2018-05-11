//
// Created by Dung Phan on 5/11/18.
//

#ifndef LSSIM_LSTOFSD_H
#define LSSIM_LSTOFSD_H

#include "G4VSensitiveDetector.hh"
#include "LSTOFHit.hh"

class G4Step;
class G4HCofThisEvent;

class LSTOFSD : public G4VSensitiveDetector {
public:
  LSTOFSD(const G4String);
  virtual ~LSTOFSD();

  // methods from base class
  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory* );

private:
  LSTOFHitsCollection* fTOFHitCollection;
};


#endif //LSSIM_LSTOFSD_H
