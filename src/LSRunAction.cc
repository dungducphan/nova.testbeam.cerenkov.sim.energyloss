#include "LSRunAction.hh"
#include "G4SystemOfUnits.hh"

LSRunAction::LSRunAction() : G4UserRunAction() {
}

LSRunAction::~LSRunAction() {
}

void LSRunAction::BeginOfRunAction(const G4Run * run) {
  fOutput = new TFile(Form("EnergyLoss_%i.root", run->GetRunID()), "RECREATE");
  fAnalysisTree = new TTree("AnalysisTree","Energy loss analysis");
  fAnalysisTree->Branch("EnergyLoss", &EnergyLoss, "EnergyLoss/D");
  hEventInRunCount = new TH1I("hEventInRunCount", "hEventInRunCount", 2, 0, 2);
}

void LSRunAction::EndOfRunAction(const G4Run * run) {
  fAnalysisTree->Write();
  hEventInRunCount->Write();
  fOutput->Write();
}

void LSRunAction::FillAnalysisTree(double energyLoss) {
  EnergyLoss = energyLoss;
  std::cout << EnergyLoss << std::endl;
  fAnalysisTree->Fill();
}

void LSRunAction::FillEventCountHist() {
  hEventInRunCount->Fill(0.5);
}
