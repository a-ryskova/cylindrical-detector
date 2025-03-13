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
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private:
    // Методы
    void DefineMaterials();
    void SetupGeometry();

    // Параметры трекера
    G4double trackerRadius;    // Радиус трекера (0.75 м)
    G4double trackerLength;    // Длина трекера (3.0 м)
    G4double magneticField;    // Величина магнитного поля (1.0 Тл)

    // Материалы
    G4Material* worldMaterial;
    G4Material* trackerGas;    // Смесь Ar:CO2 (93:7)

    // Логические объемы
    G4LogicalVolume* worldLogical;
    G4LogicalVolume* logicTracker;

    // Физические объемы
    G4VPhysicalVolume* physWorld;
    G4VPhysicalVolume* physTracker;

    // Магнитное поле
    G4UniformMagField* fMagField;
    G4FieldManager* fFieldMgr;
};

#endif
