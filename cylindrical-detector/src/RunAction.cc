#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

RunAction::RunAction() 
    : fNeutronsPassed(0), fAbsorbedEnergy(0.0) {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    // Сброс счетчиков перед началом прогона
    fNeutronsPassed = 0;
    fAbsorbedEnergy = 0.0;
}

void RunAction::EndOfRunAction(const G4Run*) {
    // Вывод статистики в конце прогона
    G4cout << "Run Statistics:" << G4endl;
    G4cout << "Total Neutrons Passed: " << fNeutronsPassed << G4endl;
    G4cout << "Total Absorbed Energy: " << fAbsorbedEnergy/MeV << " MeV" << G4endl;
        // Запись в файл
    std::ofstream outFile("neutron_statistics.txt");
    if (outFile.is_open()) {
        outFile << "Run Statistics:\n";
        outFile << "Total Neutrons Passed: " << fNeutronsPassed << "\n";
        outFile << "Total Absorbed Energy: " << fAbsorbedEnergy/MeV << " MeV\n";
        outFile.close();
    }
}
