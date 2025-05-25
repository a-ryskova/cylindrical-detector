#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    //if (fEdep > 0) {
        //G4cout << "Energy deposit in this event: " 
          //     << G4BestUnit(fEdep, "Energy") << G4endl;
    //}
}

void EventAction::AddEdep(G4double edep, G4ThreeVector position,
    G4ThreeVector momentum, const G4ParticleDefinition* particle)
{
fEdep += edep;
if (fRunAction) {
fRunAction->AddEdep(edep, position, momentum, particle);
}
}