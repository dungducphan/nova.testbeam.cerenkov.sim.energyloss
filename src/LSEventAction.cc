#include "LSEventAction.hh"

LSEventAction::LSEventAction(LSRunAction * runAction) : G4UserEventAction() {
  fRunAction = runAction;
}

LSEventAction::~LSEventAction() {
}

void LSEventAction::BeginOfEventAction(const G4Event * event) {
}

void LSEventAction::EndOfEventAction(const G4Event * event) {
  fRunAction->FillEventCountHist();

  G4SDManager* SDManager= G4SDManager::GetSDMpointer();
  LSTOFSD* tofSD= (LSTOFSD*)SDManager-> FindSensitiveDetector("TOFSD");
  LSTOFHitsCollection* tofHC = tofSD->GetHitCollection();

  // Find energy at TOF US:
  G4double energyAtTOF_US = 0*MeV;
  G4double energyAtTOF_DS = 1E30*MeV;
  bool hitTOFUS = false;
  bool hitTOFDS = false;

  for (size_t i = 0; i < tofHC->GetSize(); i++) {
    LSTOFHit* aHit = (LSTOFHit*) tofHC->GetHit(i);

    if (aHit->GetTOFID() == 1) {
      hitTOFUS = true;
      energyAtTOF_US = (energyAtTOF_US/MeV > aHit->GetEnergy()/MeV) ? energyAtTOF_US : aHit->GetEnergy();
    }

    if (aHit->GetTOFID() == 2) {
      hitTOFDS = true;
      energyAtTOF_DS = (energyAtTOF_DS/MeV < aHit->GetEnergy()/MeV) ? energyAtTOF_DS : aHit->GetEnergy();
    }

    if (hitTOFDS && hitTOFUS) {
      fRunAction->FillAnalysisTree(energyAtTOF_US/MeV, (energyAtTOF_US - energyAtTOF_DS)/MeV);
    }
  }

}


