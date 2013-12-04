/**
 * @file EyeData.h
 */

#ifndef _EYEDATA_H_
#define _EYEDATA_H_

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

using namespace osg;

/**
 * All data pertaining to the ocular focal point of the test subject.  @todo plug in eye tracking to calculate the actual focal point
 */
class EyeData
{
	public:
		/**
		 * Constructor
		 */
		EyeData(osg::Vec3 right, osg::Vec3 front, osg::Vec3 up);
		/**
		 * Destructor
		 */
		virtual ~EyeData();
		
		/**
		* Returns position of 1 unit to the right of the head position.
		*/
		osg::Vec3 getRight();
		/**
		* Returns position of 1 unit in front of the head position.
		*/
		osg::Vec3 getFront();
		/**
		* Returns position of 1 unit above the head position.
		*/
		osg::Vec3 getUp();
	protected:
		osg::Vec3 _right;	
		osg::Vec3 _front;
		osg::Vec3 _up;
		
};

#endif
