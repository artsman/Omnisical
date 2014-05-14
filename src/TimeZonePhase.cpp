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
#include "TimeZonePhase.he"
#include "Constants.he"
#include "iCalTools.he"

#include <boost/format.hpp>

using namespace OmnisTools;
using namespace iCalTools;
using namespace LibiCalConstants;
using boost::shared_ptr;
using boost::format;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

NVObjTimeZonePhase::NVObjTimeZonePhase(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

NVObjTimeZonePhase::~NVObjTimeZonePhase()
{ }

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjTimeZonePhase::copy( NVObjTimeZonePhase* pObj ) {
    NVObjBase::copy(pObj);
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cTimeZonePhaseMethodError      = 14000;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cTimeZonePhaseMethodsParamsTable[] = 
{
	// $error
    14800, fftInteger  , 0, 0,
	14801, fftCharacter, 0, 0,
	14802, fftCharacter, 0, 0,
	14803, fftCharacter, 0, 0,
	14804, fftNumber,    0, 0,
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
ECOmethodEvent cTimeZonePhaseMethodsTable[] = 
{
	cTimeZonePhaseMethodError,      cTimeZonePhaseMethodError,      fftNumber, 4, &cTimeZonePhaseMethodsParamsTable[0], 0, 0,
};

// List of methods in Simple
qlong NVObjTimeZonePhase::returnMethods(tThreadData* pThreadData)
{
	const qshort cTimeZonePhaseMethodCount = sizeof(cTimeZonePhaseMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cTimeZonePhaseMethodsTable[0], cTimeZonePhaseMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjTimeZonePhase::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	//qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
	switch( funcId )
	{
		case cTimeZonePhaseMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
	}
	
	callErrorMethod(pThreadData, result);
    
	return 0L;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cTimeZonePhasePropertyEmpty = 14400;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cTimeZonePhasePropertyTable[] = 
{
	cTimeZonePhasePropertyEmpty,      cTimeZonePhasePropertyEmpty,      fftConstant,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0
};

// List of properties in Simple
qlong NVObjTimeZonePhase::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cTimeZonePhasePropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cTimeZonePhasePropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjTimeZonePhase::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cTimeZonePhasePropertyEmpty:
			return qfalse;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjTimeZonePhase::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cTimeZonePhasePropertyEmpty:
			break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
    
	return 1L;
}

// Method to set a property of the object
qlong NVObjTimeZonePhase::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cTimeZonePhasePropertyEmpty:
			break;
	}
    
	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/


/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Empty method.  Determines call over head for a method
tResult NVObjTimeZonePhase::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
    return ERR_NOT_IMPLEMENTED;
}
