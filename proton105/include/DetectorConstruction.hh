#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4FieldManager;
class G4UniformMagField;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4LogicalVolume* GetTrackerLog() const { return fTrackerLog; }

  private:
    void DefineMaterials();
    
    G4Material* fGasMixture;
    G4FieldManager* fFieldMgr;
    G4UniformMagField* fMagField;
    G4LogicalVolume* fTrackerLog;
};

#endif
