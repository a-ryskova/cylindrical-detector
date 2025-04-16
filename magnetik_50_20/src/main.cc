#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "QGSP_BIC_HP.hh"

int main(int argc, char** argv) {
    // Create simulation manager
    G4RunManager* runManager = new G4RunManager();

    // Set up detector construction
    DetectorConstruction* detector = new DetectorConstruction();
    runManager->SetUserInitialization(detector);

    // Set up physics list
    G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
    runManager->SetUserInitialization(physicsList);

    // Create UI executive
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Create visualization manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // Initialize run manager
    runManager->Initialize();

    // Set up user actions - без префиксов MySensitiveDetector::
    RunAction* runAction = new RunAction();
    runManager->SetUserAction(runAction);

    EventAction* eventAction = new EventAction(runAction);
    runManager->SetUserAction(eventAction);

    PrimaryGeneratorAction* primaryGenerator = new PrimaryGeneratorAction();
    runManager->SetUserAction(primaryGenerator);

    // Get UI manager pointer
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        // Interactive mode
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else {
        // Batch mode
        G4String command = "/control/execute ";
        G4String filename = argv[1];
        UImanager->ApplyCommand(command + filename);
        
        // Run simulation
        G4int numberOfEvents = 100;
        runManager->BeamOn(numberOfEvents);
    }

    // Cleanup
    delete visManager;
    delete runManager;

    return 0;
}