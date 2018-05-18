#include "LSPrimaryGeneratorAction.hh"

LSPrimaryGeneratorAction::LSPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  std::vector<double> E = LogBins(30, 1.E1, 1.E4);
  std::vector<double> Eloss  = LogBins(30, 1.E-2, 1.E1);
  hSurf = new TH2D("ELossVsE", "ElossVsE;Eloss[MeV];E[MeV]", E.size()-1, &E[0], Eloss.size()-1, &Eloss[0]);

  G4int n_particle = 1;

  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 2.5*m));

  fRND.SetSeed(time(0));
}

LSPrimaryGeneratorAction::~LSPrimaryGeneratorAction() {
  delete fParticleGun;
}

void LSPrimaryGeneratorAction::GeneratePrimaries(G4Event * anEvent) {
  if (fParticleGun->GetParticleEnergy() != fGunEnergyCenterValue) {
    fGunEnergyCenterValue = fParticleGun->GetParticleEnergy();
    fEnergyBin = GunRandomEnergy(fELowerEdge, fEUpperEdge);
  }
  G4double GunEnergyInThisEvent = fRND.Uniform(fELowerEdge, fEUpperEdge)*MeV;

  fParticleGun->SetParticleEnergy(GunEnergyInThisEvent);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

std::vector<double> LSPrimaryGeneratorAction::LogBins(int nbins, double xlo, double xhi) {
  double log_spacing = exp( (log(xhi) - log(xlo))/(nbins) );
  std::vector<double> binning;
  for (int i = 0; i <= nbins; ++i) {
    if (i == 0) binning.push_back(xlo);
    else        binning.push_back(log_spacing*binning[i-1]);
  }
  return binning;
}

int LSPrimaryGeneratorAction::GunRandomEnergy(double& lowEgde, double& upEdge) {
  auto hX = hSurf->ProjectionX();
  auto binX = hX->FindBin(fGunEnergyCenterValue/MeV);

  lowEgde = hX->GetBinLowEdge(binX);
  upEdge  = hX->GetBinLowEdge(binX + 1);

  delete hX;

  return binX;
}


