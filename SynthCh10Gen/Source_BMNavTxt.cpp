
#include <cassert>
#include <cstdio>
#include <string>

#include "Source_BMNavTxt.h"

/*
actime      Time
aclatd      Latitude (degrees)
aclond      Longitude (degrees)
acaltf      Aircraft altitude above sea level (ft)
acktas      True Airspeed
acvifps     Total Velocity
acvxi       X Velocity
acvyi       Y Velocity
acvzi       Z Velocity
acaxi       Aircraft inertial x-coordinate acceleration (f/s^2)
acayi       Aircraft inertial y-coordinate acceleration (f/s^2)
acazi       Aircraft inertial z-coordinate acceleration (f/s^2)
acphid      Roll (degrees)
acthtad     Pitch (degrees)
acpsid      True Heading (degrees)
acmagd      Magnetic Heading (degrees)
acaoad      Angle of Attack
acthro      Throttle (%)
acnzb       Aircraft Body Normal Acceleration (f/s^2)
acazb       Aircraft body z-coordinate acceleration (f/s^2)
acgear      Aircraft landing gear position
*/

double fDmsToD(char * szDMS);

// ----------------------------------------------------------------------------
// Constructor / Destructor
// ----------------------------------------------------------------------------

ClSource_BMNavTxt::ClSource_BMNavTxt(ClSimState * pclSimState, std::string sPrefix)
    {
    this->pclSimState = pclSimState;
    this->sPrefix     = sPrefix;
    }


// ----------------------------------------------------------------------------

ClSource_BMNavTxt::~ClSource_BMNavTxt()
    {
    Close();
    }


// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

/// Open the BlueMax data file

bool ClSource_BMNavTxt::Open(std::string sFilename)
    {
    char                szLine[1000];
    char                szTrimmedLabel[100];
    char              * szLabel;
    std::string         sDataLabelKey;
    int                 iTokens;

    hBMInput = fopen(sFilename.c_str(), "r");
    if (hBMInput == NULL)
        return false;

    // Get the first header line
    fgets(szLine, sizeof(szLine), hBMInput);

    // Make the list of data labels and populate data map
    // This is the current minimum list
    // UserVariable actime aclatd aclond acaltf acktas acvifps acvxi acvyi acvzi acaxi acayi acazi acphid acthtad acpsid acmagd
    DataLabel.clear();
//    State.clear();
    iTokens = 1;
    szLabel = strtok(szLine, "\t");
    while (iTokens == 1)
        {
        iTokens = sscanf(szLabel, "%s", szTrimmedLabel);
        szLabel = strtok(NULL, "\t");
        if (iTokens == 1)
            {
            sDataLabelKey = sPrefix + szTrimmedLabel;
            DataLabel.insert(DataLabel.end(), sDataLabelKey);
            pclSimState->insert(sDataLabelKey,-1.0);
            }
        } // end while tokenizing line

    return true;

    } // end Open()


// ----------------------------------------------------------------------------

/// Close the BlueMax data file

void ClSource_BMNavTxt::Close()
    {
    if (hBMInput != NULL)
        fclose(hBMInput);

    return;
    } // end Close()

// ----------------------------------------------------------------------------

/// Read the next line of BlueMax data

bool ClSource_BMNavTxt::ReadNextLine()
    {
//    int                 iItems;
    char                szLine[2000];

    // Get the next line        
    fgets(szLine, sizeof(szLine), hBMInput);
    if (feof(hBMInput))
        return false;

    // Tokenize the line and store values with the appropriate data label
    char  * szDataItem;
    double  fDataItem;
    int     iTokens = 1;
    auto    itDataLabel = std::begin(DataLabel);
    szDataItem = strtok(szLine, "\t");
    while ((iTokens == 1) && (itDataLabel != std::end(DataLabel)))
        {
        iTokens = sscanf(szDataItem, "%lf", &fDataItem);
        szDataItem = strtok(NULL, "\t");
        if (iTokens == 1)
            {
            pclSimState->update(*itDataLabel, fDataItem);
            itDataLabel++;
            }
        } // end while tokenizing line

    return true;
    }


// ----------------------------------------------------------------------------

// Convert DMS string to degrees float
// 035:41:45.24:N

double fDmsToD(char * szDMS)
    {
    int     iItems;
    int     iDegrees;
    int     iMinutes;
    float   fSeconds;
    char    chNSEW;
    double  fDegrees;

    iItems = sscanf(szDMS, "%d:%d:%f:%c", &iDegrees, &iMinutes, &fSeconds, &chNSEW);
    fDegrees = iDegrees + iMinutes / 60.0 + fSeconds / 3600.0;
    if ((chNSEW == 'W') || (chNSEW == 'S'))
        fDegrees = -fDegrees;

    return fDegrees;
    }