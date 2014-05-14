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
#include "Value.he"
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

NVObjValue::NVObjValue(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

NVObjValue::~NVObjValue()
{ }

void KeepValue (icalvalue *value) {
    // Do nothing since libical is managing the memory
}

void FreeValue (icalvalue *value) {
    if (value) {
        icalvalue_free(value);
    }
}

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjValue::copy( NVObjValue* pObj ) {
    NVObjBase::copy(pObj);
    
    val = pObj->val;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cValueMethodError      = 5000,
                    cValueMethodInitialize = 5001;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cValueMethodsParamsTable[] = 
{
	// $error
    5800, fftInteger  , 0, 0,
	5801, fftCharacter, 0, 0,
	5802, fftCharacter, 0, 0,
	5803, fftCharacter, 0, 0,
    // $initialize
    5804, fftConstant,  0, 0,
    5805, fftCharacter, EXTD_FLAG_PARAMOPT, 0
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
ECOmethodEvent cValueMethodsTable[] = 
{
	cValueMethodError,      cValueMethodError,      fftNumber, 4, &cValueMethodsParamsTable[0], 0, 0,
	cValueMethodInitialize, cValueMethodInitialize, fftNone,   2, &cValueMethodsParamsTable[4], 0, 0
};

// List of methods in Simple
qlong NVObjValue::returnMethods(tThreadData* pThreadData)
{
	const qshort cValueMethodCount = sizeof(cValueMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cValueMethodsTable[0], cValueMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjValue::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);

	switch( funcId )
	{
		case cValueMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
		case cValueMethodInitialize:
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
const static qshort cValuePropertyType      = 5400,
                    cValuePropertyTypeDesc  = 5401,
                    cValuePropertyICSOutput = 5402,
                    cValuePropertyValue     = 5403;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cValuePropertyTable[] = 
{
	cValuePropertyType,      cValuePropertyType,      fftConstant,  EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, 0, 0,
    cValuePropertyTypeDesc,  cValuePropertyTypeDesc,  fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cValuePropertyICSOutput, cValuePropertyICSOutput, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cValuePropertyValue,     cValuePropertyValue,     fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0
};

// List of properties in Simple
qlong NVObjValue::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cValuePropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cValuePropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjValue::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cValuePropertyType:
        case cValuePropertyTypeDesc:
        case cValuePropertyICSOutput:
			return qfalse;
        case cValuePropertyValue:
            return qtrue;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjValue::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    if (!val) {
        ECOaddParam(pThreadData->mEci, &fValReturn);
        return qfalse;
    }
    
    EXTfldval testVal;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cValuePropertyType:
            getEXTFldValFromICalType<icalvalue_kind>(fValReturn, icalvalue_isa(val.get()), kConstResourcePrefix);
			break;
            
        case cValuePropertyTypeDesc:
            getEXTFldValFromChar(fValReturn, icalvalue_kind_to_string(icalvalue_isa(val.get())));
			break;
            
        case cValuePropertyICSOutput:
            getValueForType(pThreadData,testVal,val.get());  // WORKAROUND: ICS Output will crash if no value is present.
            if (testVal.isEmpty() == qfalse) {
                getEXTFldValFromChar(fValReturn, icalvalue_as_ical_string(val.get()));
            }
            break;
            
        case cValuePropertyValue:
            getValueForType(pThreadData,fValReturn,val.get());
            break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller

	return 1L;
}

// Method to set a property of the object
qlong NVObjValue::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;

	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cValuePropertyValue:
            setValueForType(pThreadData,fVal,val.get());
			break;
	}

	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

icalvalue* NVObjValue::getVal() {
    return val.get();
}

void NVObjValue::setVal(icalvalue* v, shared_ptr<icalcomponent> c, shared_ptr<icalproperty> pr) {
    comp = c;
    prop = pr;
    
    if (c || pr) {
        val = shared_ptr<icalvalue>(v, KeepValue);
    } else {
        val = shared_ptr<icalvalue>(v, FreeValue);
    }
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Empty method.  Determines call over head for a method
tResult NVObjValue::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
	EXTfldval typeVal;
    icalvalue_kind typeAssign = ICAL_NO_VALUE;
    ConstantLookup<icalvalue_kind> valTypeLookup;
    
    // Parameter 1: Type to use
    if ( getParamVar(pThreadData, 1, typeVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected value type constant.";
        return ERR_BAD_PARAMS;
	}
    int intTest = getIntFromEXTFldVal(typeVal, valTypeLookup.first(), valTypeLookup.last());
    if (intTest == -1  || !valTypeLookup.isValid(intTest)) {
        pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected value type constant.";
        return ERR_BAD_PARAMS;
    }
    
    // Convert Omnis constant into libical enum and create new component
    typeAssign = valTypeLookup.get(intTest);
    icalvalue* newVal = icalvalue_new(typeAssign);
    if (!newVal) {
        pThreadData->mExtraErrorText = str(format("Unable to create new value from libical. Error: %s") % icalerror_strerror(icalerrno));
        return ERR_METHOD_FAILED;
    }
    
    // Assign shared_ptr for current property
    setVal(newVal);
    
    // Parameter 2: (Optional) set the value of the Value
    EXTfldval valVal;
    if ( getParamVar(pThreadData, 2, valVal) == qtrue ) {
        setValueForType(pThreadData,valVal,val.get());
	}
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjValue* thisCopy = createNVObj<NVObjValue>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}
