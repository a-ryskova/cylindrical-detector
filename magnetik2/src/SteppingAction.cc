#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Получаем объем, в котором произошел шаг
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()
                                ->GetVolume()->GetLogicalVolume();

  // Проверяем, что частица находится в трекере
  static const DetectorConstruction* detectorConstruction
    = static_cast<const DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
      
  G4LogicalVolume* trackerLog = detectorConstruction->GetTrackerLog();
  
  if (volume != trackerLog) return;
  
  // Получаем выделенную энергию на этом шаге
  G4double edep = step->GetTotalEnergyDeposit();
  
  // Получаем координату x для анализа dE/dx
  G4double x = step->GetPreStepPoint()->GetPosition().x();
  
  // Добавляем информацию в EventAction
  fEventAction->AddEdep(edep, x);
}
