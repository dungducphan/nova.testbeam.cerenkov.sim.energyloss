#ifndef LSPrimaryGeneratorAction_h
#define LSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "TH2.h"
#include "TH1.h"
#include "TRandom3.h"
#include <cstdlib>

class G4ParticleGun;
class G4Event;
class G4Box;

class LSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  LSPrimaryGeneratorAction();
  virtual ~LSPrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event*);
  virtual std::vector<double> LogBins(int, double, double);
  virtual std::vector<double> LinearBins(int, double, double);
  virtual int GunRandomEnergy(double&, double&);
  virtual void GunRandomPosition(double&, double&);
  virtual int GetEnergyBin() { return fEnergyBin; }

  // method to access particle gun
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
  G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
  G4double fGunEnergyCenterValue;
  G4double fELowerEdge, fEUpperEdge;
  G4double fXStart, fYStart;
  G4double fRmean;
  G4double fRsigma;
  TH2D* hSurf;
  int fEnergyBin;

  TRandom3 fRND;
};

#endif
