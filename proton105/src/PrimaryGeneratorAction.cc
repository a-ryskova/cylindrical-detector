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

  // Определяем частицу
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  fParticleGun->SetParticleDefinition(particleDefinition);
  
  // Установка параметров пучка
  G4double momentum = 150.0*MeV;
  G4double theta = 90.0*deg;
  G4double phi = 0.0*deg;
  
  G4double px = momentum*std::sin(theta)*std::cos(phi);
  G4double py = momentum*std::sin(theta)*std::sin(phi);
  G4double pz = momentum*std::cos(theta);
  
  // Устанавливаем импульс только один раз
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
