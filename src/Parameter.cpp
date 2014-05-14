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
#include "iCalTools.he"
#include "Parameter.he"
#include "Constants.he"
#include <boost/format.hpp>

using namespace OmnisTools;
using namespace LibiCalConstants;
using namespace iCalTools;
using boost::shared_ptr;
using boost::format;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

NVObjParameter::NVObjParameter(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

NVObjParameter::~NVObjParameter()
{ }

void KeepParameter (icalparameter *param) {
    // Do nothing since libical is managing the memory
}

void FreeParameter (icalparameter *param) {
    if (param) {
        icalparameter_free(param);
    }
}

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjParameter::copy( NVObjParameter* pObj ) {
    NVObjBase::copy(pObj);
    
    param = pObj->param;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cParamMethodError      = 4000,
                    cParamMethodInitialize = 4001;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cParameterMethodsParamsTable[] = 
{
	// $error
    4800, fftInteger  , 0, 0,
	4801, fftCharacter, 0, 0,
	4802, fftCharacter, 0, 0,
	4803, fftCharacter, 0, 0,
    // $initialize
    4804, fftConstant,  0, 0,
    4805, fftCharacter, EXTD_FLAG_PARAMOPT, 0
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
ECOmethodEvent cParameterMethodsTable[] = 
{
	cParamMethodError,      cParamMethodError,      fftNumber, 4, &cParameterMethodsParamsTable[0], 0, 0,
	cParamMethodInitialize, cParamMethodInitialize, fftNone,   2, &cParameterMethodsParamsTable[4], 0, 0
};

// List of methods in Simple
qlong NVObjParameter::returnMethods(tThreadData* pThreadData)
{
	const qshort cParamMethodCount = sizeof(cParameterMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cParameterMethodsTable[0], cParamMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjParameter::methodCall( tThreadData* pThreadData )
{
	tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
	switch( funcId )
	{
		case cParamMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
		case cParamMethodInitialize:
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
const static qshort cParamPropertyType      = 4400,
                    cParamPropertyTypeDesc  = 4401,
                    cParamPropertyICSOutput = 4402,
                    cParamPropertyIANAName  = 4403,
                    cParamPropertyIANAValue = 4404,
                    cParamPropertyValue     = 4405;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cParameterPropertyTable[] = 
{
	cParamPropertyType,      cParamPropertyType,      fftConstant,  EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, 0, 0,
    cParamPropertyTypeDesc,  cParamPropertyTypeDesc,  fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cParamPropertyICSOutput, cParamPropertyICSOutput, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cParamPropertyIANAName,  cParamPropertyIANAName,  fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cParamPropertyIANAValue, cParamPropertyIANAValue, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cParamPropertyValue,     cParamPropertyValue,     fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0
};

// List of properties in Simple
qlong NVObjParameter::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cParameterPropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cParameterPropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjParameter::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cParamPropertyType:
        case cParamPropertyICSOutput:
			return qfalse;
        case cParamPropertyIANAName:
        case cParamPropertyIANAValue:
        case cParamPropertyValue:
            return qtrue;
        case cParamPropertyTypeDesc:
            if (icalparameter_isa(param.get()) == ICAL_X_PARAMETER) {
                return qtrue;
            } else {
                return qfalse;
            }
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjParameter::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    if (!param) {
        ECOaddParam(pThreadData->mEci, &fValReturn);
        return qfalse;
    }
    
    EXTfldval testVal;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cParamPropertyType:
            getEXTFldValFromICalType<icalparameter_kind>(fValReturn, icalparameter_isa(param.get()), kConstResourcePrefix);
			break;
            
        case cParamPropertyTypeDesc:
            if (icalparameter_isa(param.get()) == ICAL_X_PARAMETER) {
                getEXTFldValFromChar(fValReturn, icalparameter_get_xname(param.get()));
            } else {
                getEXTFldValFromChar(fValReturn, icalparameter_kind_to_string(icalparameter_isa(param.get())));
            }
			break;
            
        case cParamPropertyICSOutput:
            getValueForType(pThreadData,testVal,param.get());  // WORKAROUND: ICS Output will crash if no value is present.
            if (testVal.isEmpty() == qfalse) {
                getEXTFldValFromChar(fValReturn, icalparameter_as_ical_string(param.get()));
            }
			break;
            
        case cParamPropertyIANAName:
            getEXTFldValFromiCalChar(fValReturn, icalparameter_get_iana_name(param.get()));
			break;
            
        case cParamPropertyIANAValue:
            getEXTFldValFromiCalChar(fValReturn, icalparameter_get_iana_value(param.get()));
			break;
            
        case cParamPropertyValue:
            getValueForType(pThreadData,fValReturn,param.get());
            break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller

	return qtrue;
}

// Method to set a property of the object
qlong NVObjParameter::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
    std::string stringAssign;
    
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cParamPropertyValue:
            setValueForType(pThreadData,fVal,param.get());
			break;
        case cParamPropertyTypeDesc:
            if (icalparameter_isa(param.get()) == ICAL_X_PARAMETER) {
                stringAssign = getStringFromEXTFldVal(fVal);
                icalparameter_set_xname(param.get(), stringAssign.c_str());
            }
			break;
        case cParamPropertyIANAName:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_iana_name(param.get(), stringAssign.c_str());
			break;
        case cParamPropertyIANAValue:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_iana_value(param.get(), stringAssign.c_str());
			break;
	}

	return qtrue;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

icalparameter* NVObjParameter::getParam() {
    return param.get();
}

void NVObjParameter::setParam(icalparameter* p, shared_ptr<icalcomponent> c, shared_ptr<icalproperty> pr) {
    comp = c;
    prop = pr;
    
    if (c || pr) {
        param = shared_ptr<icalparameter>(p, KeepParameter);
    } else {
        param = shared_ptr<icalparameter>(p, FreeParameter);
    }
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Empty method.  Determines call over head for a method
tResult NVObjParameter::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
	EXTfldval typeVal;
    icalparameter_kind typeAssign = ICAL_NO_PARAMETER;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    
    // Read parameter
    if ( getParamVar(pThreadData, 1, typeVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected parameter type constant.";
        return ERR_BAD_PARAMS;
	}
    
    // Convert Omnis constant into libical enum and create new component
    int intTest = -1;
    std::string xName;
    if (getType(typeVal).valType == fftCharacter) {
        typeAssign = ICAL_X_PARAMETER;
        xName = getStringFromEXTFldVal(typeVal);
        if ( xName.empty() || xName[0] != 'X' ) {
            pThreadData->mExtraErrorText = "First parameter, X-Name, is unrecognized.  Expected X property name; must start with 'X'.";
            return ERR_BAD_PARAMS;
        }
    } else {
        intTest = getIntFromEXTFldVal(typeVal, paramTypeLookup.first(), paramTypeLookup.last());
        if (intTest == -1  || !paramTypeLookup.isValid(intTest)) {
            pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected parameter type constant.";
            return ERR_BAD_PARAMS;
        }
        typeAssign = paramTypeLookup.get(intTest);
    }
    
    icalparameter* newParam = icalparameter_new(typeAssign);
    if (!newParam) {
        pThreadData->mExtraErrorText = str(format("Unable to create new parameter from libical. Error: %s") % icalerror_strerror(icalerrno));
        return ERR_METHOD_FAILED;
    }
    
    // If this is an X Property and the name was passed in, then set it up now.
    if (!xName.empty()) {
        icalparameter_set_xname(newParam, xName.c_str());
    }
    
    // Assign shared_ptr for current property
    setParam(newParam);
    
    // Parameter 2: (Optional) set the value of the Parameter
    EXTfldval valVal;
    if ( getParamVar(pThreadData, 2, valVal) == qtrue ) {
        setValueForType(pThreadData,valVal,param.get());
	}
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjParameter* thisCopy = createNVObj<NVObjParameter>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}
