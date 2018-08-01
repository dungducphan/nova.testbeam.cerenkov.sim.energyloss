#include "LSDetectorConstruction.hh"
#include "LSTOFSD.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"


#include "G4SDManager.hh"

LSDetectorConstruction::LSDetectorConstruction() : G4VUserDetectorConstruction() {
}

LSDetectorConstruction::~LSDetectorConstruction() {
}

void LSDetectorConstruction::BuildMaterial() {
  G4NistManager * nist = G4NistManager::Instance();

  G4int natoms, ncomponents;
  G4double a, z, den;


  G4Element* H = new G4Element("Hydrogen", "H", z=1, a  = 1.01*g/mole);
  G4Element* C = new G4Element("Carbon", "C", z=6, a = 12.01*g/mole);

  
  // Air
  fAir = nist->FindOrBuildMaterial("G4_AIR");

  //Helium
  fHe = nist->FindOrBuildMaterial("G4_He");

  // CO2
  fCO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

  // Kapton
  fKapton = nist->FindOrBuildMaterial("G4_KAPTON");

  // Mylar
  fMylar = nist->FindOrBuildMaterial("G4_MYLAR");

  // Vacuum
  G4double density= 2.376e-15*g/cm3;
  G4double temperature= 300*kelvin;
  G4double pressure= 2.0e-7*bar;
  fVacuum = new G4Material("Vacuum", density, 1, kStateGas,temperature,pressure);
  fVacuum->AddMaterial(fAir, 1.);

  //PolyvinylToulene TOF
  fPVT = new G4Material("PVT", den=1.023*g/cm3, ncomponents=2, kStateSolid);
  fPVT->AddElement(C, natoms=9);
  fPVT->AddElement(H, natoms=10);

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
  tofLog = new G4LogicalVolume(tofBox, fPVT, "tofLog", 0, 0, 0);

  /**
   * [Detector]
   */
  detBox = new G4Box("detBox", det_hx/2., det_hy/2., det_hz/2.);
  detLog = new G4LogicalVolume(detBox, fAir, "detLog", 0, 0, 0);
  
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
  worldLog = new G4LogicalVolume(worldBox, fVacuum, "WorldLog");


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
  detPhy_us = new G4PVPlacement(0, G4ThreeVector(0, 0, +det_z), detLog, "detPhy_us", worldLog, false, 0);
  detPhy_ds = new G4PVPlacement(0, G4ThreeVector(0, 0, -det_z), detLog, "detPhy_ds", worldLog, false, 0);
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

  return worldPhy;
}

void LSDetectorConstruction::ConstructSDandField() {
  LSTOFSD* DETSD_ds = new LSTOFSD("DETSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(DETSD_ds);

  SetSensitiveDetector("detLog", DETSD_ds, true);
}
