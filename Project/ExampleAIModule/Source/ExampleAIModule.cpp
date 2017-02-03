#include "ExampleAIModule.h"
#include <iostream>


using namespace BWAPI;
using namespace Filter;
using namespace std;

vector <BWAPI::Unit> *workers;
vector <BWAPI::Unit> *resourceDepots;

void ExampleAIModule::onStart()
{
	workers = new vector<BWAPI::Unit>();
	BuildingConstruction::expansionCount = 0;

	for (auto &unit : Broodwar->self()->getUnits())
	{
		//Start performing actions
		// If the unit is a worker unit
		if (unit->getType().isWorker())
			workers->push_back(unit);

		//Store a pointer to refer to our resource dpots 
		//NOTE: This will only have 1 depot for the time being
		if (unit->getType().isResourceDepot())
			resourceDepots->push_back(unit);

	}
}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
	// Called once every game frame.
	// Display the game frame rate as text in the upper left area of the screen.
	Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS() );
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );

	// Return if the game is a replay or is paused.
	if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
	//return;
	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
		return;

	// Iterate through all the units that we own.
	for (auto &unit : Broodwar->self()->getUnits())
	{
		if (!UnitAction::checkUnitState(unit))
			continue;

		//Start performing actions with unit.
		// If the unit is a worker unit.
		if ( unit->getType().isWorker() )
		{
			ResourceGathering::workerGather(unit);
		}
		else if ( unit->getType().isResourceDepot() ) // A resource depot is a Command Center, Nexus, or Hatchery
		{
			ResourceGathering::buildWorker(unit);
		}
	}
}

