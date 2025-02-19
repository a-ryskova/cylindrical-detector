// PrimaryGeneratorAction.cc
#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4UImanager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() 
: G4VUserPrimaryGeneratorAction(),
  G4UImessenger(),
  particleSource(nullptr),
  energyCmd(nullptr)
{
    particleSource = new G4GeneralParticleSource();
    
    G4ParticleDefinition* neutron = 
        G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    
    particleSource->SetParticleDefinition(neutron);
    
    // 4π source from center
    particleSource->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
    particleSource->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
    particleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0,0,0));

    // Set default energy to 2 MeV
    particleSource->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
    particleSource->GetCurrentSource()->GetEneDist()->SetMonoEnergy(2.0*MeV);

    // Create energy command
    energyCmd = new G4UIcmdWithADouble("/gun/energy", this);
    energyCmd->SetGuidance("Set neutron energy");
    energyCmd->SetParameterName("Energy", false);
    energyCmd->SetRange("Energy>0.");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleSource;
    delete energyCmd;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    particleSource->GeneratePrimaryVertex(event);
}

void PrimaryGeneratorAction::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == energyCmd) {
        G4double energy = energyCmd->GetNewDoubleValue(newValue);
        particleSource->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        particleSource->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);
    }
}
