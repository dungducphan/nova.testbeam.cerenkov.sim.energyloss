#ifndef LSEventAction_h
#define LSEventAction_h 1

#include "LSRunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4SDManager.hh"
#include "LSDETSD.hh"
#include "LSDETHit.hh"
#include "G4SystemOfUnits.hh"

class LSRunAction;
class LSDETHit;

class LSEventAction : public G4UserEventAction {
public:
  LSEventAction(LSRunAction* runAction);
  virtual ~LSEventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

private:
  LSRunAction* fRunAction;
};

#endif
