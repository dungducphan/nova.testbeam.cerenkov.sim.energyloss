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

void Analyze(std::string, unsigned int);
void EnergyLossHistogram();

void Analyze(std::string pName, unsigned int NumberOfEvents) {
  particleName = pName;
  rootfile = new TFile(Form("Results/0.52TOF/EnergyLoss_%s.root", particleName.c_str()), "READ");
  analysisTree = (TTree*) rootfile->Get("AnalysisTree");
  analysisTree->SetBranchAddress("Energy", &Energy, &b_Energy);
  analysisTree->SetBranchAddress("EnergyLoss", &EnergyLoss, &b_EnergyLoss);

  EnergyLossHistogram();

  rootfile->Close();



  
}


void EnergyLossHistogram() {
//   // GeneralStyle();



    unsigned int N = analysisTree->GetEntries();
    // std::vector<double> ELossBin = LogBins(60, 1.E0, 1.E3);
    // TH1D* energylost = new TH1D(Form("elosshist_%s", particleName.c_str()), ";Energy Loss (MeV);Normalized Count", ELossBin.size() - 1, &ELossBin[0]);

    TH1D* energylost= new TH1D("elosshist_%s", particleName.c_str(), 60, 1.E0, 1.E3);
    TH1D* energy = new TH1D("Energy_hist", particleName.c_str(), 60, 1.E0, 1.E3);

    for (unsigned int i = 0; i < N; i++) {
        analysisTree->GetEntry(i);
	energylost->Fill(EnergyLoss);
	energy->Fill(Energy);

    }

    TCanvas* c1 = new TCanvas();
    TCanvas* c2 = new TCanvas();
// //
    //StyleLogLog(c1);

    energylost->Draw();
    energy->Draw();




    StyleTH1(energylost, kAzure, 2, kSolid, true, kDot, 1);
    energylost->GetYaxis()->SetRangeUser(1E-4,1E2);
    StyleTH1(energy, kAzure, 2, kSolid, true, kDot, 1);
    energylost->Draw("hist e");
    energy->Draw("hist e");
    
   



    TLegend* leg = new TLegend(0.6, 0.65, 0.8, 0.85);
    leg->AddEntry(energylost, Form("%s", particleName.c_str()), "pl");
    leg->Draw();

    c1->Modified();
    c1->Update();
    c2->Modified();
    c2->Update();
    c1->SaveAs(Form("Results/elossHisto_%s.pdf", particleName.c_str()));
    c2->SaveAs(Form("Results/energy_%s.pdf", particleName.c_str()));

    histogramFile = new TFile("histograms.root", "UPDATE");
    histogramFile->cd();
    energylost->Write(0, TObject::kOverwrite);
    histogramFile->Write(0, TObject::kOverwrite);
    histogramFile->Close();
    
    delete(histogramFile);

    histogramFile = new TFile("histograms.root", "UPDATE");
    histogramFile->cd();
    energy->Write(0, TObject::kOverwrite);
    histogramFile->Write(0, TObject::kOverwrite);
    histogramFile->Close();

    delete(histogramFile);


  // TCanvas* c = new TCanvas();
  // StyleLogLog(c);
  // c->SetLogz();

  // energylost->GetXaxis()->CenterTitle();
  // energylost->GetYaxis()->CenterTitle();
  // energylost->Draw("COLZ");

  // c->Modified();
  // c->Update();
  // c->SaveAs(Form("Results/eloss_%s.pdf", particleName.c_str()));

  // histogramFile = new TFile("histograms.root", "UPDATE");
  // histogramFile->cd();
  // energylost->Write(0, TObject::kOverwrite);
  // histogramFile->Write(0, TObject::kOverwrite);
  // histogramFile->Close();
  // delete(histogramFile);


}
