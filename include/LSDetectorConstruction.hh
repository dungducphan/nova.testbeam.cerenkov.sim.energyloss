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
   * LAr VOLUME
   */
  G4double larvol_hx = 3.0 * m;
  G4double larvol_hy = 3.0 * m;
  G4double larvol_hz = 3.0 * m;
  G4Box * larvolBox;
  G4LogicalVolume * larvolLog;
  G4VPhysicalVolume * larvolPhy;

  /**
   * LAr HOUSING
   */
  G4double larhouse_thickx = 10.0 * cm; // Wall thickness
  G4double larhouse_thicky = 10.0 * cm;
  G4double larhouse_thickz = 10.0 * cm;
  G4double larhouse_hx = larvol_hx + larhouse_thickx * 2.;
  G4double larhouse_hy = larvol_hy + larhouse_thicky * 2.;
  G4double larhouse_hz = larvol_hz + larhouse_thickz * 2.;
  G4Box * larhouseBox;
  G4LogicalVolume * larhouseLog;
  G4VPhysicalVolume * larhousePhy;

  /**
   * MATERIALS
   */
  G4Material * fAir;
  G4Material * fLAr;
  G4Material * fAl;

  /**
   * MATERIAL PROPERTY TABLE
   */
  G4MaterialPropertiesTable* fLAr_mt;
};


#endif
