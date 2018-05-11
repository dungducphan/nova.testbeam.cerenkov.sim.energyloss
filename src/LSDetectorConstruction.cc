#include "LSDetectorConstruction.hh"

LSDetectorConstruction::LSDetectorConstruction() : G4VUserDetectorConstruction() {
}

LSDetectorConstruction::~LSDetectorConstruction() {
}

void LSDetectorConstruction::BuildMaterial() {
  G4NistManager * nist = G4NistManager::Instance();

  // Air
  fAir = nist->FindOrBuildMaterial("G4_AIR");

  // CO2
  fCO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

  // Kapton
  fKapton = nist->FindOrBuildMaterial("G4_KAPTON");

  return;
}

G4VPhysicalVolume * LSDetectorConstruction::Construct() {
  BuildMaterial();
  
  // Overlapping check
  G4bool checkOverlaps = true;

  /**
   * [Kapton]
   */
  kaptonBox = new G4Box("kaptonBox", kapton_hx/2., kapton_hy/2., kapton_hz/2.);
  kaptonLog = new G4LogicalVolume(kaptonBox, fKapton,"kaptonLog", 0, 0, 0);

  /**
   * [CO2 Volume description]
   */
  co2Box = new G4Box("co2Box", co2_hx/2., co2_hy/2., co2_hz/2.);
  co2Log = new G4LogicalVolume(co2Box, fCO2,"co2Log", 0, 0, 0);

  /**
   * [World description]
   */
  worldBox = new G4Box("WorldBox", world_hx/2., world_hy/2., world_hz/2.);
  worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");


  /**
   * Placing volumes in WORLD
   * Remember to define all the needed logical volumes before arranging place hierarchy!
   */
  kaptonPhy_us = new G4PVPlacement(0, G4ThreeVector(0, 0, +usds_z), kaptonLog, "kaptonPhy_us", worldLog, false, 0);
  kaptonPhy_ds = new G4PVPlacement(0, G4ThreeVector(0, 0, -usds_z), kaptonLog, "kaptonPhy_ds", worldLog, false, 0);
  kaptonPhy_mirror = new G4PVPlacement(0, G4ThreeVector(), kaptonLog, "kaptonPhy_mirror", worldLog, false, 0);
  co2Phy = new G4PVPlacement(0, G4ThreeVector(), co2Log, "LArVolumePhy", worldLog, false, 0);
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

  return worldPhy;
}
