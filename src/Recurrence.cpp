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
#include "Recurrence.he"
#include "Constants.he"
#include "iCalTools.he"

#include "SystemDate.h"

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace OmnisTools;
using namespace iCalTools;
using namespace LibiCalConstants;
using boost::shared_ptr;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::format;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

NVObjRecurrence::NVObjRecurrence(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ 
	clear();
}

NVObjRecurrence::~NVObjRecurrence()
{ }

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjRecurrence::copy( NVObjRecurrence* pObj ) {
    NVObjBase::copy(pObj);
    
    recur = pObj->recur;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cRecurrenceMethodError      = 9000,
                    cRecurrenceMethodInitialize  = 9001,
                    cRecurrenceMethodClear      = 9002,
                    cRecurrenceMethodDatesUntil = 9003;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cRecurrenceMethodsParamsTable[] = 
{
	// $error
    9800, fftInteger  , 0, 0,
	9801, fftCharacter, 0, 0,
	9802, fftCharacter, 0, 0,
	9803, fftCharacter, 0, 0,
    // $intialize
    9804, fftConstant, EXTD_FLAG_PARAMOPT, 0,
    9805, fftInteger,  EXTD_FLAG_PARAMOPT, 0,
    9806, fftObject,   EXTD_FLAG_PARAMOPT, 0,
    9807, fftConstant, EXTD_FLAG_PARAMOPT, 0,
    9808, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9809, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9810, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9811, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9812, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9813, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9814, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9815, fftList,     EXTD_FLAG_PARAMOPT, 0,
    9816, fftList,     EXTD_FLAG_PARAMOPT, 0,
    // $datesUntil
    9817, fftObject, 0, 0,
    9818, fftObject,   EXTD_FLAG_PARAMOPT, 0,
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
ECOmethodEvent cRecurrenceMethodsTable[] = 
{
	cRecurrenceMethodError,      cRecurrenceMethodError,      fftNumber, 4, &cRecurrenceMethodsParamsTable[0], 0, 0,
    cRecurrenceMethodInitialize, cRecurrenceMethodInitialize, fftNone,  13, &cRecurrenceMethodsParamsTable[4], 0, 0,
    cRecurrenceMethodClear,      cRecurrenceMethodClear,      fftNone,   0, 0, 0, 0,
    cRecurrenceMethodDatesUntil, cRecurrenceMethodDatesUntil, fftList,   2, &cRecurrenceMethodsParamsTable[17], 0, 0
};

// List of methods in Simple
qlong NVObjRecurrence::returnMethods(tThreadData* pThreadData)
{
	const qshort cRecurrenceMethodCount = sizeof(cRecurrenceMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cRecurrenceMethodsTable[0], cRecurrenceMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjRecurrence::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
	switch( funcId )
	{
		case cRecurrenceMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
        case cRecurrenceMethodInitialize:
			result = methodInitialize(pThreadData, paramCount);
			break;
        case cRecurrenceMethodClear:
			result = methodClear(pThreadData, paramCount);
			break;
        case cRecurrenceMethodDatesUntil:
			result = methodDatesUntil(pThreadData, paramCount);
			break;
	}
	
	callErrorMethod(pThreadData, result);
    
	return 0L;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cRecurrencePropertyFrequency     = 9400,
                    cRecurrencePropertyUntilDate     = 9401,
                    cRecurrencePropertyCount         = 9402,
                    cRecurrencePropertyInterval      = 9403,
                    cRecurrencePropertyWeekStart     = 9404,                    
                    cRecurrencePropertyBySecond      = 9405,
                    cRecurrencePropertyByMinute      = 9406,
                    cRecurrencePropertyByHour        = 9407,
                    cRecurrencePropertyByDay         = 9408,
                    cRecurrencePropertyByMonthDay    = 9409,
                    cRecurrencePropertyByYearDay     = 9410,
                    cRecurrencePropertyByWeekNumber  = 9411,
                    cRecurrencePropertyByMonth       = 9412,
                    cRecurrencePropertyBySetPosition = 9413,
                    cRecurrencePropertyBySecondChar      = 9414,
                    cRecurrencePropertyByMinuteChar      = 9415,
                    cRecurrencePropertyByHourChar        = 9416,
                    cRecurrencePropertyByDayChar         = 9417,
                    cRecurrencePropertyByMonthDayChar    = 9418,
                    cRecurrencePropertyByYearDayChar     = 9419,
                    cRecurrencePropertyByWeekNumberChar  = 9420,
                    cRecurrencePropertyByMonthChar       = 9421,
                    cRecurrencePropertyBySetPositionChar = 9422;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cRecurrencePropertyTable[] = 
{
	cRecurrencePropertyFrequency,         cRecurrencePropertyFrequency,         fftConstant,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyUntilDate,         cRecurrencePropertyUntilDate,         fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyCount,             cRecurrencePropertyCount,             fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyInterval,          cRecurrencePropertyInterval,          fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyWeekStart,         cRecurrencePropertyWeekStart,         fftConstant,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyBySecond,          cRecurrencePropertyBySecond,          fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByMinute,          cRecurrencePropertyByMinute,          fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByHour,            cRecurrencePropertyByHour,            fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByDay,             cRecurrencePropertyByDay,             fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByMonthDay,        cRecurrencePropertyByMonthDay,        fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByYearDay,         cRecurrencePropertyByYearDay,         fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByWeekNumber,      cRecurrencePropertyByWeekNumber,      fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByMonth,           cRecurrencePropertyByMonth,           fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyBySetPosition,     cRecurrencePropertyBySetPosition,     fftList,      EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyBySecondChar,      cRecurrencePropertyBySecondChar,      fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByMinuteChar,      cRecurrencePropertyByMinuteChar,      fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByHourChar,        cRecurrencePropertyByHourChar,        fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByDayChar,         cRecurrencePropertyByDayChar,         fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByMonthDayChar,    cRecurrencePropertyByMonthDayChar,    fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByYearDayChar,     cRecurrencePropertyByYearDayChar,     fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByWeekNumberChar,  cRecurrencePropertyByWeekNumberChar,  fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyByMonthChar,       cRecurrencePropertyByMonthChar,       fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0,
    cRecurrencePropertyBySetPositionChar, cRecurrencePropertyBySetPositionChar, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0 ,0
};

// List of properties in Simple
qlong NVObjRecurrence::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cRecurrencePropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cRecurrencePropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjRecurrence::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cRecurrencePropertyFrequency:
        case cRecurrencePropertyUntilDate:
        case cRecurrencePropertyCount:
        case cRecurrencePropertyInterval:
        case cRecurrencePropertyWeekStart:                    
        case cRecurrencePropertyBySecond:
        case cRecurrencePropertyByMinute:
        case cRecurrencePropertyByHour:
        case cRecurrencePropertyByDay:
        case cRecurrencePropertyByMonthDay:
        case cRecurrencePropertyByYearDay:
        case cRecurrencePropertyByWeekNumber:
        case cRecurrencePropertyByMonth:
        case cRecurrencePropertyBySetPosition:
			return qtrue;
        case cRecurrencePropertyBySecondChar:
        case cRecurrencePropertyByMinuteChar:
        case cRecurrencePropertyByHourChar:
        case cRecurrencePropertyByDayChar:
        case cRecurrencePropertyByMonthDayChar:
        case cRecurrencePropertyByYearDayChar:
        case cRecurrencePropertyByWeekNumberChar:
        case cRecurrencePropertyByMonthChar:
        case cRecurrencePropertyBySetPositionChar:
			return qtrue;
		default:
			return qfalse;
	}
}

// File static helper function to create a comma-delimmited string of items in an iCal Recurrence array
static std::string stringizeICalShortArray(int size, short* inArray) 
{
    if (inArray == 0 || inArray[0] == 0)
        return "";
    
    std::string stringAssign;
    
    stringAssign.clear();
    for(int x = 0; x < size && inArray[x] != ICAL_RECURRENCE_ARRAY_MAX; ++x) 
    {
        stringAssign += str(format("%d") % inArray[x]);
        
        // Add comma delimitter if not the last item
        if ( (x+1) >= size && inArray[x+1] != ICAL_RECURRENCE_ARRAY_MAX) {
            stringAssign += ",";
        }
    }
    
    return stringAssign;
}

// File static helper function to create a comma-delimmited string of items in an iCal Recurrence array
static std::string stringizeICalByDayArray(int size, short* inArray) 
{
    if (inArray == 0 || inArray[0] == 0)
        return "";
    
    std::string stringAssign;
    int dayNum;
    
    stringAssign.clear();
    for(int x = 0; x < size && inArray[x] != ICAL_RECURRENCE_ARRAY_MAX; ++x) 
    {
        // Decode recurrence number
        dayNum = icalrecurrencetype_day_position(inArray[x]);
        if(dayNum > 0) {
          stringAssign += str(format("+%d") % dayNum);
        } else if (dayNum < 0) { 
            // Negative character "-" is added by boost::format
            stringAssign += str(format("%d") % dayNum);
        } else {
            stringAssign += str(format("%d") % dayNum);
        }
        
        // Decode day of the week 
        switch (icalrecurrencetype_day_day_of_week(inArray[x])) {
            case ICAL_SUNDAY_WEEKDAY:
                stringAssign += "SU";
                break;
            case ICAL_MONDAY_WEEKDAY:
                stringAssign += "MO";
                break;
            case ICAL_TUESDAY_WEEKDAY:
                stringAssign += "TU";
                break;
            case ICAL_WEDNESDAY_WEEKDAY:
                stringAssign += "WE";
                break;
            case ICAL_THURSDAY_WEEKDAY:
                stringAssign += "TH";
                break;
            case ICAL_FRIDAY_WEEKDAY:
                stringAssign += "FR";
                break;
            case ICAL_SATURDAY_WEEKDAY:
                stringAssign += "SA";
                break;
            case ICAL_NO_WEEKDAY:
                // Add nothing
                break;
            default:
                break;
        }
        
        // Add comma delimitter if not the last item
        if ( (x+1) >= size && inArray[x+1] != ICAL_RECURRENCE_ARRAY_MAX) {
            stringAssign += ",";
        }
    }
    
    return stringAssign;
}

// File static helper function to convert an iCal recurrence array into a single-column list
static void getEXTFldValFromICalRecurrenceArray( tThreadData* pThreadData, EXTfldval& fVal, short inArray[], short size, std::string title) 
{  
    EXTqlist *retList = new EXTqlist(listVlen);
    
    // Add column to list
    str255 colName = initStr255(title.c_str());
    retList->addCol( fftInteger, dpDefault, 0, &colName );
    
    // Build list (but only if there is values)
    if (inArray != 0 && inArray[0] != 0) {
        EXTfldval colVal;
        for(int x = 0, row = 1; x < size && inArray[x] != ICAL_RECURRENCE_ARRAY_MAX; ++x && ++row) 
        {
            retList->insertRow();
            retList->getColValRef(row, 1, colVal, qtrue);
            
            getEXTFldValFromInt(colVal,inArray[x]);
        }
    }
    
    // Place list in EXTfldval container
    fVal.setList(retList, qtrue);
}


// File static helper function to convert an iCal recurrence array into a single-column list
template <int T_SIZE>
static shared_ptr<short> getICalRecurrenceArrayFromEXTFldVal( tThreadData* pThreadData, EXTfldval& fVal) 
{   	
	shared_ptr<short> retArray = shared_ptr<short>(new short[T_SIZE]);
	
	// Clear out contents
    memset(retArray.get(),ICAL_RECURRENCE_ARRAY_MAX_BYTE,sizeof(short)*T_SIZE);
    
    // Get list reference
    EXTqlist *list = fVal.getList(qfalse);
    
    // Build list
    EXTfldval colVal;
    int row, x;
    for(x = 0, row = 1; row <= list->rowCnt() && x < T_SIZE; ++x && ++row) {
        // Fetch value
        list->getColValRef(row, 1, colVal, qfalse);
        // Set position in array
        (retArray.get())[x] = getIntFromEXTFldVal(colVal);
    }

	return retArray;
}

// File static helper function to convert an iCal recurrence array into a single-column list
static void getEXTFldValFromICalRecurrenceDayArray( tThreadData* pThreadData, EXTfldval& fVal, short inArray[], short size) {
    
    EXTqlist *retList = new EXTqlist(listVlen);
    
    // Add column to list
    str255 colName;
    colName = initStr255("DayPosition");
    retList->addCol( fftInteger, dpDefault, 0, &colName );
    
    colName = initStr255("DayOfWeek");
    retList->addCol( fftConstant, dpDefault, 0, &colName );
    
    // Take values from array and place them in the list (but only if there is values)
    if (inArray != 0 && inArray[0] != 0) {
        EXTfldval colVal;
        for(int x = 0, row = 1; x < size && inArray[x] != ICAL_RECURRENCE_ARRAY_MAX; ++x && ++row) 
        {
            retList->insertRow();
            
            // Day Position
            retList->getColValRef(row, 1, colVal, qtrue);
            getEXTFldValFromInt(colVal,icalrecurrencetype_day_position(inArray[x]));
            
            // Day of Week
            retList->getColValRef(row, 2, colVal, qtrue);
            getEXTFldValFromICalType<icalrecurrencetype_weekday>(colVal, icalrecurrencetype_day_day_of_week(inArray[x]));
        }
    }
    
    // Place list in EXTfldval container
    fVal.setList(retList, qtrue);
}

// File static helper function to convert an iCal recurrence array into a single-column list
template<>
static shared_ptr<short> getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_DAY_SIZE>( tThreadData* pThreadData, EXTfldval& fVal) 
{    
    shared_ptr<short> retArray = shared_ptr<short>(new short[ICAL_BY_DAY_SIZE]);
	
	// Clear out contents
    memset(retArray.get(),ICAL_RECURRENCE_ARRAY_MAX_BYTE,sizeof(short)*ICAL_BY_DAY_SIZE);
    
    // Get list reference
    EXTqlist *list = fVal.getList(qfalse);
    
    // Build list
    EXTfldval colVal;
    int day;
    icalrecurrencetype_weekday weekday;
    
    // Take values from list and encode in the array
    int row, x;
    for(x = 0, row = 1; row <= list->rowCnt() && x < ICAL_BY_DAY_SIZE; ++row && ++x) {
        
        // Day Position
        list->getColValRef(row, 1, colVal, qfalse);
        day = getIntFromEXTFldVal(colVal);
        
        // Day of Week
        list->getColValRef(row, 2, colVal, qfalse);
        weekday = getICalTypeFromEXTFldVal<icalrecurrencetype_weekday>(colVal);
        
        // HACK: Taken from source; The array is encoded as (day position * 7) + day of week
        // SEE: icalrecur.c -> icalrecurrencetype_day_day_of_week
        retArray.get()[x] = (day*8) + weekday;
    }

	return retArray;
}

// Method to retrieve a property of the object
qlong NVObjRecurrence::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    NVObjDate *dateAssign;
    std::string stringAssign;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cRecurrencePropertyFrequency:
            // Return frequency constant
            getEXTFldValFromICalType<icalrecurrencetype_frequency>(fValReturn, recur.freq, kConstResourcePrefix);
			break;
        case cRecurrencePropertyUntilDate:            
            // Return date object with "until" date (Usually use this or $count but not both)
            dateAssign = createNVObj<NVObjDate>(pThreadData);
            dateAssign->setDateTime(recur.until);
            
            getEXTFldValForObj<NVObjDate>(fValReturn, dateAssign);
			break;
        case cRecurrencePropertyCount:
            // Return count of recurrences (Usually use this or $untilDate but not both)
            getEXTFldValFromInt(fValReturn, recur.count);
            break;
        case cRecurrencePropertyInterval:
            // Return interval between events
            getEXTFldValFromInt(fValReturn, recur.interval);
			break;
        case cRecurrencePropertyWeekStart: 
            // Return week start constant
            getEXTFldValFromICalType<icalrecurrencetype_weekday>(fValReturn, recur.week_start, kConstResourcePrefix);
			break;                  
        case cRecurrencePropertyBySecond:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_second, ICAL_BY_SECOND_SIZE, "Second");
			break;
        case cRecurrencePropertyByMinute:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_minute, ICAL_BY_MINUTE_SIZE, "Minute");
			break;
        case cRecurrencePropertyByHour:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_hour, ICAL_BY_HOUR_SIZE, "Hour");
			break;
        case cRecurrencePropertyByDay:
            // NOTE: Encoded value; slightly different routine.
            getEXTFldValFromICalRecurrenceDayArray(pThreadData, fValReturn, recur.by_day, ICAL_BY_DAY_SIZE);
			break;
        case cRecurrencePropertyByMonthDay:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_month_day, ICAL_BY_MONTHDAY_SIZE, "MonthDay");
			break;
        case cRecurrencePropertyByYearDay:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_year_day, ICAL_BY_YEARDAY_SIZE, "YearDay");
			break;
        case cRecurrencePropertyByWeekNumber:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_week_no, ICAL_BY_WEEKNO_SIZE, "WeekNumber");
			break;
        case cRecurrencePropertyByMonth:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_month, ICAL_BY_MONTH_SIZE, "Month");
			break;
        case cRecurrencePropertyBySetPosition:
            getEXTFldValFromICalRecurrenceArray(pThreadData, fValReturn, recur.by_set_pos, ICAL_BY_SETPOS_SIZE, "SetPosition");
			break;
            
        /**************************
         * READ ONLY PROPERTIES 
         **************************/
        case cRecurrencePropertyBySecondChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_SECOND_SIZE, recur.by_second));
			break;
        case cRecurrencePropertyByMinuteChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_MINUTE_SIZE, recur.by_minute));
			break;
        case cRecurrencePropertyByHourChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_HOUR_SIZE, recur.by_hour));
			break;
        case cRecurrencePropertyByDayChar:
            // NOTE: Encoded value; slightly different stringize routine.
            getEXTFldValFromString(fValReturn, stringizeICalByDayArray(ICAL_BY_DAY_SIZE, recur.by_day));
			break;
        case cRecurrencePropertyByMonthDayChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_MONTHDAY_SIZE, recur.by_month_day));
			break;
        case cRecurrencePropertyByYearDayChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_YEARDAY_SIZE, recur.by_year_day));
			break;
        case cRecurrencePropertyByWeekNumberChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_WEEKNO_SIZE, recur.by_week_no));
			break;
        case cRecurrencePropertyByMonthChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_MONTH_SIZE, recur.by_month));
			break;
        case cRecurrencePropertyBySetPositionChar:
            getEXTFldValFromString(fValReturn, stringizeICalShortArray(ICAL_BY_SETPOS_SIZE, recur.by_set_pos));
			break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
    
	return 1L;
}

