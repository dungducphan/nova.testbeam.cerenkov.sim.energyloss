#include <cstring>
#include <iostream>

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
void CountSignalSurvive(unsigned int);
void EstimateEnergyLoss();
void EnergyLossHistogram();

void Analysis(std::string pName, unsigned int NumberOfEvents) {
  particleName = pName;
  rootfile = new TFile(Form("Results/EnergyLoss_%s.root", particleName.c_str()), "READ");
  analysisTree = (TTree*) rootfile->Get("AnalysisTree");
  analysisTree->SetBranchAddress("Energy", &Energy, &b_Energy);
  analysisTree->SetBranchAddress("EnergyLoss", &EnergyLoss, &b_EnergyLoss);

  CountSignalSurvive(NumberOfEvents);
  EstimateEnergyLoss();
//  EnergyLossHistogram();

  rootfile->Close();
}

void CountSignalSurvive(unsigned int NumberOfEvents) {
  GeneralStyle();

  unsigned int N = analysisTree->GetEntries();
  std::vector<double> E = LogBins(30, 1.E1, 1.E4);
  TH1D* energy = new TH1D(Form("survival_%s", particleName.c_str()), ";Energy (MeV);Survival (%)", E.size() - 1, &E[0]);
  for (unsigned int i = 0; i < N; i++) {
    analysisTree->GetEntry(i);
    energy->Fill(Energy);
  }

  for (unsigned int i = 0; i < energy->GetSize(); i++) {
    energy->SetBinError(i, 1/TMath::Sqrt((double)energy->GetBinContent(i)));
    energy->SetBinContent(i, 100*(double)energy->GetBinContent(i)/(double)NumberOfEvents);
  }

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
  delete(histogramFile);
}

void EstimateEnergyLoss() {
  GeneralStyle();

  unsigned int N = analysisTree->GetEntries();
  std::vector<double> E     = LogBins(60, 1.E1, 1.E4);
  std::vector<double> Eloss = LogBins(80, 1E0, 1E4);
  TH2D *hEloss = new TH2D(Form("eloss_%s", particleName.c_str()), Form("%s;Energy (MeV);Energy loss (MeV)", particleName.c_str()), E.size() - 1, &E[0], Eloss.size() - 1, &Eloss[0]);

  unsigned int count_E100200 = 0;
  unsigned int count_E100200_Loss0p1p = 0;
  unsigned int count_E100200_Loss1p5p = 0;
  unsigned int count_E100200_Loss5p10p = 0;
  unsigned int count_E100200_Lossg10p = 0;
  unsigned int count_E200500 = 0;
  unsigned int count_E200500_Loss0p1p = 0;
  unsigned int count_E200500_Loss1p5p = 0;
  unsigned int count_E200500_Loss5p10p = 0;
  unsigned int count_E200500_Lossg10p = 0;
  unsigned int count_E5001k = 0;
  unsigned int count_E5001k_Loss0p1p = 0;
  unsigned int count_E5001k_Loss1p5p = 0;
  unsigned int count_E5001k_Loss5p10p = 0;
  unsigned int count_E5001k_Lossg10p = 0;
  for (unsigned int i = 0; i < N; i++) {
    analysisTree->GetEntry(i);
    hEloss->Fill(Energy, EnergyLoss);
    if (Energy >= 100 && Energy < 200) {
        count_E100200++;
        if      (EnergyLoss/Energy < 0.01)                              count_E100200_Loss0p1p++;
        else if (EnergyLoss/Energy < 0.05 && EnergyLoss/Energy >= 0.01) count_E100200_Loss1p5p++;
        else if (EnergyLoss/Energy < 0.10 && EnergyLoss/Energy >= 0.05) count_E100200_Loss5p10p++;
        else                                                            count_E100200_Lossg10p++;
    } else if (Energy >= 200 && Energy < 500) {
        count_E200500++;
        if      (EnergyLoss/Energy < 0.01)                              count_E200500_Loss0p1p++;
        else if (EnergyLoss/Energy < 0.05 && EnergyLoss/Energy >= 0.01) count_E200500_Loss1p5p++;
        else if (EnergyLoss/Energy < 0.10 && EnergyLoss/Energy >= 0.05) count_E200500_Loss5p10p++;
        else                                                            count_E200500_Lossg10p++;
    } else if (Energy >= 500 && Energy <= 1000) {
        count_E5001k++;
        if      (EnergyLoss/Energy < 0.01)                              count_E5001k_Loss0p1p++;
        else if (EnergyLoss/Energy < 0.05 && EnergyLoss/Energy >= 0.01) count_E5001k_Loss1p5p++;
        else if (EnergyLoss/Energy < 0.10 && EnergyLoss/Energy >= 0.05) count_E5001k_Loss5p10p++;
        else                                                            count_E5001k_Lossg10p++;
    } else continue;
  }

  std::cout << Form("100-200~MeV & %.2f & %.2f & %.2f & %.2f \\\\ \\hline \\hline",
                    100*(double)count_E100200_Loss0p1p/(double)count_E100200,
                    100*(double)count_E100200_Loss1p5p/(double)count_E100200,
                    100*(double)count_E100200_Loss5p10p/(double)count_E100200,
                    100*(double)count_E100200_Lossg10p/(double)count_E100200) << std::endl;
    std::cout << Form("200-500~MeV & %.2f & %.2f & %.2f & %.2f \\\\ \\hline \\hline",
                      100*(double)count_E200500_Loss0p1p/(double)count_E200500,
                      100*(double)count_E200500_Loss1p5p/(double)count_E200500,
                      100*(double)count_E200500_Loss5p10p/(double)count_E200500,
                      100*(double)count_E200500_Lossg10p/(double)count_E200500) << std::endl;
    std::cout << Form("500-1000~MeV & %.2f & %.2f & %.2f & %.2f \\\\ \\hline \\hline",
                      100*(double)count_E5001k_Loss0p1p/(double)count_E5001k,
                      100*(double)count_E5001k_Loss1p5p/(double)count_E5001k,
                      100*(double)count_E5001k_Loss5p10p/(double)count_E5001k,
                      100*(double)count_E5001k_Lossg10p/(double)count_E5001k) << std::endl;



  TCanvas* c = new TCanvas();
  StyleLogLog(c);
  c->SetLogz();

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
  delete(histogramFile);
}

