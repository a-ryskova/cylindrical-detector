#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "MySensitiveDetector.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
    : worldMaterial(nullptr), cylinderMaterial(nullptr), leadMaterial(nullptr),
      worldLogic(nullptr), cylinderLogic(nullptr), leadLayerLogic(nullptr)
{
    innerRadius = 1.0 * m;
    cylinderHeight = 2.0 * m;
    leadThickness = 10 * cm;
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineMaterials() {
    G4NistManager* nistManager = G4NistManager::Instance();

    worldMaterial = nistManager->FindOrBuildMaterial("G4_AIR");
    cylinderMaterial = nistManager->FindOrBuildMaterial("G4_AIR");
    leadMaterial = nistManager->FindOrBuildMaterial("G4_Pb");

    if (!worldMaterial || !cylinderMaterial || !leadMaterial) {
        G4cerr << "Error: Failed to create materials!" << G4endl;
        exit(1);
    }
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    DefineMaterials();

    // World
    G4Box* worldSolid = new G4Box(
        "World",
        3 * innerRadius,
        3 * innerRadius,
        3 * cylinderHeight
    );
    worldLogic = new G4LogicalVolume(worldSolid, worldMaterial, "World");
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(
        nullptr, G4ThreeVector(), worldLogic, "World", nullptr, false, 0
    );

    // Inner cylinder
    G4Tubs* cylinderSolid = new G4Tubs(
        "Cylinder",
        0,
        innerRadius,
        cylinderHeight / 2,
        0,
        360 * degree
    );
    cylinderLogic = new G4LogicalVolume(cylinderSolid, cylinderMaterial, "Cylinder");
    new G4PVPlacement(
        nullptr, G4ThreeVector(), cylinderLogic, "Cylinder", worldLogic, false, 0
    );

    // Lead layer
    G4Tubs* leadLayerSolid = new G4Tubs(
        "LeadLayer",
        innerRadius,
        innerRadius + leadThickness,
        cylinderHeight / 2,
        0,
        360 * degree
    );
    leadLayerLogic = new G4LogicalVolume(leadLayerSolid, leadMaterial, "LeadLayer");
    new G4PVPlacement(
        nullptr, G4ThreeVector(), leadLayerLogic, "LeadLayer", worldLogic, false, 0
    );

    return worldPhys;
}

void DetectorConstruction::ConstructSDandField() {
    MySensitiveDetector* sensitiveDetector = new MySensitiveDetector("TrackerSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);

    if (leadLayerLogic) {
        leadLayerLogic->SetSensitiveDetector(sensitiveDetector);
        G4cout << "Sensitive detector attached to lead layer" << G4endl;
    } else {
        G4cerr << "ERROR: Lead layer logical volume not found!" << G4endl;
    }
}