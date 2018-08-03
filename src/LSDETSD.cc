//
// Created by Dung Phan on 5/11/18.
//

#include "LSDETSD.hh"


#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"

LSDETSD::LSDETSD(const G4String name)
    : G4VSensitiveDetector(name), fDETHitCollection(0) {
  collectionName.insert("DETHitCollection");
}

LSDETSD::~LSDETSD() {
}

void LSDETSD::Initialize(G4HCofThisEvent* hce) {
  // Create hits collection
  fDETHitCollection = new LSDETHitsCollection(SensitiveDetectorName,collectionName[0]);

  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fDETHitCollection );
}


G4bool LSDETSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();

  // Need to know if this is the primary particle
  if(theTrack->GetParentID() != 0) return false;

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();

  G4int detid = 0;
  if (thePrePoint->GetPhysicalVolume()->GetName() == "detPhy_us") {
    detid = 1;
  } else if (thePrePoint->GetPhysicalVolume()->GetName() == "detPhy_ds") {
    detid = 2;
  }

  G4double energy = theTrack -> GetKineticEnergy();

  // Creating the hit and add it to the collection
  fDETHitCollection->insert(new LSDETHit(detid, energy));

  return true;
}
