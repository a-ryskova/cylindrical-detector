//Генерирует три типа частиц:
//Пионы (π+)
//Каоны (K+)
//Протоны (p)
//Импульс частиц: 50 МэВ/c
//Полярный угол вылета: 20 градусов
//Азимутальный угол генерируется случайным образом

#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4RandomDirection.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fPion = particleTable->FindParticle("pi+");
    fKaon = particleTable->FindParticle("kaon+");
    fProton = particleTable->FindParticle("proton");
    
    fMomentum = 50.0*MeV;
    fPolarAngle = 20.0*deg;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // Установка позиции частицы (точка взаимодействия пучков)
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    
    // Вычисление компонент импульса
    G4double px = fMomentum * std::sin(fPolarAngle);
    G4double pz = fMomentum * std::cos(fPolarAngle);
    
    // Случайный азимутальный угол
    G4double phi = G4UniformRand() * 360.0*deg;
    G4double py = px * std::sin(phi);
    px = px * std::cos(phi);
    
    fParticleGun->SetParticleMomentum(G4ThreeVector(px, py, pz));

    // Чередование типов частиц
    static int particleCounter = 0;
    switch(particleCounter % 3) {
        case 0:
            fParticleGun->SetParticleDefinition(fPion);
            break;
        case 1:
            fParticleGun->SetParticleDefinition(fKaon);
            break;
        case 2:
            fParticleGun->SetParticleDefinition(fProton);
            break;
    }
    particleCounter++;

    fParticleGun->GeneratePrimaryVertex(anEvent);
}