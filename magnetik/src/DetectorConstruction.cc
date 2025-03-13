#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "MySensitiveDetector.hh"
#include "G4SDManager.hh"
//Создается цилиндрический детектор диаметром 1.5 м и длиной 3 м
//Заполняется газовой смесью Ar:CO2 (93:7) при атмосферном давлении
//Устанавливается однородное магнитное поле 1 Тесла вдоль оси Z
DetectorConstruction::DetectorConstruction()
    : trackerRadius(0.75*m),
      trackerLength(3.0*m),
      magneticField(1.0*tesla),
      worldMaterial(nullptr),
      trackerGas(nullptr),
      worldLogical(nullptr),
      logicTracker(nullptr),
      physWorld(nullptr),
      physTracker(nullptr),
      fMagField(nullptr),
      fFieldMgr(nullptr)
{
    DefineMaterials();
}
DetectorConstruction::~DetectorConstruction()
{
    delete fMagField;
}
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Создание мира
    G4double worldSize = 5.0*m;
    G4Box* solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    worldLogical = new G4LogicalVolume(solidWorld, worldMaterial, "World");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, 0);

    // Создание трекера
    G4Tubs* solidTracker = new G4Tubs("Tracker", 0, trackerRadius, trackerLength/2, 0, 360*deg);
    logicTracker = new G4LogicalVolume(solidTracker, trackerGas, "Tracker");
    physTracker = new G4PVPlacement(0, G4ThreeVector(), logicTracker, "Tracker", worldLogical, false, 0);

    return physWorld;
}
void DetectorConstruction::ConstructSDandField()
{
    // Создание и регистрация чувствительного детектора
    MySensitiveDetector* sensDet = new MySensitiveDetector("TrackerSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    
    // Назначение чувствительного детектора логическому объему трекера
    if (logicTracker != nullptr) {
        SetSensitiveDetector("Tracker", sensDet);
    }

    // Создание однородного магнитного поля
    if (!fMagField) {
        // Создаем поле величиной 1 Тесла вдоль оси Z
        fMagField = new G4UniformMagField(G4ThreeVector(0., 0., magneticField));
        
        // Получаем менеджер поля и устанавливаем наше поле
        fFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        fFieldMgr->SetDetectorField(fMagField);
        
        // Создаем навигатор по полю для расчета траекторий частиц
        fFieldMgr->CreateChordFinder(fMagField);
    }
}

void DetectorConstruction::DefineMaterials()

{
    G4NistManager* nist = G4NistManager::Instance();
    
    // Определение материала мира
    worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
    
    // Создание газовой смеси Ar:CO2 (93:7)
    G4Material* Ar = nist->FindOrBuildMaterial("G4_Ar");
    G4Material* CO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    
    G4double density = 1.784*mg/cm3;  // Плотность при атмосферном давлении
    trackerGas = new G4Material("TrackerGas", density, 2);
    trackerGas->AddMaterial(Ar, 0.93);
    trackerGas->AddMaterial(CO2, 0.07);
}