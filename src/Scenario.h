//
//  Scenario.h
//  RosterDesNurses
//

#ifndef __Scenario__
#define __Scenario__

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "MyTools.h"
#include "Nurse.h"

using std::map;
using std::pair;
using std::string;
using std::vector;

// the penalties for violating the soft constraints on the nurses' schedules
// are in the problem definition
// they are set as static constant values in case they need to be shared with
// other classes (e.g. solvers)
//
static const int costConsShifts_       = 15;
static const int costConsDaysWork_     = 30;
static const int costConsDaysOff_      = 30;
static const int costPreferences_      = 10;
static const int costCompleteWeekEnd_  = 30;
static const int costTotalShifts_      = 20;
static const int costTotalWeekEnds_    = 30;

//-----------------------------------------------------------------------------
//
//  C l a s s   S c e n a r i o
//
//  Class that contains all the attributes describing the scenario
//
//-----------------------------------------------------------------------------

class Scenario {

public:

	// Constructor and destructor
	//
	Scenario(string name, int nbWeeks,
			int nbSkills, vector<string> intToSkill, map<string,int> skillToInt,
			int nbShifts, vector<string> intToShift, map<string,int> shiftToInt,
			vector<int> minConsShifts, vector<int> maxConsShifts,
			vector<int> nbForbiddenSuccessors, vector2D forbiddenSuccessors,
			int nbContracts, map<string,Contract*> contracts,
			int nbNurses, vector<Nurse> theNurses, map<string,int> nurseNameToInt) :
				name_(name), nbWeeks_(nbWeeks),
				nbSkills_(nbSkills), intToSkill_(intToSkill), skillToInt_(skillToInt),
				nbShifts_(nbShifts), intToShift_(intToShift), shiftToInt_(shiftToInt),
				minConsShifts_(minConsShifts), maxConsShifts_(maxConsShifts),
				nbForbiddenSuccessors_(nbForbiddenSuccessors), forbiddenSuccessors_(forbiddenSuccessors),
				nbContracts_(nbContracts), contracts_(contracts),
				nbNurses_(nbNurses), theNurses_(theNurses), nurseNameToInt_(nurseNameToInt){

		// To make sure that it is modified later when reading the history data file
		//
		thisWeek_ = -1;
	}
	~Scenario();


	//constant attributes are public
public:

	// name of the scenario
	//
	const std::string name_;

	// total number of weeks and current week being planned
	//
	const int nbWeeks_;

	// number of skills, a map and a vector matching the name of each skill to an
	// index and reversely
	//
	const int nbSkills_;
	const vector<string> intToSkill_;
	const map<string,int> skillToInt_;

	// number of shifts, a map and a vector matching the name of each shift to an
	// index and reversely
	// minimum and maximum number consecutive assignments for each shift,
	// and penalty for violations of these bounds
	//
	const int nbShifts_;
	const vector<string> intToShift_;
	const map<string,int> shiftToInt_;
	const vector<int> minConsShifts_, maxConsShifts_;

	// for each shift, the number of forbidden successors and a table containing
	// the indices of these forbidden successors
	//
	const vector<int> nbForbiddenSuccessors_;
	const vector2D forbiddenSuccessors_;

	// Vector of possible contract types
	//
	const int nbContracts_;
	const map<string, Contract*> contracts_;

	// number of nurses, and vector of all the nurses
	//
	const int nbNurses_;
	const vector<Nurse> theNurses_;
	map<string,int> nurseNameToInt_;


private:
	//------------------------------------------------
	// From the Week data file
	//------------------------------------------------
	// Name of the week
	string weekName_;
	// Current week demand for each DAY, SHIFT, and SKILL
	//
	vector3D minWeekDemand_;
	vector3D optWeekDemand_;
	// Shift off requests : Preferences for each nurse : which (day,shift) do they want off ?
	//
	int nbShiftOffRequests_;
	Preferences weekPreferences_;
	//------------------------------------------------


	//------------------------------------------------
	// From the History data file
	//------------------------------------------------
	// Initial historical state of the nurses
	//
	vector<State> initialState_;
	// index of the week that is being scheduled
	//
	int thisWeek_;
	//------------------------------------------------


	//------------------------------------------------
	// From the custom file
	//------------------------------------------------
	//------------------------------------------------

public:
	// getters for the class attributes
	//
	int nbWeeks() {return nbWeeks_;}
	int thisWeek() {return thisWeek_;}
	string weekName() {return weekName_;}


	// getters for the attributes of the nurses
	//
	int minTotalShiftsOf(int whichNurse) {
		return theNurses_[whichNurse].minTotalShifts_;
	}
	int maxTotalShiftsOf(int whichNurse) {
		return theNurses_[whichNurse].maxTotalShifts_;
	}
	int minConsDaysWorkOf(int whichNurse) {
		return theNurses_[whichNurse].minConsDaysWork_;
	}
	int maxConsDaysWorkOf(int whichNurse) {
		return theNurses_[whichNurse].maxConsDaysWork_;
	}
	int minConsDaysOffOf(int whichNurse) {
		return theNurses_[whichNurse].maxConsDaysOff_;
	}
	int maxConsDaysOffOf(int whichNurse) {
		return theNurses_[whichNurse].maxConsDaysOff_;
	}
	int maxTotalWeekEndsOf(int whichNurse) {
		return theNurses_[whichNurse].maxTotalWeekEnds_;
	}
	bool isCompleteWeekEndsOf(int whichNurse) {
		return theNurses_[whichNurse].isCompleteWeekEnds_;
	}

	// Setters to class attributes

	// When reading the week file (Demand and preferences
	//
	inline void setWeekName(string weekName){ weekName_ = weekName;}
	inline void setMinWeekDemand(vector3D minWeekDemand){ minWeekDemand_ = minWeekDemand; }
	inline void setOptWeekDemand(vector3D optWeekDemand){ optWeekDemand_ = optWeekDemand; }
	inline void setTNbShiftOffRequests(int nbShiftOffRequests){ nbShiftOffRequests_ = nbShiftOffRequests; }
	inline void setWeekPreferences(Preferences weekPreferences){ weekPreferences_ = weekPreferences; }

	// When reading the history file
	//
	inline void setThisWeek(int thisWeek){ thisWeek_ = thisWeek; }
	inline void setInitialState(vector<State> initialState){ initialState_ = initialState;}

	// Initialize the attributes of the scenario with the content of the input
	// file
	//
	void readScenario(string fileName);

    // Display methods: toString + override operator<< (easier)
    //
    string toString();
    friend std::ostream& operator<< (std::ostream& outs, Scenario obj) {return outs << obj.toString();}
};


#endif /* defined(__ATCSolver__CftSolver__) */
