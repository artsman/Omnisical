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
#include "Date.he"
#include "Constants.he"
#include "iCalTools.he"

#include "TimeZone.he"
#include "SystemDate.h"

#include <ctime>
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

NVObjDate::NVObjDate(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst) { }

NVObjDate::~NVObjDate()
{ }

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjDate::copy( NVObjDate* pObj ) {
    NVObjBase::copy(pObj);
    
    datetime = pObj->datetime;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cDateMethodError      = 6000,
                    cDateMethodInitialize = 6001,
                    cDateMethodConvertTo  = 6002;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cDateMethodsParamsTable[] = 
{
	// $error
    6800, fftInteger  , 0, 0,
	6801, fftCharacter, 0, 0,
	6802, fftCharacter, 0, 0,
	6803, fftCharacter, 0, 0,
	6804, fftNumber,    0, 0,
    // $initialize
    6805, fftDate,      0, 0,
    6805, fftObject,    EXTD_FLAG_PARAMOPT, 0,
    // $convertTo
    6806, fftObject,    0, 0
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
ECOmethodEvent cDateMethodsTable[] = 
{
	cDateMethodError,      cDateMethodError,      fftNone,   4, &cDateMethodsParamsTable[0], 0, 0,
    cDateMethodInitialize, cDateMethodInitialize, fftNone,   2, &cDateMethodsParamsTable[4], 0, 0,
    cDateMethodConvertTo,  cDateMethodConvertTo,  fftObject, 1, &cDateMethodsParamsTable[6], 0, 0
};

// List of methods in Simple
qlong NVObjDate::returnMethods(tThreadData* pThreadData)
{
	const qshort cDateMethodCount = sizeof(cDateMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cDateMethodsTable[0], cDateMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjDate::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
	switch( funcId )
	{
		case cDateMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
        case cDateMethodInitialize:
            pThreadData->mCurMethodName = "$initialize";
            result = methodInitialize(pThreadData, paramCount);
            break;
        case cDateMethodConvertTo:
            pThreadData->mCurMethodName = "$convertTo";
            result = methodConvertTo(pThreadData, paramCount);
            break;
	}
	
	callErrorMethod(pThreadData, result);
    
	return 0L;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cDatePropertyYear         = 6400,
                    cDatePropertyMonth        = 6401,
                    cDatePropertyDay          = 6402,
                    cDatePropertyHour         = 6403,
                    cDatePropertyMinute       = 6404,
                    cDatePropertySecond       = 6405,
                    cDatePropertyIsUTC        = 6406,
                    cDatePropertyIsDate       = 6407,
                    cDatePropertyIsDaylight   = 6408,
                    cDatePropertyTimezone     = 6409,
                    cDatePropertyTimezoneDesc = 6410,
                    cDatePropertyDate         = 6411;

// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cDatePropertyTable[] = 
{
	cDatePropertyYear,         cDatePropertyYear,         fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyMonth,        cDatePropertyMonth,        fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyDay,          cDatePropertyDay,          fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyHour,         cDatePropertyHour,         fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyMinute,       cDatePropertyMinute,       fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertySecond,       cDatePropertySecond,       fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyIsUTC,        cDatePropertyIsUTC,        fftBoolean,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyIsDate,       cDatePropertyIsDate,       fftBoolean,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyIsDaylight,   cDatePropertyIsDaylight,   fftBoolean,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyTimezone,     cDatePropertyTimezone,     fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyTimezoneDesc, cDatePropertyTimezoneDesc, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cDatePropertyDate,         cDatePropertyDate,         fftDate,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0
};

// List of properties in Simple
qlong NVObjDate::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cDatePropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cDatePropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjDate::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
        case cDatePropertyYear:
        case cDatePropertyMonth:
        case cDatePropertyDay:
        case cDatePropertyHour:
        case cDatePropertyMinute:
        case cDatePropertySecond:
        case cDatePropertyTimezone:
        case cDatePropertyDate:
            return qtrue;
        case cDatePropertyIsUTC:
        case cDatePropertyIsDate:
        case cDatePropertyIsDaylight:
        case cDatePropertyTimezoneDesc:
			return qfalse;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjDate::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    
    NVObjTimeZone* omnisTimezone;
    icaltimezone* timezoneAssign;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cDatePropertyYear:
            getEXTFldValFromInt(fValReturn, datetime.year);
            break;
        case cDatePropertyMonth:
            getEXTFldValFromInt(fValReturn, datetime.month);
            break;
        case cDatePropertyDay:
            getEXTFldValFromInt(fValReturn, datetime.day);
            break;
        case cDatePropertyHour:
            getEXTFldValFromInt(fValReturn, datetime.hour);
            break;
        case cDatePropertyMinute:
            getEXTFldValFromInt(fValReturn, datetime.minute);
            break;
        case cDatePropertySecond:
            getEXTFldValFromInt(fValReturn, datetime.second);
            break;
        case cDatePropertyIsUTC:
            getEXTFldValFromBool(fValReturn, icaltime_is_utc(datetime) == 1);
            break;
        case cDatePropertyIsDate:
            getEXTFldValFromBool(fValReturn, icaltime_is_date(datetime) == 1);
            break;
        case cDatePropertyIsDaylight:
            getEXTFldValFromBool(fValReturn, datetime.is_daylight == 1);
            break;
        case cDatePropertyTimezone:
            timezoneAssign = const_cast<icaltimezone*>(icaltime_get_timezone(datetime));
            if (timezoneAssign && datetime.is_date != 1) {
                omnisTimezone = createNVObj<NVObjTimeZone>(pThreadData);
                if (omnisTimezone) {
                    omnisTimezone->setTimezone(timezoneAssign);
                    getEXTFldValForObj<NVObjTimeZone>(fValReturn, omnisTimezone);
                }
            }
            break;
        case cDatePropertyTimezoneDesc:
            // Fetch the display name for the time zone
            
            timezoneAssign = const_cast<icaltimezone*>(icaltime_get_timezone(datetime));
            if (timezoneAssign && datetime.is_date != 1) {
                getEXTFldValFromChar(fValReturn, icaltimezone_get_display_name(timezoneAssign));
            }
            
            break;
        case cDatePropertyDate:
            getEXTFldValFromTimeType(fValReturn, datetime);
            break;
    }

    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
    
	return 1L;
}

// Method to set a property of the object
qlong NVObjDate::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
    NVObjTimeZone* omnisTimezone;
    icaltimezone* timezoneAssign;
    
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cDatePropertyYear:
            datetime.year = getIntFromEXTFldVal(fVal);
            break;
        case cDatePropertyMonth:
            datetime.month = getIntFromEXTFldVal(fVal);
            break;
        case cDatePropertyDay:
            datetime.day = getIntFromEXTFldVal(fVal);
            break;
        case cDatePropertyHour:
            datetime.hour = getIntFromEXTFldVal(fVal);
            break;
        case cDatePropertyMinute:
            datetime.minute = getIntFromEXTFldVal(fVal);
            break;
        case cDatePropertySecond:
            datetime.second = getIntFromEXTFldVal(fVal);
            break;
        case cDatePropertyTimezone:
            if( datetime.is_date != 1) { // Timezone on a date is irrelevant
                omnisTimezone = getObjForEXTfldval<NVObjTimeZone>(pThreadData, fVal);
                if( omnisTimezone ) {
                    timezoneAssign = omnisTimezone->getTimezone();
                    if (timezoneAssign) {
                        icaltime_set_timezone(&datetime, timezoneAssign);
                    }
                }
            }
            break;
        case cDatePropertyDate:
            datetime = getTimeTypeFromEXTFldVal(pThreadData, fVal);
			break;
	}
    
	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

icaltimetype NVObjDate::getDateTime() {
    return datetime;
}

void NVObjDate::setDateTime(const icaltimetype& newDate) {
    datetime = newDate;
}

void NVObjDate::setCurrentTimeZone(icaltimetype& datetime) {
    icaltimezone* utcZone = icaltimezone_get_builtin_timezone("UTC");
    icaltime_set_timezone(&datetime, utcZone);
    
    // Get current time zone and convert to it (if it's not UTC)
    SystemTimeZone curZone;
    icaltimezone* icalZone;
    std::string location = curZone.name();
    if (location.compare("UTC") != 0) {
        icalZone = icaltimezone_get_builtin_timezone(location.c_str());
        
        // Convert to current time zone
        datetime = icaltime_convert_to_zone(datetime, icalZone);
    }
}


/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Intialize new date object from Omnis date
tResult NVObjDate::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
    // Parameter 1: (Optional) Omnis date to use
    EXTfldval dateVal;
    if ( getParamVar(pThreadData, 1, dateVal) == qtrue ) {
        if (getType(dateVal).valType != fftDate) {
            pThreadData->mExtraErrorText = "Unrecognized first parameter, omnisDate.  Expected Omnis date";
            return ERR_BAD_PARAMS;
        }
        
	}
    
    // Parameter 2: (Optional) Location of time zone to initialize with
    EXTfldval timezoneVal;
    
    // Default to UTC
    std::string locationString;
    SystemTimeZone curZone;
    icaltimezone* timezoneAssign = icaltimezone_get_builtin_timezone(curZone.name().c_str());
    NVObjTimeZone* omnisZone = 0;
    
    if ( getParamVar(pThreadData, 2, timezoneVal) == qtrue ) {
        if (getType(timezoneVal).valType == fftCharacter) {
            // Passed in timezone location
            locationString = getStringFromEXTFldVal(timezoneVal);
            if (boost::iequals(locationString,"UTC")) {
                timezoneAssign = icaltimezone_get_utc_timezone();
            } else {
                timezoneAssign = icaltimezone_get_builtin_timezone(locationString.c_str());
                if (!timezoneAssign) {
                    pThreadData->mExtraErrorText = str(format("Unrecognized location, \"%s\", in second parameter, timezone.  Expected timezone object or timezone location.") % locationString);
                    return ERR_BAD_PARAMS;
                }
            }
            
        } else if (getType(timezoneVal).valType == fftObject) {
            // Passed in Omnis Timezone object
            omnisZone = getObjForEXTfldval<NVObjTimeZone>(pThreadData, timezoneVal);
            if (!omnisZone) {
                pThreadData->mExtraErrorText = "Unrecognized timezone object in second parameter, timezone.  Expected timezone object or timezone location.";
                return ERR_BAD_PARAMS;
            }
            
            timezoneAssign = omnisZone->getTimezone();
            if (!omnisZone) {
                pThreadData->mExtraErrorText = "Unrecognized data within timezone object in second parameter, timezone.  Expected timezone object or timezone location.";
                return ERR_BAD_PARAMS;
            }
            
        } else {
            pThreadData->mExtraErrorText = "Unrecognized second parameter, timezone.  Expected timezone object or timezone location.";
            return ERR_BAD_PARAMS;
        }
	}
    // By this point we must have a timezoneAssign pointer (either default or from the 2nd parameter
    
    // Assign datetime and time zone
    if( pParamCount > 0) {
        // Use parameters
        datetime = getTimeTypeFromEXTFldVal(pThreadData, dateVal);
        icaltime_set_timezone(&datetime, timezoneAssign);
    } else {
        // Use current time
        datetime = icaltime_from_timet(time(NULL), 0);
        setCurrentTimeZone(datetime);
    }
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjDate* thisCopy = createNVObj<NVObjDate>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Convert date to different time zone
tResult NVObjDate::methodConvertTo( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!icaltime_is_valid_time(datetime)) {
        pThreadData->mExtraErrorText = "Date invalid.  Date must be valid before converting timezones.";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval timezoneVal;
    
    // Default to UTC
    std::string locationString; 
    icaltimezone* timezoneAssign = 0;
    NVObjTimeZone* omnisZone = 0;
    
    // Parameter 1: Location of time zone to convert to
    if ( getParamVar(pThreadData, 1, timezoneVal) != qtrue ) {
        pThreadData->mExtraErrorText = "Unrecognized first parameter, timezone.  Expected timezone object or timezone location.";
        return ERR_BAD_PARAMS;
    }
    
    if (getType(timezoneVal).valType == fftCharacter) {
        // Passed in timezone location
        locationString = getStringFromEXTFldVal(timezoneVal);
        if (boost::iequals(locationString,"UTC")) {
            timezoneAssign = icaltimezone_get_utc_timezone();
        } else {
            timezoneAssign = icaltimezone_get_builtin_timezone(locationString.c_str());
            if (!timezoneAssign) {
                pThreadData->mExtraErrorText = str(format("Unrecognized location, \"%s\", in first parameter, timezone.  Expected timezone object or timezone location.") % locationString);
                return ERR_BAD_PARAMS;
            }
        }
        
    } else if (getType(timezoneVal).valType == fftObject) {
        // Passed in Omnis Timezone object
        omnisZone = getObjForEXTfldval<NVObjTimeZone>(pThreadData, timezoneVal);
        if (!omnisZone) {
            pThreadData->mExtraErrorText = "Unrecognized timezone object in first parameter, timezone.  Expected timezone object or timezone location.";
            return ERR_BAD_PARAMS;
        }
        
        timezoneAssign = omnisZone->getTimezone();
        if (!omnisZone) {
            pThreadData->mExtraErrorText = "Unrecognized data within timezone object in first parameter, timezone.  Expected timezone object or timezone location.";
            return ERR_BAD_PARAMS;
        }
    } else {
        pThreadData->mExtraErrorText = "Unrecognized first parameter, timezone.  Expected timezone object or timezone location.";
        return ERR_BAD_PARAMS;
    }
    
    // Convert time zone
    datetime = icaltime_convert_to_zone(datetime, timezoneAssign);
    
    return METHOD_DONE_RETURN;
}
