#include "LSEventAction.hh"

LSEventAction::LSEventAction(LSRunAction *runAction) : G4UserEventAction() {
    fRunAction = runAction;
}

LSEventAction::~LSEventAction() {
}

void LSEventAction::BeginOfEventAction(const G4Event *event) {
}

void LSEventAction::EndOfEventAction(const G4Event *event) {
    fRunAction->FillEventCountHist();

    G4SDManager *SDManager = G4SDManager::GetSDMpointer();
    LSDETSD *detSD = (LSDETSD *) SDManager->FindSensitiveDetector("DETSD");
    LSDETHitsCollection *detHC = detSD->GetHitCollection();

    // Find energy at DET US:
    G4double energyAtDET_US = 0 * MeV;
    G4double energyAtDET_DS = 1E30 * MeV;
    bool hitDETUS = false;
    bool hitDETDS = false;

    for (size_t i = 0; i < detHC->GetSize(); i++) {
        LSDETHit *aHit = (LSDETHit *) detHC->GetHit(i);

        if (aHit->GetDETID() == 1) {
            hitDETUS = true;
            energyAtDET_US = (energyAtDET_US / MeV > aHit->GetEnergy() / MeV) ? energyAtDET_US : aHit->GetEnergy();
        }

        if (aHit->GetDETID() == 2) {
            hitDETDS = true;
            energyAtDET_DS = (energyAtDET_DS / MeV < aHit->GetEnergy() / MeV) ? energyAtDET_DS : aHit->GetEnergy();
        }
    }

    if (hitDETDS && hitDETUS) {
        fRunAction->FillAnalysisTree(energyAtDET_US / MeV, (energyAtDET_US - energyAtDET_DS) / MeV);
    }
}


