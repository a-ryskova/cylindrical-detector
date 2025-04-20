#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Construct the default run manager
  auto* runManager = G4RunManagerFactory::CreateRunManager();
  
  // Геометрия детектора
  runManager->SetUserInitialization(new DetectorConstruction());

  // Физические процессы
  runManager->SetUserInitialization(new PhysicsList());

  // Действия пользователя
  runManager->SetUserInitialization(new ActionInitialization());

  // Визуализация
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Интерфейс пользователя
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    // Batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else {
    // Interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
  
  return 0;
}
