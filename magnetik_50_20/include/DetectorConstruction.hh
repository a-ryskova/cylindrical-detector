#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "MySensitiveDetector.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

private:
    void DefineMaterials();

    // Параметры геометрии
    G4double innerRadius;
    G4double cylinderHeight;
    G4double leadThickness;

    // Материалы
    G4Material* worldMaterial;
    G4Material* cylinderMaterial;
    G4Material* leadMaterial;

    // Логические объемы
    G4LogicalVolume* worldLogic;
    G4LogicalVolume* cylinderLogic;
    G4LogicalVolume* leadLayerLogic;
};

#endif
