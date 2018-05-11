#include "LSDetectorConstruction.hh"

LSDetectorConstruction::LSDetectorConstruction() : G4VUserDetectorConstruction() {
}

LSDetectorConstruction::~LSDetectorConstruction() {
}

void LSDetectorConstruction::BuildMaterial() {
  G4NistManager * nist = G4NistManager::Instance();
  G4double z, a, density;

  // Air
  fAir = nist->FindOrBuildMaterial("G4_AIR");

  // Aluminum
  fAl = nist->FindOrBuildMaterial("G4_Al");

  // Liquid Argon (Properties data taken from LArSOFT)
  fLAr = new G4Material("LAr",z = 18., a = 39.948 * g/mole, density = 1.3954 * g/cm3);
  G4double LAr_ScintEnergy[] = {6.0*eV, 6.7*eV, 7.1*eV, 7.4*eV, 7.7*eV, 7.9*eV, 8.1*eV, 8.4*eV,  8.5*eV, 8.6*eV, 8.8*eV, 9.0*eV, 9.1*eV, 9.4*eV, 9.8*eV, 10.4*eV, 10.7*eV};
  const G4int LArScintNum = sizeof(LAr_ScintEnergy)/sizeof(G4double);
  G4double LAr_SCINT[] = {0.0, 0.04, 0.12, 0.27, 0.44, 0.62, 0.80, 0.91, 0.92, 0.85, 0.70, 0.50, 0.31, 0.13, 0.04,  0.01, 0.0};
  assert(sizeof(LAr_SCINT) == sizeof(LAr_ScintEnergy));
  G4double LAr_RIndEnergy[] = {1.900*eV,  2.934*eV,  3.592*eV,  5.566*eV,  6.694*eV,  7.540*eV,  8.574*eV,  9.044*eV,  9.232*eV, 9.420*eV,  9.514*eV,  9.608*eV,  9.702*eV,  9.796*eV,  9.890*eV,  9.984*eV,  10.08*eV,  10.27*eV,  10.45*eV,  10.74*eV,  10.92*eV};
  const G4int LArRIndNum = sizeof(LAr_RIndEnergy)/sizeof(G4double);
  G4double LAr_RIND[] = {1.232, 1.236, 1.240, 1.261, 1.282, 1.306, 1.353, 1.387, 1.404, 1.423, 1.434, 1.446, 1.459, 1.473, 1.488, 1.505,  1.524, 1.569, 1.627, 1.751, 1.879};
  assert(sizeof(LAr_RIND) == sizeof(LAr_RIndEnergy));
  G4double LAr_AbsLEnergy[] = {4*eV, 5*eV, 6*eV, 7*eV, 8*eV, 9*eV, 10*eV, 11*eV};
  const G4int LArAbsLNum = sizeof(LAr_AbsLEnergy)/sizeof(G4double);
  G4double LAr_ABSL[]  = {2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm};
  assert(sizeof(LAr_ABSL) == sizeof(LAr_AbsLEnergy));
  fLAr_mt = new G4MaterialPropertiesTable();
  fLAr_mt->AddProperty("FASTCOMPONENT", LAr_ScintEnergy, LAr_SCINT, LArScintNum);
  fLAr_mt->AddProperty("SLOWCOMPONENT", LAr_ScintEnergy, LAr_SCINT, LArScintNum);
  fLAr_mt->AddProperty("RINDEX",        LAr_RIndEnergy, LAr_RIND,  LArRIndNum);
  fLAr_mt->AddProperty("ABSLENGTH",     LAr_AbsLEnergy, LAr_ABSL,  LArAbsLNum);
  fLAr_mt->AddConstProperty("SCINTILLATIONYIELD",24000./MeV);
  fLAr_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  fLAr_mt->AddConstProperty("FASTTIMECONSTANT",6.*ns);
  fLAr_mt->AddConstProperty("SLOWTIMECONSTANT",1590.*ns);
  fLAr_mt->AddConstProperty("YIELDRATIO",0.3);
  fLAr->SetMaterialPropertiesTable(fLAr_mt);
  fLAr->GetIonisation()->SetBirksConstant(0.69*mm/MeV);

  return;
}

G4VPhysicalVolume * LSDetectorConstruction::Construct() {
  BuildMaterial();
  
  // Overlapping check
  G4bool checkOverlaps = true;

  /**
   * [LAr Housing Volume description]
   * @param  [name]       [description]
   * @param  larhouse_hx  [x dimension of LAr housing volume]
   * @param  larhouse_hy  [y dimension of LAr housing volume]
   * @param  larhouse_hz  [z dimension of LAr housing volume]
   * @param  fAl          [LAr housing volume is made of aluminum]
   */
  larhouseBox = new G4Box("LArHouseBox", larhouse_hx/2., larhouse_hy/2., larhouse_hz/2.);
  larhouseLog = new G4LogicalVolume(larhouseBox, fAl, "LArHouseLog", 0, 0, 0);

  /**
   * [LAr Volume description]
   * @param  [name]     [description]
   * @param  larvol_hx  [x dimension of LAr volume]
   * @param  larvol_hy  [y dimension of LAr volume]
   * @param  larvol_hz  [z dimension of LAr volume]
   * @param  fLAr       [LAr volume is made of liquid argon]
   */
  larvolBox = new G4Box("LArVolumeBox", larvol_hx/2., larvol_hy/2., larvol_hz/2.);
  larvolLog = new G4LogicalVolume(larvolBox, fLAr,"LArVolumeLog", 0, 0, 0);
  // Set Visual Properties
  G4VisAttributes* larvolVA = new G4VisAttributes();
  larvolVA->SetColor(G4Colour(0.8,0.8,0.8));
  larvolVA->SetForceWireframe(true);
  larvolLog->SetVisAttributes(larvolVA);

  /**
   * [World description]
   * @param  [name]   [description]
   * @param  world_hx [x dimension of world volume]
   * @param  world_hy [y dimension of world volume]
   * @param  world_hz [z dimension of world volume]
   * @param  fAir     [the world is made of air]
   * @return          [worldPhy as a G4VPhysicalVolume]
   */
  worldBox = new G4Box("WorldBox", world_hx, world_hy, world_hz);
  worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");


  /**
   * Placing volumes in WORLD
   * Remember to define all the needed logical volumes before arranging place hierarchy!
   */
  larhousePhy = new G4PVPlacement(0, G4ThreeVector(), larhouseLog, "LArVolumePhy", worldLog, false, 0);
  larvolPhy = new G4PVPlacement(0, G4ThreeVector(), larvolLog, "LArVolumePhy", larhouseLog, false, 0);
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

  return worldPhy;
}
