// The MIT License (MIT)

// Copyright (c) 2014 Arts Management Systems Ltd.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <extcomp.he>
#include "TimeZone.he"
#include "Constants.he"
#include "iCalTools.he"
#include "SystemDate.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace OmnisTools;
using namespace iCalTools;
using namespace LibiCalConstants;
using boost::shared_ptr;
using boost::format;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

NVObjTimeZone::NVObjTimeZone(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst), timezone(0)
{ }

NVObjTimeZone::~NVObjTimeZone()
{ }

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjTimeZone::copy( NVObjTimeZone* pObj ) {
    NVObjBase::copy(pObj);
    
    timezone = pObj->timezone;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cTimeZoneMethodError      = 15000,
                    cTimeZoneMethodInitialize = 15001;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cTimeZoneMethodsParamsTable[] = 
{
	// $error
    15800, fftInteger  , 0, 0,
	15801, fftCharacter, 0, 0,
	15802, fftCharacter, 0, 0,
	15803, fftCharacter, 0, 0,
	15804, fftNumber,    0, 0,
    // $initialize
    15805, fftCharacter, EXTD_FLAG_PARAMOPT, 0
};

// Table of Methods available
// Columns are:
// 1) Unique ID 
// 2) Name of Method (Resource #)
// 3) Return Type 
// 4) # of Parameters
// 5) Array of Parameter Names (Taken from MethodsParamsTable.  Increments # of parameters past this pointer) 
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOmethodEvent cTimeZoneMethodsTable[] = 
{
	cTimeZoneMethodError,      cTimeZoneMethodError,      fftNone, 4, &cTimeZoneMethodsParamsTable[0], 0, 0,
    cTimeZoneMethodInitialize, cTimeZoneMethodInitialize, fftNone, 1, &cTimeZoneMethodsParamsTable[4], 0, 0,
};

// List of methods in Simple
qlong NVObjTimeZone::returnMethods(tThreadData* pThreadData)
{
	const qshort cTimeZoneMethodCount = sizeof(cTimeZoneMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cTimeZoneMethodsTable[0], cTimeZoneMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjTimeZone::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
	switch( funcId )
	{
        case cTimeZoneMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
        case cTimeZoneMethodInitialize:
            pThreadData->mCurMethodName = "$initialize";
            result = methodInitialize(pThreadData, paramCount);
            break;
	}
	
	callErrorMethod(pThreadData, result);
    
	return 0L;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cTimeZonePropertyTzid      = 15400,
                    cTimeZonePropertyName      = 15401,
                    cTimeZonePropertyLocation  = 15402,
                    cTimeZonePropertyLatitude  = 15403,
                    cTimeZonePropertyLongitude = 15404,
                    cTimeZonePropertyTzNames   = 15405;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cTimeZonePropertyTable[] = 
{
	cTimeZonePropertyTzid,      cTimeZonePropertyTzid,      fftCharacter,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cTimeZonePropertyName,      cTimeZonePropertyName,      fftCharacter,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cTimeZonePropertyLocation,  cTimeZonePropertyLocation,  fftCharacter,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cTimeZonePropertyLatitude,  cTimeZonePropertyLatitude,  fftNumber,     EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cTimeZonePropertyLongitude, cTimeZonePropertyLongitude, fftNumber,     EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cTimeZonePropertyTzNames,   cTimeZonePropertyTzNames,   fftCharacter,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0
};

// List of properties in Simple
qlong NVObjTimeZone::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cTimeZonePropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cTimeZonePropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjTimeZone::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cTimeZonePropertyTzid:
        case cTimeZonePropertyName:
        case cTimeZonePropertyLocation:            
        case cTimeZonePropertyLatitude:
        case cTimeZonePropertyLongitude:
        case cTimeZonePropertyTzNames:
			return qfalse;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjTimeZone::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cTimeZonePropertyTzid:
            getEXTFldValFromChar(fValReturn, icaltimezone_get_tzid(timezone));
            break;
        case cTimeZonePropertyName:
            getEXTFldValFromChar(fValReturn, icaltimezone_get_display_name(timezone));
            break;
        case cTimeZonePropertyLocation:
            getEXTFldValFromChar(fValReturn, icaltimezone_get_location(timezone));
            break;
        case cTimeZonePropertyLatitude:
            getEXTFldValFromDouble(fValReturn, icaltimezone_get_latitude(timezone));
            break;
        case cTimeZonePropertyLongitude:
            getEXTFldValFromDouble(fValReturn, icaltimezone_get_longitude(timezone));
            break;
        case cTimeZonePropertyTzNames:
            getEXTFldValFromChar(fValReturn, icaltimezone_get_tznames(timezone));
			break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
    
	return 1L;
}

// Method to set a property of the object
qlong NVObjTimeZone::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cTimeZonePropertyTzid:
        case cTimeZonePropertyName:
        case cTimeZonePropertyLocation:            
        case cTimeZonePropertyLatitude:
        case cTimeZonePropertyLongitude:
        case cTimeZonePropertyTzNames:
			break;
	}
    
	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

icaltimezone* NVObjTimeZone::getTimezone() {
    return timezone;
}

void NVObjTimeZone::setTimezone(icaltimezone* newTimezone) {
    timezone = newTimezone;
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Initializes the time zone.
tResult NVObjTimeZone::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
    // Parameter 1: (Optional) Location of time zone to initialize with
    EXTfldval locationVal;
    
    SystemTimeZone curZone;
    
    std::string locationString; 
    if ( getParamVar(pThreadData, 1, locationVal) == qtrue ) {
        locationString = getStringFromEXTFldVal(locationVal);
        
        if (boost::iequals(locationString,"UTC")) {
            timezone = icaltimezone_get_utc_timezone();  
        } else {
            timezone = icaltimezone_get_builtin_timezone(locationString.c_str());
        }
    } else {
        timezone = icaltimezone_get_builtin_timezone(curZone.name().c_str());
    }
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjTimeZone* thisCopy = createNVObj<NVObjTimeZone>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}
