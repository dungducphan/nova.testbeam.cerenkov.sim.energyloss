#ifndef LSRunAction_h
#define LSRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "globals.hh"

#include "LSPrimaryGeneratorAction.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"

class G4Run;
class G4RunManager;

class LSRunAction : public G4UserRunAction {
public:
  LSRunAction();
  virtual ~LSRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  TFile* fOutput;
  TTree* fAnalysisTree;
  double EnergyLoss;
  double Energy;
  TH1I*  hEventInRunCount;

  virtual void FillAnalysisTree(double energy, double energyLoss);
  virtual void FillEventCountHist();

};

#endif
