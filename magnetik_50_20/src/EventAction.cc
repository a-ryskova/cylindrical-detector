#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction(RunAction* runAction) 
    : fRunAction(runAction), 
      fKaonPassedCounter(0), 
      fEventAbsorbedEnergy(0.0) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    // Reset counters before each event
    fKaonPassedCounter = 0;
    fEventAbsorbedEnergy = 0.0;
}

void EventAction::EndOfEventAction(const G4Event*) {
    // Update statistics for the entire run
    fRunAction->AddKaonPassed(fKaonPassedCounter);
    fRunAction->AddAbsorbedEnergy(fEventAbsorbedEnergy);
}

void EventAction::ProcessKaonStep(const G4Step* step) {
    G4Track* track = step->GetTrack();
    
    // Check if it's a kaon
    if (track->GetParticleDefinition()->GetParticleName() != "kaon+") 
        return;

    // Check energy absorption in lead layer
    G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()
                            ->GetVolume()->GetName();
    
    if (volumeName == "LeadLayer") {
        // Energy deposited in this step
        G4double energyDeposited = step->GetTotalEnergyDeposit();
        
        if (energyDeposited > 0) {
            fEventAbsorbedEnergy += energyDeposited;
            fKaonPassedCounter++;
        }
    }
}