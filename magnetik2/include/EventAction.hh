#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.hh"  // Add this include instead of forward declaration

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep, G4double x) { 
      fEdep += edep; 
      fRunAction->AddEdep(edep, x);
    }

  private:
    RunAction* fRunAction;
    G4double   fEdep;
};

#endif