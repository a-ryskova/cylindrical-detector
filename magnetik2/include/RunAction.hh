#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

// Remove the forward declaration and include the proper header
#include "G4AnalysisManager.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    
    void AddEdep(G4double edep, G4double x); 

  private:
    G4AnalysisManager* fAnalysisManager;
};

#endif