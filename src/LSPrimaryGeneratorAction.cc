#include "LSPrimaryGeneratorAction.hh"
#include "TMath.h"

LSPrimaryGeneratorAction::LSPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
    std::vector<double> E = LogBins(30, 1.E1, 1.E4);
    std::vector<double> Eloss = LogBins(30, 0, 30);
    hSurf = new TH2D("ELossVsE", "ElossVsE;Eloss[MeV];E[MeV]", E.size() - 1, &E[0], Eloss.size() - 1, &Eloss[0]);

    G4int n_particle = 1;

    fParticleGun = new G4ParticleGun(n_particle);
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    fRND.SetSeed(time(0));
    fRmean = 0 * m;
    fRsigma = 5 * cm;
}

LSPrimaryGeneratorAction::~LSPrimaryGeneratorAction() {
    delete fParticleGun;
}

void LSPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    // Randomize Energy
    if (fParticleGun->GetParticleEnergy() != fGunEnergyCenterValue) {
        fGunEnergyCenterValue = fParticleGun->GetParticleEnergy();
        fEnergyBin = GunRandomEnergy(fELowerEdge, fEUpperEdge);
    }
    G4double GunEnergyInThisEvent = fRND.Uniform(fELowerEdge, fEUpperEdge) * MeV;

    // Randomize Momentum Direction
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));

    // Randomize Start Position
    GunRandomPosition(fXStart, fYStart);
    fParticleGun->SetParticlePosition(G4ThreeVector(fXStart, fYStart, 2.5 * m));

    fParticleGun->SetParticleEnergy(GunEnergyInThisEvent);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

std::vector<double> LSPrimaryGeneratorAction::LogBins(int nbins, double xlo, double xhi) {
    double log_spacing = exp((log(xhi) - log(xlo)) / (nbins));
    std::vector<double> binning;
    for (int i = 0; i <= nbins; ++i) {
        if (i == 0) binning.push_back(xlo);
        else binning.push_back(log_spacing * binning[i - 1]);
    }
    return binning;
}

std::vector<double> LSPrimaryGeneratorAction::LinearBins(int nbins, double xlo, double xhi) {
    double linear_spacing = (xhi - xlo) / (double) nbins;
    std::vector<double> binning;
    for (int i = 0; i <= nbins; ++i) {
        if (i == 0) binning.push_back(xlo);
        else binning.push_back(linear_spacing + binning[i - 1]);
    }
    return binning;
}

int LSPrimaryGeneratorAction::GunRandomEnergy(double &lowEgde, double &upEdge) {
    auto hX = hSurf->ProjectionX();
    auto binX = hX->FindBin(fGunEnergyCenterValue / MeV);

    lowEgde = hX->GetBinLowEdge(binX);
    upEdge = hX->GetBinLowEdge(binX + 1);

    delete hX;

    return binX;
}

void LSPrimaryGeneratorAction::GunRandomPosition(double &xStart, double &yStart) {
    G4double radius = fRND.Gaus(fRmean / m, fRsigma / m) * m;
    G4double phi = fRND.Uniform(0, TMath::Pi() * 2);

    xStart = radius * TMath::Cos(phi);
    yStart = radius * TMath::Sin(phi);
}




