#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "GlobalConfiguration.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      particleSource(nullptr) {
    
    // Создаем источник частиц
    particleSource = new G4GeneralParticleSource();
    
    // Настраиваем параметры генерации каонов
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* kaon = particleTable->FindParticle("kaon+");
    
    if (!kaon) {
        G4cerr << "Error: Could not find kaon+ particle definition!" << G4endl;
        return;
    }
    
    // Устанавливаем тип частицы (каон)
    particleSource->SetParticleDefinition(kaon);
    
    // Используем значения из GlobalConfiguration
    particleSource->GetCurrentSource()->GetEneDist()->SetMonoEnergy(GlobalConfig::PARTICLE_MOMENTUM);

    // Ставим источник перед цилиндром по оси z
    particleSource->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
    particleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., -1.5 * m));

    // Направляем строго вдоль оси z (в центр цилиндра и leadLayer)
    particleSource->GetCurrentSource()->GetAngDist()->SetAngDistType("direction");
    particleSource->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    // Генерируем первичную частицу
    particleSource->GeneratePrimaryVertex(event);
}