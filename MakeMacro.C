#include <vector>
#include <cstring>
#include <fstream>

#include "TH2.h"

unsigned int NumberOfEvent = 10000;

void MakeMacro();
std::vector<double> LogBins(int, double, double);
std::vector<double> LinearBins(int, double, double);

std::vector<double> LogBins(int nbins, double xlo, double xhi) {
  double log_spacing = exp( (log(xhi) - log(xlo))/(nbins) );
  std::vector<double> binning;
  for (int i = 0; i <= nbins; ++i) {
    if (i == 0) binning.push_back(xlo);
    else        binning.push_back(log_spacing*binning[i-1]);
  }
  return binning;
}

std::vector<double> LinearBins(int nbins, double xlo, double xhi) {
  double linear_spacing = (xhi - xlo)/ (double) nbins;
  std::vector<double> binning;
  for (int i = 0; i <= nbins; ++i) {
    if (i == 0) binning.push_back(xlo);
    else        binning.push_back(linear_spacing + binning[i-1]);
  }
  return binning;
}

void MakeMacro() {
  std::vector<std::string> ParticleList;
  ParticleList.push_back("e-");
  ParticleList.push_back("mu-");
  ParticleList.push_back("gamma");
  ParticleList.push_back("proton");
  ParticleList.push_back("neutron");
  ParticleList.push_back("pi0");
  ParticleList.push_back("pi-");
  ParticleList.push_back("kaon0");
  ParticleList.push_back("kaon-");

  std::vector<double> E     = LogBins(60, 1.E1, 1.E4);
  std::vector<double> Eloss = LogBins(60, 1.E1, 1.E3);
  TH2D *hSurf = new TH2D("ELossVsE", "", E.size() - 1, &E[0], Eloss.size() - 1, &Eloss[0]);
  auto hX = hSurf->ProjectionX();

  // Make macro files for Geant4
  for (unsigned int k = 0; k < ParticleList.size(); k++) {
    std::string particleName = ParticleList.at(k);
    std::ofstream out(Form("Macros/run_%s.mac", particleName.c_str()));
    out << Form("/gun/particle %s", particleName.c_str()) << std::endl;
    out << Form("/run/initialize") << std::endl;
    for (unsigned int i = 1; i < hX->GetSize(); i++) {
      out << Form("/gun/energy %.4f MeV", hX->GetBinCenter(i)) << std::endl;
      out << Form("/run/beamOn %i", NumberOfEvent) << std::endl;
    }
    out.close();
  }

  // Make executing script to run the macros
  {
    std::ofstream out(Form("run.sh"));
    out << Form("#!/bin/bash") << std::endl;
    out << std::endl;
    for (unsigned int k = 0; k < ParticleList.size(); k++) {
      out << Form("./LSsim Macros/run_%s.mac", ParticleList.at(k).c_str()) << std::endl;
    }
    out.close();
  }

  // Make clean script to clean the macros and Output folder
  {
    std::ofstream out(Form("clean.sh"));
    out << Form("#!/bin/bash") << std::endl;
    out << Form("rm Macros/*") << std::endl;
    out << Form("rm Output/*") << std::endl;
    out << Form("rm join.sh") << std::endl;
    out << Form("rm Results/*") << std::endl;
    out.close();
  }


  // Make join script to join the root file from the same particle run
  {
    std::ofstream out(Form("join.sh"));
    out << Form("#!/bin/bash") << std::endl;
    for (unsigned int k = 0; k < ParticleList.size(); k++) {
      out << Form("hadd Results/EnergyLoss_%s.root Output/EnergyLoss_%s_*.root", ParticleList.at(k).c_str(), ParticleList.at(k).c_str()) << std::endl;
    }
    out.close();
  }
}