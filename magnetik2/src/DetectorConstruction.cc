#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fGasMixture(nullptr),
  fFieldMgr(nullptr),
  fMagField(nullptr),
  fTrackerLog(nullptr)   
{ 
  DefineMaterials();
}

DetectorConstruction::~DetectorConstruction()
{
  delete fMagField;
}

void DetectorConstruction::DefineMaterials()
{ 
  G4NistManager* nist = G4NistManager::Instance();
  
  // Элементы для создания смеси газов
  G4Element* elAr = nist->FindOrBuildElement("Ar");
  G4Element* elC = nist->FindOrBuildElement("C");
  G4Element* elO = nist->FindOrBuildElement("O");
  
  // Создаем CO2
  G4Material* CO2 = new G4Material("CarbonDioxide", 1.977*kg/m3, 2, kStateGas, 
                                  273.15*kelvin, 1.0*atmosphere);
  CO2->AddElement(elC, 1);
  CO2->AddElement(elO, 2);
  
  // Создаем аргон
  G4Material* Ar = new G4Material("Argon", 1.784*kg/m3, 1, kStateGas,
                                 273.15*kelvin, 1.0*atmosphere);
  Ar->AddElement(elAr, 1);
  
  // Создаем смесь Ar:CO2 (93:7)
  fGasMixture = new G4Material("GasMixture", 1.784*0.93*kg/m3 + 1.977*0.07*kg/m3, 
                              2, kStateGas, 273.15*kelvin, 1.0*atmosphere);
  fGasMixture->AddMaterial(Ar, 0.93);
  fGasMixture->AddMaterial(CO2, 0.07);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Мировой объем - лаборатория
  G4double world_sizeXY = 4.0*m;
  G4double world_sizeZ  = 4.0*m;
  
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(nullptr,             // без поворота
                      G4ThreeVector(),     // в (0,0,0)
                      logicWorld,          // его логический объем
                      "World",             // его название
                      nullptr,             // его родитель (нет)
                      false,               // булева операция
                      0,                   // копия №
                      true);               // проверка перекрытий
    
  // Трекер (цилиндрический газовый детектор)
  G4double tracker_diameter = 1.5*m;
  G4double tracker_length = 3.0*m;
  
  G4Tubs* solidTracker = new G4Tubs("Tracker",
                                   0,                    // внутренний радиус
                                   0.5*tracker_diameter, // внешний радиус
                                   0.5*tracker_length,   // половина длины по Z
                                   0*deg,                // начальный угол
                                   360*deg);             // конечный угол
  
  fTrackerLog = new G4LogicalVolume(solidTracker, fGasMixture, "Tracker");
  
  new G4PVPlacement(nullptr,               // без поворота
                   G4ThreeVector(),        // в (0,0,0)
                   fTrackerLog,            // логический объем
                   "TrackerPhys",          // имя
                   logicWorld,             // родительский объем
                   false,                  // булева операция
                   0,                      // копия №
                   true);                  // проверка перекрытий
  
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  // Создание магнитного поля
  fMagField = new G4UniformMagField(G4ThreeVector(0., 0., 1.0*tesla));
  
  // Создание менеджера полей
  fFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fFieldMgr->SetDetectorField(fMagField);
  fFieldMgr->CreateChordFinder(fMagField);
  
  // Применяем поле к трекеру
  fTrackerLog->SetFieldManager(fFieldMgr, true);
}
