/**
 * @file Paradigm.h
 */

#ifndef _PARADIGM_H_
#define _PARADIGM_H_

//OpenSceneGraph
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>

//CalVR
#include <cvrKernel/PluginHelper.h>
#include <cvrConfig/ConfigManager.h>
#include <cvrConfig/XMLReader.h>

//std libraries
#include <vector>
#include <iostream>
#include <tuple>

//project files
#include "Path.h"
#include "Cue.h"
#include "Cues/WallCue.h"
#include "Cues/FloorCue.h"
#include "Cues/EndCue.h"
#include "Cues/StartCue.h"
#include "Cues/AuditoryCue.h"
//#include "Controller/OutboundPackets/TrialSetup.h"

using namespace std;
using namespace cvr;

/**
 * Desribes how the starting or finishing position varies between trials.
 */
enum PositionType { Constant = 0/**< Constant */, Random /**< Random */ };


/**
 * Grouping of like trials.
 */
class Paradigm
{
    public:
		/**
		* @param paradigmID Unique identifier for this paradigm
		* @param file XML configuration file detailing this paradigm setup
		* 
		* Constructor
		*/
		Paradigm(string paradigmID, string file);
		/**
		* Destructor
		*/
		virtual ~Paradigm();

	//Modifier Methods
		/**
		* @param p Path to be added
		* 
		* Add a path to the _paths list for data recording.
		*/
		void addPath(Path* p);
		/**
		* Override the trial limit increasing the limit by one.
		*/
		void addTrial();
		/**
		* Find a new (if that is the policy) starting position.
		*/
		void setStartingPos();
		/**
		* Find a new (if that is the policy) finish position.
		*/
		void setFinishPos();
		/**
		* Toggle a cue belonging to this paradigm.
		*/
		void toggle(tuple<string, bool> t);

	//Access Methods
		/**
		* Returns the starting position.
		*/
		int getStartingPos();
		/**
		* Returns the finish position.
		*/
		int getFinishPos();
		/**
		* Returns how many more trials can be run.
		*/
		int trialsRemaining();
		/**
		* Returns whether or not there is not a trial limit.
		*/
		bool isContinuous();
		/**
		* Returns whether or not moving to the next paradigm is an option.
		*/
		bool canEnd();
		/**
		* Returns the number of tiles wide (x-axis) this paradigm is.
		*/
		int getWidth();
		/**
		* Returns the number of tiles long (y-axis) this paradigm is.
		*/
		int getLength();
		/**
		* Returns the size (scene space) of a single tile.
		*/
		int getTileSize();
		/**
		* Returns how many trials have been run (including any active trial) so far.
		*/
		int getTrialNumber();
		/**
		* @param duration Elapsed time since start of trial
		* 
		* Returns how much time is left in a current trial.
		*/
		float getTimeRemaining(float duration);
		/**
		* Returns
		*/
		string getID();
		/**
		* Returns the list of cues belonging to this paradigm.
		*/
		vector<Cue*> getCues();
		/**
		* Returns whether or not geometry will be automatically loaded.
		*/
		bool isAutoLoad();
		/**
		* Returns whether or not red tiles will be rendered.
		*/
		bool isColoredGrid();
		/**
		* Returns whether or not the destination is indicated by a green circle.
		*/
		bool isTileCircle();
		/**
		* Returns the ending procedure corresponding to this paradigm.
		*/
		EndCue* getEnding();
		/**
		* Returns the starting procedure corresponding to this paradigm.
		*/
		StartCue* getStart();
		
	//Debug
		string printCues();

	//Cue Rendering
		/**
		* @param _geoRoot MatrixTransform of scene space.
		* @param ts Setup of the currently running trial (or trial about to run).
		* 
		* Used to add geometry to the scene corresponding to all the Cue objects belonging to this paradigm.
		*/
		void renderCues(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
		
	//Sound Methods
		void playSuccess();		
		void playFailure();				
		void playSoundCues(float time);	
		void playSoundCues(Vec3 pos);
		void setSoundStartTimer(float time);
    protected:
		//internal utilities
		void newStart();
        void newFinish();
        Cue* newCue(string type, string file);
		
		//paradigm identifier values
		string _paradigmID;			///< unique identifier for paradigm
		
		///cotrol parameters @todo refactor this into the TrialSetup object
		int _length;	
		int _width;
		int _tileSize;				///< @todo value being taken from general config file. not being implimented yet
		int _wallHeight;			///< @todo value being taken from general config file. not being implimented yet
		std::vector<Cue*> _cues;	///< @todo refactor this so cues are grouped together by type.
		PositionType _startingPosType;
		PositionType _finishPosType;
		int _startingPos;			///< initial tile position
		osg::Vec3 _startingLookAt;	///< @todo note: not being used. Could be used.
		int _finishPos;				///< destination tile
		int _numTrials;				///< maximum number of trials that must be run
		int _minTrials;				///< minimum number of trials that must be run
		int _timeOut;				///< maximum amount of time allowed in a trial		
		bool _gridLines;			///< @todo note: not being used. Makes maze too easy.
		
		//data recording values
		std::vector<Path*> _trials;	//id, path
		
		bool _autoLoad;				///< indicates if geometry will load automatically
		
		vector<AuditoryCue*> _soundCues; 
		
};






#endif
