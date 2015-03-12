class CfgPatches
{
	class HEHU_MF
	{
		requiredVersion=1.0;
		requiredAddons[]=
		{
			"A3_Modules_F",
			"A3_Weapons_F_Ammoboxes",
			"cba_main"
		};

		units[] = {
			"HEHU_MF_Module",
			"HEHU_MF_ModuleSpawner",
			"HEHU_MF_ModuleTargetCounter",
			"HEHU_MF_ModuleAutomaticEndGame",
			"HEHU_MF_ArsenalBox"	
		};

		author[] = {"Magnus Bergmark"};
		authorUrl = "https://github.com/Mange";

		version = 0.0.1;
		versionStr = "0.0.1";
		versionAr[] = {0,0,1};
	};
};

/* Define module category */
class CfgFactionClasses
{
	class NO_CATEGORY;
	class HEHU_MF_Modules: NO_CATEGORY
	{
		displayName = "HEHU Mission Framework";
	};
};

/* Define modules */
class CfgVehicles {
	class Logic;
	class Module_F: Logic {
		class ArgumentsBaseUnits {
			class Units;
		};
		class ModuleDescription {
			class AnyBrain;
		};
	};

	class thingX;

	class HEHU_MF_Module: Module_F
	{
		scope = 0;
		author = "Magnus Bergmark";
		category = "HEHU_MF_Modules";
	};

	class HEHU_MF_ModuleSpawner: HEHU_MF_Module
	{
		displayName = "Spawner";
		author = "Magnus Bergmark";
		scope = 2;
		function = "HEHU_MF_fnc_moduleSpawner";
		// Execution priority, modules with lower number are executed first. 0 is used when the attribute is undefined
		functionPriority = 1;

		// 0 for server only execution, 1 for remote execution on all clients upon mission start, 2 for persistent execution
		isGlobal = 0;

		isTriggerActivated = 0;
		isDisposable = 1;

		class Arguments
		{
			// Module specific arguments
			class NumberOfEnemies
  			{
				displayName = "Number of enemies";
				description = "0 means choose from global config.";
				typeName = "NUMBER";
				defaultValue = "0";
			};
			class EnemyAlertness
  			{
				displayName = "Enemy alertness";
				typeName = "NUMBER";
				class values
				{
					class UseDefault		{name = "Use global config"; value = -1; default = 1;};
					class StandStill		{name = "Stand still"; value = 0;};
					class RespondThreats	{name = "Respond to threats"; value = 1;};
					class SomePatrol		{name = "Some will look for you"; value = 2;};
					class MostPatrol		{name = "Most will look for you"; value = 3;};
					class AllPatrol			{name = "All will look for you"; value = 4;};
				};
			};
		};
		
		class ModuleDescription: ModuleDescription
		{
			description = "Spawns units in random building positions.";
			sync[] = {"LocationArea_F"};

			class LocationArea_F
			{
				description[] = { // Multi-line descriptions are supported
					"CQB area, with independent unit combinations and positions."
				};
				position = 0; // Position is taken into effect
				direction = 0; // Direction is taken into effect
				optional = 0; // Synced entity is optional
				duplicate = 1; // Multiple entities of this type can be synced
				sync[] = {"EmptyDetector", "AnyAI"};
			};
	 
			class EmptyDetector
			{
				description[] = {
					"Trigger areas",
					"Buildings inside the areas will be candidates for unit placements."
				};
				position = 1; // Position is taken into effect
				direction = 1; // Direction is taken into effect
				optional = 0; // Synced entity is optional
				duplicate = 1; // Multiple entities of this type can be synced
			};
		};	
	};

	class HEHU_MF_ModuleTargetCounter: HEHU_MF_Module
	{
		displayName = "Target counter";
		author = "Magnus Bergmark";
		scope = 2;

		function = "HEHU_MF_fnc_moduleTargetCounter";
		// Execution priority, modules with lower number are executed first. 0 is used when the attribute is undefined
		functionPriority = 100;