void EnergyLossHistogram() {
    GeneralStyle();

    unsigned int N = analysisTree->GetEntries();
    std::vector<double> ELossBin = LogBins(60, 1.E0, 1.E3);
    TH1D* energyloss_100to500 = new TH1D(Form("elosshist_100to500_%s", particleName.c_str()), ";Energy Loss (MeV);Normalized Count", ELossBin.size() - 1, &ELossBin[0]);
    TH1D* energyloss_500to1k = new TH1D(Form("elosshist_500to1k_%s", particleName.c_str()), ";Energy Loss (MeV);Normalized Count", ELossBin.size() - 1, &ELossBin[0]);

    unsigned int count100to500 = 0;
    unsigned int count500to1k = 0;
    for (unsigned int i = 0; i < N; i++) {
        analysisTree->GetEntry(i);
        if (Energy >= 100 && Energy < 500) {
            energyloss_100to500->Fill(EnergyLoss);
            count100to500++;
        }
        else if (Energy >= 500 && Energy <= 1000) {
            energyloss_500to1k->Fill(EnergyLoss);
            count500to1k++;
        }
    }

    for (unsigned int i = 0; i < energyloss_100to500->GetSize(); i++) {
        if (energyloss_100to500->GetBinContent(i) > 0) {
            energyloss_100to500->SetBinError(i, 1/TMath::Sqrt((double)energyloss_100to500->GetBinContent(i)));
        } else {
            energyloss_100to500->SetBinError(i, 0);
        }
        energyloss_100to500->SetBinContent(i, 100*(double)energyloss_100to500->GetBinContent(i)/(double)count100to500);
    }

    for (unsigned int i = 0; i < energyloss_500to1k->GetSize(); i++) {
        if (energyloss_500to1k->GetBinContent(i) > 0) {
            energyloss_500to1k->SetBinError(i, 1/TMath::Sqrt((double)energyloss_500to1k->GetBinContent(i)));
        } else {
            energyloss_500to1k->SetBinError(i, 0);
        }
        energyloss_500to1k->SetBinContent(i, 100*(double)energyloss_500to1k->GetBinContent(i)/(double)count500to1k);
    }

    TCanvas* c = new TCanvas();
    StyleLogLog(c);
    StyleTH1(energyloss_100to500, kAzure, 2, kSolid, true, kDot, 1);
    energyloss_100to500->GetYaxis()->SetRangeUser(1E-4,1E2);
    energyloss_100to500->Draw("hist e");
    StyleTH1(energyloss_500to1k, kOrange, 2, kSolid, true, kDot, 1);
    energyloss_500to1k->Draw("hist e same");



    TLegend* leg = new TLegend(0.6, 0.65, 0.8, 0.85);
    leg->AddEntry(energyloss_100to500, Form("%s 100-500 MeV", particleName.c_str()), "pl");
    leg->AddEntry(energyloss_500to1k, Form("%s 500-1000 MeV", particleName.c_str()), "pl");
    leg->Draw();

    c->Modified();
    c->Update();
    c->SaveAs(Form("Results/elossHisto_%s.pdf", particleName.c_str()));

    histogramFile = new TFile("histograms.root", "UPDATE");
    histogramFile->cd();
    energyloss_100to500->Write(0, TObject::kOverwrite);
    energyloss_500to1k->Write(0, TObject::kOverwrite);
    histogramFile->Write(0, TObject::kOverwrite);
    histogramFile->Close();
    delete(histogramFile);
}
