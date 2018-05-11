#include "LSDetectorConstruction.hh"
#include "LSTOFSD.hh"

#include "G4SDManager.hh"

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

  // Mylar
  fMylar = nist->FindOrBuildMaterial("G4_MYLAR");

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
   * [Fake TOF]
   */
  tofBox = new G4Box("tofBox", tof_hx/2., tof_hy/2., tof_hz/2.);
  tofLog = new G4LogicalVolume(tofBox, fAir,"tofLog", 0, 0, 0);

  /**
   * [Mirror]
   */
  mirrorBox = new G4Box("mirrorBox", mirror_hx/2., mirror_hy/2., mirror_hz/2.);
  mirrorLog = new G4LogicalVolume(mirrorBox, fMylar,"mirrorLog", 0, 0, 0);

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
  co2Phy = new G4PVPlacement(0, G4ThreeVector(), co2Log, "CO2Phy", worldLog, false, 0);
  kaptonPhy_us = new G4PVPlacement(0, G4ThreeVector(0, 0, +usds_z), kaptonLog, "kaptonPhy_us", worldLog, false, 0);
  kaptonPhy_ds = new G4PVPlacement(0, G4ThreeVector(0, 0, -usds_z), kaptonLog, "kaptonPhy_ds", worldLog, false, 0);
  mirrorPhy = new G4PVPlacement(0, G4ThreeVector(0, 0, mirror_z), mirrorLog, "mirrorPy", co2Log, false, 0);
  tofPhy_us = new G4PVPlacement(0, G4ThreeVector(0, 0, +tof_z), tofLog, "tofPhy_us", worldLog, false, 0);
  tofPhy_ds = new G4PVPlacement(0, G4ThreeVector(0, 0, -tof_z), tofLog, "tofPhy_ds", worldLog, false, 0);
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

  return worldPhy;
}

void LSDetectorConstruction::ConstructSDandField() {
  LSTOFSD* TOFSD_ds = new LSTOFSD("TOFSD_ds");
  G4SDManager::GetSDMpointer()->AddNewDetector(TOFSD_ds);

  SetSensitiveDetector("tofLog", TOFSD_ds, true);
}
