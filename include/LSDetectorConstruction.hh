#ifndef LSDetectorConstruction_h
#define LSDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4PVReplica.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class LSDetectorConstruction : public G4VUserDetectorConstruction {
public:
  LSDetectorConstruction();
  virtual ~LSDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  virtual void BuildMaterial();

private:
  /**
   * WORLD
   */
  G4double world_hx = 5.0 * m;
  G4double world_hy = 5.0 * m;
  G4double world_hz = 5.0 * m;
  G4Box * worldBox;
  G4LogicalVolume * worldLog;
  G4VPhysicalVolume * worldPhy;

  /**
   * CO2 VOLUME
   */
  G4double co2_hx = 2.0 * m;
  G4double co2_hy = 2.0 * m;
  G4double co2_hz = 2.5 * m;
  G4Box * co2Box;
  G4LogicalVolume * co2Log;
  G4VPhysicalVolume * co2Phy;

  /**
   * Kapton VOLUME
   */
  G4double kapton_hx = 1.0 * m;
  G4double kapton_hy = 1.0 * m;
  G4double kapton_hz = 1.0 * mm;
  G4Box * kaptonBox;
  G4LogicalVolume * kaptonLog;
  G4VPhysicalVolume * kaptonPhy_us, * kaptonPhy_ds, * kaptonPhy_mirror; // upstream + downstream
  G4double usds_z = co2_hz/2 + kapton_hz/2;

  /**
   * MATERIALS
   */
  G4Material * fAir;
  G4Material * fCO2;
  G4Material * fKapton;
};


#endif
