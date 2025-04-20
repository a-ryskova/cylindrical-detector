#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(nullptr)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // Выбираем каон K+ с импульсом 50 МэВ/c
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("kaon+");
  fParticleGun->SetParticleDefinition(particleDefinition);
  
  // Устанавливаем импульс 50 МэВ/c
  G4double momentum = 3.0*GeV;
  G4double theta = 90.0*deg;  // полярный угол
  G4double phi = 0.0*deg;     // азимутальный угол
  
  G4double px = momentum*std::sin(theta)*std::cos(phi);
  G4double py = momentum*std::sin(theta)*std::sin(phi);
  G4double pz = momentum*std::cos(theta);
  
  fParticleGun->SetParticleMomentum(G4ThreeVector(px, py, pz));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Частица вылетает из центра детектора (из точки встречи пучков)
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::SetParticleType(G4String particleName)
{
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  
  if (particleDefinition != nullptr)
    fParticleGun->SetParticleDefinition(particleDefinition);
}
