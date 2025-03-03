// PrimaryGeneratorAction.hh
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithADouble.hh"

class G4UIcmdWithADouble;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction, public G4UImessenger {
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    // Make sure to declare this override explicitly
    virtual void GeneratePrimaries(G4Event*) override;
    
    // Make sure to declare this override explicitly
    virtual void SetNewValue(G4UIcommand*, G4String) override;

private:
    G4GeneralParticleSource* particleSource;
    G4UIcmdWithADouble* energyCmd;
};

#endif