// Method to set a property of the object
qlong NVObjRecurrence::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
    NVObjDate* dateAssign;
	shared_ptr<short> arrayAssign;
    
    // Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cRecurrencePropertyFrequency:
            recur.freq = getICalTypeFromEXTFldVal<icalrecurrencetype_frequency>(fVal);
			break;
        case cRecurrencePropertyUntilDate:
            dateAssign = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
            recur.until = dateAssign->getDateTime();
			break;
        case cRecurrencePropertyCount:
            recur.count = getIntFromEXTFldVal(fVal);
			break;
        case cRecurrencePropertyInterval:
            recur.interval = getIntFromEXTFldVal(fVal);
			break;
        case cRecurrencePropertyWeekStart: 
            recur.week_start = getICalTypeFromEXTFldVal<icalrecurrencetype_weekday>(fVal);
			break;                   
        case cRecurrencePropertyBySecond:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_SECOND_SIZE>(pThreadData, fVal);
            memcpy(recur.by_second,arrayAssign.get(), sizeof(short)*ICAL_BY_SECOND_SIZE);
			break;
        case cRecurrencePropertyByMinute:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_MINUTE_SIZE>(pThreadData, fVal);
			memcpy(recur.by_minute,arrayAssign.get(), sizeof(short)*ICAL_BY_MINUTE_SIZE);
			break;
        case cRecurrencePropertyByHour:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_HOUR_SIZE>(pThreadData, fVal);
			memcpy(recur.by_hour,arrayAssign.get(), sizeof(short)*ICAL_BY_HOUR_SIZE);
			break;
        case cRecurrencePropertyByDay:
            // NOTE: Encoded value; slightly different routine.
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_DAY_SIZE>(pThreadData, fVal);
			memcpy(recur.by_day,arrayAssign.get(), sizeof(short)*ICAL_BY_DAY_SIZE);
			break;
        case cRecurrencePropertyByMonthDay:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_MONTHDAY_SIZE>(pThreadData, fVal);
			memcpy(recur.by_month_day,arrayAssign.get(), sizeof(short)*ICAL_BY_MONTHDAY_SIZE);
			break;
        case cRecurrencePropertyByYearDay:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_YEARDAY_SIZE>(pThreadData, fVal);
			memcpy(recur.by_year_day,arrayAssign.get(), sizeof(short)*ICAL_BY_YEARDAY_SIZE);
			break;
        case cRecurrencePropertyByWeekNumber:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_WEEKNO_SIZE>(pThreadData, fVal);
			memcpy(recur.by_week_no,arrayAssign.get(), sizeof(short)*ICAL_BY_WEEKNO_SIZE);
			break;
        case cRecurrencePropertyByMonth:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_MONTH_SIZE>(pThreadData, fVal);
			memcpy(recur.by_month,arrayAssign.get(), sizeof(short)*ICAL_BY_MONTH_SIZE);
			break;
        case cRecurrencePropertyBySetPosition:
            arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_SETPOS_SIZE>(pThreadData, fVal);
			memcpy(recur.by_set_pos,arrayAssign.get(), sizeof(short)*ICAL_BY_SETPOS_SIZE);
			break;
	}
    
	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

