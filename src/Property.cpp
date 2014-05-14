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
#include "Property.he"
#include "Parameter.he"
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

NVObjProperty::NVObjProperty(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

NVObjProperty::~NVObjProperty()
{ }

void KeepProperty (icalproperty *prop) {
    // Do nothing since libical is managing the memory
}

void FreeProperty (icalproperty *prop) {
    if (prop) {
        icalproperty_free(prop);
    }
}

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjProperty::copy( NVObjProperty* pObj ) {
    NVObjBase::copy(pObj);
    
    prop = pObj->prop;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cPropMethodError            = 3000,
					cPropMethodInitialize       = 3001,
					cPropMethodFirstParameter   = 3002,
                    cPropMethodNextParameter    = 3003,
                    cPropMethodAddParameter     = 3004,
                    cPropMethodRemoveParameter  = 3005,
                    cPropMethodFirstParameterValue = 3006,
                    cPropMethodNextParameterValue  = 3007;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cPropMethodsParamsTable[] = 
{
	// $error
    3800, fftInteger  , 0, 0,
	3801, fftCharacter, 0, 0,
	3802, fftCharacter, 0, 0,
	3803, fftCharacter, 0, 0,
    // $initialize
    3804, fftConstant,  0, 0,
    3805, fftCharacter, EXTD_FLAG_PARAMOPT, 0,
    // $firstparameter
    3806, fftConstant,  EXTD_FLAG_PARAMOPT, 0,
    // $nextparameter
    3807, fftConstant,  EXTD_FLAG_PARAMOPT, 0,
    // $addParameter
    3808, fftObject,  0, 0,
    // $removeParameter
    3809, fftObject,  0, 0,
    // $firstParameterValue
    3810, fftConstant,  0, 0,
    // $nextParameterValue
    3811, fftConstant,  0, 0
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
ECOmethodEvent cPropMethodsTable[] = 
{
	cPropMethodError,               cPropMethodError,               fftNumber, 4, &cPropMethodsParamsTable[0],  0, 0,
    cPropMethodInitialize,          cPropMethodInitialize,          fftNone,   2, &cPropMethodsParamsTable[4],  0, 0,
	cPropMethodFirstParameter,      cPropMethodFirstParameter,      fftObject, 1, &cPropMethodsParamsTable[6],  0, 0,
    cPropMethodNextParameter,       cPropMethodNextParameter,       fftObject, 1, &cPropMethodsParamsTable[7],  0, 0,
    cPropMethodAddParameter,        cPropMethodAddParameter,        fftNone,   1, &cPropMethodsParamsTable[8],  0, 0,
    cPropMethodRemoveParameter,     cPropMethodRemoveParameter,     fftNone,   1, &cPropMethodsParamsTable[9],  0, 0,
    cPropMethodFirstParameterValue, cPropMethodFirstParameterValue, fftObject, 1, &cPropMethodsParamsTable[10], 0, 0,
    cPropMethodNextParameterValue,  cPropMethodNextParameterValue,  fftObject, 1, &cPropMethodsParamsTable[11], 0, 0,
};

// List of methods in Simple
qlong NVObjProperty::returnMethods(tThreadData* pThreadData)
{
	const qshort cPropMethodCount = sizeof(cPropMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cPropMethodsTable[0], cPropMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjProperty::methodCall( tThreadData* pThreadData )
{
    tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);

	switch( funcId )
	{
		case cPropMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
        case cPropMethodInitialize:
			pThreadData->mCurMethodName = "$initialize";
			result = methodInitialize(pThreadData, paramCount);
			break;
		case cPropMethodFirstParameter:
			pThreadData->mCurMethodName = "$firstParameter";
			result = methodFirstParameter(pThreadData, paramCount);
			break;
        case cPropMethodNextParameter:
			pThreadData->mCurMethodName = "$nextParameter";
			result = methodNextParameter(pThreadData, paramCount);
			break;
        case cPropMethodAddParameter:
            pThreadData->mCurMethodName = "$addParameter";
			result = methodAddParameter(pThreadData, paramCount);
            break;
        case cPropMethodRemoveParameter:
            pThreadData->mCurMethodName = "$removeParameter";
			result = methodRemoveParameter(pThreadData, paramCount);
            break;
        case cPropMethodFirstParameterValue:
			pThreadData->mCurMethodName = "$firstParameterValue";
			result = methodFirstParameterValue(pThreadData, paramCount);
			break;
        case cPropMethodNextParameterValue:
			pThreadData->mCurMethodName = "$nextParameterValue";
			result = methodNextParameterValue(pThreadData, paramCount);
			break;
	}
	
	callErrorMethod(pThreadData, result);

	return 0L;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cPropPropertyType             = 3400,
                    cPropPropertyTypeDesc         = 3401,
                    cPropPropertyCountParameters  = 3402,
                    cPropPropertyValueObj         = 3403,
                    cPropPropertyICSOutput        = 3404,
                    cPropPropertyValue            = 3405;


// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cPropPropertyTable[] = 
{
	cPropPropertyType,             cPropPropertyType,             fftConstant,  EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, 0, 0,
    cPropPropertyTypeDesc,         cPropPropertyTypeDesc,         fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cPropPropertyCountParameters,  cPropPropertyCountParameters,  fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cPropPropertyValueObj,         cPropPropertyValueObj,         fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cPropPropertyICSOutput,        cPropPropertyICSOutput,        fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cPropPropertyValue,            cPropPropertyValue,            fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0
};

// List of properties in Simple
qlong NVObjProperty::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cPropPropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cPropPropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjProperty::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cPropPropertyType:
            
        case cPropPropertyCountParameters:
        case cPropPropertyValueObj:
        case cPropPropertyICSOutput:
            return qfalse;
        case cPropPropertyValue:
            return qtrue;
        case cPropPropertyTypeDesc:
            if (icalproperty_isa(prop.get()) == ICAL_X_PROPERTY) {
                return qtrue;
            } else {
                return qfalse;
            }
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjProperty::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
    if (!prop) {
        ECOaddParam(pThreadData->mEci, &fValReturn);
        return qtrue;
    }
    
    EXTfldval testVal;
    NVObjValue *valueAssign;
    icalvalue *theVal; 
    ConstantLookup<icalproperty_kind> propTypeLookup;
    
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropPropertyType:
            getEXTFldValFromICalType<icalproperty_kind>(fValReturn, icalproperty_isa(prop.get()), kConstResourcePrefix);
			break;	
            
        case cPropPropertyTypeDesc:
            if (icalproperty_isa(prop.get()) == ICAL_X_PROPERTY) {
                getEXTFldValFromChar(fValReturn, icalproperty_get_x_name(prop.get()));
            } else {
                getEXTFldValFromChar(fValReturn, icalproperty_kind_to_string(icalproperty_isa(prop.get())));
            }
            break;
            
        case cPropPropertyCountParameters:
            getEXTFldValFromInt(fValReturn, icalproperty_count_parameters(prop.get()));
			break;
            
        case cPropPropertyValueObj:
            // Retreive the value object
            theVal = icalproperty_get_value(prop.get());
            if (theVal) {
                // Create new Omnis object and set pointer for Omnis object
                valueAssign = createNVObj<NVObjValue>(pThreadData);
                if (valueAssign) {
                    valueAssign->setVal(theVal, comp, prop);
                    getEXTFldValForObj<NVObjValue>(fValReturn, valueAssign);
                }
            }
            break;
            
        case cPropPropertyICSOutput:
            getValueForType(pThreadData,testVal,prop.get()); // WORKAROUND: ICS Output will crash if no value is present.
            if (testVal.isEmpty() == qfalse) {
                getEXTFldValFromChar(fValReturn, icalproperty_as_ical_string(prop.get()));
            }
            break;
            
        case cPropPropertyValue:
            getValueForType(pThreadData,fValReturn,prop.get());
            break;
            
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller

	return 1L;
}

// Method to set a property of the object
qlong NVObjProperty::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
    
    if (!prop) {
        return qfalse;
    }
    
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
    
    std::string stringAssign;
    
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropPropertyValue:
            setValueForType(pThreadData,fVal,prop.get());
            break;
        case cPropPropertyTypeDesc:
            if (icalproperty_isa(prop.get()) == ICAL_X_PROPERTY) {
                stringAssign = getStringFromEXTFldVal(fVal);
                icalproperty_set_x_name(prop.get(), stringAssign.c_str());
            }
            break;
	}

	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

icalproperty* NVObjProperty::getProp() {
    return prop.get();
}

void NVObjProperty::setProp(icalproperty* p, shared_ptr<icalcomponent> c) {
    comp = c;
    
    if (c) {
        prop = shared_ptr<icalproperty>(p, KeepProperty);
    } else  {
        prop = shared_ptr<icalproperty>(p, FreeProperty);
    }
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Initialize new property
tResult NVObjProperty::methodInitialize( tThreadData* pThreadData, qshort pParamCount ) {
    EXTfldval typeVal;
    icalproperty_kind typeAssign = ICAL_NO_PROPERTY;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    
    // Parameter 1: Property type constant o X property name
    if ( getParamVar(pThreadData, 1, typeVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected property type constant or X property name.";
        return ERR_BAD_PARAMS;
	}
    
    int intTest = -1;
    std::string xName;
    if (getType(typeVal).valType == fftCharacter) {
        typeAssign = ICAL_X_PROPERTY;
        xName = getStringFromEXTFldVal(typeVal);
        if ( xName.empty() || xName[0] != 'X' ) {
            pThreadData->mExtraErrorText = "First parameter, X-Name, is unrecognized.  Expected X property name; must start with 'X'.";
            return ERR_BAD_PARAMS;
        }
    } else {
        intTest = getIntFromEXTFldVal(typeVal, propTypeLookup.first(), propTypeLookup.last());
        if (intTest == -1  || !propTypeLookup.isValid(intTest)) {
            pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected property type constant or X property name.";
            return ERR_BAD_PARAMS;
        }
        
        // Convert Omnis constant into libical enum
        typeAssign = propTypeLookup.get(intTest);
    }
    
    // Create component
    icalproperty* newProp = icalproperty_new(typeAssign);
    if (!newProp) {
        pThreadData->mExtraErrorText = str(format("Unable to create new property from libical. Error: %s") % icalerror_strerror(icalerrno));
        return ERR_METHOD_FAILED;
    }
    
    // If this is an X Property and the name was passed in, then set it up now.
    if (!xName.empty()) {
        icalproperty_set_x_name(newProp, xName.c_str());
    }
    
    // Assign shared_ptr for current property
    setProp(newProp);
    
    // Parameter 2: (Optional) set the value of the Property
    EXTfldval valVal;
    if ( getParamVar(pThreadData, 2, valVal) == qtrue ) {
        setValueForType(pThreadData,valVal,prop.get());
	}
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjProperty* thisCopy = createNVObj<NVObjProperty>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Returns the first parameter in the property
tResult NVObjProperty::methodFirstParameter( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!prop) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalparameter_kind filterType = ICAL_ANY_PARAMETER;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    
    // Param 1: (Optional) Set the type of component to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, paramTypeLookup.first(), paramTypeLookup.last());
        if (filterInt > 0) {
            filterType = paramTypeLookup.get(filterInt);
        }
	}
    
    // Get first parameter and set the return value (if a value is found)
    NVObjParameter *newParam;
    icalparameter* first = icalproperty_get_first_parameter(prop.get(), filterType);
    if (first) {
        // Create new Omnis object and set pointer for Omnis object
        newParam = createNVObj<NVObjParameter>(pThreadData);
        if (newParam) {
            newParam->setParam(first, comp, prop);
            getEXTFldValForObj<NVObjParameter>(retVal, newParam);
        }
    }
    
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Returns the next parameter in the property
tResult NVObjProperty::methodNextParameter( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!prop) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalparameter_kind filterType = ICAL_ANY_PARAMETER;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    
    // Param 1: (Optional) Set the type of parameter to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, paramTypeLookup.first(), paramTypeLookup.last());
        if (filterInt > 0) {
            filterType = paramTypeLookup.get(filterInt);
        }
	}
    
    // Get next parameter and set the return value (if a value is found)
    NVObjParameter *newParam;
    icalparameter* next = icalproperty_get_next_parameter(prop.get(), filterType);
    if (next) {
        // Create new Omnis object and set pointer for Omnis object
        newParam = createNVObj<NVObjParameter>(pThreadData);
        if (newParam) {
            newParam->setParam(next, comp, prop);
            getEXTFldValForObj<NVObjParameter>(retVal, newParam);
        }
    }
    
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Adds a parameter to the property
tResult NVObjProperty::methodAddParameter( tThreadData* pThreadData, qshort pParamCount )
{
    if (!prop) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: Parameter object to add
    EXTfldval paramVal;
    if ( getParamVar(pThreadData, 1, paramVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, param, is unrecognized. Expected parameter object.";
        return ERR_BAD_PARAMS;
	}
    NVObjParameter *paramParam = getObjForEXTfldval<NVObjParameter>(pThreadData, paramVal);
    if (!paramParam) {
        pThreadData->mExtraErrorText = "First parameter, param, is unrecognized. Expected parameter object.";
        return ERR_BAD_PARAMS;
    }
    
    // Make a copy of the component for proper cleanup with shared_ptr
    icalparameter *newParam = icalparameter_new_clone(paramParam->getParam());
    
    // Add the component (Uses the set parameter since that will remove any previous parameters of the same kind)
    icalproperty_set_parameter(prop.get(), newParam);
    
    // Alter the object
    paramParam->setParam(newParam, comp, prop);
    
    // Mark the parameter as changed
    ECOsetParameterChanged( pThreadData->mEci, 1 );
    
	return METHOD_DONE_RETURN;
}

// Removes a parameter from the property
tResult NVObjProperty::methodRemoveParameter( tThreadData* pThreadData, qshort pParamCount )
{
    if (!prop) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: Parameter object to remove
    EXTfldval paramVal;
    if ( getParamVar(pThreadData, 1, paramVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, param, is unrecognized. Expected parameter object.";
        return ERR_BAD_PARAMS;
	}
    NVObjParameter *paramParam = getObjForEXTfldval<NVObjParameter>(pThreadData, paramVal);
    if (!paramParam) {
        pThreadData->mExtraErrorText = "First parameter, param, is unrecognized. Expected parameter object.";
        return ERR_BAD_PARAMS;
    }
    
    // Add the component
    icalproperty_remove_parameter_by_ref(prop.get(), paramParam->getParam());
    
    // Alter the object to be empty (This will destruct the instance paramParam NVObjParameter instance)
    paramVal.setEmpty(fftObject, dpDefault);
    
    // Mark the parameter as changed
    ECOsetParameterChanged( pThreadData->mEci, 1 );
    
	return METHOD_DONE_RETURN;
}

// Returns the first parameter value in the property
tResult NVObjProperty::methodFirstParameterValue( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!prop) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval objVal,retVal;
    icalparameter_kind filterType = ICAL_ANY_PARAMETER;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    
    // Param 1: (Optional) Set the type of component to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, paramTypeLookup.first(), paramTypeLookup.last());
        if (filterInt > 0) {
            filterType = paramTypeLookup.get(filterInt);
        }
	}
    
    // Get first parameter
    icalparameter* first = icalproperty_get_first_parameter(prop.get(), filterType);
    if (!first) {
        ECOaddParam(pThreadData->mEci, &retVal);
        return METHOD_DONE_RETURN;
    }
    
    // Return the value to the caller
    getValueForType(pThreadData,retVal,first);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Returns the next parameter value in the property
tResult NVObjProperty::methodNextParameterValue( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!prop) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval objVal, retVal;
    icalparameter_kind filterType = ICAL_ANY_PARAMETER;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    
    // Param 1: (Optional) Set the type of parameter to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, paramTypeLookup.first(), paramTypeLookup.last());
        if (filterInt > 0) {
            filterType = paramTypeLookup.get(filterInt);
        }
	}
    
    // Get next parameter
    icalparameter* next = icalproperty_get_next_parameter(prop.get(), filterType);
    if (!next) {
        ECOaddParam(pThreadData->mEci, &retVal);
        return METHOD_DONE_RETURN;
    }
    
    // Return the value to the caller
    getValueForType(pThreadData,retVal,next);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}