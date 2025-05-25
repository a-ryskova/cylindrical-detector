#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class RunAction; // forward declaration

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
    void AddEdep(G4double edep, G4ThreeVector position, 
        G4ThreeVector momentum, const G4ParticleDefinition* particle);

private:
    RunAction* fRunAction;
    G4double fEdep;
};


#endif