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
#include "OmnisTools.he"
#include "Static.he"
#include "TimeZone.he"

#include "SystemDate.h"

using namespace OmnisTools;

// Define static methods
const static qshort cStaticMethodSetZoneDirectory     = 20000,
                    cStaticMethodICalErrorString      = 20001,
                    cStaticMethodGetBuiltInTimezones  = 20002,
                    cStaticMethodGetCurrentTimezone   = 20003;

// Parameters for Static Methods
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cStaticMethodsParamsTable[] = 
{
	// $setZoneDirectory
    20800, fftCharacter  , 0, 0
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
ECOmethodEvent cStaticMethodsTable[] = 
{
	cStaticMethodSetZoneDirectory,     cStaticMethodSetZoneDirectory,     fftNone,      1, &cStaticMethodsParamsTable[0], 0, 0,
    cStaticMethodICalErrorString,      cStaticMethodICalErrorString,      fftCharacter, 0,                             0, 0, 0,
    cStaticMethodGetBuiltInTimezones,  cStaticMethodGetBuiltInTimezones,  fftList,      0,                             0, 0, 0,
    cStaticMethodGetCurrentTimezone,   cStaticMethodGetCurrentTimezone,   fftRow,       0,                             0, 0, 0
};

// List of methods in Simple
qlong returnStaticMethods(tThreadData* pThreadData)
{
	const qshort cStaticMethodCount = sizeof(cStaticMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cStaticMethodsTable[0], cStaticMethodCount );
}

// Implementation of single static method
void methodStaticSetZoneDirectory(tThreadData* pThreadData, qshort paramCount) {
	EXTfldval pathVal;
    if ( getParamVar(pThreadData, 1, pathVal) != qtrue) {
        pThreadData->mExtraErrorText = "First parameter, pathName, is unrecognized.  Expected path name.";
        return;
    }
    if (ensurePosixPath(pathVal) != qtrue) {
        pThreadData->mExtraErrorText = "First parameter, pathName, is unrecognized.  Expected path name.";
        return;
    }
    
    // Convert to std::string
    std::string path = getStringFromEXTFldVal(pathVal);
    
    // Pass to libical
    set_zone_directory( const_cast<char*>(path.c_str()) );  // Directory expected as non-const char*
	
	return;
}

// Implementation of single static method
void methodStaticICalErrorString(tThreadData* pThreadData, qshort paramCount) {
    EXTfldval retVal;
    
    // Retreive error from ical
    getEXTFldValFromChar(retVal, icalerror_strerror(icalerrno));
	
    // Return error string to caller
    ECOaddParam(pThreadData->mEci, &retVal);
    
	return;
}

// Fetch all time zones built into libical
void methodStaticBuiltInTimezones(tThreadData* pThreadData, qshort paramCount) {
    
    EXTqlist* listVal = new EXTqlist(listVlen);  
    EXTfldval colName, colValue, retVal;
    NVObjTimeZone* omnisTimezone = 0;
    
    // Setup list definition
    getEXTFldValFromChar(colName, "TZID");
    listVal->addCol( fftCharacter, dpFcharacter, 10000000, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "Name");
    listVal->addCol( fftCharacter, dpFcharacter, 10000000, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "Location");
    listVal->addCol( fftCharacter, dpFcharacter, 10000000, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "Latitude");
    listVal->addCol( fftNumber, dpDefault, 0, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "Longitude");
    listVal->addCol( fftNumber, dpDefault, 0, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "TZNames");
    listVal->addCol( fftCharacter, dpFcharacter, 10000000, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "Timezone");
    listVal->addCol( fftObject, dpDefault, 0, &colName.getChar(qtrue) );
    
    // Fetch all time built-in timezones
    icalarray* zones = icaltimezone_get_builtin_timezones();
    icaltimezone *currentZone = 0;
    
    // Loop all elements in array and create list of time zones
    for (int x = 0; x < (int) zones->num_elements; ++x) {
        listVal->insertRow();
        
        currentZone = (icaltimezone*) icalarray_element_at(zones, x);

        // Setup tzid column
        listVal->getColValRef(x+1, 1, colValue, qtrue);
        colValue.setEmpty(fftCharacter, dpFcharacter);
        if (currentZone) {
            getEXTFldValFromChar(colValue, icaltimezone_get_tzid(currentZone));
        }
        
        // Setup name column
        listVal->getColValRef(x+1, 2, colValue, qtrue);
        colValue.setEmpty(fftCharacter, dpFcharacter);
        if (currentZone) {
            getEXTFldValFromChar(colValue, icaltimezone_get_display_name(currentZone));
        }
        
        // Setup location column
        listVal->getColValRef(x+1, 3, colValue, qtrue);
        colValue.setEmpty(fftCharacter, dpFcharacter);
        if (currentZone) {
            getEXTFldValFromChar(colValue, icaltimezone_get_location(currentZone));
        }
        
        // Setup latitude column
        listVal->getColValRef(x+1, 4, colValue, qtrue);
        colValue.setEmpty(fftNumber, dpDefault);
        if (currentZone) {
            getEXTFldValFromDouble(colValue, icaltimezone_get_latitude(currentZone));
        }
        
        // Setup longitude column
        listVal->getColValRef(x+1, 5, colValue, qtrue);
        colValue.setEmpty(fftNumber, dpDefault);
        if (currentZone) {
            getEXTFldValFromDouble(colValue, icaltimezone_get_longitude(currentZone));
        }
        
        // Setup tznames column
        listVal->getColValRef(x+1, 6, colValue, qtrue);
        colValue.setEmpty(fftCharacter, dpFcharacter);
        if (currentZone) {
            getEXTFldValFromChar(colValue, icaltimezone_get_tznames(currentZone));
        }
        
        // Setup object column
        listVal->getColValRef(x+1, 7, colValue, qtrue);
        colValue.setEmpty(fftObject, dpDefault);
        
        omnisTimezone = createNVObj<NVObjTimeZone>(pThreadData);
        if (omnisTimezone && currentZone) {
            omnisTimezone->setTimezone(currentZone);
            getEXTFldValForObj(colValue, omnisTimezone);
        }
    }
    
    // Return list to caller
    retVal.setList(listVal, qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
	return;
}

// Return row with current time zone
void methodStaticCurrentTimezone(tThreadData* pThreadData, qshort paramCount) {
    
    EXTqlist* listVal = new EXTqlist(listVlen);  
    EXTfldval colName, colValue, retVal;
    SystemTimeZone curZone;
    
    // Setup list definition
    getEXTFldValFromChar(colName, "Name");
    listVal->addCol( fftCharacter, dpFcharacter, 10000000, &colName.getChar(qtrue) );
    
    getEXTFldValFromChar(colName, "IsDaylight");
    listVal->addCol( fftBoolean, dpDefault, 0, &colName.getChar(qtrue) );
    
    // Insert single row 
    listVal->insertRow();
    listVal->getColValRef(1, 1, colValue, qtrue);
    getEXTFldValFromString(colValue, curZone.name());
    
    listVal->getColValRef(1, 2, colValue, qtrue);
    getEXTFldValFromBool(colValue, curZone.isDaylight());
    
    // Set current line
    listVal->setCurRow(1);
    
    // Return list to caller
    retVal.setList(listVal, qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
}

// Static method dispatch
qlong staticMethodCall( OmnisTools::tThreadData* pThreadData ) {
	
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
	
	switch( funcId )
	{
		case cStaticMethodSetZoneDirectory:
			pThreadData->mCurMethodName = "$setZoneDirectory";
			methodStaticSetZoneDirectory(pThreadData, paramCount);
			break;
        case cStaticMethodICalErrorString:
			pThreadData->mCurMethodName = "$icalErrorString";
			methodStaticICalErrorString(pThreadData, paramCount);
			break;
        case cStaticMethodGetBuiltInTimezones:
			pThreadData->mCurMethodName = "$getBuiltinTimezones";
			methodStaticBuiltInTimezones(pThreadData, paramCount);
			break;
        case cStaticMethodGetCurrentTimezone:
			pThreadData->mCurMethodName = "$getCurrentTimezone";
			methodStaticCurrentTimezone(pThreadData, paramCount);
			break;
	}
	
	return 0L;
}

