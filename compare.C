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

TFile* rootfile1;
TTree* analysisTree1;

TFile* rootfile2;
TTree* analysisTree2;

TFile* rootfile3;
TTree* analysisTree3;

TFile* rootfile4;
TTree* analysisTree4;




Double_t  Energy1, Energy2, Energy3, Energy4;
Double_t  EnergyLoss1, EnergyLoss2, EnergyLoss3, EnergyLoss4;
TBranch   *b_Energy1, *b_Energy2, *b_Energy3, *b_Energy4;
TBranch   *b_EnergyLoss1, *b_EnergyLoss2, *b_EnergyLoss3, *b_EnergyLoss4;

std::string particleName;

void compare(std::string, unsigned int);
void EnergyHistogram();
void EnergyLossHistogram();

void compare(std::string pName, unsigned int NumberOfEvents) {
  particleName = pName;
  rootfile1 = new TFile(Form("Results/world/EnergyLoss_%s.root",particleName.c_str()), "READ");

  rootfile2 = new TFile(Form("Results/kapton.c02.world/EnergyLoss_%s.root",particleName.c_str()), "READ");

  rootfile3 = new TFile(Form("Results/mirror.kapton.c02.world/EnergyLoss_%s.root",particleName.c_str()), "READ");

  rootfile4 = new TFile(Form("Results/He_world/EnergyLoss_%s.root",particleName.c_str()), "READ");

  analysisTree1 = (TTree*) rootfile1->Get("AnalysisTree");
  analysisTree1->SetBranchAddress("Energy", &Energy1, &b_Energy1);
  analysisTree1->SetBranchAddress("EnergyLoss", &EnergyLoss1, &b_EnergyLoss1);

  analysisTree2 = (TTree*) rootfile2->Get("AnalysisTree");
  analysisTree2->SetBranchAddress("Energy", &Energy2, &b_Energy2);
  analysisTree2->SetBranchAddress("EnergyLoss", &EnergyLoss2, &b_EnergyLoss2);

  analysisTree3 = (TTree*) rootfile3->Get("AnalysisTree");
  analysisTree3->SetBranchAddress("Energy", &Energy3, &b_Energy3);
  analysisTree3->SetBranchAddress("EnergyLoss", &EnergyLoss3, &b_EnergyLoss3);

  analysisTree4 = (TTree*) rootfile4->Get("AnalysisTree");
  analysisTree4->SetBranchAddress("Energy", &Energy4, &b_Energy4);
  analysisTree4->SetBranchAddress("EnergyLoss", &EnergyLoss4, &b_EnergyLoss4);

    
  EnergyHistogram();
  EnergyLossHistogram();

  
  rootfile1->Close();
  rootfile2->Close();
  rootfile3->Close();
  rootfile4->Close();

}
void EnergyHistogram(){
  GeneralStyle();


  unsigned int N1 = analysisTree1->GetEntries();
  unsigned int N2 = analysisTree2->GetEntries();
  unsigned int N3 = analysisTree3->GetEntries();
  unsigned int N4 = analysisTree4->GetEntries();

  std::cout<< "N1 = " << N1 << std::endl;
  std::cout<< "N2 = " << N2 << std::endl;
  std::cout<< "N3 = " << N3 << std::endl;
  std::cout<< "N4 = " << N4 << std::endl;
  
  TH1D* energy1 = new TH1D("h1", Form("Energy_%s",particleName.c_str()), 2000, 0, 20000);
  TH1D* energy2 = new TH1D("h2", "kapton.co2.air", 2000, 0, 20000);
  TH1D* energy3 = new TH1D("h3", "air_FullExperiment", 2000, 0, 20000);
  TH1D* energy4 = new TH1D("h4", "He_FullExperiment", 2000, 0, 20000);

  for (unsigned int i = 0; i < N1; i++) {
    analysisTree1->GetEntry(i);
    energy1->Fill(Energy1);

  }

  for (unsigned int i = 0; i < N2; i++) {
    analysisTree2->GetEntry(i);
    energy2->Fill(Energy2);
    

  }

  for (unsigned int i = 0; i < N3; i++) {
    analysisTree3->GetEntry(i);
    energy3->Fill(Energy3);
  }
  
  for (unsigned int i = 0; i < N4; i++) {
    analysisTree4->GetEntry(i);
    energy4->Fill(Energy4);
  }



  TCanvas* c = new TCanvas();
  StyleLogLog(c);
  StyleTH1(energy1, kAzure, 2, kSolid, true, kDot, 1);
  energy1->GetYaxis()->SetRangeUser(1E0, 1E6);
  energy1->Draw("hist e");
  StyleTH1(energy2, kOrange, 2, kSolid, true, kDot, 1);
  energy2->Draw("hist e same");
  StyleTH1(energy3, kSpring, 2, kSolid, true, kDot, 1);
  energy3->Draw("hist e same");
  StyleTH1(energy4, kTeal, 2, kSolid, true, kDot, 1);
  energy4->Draw("hist e same");
  
  

  TLegend* leg = new TLegend(0.6, 0.65, 0.8, 0.85);
  leg->AddEntry(energy1, "Just Air");
  leg->AddEntry(energy2, "Air.Kapton.C02");
  leg->AddEntry(energy3, "Air.FullExperiment");
  leg->AddEntry(energy4, "He.FullExperiment");
  leg->Draw();

  
  c->Modified();
  c->Update();
  c->SaveAs(Form("Results/energyHisto_%s.pdf", particleName.c_str()));

  histogramFile = new TFile("histograms.root", "UPDATE");
  histogramFile->cd();
  energy1->Write(0, TObject::kOverwrite);
  energy2->Write(0, TObject::kOverwrite);
  energy3->Write(0, TObject::kOverwrite);
  energy4->Write(0, TObject::kOverwrite);
  histogramFile->Write(0, TObject::kOverwrite);
  histogramFile->Close();
  delete(histogramFile);
 
  


  
 }