		// 0 for server only execution, 1 for remote execution on all clients upon mission start, 2 for persistent execution
		isGlobal = 2;

		isTriggerActivated = 0;
		isDisposable = 1;

		class Arguments: ArgumentsBaseUnits
		{
			class Units: Units {};

			// Module specific arguments
			class Precision
  			{
				displayName = "Precision";
				description = "How accurate the counter will be.";
				typeName = "NUMBER";
				class values
				{
					class Exact		{name = "Exact (number of enemies)"; value = 1; default = 1};
					class Inexact	{name = "Inexact (few, many, etc.)"; value = 2;};
					class Unhelpful	{name = "Unhelpful (enemies left, no enemies left)"; value = 3;};
				};
			};
			class RefreshInterval
  			{
				displayName = "Refresh interval";
				typeName = "NUMBER";
				class values
				{
					class 100ms	{name = "100ms"; value = 0.1;};
					class 1s	{name = "1s"; value = 1;};
					class 2s	{name = "2s"; value = 2; default = 1};
					class 3s	{name = "3s"; value = 3;};
					class 4s	{name = "4s"; value = 4;};
					class 5s	{name = "5s"; value = 5;};
				};
			};
			class AutoHide
  			{
				displayName = "Stop when no enemies left";
				description = "Should target counter disappear when no enemies are left?";
				typeName = "BOOL";
				defaultValue = false;
			};
		};
		
		class ModuleDescription: ModuleDescription
		{
			description = "Shows number of enemies left to synchronized (or all) players.";
			sync[] = {"AnyPlayer"};
		};	
	};

	class HEHU_MF_ModuleAutomaticEndGame: HEHU_MF_Module
	{
		displayName = "Automatic end game";
		author = "Magnus Bergmark";
		scope = 2;

		function = "HEHU_MF_fnc_moduleAutomaticEndGame";
		// Execution priority, modules with lower number are executed first. 0 is used when the attribute is undefined
		functionPriority = 100;

		// 0 for server only execution, 1 for remote execution on all clients upon mission start, 2 for persistent execution
		isGlobal = 2;

		isTriggerActivated = 1;
		isDisposable = 1;
	
		class ModuleDescription: ModuleDescription
		{
			description = "Ends the mission when all enemies are dead (and optional triggers are activated).";
			sync[] = {};
		};	
	};

	class HEHU_MF_ArsenalBox: thingX
	{
		// Don't inherit from a real ammo box to remove the vanilla inventory from them.
		// You can only interact with this box as a Virtual Arsenal object.
		//
		// class thingX;
		//   class ReammoBox_F;
		//      class Box_NATO_Wps_F;
		//
		// TODO: Make it possible to sling load this box, or make a variant that is.

		vehicleclass = "Ammo";  // ReammoBox_F
		animated = 0;           // ReammoBox_F
		accuracy = 0.2;         // ReammoBox_F
		icon = "iconCrateWpns"; // Box_NATO_Wps_F
		mapsize = 1.81;         // Box_NATO_Wps_F
		model = "\A3\weapons_F\AmmoBoxes\WpnsBox_F"; // Box_NATO_Wps_F

		displayName = "HEHU Ammo box";
		author = "Magnus Bergmark";
		scope = 2;
		class Eventhandlers
		{
			init = "[""AmmoboxInit"",[(_this select 0), true]] spawn BIS_fnc_arsenal;";
		};
	};
};

class CfgFunctions {
	class HEHU_MF
	{
		tag = "HEHU_MF";
		class HEHU_MF_funcs
		{
			file = "hehu_mf\functions";

			class getSpawnerSettings{};
			class moduleSpawner{};
			class moduleTargetCounter{};
			class moduleAutomaticEndGame{};

			class getBuildingPositionsInTriggerAreas{};
			class findBuildings{};

			class inferEnemyUnits{};
			class getSpawns{};
			class getSpawnsInLogic{};
			class spawn{};
			class spawnUnit{};
			class setupPatrol{};

			class aliveEnemies{};
			class targetCounter{};
		};
	};
};