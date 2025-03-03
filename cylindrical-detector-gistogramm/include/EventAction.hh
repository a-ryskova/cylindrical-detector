#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Step.hh"
#include "RunAction.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    // Метод для обработки шага частицы
    void ProcessNeutronStep(const G4Step* step);

private:
    RunAction* fRunAction;
    G4int fNeutronPassedCounter;
    G4double fEventAbsorbedEnergy;
};

#endif
