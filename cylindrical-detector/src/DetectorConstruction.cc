#include "../include/DetectorConstruction.hh"
#include "G4RunManager.hh"

DetectorConstruction::DetectorConstruction() 
    : worldMaterial(nullptr), 
      cylinderMaterial(nullptr), 
      leadMaterial(nullptr), 
      worldLogic(nullptr), 
      cylinderLogic(nullptr), 
      leadLayerLogic(nullptr) {
    
    // Параметры геометрии
    innerRadius = 1.0 * m;       // Внутренний радиус 1 метр
    cylinderHeight = 2.0 * m;    // Высота 2 метра
    leadThickness = 10 * cm;     // Толщина свинцового слоя 10 см
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineMaterials() {
    G4NistManager* nistManager = G4NistManager::Instance();
    
    // Создание материалов
    worldMaterial = nistManager->FindOrBuildMaterial("G4_AIR");
    cylinderMaterial = nistManager->FindOrBuildMaterial("G4_AIR");
    leadMaterial = nistManager->FindOrBuildMaterial("G4_Pb");

    // Проверка создания материалов
    if (!worldMaterial || !cylinderMaterial || !leadMaterial) {
        G4cerr << "Error: Failed to create materials!" << G4endl;
        exit(1);
    }
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Инициализация материалов
    DefineMaterials();

    // Создание мирового объема
    G4Box* worldSolid = new G4Box(
        "World", 
        3 * innerRadius, 
        3 * innerRadius, 
        3 * cylinderHeight
    );

    worldLogic = new G4LogicalVolume(
        worldSolid, 
        worldMaterial, 
        "World"
    );

    // Создание физического мирового объема
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(
        nullptr,                // no rotation
        G4ThreeVector(),        // at (0,0,0)
        worldLogic,             // its logical volume
        "World",                // its name
        nullptr,                // its mother  volume
        false,                  // no boolean operation
        0                       // copy number
    );

    // Создание внутреннего цилиндра
    G4Tubs* cylinderSolid = new G4Tubs(
        "Cylinder",             // name
        0,                      // inner radius
        innerRadius,            // outer radius
        cylinderHeight/2,       // height 
        0,                      // start angle
        360 * degree            // angle span
    );

    cylinderLogic = new G4LogicalVolume(
        cylinderSolid, 
        cylinderMaterial, 
        "Cylinder"
    );

    new G4PVPlacement(
        nullptr,                // no rotation
        G4ThreeVector(),        // at (0,0,0)
        cylinderLogic,          // its logical volume
        "Cylinder",             // its name
        worldLogic,             // its mother volume
        false,                  // no boolean operation
        0                       // copy number
    );

    // Создание свинцового слоя
    G4Tubs* leadLayerSolid = new G4Tubs(
        "LeadLayer",            // name
        innerRadius,            // inner radius
        innerRadius + leadThickness, // outer radius
        cylinderHeight/2,       // height
        0,                      // start angle
        360 * degree            // angle span
    );

    leadLayerLogic = new G4LogicalVolume(
        leadLayerSolid, 
        leadMaterial, 
        "LeadLayer"
    );

    new G4PVPlacement(
        nullptr,                // no rotation
        G4ThreeVector(),        // at (0,0,0)
        leadLayerLogic,         // its logical volume
        "LeadLayer",            // its name
        worldLogic,             // its mother volume
        false,                  // no boolean operation
        0                       // copy number
    );

    return worldPhys;
}
