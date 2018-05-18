#include <cstring>

#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH2.h"

#include "MakeMacro.C"
#include "Style.C"

TFile* histogramFile;

TFile* rootfile;
TTree* analysisTree;

Double_t  Energy;
Double_t  EnergyLoss;
TBranch   *b_Energy;
TBranch   *b_EnergyLoss;

std::string particleName;

void Analysis(std::string, unsigned int);
void CountSignalSurvive();
void EstimateEnergyLoss();

void Analysis(std::string pName, unsigned int NumberOfEvents) {
  particleName = pName;
  rootfile = new TFile(Form("Results/EnergyLoss_%s.root", particleName.c_str()), "READ");
  analysisTree = (TTree*) rootfile->Get("AnalysisTree");
  analysisTree->SetBranchAddress("Energy", &Energy, &b_Energy);
  analysisTree->SetBranchAddress("EnergyLoss", &EnergyLoss, &b_EnergyLoss);

  CountSignalSurvive();
  EstimateEnergyLoss();

  rootfile->Close();
  delete(rootfile);
  delete(analysisTree);
}

void CountSignalSurvive() {
  GeneralStyle();

  unsigned int N = analysisTree->GetEntries();
  std::vector<double> E = LogBins(30, 1.E1, 1.E4);;
  TH1D* energy = new TH1D(Form("survival_%s", particleName.c_str()), ";Energy (MeV);Survival (%)", E.size() - 1, &E[0]);
  for (unsigned int i = 0; i < N; i++) {
    analysisTree->GetEntry(i);
    energy->Fill(Energy);
  }

  for (unsigned int i = 0; i < energy->GetSize(); i++) {
    energy->SetBinError(i, 1/TMath::Sqrt((double)energy->GetBinContent(i)));
  }
  energy->Scale(100/NumberOfEvent);

  TCanvas* c = new TCanvas();
  StyleLogLinear(c);
  energy->GetYaxis()->SetRangeUser(0, 110);
  StyleTH1(energy, kAzure, 2, kSolid, true, kOpenCircle, 1);
  energy->Draw();

  TLine* line = new TLine(1E1, 100, 1E4, 100);
  StyleLine(line, kRed, 1, kDashed);
  line->Draw();

  TLegend* leg = new TLegend(0.6, 0.2, 0.8, 0.4);
  leg->AddEntry(energy, Form("%s", particleName.c_str()), "p");
  leg->Draw();

  c->Modified();
  c->Update();
  c->SaveAs(Form("Results/survival_%s.pdf", particleName.c_str()));

  histogramFile = new TFile("histograms.root", "UPDATE");
  histogramFile->cd();
  energy->Write(0, TObject::kOverwrite);
  histogramFile->Write(0, TObject::kOverwrite);
  histogramFile->Close();
}

void EstimateEnergyLoss() {
  GeneralStyle();

  unsigned int N = analysisTree->GetEntries();
  std::vector<double> E     = LogBins(60, 1.E1, 1.E4);
  std::vector<double> Eloss = LogBins(60, 1E0, 1E3);
  TH2D *hEloss = new TH2D(Form("eloss_%s", particleName.c_str()), Form("%s;Energy (MeV);Energy loss (MeV)", particleName.c_str()), E.size() - 1, &E[0], Eloss.size() - 1, &Eloss[0]);
  for (unsigned int i = 0; i < N; i++) {
    analysisTree->GetEntry(i);
    hEloss->Fill(Energy, EnergyLoss);
  }
  TCanvas* c = new TCanvas();
  StyleLogLog(c);

  hEloss->GetXaxis()->CenterTitle();
  hEloss->GetYaxis()->CenterTitle();
  hEloss->Draw("COLZ");

  c->Modified();
  c->Update();
  c->SaveAs(Form("Results/eloss_%s.pdf", particleName.c_str()));

  histogramFile = new TFile("histograms.root", "UPDATE");
  histogramFile->cd();
  hEloss->Write(0, TObject::kOverwrite);
  histogramFile->Write(0, TObject::kOverwrite);
  histogramFile->Close();
}