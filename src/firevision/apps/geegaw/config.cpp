/***************************************************************************
 *  config.cpp - config helper - generated by genconfig 0.3
 *
 *  Config class generated: Fri Jun 15 13:43:56 2007
 *  Used template: ../../../../../src/modules/tools/genconfig/config.template.cpp
 *  Template created: Thu Apr 29 14:27:25 2004
 *  Copyright  2004  Tim Niemueller
 *  niemueller@i5.informatik.rwth-aachen.de
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL file in the doc directory.
 */

/// @cond RCSOFTX_CONFIG
 
#include "config.h"

#include <iostream>
#include <utils/utils.h>

using namespace std;

/** @class GeegawConfig "config.h"
 * Configration object.
 * Auto-generated configuration object.
 */

/** Constructor.
 * @param configFile config reader
 */
GeegawConfig::GeegawConfig(CConfigReader *configFile)
{

  string errmsg;

  m_pXMLConfigFile = configFile;
  m_sPrefix = "";

  if (m_pXMLConfigFile != NULL) {
    m_sPrefix = "<RCSoftConfigFile><Lowlevel><FireVision><Front><FrontGeneral>";

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ClassifierSettings><ClassifierType>", ClassifierType, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ClassifierSettings><ClassifierType>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ClassifierSettings><ClassifierType>: " << toString(ClassifierType) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ClassifierSettings><ShrinkerType>", ShrinkerType, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ClassifierSettings><ShrinkerType>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ClassifierSettings><ShrinkerType>: " << toString(ShrinkerType) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<PanTilt><ForwardPan>", ForwardPan, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<PanTilt><ForwardPan>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<PanTilt><ForwardPan>: " << toString(ForwardPan) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<PanTilt><ForwardTilt>", ForwardTilt, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<PanTilt><ForwardTilt>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<PanTilt><ForwardTilt>: " << toString(ForwardTilt) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<PanTilt><ForwardDelay>", ForwardDelay, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<PanTilt><ForwardDelay>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<PanTilt><ForwardDelay>: " << toString(ForwardDelay) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<PanTilt><PanMinChange>", PanMinChange, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<PanTilt><PanMinChange>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<PanTilt><PanMinChange>: " << toString(PanMinChange) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<PanTilt><TiltMinChange>", TiltMinChange, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<PanTilt><TiltMinChange>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<PanTilt><TiltMinChange>: " << toString(TiltMinChange) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<PanTilt><TiltMinChange>", TiltMax, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<PanTilt><TiltMinChange>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<PanTilt><TiltMinChange>: " << toString(TiltMax) << endl << flush;

    m_sPrefix = "<RCSoftConfigFile><Lowlevel><FireVision><Stereo><StereoGeneral><StereoModelSettings>";

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ColorModelType>", ColorModel, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ColorModelType>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ColorModelType>: " << toString(ColorModel) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ScanlineModelType>", ScanlineModel, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ScanlineModelType>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ScanlineModelType>: " << toString(ScanlineModel) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ColorModel><LookupTable><ColormapDirectory>", ColormapDirectory, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ColorModel><LookupTable><ColormapDirectory>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ColorModel><LookupTable><ColormapDirectory>: " << toString(ColormapDirectory) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ColorModel><LookupTable><ColormapFilestem>", ColormapFilestem, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ColorModel><LookupTable><ColormapFilestem>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ColorModel><LookupTable><ColormapFilestem>: " << toString(ColormapFilestem) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ColorModel><LookupTable><ColormapWidth>", LookupTableWidth, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ColorModel><LookupTable><ColormapWidth>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ColorModel><LookupTable><ColormapWidth>: " << toString(LookupTableWidth) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ColorModel><LookupTable><ColormapHeight>", LookupTableHeight, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ColorModel><LookupTable><ColormapHeight>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ColorModel><LookupTable><ColormapHeight>: " << toString(LookupTableHeight) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ScanlineModel><GridModel><GridXOffset>", ScanlineGridXOffset, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ScanlineModel><GridModel><GridXOffset>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ScanlineModel><GridModel><GridXOffset>: " << toString(ScanlineGridXOffset) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<ScanlineModel><GridModel><GridYOffset>", ScanlineGridYOffset, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<ScanlineModel><GridModel><GridYOffset>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<ScanlineModel><GridModel><GridYOffset>: " << toString(ScanlineGridYOffset) << endl << flush;

    m_sPrefix = "<RCSoftConfigFile><Lowlevel><FireVision><Front><RobotFrontSettings hostname=\""+string( getenv("HOSTNAME") )+"\">";

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraType>", CameraType, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraType>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraType>: " << toString(CameraType) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraControlType>", CameraControlType, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraControlType>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraControlType>: " << toString(CameraControlType) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><HorizontalViewingAngle>", HorizontalViewingAngle, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><HorizontalViewingAngle>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraSettings type=\""+toString(CameraType)+"\"><HorizontalViewingAngle>: " << toString(HorizontalViewingAngle) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><VerticalViewingAngle>", VerticalViewingAngle, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><VerticalViewingAngle>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraSettings type=\""+toString(CameraType)+"\"><VerticalViewingAngle>: " << toString(VerticalViewingAngle) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraHeight>", CameraHeight, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraHeight>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraSettings type=\""+toString(CameraType)+"\"><CameraHeight>: " << toString(CameraHeight) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraOffsetX>", CameraOffsetX, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraOffsetX>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraSettings type=\""+toString(CameraType)+"\"><CameraOffsetX>: " << toString(CameraOffsetX) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraOffsetY>", CameraOffsetY, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraOffsetY>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraSettings type=\""+toString(CameraType)+"\"><CameraOffsetY>: " << toString(CameraOffsetY) << endl << flush;

    if (!m_pXMLConfigFile->GetNodeValue(m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraOrientation>", CameraOrientation, errmsg)) {
        cout << endl << "** XML ERROR while trying to fetch "+m_sPrefix+"<CameraSettings type=\""+toString(CameraType)+"\"><CameraOrientation>" << endl << flush;
        cout << __FUNCTION__ << ": " << __FILE__ << "[" << __LINE__ << "]" << endl << flush;
        cout << "Error message from ConfigReader was: " << errmsg << endl;
        exit(1);
    }
    cout << m_sPrefix << "<CameraSettings type=\""+toString(CameraType)+"\"><CameraOrientation>: " << toString(CameraOrientation) << endl << flush;

  } else {
    cout << "*** XML ERROR: No XMLConfigReader given to Config Object. Did NOT read config!" << endl << flush;
    exit(2);
  }

}

/** Destructor. */
GeegawConfig::~GeegawConfig()
{
}

/// @endcond

