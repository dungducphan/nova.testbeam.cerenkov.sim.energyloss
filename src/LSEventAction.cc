#include "LSEventAction.hh"

LSEventAction::LSEventAction(LSRunAction * runAction) : G4UserEventAction() {
}

LSEventAction::~LSEventAction() {
}

void LSEventAction::BeginOfEventAction(const G4Event * event) {
  std::cout << event->GetEventID();
}

void LSEventAction::EndOfEventAction(const G4Event * event) {
  std::cout << std::endl;
}
