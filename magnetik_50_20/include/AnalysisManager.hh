#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include "G4AnalysisManager.hh"
#include "globals.hh"

class AnalysisManager {
public:
    static AnalysisManager* GetInstance();
    
    void Book();
    void Finish();
    
private:
    AnalysisManager();
    ~AnalysisManager();
    
    static AnalysisManager* fInstance;
    G4AnalysisManager* fAnalysisManager;
};

#endif