// WORKAROUND: Calling icalreccurrencetype_clear() on Windows was corrupting the heap.  Moving the code locally seems to work better.
// See: libical -> icalrecur.c
void NVObjRecurrence::clear()
{
    memset(&recur,ICAL_RECURRENCE_ARRAY_MAX_BYTE,sizeof(struct icalrecurrencetype));

    recur.week_start = ICAL_MONDAY_WEEKDAY;
    recur.freq = ICAL_NO_RECURRENCE;
    recur.interval = 1;
    memset(&(recur.until),0,sizeof(struct icaltimetype));
    recur.count = 0;
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Initialize method.  Intialize a recurrence object and return a copy
tResult NVObjRecurrence::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
    EXTfldval curVal;
    
    // Save current value in case of error
    icalrecurrencetype savedRecur = recur;
     
    if(pParamCount > 0) 
    {
        // Parameter 1: (Optional) Frequency
        if (getParamVar(pThreadData, 1, curVal) == qtrue) {
            if( getType(curVal).valType == fftConstant || getType(curVal).valType == fftInteger ) {
              recur.freq = getICalTypeFromEXTFldVal<icalrecurrencetype_frequency>(curVal);
            } else {
                recur = savedRecur;
                pThreadData->mExtraErrorText = "Parameter 1 unrecognized.  Expected frequency constant.";
                return ERR_METHOD_FAILED;
            }
        }
        
        // Parameter 2: (Optional) Interval
        if (getParamVar(pThreadData, 2, curVal) == qtrue) {
            if( getType(curVal).valType == fftInteger ) {
                recur.interval = getIntFromEXTFldVal(curVal);
            } else {
                recur = savedRecur; 
                pThreadData->mExtraErrorText = "Parameter 2 unrecognized.  Expected interval integer.";
                return ERR_METHOD_FAILED;
            }
        }
        
        // Parameter 3: (Optional) Until Date or Count
        NVObjDate* omnisDateObj;
        SystemTimeZone curZone;
        if (getParamVar(pThreadData, 3, curVal) == qtrue) {
            if( getType(curVal).valType == fftInteger ) {
                recur.count = getIntFromEXTFldVal(curVal);
            } else if( getType(curVal).valType == fftObject || getType(curVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, curVal);
                if (omnisDateObj) {
                    recur.until = omnisDateObj->getDateTime();
                }  
            } else if (getType(curVal).valType == fftDate) {
                recur.until = getTimeTypeFromEXTFldVal(pThreadData, curVal);
                icaltime_set_timezone(&(recur.until), icaltimezone_get_builtin_timezone(curZone.name().c_str()));
            } else {
                recur = savedRecur;
                pThreadData->mExtraErrorText = "Parameter 3 unrecognized.  Expected date or count limit integer.";
                return ERR_METHOD_FAILED;
            }
        }
        
        // Parameter 4: (Optional) Week Start
        if (getParamVar(pThreadData, 4, curVal) == qtrue) {
            if( getType(curVal).valType == fftConstant || getType(curVal).valType == fftInteger ) {
                recur.week_start = getICalTypeFromEXTFldVal<icalrecurrencetype_weekday>(curVal);
            } else {
                recur = savedRecur;
                pThreadData->mExtraErrorText = "Parameter 4 unrecognized.  Expected week start constant.";
                return ERR_METHOD_FAILED;
            }
        }
    }
    
    // Only perform list initialization if some are passed
    if( pParamCount >= 5 ) {
        
        shared_ptr<short> arrayAssign;
        
        // Parameter 5: (Optional) Seconds
        if (getParamVar(pThreadData, 5, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_SECOND_SIZE>(pThreadData, curVal);
                memcpy(recur.by_second,arrayAssign.get(), sizeof(short)*ICAL_BY_SECOND_SIZE);
            }
        }
        
        // Parameter 6: (Optional) Minutes
        if (getParamVar(pThreadData, 6, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_MINUTE_SIZE>(pThreadData, curVal);
                memcpy(recur.by_minute,arrayAssign.get(), sizeof(short)*ICAL_BY_MINUTE_SIZE);
            }
        }
        
        // Parameter 7: (Optional) Hours
        if (getParamVar(pThreadData, 7, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_HOUR_SIZE>(pThreadData, curVal);
                memcpy(recur.by_hour,arrayAssign.get(), sizeof(short)*ICAL_BY_HOUR_SIZE);
            }
        }
        
        // Parameter 8: (Optional) Day
        if (getParamVar(pThreadData, 8, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                // NOTE: Uses slightly different function due to date packing
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_DAY_SIZE>(pThreadData, curVal);
                memcpy(recur.by_day,arrayAssign.get(), sizeof(short)*ICAL_BY_DAY_SIZE);
            }
        }
        
        // Parameter 9: (Optional) Month Day
        if (getParamVar(pThreadData, 9, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_MONTHDAY_SIZE>(pThreadData, curVal);
                memcpy(recur.by_month_day,arrayAssign.get(), sizeof(short)*ICAL_BY_MONTHDAY_SIZE);
            }
        }
        
        // Parameter 10: (Optional) Year Day
        if (getParamVar(pThreadData, 10, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_SECOND_SIZE>(pThreadData, curVal);
                memcpy(recur.by_second,arrayAssign.get(), sizeof(short)*ICAL_BY_SECOND_SIZE);
            }
        }
        
        // Parameter 11: (Optional) Week Number
        if (getParamVar(pThreadData, 11, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_WEEKNO_SIZE>(pThreadData, curVal);
                memcpy(recur.by_week_no,arrayAssign.get(), sizeof(short)*ICAL_BY_WEEKNO_SIZE);
            }
        }
        
        // Parameter 12: (Optional) Month
        if (getParamVar(pThreadData, 12, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_MONTH_SIZE>(pThreadData, curVal);
                memcpy(recur.by_month,arrayAssign.get(), sizeof(short)*ICAL_BY_MONTH_SIZE);
            }
        }
        
        // Parameter 13: (Optional) Set Position
        if (getParamVar(pThreadData, 13, curVal) == qtrue) {
            if( getType(curVal).valType == fftList ) {
                arrayAssign = getICalRecurrenceArrayFromEXTFldVal<ICAL_BY_SETPOS_SIZE>(pThreadData, curVal);
                memcpy(recur.by_set_pos,arrayAssign.get(), sizeof(short)*ICAL_BY_SETPOS_SIZE);
            }
        }
    }
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjRecurrence* thisCopy = createNVObj<NVObjRecurrence>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Clears the current contents of the object
tResult NVObjRecurrence::methodClear( tThreadData* pThreadData, qshort pParamCount )
{ 
    //icalrecurrencetype_clear(&recur);
	//recur.week_start = ICAL_MONDAY_WEEKDAY; // WORKAROUND: Windows kept assigning higher number to this enum despite it being explicitly set.
    
    clear();

    return METHOD_OK;
}

// Returns a list of dates up until the To date (param 1) and optionally dates only past the From date (param 2)
const static int MAX_ROWS = 50000;
tResult NVObjRecurrence::methodDatesUntil( tThreadData* pThreadData, qshort pParamCount )
{    
    if (pParamCount != 2) {
        pThreadData->mExtraErrorText = "Invalid parameters.  Requires exactly 2 parameters, (1) 'From' Date and (2) 'To' Date.";
        return METHOD_FAILED;
    }
    
    EXTfldval fromDateVal, param2Val;
    NVObjDate *omnisDateObj;
    SystemTimeZone curZone;
    
    // Parameter 1: From Date
    if( getParamVar(pThreadData, 1, fromDateVal) != qtrue) {
        pThreadData->mExtraErrorText = "Parameter 1, From Date, is unrecognized.  Expected Date object.";
        return METHOD_FAILED;
    }
    icaltimetype fromDate;
    if( getType(fromDateVal).valType == fftDate) {
        // Passed Omnis date or Date object
        fromDate = getTimeTypeFromEXTFldVal(pThreadData, fromDateVal);
        icaltime_set_timezone(&fromDate, icaltimezone_get_builtin_timezone(curZone.name().c_str()));
        
    } else if (getType(fromDateVal).valType == fftObject ||
               getType(fromDateVal).valType == fftObjref) 
    {
        omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fromDateVal);
        if (omnisDateObj) {
            fromDate = omnisDateObj->getDateTime();
        } else {
            pThreadData->mExtraErrorText = "Parameter 1, From Date, is unrecognized.  Expected Date object.";
            return METHOD_FAILED;
        }
    }
        
    // Parameter 2: (Optional) To Date
    icaltimetype toDate = icaltime_null_time();
    int maxRows = MAX_ROWS;
    if( getParamVar(pThreadData, 2, param2Val) == qtrue) {
        if (getType(param2Val).valType == fftInteger) {
            // Passed maximum number of returned lines
            maxRows = getIntFromEXTFldVal(param2Val);
        } else if( getType(param2Val).valType == fftDate) {
            // Passed Omnis date or Date object
            toDate = getTimeTypeFromEXTFldVal(pThreadData, param2Val);
            icaltime_set_timezone(&toDate, icaltimezone_get_builtin_timezone(curZone.name().c_str()));
        } else if (getType(param2Val).valType == fftObject ||
                   getType(param2Val).valType == fftObjref) {
            omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, param2Val);
            if (omnisDateObj) {
                toDate = omnisDateObj->getDateTime();
            }
        } else {
            pThreadData->mExtraErrorText = "Parameter 2, is unrecognized.  Expected Date object, Omnis Date, or integer.";
            return METHOD_FAILED;
        }
    }
    
    // Convert dates/times to UTC
    icaltimezone* utcZone = icaltimezone_get_utc_timezone();
    icaltimezone* fromZone = const_cast<icaltimezone*>(fromDate.zone);
    icaltimezone_convert_time(&fromDate, fromZone, utcZone);
    icaltimezone_convert_time(&toDate, const_cast<icaltimezone*>(toDate.zone), utcZone);
    
    // Create list
    EXTqlist* retList = new EXTqlist(listVlen);
    str255 colName;
    
    colName = initStr255("Date");
    retList->addCol(fftObject,dpDefault,0,&colName);
    colName = initStr255("OmnisDate");
    retList->addCol(fftDate,dpFdtimeC,0,&colName);
    
    // Iterate
	icalrecur_iterator* it = icalrecur_iterator_new(recur, fromDate);
    if( it == 0) {
        pThreadData->mExtraErrorText = str(format("Unable to determine dates. Error: %s") % icalerror_strerror(icalerrno));
        return ERR_METHOD_FAILED;
    }
    
    icaltimetype curDate = icalrecur_iterator_next(it);
    qshort row = 0;
    EXTfldval colVal, dateVal;
    NVObjDate *beforeDate, *afterDate;
    while(!icaltime_is_null_time(curDate) 
          && (icaltime_is_null_time(toDate) || (!icaltime_is_null_time(toDate) && icaltime_compare(curDate,toDate) <= 0))
          && row < maxRows) {
        // New Row
        retList->insertRow();
        ++row;
        
        icaltime_set_timezone(&curDate,icaltimezone_get_utc_timezone());
        curDate.is_utc = 0;
        
        // Convert to timezone of the first date
        icaltimezone_convert_time(&curDate, utcZone, fromZone);
        
        getEXTFldValFromTimeType(dateVal, curDate, qtrue, pThreadData);
        beforeDate = getObjForEXTfldval<NVObjDate>(pThreadData, dateVal);
        
        // Date object
        retList->getColValRef(row, 1, colVal, qtrue);
        getEXTFldValFromTimeType(colVal, curDate, qtrue, pThreadData);
        colVal = dateVal;
        
        afterDate = getObjForEXTfldval<NVObjDate>(pThreadData, colVal);
        
        // Omnis date
        retList->getColValRef(row, 2, colVal, qtrue);
        getEXTFldValFromTimeType(colVal, curDate, qfalse, pThreadData);
        
        curDate = icalrecur_iterator_next(it);
    }
    // Free iterator
    icalrecur_iterator_free(it);
    
    EXTfldval retVal;
    retVal.setList(retList,qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}
