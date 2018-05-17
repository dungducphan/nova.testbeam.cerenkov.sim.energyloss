//
// Created by Dung Phan on 5/11/18.
//

#include "LSTOFSD.hh"


#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"

LSTOFSD::LSTOFSD(const G4String name)
    : G4VSensitiveDetector(name), fTOFHitCollection(0) {
  collectionName.insert("TOFHitCollection");
}

LSTOFSD::~LSTOFSD() {
}

void LSTOFSD::Initialize(G4HCofThisEvent* hce) {
  // Create hits collection
  fTOFHitCollection = new LSTOFHitsCollection(SensitiveDetectorName,collectionName[0]);

  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fTOFHitCollection );
}


G4bool LSTOFSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();

  // Need to know if this is the primary particle
  if(theTrack->GetParentID() != 0) return false;

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();

  G4int tofid = 0;
  if (thePrePoint->GetPhysicalVolume()->GetName() == "tofPhy_us") {
    tofid = 1;
  } else if (thePrePoint->GetPhysicalVolume()->GetName() == "tofPhy_ds") {
    tofid = 2;
  }

  G4double energy = theTrack -> GetTotalEnergy();

  // Creating the hit and add it to the collection
  fTOFHitCollection->insert(new LSTOFHit(tofid, energy));

  return true;
}
