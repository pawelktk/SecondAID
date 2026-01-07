#pragma once
#include <vector>
#include <string>

namespace ScriptAPI {

struct ParamDesc {
    const char* Name;
    const char* Type;
    const char* Desc;
    bool IsOptional;
};

struct FuncDesc {
    const char* Name;
    const char* ReturnType;
    const char* Desc;
    const char* Group;
    std::vector<ParamDesc> Params;
};

class Database {
public:
    static const std::vector<FuncDesc>& GetEngineFunctions() {
        static const std::vector<FuncDesc> funcs = {
            { "AICheckWorkingPlace", "Boolean", "Searches for a plan on the given subpath. The data values for the search is specified by pairs of DataName followed by its value", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to check", false },
                { "BuildingType", "Number", "Building type that must be under control of a dynasty", false },
                { "BuildingCount", "Number", "Count of the building needed to be under dynasty control", false },
            }},
            { "AIExecutePlan", "Boolean", "Searches for a plan on the given subpath. The data values for the search is specified by pairs of DataName followed by its value", "Global", {
                { "pObject", "Alias ( of type simobject)", "Object to search the plan for", false },
                { "Path", "String", "Path of the plan", false },
                { "DataName", "String", "Name of a data value", true },
                { "DataValue", "String or Alias ( of type rootref)", "Value of the data", true },
                { "Variable Argument List", "...", "...", false },
            }}, // [AUTO-FIXED PARAMS]
            { "AIFindGoodClass", "void", "This ai function searches for a good new class for a child", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "ActionIsEvidence", "Boolean", "Increases/Decreases the lock counter of an action", "Global", {
                { "pAction", "Alias ( of type actiondata)", "Action to modify", false },
            }},
            { "ActionIsStopped", "Boolean", "returns true, if an action is stopped", "Global", {
                { "pAction", "Alias ( of type actiondata)", "Action to check", false },
            }},
            { "ActionLock", "Boolean", "Increases/Decreases the lock counter of an action", "Global", {
                { "pAction", "Alias ( of type actiondata)", "Action to modify", false },
                { "iValue", "Number", "increment -1 - decrement", false },
            }},
            { "ActionSetData", "Boolean", "Sets a data value for an action, The action must be started bevor with CommitAction()", "Global", {
                { "pAction", "Alias ( of type actiondata)", "Action to modify", false },
                { "DataName", "String", "Name of the data to set", false },
                { "DataValue", "Number", "int Value to add", false },
            }},
            { "AddEvidence", "Number", "creates a new event, and stores evidence in pWitnesses memory", "Global", {
                { "pWitness", "Alias ( of type sim)", "the one who gets the evidence", false },
                { "pOffender", "Alias ( of type sim)", "the one who commited the crime", false },
                { "pVictim", "Alias ( of type guildobject)", "victim of the crime", false },
                { "TypeID", "Number", "see Database: cs_memoryevent", false },
            }},
            { "AddImpact", "Boolean", "Sets an impact from table gl_impact_type", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object that will hold the impact", false },
                { "ImpactID", "Number or String", "ID or name in gl_impact_type table", false },
                { "Value", "Number", "modifier value of the impact - modifier type depends on ImpactType", false },
                { "TimeLeft", "Number", "time argument for impact (in ingame hours)- impact type depends on ImpactType", false },
            }},
            { "AddImpactDependendImpact", "Boolean", "Sets an impact from table gl_impact_type", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object that will hold the impact", false },
                { "DependendImpactID", "Number", "the impact the newly set impact is depending on (either", false },
                { "ImpactID", "Number", "ID or name in gl_impact_type table", false },
                { "Value", "Number", "modifier value of the impact - modifier type depends on ImpactType", false },
            }},
            { "AddItems", "Number", "Add's items to pObjects inventory.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to add items into (cl_Building, cl_Sim, cl_Cart, ...)", false },
                { "Item", "Number or String", "database id or name of the item", false },
                { "ItemCount", "Number", "count of items to add", false },
                { "InventoryType", "Any", "of the inventory to add the items to. If no inventorytype is specified the standardt inventory will be used.", false },
            }}, // [AUTO-FIXED PARAMS]
            { "AddObjectDependendImpact", "Boolean", "Sets an impact from table gl_impact_type", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object that will hold the impact", false },
                { "iObjectDependency", "Number", "the dependend object", false },
                { "ImpactID", "Number", "ID or name in gl_impact_type table", false },
                { "Value", "Number", "modifier value of the impact - modifier type depends on ImpactType", false },
            }},
            { "AliasExists", "Boolean", "Indicates it the given alias exists within the measure.", "Global", {
                { "AliasName", "Alias ( of type corenode)", "The Name of the potentially existing alias.", false },
            }},
            { "AlignTo", "Boolean", "Aligns the object to dest.", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "the object to align", false },
                { "pDest", "Alias ( of type guildobject)", "the object to align to", false },
            }},
            { "AlignToNearest", "Boolean", "Aligns the sim to the nearest sim", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to align", false },
            }},
            { "AllowAllMeasures", "Boolean", "Allow all measures to be executed for the given AliasObject. This sets the object back to the default state.", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "Object to modify", false },
            }},
            { "AllowMeasure", "Boolean", "Allow a Measure by ID for the given AliasObject. The mode specifys if the object allows the starting of a measure with it as owner (EN_ACTIVE), to use the object as measure target (EN_PASSIVE) or both (EN_BOTH).", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "Object to modify", false },
                { "Measure", "Number or String", "Measure to set exclusive (int: database id string: measure name)", false },
                { "Mode", "Number", "EN_PASSIVE/EN_ACTIVE/EN_BOTH", false },
            }},
            { "Attach3DSound", "Boolean", "Attach a ne sound to a guild object.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the 3d-object to attach the sound to", false },
                { "SampleName", "String", "filename of the sample", false },
                { "Volume", "Number", "Volume (0..1)", true },
            }},
            { "BankGetOffer", "Boolean", "Get's a credit offer from a bank", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to take the credit from (bank)", false },
                { "pSim", "Alias ( of type sim)", "sim who takes the credit", false },
                { "Number", "Number", "Number of credit offer", false },
                { "o_pCredit", "Alias ( of type credit)", "the credit on succes", false },
            }},
            { "BankSetCreditOffer", "Boolean", "Creates a credit offer in a bank", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to create the credit (bank)", false },
                { "pSim", "Alias ( of type sim)", "Number of credit offer", false },
                { "Number", "Number", "sum", false },
                { "Sum", "Number", "Interest", false },
                { "Interest", "Number", "Runtime", false },
            }},
            { "BaseFE", "Nil", "sets a basic facial expression to use when an emote stops. No channel required, this goes to channel cl_CharacterModel::EN_EXPRESSION_CHN_MOOD always.", "Global", {
                { "pTarget", "Alias ( of type sim)", "Target Sim Alias", false },
                { "AnimName", "String", "name of the expression", false },
                { "Weight", "Number", "weight of facial expression (1.0f is full weight)", false },
                { "Time", "Number", "runtime", false },
            }},
            { "BattleFlee", "Boolean", "Flee from a battle", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the fighting object", false },
                { "a_pFleeObj", "Alias ( of type simobject)", "the object to flee to", false },
                { "a_fRadius", "Number", "radius to run (if no fleeobject is specified)", false },
            }},
            { "BattleGetNextEnemy", "Boolean", "Gets the next target by evalulation of threat", "Global", {
                { "pOwner", "Alias ( of type guildobject)", "the fighting object", false },
            }},
            { "BattleGetWeaponName", "String", "Returns the currently selected weapon Weapon", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the fighting object", false },
            }},
            { "BattleIsFighting", "Boolean", "Checks if object is involved in a battle", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the fighting object", false },
            }},
            { "BattleJoin", "Number", "Joins a Battle (and creates it before if needed)", "Global", {
                { "pOwner", "Alias ( of type guildobject)", "the initiator of the battle", false },
                { "pTarget", "Alias ( of type guildobject)", "the target to attack", false },
                { "Defense", "Boolean", "true: defensive attack (eg. protect building), means a guard is not aggressor", false },
                { "a_bSilent", "Boolean", "true: the AttackAction is NOT Created on the Aggressor, which means so ai reaction of ppl (gape, assist, flee) whatsoever", true },
            }},
            { "BattleLeave", "Boolean", "Leaves A Battle", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the fighting object", false },
            }},
            { "BattleWeaponPresent", "Boolean", "Draws a Weapon", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the fighting object", false },
            }},
            { "BattleWeaponStore", "Boolean", "Sheathes a Weapon", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the fighting object", false },
            }},
            { "BeginUseLocator", "Boolean", "Blocks a locator and moves the sim to it. The sim is switching to Stance while using the locator. The function waits until", "Global", {
                { "pSim", "Alias ( of type movingsimobject)", "the sim", false },
                { "Position", "Alias ( of type position)", "Name of the locator", false },
                { "Stance", "Number", "Stance to switch into (GL_STANCE_STAND, GL_STANCE_SIT, GL_STANCE_LAY)", false },
                { "MoveTo", "Number", "true: move to the locator", false },
            }},
            { "BeginUseLocatorNoWait", "Number", "Blocks a locator and moves the sim to it. The sim is switching to Stance while using the locator", "Global", {
                { "pSim", "Alias ( of type movingsimobject)", "the sim", false },
                { "Position", "Alias ( of type position)", "Name of the locator", false },
                { "Stance", "Number", "Stance to switch into (GL_STANCE_STAND, GL_STANCE_SIT, GL_STANCE_LAY)", false },
                { "MoveTo", "Number", "true: move to the locator", false },
            }},
            { "BindQuest", "Boolean", "binds the quest with the given questname", "Global", {
                { "pQuestname", "String", "quest to bind", false },
                { "pSource", "Alias ( of type guildobject)", "Questsource", true },
            }},
            { "BlockChar", "Boolean", "Blocks a char exclusiv for the current measure", "Global", {
                { "pTarget", "Alias ( of type corenode)", "target to block", false },
            }},
            { "BlockLocator", "Boolean", "Blocks a free locator for use for pObject only.", "Global", {
                { "pObject", "Alias ( of type simobject)", "the SimObject who blocks this locator", false },
                { "Position", "Alias ( of type position)", "the position to block (retreived by GetFreeLocatorByName or GetLocatorByName)", false },
            }},
            { "BuildingBuy", "Boolean", "Buys a building for a sim.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to buy", false },
                { "pBuyer", "Alias ( of type sim)", "the dynasty sim, who buys this building", false },
                { "BuyMode", "Any", "BM_NORMAL (Pay Money for buying, seller gains the money), BM_CAPTURE (Without any payment), BM_STARTUP (No money for buyer, but money for the seller)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "BuildingCanBeEntered", "Boolean", "checks if a building can be entered by a movingsimobject", "Global", {
                { "pBuilding", "Alias ( of type building)", "the building to check", false },
                { "pObject", "Alias ( of type movingsimobject)", "the MovingSimObject that will be checked", false },
            }},
            { "BuildingCanLevelUp", "Boolean", "checks wheter the given building can be leveled up to the given prototype", "Global", {
                { "pTarget", "Alias ( of type building)", "Target Building Alias", false },
                { "NewProto", "Number", "Proto", false },
            }},
            { "BuildingCanProduce", "Boolean", "Level the building up.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to level up", false },
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "BuildingFindSimByName", "Boolean", "Finds a sim with a certain name in a certain building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "in which to search", false },
                { "FirstName", "Any", "of the sim to search", false },
                { "LastName", "Any", "of the sim to search", false },
                { "NewAlias", "Any", "given to the found sim", false },
            }}, // [AUTO-FIXED PARAMS]
            { "BuildingFindSimByProperty", "Boolean", "Finds a sim with a certain property in a certain building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "in which to search", false },
                { "PropertyName", "Any", "name of the property", false },
                { "PropertyValue", "Number", "int value of the property", false },
                { "NewAlias", "Any", "given to the found sim", false },
            }}, // [AUTO-FIXED PARAMS]
            { "BuildingGetAISetting", "void", "gets a setting from the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to level up", false },
                { "Setting", "String", "settting value to query (\"Enable\", \"BuySell\", \"BuySell_Selection\", \"BuySell_Radius\", \"BuySell_Stock\", \"BuySell_PriceLevel\", \"BuySell_SellStock\", \"BuySell_Carts\", \"Workers\", \"Workers_Quality\", \"Workers_Favor\", \"Budget\", \"Budget_Repair\", \"Budget_Upgrades\")", false },
            }},
            { "BuildingGetBaseHP", "Nil", "gets the max hp of a building", "Global", {
                { "iProto", "Number", "building proto", false },
            }},
            { "BuildingGetBasePrice", "Nil", "gets the base price (for buying) from the building. The price is defined in the database", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
            }},
            { "BuildingGetCart", "Number", "Get a cart from a building by it's number", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
                { "number", "Number", "number of cart(0..BuildingGetCartCount(...) )", false },
                { "o_Cart", "Alias ( of type cart)", "the cart on success", false },
            }},
            { "BuildingGetCartCount", "Number", "Get the amount of the carts of a building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
            }},
            { "BuildingGetCharacterClass", "Number", "returns the character class (GL_CLASS_PATRON, GL_CLASS_ARTISAN, GL_CLASS_SCHOLAR, GL_CLASS_CHISELER, GL_CLASS_NPC) needed for the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building", false },
            }},
            { "BuildingGetCity", "Boolean", "Gets the settlement the building is belonging to.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "o_Settlement", "Alias ( of type settlement)", "settlement the building is belonging to", false },
            }},
            { "BuildingGetClass", "Number", "returns the class from the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building", false },
            }},
            { "BuildingGetCutscene", "Boolean", "returns the cutscene of a building. Must have been set before by BuildingLockForCutscene().", "Global", {
                { "pBuilding", "Alias ( of type building)", "", false },
                { "CutsceneAlias", "Any", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "BuildingGetFixedCosts", "Nil", "returns the estimated fixed costs of a building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
            }},
            { "BuildingGetFlag", "Number", "Get's the main flag of the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "o_AliasName", "Alias ( of type i_object3d)", "the flags alias", false },
                { "a_iFlagNumber", "Number", "if there are more than one flag, one might want to have the others too", false },
            }},
            { "BuildingGetForSale", "Boolean", "Sets the \"ForSale\"-Flag for the building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building", false },
            }},
            { "BuildingGetFreeBudget", "Nil", "returns the free budget of a building (remaining budget - fixed costs)", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
            }},
            { "BuildingGetInsideSimList", "Nil", "returns a list of all sims inside a building", "Global", {
                { "pBuilding", "Alias ( of type building)", "", false },
                { "ListAlias", "Any", "alias of a list of sims", false },
            }}, // [AUTO-FIXED PARAMS]
            { "BuildingGetLevel", "Number", "returns the level from the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building", false },
            }},
            { "BuildingGetMaxWorkerCount", "Number", "Get's the max count of workers a building can have", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
            }},
            { "BuildingGetNPC", "Boolean", "Gets the building npc by its database npc id", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "Number", "Number", "the building npc on success", false },
            }},
            { "BuildingGetOwner", "Boolean", "Gets the owner of the building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "o_Owner", "Alias ( of type sim)", "owner of the building on success", false },
            }},
            { "BuildingGetPriceProto", "Nil", "gets the base price of a building", "Global", {
                { "Proto", "Number", "proto of the building", false },
            }},
            { "BuildingGetProducerCount", "Number", "get the proto from the given building", "Global", {
                { "pBuilding", "Alias ( of type building)", "the building", false },
                { "ItemId", "Number", "itemid to check", false },
            }},
            { "BuildingGetProfession", "Number", "Get's the profession for the workers of the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
            }},
            { "BuildingGetProto", "Number", "get the proto from the given building", "Global", {
                { "pBuilding", "Alias ( of type building)", "the building", false },
            }},
            { "BuildingGetReligion", "Number", "returns the religion from the building (if the building is a church)", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building", false },
            }},
            { "BuildingGetRepairPrice", "Nil", "gets the repair price of a building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
            }},
            { "BuildingGetSim", "Boolean", "Get an assigned sim from this building. The sims can be outside the building (but working on it like mine) or in different rooms.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "number", "Number", "number of sim ( 0 ... BuildingGetSimCount(...) )", false },
                { "o_Sim", "Alias ( of type sim)", "sim on success", false },
            }},
            { "BuildingGetSimCount", "Number", "Get the amount of all sims assigned to this building. The sims can be outside the building (but working on it like mine) or in different rooms.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
            }},
            { "BuildingGetType", "Number", "returns the type from the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building", false },
            }},
            { "BuildingGetValue", "Nil", "gets the current value of the building. This value includes building/carts/wares/upgrades", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to query", false },
            }},
            { "BuildingGetWorker", "Boolean", "Get a worker from the building by number", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "number", "Number", "number of worker ( 0 ... BuildingGetWorkerCount(...) -1 )", false },
                { "o_Worker", "Alias ( of type sim)", "worker on success", false },
            }},
            { "BuildingGetWorkerCount", "Number", "Get the amount of workers from the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
            }},
            { "BuildingGetWorkingEnd", "Number", "get the working end time from a building", "Global", {
                { "pBuilding", "Alias ( of type building)", "the building", false },
            }},
            { "BuildingGetWorkingStart", "Number", "get the working start time from a building", "Global", {
                { "pBuilding", "Alias ( of type building)", "the building", false },
            }},
            { "BuildingHasUpgrade", "Boolean", "Checks if a building has an upgrade", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "Upgrade", "Number or String", "Name or id of the upgrade (from the database)", false },
            }},
            { "BuildingInternalLevelUp", "Boolean", "Levels a building up to the given proto", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to level up", false },
                { "Proto", "Number", "Proto to level up to", false },
            }},
            { "BuildingLevelMeUp", "Boolean", "Level the building up.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to level up", false },
                { "iProto", "Number", "prototype to level up to. You can use -1 for the proto to level to the default next level", false },
            }},
            { "BuildingLockForCutscene", "Nil", "Locks a building and all sims inside for a cutscene, pCutscene=0 unlocks the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "the Cutscene that takes control of the building", false },
                { "pCutscene", "Alias ( of type cutscene)", "", false },
            }},
            { "BuildingRallyWorkers", "Boolean", "BuildingRallyWorkers to the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to evacuate", false },
            }},
            { "BuildingSetFlagColor", "Number", "Sets's the main flags color of the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "pDynasty", "Alias ( of type dynasty)", "if this Alias is not set, the color is set to default/neutral white", false },
                { "a_iFlagNumber", "Number", "if there are more than one flag, one might want to have the others too", false },
            }},
            { "BuildingSetForSale", "Boolean", "Sets the \"ForSale\"-Flag for the building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building", false },
                { "ForSale", "Boolean", "true: set the building for sale / false: remove the for sale sign", false },
            }},
            { "BuildingSetOwner", "Boolean", "Sets the owner of the building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "building", false },
                { "pOwner", "Alias ( of type sim)", "new owner of the building", false },
            }},
            { "CalcDistance", "Number", "Returns the distance between the objects.", "Global", {
                { "FirstObject", "Alias ( of type simobject)", "The first object.", false },
                { "SecondObject", "Alias ( of type simobject)", "The second object.", false },
            }},
            { "CalculateCourtingDifficulty", "Boolean", "Calculates the difficulty of the courting", "Global", {
                { "CourterAlias", "Alias ( of type sim)", "The courter", false },
                { "CourtedAlias", "Alias ( of type sim)", "The courted", false },
            }},
            { "CameraBlend", "Nil", "tells the camera controller to make a smooth transition from the current position to the next one specified.", "Global", {
                { "TransitionTime", "Number", "time to perform the blending", false },
                { "TransitionType", "Number", "optional type of blending: 0=linear (default), 1 = sigmoid (smooth)", true },
            }},
            { "CameraCheckLineOfSight", "void", "checks whether the camera position is inside a building", "Global", {
                { "pObject", "Alias ( of type guildobject)", "pObject that should be visible", false },
            }},
            { "CameraFollow", "Boolean", "Tells the camera to follow an object (works only in terrain)", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to follow", false },
            }},
            { "CameraIndoorGetBuilding", "void", "gets the building the camera is currently in", "Global", {
                { "o_pBuilding", "Alias ( of type building)", "alias of the building on success", false },
            }},
            { "CameraIndoorGetRoom", "void", "gets the room the camera is currently in", "Global", {
                { "o_pRoom", "Alias ( of type room)", "alias of the room on success", false },
            }},
            { "CameraIndoorSetBuilding", "Boolean", "Switches to indoor view mode, last camera position for given building is used or otherwise if not available the default position.", "Global", {
                { "pBuilding", "Alias ( of type building)", "Alias of the building", false },
            }},
            { "CameraIndoorSetRoom", "Boolean", "Switches to indoor view mode, last camera position for given building is used or otherwise if not available the default position.", "Global", {
                { "pRoom", "Alias ( of type room)", "Alias of the building", false },
            }},
            { "CameraIsIndoor", "Boolean", "returns true if the camera is inside a building, regardless of being locked or not", "Global", {}}, // No params
            { "CameraIsTerrain", "Boolean", "returns true if the camera has been locked, regardless of being inside a building or in terrain", "Global", {}}, // No params
            { "CameraLock", "Nil", "Lock variant #1: Lock the camera to given position", "Global", {
                { "pLocator", "Alias ( of type position)", "a locator that defines a camera position", false },
            }},
            { "CameraLockLookAt", "Nil", "lets the cutscene camera look at a given object or position, only the view angle is affected", "Global", {
                { "pPosition", "Alias ( of type position)", "a locator or guildobject", false },
            }},
            { "CameraTerrainSet", "Boolean", "Switches to terrain view mode, last camera position in terrain is used.", "Global", {}}, // No params
            { "CameraTerrainSetPos", "Nil", "set terrain camera to focus on any object that has position (locator,sim,building,etc.) at given distance and angle", "Global", {
                { "*pPosition", "Any", "any object of Sim,Locator/Position,Building", false },
                { "Distance", "Number", "(optional) distance/zoom of the camera", true },
                { "Angle", "Number", "(optional) Y angle (view direction) of the camera", true },
            }}, // [AUTO-FIXED PARAMS]
            { "CameraUnlock", "Nil", "Removes the camera lock and restores the last camera mode and position before the camera became locked.", "Global", {}}, // No params
            { "CameraenableUserUnlock", "Nil", "enables or disables the behavior that a right mouse button click performs a CameraUnlock()", "Global", {
                { "bEnable", "Boolean", "true ti enable right mouse button unlock", false },
            }},
            { "CampaignExit", "Nil", "exit's the current campaign", "Global", {
                { "Won", "Boolean", "true: campaign is won and the next chapter will be enabled", false },
            }},
            { "CanAddItems", "Boolean", "checks, if an item can be added to the inventory", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to add items into (cl_Building, cl_Sim, cl_Cart, ...)", false },
                { "Item", "Number or String", "database id or name of the item", false },
                { "ItemCount", "Number", "count of items to add", false },
                { "InventoryType", "Any", "of the inventory to add the items to. If no inventorytype is specified the standardt inventory will be used.", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CanBeControlled", "Boolean", "checks if the object can currently be controlled by a dynasty (Filter Object.CanBeControlled() )", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to check (sim/cart/building/...)", false },
                { "pDyn", "Alias ( of type dynasty)", "Dynasty who wants to control the sim", false },
            }},
            { "CanBeInterruptetBy", "Boolean", "Checks, if a measure can be started on the object pThis", "Global", {
                { "pThis", "Alias ( of type measuresimobject)", "the object that is the this object of the new measure", false },
                { "pDest", "Alias ( of type measuresimobject)", "the object that is the target object of the new measure", false },
                { "Measure", "Number or String", "The measure to start. int = measure database ID / string = name of measure", false },
            }},
            { "CanBuildWorkshop", "void", "Checks if the given dynasty is allowed to build one more workshop", "Global", {
                { "Dynasty", "Alias ( of type dynasty)", "The dynasty", false },
            }},
            { "CarryObject", "Boolean", "attaches a 3d object to a sim at its hand.", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim to carry the object", false },
                { "Filename", "String", "the .nif-file to create the new 3d object", false },
                { "RightHand", "Boolean", "true if the object is carried in the right hand", false },
            }},
            { "CartGetOperator", "Boolean", "Get the operator from a cart.", "Global", {
                { "pCart", "Alias ( of type cart)", "cart to query", false },
                { "o_Operator", "Alias ( of type sim)", "the operator on success", false },
            }},
            { "CartGetType", "Number", "Gets the type of the cart.", "Global", {
                { "pCart", "Alias ( of type cart)", "cart to query", false },
            }},
            { "ChangeAlias", "Boolean", "Changes the name of an alias.", "Global", {
                { "OldName", "String", "The old alias name.", false },
                { "NewName", "String", "The new alias name.", false },
            }},
            { "ChangeAnimation", "Number", "Changes the currently played animation to a new one", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
                { "AnimName", "String", "Name of the new animation", false },
                { "Layer", "Number", "layer for the animation (0 = default)", true },
            }},
            { "ChangeAppearance", "Nil", "displays the changeappearance dialog for the given sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to display the change appearance dialog for", false },
            }},
            { "ChangeResidence", "Boolean", "Sets a new residence for a dynasty sim", "Global", {
                { "pSim", "Alias ( of type sim)", "dynasty sim", false },
                { "pBuilding", "Alias ( of type building)", "building to assign the sim to", false },
            }},
            { "CheckGameTimerEnd", "Boolean", "checks if the gametimer (if set) has ended", "Global", {}}, // No params
            { "CheckPlayerBankrupt", "Nil", "Checks if the player dynasty is indebted", "Global", {
                { "Dynasty", "Alias ( of type dynasty)", "Dynasty to check", false },
            }},
            { "CheckPlayerExtinct", "Nil", "Checks if the player dynasty extincted", "Global", {
                { "Dynasty", "Alias ( of type dynasty)", "Dynasty to check", false },
            }},
            { "CheckSkill", "Boolean", "makes a skillcheck against the level of the given skill and the given skillvalue", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
                { "Skill", "Number", "skill", false },
                { "Modifier", "Number", "skillvalue to compare with or selfdefined modifier", false },
            }},
            { "CityAddPenalty", "Nil", "the city sets a penalty for a given sim", "Global", {
                { "pSettlment", "Alias ( of type settlement)", "the city that sets the penalty (important for the the execution of the penalty)", false },
                { "pSim", "Alias ( of type sim)", "the sim to punish", false },
                { "Penalty", "Number", "typeId of penalty", false },
                { "Value", "Number", "duration of imprisonment or pillory / height of fee, default=0, must not exist", false },
            }},
            { "CityAssignTrialRoles", "Boolean", "assign", "Global", {
                { "pSettlement", "Alias ( of type settlement)", "alias of the settlement from which to take the", false },
            }},
            { "CityBuildNewBuilding", "Boolean", "Builds a new building. The building is defined by it's proto", "Global", {
                { "pCity", "Alias ( of type settlement)", "city to build the building in", false },
                { "Proto", "Number", "proto of the new building", false },
                { "pOwner", "Alias ( of type sim)", "owner for the new building", false },
                { "ret_NewBuilding", "Alias ( of type building)", "the new building on success", true },
                { "Position", "Alias ( of type corenode)", "position hint (if specified, the ai trys to build the building new the position)", true },
            }},
            { "CityEndOfficeElection", "Boolean", "ends a selection, and makes the winner the new office holder", "Global", {
                { "pCity", "Alias ( of type settlement)", "", false },
                { "pOffice", "Alias ( of type office)", "", false },
                { "pWinner", "Alias ( of type sim)", "", false },
            }},
            { "CityFindCrowdedPlace", "Number", "finds a random crowded place in the city", "Global", {
                { "pCity", "Alias ( of type settlement)", "settlement", false },
                { "pSim", "Alias ( of type sim)", "the position (if return > 0)", false },
            }},
            { "CityFindSquad", "void", "Get's a squad for a dynasty, with a free place", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to look at it's servants for a squad", false },
                { "pSim", "Alias ( of type sim)", "object to search for a free squad place", true },
                { "ret_Squad", "Alias ( of type aisquad)", "the squad on success", false },
            }},
            { "CityGetBuildingByName", "Boolean", "Get's a building from a city by it's name and returns it in ReturnAlias", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "Name", "String", "Name of the building", false },
                { "o_ReturnAlias", "String", "The alias for storing the building", false },
            }},
            { "CityGetBuildingCount", "Boolean", "Get's the count of building from a city", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "iClass", "Number", "Class of the building or -1 if no class filter", false },
                { "iType", "Number", "Type of the building or -1 of no type filter", false },
                { "iLevel", "Number", "Level of the building or -1 of no type filter", false },
                { "iSubLevel", "Number", "SubLevel of the building or -1 of no type filter", false },
                { "IsBuyable", "Any", "Building is FILTER_IS_BUYABLE / FILTER_ISNOT_BUYABLE / FILTER_IGNORE / FILTER_NO_DYNASTY / FILTER_HAS_DYNASTY", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetBuildingCountForCharacter", "Boolean", "Get's the building count from a city for a character class", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "Class", "Number", "CharacterClass of the new owner", false },
                { "Religion", "Number", "Building is FILTER_IS_BUYABLE / FILTER_ISNOT_BUYABLE / FILTER_IGNORE / FILTER_NO_DYNASTY / FILTER_HAS_DYNASTY", false },
            }},
            { "CityGetBuildingForCharacter", "Boolean", "Get's a random building from a city for a character class and returns it in ReturnAlias", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "pSim", "Alias ( of type sim)", "sim to get the building for (or direct the class)", false },
                { "buyable", "Any", "Building is FILTER_IS_BUYABLE / FILTER_ISNOT_BUYABLE / FILTER_IGNORE / FILTER_NO_DYNASTY / FILTER_HAS_DYNASTY", false },
                { "ret_ReturnAlias", "String", "The alias for storing the building", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetBuildings", "void", "Get's all building from a city and returns it the alias", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "iClass", "Number", "Class of the building or -1 if no class filter", false },
                { "iType", "Number", "Type of the building or -1 of no type filter", false },
                { "iLevel", "Number", "Level of the building or -1 of no type filter", false },
                { "iSubLevel", "Number", "SubLevel of the building or -1 of no type filter", false },
                { "buyable", "Any", "Building is: FILTER_IS_BUYABLE / FILTER_ISNOT_BUYABLE / FILTER_IGNORE / FILTER_NO_DYNASTY / FILTER_HAS_DYNASTY", false },
                { "ret_ReturnAlias", "String", "eg. \"Building0\", \"Building1\", ...", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetCitizenCount", "Number", "Get the amount of citizen of a city", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to query", false },
            }},
            { "CityGetCrimeRate", "Number", "Get the crime rate (0..100) of the settlement", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to query", false },
            }},
            { "CityGetEventText", "void", "return the text assotiated with an event from the event schedule of a city", "Global", {
                { "pCity", "Alias ( of type city)", "city that has the event schedule", false },
                { "index", "Number", "index of the event", false },
            }},
            { "CityGetHighestOfficeLevel", "Number", "returns the hieghest office level of the given city", "Global", {
                { "pCity", "Alias ( of type settlement)", "city alias", false },
            }},
            { "CityGetLevel", "Number", "Get the level of the settlement", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to query", false },
            }},
            { "CityGetMaxWorkerhutLevel", "Number", "Gets the max level of workerhuts a city can build", "Global", {
                { "pCity", "Alias ( of type settlement)", "city to query", false },
            }},
            { "CityGetNearestBuilding", "Boolean", "Get's a random building from a city and returns it in ReturnAlias", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "pObject", "Alias ( of type sim)", "sim to get the nearest building for", false },
                { "iClass", "Number", "Class of the building or -1 if no class filter", false },
                { "iType", "Number", "Type of the building or -1 of no type filter", false },
                { "iLevel", "Number", "Level of the building or -1 of no type filter", false },
                { "iSubLevel", "Number", "SubLevel of the building or -1 of no type filter", false },
                { "buyable", "Any", "Building is: FILTER_IS_BUYABLE / FILTER_ISNOT_BUYABLE / FILTER_IGNORE / FILTER_NO_DYNASTY / FILTER_HAS_DYNASTY", false },
                { "ret_ReturnAlias", "String", "The alias for storing the building", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetNumEvent", "Number", "Gets the number of events listed in the event schedule of a city", "Global", {
                { "pCity", "Alias ( of type city)", "alias of the settlement", false },
            }},
            { "CityGetOffice", "Boolean", "pushs the office identified with OfficeLevel und OfficeID in the given city into the given Target Alias", "Global", {
                { "pCity", "Alias ( of type settlement)", "City Alias", false },
                { "Officelevel", "Number", "level of the wanted office", false },
                { "OfficeID", "Number", "id of the wanted office (in the office level?)", false },
                { "o_TargetOfficeAlias", "Alias ( of type office)", "office alias", false },
            }},
            { "CityGetOfficeCountAtLevel", "Number", "returns the number of offices in the given office level of the given city", "Global", {
                { "pCity", "Alias ( of type settlement)", "city alias", false },
                { "Level", "Number", "office level", false },
            }},
            { "CityGetPenalty", "Boolean", "Get's a penalty for a sim in the city", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to look for the penalty", false },
                { "pSim", "Alias ( of type sim)", "Offender", false },
                { "PenaltyType", "Number", "type of the penalty (PENALTY_MONEY, PENALTY_PRISON, PENALTY_DEATH, PENALTY_TITLE, PENALTY_UNKNOWN(all) )", false },
                { "Fugitive", "Boolean", "if true, only penalties are returned, the offender is fugitive", false },
                { "ret_pPenalty", "Alias ( of type penalty)", "Penalty on success", false },
            }},
            { "CityGetRandomBuilding", "Boolean", "Get's a random building from a city and returns it in ReturnAlias", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to search the building", false },
                { "iClass", "Number", "Class of the building or -1 if no class filter", false },
                { "iType", "Number", "Type of the building or -1 of no type filter", false },
                { "iLevel", "Number", "Level of the building or -1 of no type filter", false },
                { "iSubLevel", "Number", "SubLevel of the building or -1 of no type filter", false },
                { "buyable", "Any", "Building is: FILTER_IS_BUYABLE / FILTER_ISNOT_BUYABLE / FILTER_IGNORE / FILTER_NO_DYNASTY / FILTER_HAS_DYNASTY", false },
                { "ret_ReturnAlias", "String", "The alias for storing the building", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetSeller", "void", "returns a seller for the item, who has min Count items of the type", "Global", {
                { "pCity", "Alias ( of type settlement)", "city to query", false },
                { "pBuyer", "Alias ( of type guildobject)", "buyer of the market", false },
                { "Item", "String", "Name or database-id of the item", false },
                { "Count", "Any", "amount of items to buy", false },
                { "ret_pSeller", "Alias ( of type guildobject)", "the market on success", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetServant", "Boolean", "Get a town servant of a city by its number [and it's profession]", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to query", false },
                { "Number", "Number", "number of the guard (0 <= number < CityGetGuardCount() )", false },
                { "Profession", "Any", "Profession from the servant ( GL_PROFESSION_CITYGUARD / GL_PROFESSION_PRISONGUARD / GL_PROFESSION_INSPECTOR / GL_PROFESSION_MONITOR)", true },
                { "ret_Servant", "Alias ( of type sim)", "the servant", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGetServantCount", "Number", "Get the amount of servants of a city", "Global", {
                { "pCity", "Alias ( of type settlement)", "City to query", false },
                { "Profession", "Any", "Profession from the servant (GL_PROFESSION_CITYGUARD / GL_PROFESSION_PRISONGUARD / GL_PROFESSION_INSPECTOR / GL_PROFESSION_MONITOR)", true },
            }}, // [AUTO-FIXED PARAMS]
            { "CityGuardScan", "Boolean", "Scans the surrounding of the guard for a wanted one.", "Global", {
                { "pGuard", "Alias ( of type sim)", "Guard who looks around", false },
                { "ret_Penalty", "Alias ( of type penalty)", "returns a wanted on success", false },
            }},
            { "CityLevel2Label", "String", "translates a city level to its label", "Global", {
                { "CityLevel", "Number", "City level", false },
            }},
            { "CityModifyCrimeRate", "Number", "Modifies the crime rate (0..100) of the settlement", "Global", {
                { "pCity", "Alias ( of type settlement)", "City", false },
                { "Delta", "Any", "= decrement CrimeRate down to 0)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CityRemoveFromOffice", "Nil", "removes the given sim from the office", "Global", {
                { "City", "Alias ( of type settlement)", "", false },
                { "pSim", "Alias ( of type sim)", "", false },
            }},
            { "CityScheduleCutsceneEvent", "Number", "create a new event and assign a time slot", "Global", {
                { "pSettlement", "Alias ( of type settlement)", "alias of the settlement that does the scheduling", false },
                { "EventAliasName", "Any", "the alias that is given to the newly created event", false },
                { "pCutScene", "Alias ( of type cutscene)", "alias of the cutscene that will be called", false },
                { "Function", "Any", "name of the function of the cutscene that will be called", false },
                { "HourOfDay", "Number", "the hour of day when the event should happen", false },
                { "MinTimeInFuture", "Number", "the minimum time that the event must be in the future.", false },
                { "*pTextLabel", "Any", "label of description text that will appear in the settlement schedule", false },
                { "Variable Argument List", "...", "variable argument list for text replacement", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CitySetMaxWorkerhutLevel", "Boolean", "Sets the max level of workerhuts a city can build", "Global", {
                { "pCity", "Alias ( of type settlement)", "city to modify", false },
                { "Level", "Number", "max level of the working huts (1..3)", false },
            }},
            { "CitySetOfficeApplicant", "Boolean", "returns if the given sim can apply for the given office in the given settlement", "Global", {
                { "pCity", "Alias ( of type settlement)", "Settlement Alias", false },
                { "pApplier", "Alias ( of type sim)", "Sim Alias", false },
                { "pOffice", "Alias ( of type office)", "Office Alias", false },
            }},
            { "CitySimBreakout", "Boolean", "notify the settlement, that a sim has broken out of the prison", "Global", {
                { "pCity", "Alias ( of type settlement)", "settlement", false },
                { "pSim", "Alias ( of type sim)", "offender", false },
            }},
            { "ClearStateImpact", "Boolean", "Clear's a state depended impact. This function is only valid in a state script", "Global", {
                { "StateImpact", "String", "state impact to set (\"no_idle\", ...)", false },
            }},
            { "CommitAction", "Boolean", "This function commits an action. The action can create evidences and controls the observer if nessesary. The action are defined in the database.", "Global", {
                { "ActionName", "String", "database name of the action", false },
                { "pActor", "Alias ( of type guildobject)", "the object what started the action", false },
                { "pVictim", "Alias ( of type sim) or Alias ( of type building)", "the victim of the action (can be a sim or a building --> (owner) )", true },
                { "pLocation", "Alias ( of type guildobject)", "the location where the action happend", true },
                { "ret_Action", "Alias ( of type actiondata)", "the new action on success", false },
            }},
            { "CopyAlias", "Boolean", "This function copies the object pSource to pDest.", "Global", {
                { "pSource", "Alias ( of type serializeable)", "The object to copy", false },
                { "pDest", "Alias ( of type serializeable)", "The dest to copy into", false },
            }},
            { "CopyAliasFromCutscene", "Boolean", "copy an aliased object to a cutscene", "Global", {
                { "AliasInCutscene", "Any", "alias of the object to copy", false },
                { "pCutscene", "Alias ( of type cutscene)", "alias of the cutscene", false },
                { "LocalAlias", "Any", "alias under which the object will be known locally", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CopyAliasToCutscene", "Nil", "copy an aliased object to a cutscene", "Global", {
                { "LocalAlias", "Any", "alias of the object to copy", false },
                { "pCutscene", "Alias ( of type cutscene)", "alias of the cutscene", false },
                { "CutsceneAlias", "Any", "alias under which the object is made available to the cutscene", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CourtingGetMaxCourtingId", "Number", "returns the max amount of courting action", "Global", {}}, // No params
            { "CourtingId2Measure", "Number", "translates the courting id to the measure name", "Global", {
                { "CourtingId", "Number", "Courting id to translate", false },
            }},
            { "CreateCutscene", "Boolean", "create a new cutscene", "Global", {
                { "CutsceneScript", "Any", "name of the script file that is located in the \"scripts/cutscenes/\" folder", false },
                { "o_pCutscene", "Alias ( of type cutscene)", "alias of the newly created cutscene", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CreateScriptcall", "Boolean", "Generates a timed script call of the function 'Function'. The script is specified by the script filename and the function Name.", "Global", {
                { "Name", "Any", "Name for the scriptcall (for queriing deleting this call eg.)", false },
                { "Timer", "Any", "time offset for executing the call (1 for 1h)", false },
                { "ScriptFilename", "Any", "filename of the script without the starting directory 'scripts'/' (eg. Measures/ms_run.lua)", false },
                { "Function", "Any", "function to call", false },
                { "pOwner", "Alias ( of type simobject)", "not measure)", false },
                { "pTarget", "Alias ( of type simobject)", "\"Destination\" from the script", true },
                { "UserValue", "Any", "User defined parameter for the Function", true },
            }}, // [AUTO-FIXED PARAMS]
            { "CreditGetInterest", "Boolean", "Get's the total value (sum + interests) of a credit", "Global", {
                { "pCredit", "Alias ( of type credit)", "the credit", false },
            }},
            { "CreditGetRemainingTime", "Boolean", "Get's the total value (sum + interests) of a credit", "Global", {
                { "pCredit", "Alias ( of type credit)", "the credit", false },
            }},
            { "CreditGetRuntime", "Boolean", "Get's the total value (sum + interests) of a credit", "Global", {
                { "pCredit", "Alias ( of type credit)", "the credit", false },
            }},
            { "CreditGetSum", "Boolean", "Get's the sum of a credit", "Global", {
                { "pCredit", "Alias ( of type credit)", "the credit", false },
            }},
            { "CreditGetTotal", "Boolean", "Get's the total value (sum + interests) of a credit", "Global", {
                { "pCredit", "Alias ( of type credit)", "the credit", false },
            }},
            { "CreditMoney", "Boolean", "Gives the object money.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to get the money (cl_Sim/cl_Dynasty/cl_Building/cl_Cart/cl_Settlement...)", false },
                { "Amount", "Number", "Amount to get", false },
                { "Purpose", "String", "Reason to get (for balance)", false },
            }},
            { "CrimeForfeit", "Nil", "removes a crime event so that nobody can charge the offender for that particular crime", "Global", {
                { "pCrimeEvent", "Alias ( of type corenode)", "the crime that should be removed", false },
                { "Usage", "Number", "1 = used for accuse, 2 = forgiven by church", false },
            }},
            { "CrimeGetEvidenceValue", "Nil", "returns the evidence value for a single evidence", "Global", {
                { "pCrimeEvent", "Alias ( of type corenode)", "the crime that should be removed", false },
            }},
            { "CutsceneAddEvent", "Nil", "calls a cutscene function in given time", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "", false },
                { "FunctionName", "Any", "Name of the function (prefix for luafile not required)", false },
                { "Minutes", "Number", "time in minutes when to call the function", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CutsceneAddTriggerEvent", "Nil", "adds a cutscene trigger event:", "Global", {
                { "pCutScene", "Alias ( of type cutscene)", "", false },
                { "FunctionName", "String", "", false },
                { "TriggerName", "String", "", false },
                { "TriggerCnt", "Number", "", false },
                { "TimeOutMinutes", "Number", "", false },
            }},
            { "CutsceneCallThread", "Nil", "starts a parallel running script, using pObject as contextobject", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "the cutscene th call is started from", false },
                { "FunctionName", "Any", "", false },
                { "pObject", "Alias ( of type guildobject)", "the object that is the contextobject inside the function call, the Cutscene is still accessible by \"owner\"", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CutsceneCameraBlend", "Nil", "tells the cutscene camera controller to make a smooth transition from the current position to the next one specified.", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "cutscene containing the camera", false },
                { "TransitionTime", "Number", "time to perform the blending", false },
                { "TransitionType", "Number", "optional type of blending: 0=linear (default), 1 = sigmoid (smooth)", true },
            }},
            { "CutsceneCameraCreate", "Nil", "creates a camera object and associates it with the cutscene. note that you should set a position immeadiatly after this .", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "pCutscene -", false },
                { "pFocusObject", "Alias ( of type guildobject)", "locator or Guildobject determining the areaID for the cutscene camera", false },
            }},
            { "CutsceneCameraRelease", "Nil", "releases the cutscene camera object associated with this cutscene, if the camera control is in cutscene mode", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "", false },
            }},
            { "CutsceneCameraSetAbsolutePosition", "Nil", "set the cutscenecamera to a given position", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "Cutscene that contains the camera", false },
                { "pLocator", "Alias ( of type position)", "a locator that defines a camera position", false },
            }},
            { "CutsceneCameraSetRelativePosition", "Nil", "set the cutscene camera to a position specified by an object and a database camera type.", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "cutscene that contains the camera", false },
                { "CameraType", "Any", "label or database id of a camera type as described by database table obj_Positions", false },
                { "pObject", "Alias ( of type corenode)", "any object that has a position and a rotation", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CutsceneCollectEvidences", "Number", "collects all evidences of pWitness against pOffender and stores them in the cutscenes dataholder. If more than 3 evidences exist evidences #3..n are combined to a single evidence", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "the cutscene that will store the evidences", false },
                { "pWitness", "Alias ( of type sim)", "he who has the evidences", false },
                { "pOffender", "Alias ( of type sim)", "only evidence where pOffender was actor will be collected.", false },
                { "bMarkAsUsed", "Boolean", "mark the return evidences as used.", true },
            }},
            { "CutsceneGetData", "Nil", "copies a property (alias or data element) from a cutscene to the current measure", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "the cutscene the data element is copied from", false },
                { "AliasName", "Any", "Name of the Property", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CutsceneGetMeasureLockTime", "void", "no comment", "Global", {}}, // No params
            { "CutsceneHUDShow", "Nil", "no comment", "Global", {}}, // No params
            { "CutsceneLocalPlayerIsWatching", "Nil", "checks if the local player is watching the cutscene", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "", false },
            }},
            { "CutsceneSendEventTrigger", "Nil", "sends a named event trigger to the given cutscene", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "", false },
                { "TriggerName", "String", "", false },
            }},
            { "CutsceneSetData", "Nil", "copies a property (alias or data element) from the current measure to a cutscene", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "the cutscene the data element is copied to", false },
                { "AliasName", "Any", "Name of the Property", false },
            }}, // [AUTO-FIXED PARAMS]
            { "CutsceneSetMeasureLockTime", "Nil", "", "Global", {
                { "Time", "Number", "the cutscene for which the time bar is to be used.", false },
            }},
            { "CutsceneSetTimeBar", "Nil", "activates the time-out bar for a given cutscene. Works only while letterboxeffect is visible. Expires after duration.", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "the cutscene for which the time bar is to be used.", false },
                { "Duration", "Number", "time in seconds", false },
            }},
            { "Detach3DSound", "Boolean", "Detaches a 3d sound from an object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to detach the sound from", false },
            }},
            { "DoNewBornStuff", "Boolean", "Initializes attributes for new born sims", "Global", {}}, // No params
            { "DynastyAddMember", "Boolean", "Add's a sim to the dynasty", "Global", {
                { "pDynasty", "Alias ( of type dynasty)", "Dynasty", false },
                { "pSim", "Alias ( of type sim)", "Sim to add", false },
            }},
            { "DynastyCreate", "Boolean", "Creates a new dynasty.", "Global", {
                { "Color", "Number", "Flag-Color", false },
                { "IsPlayer", "Boolean", "true:Player dynasty", false },
                { "PeerID", "Number", "true:Shadow dynasty", false },
                { "ret_Dynasty", "Alias ( of type dynasty)", "network PeerID of dynasty controlling player", false },
                { "Shadow", "Boolean", "new created dynasty", true },
            }},
            { "DynastyFindIdleWorker", "Boolean", "searches in the dynasty for an idle worker. The worker is filtered by it's profession and the interrupt value of the current measure", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "Profession", "Number", "profession of the worker", false },
                { "InterruptValue", "Number", "max interrupt value of the current measure", false },
                { "ret_Worker", "Alias ( of type sim)", "(return)the worker on success", false },
            }},
            { "DynastyFindNewBuilding", "Boolean", "Gets a new working building for a dynasty.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "BuyMode", "Any", "BM_NORMAL (Pay Money for buying, seller gains the money), BM_CAPTURE (Without any payment), BM_STARTUP (No money for buyer, but money for the seller)", false },
                { "ret_NewWorkingBuilding", "Alias ( of type building)", "the new working building on success", false },
            }}, // [AUTO-FIXED PARAMS]
            { "DynastyFindSquad", "void", "Get's a squad for a dynasty, with a free place", "Global", {
                { "pDynasty", "Alias ( of type dynasty)", "object to search for a free squad place", false },
                { "ret_Squad", "Alias ( of type aisquad)", "the squad on success", false },
            }},
            { "DynastyFindTargetDynasty", "Boolean", "Gets a random victim of a dynasty for a illegal action", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "bIncrement", "Boolean", "true: search target for increment the favor false: search target for decrement the favor", false },
                { "o_Victim", "Alias ( of type dynasty)", "the victim", false },
            }},
            { "DynastyForceCalcDiplomacy", "Boolean", "updates the Diplomacy states immediate, rather than waiting for 1 hour", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyGetBuildingCount", "Number", "Get's the count of buildings for a dynasty.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "BuildingClass", "Number", "Class of the buildings (-1 for all)", true },
                { "BuildingType", "Number", "Type of the buildings (-1 for all)", true },
            }},
            { "DynastyGetColor", "Number or Number or Number", "returns the dynasty color as three return values", "Global", {
                { "pDest", "Any", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "DynastyGetCredit", "Boolean", "Get's a credit from a dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
                { "Number", "Number", "Number of credit offer", false },
                { "o_pCredit", "Alias ( of type credit)", "the credit on succes", false },
            }},
            { "DynastyGetDiplomacyOffer", "Number", "Get the wanted diplomacy state from pObject1 to pObject2", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "dynasty or a part of the first dynasty", false },
                { "pObject2", "Alias ( of type guildobject)", "dynasty or a part of the second dynasty", false },
            }},
            { "DynastyGetDiplomacyState", "Number", "Get the diplomacy status between two dynasties.", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "dynasty or a part of the first dynasty", false },
                { "pObject2", "Alias ( of type guildobject)", "dynasty or a part of the second dynasty", false },
            }},
            { "DynastyGetFamilyMember", "Boolean", "Gets a family member by number from a dynasty", "Global", {
                { "pDynasty", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "number", "Number", "number of the member (0...DynastyGetFamilyMemberCount(...))", false },
                { "o_Member", "Alias ( of type sim)", "the family member on success", false },
            }},
            { "DynastyGetFamilyMemberCount", "Number", "returns the amount of family members of the dynasty", "Global", {
                { "pDynasty", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
            }},
            { "DynastyGetFlagNumber", "Number", "retrieve the flag number for the given dynasty", "Global", {
                { "pDynasty", "Alias ( of type dynasty)", "", false },
            }},
            { "DynastyGetHomeCity", "Boolean", "Gets the home city of a dynasty", "Global", {
                { "pDynasty", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "o_pCity", "Alias ( of type settlement)", "home settlement on success", false },
            }},
            { "DynastyGetMember", "Boolean", "Gets a member by number from a dynasty", "Global", {
                { "pDynasty", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "number", "Number", "number of the member (0...DynastyGetMemberCount(...))", false },
                { "o_Member", "Alias ( of type sim)", "the member on success", false },
            }},
            { "DynastyGetMemberCount", "Number", "returns the count of members of the dynasty", "Global", {
                { "pDynasty", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
            }},
            { "DynastyGetMemberRandom", "Boolean", "Gets a random member from a dynasty", "Global", {
                { "pDynasty", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "o_Member", "Alias ( of type sim)", "the member on success", false },
            }},
            { "DynastyGetRandomBuilding", "Boolean", "Get's a random building for a dynasty.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "Class", "Number", "class of the building (-1 for all)", true },
                { "Type", "Number", "Type of the building (-1 for all)", true },
                { "o_Building", "Alias ( of type building)", "returns a building on success", false },
            }},
            { "DynastyGetRandomVictim", "Boolean", "Gets a random victim of a dynasty for a illegal action", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "Favor", "Number", "the maximal favor the victim can have", false },
                { "o_Victim", "Alias ( of type dynasty)", "the victim", false },
            }},
            { "DynastyGetRanking", "void", "returns an abstract value showing the current progress of the dynasty in wealth, property and char development", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyGetTeam", "void", "Returns the team of a dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyGetWorker", "Boolean", "Get's a worker from the dynasty.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "Type", "Number", "Type of the worker (-1 for all)", false },
                { "Idx", "Number", "position of the worker (0...DynastyGetWorkerCount(..) )", false },
                { "o_Worker", "Alias ( of type sim)", "worker on success", false },
            }},
            { "DynastyGetWorkerCount", "Number", "returns the count of the workers of a dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "Type", "Number", "type of worker (-1 for all)", true },
            }},
            { "DynastyIsAI", "Boolean", "Returns true if the ai controls this dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyIsDead", "Boolean", "Returns true if dynasty is dead", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyIsPlayer", "Boolean", "Returns true if a player controls this dynasty. Attention: if the given object is a sim of a player dynasty AND then sim is not a party member, the function returns false. This is done because the ai had to do decisions for the familiy sim.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyIsShadow", "Boolean", "Returns true if the dynasty or the objects dynasty is a shadow dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "DynastyMakeImpact", "Boolean", "Make an impact on the character of a dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of a dynasty", false },
                { "CharacterValue", "Number", "Character value to modify", false },
                { "Value", "Number", "delta to change the character value", false },
            }},
            { "DynastyPaybackCredit", "Boolean", "Takes a credit from a bank. The credit is defined by the building, the sim and its number", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
                { "iNumber", "Number", "number of the offer to take", false },
            }},
            { "DynastyRemoveDiplomacyStateImpact", "Boolean", "removes running diplomacy state impacts on dynasties dynasty's", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "dynasty or a part of the first dynasty", false },
                { "pObject2", "Alias ( of type guildobject)", "dynasty or a part of the second dynasty", false },
                { "DependentID", "Number", "impact ID to remove", false },
            }},
            { "DynastyRemoveMember", "Boolean", "removes a sim from the dynasty", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to remove", false },
            }},
            { "DynastySetDiplomacyState", "Boolean", "", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "dynasty or a part of the first dynasty", false },
                { "pObject2", "Alias ( of type guildobject)", "dynasty or a part of the second dynasty", false },
                { "State", "Any", "(DIP_FOE / DIP_NEUTRAL / DIP_NAP / DIP_ALLIANCE)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "DynastySetMaxDiplomacyState", "Boolean", "Set the min diplomacy state between to dynasty's", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "dynasty or a part of the first dynasty", false },
                { "pObject2", "Alias ( of type guildobject)", "dynasty or a part of the second dynasty", false },
                { "State", "Any", "min state (DIP_FOE / DIP_NEUTRAL / DIP_NAP / DIP_ALLIANCE)", false },
                { "DependentID", "Number", "an id with which this impact is identifiable to remove it before the measure expiry (in case something happens)", false },
                { "TimeOutInHours", "Any", "timeout value of the impact (-1 for endless)", true },
            }}, // [AUTO-FIXED PARAMS]
            { "DynastySetMinDiplomacyState", "Boolean", "Set the min diplomacy state between to dynasty's", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "dynasty or a part of the first dynasty", false },
                { "pObject2", "Alias ( of type guildobject)", "dynasty or a part of the second dynasty", false },
                { "DependentID", "Number", "min state (DIP_FOE / DIP_NEUTRAL / DIP_NAP / DIP_ALLIANCE)", false },
                { "State", "Any", "an id with which this impact is identifiable to remove it before the measure expiry (in case something happens)", false },
                { "TimeOutInHours", "Any", "timeout value of the impact (-1 for endless)", true },
            }}, // [AUTO-FIXED PARAMS]
            { "DynastySetTeam", "void", "sets the team of a dynasty", "Global", {
                { "pObject", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
                { "Team", "Any", "Number of the team (0 = no team)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "DynastyTakeCredit", "Boolean", "Takes a credit from a bank. The credit is defined by the building, the sim and its number", "Global", {
                { "pBuilding", "Alias ( of type building)", "Bank", false },
                { "pSim", "Alias ( of type sim)", "the sim who has the offers", false },
                { "iNumber", "Number", "number of the offer to take", false },
            }},
            { "EndCutscene", "Nil", "terminates the cutscene", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "alias of the cutscene to terminate", false },
            }},
            { "EndUseLocator", "Boolean", "Stops the session startet with BeginUseLocator or BeginUseLocatorNoWait. The locator will be freed, and the stance of the sim is changed to Stance. The function will wait for finishing the stance.", "Global", {
                { "pSim", "Alias ( of type movingsimobject)", "the sim", false },
                { "Position", "Alias ( of type position)", "Name of the locator", false },
                { "Stance", "Number", "Stance to switch into (GL_STANCE_STAND, GL_STANCE_SIT, GL_STANCE_LAY)", false },
            }},
            { "EndUseLocatorNoWait", "Number", "Stops the session startet with BeginUseLocator or BeginUseLocatorNoWait. The locator will be freed, and the stance of the sim is changed to Stance", "Global", {
                { "pSim", "Alias ( of type movingsimobject)", "the sim", false },
                { "Stance", "Number", "Name of the locator", false },
            }},
            { "EndVision", "Boolean", "end the vision started with StartVision", "Global", {}}, // No params
            { "EnterVehicle", "Boolean", "Lets the operator enter the vehicle", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim who should enter the vehicle", false },
                { "Vehicle", "Alias ( of type vehicle)", "The vehicle the sim should enter", false },
            }},
            { "Evacuate", "Boolean", "Evacuate the building (all sims leaves the building)", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to evacuate", false },
                { "bOnlyOtherDynasties", "Boolean", "only affects non allied guys from other dynasties", true },
            }},
            { "EvidenceGetActor", "Boolean", "get the actor of an evidence of the sim SimAlias and stores it in DestinationAlias", "Global", {
                { "pEvidence", "Alias ( of type simmemoryevent)", "the evidence", false },
                { "o_Actor", "Alias ( of type sim)", "the actor on success", false },
            }},
            { "EvidenceGetSeverity", "Number", "returns the severity of the giving evidence", "Global", {
                { "pEvidence", "Alias ( of type simmemoryevent)", "the evidence", false },
            }},
            { "ExitBuildingWithCamera", "Nil", "exits the building with the camera if the camera is indoor", "Global", {}}, // No params
            { "ExitCurrentBuilding", "Boolean", "Lets the sim leaves the current building", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to leave building", false },
                { "Result", "String", "Resultname (Data)", true },
            }},
            { "ExitCurrentVehicle", "Boolean", "Lets the operator of the vehicle leave it", "Global", {
                { "pVehicle", "Alias ( of type vehicle)", "vehicle to leave", false },
            }},
            { "FadeInFE", "Nil", "activates a facial expression on the character", "Global", {
                { "pTarget", "Alias ( of type sim)", "Target Sim Alias", false },
                { "AnimName", "String", "name of facial expression", false },
                { "Weight", "Number", "weight of facial expression (1.0f is full weight)", false },
                { "FadeTime", "Number", "time to fade in the facial expression", false },
                { "Channel", "Number", "channel to modify", false },
            }},
            { "FadeOutAllFE", "Nil", "Fades out all facial expressions", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim whose fe will be fadet out", false },
                { "Channel", "Number", "channel to modify", false },
            }},
            { "FadeOutFE", "Nil", "deactivates a facial expression on the character", "Global", {
                { "pTarget", "Alias ( of type sim)", "Target Sim Alias", false },
                { "AnimName", "String", "name of the expression", false },
                { "FadeTime", "Number", "fade out time", false },
                { "Channel", "Number", "channel to modify", false },
            }},
            { "Fight", "Boolean", "Starts a fight between pObject and pTarget", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object who starts the fight", false },
                { "pTarget", "Alias ( of type guildobject)", "the attacking target", false },
                { "Type", "String", "\"subdue\" is default", true },
                { "ResultData", "String", "Data to write the fight result into (to get the value, use GetData(ResultData) )", false },
            }},
            { "Find", "Number", "checks a given amount of objects by the given filter string. The objects that passes the filter will be copied to the Result. Specifics depends on the filterstring and the used filters. If more than one object is found the resultname will be extended by an idx for ever found object (f.e. Result1, Result2...)", "Global", {
                { "TargetName", "String", "base amount of objects to bo searched by the filter", false },
                { "FilterString", "String", "the filter string", false },
                { "ResultName", "String", "alias of the result(s)", false },
                { "MaxResults", "Number", "max number of results.", false },
            }},
            { "FindByID", "Number", "checks a given amount of objects by the given filter via Filter DatabaseID. The objects that passes the filter will be copied to the Result. Specifics depends on the filterstring and the used filters. If more than one object is found the resultname will be extended by an idx for ever found object (f.e. Result1, Result2...)", "Global", {
                { "TargetName", "String", "base amount of objects to bo searched by the filter", false },
                { "FilterDBId", "Number", "ID of the filter in the Database", false },
                { "ResultName", "String", "alias of the result(s)", false },
                { "MaxResults", "Number", "max number of results", false },
            }},
            { "FindByName", "Number", "checks a given amount of objects by the given filter via name int Database. The objects that passes the filter will be copied to the Result. Specifics depends on the filterstring and the used filters. If more than one object is found the resultname will be extended by an idx for ever found object (f.e. Result1, Result2...)", "Global", {
                { "TargetName", "String", "base amount of objects to bo searched by the filter", false },
                { "FilterName", "String", "name of the filter in the Database", false },
                { "ResultName", "String", "alias of the result(s)", false },
                { "MaxResults", "Number", "max number of results", false },
            }},
            { "FindNearestBuilding", "Boolean", "Get's the nearest building (type/class/level) from pObject", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to start search", false },
                { "iClass", "Number", "Class of the building or -1 if no class filter", false },
                { "iType", "Number", "Type of the building or -1 of no type filter", false },
                { "iLevel", "Number", "Level of the building or -1 of no type filter", false },
                { "buyable", "Boolean", "false: all buildings", false },
                { "ret_ReturnAlias", "String", "The alias for storing the building", false },
            }},
            { "FindResourceProto", "Number", "gets a resource proto for an item", "Global", {}}, // No params
            { "FindWorker", "void", "Search for a worker for a building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to hire a worker", false },
                { "o_Worker", "Alias ( of type sim)", "the found worker", false },
            }},
            { "Fire", "Boolean", "Fires the sim from his current job", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to fire", false },
            }},
            { "ForbidMeasure", "Boolean", "Forbid a Measure by ID for the given AliasObject. The mode specifys if the object forbids the starting of a measure with it as owner (EN_ACTIVE), to use the object as measure target (EN_PASSIVE) or both (EN_BOTH).", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "Object to modify", false },
                { "Measure", "Number or String", "Measure to set exclusive (int: database id string: measure name)", false },
                { "Mode", "Number", "EN_PASSIVE/EN_ACTIVE/EN_BOTH", false },
            }},
            { "GFXRand", "Number", "Returns a random integer (0 <= Rand(MaxValue) < MaxValue)", "Global", {
                { "MaxValue", "Number", "Range for Random", false },
            }},
            { "Gametime2Total", "Number", "Transfer a time given in gametime (1 = 1h ingame time) to total-value (used by textsystem)", "Global", {
                { "Gametime", "Number", "Gametime to convert (1 = 1h ingame time)", false },
            }},
            { "GetAliasByID", "Boolean", "Assigns the given Alias for the object with the given ID.", "Global", {
                { "ID", "Number", "The ID of the object to get", false },
                { "p_Object", "Alias ( of type simobject)", "The desired Alias.on success", false },
            }},
            { "GetAnimationLength", "Number", "Get the lenght of an animation.", "Global", {
                { "pMoveSim", "Alias ( of type movingsimobject)", "Object", false },
                { "AnimName", "String", "Name of the new animation", false },
                { "Layer", "Number", "layer for the animation (0 = default)", true },
            }},
            { "GetArmor", "Number", "returns the armorvalue of the object (sims cumulated armored objects in inventory e.g.)", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the asked object", false },
            }},
            { "GetBuildingBasePrice", "Nil", "gets the base price of a building", "Global", {
                { "Proto", "Any", "building proto", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GetCameraPosition", "Number or Number or Number", "gets the current camera position and zoom from the world - eg.: CamX, CamZ, Zoom = GetCameraPosition()", "Global", {}}, // No params
            { "GetCrimeLocation", "Boolean", "retrieve a crime location from the memory", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object", false },
                { "ret_pLocation", "Alias ( of type guildobject)", "location of the crime (on success)", false },
            }},
            { "GetCurrentMeasureID", "Number", "get's the current measure id of the object", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "object to check", false },
            }},
            { "GetCurrentMeasureName", "String", "Gets the current measure name of the object", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "object to get the measure name from", false },
            }},
            { "GetCurrentMeasurePriority", "Number", "get's the current measure from the object and return it's priority", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "object to check", false },
            }},
            { "GetData", "Number or String", "Returns data from the measure.", "Global", {
                { "DataName", "String", "The name of the data.", false },
            }},
            { "GetDatabaseValue", "Number or String", "Returns an entry from a database table.", "Global", {
                { "TableName", "String", "The name of the database table.", false },
                { "RowID", "Number", "The ID of the row.", false },
                { "ColumnName", "String", "The name of the column for the desired value.", false },
            }},
            { "GetDirectionTo", "Number", "Returns the direction from the first object to the second", "Global", {
                { "pFirstObject", "Alias ( of type guildobject)", "The first object", false },
                { "pSecondObject", "Alias ( of type guildobject)", "The second object", false },
            }},
            { "GetDistance", "Number", "Get the grafical distance between to objects (Position/Sim/Building/...). The funktion returns -1, if the two objects are in different move-systems (eg. different rooms)", "Global", {
                { "pObject1", "Alias ( of type corenode)", "Object1 (cl_Position/cl_Sim/cl_Building/...)", false },
                { "pObject2", "Alias ( of type corenode)", "Object2 (cl_Position/cl_Sim/cl_Building/...)", false },
            }},
            { "GetDynasty", "Number", "Returns the dynasty for this object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to check (cl_Building/cl_Sim/cl_Cart/...)", false },
                { "o_Dynasty", "Alias ( of type dynasty)", "dynasty on success", false },
            }},
            { "GetDynastyID", "Number", "Returns the dynasty id for this object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to check (cl_Building/cl_Sim/cl_Cart/...)", false },
            }},
            { "GetEvadePosition", "Boolean", "Calcs an evade position for a sim with the given radius.", "Global", {
                { "pMovSim", "Alias ( of type movingsimobject)", "Sim to calc the position for", false },
                { "Distance", "Number", "max distance from the current position", false },
                { "o_Position", "Alias ( of type position)", "the flee position on success", false },
            }},
            { "GetEvidenceAlignmentSum", "Number", "calculates the alignment sum of all evidences the target has about actor", "Global", {
                { "pTarget", "Alias ( of type sim)", "Target Sim Alias", false },
                { "pActor", "Alias ( of type sim)", "Actor Sim Alias", false },
            }},
            { "GetEvidenceValues", "Number", "Returns the value sum of all evidences the target has about actor", "Global", {
                { "Target", "Alias ( of type sim)", "Target sim Alias", false },
                { "Actor", "Alias ( of type sim)", "Actor sim alias", false },
            }},
            { "GetFavorToDynasty", "Number", "Returns the favor from pObject to pTargetDynasty. If pObject is a dynasty-sim the favor from the dynasty of pObject to pTargetDynasty will be returned.", "Global", {
                { "pObject", "Alias ( of type sim pobject) or Alias ( of type dynasty)", "The sim or dynasty whose favor is searched. If it is a dynasty-sim the favor of the dynasty of it will be used.", false },
                { "pTargetDynasty", "Alias ( of type guildobject)", "dynasty or a part of the dynasty", false },
            }},
            { "GetFavorToSim", "Number", "Returns the favor from pObject to pTargetSim. If pTargetSim is a dynasty-sim the favor to the dynasty will be returned. If pObject is a dynasty-sim the favor from the dynasty of pObject will be returned.", "Global", {
                { "pObject", "Alias ( of type sim pobject) or Alias ( of type dynasty)", "The sim or dynasty whose favor is searched. If it is a dynasty-sim the favor of the dynasty of it will be used.", false },
                { "pTargetSim", "Alias ( of type sim)", "The sim to which the favor is searched.", false },
            }},
            { "GetFleePosition", "Boolean", "Calcs a flee position for a sim with the given distance from pCenter.", "Global", {
                { "pMovSim", "Alias ( of type movingsimobject)", "Sim to calc the position for", false },
                { "pCenter", "Alias ( of type simobject)", "Object to flee from", false },
                { "Distance", "Number", "distance from pCenter", false },
                { "o_Position", "Alias ( of type position)", "the flee position on success", false },
            }},
            { "GetFreeLocatorByName", "Boolean", "this function looks for a free locator with the name LocatorName + Number. For example the call GetFreeLocatorByName(\"Work\", \"Entry\", 1, 3) checks the locator \"Entry1\", \"Entry2\", \"Entry3\" of the building \"work\"", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "LocatorName", "String", "Name of the Locator without the Number", false },
                { "StartNo", "Number", "first number to check (-1=default)", true },
                { "EndNo", "Number", "last locator to check (-1=default)", true },
                { "o_Position", "Alias ( of type position)", "free locator on success", false },
                { "SuppressErrorMessage", "Boolean", "false: normal error logging true: suppress error logging", true },
            }},
            { "GetGametime", "Number", "Get the current gametime in hours.", "Global", {}}, // No params
            { "GetHP", "Number", "Returns the current hp of the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to check", false },
            }},
            { "GetHPRelative", "Number", "Returns the current relative hp of the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to check", false },
            }},
            { "GetHomeBuilding", "Boolean", "Get's the home building for a object. If the object is a worker, his worker-hut is return, if it is a cart, it's worker building is returned. A Dynasty sim returns it's residence.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object (cl_Sim/cl_Cart)", false },
                { "o_Home", "Alias ( of type building)", "on success the home building", false },
            }},
            { "GetHomeBuildingId", "void", "Get's the id of the home building for a object. If the object is a worker, his worker-hut is return, if it is a cart, it's worker building is returned. A Dynasty sim returns it's residence.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object (cl_Sim/cl_Cart)", false },
            }},
            { "GetID", "Number", "Get the ID of the object. The id is a unique number per object in the game.", "Global", {
                { "pObject", "Alias ( of type simobject)", "object to query", false },
            }},
            { "GetImpactValue", "Number", "gets the total value of an impact", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object holding the impact", false },
                { "ImpactID", "Number", "either an ID (int) or alternately a string from gl_constants that holds an ImpactType id", false },
            }},
            { "GetInsideBuilding", "Boolean", "get's the building where the sim is currently in. If the sim is outside, the function returns false", "Global", {
                { "pSim", "Alias ( of type sim)", "the Sim", false },
                { "o_Building", "Alias ( of type building)", "the building on success", false },
            }},
            { "GetInsideBuildingID", "Boolean", "get's the building where the sim is currently in. If the sim is outside, the function returns -1", "Global", {
                { "pSim", "Alias ( of type sim)", "the Sim", false },
            }},
            { "GetInsideInfo", "void", "Gets info from the current inside building. This function returns Proto+Class+Type (example: Proto,Class,Type = SimGetInsideInfo(\"\") )", "Global", {
                { "pSim", "Alias ( of type sim)", "so", false },
            }},
            { "GetInsideRoom", "Boolean", "get's the room where the sim is currently in. If the sim is outside, the function returns false", "Global", {
                { "pSim", "Alias ( of type sim)", "the Sim", false },
                { "o_Room", "Alias ( of type building)", "the room on success", false },
            }},
            { "GetItemCount", "Number", "Get the count of an item from an object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to check its inventory", false },
                { "Item", "Number or String", "database id or name of an item", false },
                { "InventoryType", "Any", "Type of the inventory. Valid types are INVENTORY_STD -> default inventory", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GetLocalPlayerDynasty", "Boolean", "Get's the local player dynasty.", "Global", {
                { "ret_pDynasty", "Alias ( of type dynasty)", "the player dynasty on success", false },
            }},
            { "GetLocatorByName", "Boolean", "Get's a locator from a building by it's name. The search sequence is first outdoor, than indoor", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to get the locator from", false },
                { "LocatorName", "String", "Name of the locator", false },
                { "o_Position", "Alias ( of type position)", "the position on success", false },
                { "SuppressErrorMessage", "Boolean", "false: normal error logging true: suppress error logging", true },
            }},
            { "GetLocatorIndex", "Number", "Get the index number of a Locator. For example this function return 3 at the locator \"Drink_3\".", "Global", {
                { "pPosition", "Alias ( of type position)", "the locator to get the index", false },
            }},
            { "GetMaxHP", "Number", "Returns the maximum hp of the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to check", false },
            }},
            { "GetMaxWorkshopCount", "void", "Return the maximum number of allowed workshops due to the title", "Global", {
                { "Dynasty", "Alias ( of type dynasty)", "The dynasty", false },
            }},
            { "GetMeasureRepeat", "void", "Gets the repeat timer for a measure standard measure repeat timer for this measure. If no destination is specified, the timer is global for executing this measure. If a destination is specified the measure is only forbidden with the destination as target.", "Global", {
                { "pOwner", "Alias ( of type guildobject)", "The measure to start. int = measure database ID / string = name of measure", false },
            }},
            { "GetMeasureRepeatName", "String", "Sets the standard measure repeat timer for this measure. See SetMeasureRepeat(...)", "Global", {
                { "pDestination", "Alias ( of type simobect)", "Destination object for the timer.", true },
            }},
            { "GetMeasureRepeatName2", "String", "Gets the standard measure repeat timer name for a measure", "Global", {
                { "Measure", "Number or String", "The measure to start. int = measure database ID / string = name of measure", false },
            }},
            { "GetMoney", "Number", "Get's the money the object has. ATTENTION: DO NOT use this with building aliases or it will cause OOS!", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to query (cl_Sim/cl_Dynasty/cl_Cart/cl_Settlement...)", false },
            }},
            { "GetName", "String", "Get the internal name of an object. This name is not localized and should only be used on debug/intern matters.", "Global", {
                { "pObject", "Alias ( of type corenode)", "object to query", false },
            }},
            { "GetNearestSettlement", "Boolean", "Gets the nearest settlement for a object (sim/cart/building)", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to find the current settlement for", false },
                { "ret_pCity", "Alias ( of type settlement)", "the settlement on success", false },
            }},
            { "GetNobilityTitle", "Number", "Returns the nobility title of the given sim or the status of the process", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim whose nobility title will be returned", false },
                { "InProcess", "Boolean", "Set this true if you want to check if a new title is currently granted", true },
            }},
            { "GetNobilityTitleLabel", "Number", "Returns the nobility title label of the given title", "Global", {
                { "Title", "Number", "The title whose label will be returned", false },
            }},
            { "GetOfficeTypeHolder", "Boolean", "retrieves the owner of a given office type", "Global", {
                { "pSettlement", "Alias ( of type guildobject)", "", false },
                { "OfficeType", "Number", "", false },
                { "Result", "String", "", false },
            }},
            { "GetOutdoorLocator", "void", "Get's outdoor-locator from the terrain by it's name. If there is more than one locator buy that name, the locator are returned with the name + the index number. eg. if ret_Position is \"Pos\", and 3 locators are found, the locators are returned as \"Pos0\", \"Pos1\", \"Pos2\". The first found locator is also returned as \"Pos\".", "Global", {
                { "LocatorName", "String", "Name of the locator", false },
                { "MaxCount", "Any", "Max count of the locators that should be returned (-1 for all)", false },
                { "ret_Position", "Alias ( of type position)", "The positions on success.", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GetOutdoorMovePosition", "Boolean", "Gets the normal outdoor moveto position for that building.", "Global", {
                { "pMove", "Alias ( of type movingsimobject)", "moveable object (cl_Sim/cl_Cart)", true },
                { "pTarget", "Alias ( of type guildobject)", "Target to move to", false },
                { "o_Position", "Alias ( of type position)", "the position on success", false },
            }},
            { "GetPosition", "Boolean", "Get's the position of an object (sim/building/cart/...) and returns it in o_Position", "Global", {
                { "ObjectAlias", "Alias ( of type guildobject) or Alias ( of type position)", "Object to get the position from", false },
                { "o_Position", "Alias ( of type position)", "position", false },
            }},
            { "GetPositionOfSubobject", "Number", "Gets the Position of", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to ask", false },
                { "subobjectname", "String", "the name of the asked subobject", false },
                { "o_Position", "Alias ( of type position)", "the resulting position", false },
            }},
            { "GetProperty", "Number or String", "Gets a certain property from the given object", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose property should be returned.", false },
                { "PropertyName", "String", "The name of the property.", false },
            }},
            { "GetRadius", "Number", "returns the radius of a moving sim object (Sim, Cart, Vehicles) for movement/range purposes", "Global", {
                { "pMoveSim", "Alias ( of type movingsimobject)", "", false },
            }},
            { "GetRemainingInventorySpace", "Number", "returns the remaining space in the objects inventory for the given item", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to check", false },
                { "Item", "Number or String", "database id or name of the item", false },
                { "InventoryType", "Any", "Type of the inventory Valid types are INVENTORY_STD -> default inventory, INVENTORY_SELL -> building sell inventory, INVENTORY_EQUIPMENT -> sims equipment inventory", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GetRepeatTimerEndtime", "void", "Get's the gildtime, when the timer object expires. The function returns -1, if the timer doesn't exist", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object who owns the timer", false },
                { "TimerName", "String", "Name of the timer", false },
            }},
            { "GetRepeatTimerLeft", "void", "Get's the left time on the timer object. The function returns -1, if the timer doesn't exists (anymore)", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object who owns the timer", false },
                { "TimerName", "String", "Name of the timer", false },
            }},
            { "GetRotation", "Number", "Get the rotation between to objects (Position/Sim/Building/...)", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "Object1 (cl_Position/cl_Sim/cl_Building/...)", false },
                { "pObject2", "Alias ( of type guildobject)", "Object2 (cl_Position/cl_Sim/cl_Building/...)", false },
            }},
            { "GetRotationTo", "Number", "Returns the euler rotation difference from the first object to the second", "Global", {
                { "pFirstObject", "Alias ( of type guildobject)", "The first object", false },
                { "pSecondObject", "Alias ( of type guildobject)", "The second object", false },
            }},
            { "GetRound", "Number", "Get the current round (0...n)", "Global", {}}, // No params
            { "GetScenario", "Boolean", "make the guildworld accessible via an alias", "Global", {
                { "*pWorld", "Any", "alias assigned to the world", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GetSeason", "void", "Returns the current season.", "Global", {}}, // No params
            { "GetSettingNumber", "Number", "Get a float/int value from the config file", "Global", {
                { "Group", "String", "ini-file group", false },
                { "Name", "String", "entry name", false },
                { "Default", "Number", "default value (if no value is found)", false },
            }},
            { "GetSettingString", "Number", "Get a string value from the config file", "Global", {
                { "Group", "String", "ini-file group", false },
                { "Name", "String", "entry name", false },
                { "Default", "String", "default value (if no value is found)", false },
            }},
            { "GetSettlement", "Boolean", "Get's the assigned settlement of the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Sim", false },
                { "o_City", "Alias ( of type settlement)", "Settlement on success", false },
            }},
            { "GetSettlementID", "Number", "Get's the id of the assigned settlement of the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Sim", false },
            }},
            { "GetSimType", "Number", "returns the type of the sim ( 0 = SIM, 1 = DYNASTY SIM, 2 = PARTY SIM)", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim", false },
            }},
            { "GetSkillValue", "Number", "Returns the value of the given skill for a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
                { "SkillNumber", "Number", "Skill to check (use script constants like \"BARGAINING\" !)", false },
            }},
            { "GetState", "Boolean", "returns the status of an object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to query", false },
                { "State", "Number", "state to set (STATE_IDLE, STATE_REPAIRING, STATE_BURNING, ...)", false },
            }},
            { "GetStateImpact", "void", "Get's a state depended impact from the pObject.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "state impact to set (\"no_idle\", ...)", false },
            }},
            { "GetVehicle", "Boolean", "returns the vehicle a moving sim object is attached to", "Global", {
                { "pMoveSim", "Alias ( of type movingsimobject)", "the asked object", false },
                { "pVehicle", "Alias ( of type vehicle)", "the attached-to vehicle", false },
            }},
            { "GfxAttachObject", "Boolean", "Attaches an graphics object on the basis of a filename to the measure.", "Global", {
                { "Alias", "String", "The alias for the object.", false },
                { "Filename", "String", "The name of the file starting at \"W:\\TheGuild2\\GameData\\Objects\".", false },
            }},
            { "GfxAttachObjectByID", "Boolean", "Attaches an graphics object on the basis of a database-id to the measure.", "Global", {
                { "Alias", "String", "The alias for the object.", false },
                { "ModelID", "Number", "The database-id of the model.", false },
            }},
            { "GfxDetachAllObjects", "Boolean", "Detaches all attached graphics objects.", "Global", {}}, // No params
            { "GfxDetachObject", "Boolean", "Detaches an attached graphics object from the measure.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to detach", false },
            }},
            { "GfxDropToFloor", "Boolean", "Sets an object to the ground of the current scene.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object which will be positioned.", false },
            }},
            { "GfxGetHeight", "Number", "Returns the height of the object.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose height will be returned.", false },
            }},
            { "GfxIsMoving", "Boolean", "Indicates if an attached graphics object is moving.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object which is possibly in movement.", false },
            }},
            { "GfxMoveNoWait", "Boolean", "Moves an object with a certain speed while proceeding with the script.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be moved.", false },
                { "DirX", "Number", "The moving direction in x-coordinates.", false },
                { "DirY", "Number", "The moving direction in y-coordinates.", false },
                { "DirZ", "Number", "The moving direction in z-coordinates.", false },
                { "Speed", "Number", "The moving speed in units per second.", false },
                { "Worldcoordinates", "Boolean", "True means the direction is given in worldcoordinates, false means objectcoordinates.", false },
            }},
            { "GfxMoveTo", "Boolean", "Moves an object to the position of another object within a certain time. The script will wait for the movement to continue.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be moved.", false },
                { "DestinationObject", "Alias ( of type corenode)", "The object to be moved to.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
            }},
            { "GfxMoveToNoWait", "Boolean", "Moves an object to the position of another object within a certain time while proceeding with the script.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be moved.", false },
                { "DestinationObject", "Alias ( of type corenode)", "The object to be moved to.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
            }},
            { "GfxMoveToPosition", "Boolean", "Moves an object to a certain position within a certain time. The script will wait for the movement to continue.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be moved.", false },
                { "PosX", "Number", "The x-coordinate of the position.", false },
                { "PosY", "Number", "The y-coordinate of the position.", false },
                { "PosZ", "Number", "The z-coordinate of the position.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
                { "Absolute", "Boolean", "True means the given position is meant to be in worldcoordinates, false means relative to the objects position.", false },
            }},
            { "GfxMoveToPositionNoWait", "Boolean", "Moves an object to a certain position within a certain time while proceeding with the script.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be moved.", false },
                { "PosX", "Number", "The x-coordinate of the position.", false },
                { "PosY", "Number", "The y-coordinate of the position.", false },
                { "PosZ", "Number", "The z-coordinate of the position.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
                { "Absolute", "Boolean", "True means the given position is meant to be in worldcoordinates, false means relative to the objects position.", false },
            }},
            { "GfxRotateNoWait", "Boolean", "Rotates an object around itself around a given axis while proceeding with the script.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be rotated.", false },
                { "AxisX", "Number", "The x-coordinate of the rotation axis.", false },
                { "AxisY", "Number", "The y-coordinate of the rotation axis.", false },
                { "AxisZ", "Number", "The z-coordinate of the rotation axis.", false },
                { "Speed", "Number", "The rotation speed in degrees per second.", false },
                { "Worldcoordinates", "Boolean", "True means the rotation axis is meant to be in worldcoordinates, false means relative to the objects rotation.", false },
            }},
            { "GfxRotateTo", "Number", "Rotates an object to the rotation of another object. The script will wait for the rotation to continue.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be rotated.", false },
                { "DestinationObject", "Alias ( of type corenode)", "The object to be rotated to.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
            }},
            { "GfxRotateToAngle", "Boolean", "Rotates an object to a certain angle within a given time. The script will wait for the rotation to continue.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be rotated.", false },
                { "AngleX", "Number", "The x-coordinate of the angle.", false },
                { "AngleY", "Number", "The y-coordinate of the angle.", false },
                { "AngleZ", "Number", "The z-coordinate of the angle.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
                { "Absolute", "Boolean", "True means the angle is meant to be in worldcoordinates, false means relative to the objects rotation.", false },
            }},
            { "GfxRotateToAngleNoWait", "Boolean", "Rotates an object to a certain angle within a given time while proceeding with the script.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be rotated.", false },
                { "AngleX", "Number", "The x-coordinate of the angle.", false },
                { "AngleY", "Number", "The y-coordinate of the angle.", false },
                { "AngleZ", "Number", "The z-coordinate of the angle.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
                { "Absolute", "Boolean", "True means the angle is meant to be in worldcoordinates, false means relative to the objects rotation.", false },
            }},
            { "GfxRotateToNoWait", "Number", "Rotates an object to the rotation of another object while proceeding with the script.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object to be rotated.", false },
                { "DestinationObject", "Alias ( of type corenode)", "The object to be rotated to.", false },
                { "TimeSpan", "Number", "The time in which the movement should be finished.", false },
            }},
            { "GfxScale", "Boolean", "Sets the scale of an attached graphics object.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose scale will be set.", false },
                { "ScaleFactor", "Number", "The new scale factor.", false },
            }},
            { "GfxSetPosition", "Boolean", "Sets the position of an object related to the measure.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose position will be set.", false },
                { "PosX", "Number", "The new position in x-direction.", false },
                { "PosY", "Number", "The new position in y-direction.", false },
                { "PosZ", "Number", "The new position in z-direction.", false },
                { "Absolute", "Boolean", "Indicates if the given position is meant to be relative to the current position or not.", false },
            }},
            { "GfxSetPositionTo", "Boolean", "Sets an object to the position of another object.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object which will be positioned.", false },
                { "DestinationObject", "Alias ( of type corenode)", "The object to where the object will be positioned.", false },
            }},
            { "GfxSetRotation", "Nil", "Sets the rotation of an object related to the measure.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose rotation will be set.", false },
                { "AngleX", "Number", "The new rotation around the x-axis.", false },
                { "AngleY", "Number", "The new rotation around the y-axis.", false },
                { "AngleZ", "Number", "The new rotation around the z-axis.", false },
                { "Absolute", "Boolean", "Indicates if the given rotation is meant to be relative to the current rotation or not.", false },
            }},
            { "GfxSetRotationTo", "Boolean", "Rotates an object to the rotation of another object.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object which will be rotated.", false },
                { "DestinationObject", "Alias ( of type corenode)", "The object to where the object will be rotated.", false },
            }},
            { "GfxStartParticle", "Boolean", "Attaches a particle effect on the basis of a filename.", "Global", {
                { "Alias", "String", "The alias for the particle effect.", false },
                { "Filename", "String", "The name of the file starting at \"W:\\TheGuild2\\GameData\\Objects\".", false },
            }},
            { "GfxStopParticle", "Boolean", "Stops the emitting of a particle effect and kills the effect.", "Global", {
                { "Alias", "String", "The alias for the particle effect.", false },
            }},
            { "GuildGameObjects_BuildingGetPrisoner", "Number", "Gets The hijacked guy withing this Building (ofc oly if there is one)", "Global", {
                { "pBuilding", "Alias ( of type building)", "building to check", false },
                { "pHijacked", "Alias ( of type sim)", "the hijacked sim", false },
            }},
            { "GuildGameObjects_CutsceneCallScheduled", "Nil", "call a function inside a cutscene script. This operation immediatly return", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "Alias to a cutscene object for which the function is called", false },
                { "FunctionName", "Any", "Name of the Function to be called (without scriptfile prefix)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GuildGameObjects_CutsceneCallUnscheduled", "Nil", "call a function inside a cutscene script. The called function is executed completely after the call to CutsceneCallUnscheduled", "Global", {
                { "pCutscene", "Alias ( of type cutscene)", "Alias to a cutscene object for which the function is called", false },
                { "FunctionName", "Any", "Name of the Function to be called (without scriptfile prefix)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GuildII_ClearImportantPersonSection", "Nil", "", "Global", {
                { "Label", "Any", "", false },
                { "Variable Argument List", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GuildII_CreateImportantPersonSection", "Nil", "", "Global", {
                { "Label", "Any", "", false },
                { "Variable Argument List", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "GuildII_SetImportantPerson", "Nil", "sets the measure runtime for displaying in the Measure OSH", "Global", {
                { "SimID", "Number", "", false },
                { "SectionName", "Any", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "HasData", "Boolean", "Indicates it a certain data exists.", "Global", {
                { "DataName", "String", "The name of the data which probably exists.", false },
            }},
            { "HasProperty", "Boolean", "Indicates if the given object has a certain property.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object which should have the property.", false },
                { "PropertyName", "String", "The name of the property.", false },
            }},
            { "HasRunningQuest", "Boolean", "returns if the given actor has the given quest running", "Global", {
                { "pQuestname", "String", "questname", false },
                { "pActor", "Alias ( of type sim)", "actor alias", false },
            }},
            { "HideObject", "Boolean", "Hide's a 3d object in the scene of pObject", "Global", {
                { "pObject", "Alias ( of type simobject)", "an object to identify the scene for hiding the object", false },
                { "ObjectName", "String", "Name of the object to hide", false },
            }},
            { "HideTutorialBox", "Boolean", "Hides the tutorial box", "Global", {}}, // No params
            { "HireWorker", "void", "Search and hire a worker for a building.", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to hire a worker", false },
                { "o_Worker", "Alias ( of type sim)", "the hired worker", true },
            }},
            { "HudAddToSelection", "Boolean", "adds an object to the selection of the local player", "Global", {}}, // No params
            { "HudCancelUserSelection", "Nil", "cancels the user measure selection", "Global", {}}, // No params
            { "HudClearSelection", "Boolean", "Clears the selection of the local player", "Global", {}}, // No params
            { "HudDeselect", "Boolean", "This function deselects the current selected object", "Global", {}}, // No params
            { "HudEnableElement", "Boolean", "enables/disables a hud element specified by element name.", "Global", {
                { "pElementName", "String", "element name. Can be one of the following:", false },
                { "State", "Boolean", "", false },
            }},
            { "HudGetActiveCharacter", "Boolean", "adds the currently active playercharacter (Sim) into the given aliasname", "Global", {
                { "o_pAlias", "Alias ( of type sim)", "destalias", false },
            }},
            { "HudGetActiveSheet", "void", "returns the name of the currently opened sheet if exists !! untested !!", "Global", {
                { "Nil", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "HudGetSelected", "Boolean", "This function writes the selected object into the given alias", "Global", {
                { "Object", "Alias ( of type guildobject)", "The selected object", false },
            }},
            { "HudPanelIsVisible", "Boolean", "returns if the panel specified by the given panel name is visible or not", "Global", {
                { "PanelName", "Any", "panel name", false },
            }}, // [AUTO-FIXED PARAMS]
            { "HudSelect", "Boolean", "This function selects a guildobject in the hud as the current selection.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "a object to select", false },
            }},
            { "HudSetViewToObject", "Nil", "set the camera to focus on any object that has position (locator,sim,building,etc.) at given distance and angle", "Global", {
                { "*pPosition", "Any", "any object of Sim,Locator/Position,Building", false },
                { "Distance", "Number", "(optional) distance/zoom of the camera", true },
                { "Angle", "Number", "(optional) Y angle (view direction) of the camera", true },
                { "JumpTo", "Boolean", "(optional) true: jump direct to target - false: normal jumping", true },
            }}, // [AUTO-FIXED PARAMS]
            { "ImpactGetMaxTimeleft", "void", "Get the maximal timeleft of an impact type from the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object that will hold the impact", false },
                { "ImpactID", "Number or String", "ID or name in gl_impact_type table", false },
            }},
            { "Include", "Boolean", "includes the given script into the running script.", "Global", {
                { "pScriptName", "String", "scriptname", false },
            }},
            { "IncrementXP", "Number", "Increment the xp from pTarget.", "Global", {
                { "pTarget", "Alias ( of type sim)", "Sim to modify it's xp", false },
                { "XP", "Number", "value to add", false },
            }},
            { "InitAlias", "Boolean", "gets a measure alias by user selection, the scripts waits for the selection to be completed", "Global", {
                { "pName", "String", "alias name", false },
                { "pPanelParam", "String", "panel param", false },
                { "pFilter", "String", "alias filter. If no filter needed (which means every selectable object is valid!) use \"\".", false },
                { "pTargetMessage", "String", "target message label", false },
                { "AIFunc", "Any", "lua function name for the AI initialisation. If no AIFunc needed it has to be 0. If a false or not existing AIFunc is given this will result in an tolua error. Remember in lua you have to prefix with the whole scriptname. EG ms_walk_myfunction. The scriptname has to be lowercase.", false },
            }}, // [AUTO-FIXED PARAMS]
            { "InitData", "String", "initialize a measure data, the scripts waits for the initialization to be completed", "Global", {
                { "pPanelParam", "String", "parameter to define the shown inputpanel", false },
                { "AIFunc", "Any", "lua function name for the AI initialisation. If no AIFunc needed it has to be 0. If a false or not existing AIFunc is given this will result in an tolua error. Remember in lua you have to prefix with the whole scriptname. EG ms_walk_myfunction. The scriptname has to be lowercase.", false },
                { "HeaderLabel", "String", "Header Text Label", false },
                { "BodyLabel", "String", "Body Text Label, !!! Obsolete, only remains for compatibility reasons", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for header and buttons", false },
            }}, // [AUTO-FIXED PARAMS]
            { "InternalDie", "Boolean", "lets the object die (cl_Building or cl_Sim)", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to die", false },
            }},
            { "InternalRemove", "Boolean", "kill the object from the world", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to remove", false },
            }},
            { "InventoryGetSlotCount", "void", "Gets the slot count from a inventory of the pOwner.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "owner of the inventory", false },
                { "InventoryType", "Any", "of the inventory to add the items to (INVENTORY_STD, INVENTORY_SELL, INVENTORY_EQUIPMENT)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "InventoryGetSlotInfo", "void", "Gets the slot info from a inventory of the pOwner.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Owner of the measure to start", false },
                { "Slot", "Any", "0 <= Number < InventoryGetSlotCount(pOwner)", false },
                { "InventoryType", "Any", "of the inventory to add the items to (INVENTORY_STD, INVENTORY_SELL, INVENTORY_EQUIPMENT)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "IsAIDriven", "Boolean", "Returns true if the measure is ai driven.", "Global", {}}, // No params
            { "IsDead", "Boolean", "returns true, if the object is dead", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to query", false },
            }},
            { "IsDestroyed", "Boolean", "Returns if the measure object is in the \"destroyed\" state.", "Global", {}}, // No params
            { "IsDynastySim", "Boolean", "Returns true if the sim is a dynasty sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "IsGUIDriven", "Boolean", "Returns true if the measure is gui driven.", "Global", {}}, // No params
            { "IsInLoadingRange", "Boolean", "This function returns true, if the first object is in loading range to the second", "Global", {
                { "pObject", "Alias ( of type guildobject)", "first object", false },
                { "pDest", "Alias ( of type guildobject)", "second object", false },
            }},
            { "IsInit", "Boolean", "Returns if the measure object is in the \"init\" state.", "Global", {}}, // No params
            { "IsLocatorFree", "Number", "Returns true if the given locator is free", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to query", false },
            }},
            { "IsMultiplayerGame", "Boolean", "Indicates if it is a multiplayer game", "Global", {}}, // No params
            { "IsOnlyPartyMember", "Boolean", "Returns true if the sim is the only member of its party", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "IsPartyMember", "Boolean", "Returns true if the sim is a party member", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "IsReady", "Boolean", "Returns if the measure object is in the \"ready\" state.", "Global", {}}, // No params
            { "IsStateDriven", "Boolean", "Returns if the measure is state driven.", "Global", {}}, // No params
            { "IsType", "Boolean", "Checks, if a object is from a type (Sim, Building)", "Global", {
                { "pObject", "Alias ( of type corenode)", "Object to check", false },
                { "TypeName", "String", "Name of the class (Sim, Building, Cart, ...)", false },
            }},
            { "ItemGetBasePrice", "void", "Gets the base price of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetCategory", "Number", "Gets the category of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetID", "Number", "Gets the id of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetLabel", "String", "Gets the label of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
                { "Singular", "Boolean", "true: singular false: plural", false },
            }},
            { "ItemGetName", "String", "Gets the name of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetPriceBuy", "void", "Gets the base price of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
                { "pMarket", "Alias ( of type market)", "Market to query", false },
            }},
            { "ItemGetPriceSell", "void", "Gets the base price of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
                { "pMarket", "Alias ( of type market)", "Market to query", false },
            }},
            { "ItemGetProductionAmount", "void", "Gets the amount of items a production will produce", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetProductionTime", "void", "Gets the time (in hours) of the production of this item", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetSlot", "Number", "Gets the name of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetSubstLevel", "void", "Gets the substitutionslevel of an item", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "ItemGetType", "Number", "Gets the type of an item.", "Global", {
                { "Item", "Number or String", "database id or name of an item", false },
            }},
            { "Kill", "Boolean", "Kills the object (cl_Building or cl_Sim)", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to be killed", false },
            }},
            { "KillQuest", "Nil", "kills the quest specified with pQuestname and QuestSource. If QuestSource is 0 all Quest with pQuestname will be killed. If both are 0, the current quest will be killed.", "Global", {
                { "pQuestname", "String", "questname", false },
                { "pQuestSourceAlias", "Alias ( of type guildobject)", "questsource alias", false },
            }},
            { "ListAdd", "Nil", "adds the an object to a list", "Global", {
                { "ListAliasName", "Any", "alias of the list", false },
                { "ObjectAliasName", "Any", "alias of the object", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ListClear", "Nil", "removes all elements from a list", "Global", {
                { "ListAlias", "Any", "alias of the list to remove from", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ListGetElement", "Number", "gets an element from a list by index, and makes it accessible by ObjectAlias if successfull", "Global", {
                { "ListAlias", "Any", "alias of the list", false },
                { "Index", "Number", "Index of the object to retrieve", false },
                { "ObjectAlias", "Any", "Alias that is assigned to the retrieved object if exists", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ListMerge", "Nil", "adds the elements of SrcListAliasName to DestListAliasName. The members are not removed from the sourcelist", "Global", {
                { "DestListAliasName", "Any", "alias of the destination list", false },
                { "SrcListAliasName", "Any", "alias of the source list", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ListNew", "Nil", "Create a list under the alias ListAliasName", "Global", {
                { "ListAliasName", "Any", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ListRemove", "Nil", "removes an element from a list", "Global", {
                { "ListAlias", "Any", "alias of the list to remove from", false },
                { "ObjectAlias", "Any", "alias of the element to remove", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ListSize", "Number", "returns the size of a list", "Global", {
                { "ListAlias", "Any", "alias of the list", false },
            }}, // [AUTO-FIXED PARAMS]
            { "LocaterGetBlocker", "void", "Blocks a free locator for use for pObject only.", "Global", {
                { "Position", "Alias ( of type position)", "the SimObject who blocks this locator", false },
            }},
            { "LocatorStatus", "Boolean", "check's the locator status", "Global", {
                { "pBuilding", "Alias ( of type building)", "Building to get the locator from", false },
                { "LocatorName", "String", "Name of the locator", false },
            }},
            { "LogText", "Nil", "Logs the given text.", "Global", {
                { "Text", "String", "The text to be loged.", false },
            }},
            { "LoopAnimation", "Number", "Loops a animation on pMoveSim.", "Global", {
                { "pMoveSim", "Alias ( of type movingsimobject)", "Object", false },
                { "AnimName", "String", "Name of the new animation", false },
                { "LoopTime", "Number", "time to loop in seconds", false },
                { "Layer", "Number", "layer for the animation (0 = default)", true },
            }},
            { "MapHighlightObject", "Boolean", "shows/hides the highlight in the minimap/map for the given object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to highlight in minimap/map", false },
                { "ShowInMinimap", "Boolean", "true: show highlight in minimap, false: hide highlight in minimap", false },
                { "ShowInMap", "Boolean", "true: show highlight in map, false: hide highlight in map", false },
            }},
            { "MeasureAddAlias", "Boolean", "Add's a alias parameter to an AIMeasure. This data is requested in the init function of the measure", "Global", {
                { "pMeasure", "Alias ( of type aimeasure)", "AIMeasure", false },
                { "DataType", "String", "Typ of the data (script specific)", false },
                { "pAlias", "Alias ( of type rootref)", "Alias to add", false },
                { "InitBlock", "Boolean", "false: adds it directly to the measure / true: Add the data value to the init block", false },
            }},
            { "MeasureAddData", "Boolean", "Add's a data member to an AIMeasure. This data is requested on the init function of the measure", "Global", {
                { "pMeasure", "Alias ( of type aimeasure)", "AIMeasure", false },
                { "DataType", "String", "Typ of Data (Measure specific)", false },
                { "Data", "Number or Boolean or String", "Value (can be float/bool/string)", false },
                { "InitBlock", "Boolean", "false: adds it directly to the measure / true: Add the data value to the init block", false },
            }},
            { "MeasureClearRuntimeData", "Nil", "clears the script runtime data, which means the script can only access the init data after the function call", "Global", {
                { "Nil", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MeasureCreate", "Boolean", "Creates a measure for inscript measure starting", "Global", {
                { "o_Measure", "Alias ( of type aimeasure)", "the new created ai measure", false },
            }},
            { "MeasureGetID", "Number", "returns the database id of a measure", "Global", {
                { "Measure", "String", "The measure to start. string = name of measure", false },
            }},
            { "MeasureGetPriority", "Number", "Returns the priority of the measure.", "Global", {}}, // No params
            { "MeasureIsValid", "Boolean", "checks, if the measure is valid. The measure must be defined by it's owner, [a target object] and the measure itself)", "Global", {
                { "pOwner", "Alias ( of type measuresimobject)", "owner of the measure to start", false },
                { "pTarget", "Alias ( of type corenode)", "measure target", true },
                { "Measure", "Number or String", "The measure to start. int = measure database ID / string = name of measure", false },
            }},
            { "MeasureRun", "String", "Directly starts a measure. Only measure's with max. two parameters can be started this way", "Global", {
                { "pThisObject", "Alias ( of type guildobject)", "the object that is the this object of the new measure", false },
                { "pDestination", "Alias ( of type corenode)", "the destination of the measure", false },
                { "Measure", "Number or String", "The measure to start. int = measure database ID / string = name of measure", false },
                { "force", "Boolean", "true: force the measure to start (dont look at priorities) false: use priorities (default)", true },
            }},
            { "MeasureSetNotRestartable", "Nil", "forbids the measure to restart, which means the measur will cleanup instead of restart", "Global", {
                { "Nil", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MeasureSetPriority", "Boolean", "Sets a new priority to this measure.", "Global", {
                { "Priority", "Number", "The new priority.", false },
            }},
            { "MeasureStart", "Boolean", "Start's a Measure on the ThisObject.", "Global", {
                { "pMeasure", "Alias ( of type aimeasure)", "AIMeasure", false },
                { "pThisObject", "Alias ( of type guildobject)", "the object that is the this object of the new measure", false },
                { "pDestination", "Alias ( of type corenode)", "the destination of the measure", false },
                { "Measure", "Number or String", "The measure to start. int = measure database ID / string = name of measure", false },
                { "force", "Boolean", "true: force the measure to start (dont look at priorities) false: use priorities (default)", true },
            }},
            { "ModifyFavorToDynasty", "Number", "Modifies the favor from pObject to pTargetDynasty by the given amount. If pObject is a dynasty-sim the favor from the dynasty of pObject will be modified.", "Global", {
                { "pObject", "Alias ( of type sim pobject) or Alias ( of type dynasty)", "The sim or dynasty whose favor will be modified. If it is a dynasty-sim the favor of the dynasty of it will be modified.", false },
                { "pTargetDynasty", "Alias ( of type dynasty)", "The dynasty to which the favor will be modified.", false },
                { "Modification", "Number", "The amount of the modification.", false },
            }},
            { "ModifyFavorToSim", "Number", "Modifies the favor from pObject to pTargetSim by the given amount. If pTargetSim is a dynasty-sim the favor to the dynasty will be modified. If pObject is a dynasty-sim the favor from the dynasty of pObject will be modified.", "Global", {
                { "pObject", "Alias ( of type sim pobject) or Alias ( of type dynasty)", "The sim or dynasty whose favor will be modified. If it is a dynasty-sim the favor of the dynasty of it will be modified.", false },
                { "pTargetSim", "Alias ( of type sim)", "The sim to which the favor will be modified.", false },
                { "Modification", "Number", "The amount of the modification.", false },
            }},
            { "ModifyHP", "Boolean", "Set the hp points of the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object to modify", false },
                { "Status", "Number", "the new status", false },
                { "OverheadSymbol", "Boolean", "True means show an overhead symbol (default), false means don�t show an overhead symbol", false },
                { "a_fForcedMinValue", "Number", "the absolute health will/must not fall under this value", false },
            }},
            { "MoveSetActivity", "Number", "sets a certain activity for pObject.", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "The object to behave like that", false },
                { "activity", "String", "possible activities are currently \"drunk\", \"carry\", \"carrypeel\", \"pillory\", \"arrest\", \"execute\", \"converse\", \"unconscious\", \"carrywood\", \"childplay\", \"chop\", \"duel\", \"duelshoot\", (for internal use \"fighting\", \"fightingunarmed\" \"wagondraw\", \"wagonsteer\"). Disable/reset activity by not using the second param.", false },
            }},
            { "MoveSetStance", "Number", "sets a certain stance for pObject.", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "The object to get this stance", false },
                { "iStance", "Number", "possible stances are GL_STANCE_STAND, GL_STANCE_SIT, GL_STANCE_SITBENCH, GL_STANCE_SITGROUND , GL_STANCE_CROUCH, GL_STANCE_KNEEL, GL_STANCE_LAY, GL_STANCE_LAYGROUND", false },
            }},
            { "MoveStop", "Boolean", "stop the current move task for pObject.", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "The object to stop moveing (cl_Sim/cl_Cart/...)", false },
            }},
            { "MsgBox", "Boolean", "shows the given text in the hud-messagebox. Text has to be a label which will be localized with the textsystem. The script waits for the user to close the messagebox.", "Global", {
                { "pTarget", "Alias ( of type guildobject)", "Target of the message. to decide if the message concerns the player. If 0 the message goes to all players", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Target for jump to", false },
                { "pPanelParam", "String", "panel related parameters.", false },
                { "HeaderLabel", "String", "Header Text Label", false },
                { "BodyLabel", "String", "Body Text Label", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for header and bodylabel", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgBoxNoWait", "Boolean", "shows the given text in the hud-messagebox. Text has to be a label which will be localized with the textsystem.", "Global", {
                { "pTarget", "Alias ( of type guildobject)", "Target of the message. to decide if the message concerns the player. If 0 the message goes to all players", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Target for jump to", false },
                { "HeaderLabel", "String", "Header Text Label", false },
                { "BodyLabel", "String", "Body Text Label", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for header and bodylabel", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgDebugMeasure", "Nil", "Sets a debug message text.", "Global", {
                { "Text", "String", "The debug message text.", false },
            }},
            { "MsgMeasure", "Boolean", "sends a measure message for the given object", "Global", {
                { "Object", "Alias ( of type measuresimobject)", "The object to which the message will be assigned to.", false },
                { "Message", "String", "The text of the message.", false },
            }},
            { "MsgNews", "String", "sends a news message for the given object and waits for the user to deal with it or for the given timeout", "Global", {
                { "pDestination", "Alias ( of type guildobject)", "Object Alias to decide if the target is the player", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Jump to Target", false },
                { "pPanelParam", "String", "panel related parameters.", false },
                { "AIFunc", "Any", "lua function name for the AI reaction. If no AIFunc needed it has to be 0. If a false or not existing AIFunc is given this will result in an tolua error. Remember in lua you have to prefix with the whole scriptname. EG ms_walk_myfunction. The scriptname has to be lowercase", false },
                { "pMessageClass", "String", "the message class", false },
                { "TimeOut", "Number", "time out time for the news in gametime hours. time after which the news will be removed automaticaly. If no timeout should be attached use -1. If removed automaticaly the name of the last pressed button will be \"C\".", false },
                { "pHeaderLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "pBodyLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text, will be share by HeaderLabel and BodyLabel", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgNewsNoWait", "Boolean", "sends a news message for the given object", "Global", {
                { "pDestination", "Alias ( of type guildobject)", "Object Alias to decide if the target is the player", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Jump to Target", false },
                { "pPanelParam", "String", "panel related parameters.", false },
                { "pMessageClass", "String", "the message class", false },
                { "TimeOut", "Number", "time out time for the news in gametime hours. time after which the news will be removed automaticaly. If no timeout should be attached use -1.", false },
                { "pHeaderLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "pBodyLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text, will be share by HeaderLabel and BodyLabel", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgQuest", "Boolean", "shows the given text in the hud-messagebox and waits until the user closes the questbox", "Global", {
                { "pTarget", "Alias ( of type guildobject)", "Target of the message. to decide if the message concerns the player. If 0 the message goes to all players", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Target for jump to", false },
                { "pPanelParam", "String", "panel related parameters.", false },
                { "HeaderLabel", "String", "Header Text Label", false },
                { "BodyLabel", "String", "Body Text Label", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for header and bodylabel", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgQuestIntro", "Boolean", "shows the given text in the quest intro panel", "Global", {
                { "pTextLabel", "String", "Textlabel", false },
            }},
            { "MsgQuestNoWait", "Boolean", "shows the given text in the hud-messagebox.", "Global", {
                { "pTarget", "Alias ( of type guildobject)", "Target of the message. to decide if the message concerns the player. If 0 the message goes to all players", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Target for jump to", false },
                { "HeaderLabel", "String", "Header Text Label", false },
                { "BodyLabel", "String", "Body Text Label", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for header and bodylabel", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgQuick", "Boolean", "sends a quick message for the given object", "Global", {
                { "Object", "Alias ( of type measuresimobject)", "The object to which the message will be assigned to.", false },
                { "Message", "String", "The text of the message.", false },
            }},
            { "MsgSay", "Boolean", "sends a dialog message for the given object and waits until the dialog closes", "Global", {
                { "pSpeaker", "Alias ( of type guildobject)", "Speaker of the message", false },
                { "pTextLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgSayInteraction", "String", "sends a dialog message which provides user interaction and waits until the dialog closes", "Global", {
                { "pTarget", "Alias ( of type guildobject)", "Target of the message (the one who should interact with the message)", false },
                { "pSpeaker", "Alias ( of type guildobject)", "Speaker of the message", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Jump To Target", false },
                { "pPanelParam", "String", "panel related parameters.", false },
                { "AIFunc", "Any", "ai func", false },
                { "pLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgSayInteractionNoWait", "String", "sends a dialog message which provides user interaction", "Global", {
                { "pTarget", "Alias ( of type guildobject)", "Target of the message (the one who should interact with the message)", false },
                { "pSpeaker", "Alias ( of type guildobject)", "Speaker of the message", false },
                { "pJumpToTarget", "Alias ( of type corenode)", "Jump To Target", false },
                { "pPanelParam", "String", "panel related parameters.", false },
                { "AIFunc", "Any", "ai func", false },
                { "pLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgSayNoWait", "Boolean", "sends a dialog message for the given object", "Global", {
                { "pSpeaker", "Alias ( of type guildobject)", "Speaker of the message", false },
                { "pTextLabel", "String", "textlabel, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text", false },
            }}, // [AUTO-FIXED PARAMS]
            { "MsgSystem", "Boolean", "sends a system message for the given object", "Global", {
                { "Object", "Alias ( of type measuresimobject)", "The object to which the message will be assigned to.", false },
                { "Message", "String", "The text of the message.", false },
            }},
            { "Name2BuildingType", "Number", "transforms a name of a building type to its number - eg. it returns for \"tavern\" the id 3 (see database table obj_buildign_type)", "Global", {
                { "Name", "String", "Name of the type (eg. \"tavern\", \"smithy\", ...)", false },
            }},
            { "NotifyOnStateChange", "Nil", "Add a notifiing function, if a state from a object changes. The scriptcall is not sheduled and is called right bevor the state change.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to observe", false },
                { "State", "Number", "state to observe", false },
                { "ScriptFilename", "Any", "filename of the script without the starting directory 'scripts'/' (eg. Measures/ms_run.lua)", false },
                { "Function", "Any", "the syntax for this function ist function NotifyFunction(ObjectAlias,StateIdentifier,NewStateValue)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "OSHSetMeasureCost", "Nil", "sets the measure cost for displaying in the Measure Tooltip", "Global", {
                { "Label", "Any", "", false },
                { "Variable Argument List", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "OSHSetMeasureRepeat", "Nil", "sets the measure repeattime for displaying in the Measure OSH", "Global", {
                { "Label", "Any", "", false },
                { "Variable Argument List", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "OSHSetMeasureRuntime", "Nil", "sets the measure runtime for displaying in the Measure OSH", "Global", {
                { "Label", "Any", "", false },
                { "Variable Argument List", "...", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ObjectClearMeasureIcon", "Nil", "clears the objects measure icon to show in renderview and simicons. resets to the default behavior", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to set the measure icon for", false },
            }},
            { "ObjectClearStateIcon", "Nil", "clears the objects state icon to show in renderview. resets to the default behavior", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to set the state icon for", false },
            }},
            { "ObjectSetHighlight", "Boolean", "sets an object highlight", "Global", {
                { "pObject", "Alias ( of type guildobject)", "alias of the given object", false },
                { "State", "Boolean", "highlight switch, true -> on, false -> off", false },
            }},
            { "ObjectSetMeasureIcon", "Nil", "sets the objects measure icon to show in renderview and simicons. Overrides the default behavior so be carefull to cleanup your settings", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to set the measure icon for", false },
                { "IconPath", "Any", "path of the icon texture to show or 0 to go back to default behavior", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ObjectSetStateIcon", "Nil", "sets the objects state icon to show in renderview. Overrides the default behavior so be carefull to cleanup your settings", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to set the state icon for", false },
                { "IconPath", "Any", "path of the icon texture to show or 0 to go back to default behavior", false },
            }}, // [AUTO-FIXED PARAMS]
            { "OfficeGetAccessRights", "Boolean", "returns the acces right of the given sim to the given office", "Global", {
                { "pOffice", "Alias ( of type office)", "", false },
                { "pSim", "Alias ( of type sim)", "", false },
                { "Rights", "Any", "Rights to check (EN_POLL = 1, EN_REELECT = 2, EN_REMOVE = 4, EN_APPLY = 8)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "OfficeGetApplicantCount", "Number", "returns the applicant count for the given office", "Global", {
                { "pOffice", "Alias ( of type office)", "Target Office Alias", false },
            }},
            { "OfficeGetHolder", "Boolean", "pushes the office holder of the given office into the given DestAlias", "Global", {
                { "pOffice", "Alias ( of type office)", "Office Alias", false },
                { "DestAlias", "String", "Given Office Holder Alias", false },
            }},
            { "OfficeGetHolderId", "Number", "returns the id of the office holder", "Global", {
                { "pOffice", "Alias ( of type office)", "Target Office Alias", false },
            }},
            { "OfficeGetLevel", "Number", "retrieves the office level", "Global", {
                { "pOffice", "Alias ( of type office)", "", false },
            }},
            { "OfficeGetShadowApplicantCount", "Number", "returns the shadow dynasty applicant count for the given office", "Global", {
                { "pOffice", "Alias ( of type office)", "Target Office Alias", false },
            }},
            { "OfficeGetTextLabel", "void", "Gets the textlabel of given office", "Global", {
                { "pOffice", "Alias ( of type office)", "pOffice office to check", false },
            }},
            { "OfficeSetBlock", "Boolean", "blocks an office against further applications or depositions (for use while an election takes place)", "Global", {
                { "pOffice", "Alias ( of type office)", "", false },
                { "Block", "Boolean", "", false },
            }},
            { "PenaltyFinish", "Boolean", "Set or clears a sim from the wanted-list of a city. If Time is 0, the sim is removed.", "Global", {
                { "pPenalty", "Alias ( of type penalty)", "penalty to check", false },
            }},
            { "PenaltyGetOffender", "Number", "Get the offender from a penalty", "Global", {
                { "pPenalty", "Alias ( of type penalty)", "penalty to check", false },
                { "ret_pOffender", "Alias ( of type sim)", "the offender on success", false },
            }},
            { "PenaltyGetPrisonTime", "Number", "Get the prison time of a prison penalty", "Global", {
                { "pPenalty", "Alias ( of type penalty)", "penalty to check", false },
            }},
            { "PenaltyGetType", "Number", "get's the type of a penalty", "Global", {
                { "pPenalty", "Alias ( of type penalty)", "penalty to check", false },
            }},
            { "PenaltyReset", "Boolean", "resets a penalty back to the wanted (open) status. city guards will arrest sim with this state, if they get in range to them", "Global", {
                { "pPenalty", "Alias ( of type penalty)", "penalty to check", false },
            }},
            { "PhysicsRand", "Number", "Returns a random integer (0 <= Rand(MaxValue) < MaxValue)", "Global", {
                { "MaxValue", "Number", "Range for Random", false },
            }},
            { "PlayAnimation", "Number", "plays a animation on the object. The functions waits for the end of the animation", "Global", {
                { "pObj", "Alias ( of type sim)", "object", false },
                { "AnimName", "String", "Animation to play", false },
                { "Layer", "Number", "layer for the animation (0 = default)", true },
            }},
            { "PlayAnimationNoWait", "Number", "plays a animation on the object", "Global", {
                { "pObj", "Alias ( of type sim)", "object", false },
                { "AnimName", "String", "Animation to play", false },
                { "Layer", "Number", "layer for the animation (0 = default)", true },
            }},
            { "PlayFE", "Nil", "plays a facial expression", "Global", {
                { "pTarget", "Alias ( of type sim)", "Target Sim Alias", false },
                { "AnimName", "String", "name of the expression", false },
                { "Weight", "Number", "weight of facial expression (1.0f is full weight)", false },
                { "Time", "Number", "runtime", false },
                { "Channel", "Number", "channel to modify", false },
            }},
            { "PlaySound", "Nil", "Plays a single sound which is heared regardless of the position of the camera.", "Global", {
                { "Filename", "String", "The filename starting from \"W:\\TheGuild2\\GameData\\sfx\".", false },
                { "Volume", "Number", "The volume of the sample from 0.0 - 1.0.", false },
                { "LoopCount", "Number", "The loop count of the sound.", false },
                { "Channel", "String", "The soundchannel where the sound should be played.", false },
            }},
            { "PlaySound3D", "Boolean", "Plays a sound on the position of \"SoundSource\".", "Global", {
                { "SoundSource", "Alias ( of type guildobject)", "The source of the sound", false },
                { "Sample", "String", "The filename starting at \"W:\\TheGuild2\\GameData\\sfx\" (i.e. \"Effects/chest_open+0.wav\")", false },
                { "Volume", "Number", "The volume from 0.0 to 1.0 (default 1.0)", true },
            }},
            { "PlaySound3DVariation", "Boolean", "Plays a random sound from the given foldername on the position of object.", "Global", {
                { "SoundSource", "Alias ( of type guildobject)", "The source of the sound", false },
                { "Foldername", "String", "The folder from where the sample will be choosen starting at \"W:\\TheGuild2\\GameData\\sfx\" (i.e. \"Effects/combat_cannon_strike_ground\")", false },
                { "Volume", "Number", "The volume from 0.0 to 1.0 (default 1.0)", true },
            }},
            { "PlaySoundVariation", "Boolean", "Plays a random global sound (regardless of the position of the camera)", "Global", {
                { "Foldername", "String", "The folder from where the sample will be choosen starting at \"W:\\TheGuild2\\GameData\\sfx\" (i.e. \"Effects/combat_cannon_strike_ground\")", false },
                { "Volume", "Number", "The volume from 0.0 to 1.0 (default 1.0)", true },
                { "LoopCount", "Number", "The loop-count (default 1). 0 means infinite loops", true },
                { "Channel", "String", "The channel (default \"c3\")", true },
            }},
            { "PlayerMeasureStarted", "Boolean", "returns if the player hast already started the measure with the given MeasureID !! untested !!", "Global", {
                { "MeasureName", "Any", "name of the relevant measure", false },
            }}, // [AUTO-FIXED PARAMS]
            { "Plunder", "Boolean", "Transfers the valuables from the targets inventory until no room is left", "Global", {
                { "pPlunderer", "Alias ( of type guildobject)", "the plunderer", false },
                { "pTarget", "Alias ( of type guildobject)", "the target", false },
                { "a_iNumbersOfSlots", "Number", "the numbers of slots to transfer (allows consecutive plundering)", false },
            }},
            { "PositionGetArea", "Number", "Returns the area of a position", "Global", {
                { "pPosition", "Alias ( of type position)", "Position", false },
            }},
            { "PositionGetRotation", "Number", "Returns the y-rotation of the given position", "Global", {
                { "Position", "Alias ( of type position)", "The position", false },
            }},
            { "PositionGetVector", "void", "Returns x,y,z values from a position (example: x,y,z = PositionGetVector(\"MyLocator\")", "Global", {
                { "pPosition", "Alias ( of type position)", "Position", false },
            }},
            { "PositionModify", "Boolean", "Modifies a position by DeltaX/DeltaY/DeltaZ", "Global", {
                { "pPosition", "Alias ( of type position)", "Positon to modify", false },
                { "DeltaX", "Number", "x modification", false },
                { "DeltaY", "Number", "y modification", false },
                { "DeltaZ", "Number", "z modification", false },
            }},
            { "PositionMove", "Boolean", "Modifies a position by moving it Distance Unit's to ObjectAlias. If angle is 0 degree's, the position is moving towards the object. If angle is 180 degree's, the position is moving back from the object. 90 degrees move's right and 270 to the left", "Global", {
                { "pPosition", "Alias ( of type position)", "Positon to modify", false },
                { "Distance", "Number", "the distance to move", false },
                { "ObjectAlias", "Alias ( of type guildobject) or Alias ( of type position)", "the destination of the movement", false },
                { "Angle", "Any", "the angle of the movement 0...360� (0 -> towards to object, 90 -> right, 180 -> back, 270 -> left)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "PositionSetArea", "Boolean", "Sets a area of a position", "Global", {
                { "pPosition", "Alias ( of type position)", "Position", false },
                { "Area", "Number", "move area", false },
            }},
            { "PositionSetVector", "Boolean", "Sets x,y,z for a position", "Global", {
                { "pPosition", "Alias ( of type position)", "Position", false },
                { "x", "Number", "x-position", true },
                { "y", "Number", "y-position", true },
                { "z", "Number", "z-position", true },
            }},
            { "ProfessionGetLabel", "String", "Gets the label of an item.", "Global", {
                { "Profession", "Any", "database id or name of an item", false },
                { "Gender", "Any", "true: singular false: plural", true },
            }}, // [AUTO-FIXED PARAMS]
            { "QuestTalk", "Boolean", "Executes the internal quest talk between two sims", "Global", {
                { "pTalker", "Alias ( of type sim)", "the initiator of the quest talk", false },
                { "pTarget", "Alias ( of type sim)", "the talk partner", false },
            }},
            { "ReadyToRepeat", "Boolean", "Check's if the timer event 'Name' still exists for this object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to check", false },
                { "TimerName", "String", "of the timer event", false },
            }},
            { "ReleaseAvoidanceGroup", "Boolean", "the avoidance group the sim is belonging to is released. This affects all sims of this group.", "Global", {
                { "in_pSim", "Alias ( of type sim)", "an arbitrary sim of an avoidance group", false },
            }},
            { "ReleaseLocator", "Boolean", "Releases a locator from the use of pObject.", "Global", {
                { "pObject", "Alias ( of type simobject)", "the SimObject who has blocked this locator", false },
                { "Position", "Alias ( of type position)", "the position to release (block by BlockLocator). if no position is specified all blocked locators form pObject are freed", true },
            }},
            { "RemoveAlias", "Boolean", "Removes an alias from the measure.", "Global", {
                { "AliasName", "String", "The name of the alias which should be removed.", false },
            }},
            { "RemoveAllObjectDependendImpacts", "Boolean", "remove all impacts from pObject that were caused by pObjectDependency", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object that will hold the impact", false },
                { "iObjectDependency", "Alias ( of type simobject) or Number", "the object that caused the impact", false },
            }},
            { "RemoveAllOverheadSymbols", "Boolean", "removes all existing overhead symbols from an object - regardless who creates the overhead symbol", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to show the overhead symbol for", false },
            }},
            { "RemoveData", "Nil", "Remove arbitrary data from the measure.", "Global", {
                { "DataName", "String", "The unique name for the data.", false },
            }},
            { "RemoveDialogBox", "Boolean", "closes the dialogbox of the given object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "", false },
            }},
            { "RemoveEmptySlots", "Number", "Remove all empty slots from the inventory", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to remove items from (cl_Building, cl_Sim, cl_Cart, ...)", false },
                { "InventoryType", "Any", "of the inventory to remove the items from (INVENTORY_STD (default)/ INVENTORY_SELL / INVENTORY_EQUIPMENT)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "RemoveEvidences", "Nil", "removes all evidences about a given actor from target's memory", "Global", {
                { "pTarget", "Alias ( of type sim)", "Target Sim Alias", false },
                { "pActor", "Alias ( of type sim)", "Actor Sim Alias", false },
            }},
            { "RemoveImpact", "Boolean", "Removes the given impact from the given object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "The object whose impact will be removed", false },
                { "ImpactType", "String or Number", "The impact that will be removed", false },
            }},
            { "RemoveImpactDependendImpacts", "Boolean", "removes all impacts depending on 'DependendImpactID' and the impact 'DependendImpactID' itself", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the object that will hold the impact", false },
                { "DependendImpactID", "Number", "ID or name of impact in gl_impact_type table", false },
            }},
            { "RemoveItems", "Number", "Remove items from a objects inventory.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to remove items from (cl_Building, cl_Sim, cl_Cart, ...)", false },
                { "Item", "Number or String", "database id or name of the item", false },
                { "Count", "Number", "count of items to remove", false },
                { "InventoryType", "Any", "of the inventory to remove the items from. If no inventorytype is specified the standardt inventory will be used", false },
            }}, // [AUTO-FIXED PARAMS]
            { "RemoveOverheadSymbol", "Boolean", "removes an existing overhead symbol. The symbol will be identified by the given OverheadSymbolID", "Global", {
                { "pIDDataName", "String", "data name of the overheadsymbol ID (attached at the measure via SetData)", false },
            }},
            { "RemoveOverheadSymbols", "Boolean", "removes all permanent overhead symbols from an object, that was set by the current script", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to show the overhead symbol for", false },
            }},
            { "RemoveProperty", "Boolean", "Removes the property from the given object.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose property should be removed.", false },
                { "PropertyName", "String", "The property which should be removed.", false },
            }},
            { "RemoveScriptcall", "Boolean", "removes a scriptcall by name (attention, this removes ALL SCRICPTCALLS WITH THIS NAME ON EVERY OBJECT)", "Global", {
                { "Name", "Any", "Name for the scriptcall (for queriing deleting this call eg.)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ResetGamespeed", "Nil", "resets the gamespeed to normal", "Global", {}}, // No params
            { "ResetMouseCursorTexture", "Boolean", "resets the mouse cursor to the default cursor", "Global", {}}, // No params
            { "ResetProcessProgress", "Nil", "disables and resets the progressbars.", "Global", {
                { "Nil", "...", "void -", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ResetQuest", "Nil", "resets the quest specified with pQuestname and QuestSource. If QuestSource is 0 all Quest with pQuestname will be reseted. If both are 0, the current quest will be reseted. Remember: though the quest will be reseted, the ingame impacts of the quest via runtime can not be reseted, so they have to be undone in the questscript.", "Global", {
                { "pQuestname", "String", "questname", false },
                { "pQuestSourceAlias", "Alias ( of type guildobject)", "Questsource Alias", false },
            }},
            { "ResourceCanBeChanged", "Boolean", "returns true, if the resource can be sowed", "Global", {
                { "pResource", "Alias ( of type guildresource)", "pResource resource alias", false },
            }},
            { "ResourceFind", "void", "Searches for the nearest resource of a specific item type", "Global", {
                { "pObject", "Alias ( of type guildobject)", "the startpoint of the search (Sim/DynastySim/Cart/...)", false },
                { "Item", "Number or String", "Type of Item to search (int: datbase id string: itemname) (-1 for all)", false },
                { "o_pResource", "Alias ( of type guildresource)", "the resource", false },
                { "CanBeSowed", "Boolean", "false: search only existant resources true: search all resources that can be changed to iItemId", false },
            }},
            { "ResourceGetEntry", "Number", "returns the entry id for the given item in the resource", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
                { "Item", "Number or String", "Type of Item to search (int: datbase id string: itemname) (-1 for all)", false },
            }},
            { "ResourceGetItemId", "Number", "returns the item id related to the given resource", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
            }},
            { "ResourceGetLevel", "Number", "returns the item id related to the given resource", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
            }},
            { "ResourceGetMeasureID", "String", "returns the measure what is needed to start resource growing on this resource", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
            }},
            { "ResourceGetScriptFunc", "String", "returns the script function linked with a resource (from database)", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
            }},
            { "ResourceGetTypeCount", "void", "returns the count of the possible resource type of this resource", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
            }},
            { "ResourceGetTypeItem", "void", "returns the item of the type iNumber", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
                { "iNumber", "Any", "Number of the type (0<=iNumber<ResourceGetTypeCount() )", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ResourceSow", "Boolean", "sow's the resource iNumber at pResource", "Global", {
                { "pResource", "Alias ( of type guildresource)", "resource alias", false },
                { "iNumber", "Any", "Number of the type (0<=iNumber<ResourceGetTypeCount() )", false },
            }}, // [AUTO-FIXED PARAMS]
            { "RoomGetInsideSimList", "Nil", "returns a list of all sims inside a room", "Global", {
                { "pRoom", "Alias ( of type room)", "", false },
                { "ListAlias", "Any", "alias of a list of sims", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SatisfyNeed", "Boolean", "Satisfy a need for a sim.", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to satisfy his needs", false },
                { "NeedId", "Number", "the id of the need the satisfy", false },
                { "Value", "Number", "the value of the satisfy ( 1 = full satisfy)", false },
            }},
            { "ScenarioCreatePosition", "Boolean", "Creates a outdoor position (eg. for creating a sim/chest)", "Global", {
                { "xpos", "Number", "x position", false },
                { "zpos", "Number", "y position", false },
                { "o_Position", "Alias ( of type position)", "the created position", false },
            }},
            { "ScenarioCreateResource", "Boolean", "creates a treasure chest on the position PositionAlias", "Global", {
                { "Position", "Alias ( of type position)", "Position", false },
                { "ResourceName", "String", "name or id of the resource (found in database table gl_guildresource)", false },
                { "o_ChestAlias", "Alias ( of type building)", "the created chest", false },
            }},
            { "ScenarioGetObjectByName", "Boolean", "Get an object from the current world. To find the object the class and it's name is used", "Global", {
                { "ClassName", "String", "class of the object (cl_Settlement, cl_Sim, cl_Cart, cl_Building, ...)", false },
                { "ObjectName", "String", "Name of the object", false },
                { "o_Object", "String", "the found object", false },
            }},
            { "ScenarioGetObjects", "Number", "Get all objects from a class from the world. The Objects are returned in the aliases o_Object0 ... o_ObjectN). For example: ScenarioGetObjects(\"Settlement\", 10, \"City\") gets all settlements and returns it in the aliasse \"City0\" ... \"City9\". The count of the aliasses are returned by this function. Attention: You should use this function carefully, because you can get all objects from a big class like \"cl_Sim\" and got a hole bunch of data.", "Global", {
                { "ClassName", "String", "class of the object (cl_Settlement, cl_Sim, cl_Cart, cl_Building, ...)", false },
                { "MaxObjects", "Number", "Max count of objects to return", false },
                { "o_Object", "String", "the found object's", false },
            }},
            { "ScenarioGetRandomObject", "Boolean", "Get a random object from the current world. To find the object the class is used", "Global", {
                { "ClassName", "String", "class of the object (cl_Settlement, cl_Sim, cl_Cart, cl_Building, ...)", false },
                { "o_Object", "String", "the found object", false },
            }},
            { "ScenarioGetTimePlayed", "Number", "gets the time played (in hours) from the world", "Global", {}}, // No params
            { "ScenarioGetUnspawnedEnemiesCount", "Boolean", "returns the number of unspawned ai dynasties", "Global", {}}, // No params
            { "ScenarioGetYearsPerRound", "Number", "Get the years increment per round (default value is 4 years per round)", "Global", {}}, // No params
            { "ScenarioPauseAI", "Boolean", "pauses or unpauses the ai of the game", "Global", {
                { "Pause", "Boolean", "true: pause the au / false: unpause the ai", false },
            }},
            { "ScenarioSetNameLanguage", "Nil", "sets the language of the sim names in the current map. Values can be a language(in english) which has to be the same as the language tab in the names.txt. If the call to this is missing english will be used as default language", "Global", {
                { "Language", "Any", "language specifier (eg German, Englisch etc)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ScenarioSetOutdoorScrollBoundaries", "Boolean", "Sets the outdoor scroll boundaries of the scenario. The points must build a quad, where the points 1,2,3,4 are sorted clockwise", "Global", {
                { "X1", "Any", "x-coordinate", false },
                { "Z1", "Any", "z-coordinate", false },
                { "X2", "Any", "x-coordinate", false },
                { "Z2", "Any", "z-coordinate", false },
                { "X3", "Any", "x-coordinate", false },
                { "Z3", "Any", "z-coordinate", false },
                { "X4", "Any", "x-coordinate", false },
                { "Z4", "Any", "z-coordinate", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ScenarioSetYearsPerRound", "Boolean", "Set the years increment per round (default value is 4 years per round)", "Global", {
                { "YearsPerRound", "Number", "YearsPerRound", false },
            }},
            { "ScenarioStopUsingOutdoorScrollBoundaries", "Boolean", "Disabling the outdoor scroll boundaries", "Global", {}}, // No params
            { "ScriptOnly_StopProduction", "Nil", "stops the intern production of a sim. This function is designed for use in the production measure only.", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to start the production", false },
            }},
            { "SendCommand", "Boolean", "Sends a command to an object. The script will wait for the finish of the command to continue.", "Global", {
                { "TargetName", "String", "The object to which the command will be sent.", false },
                { "CommandName", "String", "The name of the command.", false },
                { "FilterString", "String", "An optional filter can be providede here.", false },
            }},
            { "SendCommandNoWait", "Boolean", "Sends a command to an object while proceeding with the script.", "Global", {
                { "TargetName", "String", "The object to which the command will be sent.", false },
                { "CommandName", "String", "The name of the command.", false },
                { "FilterString", "String", "An optional filter can be providede here.", false },
            }},
            { "SetActiveAvoidance", "Boolean", "sets avoidance behavior such that the sim will avoid other sims", "Global", {
                { "pSim", "Alias ( of type sim)", "bool: holdPositionMode on/off", false },
            }},
            { "SetAppearance", "Nil", "sets a sims graphical appearance", "Global", {
                { "pTarget", "Alias ( of type sim)", "", false },
                { "Type", "Number", "type of appearance to set ( 1 - Hat )", false },
                { "ID", "Number", "id of entry in charmodel.dbt", false },
            }},
            { "SetAvoidanceGroup", "Boolean", "sets an internal state such that a group of sims is treated like a large single circular object for avoidance computations", "Global", {
                { "Variable Argument List", "...", "of cl_Sim aliases", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetAvoidanceRange", "Boolean", "sets the size of the circle that is used for avoidance compuations", "Global", {
                { "pSim", "Alias ( of type sim)", "", false },
                { "Range", "Number", "new collision range value, -1 for default value", false },
            }},
            { "SetCampaignEndText", "Boolean", "placeholder for mission end text", "Global", {
                { "pObject", "Alias ( of type guildobject)", "", false },
            }},
            { "SetContext", "Boolean", "Sets the context of the given object", "Global", {
                { "MovingSimObject", "Alias ( of type movingsimobject)", "The object whose context will be set", false },
                { "Context", "String", "The new context", false },
            }},
            { "SetData", "Nil", "Attaches arbitrary data to the measure.", "Global", {
                { "DataName", "String", "The unique name for the data.", false },
                { "Data", "Number or String", "The actually data.", false },
            }},
            { "SetDestroyed", "Boolean", "Set the destroyed status of an object", "Global", {
                { "pObject", "Alias ( of type simobject)", "object to destroy", false },
            }},
            { "SetExclusiveMeasure", "Boolean", "Specifys the only available Measure by ID for the given AliasObject. The mode specifys if the measure is exclusive for starting by the owner(EN_ACTIVE), to use the object as measure target (EN_PASSIVE) or both (EN_BOTH).", "Global", {
                { "pObject", "Alias ( of type measuresimobject)", "Object to modify", false },
                { "MeasureID", "Number or String", "Measure to set exclusive (int: database id string: measure name)", false },
                { "Mode", "Number", "EN_PASSIVE/EN_ACTIVE/EN_BOTH", false },
            }},
            { "SetFavorToDynasty", "Number", "Sets the favor from pObject to pTargetDynasty. If pObject is a dynasty-sim the favor from the dynasty of pObject to pTargetDynasty will be set.", "Global", {
                { "pObject", "Alias ( of type sim pobject) or Alias ( of type dynasty)", "The sim or dynasty whose favor will be set. If it is a dynasty-sim the favor of the dynasty of it will be set.", false },
                { "pTargetDynasty", "Alias ( of type dynasty)", "The dynasty to which the favor will be set.", false },
            }},
            { "SetFavorToSim", "Number", "Sets the favor from pObject to pTargetSim. If pTargetSim is a dynasty-sim the favor to the dynasty will be set. If pObject is a dynasty-sim the favor from the dynasty of pObject will be set.", "Global", {
                { "pObject", "Alias ( of type sim pobject) or Alias ( of type dynasty)", "The sim or dynasty whose favor will be set. If it is a dynasty-sim the favor of the dynasty of it will be set.", false },
                { "pTargetSim", "Alias ( of type sim)", "The sim to which the favor will be set.", false },
                { "Favor", "Number", "The amount of favor which will be set.", false },
            }},
            { "SetHomeBuilding", "Boolean", "Set's the home building for a object. If the object is a worker, his worker-hut is return, if it is a cart, it's worker building is returned. A Dynasty sim returns it's residence.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object (cl_Sim/cl_Cart)", false },
                { "pHome", "Alias ( of type building)", "the new home building", false },
            }},
            { "SetInit", "Nil", "Sets the measure object to the \"init\" state.", "Global", {}}, // No params
            { "SetMainQuest", "Nil", "attaches the calling quest as subquest to the given mainquest. Needed for Questbook view", "Global", {
                { "MainQuestname", "Any", "Questname", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetMainQuestDescription", "Nil", "sets the given main quests description. Creates a new main quest if not allready existing. Needed for Questbook view", "Global", {
                { "MainQuestname", "Any", "Questname", false },
                { "DescLabel", "Any", "Label of the Questdescription", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetMainQuestTitle", "Nil", "sets the given main quests title. Creates a new main quest if not allready existing. Needed for Questbook view", "Global", {
                { "MainQuestname", "Any", "Questname", false },
                { "TitleLabel", "Any", "Label of the questtitle", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetMeasureRepeat", "Boolean", "Sets the standard measure repeat timer for this measure. If no destination is specified, the timer is global for executing this measure. If a destination is specified the measure is only forbidden with the destination as target.", "Global", {
                { "TimeInHours", "Any", "Time to wait till the next execution of this measure", false },
                { "pDestination", "Alias ( of type simobject)", "Destination object for the timer.", true },
            }}, // [AUTO-FIXED PARAMS]
            { "SetMouseCursorTexture", "Boolean", "changes the mouse cursor", "Global", {
                { "pTexture", "String", "", false },
            }},
            { "SetName", "Number", "Set's the name (label) for a guild object (Building/Sim/Cart/Settlement/...). If the object is a sim, the lastname is set.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to set the name for", false },
                { "Label", "String", "label to set", false },
            }},
            { "SetNobilityTitle", "Number", "Starts the grant of a new nobility title of the given sim", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim whose nobility title will be set", false },
                { "NobilityTitle", "Number", "The new nobility title", false },
                { "Instantly", "Boolean", "True (default) if the title should be given instantly, False if the title should be given within a certain time as described in the game design", true },
            }},
            { "SetOfficePrivileges", "Boolean", "Sets the privilegeVector of an office", "Global", {
                { "pOffice", "Alias ( of type office)", "pOffice", false },
                { "Variable Argument List", "...", "of privileges (cl_String)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetOfficeServants", "Boolean", "Sets the actual servants for a special office and behaviour", "Global", {
                { "pOffice", "Alias ( of type office)", "", false },
                { "Behaviour", "String", "", false },
                { "Count", "Number", "", false },
                { "Profession", "Number", "", false },
            }},
            { "SetPassiveAvoidance", "Boolean", "sets if other sims evade this sim", "Global", {
                { "pSim", "Alias ( of type sim)", "", false },
                { "bEnable", "Boolean", "bool: holdPositionMode on/off", false },
            }},
            { "SetPosition", "Boolean", "Set's the position of the given object.", "Global", {
                { "pObject", "Alias ( of type simobject) or Alias ( of type position) or Alias ( of type i_object3d)", "Object to set the position. Can be cl_Position/cl_SimObject/i_Object3D", false },
                { "x", "Number", "x-value", false },
                { "y", "Number", "y-value", false },
                { "z", "Number", "z-value", false },
            }},
            { "SetProcessMaxProgress", "Nil", "sets a free settable value of maximum progress of a process.", "Global", {
                { "Number", "...", "float fMaxProgressValue", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetProcessProgress", "Nil", "sets a free settable value of the current progress of a process (and must always be smaller than MaxProgress!).", "Global", {
                { "Number", "...", "float fProgressValue", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetProperty", "Boolean", "Sets the given value as a property to the object regardless if it already exists or not.", "Global", {
                { "Object", "Alias ( of type corenode)", "The object whose property will be set.", false },
                { "PropertyName", "String", "The name of the property.", false },
                { "Value", "Number or String", "The value of the property which is either a number or a string", false },
            }},
            { "SetQuestDescription", "Nil", "sets the quest description. pDesc must be a textlabel which will be localized via the textsystem.", "Global", {
                { "pDesc", "String", "textlabel", false },
                { "Replace", "Boolean", "if true the questdescription will be replaced, else the text will be apended to the questdescription", false },
                { "Variable Argument List", "...", "..., variable argument list for the localized text", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetQuestDescriptionByQuestname", "Boolean", "sets the quest description. pQuestname must be a name of an allready triggered quest or \"\" for this quest, pDesc must be a textlabel which will be localized via the textsystem.", "Global", {
                { "pQuestname", "String", "questname or \"\" for this quest", false },
                { "pActor", "Alias ( of type sim)", "textlabel", false },
                { "pDesc", "String", "if true the questdescription will be replaced, else the text will be apended to the questdescription", false },
                { "Replace", "Boolean", "..., variable argument list for the localized text", false },
            }},
            { "SetQuestDestination", "Nil", "sets the destination object of the running quest", "Global", {
                { "pDestinationAlias", "Alias ( of type guildobject)", "destination alias", false },
            }},
            { "SetQuestSource", "Nil", "sets the source object of the running quest", "Global", {
                { "pSourceAlias", "Alias ( of type guildobject)", "source alias", false },
            }},
            { "SetQuestTitle", "Nil", "sets the quest title. pTitle must be a textlabel which will be localized via the textsystem.", "Global", {
                { "pTitle", "String", "textlabel", false },
            }},
            { "SetReady", "Nil", "Sets the measure object to the \"ready\" state.", "Global", {}}, // No params
            { "SetRepeatTimer", "Boolean", "Create a timer event for the object. The event exists for the object for the given time. After the time is elapsed, the timer object is automatically destroyed.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object to create the named timer for", false },
                { "TimerName", "String", "Name of the timer", false },
                { "TimeInHours", "Number", "time to elapse (in hours)", false },
            }},
            { "SetRoomAnimationTime", "Boolean", "stops an object's animation in the given room of the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "cl_Building* pBuilding -", false },
                { "Room", "String", "name of the room upgrade or \"\" for the mainroom", false },
                { "Object", "String", "const char* Object -", false },
                { "Time", "Number", "Time Offset of the animation", false },
            }},
            { "SetSkillValue", "Number", "Sets the given skill value which is one of (CONSTITUTION, DEXTERITY, CHARISMA, FIGHTING, CRAFTSMANSHIP, SHADOW_ARTS, RHETORIC, EMPATHY, BARGAINING, SECRET_KNOWLEDGE) to the given sim", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim whose skill will be set to the new value", false },
                { "SkillNumber", "Number", "Skill to alter", false },
                { "Value", "Number", "The new value", false },
            }},
            { "SetState", "Boolean", "Set or clears a status of an object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to modify", false },
                { "State", "Number", "state to set (STATE_IDLE, STATE_REPAIRING, STATE_BURNING, ...)", false },
                { "Value", "Boolean", "true:set status false:clear status", false },
            }},
            { "SetStateImpact", "Boolean", "Set's a state depended impact. This function is only valid in a state script", "Global", {
                { "StateImpact", "String", "state impact to set (\"no_idle\", ...)", false },
            }},
            { "SetTime", "Boolean", "Set the current game time.", "Global", {
                { "Season", "Any", "EN_SEASON_SPRING/EN_SEASON_SUMMER/EN_SEASON_AUTUMN/EN_SEASON_WINTER", false },
                { "Year", "Number", "Year", false },
                { "Hour", "Number", "Hour (0...23)", false },
                { "Minute", "Number", "Minute (0...59)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SetTitlePrivileges", "Boolean", "Sets the privilegeVector of a title holder", "Global", {
                { "TitleHolder", "Any", "The sim or dynasty which holds the title", false },
                { "Variable Argument List", "...", "of privileges (cl_String)", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SettlementEventGetTime", "Number", "returns the time when a settlement event will happen", "Global", {
                { "pSettlementEvent", "Alias ( of type settlementevent)", "", false },
            }},
            { "SettlementGetOffice", "Nil", "returns an office", "Global", {
                { "pSettlement", "Alias ( of type settlement)", "", false },
                { "Level", "Number", "", false },
                { "Id", "Number", "", false },
                { "Alias", "String", "", false },
            }},
            { "SettlementGetOfficeCnt", "Number", "returns the number of offices in a given level", "Global", {
                { "pSettlement", "Alias ( of type settlement)", "", false },
                { "Level", "Number", "", false },
            }},
            { "SettlementGetOfficeHolder", "Boolean", "retrieves an office holder", "Global", {
                { "pSettlement", "Alias ( of type settlement)", "", false },
                { "Level", "Number", "", false },
                { "Id", "Number", "", false },
                { "Result", "String", "", false },
            }},
            { "SettlementGetOfficeLevelCnt", "Number", "returns the number of officelevels in the settlements office tree", "Global", {
                { "pSettlement", "Alias ( of type settlement)", "", false },
            }},
            { "ShowBoundingBox", "Boolean", "Shows the bounding box of an object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to show the box for", false },
                { "Show", "Boolean", "true:show box false:hide box", false },
            }},
            { "ShowBuildingFlags", "Nil", "Shows or hides the flags of a building", "Global", {
                { "Building", "Any", "The building whose flags should be shown or hidden", false },
                { "Show", "Boolean", "True to show the flags, false to hide them", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ShowMap", "Nil", "shows/hides the hud map sheet ?? untested ??", "Global", {
                { "Show", "Boolean", "true -> show the map, hide -> hide the map", false },
            }},
            { "ShowObject", "Boolean", "Show's a 3d object in the scene of pObject", "Global", {
                { "pObject", "Alias ( of type simobject)", "an object to identify the scene for hiding the object", false },
                { "ObjectName", "String", "Name of the object to show", false },
            }},
            { "ShowOverheadMeasureSymbol", "Boolean", "shows the symbol of the current measure", "Global", {
                { "pObject", "Alias ( of type guildobject)", "", false },
            }},
            { "ShowOverheadSymbol", "Boolean", "shows an overhead symbol for the given object.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "Object Alias", false },
                { "Fixed", "Boolean", "(true: fixed position false: fly out)", false },
                { "Force", "Boolean", "independ from the dynasty", false },
                { "pIDDataName", "String", "Data name of the OverheadSymbol ID, can be get via GetData()", false },
                { "pText", "String", "textlabel for the overhead symbol, will be localized via the textsystem", false },
                { "Variable Argument List", "...", "variable argument list for the localized text", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ShowPanel", "Nil", "Shows a panel in the multifunctional display. The script will wait for the movement to be finished.", "Global", {
                { "PanelName", "String", "The name of the panel to be displayed.", false },
            }},
            { "ShowPanelNoWait", "Nil", "Shows a panel in the multifunctional display.", "Global", {
                { "PanelName", "String", "The name of the panel to be displayed.", false },
            }},
            { "ShowStatistics", "Nil", "Shows the statistics screen for the player dynasty. Note that the game will pause !!!", "Global", {}}, // No params
            { "ShowTutorialBox", "String", "Shows a tutorial box specified by the given arguments, forces the script to wait for user input in the tutorial box, so be shure to add buttons on which the user can click or use ShowTutorialBoxNoWait", "Global", {
                { "X", "Number", "X position relativ to 1024", false },
                { "Y", "Number", "Y Position relativ to 768", false },
                { "Width", "Number", "Width, -1 for default width", false },
                { "Height", "Number", "Height, -1 for default height", false },
                { "ResolutionMode", "Number", "specifies if the panel should change its position at higher screen resolutions.", false },
                { "Corner", "Any", "specifies the corner and if a arrow will be displayed, valid values are:", false },
                { "PanelParam", "Any", "panel related parameters", false },
                { "HeaderText", "Any", "Header Text Label", false },
                { "BodyText", "Any", "Body Text Label", false },
                { "Picture", "Any", "optional Picture", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for headerlabel, bodylabel and optional buttons", false },
            }}, // [AUTO-FIXED PARAMS]
            { "ShowTutorialBoxNoWait", "Boolean", "Shows a tutorial box specified by the given arguments", "Global", {
                { "X", "Number", "X position relativ to 1024", false },
                { "Y", "Number", "Y Position relativ to 768", false },
                { "Width", "Number", "Width, -1 for default width", false },
                { "Height", "Number", "Height, -1 for default height", false },
                { "ResolutionMode", "Number", "specifies if the panel should change its position at higher screen resolutions.", false },
                { "Corner", "Any", "specifies the corner and if a arrow will be displayed, valid values are:", false },
                { "HeaderText", "Any", "Header Text Label", false },
                { "BodyText", "Any", "Body Text Label", false },
                { "Picture", "Any", "optional Picture", false },
                { "Variable Argument List", "...", "variable argument list. Will be shared for headerlabel, bodylabel and optional buttons", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SimAddCourtingProgress", "Boolean", "Lets the last courting action take effect on the progress", "Global", {
                { "Sim", "Alias ( of type sim)", "The courting sim", false },
            }},
            { "SimAddDate", "Boolean", "adds a date to a sims internal date planer (the datebook that is visible to the player is modified by SimAddDatebookEntry()", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim who receives the date", false },
                { "pBuilding", "Alias ( of type building)", "the building the sim is invited to", false },
                { "EventName", "Any", "(Debug) name describing the event", false },
                { "Time", "Number", "the moment when the date starts", false },
                { "MeasureName", "Any", "(optional) name of the measure that is called when its time to move to the destination, default is \"Attend\"", true },
            }}, // [AUTO-FIXED PARAMS]
            { "SimAddDatebookEntry", "void", "Adds a new entry to the datebook of a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim for which the datebook entry is added", false },
                { "GuildTime", "Number", "the date", false },
                { "pBuilding", "Alias ( of type building)", "the building where the date will happen", false },
                { "HeaderLabel", "Any", "Header of the datebook entry", false },
                { "TextLabel", "Any", "Message body of the datebook entry", false },
                { "Variable Argument List", "...", "variable argument list for text arguments", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SimApplyDeposition", "Boolean", "apply the deposition of a sim from office", "Global", {
                { "pApplicant", "Alias ( of type sim)", "the sim who submits the deposition", false },
                { "pSim", "Alias ( of type sim)", "the sim whose office is in question", false },
            }},
            { "SimArrangeLiaison", "Boolean", "Sets the SecondSim as new liaison lover for the FirstSim", "Global", {
                { "FirstSim", "Alias ( of type sim)", "The first sim", false },
                { "SecondSim", "Alias ( of type sim)", "The second sim", false },
            }},
            { "SimBeamMeUp", "Nil", "Beams a sim immediatly to a given position", "Global", {
                { "pSim", "Alias ( of type sim)", "to beam", false },
                { "pNode", "Alias ( of type corenode)", "destination", false },
                { "StopAllMeasures", "Boolean", "true: stop all measure (default)", true },
            }},
            { "SimCanBeCharged", "void", "checks if a sim is already charged, currently involved in a trial or imprisoned", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to check", false },
            }},
            { "SimCanStartCourtLover", "Boolean", "Checks if pSim can start a new affair with pCourtSim", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim who wants to start the affair", false },
                { "pCourtSim", "Alias ( of type sim)", "the target sim", false },
            }},
            { "SimCanWorkHere", "Number", "check's, if a sim can work in this building", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
                { "pBuilding", "Alias ( of type building)", "working building", false },
            }},
            { "SimChargeCharacter", "Boolean", "use the first evidence in memory for accuse", "Global", {
                { "pAccusor", "Alias ( of type sim)", "the sim who accuses", false },
                { "pAccused", "Alias ( of type sim)", "the sim who is accused", false },
            }},
            { "SimConsumeGoods", "Boolean", "Let the sim consume a good according to its need", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim", false },
            }},
            { "SimCreate", "Boolean", "This function creates a new sim based on an database template specified by TemplateDBId", "Global", {
                { "TemplateDBId", "Number", "database id for a character template", false },
                { "pHome", "Alias ( of type building)", "city to which the sim belongs to", false },
                { "pPosition", "Alias ( of type: position/cart/building/settlement)", "position of the sim to create (spawnpoint)", false },
                { "o_Sim", "Alias ( of type sim)", "the new created sim on succes", false },
            }},
            { "SimCreateCompanion", "Number", "creates a companion animal", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim that creates the animal and its future owner", false },
                { "CompanionName", "String", "Selects the type of companion", false },
                { "Duration", "Number", "Time the companion will last", false },
            }},
            { "SimDoCourtingAction", "Boolean", "Does the given action to the courted lover", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim who does the action", false },
            }},
            { "SimFindExploreTarget", "void", "Find a near unexplored position for exploring", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim for exploring", false },
                { "pCenter", "Alias ( of type position) or Alias ( of type simobject)", "center of exploring area", false },
                { "Radius", "Number", "Radius to explore", false },
                { "o_Position", "Alias ( of type position)", "a unexplored moveable point", false },
            }},
            { "SimFindProduction", "void", "searches and sets a new production for a worker and returns true, if a new production is found", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "SimFindSquad", "void", "Get's a free squad for the sim, that he can join", "Global", {
                { "pSim", "Alias ( of type sim)", "object to search for a free squad place", false },
                { "ret_Squad", "Alias ( of type aisquad)", "the squad on success", false },
            }},
            { "SimGetAge", "Number", "Returns the age of the sim", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim whose age will be returned", false },
            }},
            { "SimGetAlignment", "Number", "Get the alignment from a sim. The alignment begins at 0 (good) and ends with 100 (evil)", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimGetAssignedArea", "Number", "returns the assigned area of a sim (eg. the current working place )", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
                { "o_Area", "Alias ( of type simobject)", "the assigned area (cl_Building/cl_Terrain)", false },
            }},
            { "SimGetAssignedAreaID", "Number", "returns the assigned area id of the sim.", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
            }},
            { "SimGetBehavior", "String", "Gets the behavior of a sim.", "Global", {
                { "SimAlias", "String", "Sim", false },
            }},
            { "SimGetBeloved", "Boolean", "Get the beloved of the sim", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to get the beloved", false },
                { "o_Beloved", "Alias ( of type sim)", "the beloved on success", true },
            }},
            { "SimGetBestEvidence", "Boolean", "finds the best evidence a sim has and returns it in o_Event", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim of the memory to search", false },
                { "Severity", "Number", "min severity of the evidence", false },
                { "o_Evidence", "Alias ( of type simmemoryevent)", "the evidence on success", false },
            }},
            { "SimGetChild", "Boolean", "Get a childfather from a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
                { "Number", "Number", "Number of the child (0...SimGetChildCount())", false },
                { "o_Spouse", "Alias ( of type sim)", "the father on success", false },
            }},
            { "SimGetChildCount", "Number", "Get a count of the childs from a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimGetChurch", "Boolean", "Returns the church a sim is currently linked to", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim", false },
            }},
            { "SimGetCityOfOffice", "void", "gets the settlement that is related to the office of a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "alias of the sim", false },
                { "CityAlias", "Alias ( of type settlement)", "new alias for the settlement", false },
            }},
            { "SimGetClass", "Number", "Get's the character class of a sim which is one of (1 - GL_CLASS_PATRON, 2 - GL_CLASS_ARTISAN, 3 - GL_CLASS_SCHOLAR, 4 - GL_CLASS_FIGHTER, 5 - GL_CLASS_TRADESMAN, 6 - GL_CLASS_NPC)", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to query", false },
            }},
            { "SimGetCourtLover", "Boolean", "Gets the court lover of the given sim", "Global", {
                { "SimAlias", "Any", "The sim", false },
                { "LoverAlias", "Any", "The new alias for the court lover", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SimGetCrimeList", "Nil", "creates a list of all crimes a sim has committed", "Global", {
                { "pOffender", "Alias ( of type sim)", "the sim who has committed the crimes", false },
                { "EventList", "Alias ( of type corenode)", "the alias of the created list", false },
            }},
            { "SimGetCutscene", "Boolean", "get the cutscene a sim is currently part of. Must have been set by CutsceneAddSim() before.", "Global", {
                { "pSim", "Alias ( of type sim)", "", false },
                { "CutsceneAlias", "Any", "", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SimGetDivorced", "Number", "Divorce both given sims", "Global", {
                { "Sim", "Alias ( of type sim)", "The first married sim", false },
                { "Spouse", "Alias ( of type sim)", "The second married sim", false },
            }},
            { "SimGetFaith", "Number", "Returns the Faith of the sim (range 0..100)", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim", false },
            }},
            { "SimGetFather", "Boolean", "Get the father from a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
                { "o_Spouse", "Alias ( of type sim)", "the father on success", false },
            }},
            { "SimGetFavourableCourtingAction", "Number", "Get the best measure for progressing the liasion", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to get the measure for", false },
            }},
            { "SimGetGender", "Number", "gets the gender of the sim", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
            }},
            { "SimGetHandsel", "Number", "get the handsel for the sim for hiring in the place", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to hire", false },
                { "pWorkingPlace", "Alias ( of type guildobject)", "the working place", false },
            }},
            { "SimGetLastname", "String", "Gets the family name of a sim.", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
            }},
            { "SimGetLevel", "Number", "returns the level of the pSim", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim", false },
            }},
            { "SimGetLiason", "Boolean", "Gets the liaison sim of the given sim", "Global", {
                { "SimAlias", "Any", "The sim", false },
                { "LiaisonAlias", "Any", "The new alias for the liaison sim", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SimGetMaxOfficeLevel", "Number", "gets the maximal office level a sim had occupied", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
            }},
            { "SimGetMother", "Boolean", "Get the father from a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
                { "o_Spouse", "Alias ( of type sim)", "the father on success", false },
            }},
            { "SimGetNeed", "void", "gets the current need value of the need", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to query", false },
                { "NeedId", "Number", "the id of the need the satisfy", false },
            }},
            { "SimGetOffice", "Number", "get the office index of the currently occupied office (all offices on a level are enumerated from 0 to (n-1))", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
                { "ret_pOffice", "Alias ( of type office)", "the office on success", false },
            }},
            { "SimGetOfficeID", "Number", "get the office index of the currently occupied office (all offices on a level are enumerated from 0 to (n-1))", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimGetOfficeIndex", "Number", "get the office index of the currently occupied office (all offices on a level are enumerated from 0 to (n-1))", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimGetOfficeLevel", "Number", "get the office level of the currently occupied office", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimGetPregnant", "Boolean", "Lets the sim get pregnant", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim who will become pregnant", false },
            }},
            { "SimGetProduceItemID", "Number", "gets the current production of a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
            }},
            { "SimGetProductivity", "Number", "Returns the age of the sim", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim whose age will be returned", false },
            }},
            { "SimGetProfession", "Number", "Returns the profession of the sim", "Global", {
                { "pSim", "Alias ( of type sim)", "the Sim", false },
            }},
            { "SimGetProgress", "Number", "Get the progress to the actual beloved", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to get the progress for", false },
            }},
            { "SimGetRank", "Number", "Returns the rank of the sim (0 = EN_RANK_UNDEFINED, 1 = EN_RANK_DESTITUTE, 2 = EN_RANK_POOR, 3 = EN_RANK_MIDDLE, 4 = EN_RANK_RICH, 5 = EN_RANK_WEALTHY)", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim", false },
            }},
            { "SimGetReligion", "Number", "Returns the religion of the sim (0=catholic, 1=protestantic)", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim", false },
            }},
            { "SimGetRotation", "Number", "Get the rotation from an object (Position/Sim/Building/...)", "Global", {
                { "pObject1", "Alias ( of type guildobject)", "Object (cl_Position/cl_Sim/cl_Building/...)", false },
            }},
            { "SimGetServantDynasty", "Number", "Returns the dynasty for this object", "Global", {
                { "pSim", "Alias ( of type sim)", "the object to check", false },
                { "o_Dynasty", "Alias ( of type dynasty)", "servant dynasty on success", false },
            }},
            { "SimGetSpouse", "void", "Get the spouse from a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to find it's spouse", false },
                { "o_Spouse", "Alias ( of type sim)", "the spouse on success", true },
            }},
            { "SimGetWage", "Number", "get the handsel for the sim for hiring in the place", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to hire", false },
                { "pWorkingPlace", "Alias ( of type guildobject)", "the working place", false },
            }},
            { "SimGetWealth", "Number", "Returns the wealth of the sim", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim", false },
            }},
            { "SimGetWorkingPlace", "Number", "returns the working place of a worker", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
                { "pPlace", "Alias ( of type guildobject)", "the working place on success (cl_Building/cl_Cart)", false },
            }},
            { "SimGetWorkingPlaceID", "Number", "returns the ID of the working place of a worker", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
            }},
            { "SimHasAbility", "Boolean", "checks if a sim has a specific ability", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
                { "iDBId", "Number", "database-id of the ability", false },
            }},
            { "SimHire", "String", "Hire's a sim for a place. The place can be a building or a cart", "Global", {
                { "pWorker", "Alias ( of type sim)", "Sim to hire", false },
                { "pPlace", "Alias ( of type guildobject)", "Place to hire the sim (building/cart)", false },
            }},
            { "SimIsAppliedForOffice", "Boolean", "Checks if the sim is currently applied for a office", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to check", false },
            }},
            { "SimIsInside", "Boolean", "returns true, if the sim is currently inside a building", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimIsMortal", "Boolean", "returns true, if a sim is mortal", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "SimIsWorkingTime", "Boolean", "returns true, if the sim has currently working time", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimLock", "Boolean", "Locks the a sim for a time (hours)", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to lock", false },
                { "Time", "Number", "Time (hours) to lock,", false },
            }},
            { "SimMarry", "Number", "Let the sim marry another sim", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to marry", false },
                { "pSpouse", "Alias ( of type sim)", "the spouse", false },
            }},
            { "SimPauseWorking", "Boolean", "Let's a sim pause is currently working task - if nothing else happends, the sim will start working the next logic frame where he is idle", "Global", {
                { "pSim", "Alias ( of type sim)", "sim", false },
            }},
            { "SimReleaseCourtLover", "Boolean", "Releases the current court lover of the given sim", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim whose current court lover will be released", false },
            }},
            { "SimResetBehavior", "Boolean", "Sets the behavior of a sim to the standard behavior for it's state (workless, dynasty, ...)", "Global", {
                { "SimAlias", "String", "Sim", false },
            }},
            { "SimResumePreFightMeasure", "Boolean", "for fighting state: resume the measure that was running just bevor the fight", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to resume the fight for", false },
            }},
            { "SimRunForAnOffice", "Boolean", "run for an office", "Global", {
                { "pApplicant", "Alias ( of type sim)", "the sim who applies for an office", false },
                { "pOffice", "Alias ( of type office)", "the office the sim applies for", false },
            }},
            { "SimSetAge", "Number", "Sets a new age to the sim", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim whose age will be set", false },
            }},
            { "SimSetBehavior", "Boolean", "Sets the behavior of a sim.", "Global", {
                { "SimAlias", "String", "Sim", false },
                { "Behavior", "String", "name of the behavior", false },
            }},
            { "SimSetBehaviorDataAlias", "Boolean", "Sets a alias value for the standard behavior.", "Global", {
                { "SimAlias", "String", "Sim", false },
                { "DataName", "String", "name of the data to set", false },
                { "AliasName", "String", "Alias to set", false },
            }},
            { "SimSetBehaviorDataFloat", "Boolean", "Sets a datavalue for the standard behavior. This value is set as float data ( HasData / GetData ) on the behavior script", "Global", {
                { "SimAlias", "String", "Sim", false },
                { "DataName", "String", "name of the data to set", false },
                { "DataValue", "Number", "float value", false },
            }},
            { "SimSetClass", "Number", "Set's the character class of a sim which is one of (1 - GL_CLASS_PATRON, 2 - GL_CLASS_ARTISAN, 3 - GL_CLASS_SCHOLAR, 4 - GL_CLASS_FIGHTER, 5 - GL_CLASS_TRADESMAN, 6 - GL_CLASS_NPC). For example: SimSetClass(\"\", GL_CLASS_FIGHTER)", "Global", {
                { "Sim", "Alias ( of type sim)", "Sim to set the class", false },
                { "Class", "Number", "The new class of the sim", false },
            }},
            { "SimSetCourtLover", "Boolean", "Sets the SecondSim as new lover for the FirstSim", "Global", {
                { "FirstSim", "Alias ( of type sim)", "The sim which will court for the SecondSim", false },
                { "SecondSim", "Alias ( of type sim)", "The sim which will be the new lover of the FirstSim", false },
            }},
            { "SimSetDoctorTitle", "Number", "Gives a sim a doctor title", "Global", {
                { "Sim", "Alias ( of type sim)", "The sim who will get the doctor title", false },
            }},
            { "SimSetFaith", "Nil", "Sets the faith of the sim (range 0..100)", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim", false },
                { "faith", "Number", "Value", false },
            }},
            { "SimSetFamily", "Boolean", "Add's a new children to a family. This function set's also the new family name for the child", "Global", {
                { "pChild", "Alias ( of type sim)", "new child from pMother and pFather", false },
                { "pMother", "Alias ( of type sim)", "mother of the child", false },
                { "pFather", "Alias ( of type sim)", "father of the child", true },
            }},
            { "SimSetFirstname", "Boolean", "Sets the firstname of a sim.", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to change", false },
                { "Firstname", "String", "firstname to set (textlabel)", false },
            }},
            { "SimSetHireable", "Boolean", "Enables or disables the hirestate from a sim.", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to modify", false },
                { "hireable", "Boolean", "true: can be hired false: cannot be hired", false },
            }},
            { "SimSetInvisible", "Boolean", "sets the sim invisible for all but the local player dynasty (currently for robber camoflague), this should be only executed on members of the own dynasty", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
                { "a_bToggle", "Boolean", "true or false", false },
            }},
            { "SimSetLastname", "Boolean", "Sets the family name of a sim.", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim", false },
                { "Label", "String", "Lastname to set (textlabel)", false },
            }},
            { "SimSetMortal", "Boolean", "sets the mortal state of a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to check", false },
                { "Mortal", "Boolean", "true: mortal false: imortal", false },
            }},
            { "SimSetOffice", "Nil", "lets a sim take an office", "Global", {
                { "pSim", "Alias ( of type sim)", "alias of the sim", false },
                { "pOffice", "Alias ( of type office)", "alias of the office or 0 to loose the current office", false },
            }},
            { "SimSetParents", "Boolean", "Sets the parents of a sim", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to set the parents for", false },
                { "pFather", "Alias ( of type sim)", "Father of the sim", false },
                { "pMother", "Alias ( of type sim)", "Mother of the sim", false },
            }},
            { "SimSetProduceItemID", "Boolean", "starts or stops a production for a sim (use -1 to stop)", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to produce an item", false },
                { "Item", "Number or String", "Type of Item to search (int: datbase id string: itemname) (or -1 to stop)", false },
            }},
            { "SimSetProgress", "Number", "Sets the progress to the actual beloved", "Global", {
                { "pSim", "Alias ( of type sim)", "sim whose courting progress will be set", false },
                { "NewProgress", "Number", "The new progress", false },
            }},
            { "SimSetReligion", "Nil", "Sets the religion of the sim (0=catholic, 1=protestantic)", "Global", {
                { "SimAlias", "Alias ( of type sim)", "The sim", false },
                { "Religion", "Number", "religion id", false },
            }},
            { "SimStartIdleMeasure", "Boolean", "Starts immediatly the idle measure", "Global", {
                { "pSim", "Alias ( of type sim)", "Sim to start", false },
            }},
            { "SimStopMeasure", "Nil", "Stops the current measure on the given sim", "Global", {
                { "pSim", "Alias ( of type sim)", "The sim whose measure will be stoped", false },
            }},
            { "SpendMoney", "Boolean", "Lets the object spend money.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to spend money (cl_Sim/cl_Dynasty/cl_Building/cl_Cart/...)", false },
                { "Amount", "Number", "Amount to spend", false },
                { "Purpose", "String", "Reason to spend (for balance)", false },
            }},
            { "SquadAddMember", "Boolean", "adds a new member to a existing squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "the squad to add to", false },
                { "iNumber", "Number", "the number of the place. If iNumber=-1 than a fitting position is created", false },
                { "pSim", "Alias ( of type sim)", "the sim to add", false },
            }},
            { "SquadAddRandom", "void", "Add's a new random member to the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "the member to add the new member to", false },
                { "iNumber", "Number", "the number of the member", false },
                { "iProfession", "Any", "the profession of the new member", false },
            }}, // [AUTO-FIXED PARAMS]
            { "SquadCreate", "Boolean", "Creates a new squad action on a guild object. It can be only one squad action on the object at the same time.", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to create the squad for", false },
                { "Measure", "Number or String", "The measure to start. int = measure database ID / string = name of measure", false },
                { "Alias ( of type corenode pdestination)", "...", "Destination (optional)", true },
                { "BehaviorLeader", "Any", "Behavior of the leader", true },
                { "BehaviorMember", "Any", "Behavior of each member", true },
            }}, // [AUTO-FIXED PARAMS]
            { "SquadDestroy", "Boolean", "Destroys the squad for the object", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "object to destroy the squad for", false },
            }},
            { "SquadGet", "Boolean", "gets the current squad from the object", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to query", false },
                { "ret_pSquad", "Alias ( of type aisquad)", "the squad on success", false },
            }},
            { "SquadGetLeader", "Boolean", "Get's the leader of the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "ret_pSim", "Alias ( of type sim)", "the leader on success", false },
            }},
            { "SquadGetLeaderId", "void", "Get's the leader-id of the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
            }},
            { "SquadGetMeetingPlace", "Boolean", "get's the meeting place of the squad (on success)", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "ret_pPlace", "Alias ( of type position)", "the place on success", false },
            }},
            { "SquadGetMember", "Boolean", "Get's the leader of the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "iNumber", "Number", "the leader on success", false },
            }},
            { "SquadGetMemberCount", "void", "returns the count of the people in the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad", false },
                { "IgnoreEmptyPlaces", "Boolean", "true: ignore empty places false: normal", false },
            }},
            { "SquadGetProfession", "void", "returns the profession for the position iNumber ( 0 > iNumber > SquadGetMemberCount() )", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "the member to add the new member to", false },
                { "iNumber", "Number", "Number to get the profession for (iNumber must be greater 0)", false },
            }},
            { "SquadGetState", "void", "Get's the state of the squad (SQUAD_INITIALISING, SQUAD_EXECUTING, SQUAD_SHUTDOWN)", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
            }},
            { "SquadGetVacantPlace", "void", "returns the number of the first vacant place number in the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad", false },
            }},
            { "SquadIsNeeded", "Number", "checks if a sim is needed by a squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "SquadMemberExists", "Boolean", "checks, if a member with the number iNumber exists", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "iNumber", "Number", "number of the member", false },
            }},
            { "SquadReady", "Boolean", "returns true, if all members are ready to act", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
            }},
            { "SquadRemoveMember", "Boolean", "removes this object from its current squad", "Global", {
                { "pObject", "Alias ( of type guildobject)", "object to remove from the squad. If the leader of the squad is removed, the squad is deleted.", false },
            }},
            { "SquadSetMeetingPlace", "Boolean", "Set's the meeting place of the squad", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "pPlace", "Alias ( of type guildobject)", "the new meeting place", false },
            }},
            { "SquadSetReady", "Nil", "Set's a member to a ready state. true = ready false = not ready", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
                { "iNumber", "Number", "number of the member", false },
                { "State", "Boolean", "true: ready false: not ready", false },
            }},
            { "SquadWait", "Nil", "waits until the squad has finished collecting people", "Global", {
                { "pSquad", "Alias ( of type aisquad)", "squad to check", false },
            }},
            { "SquadWaitSim", "Nil", "idle until squad collection", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to set to sleep", false },
            }},
            { "StartEmote", "Nil", "Start an emote script function from library/emotes.lua", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to play the emote", false },
                { "EmoteFunction", "Any", "function name in emotes.lua script file", false },
                { "Duration", "Number", "(optional) duration of this emote", true },
            }}, // [AUTO-FIXED PARAMS]
            { "StartGameTimer", "Nil", "starts a gametime decrementing timer which end can be check via the function CheckGameTimerEnd", "Global", {
                { "Time", "Number", "Timer time", false },
            }},
            { "StartHighPriorMusic", "Number", "Starts a music which overrules the normal music and the pause", "Global", {
                { "Location", "Number", "The location or special case for the music which is one of the following:", false },
            }},
            { "StartMagicalEffect", "Boolean", "This function generates a \"magical effect\".", "Global", {
                { "Position", "Alias ( of type position)", "The center of the effect (the height is not relevant -> It will be droped to the terrain height).", false },
                { "Filename", "Any", "The filename of the object which will be the effect (i.e. \"particles/aimingpoint.nif\").", false },
                { "Lifetime", "Number", "The lifetime of the effect in seconds.", false },
                { "Fadetime", "Number", "The duration of the fading-out (i.e. if the lifetime is 2.0 and the fadetime 0.8 then the fading-out will start after 1.2 seconds. It will always and at the end of the effect !)", false },
                { "StartRadius", "Number", "The size of the effect in units at the time the effect starts (regardless of the size of the .nif file)", false },
                { "EndRadius", "Number", "The size of the effect in units at the end of the effect.", false },
            }}, // [AUTO-FIXED PARAMS]
            { "StartMission", "Boolean", "starts a quest with the given questname with the given actor and source if the questrequirements are fullfiled. If RemainInStartup is true the questrequriements will be checked every frame until they are fullfilled an the quest can start.", "Global", {
                { "pQuestname", "String", "questname", false },
                { "pActorAlias", "Alias ( of type dynasty)", "actor", false },
            }},
            { "StartProduction", "Nil", "start the intern production of an item by a sim. This function is designed for use in the production measure only.", "Global", {
                { "pSim", "Alias ( of type sim)", "the sim to start the production", false },
            }},
            { "StartQuest", "Boolean", "starts a quest with the given questname with the given actor and source if the questrequirements are fullfiled. If RemainInStartup is true the questrequriements will be checked every frame until they are fullfilled an the quest can start.", "Global", {
                { "pQuestname", "String", "questname", false },
                { "pActorAlias", "Alias ( of type sim)", "actor", false },
                { "pSourceAlias", "Alias ( of type guildobject)", "source", false },
                { "RemainInStartup", "Boolean", "remain in startup", false },
            }},
            { "StartRoomAnimation", "Boolean", "starts an object's animation in the given room of the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "cl_Building* pBuilding -", false },
                { "Room", "String", "name of the room upgrade or \"\" for the mainroom", false },
                { "Object", "String", "const char* Object -", false },
            }},
            { "StartSingleShotParticle", "Boolean", "Create a particle effect on a given position", "Global", {
                { "ParticleName", "String", "name of the particle", false },
                { "Position", "Alias ( of type position)", "Position to create the effect", false },
                { "Scale", "Number", "The scale of the particle effect", false },
                { "EmissionLength", "Number", "The length of the emission", false },
            }},
            { "StartVision", "Boolean", "starts a heavenly vision at the postion", "Global", {
                { "Position", "Alias ( of type position) or Alias ( of type simobject)", "Position to show (cl_Building or cl_Position)", false },
            }},
            { "StopAction", "Boolean", "This function stopps emitting an action initiated with CommitAction(...). After calling this function the observers does not longer react to the action.", "Global", {
                { "ActionName", "String", "database name of the action", false },
                { "pActor", "Alias ( of type guildobject)", "the object what started the action", false },
            }},
            { "StopAnimation", "Number", "Stops all playing animation on the pMoveSim.", "Global", {
                { "pMoveSim", "Alias ( of type movingsimobject)", "Object", false },
            }},
            { "StopEmote", "Nil", "Stop the running emote", "Global", {
                { "pSim", "Alias ( of type sim)", "sim to stop the emote", false },
            }},
            { "StopFollow", "Number", "no comment", "Global", {}}, // No params
            { "StopMeasure", "Nil", "Stops the current measure.", "Global", {}}, // No params
            { "StopRoomAnimation", "Boolean", "stops an object's animation in the given room of the building", "Global", {
                { "pBuilding", "Alias ( of type building)", "cl_Building* pBuilding -", false },
                { "Room", "String", "name of the room upgrade or \"\" for the mainroom", false },
                { "Object", "String", "const char* Object -", false },
            }},
            { "StopScheduledScript", "void", "Terminates the execution of the current scheduled script", "Global", {}}, // No params
            { "Stroll", "Boolean", "Let's the pSim stroll a time.", "Global", {
                { "pSim", "Alias ( of type movingsimobject)", "the object to stroll", false },
                { "Range", "Number", "the stroll range from the current position", false },
                { "Duration", "Number", "the duration of the strolling", false },
                { "ResultData", "String", "the Datavalue to return the result back", true },
            }},
            { "SystemGetMeasureName", "String", "Get's the measure name (from the database) of the current measure", "Global", {}}, // No params
            { "SystemGetMeasurePriority", "void", "Get's the priority value of the current measure", "Global", {}}, // No params
            { "Talk", "void", "Executes the internal talk logic between two sims", "Global", {
                { "pTalker", "Alias ( of type sim)", "the initiator of the talk", false },
                { "pTarget", "Alias ( of type sim)", "the talk partner", false },
            }},
            { "ThrowObject", "Number", "Transfers all items from the source and transfer them to the destination.", "Global", {
                { "pThrower", "Alias ( of type guildobject)", "Target to throw at", false },
                { "pTarget", "Alias ( of type guildobject)", "Model of object to throw", false },
                { "sObjectModelName", "Any", "Model to Throw", false },
                { "a_fHeightRatio", "Number", "defines the steepness of the throwing parabel. 0.1 means 10% of throwing distance, 5.0 means 500% of throwing distance. was 0.5 hardcoded until now", false },
                { "o_AliasName", "Alias ( of type i_projectile)", "returned ProjectileObject", false },
            }}, // [AUTO-FIXED PARAMS]
            { "TradeMemoryEvents", "Number", "Trades the memory events from pSource to pDestinations", "Global", {
                { "pSource", "Alias ( of type sim)", "the sim who get's the memory events", false },
                { "pDestination", "Alias ( of type sim)", "the source of the memory events", false },
                { "pActor", "Alias ( of type sim)", "if a actor is specified, only memory events based on actor are shared", true },
                { "ClearMemory", "Boolean", "ClearMemory if true, the traded memory events are cleared from pSource's memory", false },
            }},
            { "Transfer", "void", "normal transfer between two guild object. Money is payed if necessary", "Global", {
                { "pExecuter", "Alias ( of type guildobject)", "Object who initiates the transfer", true },
                { "pBuyer", "Alias ( of type guildobject)", "Object to buys the items (cl_Sim/cl_Building/cl_Cart/cl_GuildResource/...)", true },
                { "BuyerInventoryType", "Any", "InventoryType of the inventory to remove the items from. If no inventorytype is specified the default inventory will be used", true },
                { "pSeller", "Alias ( of type guildobject)", "Object to buy the items (cl_Sim/cl_Building/cl_Cart/cl_GuildResource/...)", false },
                { "SellerInventoryType", "Any", "InventoryType of the inventory to add the items to. If no inventorytype is specified the default inventory will be used", false },
                { "ItemId", "Any", "Item to transfer", false },
                { "ItemCount", "Any", "Count of items to transfer", false },
            }}, // [AUTO-FIXED PARAMS]
            { "TransferItems", "Boolean", "Transfers all items from the source and transfer them to the destination.", "Global", {
                { "pSource", "Alias ( of type guildobject)", "Object to get the items (cl_Sim/cl_Building/cl_Cart/cl_GuildResource/...)", false },
                { "pDestination", "Alias ( of type guildobject)", "Object to take the items (cl_Sim/cl_Building/cl_Cart/cl_GuildResource/...)", false },
                { "SourceInventoryType", "Any", "InventoryType of the inventory to remove the items from. If no inventorytype is specified the default inventory will be used", false },
                { "DestInventoryType", "Any", "InventoryType of the inventory to add the items to. If no inventorytype is specified the default inventory will be used", false },
            }}, // [AUTO-FIXED PARAMS]
            { "WaitForSelection", "Boolean", "forces the measurescript to wait for a user selection", "Global", {
                { "pResultName", "String", "", false },
                { "pFilter", "String", "", false },
            }},
            { "Weather_GetSeason", "Number", "retrieves the actual season", "Global", {}}, // No params
            { "Weather_GetValue", "Number", "retrieves a weather value :", "Global", {
                { "Type", "Number", "int32 Type -", false },
            }},
            { "Weather_PlaySound", "Boolean", "plays a weather sound (non looped)", "Global", {
                { "Name", "String", "", false },
                { "Volume", "Number", "", false },
            }},
            { "Weather_Rand", "Number", "Random function to be used in the weather scripts", "Global", {
                { "MaxValue", "Number", "", false },
            }},
            { "Weather_SetCloudFactor", "Boolean", "sets the active cloud factor for the weather", "Global", {
                { "Sky", "Number", "float Clouds -", false },
            }},
            { "Weather_SetLight", "Boolean", "sets the weather light factor", "Global", {
                { "Light", "Number", "float Light -", false },
            }},
            { "Weather_SetParticles", "Boolean", "sets the active weather particles (rain/snow)", "Global", {
                { "Intensity", "Number", "float Intensity -", false },
            }},
            { "Weather_SetSoundLoop", "void", "no comment", "Global", {}}, // No params
            { "Weather_SetWeather", "Boolean", "", "Global", {}}, // No params
            { "Weather_SetWind", "Boolean", "sets the active weather's wind", "Global", {
                { "Strength", "Number", "", false },
            }},
            { "bool	BossCreate", "void", "Creates a player character with the values from the character creation screen", "Global", {
                { "pPosition", "Alias ( of type corenode)", "home building of the sim", true },
                { "Gender", "Number", "(GL_GENDER_FEMALE/GL_GENDER_MALE)", false },
                { "Class", "Number", "class", false },
                { "TemplateDBId", "Number", "database id for a character template", false },
                { "ret_Sim", "Alias ( of type sim)", "the sim on success", false },
            }},
            { "bool	PlayerCreate", "void", "Creates a player character with the values from the character creation screen", "Global", {
                { "pPosition", "Alias ( of type corenode)", "home building of the sim", true },
                { "ret_Sim", "Alias ( of type sim)", "the sim on success", false },
            }},
            { "f_Follow", "Boolean", "Starts a follow task for pObject.", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "the moving object (cl_Sim/cl_Cart/...)", false },
                { "pTarget", "Alias ( of type corenode)", "the destination to follow (eg. cl_Building/cl_Position/cl_Cart)", false },
                { "Speed", "Number", "moving speed (GL_MOVESPEED_WALK = normal / GL_MOVESPEED_RUN = run)", true },
                { "Range", "Number", "The object will stop at a distance of Range bevor reaching the target", true },
                { "followonce", "Boolean", "Follows untill it reaches target once", true },
                { "bScheduled", "Boolean", "notifies the task tabout the measure for later progression", true },
            }},
            { "f_FollowNoWait", "Boolean", "Starts a follow task for pObject and proceeds with the script", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "the moving object (cl_Sim/cl_Cart/...)", false },
                { "pTarget", "Alias ( of type corenode)", "the destination to follow (eg. cl_Building/cl_Position/cl_Cart)", false },
                { "Speed", "Number", "moving speed (GL_MOVESPEED_WALK = normal / GL_MOVESPEED_RUN = run)", true },
                { "Range", "Number", "The object will stop at a distance of Range bevor reaching the target", true },
                { "followonce", "Boolean", "Follows untill it reaches target once", true },
                { "bScheduled", "Boolean", "notifies the task tabout the measure for later progression", true },
            }},
            { "f_MoveTo", "Boolean", "Starts a move task for pObject.", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "The object to move (cl_Sim/cl_Cart/...)", false },
                { "pTarget", "Alias ( of type corenode)", "the destination to move to (eg. cl_Building/cl_Position/cl_Cart)", false },
                { "Speed", "Number", "moving speed (GL_MOVESPEED_SNEAK / GL_MOVESPEED_WALK / GL_MOVESPEED_RUN )", true },
                { "Range", "Number", "The object will stop at a distance of Range before reaching the target", true },
                { "bScheduled", "Boolean", "notifies the task tabout the measure for later progression", true },
            }},
            { "f_MoveToNoWait", "Boolean", "Starts a move task for pObject and proceeds with the script", "Global", {
                { "pObject", "Alias ( of type movingsimobject)", "The object to move (cl_Sim/cl_Cart/...)", false },
                { "pTarget", "Alias ( of type corenode)", "the destination to move to (eg. cl_Building/cl_Position/cl_Cart)", false },
                { "Speed", "Number", "moving speed (GL_MOVESPEED_SNEAK / GL_MOVESPEED_WALK / GL_MOVESPEED_RUN )", true },
                { "Range", "Number", "The object will stop at a distance of Range bevor reaching the target", true },
                { "bScheduled", "Boolean", "notifies the task tabout the measure for later progression", true },
            }},
            { "float	CityGetLocalMarket", "void", "returns the local market of the city", "Global", {
                { "pCity", "Alias ( of type settlement)", "city to query", false },
                { "ret_pMarket", "Alias ( of type market)", "the market on success", false },
            }},
            { "float	CityGetWantedTime", "void", "returns the prison time if a sim is wanted in a city", "Global", {
                { "pCity", "Alias ( of type settlement)", "city to query", false },
                { "pSim", "Alias ( of type sim)", "sim to check", false },
            }},
            { "float	Gametime2Realtime", "void", "Transfer a time given in gametime (1 = 1h ingame time) to realtime (1 = 1 second)", "Global", {
                { "Gametime", "Number", "Gametime to convert (1 = 1h ingame time)", false },
            }},
            { "float	Realtime2Gametime", "void", "Transfer a time given in realtime (1 = 1 second) to gametime (1 = 1h ingame time)", "Global", {
                { "Realtime", "Number", "Realtime (in seconds)", false },
            }},
            { "int	ScenarioFindBuildingProto", "void", "Finds a proto for a building by its Class/Type/Level/SubLevel. If more than one proto matches the values, a random proto is returned", "Global", {
                { "Class", "Number", "Class (-1 for all)", false },
                { "Type", "Number", "Type (-1 for all)", false },
                { "Level", "Number", "Level (-1 for all)", false },
                { "SubLevel", "Number", "Sublevel (-1 for all)", false },
            }},
            { "int	ScenarioFindBuildingProtoForCharacter", "void", "Finds a proto for a building by its Class/Type/Level/SubLevel. If more than one proto matches the values, a random proto is returned", "Global", {
                { "pSim", "Alias ( of type sim)", "character class (0 for all)", false },
                { "Level", "Number", "Level (-1 for all)", false },
                { "SubLevel", "Number", "Sublevel (-1 for all)", false },
            }},
            { "simgetservantdynastyid", "Number", "Returns the servant dynasty id for this object", "Global", {
                { "pSim", "Alias ( of type sim)", "the object to check", false },
            }},
        };
        return funcs;
    }
};

} // namespace ScriptAPI
