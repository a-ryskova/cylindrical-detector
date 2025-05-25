#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Получаем выделенную энергию
    G4double edep = step->GetTotalEnergyDeposit();
    
    // Получаем позицию частицы
    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
    
    // Получаем импульс частицы
    G4ThreeVector momentum = step->GetPreStepPoint()->GetMomentum();
    
    // Получаем определение частицы
    const G4ParticleDefinition* particle = step->GetTrack()->GetParticleDefinition();
    
    // Передаем данные в EventAction
    fEventAction->AddEdep(edep, position, momentum, particle);
}