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
#include "TimeSpan.he"
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

NVObjTimeSpan::NVObjTimeSpan(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

NVObjTimeSpan::~NVObjTimeSpan()
{ }

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjTimeSpan::copy( NVObjTimeSpan* pObj ) {
    NVObjBase::copy(pObj);
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cTimeSpanMethodError      = 16000;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cTimeSpanMethodsParamsTable[] = 
{
	// $error
    16800, fftInteger  , 0, 0,
	16801, fftCharacter, 0, 0,
	16802, fftCharacter, 0, 0,
	16803, fftCharacter, 0, 0,
	16804, fftNumber,    0, 0,
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
ECOmethodEvent cTimeSpanMethodsTable[] = 
{
	cTimeSpanMethodError,      cTimeSpanMethodError,      fftNumber, 4, &cTimeSpanMethodsParamsTable[0], 0, 0,
};

// List of methods in Simple
qlong NVObjTimeSpan::returnMethods(tThreadData* pThreadData)
{
	const qshort cTimeSpanMethodCount = sizeof(cTimeSpanMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cTimeSpanMethodsTable[0], cTimeSpanMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjTimeSpan::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	//qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
	switch( funcId )
	{
		case cTimeSpanMethodError:
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
const static qshort cTimeSpanPropertyEmpty = 16400;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cTimeSpanPropertyTable[] = 
{
	cTimeSpanPropertyEmpty,      cTimeSpanPropertyEmpty,      fftConstant,  EXTD_FLAG_PROPCUSTOM, 0, 0 ,0
};

// List of properties in Simple
qlong NVObjTimeSpan::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cTimeSpanPropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cTimeSpanPropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjTimeSpan::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cTimeSpanPropertyEmpty:
			return qfalse;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjTimeSpan::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cTimeSpanPropertyEmpty:
			break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
    
	return 1L;
}

// Method to set a property of the object
qlong NVObjTimeSpan::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cTimeSpanPropertyEmpty:
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
tResult NVObjTimeSpan::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
    return ERR_NOT_IMPLEMENTED;
}
