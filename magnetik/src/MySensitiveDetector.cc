//Обрабатывает события в детекторе:
//Регистрирует энергетические потери частиц
//Записывает длину пути
//Сохраняет данные в ntuple и гистограммы

#include "MySensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"

MySensitiveDetector::MySensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name), fDetectorName(name), fEventID(-1) {
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
    const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
    if (event) {
        fEventID = event->GetEventID();
    } else {
        fEventID = -1;
    }
    
    G4cout << "Инициализация чувствительного детектора " << fDetectorName 
           << " для события #" << fEventID << G4endl;
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4Track* track = step->GetTrack();
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4double energyDeposit = step->GetTotalEnergyDeposit();
    G4double stepLength = step->GetStepLength();
    
    if (energyDeposit > 0) {
        auto analysisManager = G4AnalysisManager::Instance();
        
        analysisManager->FillNtupleSColumn(0, particleName);
        analysisManager->FillNtupleDColumn(1, track->GetKineticEnergy()/MeV);
        analysisManager->FillNtupleDColumn(2, energyDeposit/MeV);
        analysisManager->FillNtupleDColumn(3, track->GetMomentumDirection().theta()/deg);
        analysisManager->FillNtupleDColumn(4, stepLength/mm);
        analysisManager->AddNtupleRow();
        
        analysisManager->FillH1(0, energyDeposit/MeV);
        analysisManager->FillH1(1, (energyDeposit/stepLength)/(MeV/mm));
    }

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE) {
    G4cout << "Завершение обработки события #" << fEventID 
           << " в детекторе " << fDetectorName << G4endl;
}
