#include "MySensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"

MySensitiveDetector::MySensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name), fDetectorName(name), fEventID(-1), fH2_dE_dx_kaon_id(-1)
{
    G4cout << "Создан чувствительный детектор: " << name << G4endl;
    G4String fileName = name + "_hits.csv";
    OpenOutputFile(fileName);
    if (fOutputFile.is_open()) {
        fOutputFile << "Event,Particle,Energy(keV),PosX(cm),PosY(cm),PosZ(cm),Time(ns)" << G4endl;
    }
}

MySensitiveDetector::~MySensitiveDetector() {
    CloseOutputFile();
}

void MySensitiveDetector::OpenOutputFile(const G4String& fileName) {
    fOutputFile.open(fileName);
    if (fOutputFile.is_open()) {
        G4cout << "Открыт файл для записи данных: " << fileName << G4endl;
    } else {
        G4cerr << "Ошибка: Не удалось открыть файл " << fileName << G4endl;
    }
}

void MySensitiveDetector::CloseOutputFile() {
    if (fOutputFile.is_open()) {
        fOutputFile.close();
        G4cout << "Файл данных закрыт" << G4endl;
    }
}

void MySensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
    if (fH2_dE_dx_kaon_id < 0) {
        fH2_dE_dx_kaon_id = G4AnalysisManager::Instance()->GetH2Id("h2_dE_dx_kaon");
    }
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4Track* track = step->GetTrack();

    // Логируем каждый вызов ProcessHits
    G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    G4cout << "[ProcessHits] Volume: " << volumeName
           << ", Particle: " << track->GetDefinition()->GetParticleName()
           << ", ParentID: " << track->GetParentID() << G4endl;

    // Убрать все ограничения для теста!
    // if (track->GetParentID() != 0 || track->GetDefinition()->GetParticleName() != "kaon+") return false;

    G4double energyDeposit = step->GetTotalEnergyDeposit();
    G4double stepLength = step->GetStepLength();

    if (energyDeposit > 0 && stepLength > 0) {
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH2(fH2_dE_dx_kaon_id, stepLength/mm, energyDeposit/MeV);
        G4cout << "[ProcessHits] Гистограмма заполнена: dx = " << stepLength/mm
               << " мм, dE = " << energyDeposit/MeV << " МэВ" << G4endl;
    }

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE) {
    G4cout << "Завершение обработки события #" << fEventID 
           << " в детекторе " << fDetectorName << G4endl;
}