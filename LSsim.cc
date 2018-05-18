#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4String.hh"

#include "LSDetectorConstruction.hh"
#include "LSPhysicsList.hh"
#include "LSActionInitialization.hh"

#include "QGSP_BIC_HP.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char ** argv) {
  G4RunManager * runManager = new G4RunManager;

  runManager->SetUserInitialization(new LSDetectorConstruction());
  runManager->SetUserInitialization(new LSPhysicsList());
  runManager->SetUserInitialization(new LSActionInitialization());

  #ifdef G4VIS_USE
  G4VisManager * visManager = new G4VisExecutive;
  visManager->Initialize();
  #endif

  runManager->Initialize();

  // get the pointer to the UI manager and set verbosities
  G4UImanager * UImanager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    #ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
    #endif
  } else {
    G4String command = "/control/execute ";
    G4String filename = argv[1];
    UImanager->ApplyCommand(command + filename);
  }

  #ifdef G4VIS_USE
  delete visManager;
  #endif

  delete runManager;

  return 0;
}
