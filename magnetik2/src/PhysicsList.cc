#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmParameters.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // Установка параметров электромагнитной физики
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetMinEnergy(10*eV);
  param->SetMaxEnergy(100*GeV);
  param->SetNumberOfBinsPerDecade(20);
  
  // Добавление физических конструкторов
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4EmExtraPhysics());
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4StoppingPhysics());
  RegisterPhysics(new G4IonPhysics());
}

PhysicsList::~PhysicsList()
{ }

void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
}

void PhysicsList::SetCuts()
{
  // Установка пространственных порогов обрезания для гамма, электронов и позитронов
  SetCutValue(1.0*mm, "gamma");
  SetCutValue(1.0*mm, "e-");
  SetCutValue(1.0*mm, "e+");
  SetCutValue(1.0*mm, "proton");
}
