#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction(RunAction* runAction) 
    : fRunAction(runAction), 
      fNeutronPassedCounter(0), 
      fEventAbsorbedEnergy(0.0) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    // Сброс счетчиков перед началом события
    fNeutronPassedCounter = 0;
    fEventAbsorbedEnergy = 0.0;
}

void EventAction::EndOfEventAction(const G4Event*) {
    // Обновление статистики для всего прогона
    fRunAction->AddNeutronsPassed(fNeutronPassedCounter);
    fRunAction->AddAbsorbedEnergy(fEventAbsorbedEnergy);
}

void EventAction::ProcessNeutronStep(const G4Step* step) {
    G4Track* track = step->GetTrack();
    
    // Проверяем, что это нейтрон
    if (track->GetParticleDefinition()->GetParticleName() != "neutron") 
        return;

    // Проверяем поглощение энергии в свинцовом слое
    G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()
                            ->GetVolume()->GetName();
    
    if (volumeName == "LeadLayer") {
        // Поглощенная энергия в этом шаге
        G4double energyDeposited = step->GetTotalEnergyDeposit();
        
        if (energyDeposited > 0) {
            fEventAbsorbedEnergy += energyDeposited;
            fNeutronPassedCounter++;
        }
    }
}
