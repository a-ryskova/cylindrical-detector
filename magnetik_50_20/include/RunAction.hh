#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "globals.hh"

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    // Методы для накопления статистики
    void AddKaonPassed(G4int count) { fKaonsPassed += count; }
    void AddAbsorbedEnergy(G4double energy) { fAbsorbedEnergy += energy; }

private:
    G4int fKaonsPassed;         // Счетчик прошедших каонов
    G4double fAbsorbedEnergy;   // Суммарная поглощенная энергия
};

#endif