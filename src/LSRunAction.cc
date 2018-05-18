#include "LSRunAction.hh"
#include <cstring>
#include "G4SystemOfUnits.hh"

LSRunAction::LSRunAction() : G4UserRunAction() {
}

LSRunAction::~LSRunAction() {
}

void LSRunAction::BeginOfRunAction(const G4Run * run) {
  G4RunManager* runManager = static_cast<G4RunManager*>(G4RunManager::GetRunManager());
  LSPrimaryGeneratorAction* pga = (LSPrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();
  int energyBin = pga->GetEnergyBin();
  std::string particleName = pga->GetParticleGun()->GetParticleDefinition()->GetParticleName();

  fOutput = new TFile(Form("Output/EnergyLoss_%s_%i.root", particleName.c_str(), energyBin), "RECREATE");
  fAnalysisTree = new TTree("AnalysisTree","Energy loss analysis");
  fAnalysisTree->Branch("Energy", &Energy, "Energy/D");
  fAnalysisTree->Branch("EnergyLoss", &EnergyLoss, "EnergyLoss/D");
  hEventInRunCount = new TH1I("hEventInRunCount", "hEventInRunCount", 2, 0, 2);
}

void LSRunAction::EndOfRunAction(const G4Run * run) {
  fAnalysisTree->Write(0, TObject::kOverwrite);
  hEventInRunCount->Write(0, TObject::kOverwrite);
  fOutput->Write(0, TObject::kOverwrite);
  fOutput->Close();
}

void LSRunAction::FillAnalysisTree(double energy, double energyLoss) {
  EnergyLoss = energyLoss;
  Energy = energy;
  fAnalysisTree->Fill();
}

void LSRunAction::FillEventCountHist() {
  hEventInRunCount->Fill(0.5);
}