void EnergyLossHistogram(){
  GeneralStyle();


  unsigned int N1 = analysisTree1->GetEntries();
  unsigned int N2 = analysisTree2->GetEntries();
  unsigned int N3 = analysisTree3->GetEntries();
  unsigned int N4 = analysisTree4->GetEntries();

  std::cout<< "N1 = " << N1 << std::endl;
  std::cout<< "N2 = " << N2 << std::endl;
  std::cout<< "N3 = " << N3 << std::endl;
  std::cout<< "N4 = " << N4 << std::endl;
  
  TH1D* energyloss1 = new TH1D("h1", Form("EnergyLoss_%s",particleName.c_str()), 2000, 0, 20000);
  TH1D* energyloss2 = new TH1D("h2", "kapton.co2.air", 2000, 0, 20000);
  TH1D* energyloss3 = new TH1D("h3", "air_FullExperiment", 2000, 0, 20000);
  TH1D* energyloss4 = new TH1D("h4", "He_FullExperiment", 2000, 0, 20000);

  for (unsigned int i = 0; i < N1; i++) {
    analysisTree1->GetEntry(i);
    energyloss1->Fill(EnergyLoss1);

  }

  for (unsigned int i = 0; i < N2; i++) {
    analysisTree2->GetEntry(i);
    energyloss2->Fill(EnergyLoss2);
    

  }

  for (unsigned int i = 0; i < N3; i++) {
    analysisTree3->GetEntry(i);
    energyloss3->Fill(EnergyLoss3);
  }
  
  for (unsigned int i = 0; i < N4; i++) {
    analysisTree4->GetEntry(i);
    energyloss4->Fill(EnergyLoss4);
  }



  TCanvas* c = new TCanvas();
  StyleLogLog(c);
  StyleTH1(energyloss1, kAzure, 2, kSolid, true, kDot, 1);
  energyloss1->GetYaxis()->SetRangeUser(1E0, 1E6);
  energyloss1->Draw("hist e");
  StyleTH1(energyloss2, kOrange, 2, kSolid, true, kDot, 1);
  energyloss2->Draw("hist e same");
  StyleTH1(energyloss3, kSpring, 2, kSolid, true, kDot, 1);
  energyloss3->Draw("hist e same");
  StyleTH1(energyloss4, kTeal, 2, kSolid, true, kDot, 1);
  energyloss4->Draw("hist e same");
  
  

  TLegend* leg = new TLegend(0.6, 0.65, 0.8, 0.85);
  leg->AddEntry(energyloss1, "Just Air");
  leg->AddEntry(energyloss2, "Air.Kapton.C02");
  leg->AddEntry(energyloss3, "Air.FullExperiment");
  leg->AddEntry(energyloss4, "He.FullExperiment");
  leg->Draw();

  
  c->Modified();
  c->Update();
  c->SaveAs(Form("Results/energylossHisto_%s.pdf", particleName.c_str()));

  histogramFile = new TFile("histograms.root", "UPDATE");
  histogramFile->cd();
  energyloss1->Write(0, TObject::kOverwrite);
  energyloss2->Write(0, TObject::kOverwrite);
  energyloss3->Write(0, TObject::kOverwrite);
  energyloss4->Write(0, TObject::kOverwrite);
  histogramFile->Write(0, TObject::kOverwrite);
  histogramFile->Close();
  delete(histogramFile);
 
}  

