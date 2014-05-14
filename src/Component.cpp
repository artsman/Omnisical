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
#include "Component.he"
#include "Property.he"
#include "Constants.he"
#include "iCalTools.he"

#include "Date.he"

// fopen and FILE
#include <stdio.h>

// Format of error messages
#include <boost/format.hpp>

using namespace OmnisTools;
using namespace iCalTools;
using namespace LibiCalConstants;
using boost::shared_ptr;
using boost::format;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

NVObjComponent::NVObjComponent(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst) 
{ }

NVObjComponent::~NVObjComponent()
{ }

void KeepComponent (icalcomponent *comp) {
    // Do nothing since libical is managing the memory
}

void FreeComponent (icalcomponent *comp) {
    if (comp) {
        icalcomponent_free(comp);
    }
}

/**************************************************************************************************
 **                                    COPY                                                      **
 **************************************************************************************************/

void NVObjComponent::copy( NVObjComponent* pObj ) {
    NVObjBase::copy(pObj);
    
    comp = pObj->comp;
}

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cCompMethodError              = 2000,
                    cCompMethodInitialize         = 2001,
                    cCompMethodLoad               = 2002,
                    cCompMethodLoadText           = 2003,
                    cCompMethodFirstComponent     = 2004,
                    cCompMethodNextComponent      = 2005,
                    cCompMethodFirstProperty      = 2006,
                    cCompMethodNextProperty       = 2007,
                    cCompMethodCheckRestrictions  = 2008,
                    cCompMethodStripErrors        = 2009,
                    cCompMethodAddComponent       = 2010,
                    cCompMethodRemoveComponent    = 2011,
                    cCompMethodAddProperty        = 2012,
                    cCompMethodRemoveProperty     = 2013,
                    cCompMethodFirstPropertyValue = 2014,
                    cCompMethodNextPropertyValue  = 2015,
                    cCompMethodPropertyToList     = 2016,
                    cCompMethodListToProperty     = 2017;


// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cComponentMethodsParamsTable[] = 
{
	// $error
    2800, fftInteger  , 0, 0,
	2801, fftCharacter, 0, 0,
	2802, fftCharacter, 0, 0,
	2803, fftCharacter, 0, 0,
    // $initialize
	2804, fftConstant,  0, 0,
    // $load
	2805, fftCharacter, 0, 0,
    // $loadText
	2806, fftCharacter, 0, 0,
    // $firstComponent
    2807, fftConstant,  EXTD_FLAG_PARAMOPT, 0,
    // $nextComponent
    2808, fftConstant,  EXTD_FLAG_PARAMOPT, 0,
    // $firstProperty
    2809, fftConstant,  EXTD_FLAG_PARAMOPT, 0,
    // $nextProperty
    2810, fftConstant,  EXTD_FLAG_PARAMOPT, 0,
    // $addComponent
    2811, fftObject,    0, 0,
    // $removeComponent
    2812, fftObject,    0, 0,
    // $addProperty
    2813, fftObject,    0, 0,
    // $removeProperty
    2814, fftObject,    0, 0,
    // $firstPropertyValue
    2815, fftConstant,  0, 0,
    // $nextPropertyValue
    2816, fftConstant,  0, 0,
    // $propertyToList
    2817, fftList,      0, 0,
    2818, fftCharacter, 0, 0,
    2819, fftConstant,  0, 0,
    2820, fftCharacter, 0, 0,
    2821, fftConstant,  0, 0,
    // $listToProperty
    2822, fftList,      0, 0,
    2823, fftCharacter, 0, 0,
    2824, fftConstant,  0, 0,
    2825, fftCharacter, 0, 0,
    2826, fftConstant,  0, 0
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
ECOmethodEvent cComponentMethodsTable[] = 
{
	cCompMethodError,              cCompMethodError,              fftNumber,  4, &cComponentMethodsParamsTable[0],  0, 0,
	cCompMethodInitialize,         cCompMethodInitialize,         fftNone,    1, &cComponentMethodsParamsTable[4],  0, 0,
    cCompMethodLoad,               cCompMethodLoad,               fftNone,    1, &cComponentMethodsParamsTable[5],  0, 0,
    cCompMethodLoadText,           cCompMethodLoadText,           fftNone,    1, &cComponentMethodsParamsTable[6],  0, 0,
    cCompMethodFirstComponent,     cCompMethodFirstComponent,     fftObject,  1, &cComponentMethodsParamsTable[7],  0, 0,
    cCompMethodNextComponent,      cCompMethodNextComponent,      fftObject,  1, &cComponentMethodsParamsTable[8],  0, 0,
    cCompMethodFirstProperty,      cCompMethodFirstProperty,      fftObject,  1, &cComponentMethodsParamsTable[9],  0, 0,
    cCompMethodNextProperty,       cCompMethodNextProperty,       fftObject,  1, &cComponentMethodsParamsTable[10], 0, 0,
    cCompMethodCheckRestrictions,  cCompMethodCheckRestrictions,  fftInteger, 0,                                 0, 0, 0,
    cCompMethodStripErrors,        cCompMethodStripErrors,        fftNone,    0,                                 0, 0, 0,
    cCompMethodAddComponent,       cCompMethodAddComponent,       fftNone,    1, &cComponentMethodsParamsTable[11], 0, 0,
    cCompMethodRemoveComponent,    cCompMethodRemoveComponent,    fftNone,    1, &cComponentMethodsParamsTable[12], 0, 0,
    cCompMethodAddProperty,        cCompMethodAddProperty,        fftNone,    1, &cComponentMethodsParamsTable[13], 0, 0,
    cCompMethodRemoveProperty,     cCompMethodRemoveProperty,     fftNone,    1, &cComponentMethodsParamsTable[14], 0, 0,
    cCompMethodFirstPropertyValue, cCompMethodFirstPropertyValue, fftObject,  1, &cComponentMethodsParamsTable[15], 0, 0,
    cCompMethodNextPropertyValue,  cCompMethodNextPropertyValue,  fftObject,  1, &cComponentMethodsParamsTable[16], 0, 0,
    cCompMethodPropertyToList,     cCompMethodPropertyToList,     fftNone,    5, &cComponentMethodsParamsTable[17], 0, 0,
    cCompMethodListToProperty,     cCompMethodListToProperty,     fftNone,    5, &cComponentMethodsParamsTable[22], 0, 0
};

// List of methods
qlong NVObjComponent::returnMethods(tThreadData* pThreadData)
{
	const qshort cCompMethodCount = sizeof(cComponentMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cComponentMethodsTable[0], cCompMethodCount );
}

/**************************************************************************************************
 **                                  METHOD CALL                                                 **
 **************************************************************************************************/

// Call a method
qlong NVObjComponent::methodCall( tThreadData* pThreadData )
{
    tResult result = METHOD_OK;
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
    
    switch( funcId )
	{
		case cCompMethodError:
			result = METHOD_OK; // Always return ok to prevent circular call to error.
			break;
		case cCompMethodInitialize:
			pThreadData->mCurMethodName = "$initialize";
			result = methodInitialize(pThreadData, paramCount);
			break;
        case cCompMethodLoad:
			pThreadData->mCurMethodName = "$load";
			result = methodLoad(pThreadData, paramCount);
			break;
        case cCompMethodLoadText:
			pThreadData->mCurMethodName = "$loadText";
			result = methodLoadText(pThreadData, paramCount);
			break;
        case cCompMethodFirstComponent:
			pThreadData->mCurMethodName = "$firstComponent";
			result = methodFirstComponent(pThreadData, paramCount);
			break;
        case cCompMethodNextComponent:
			pThreadData->mCurMethodName = "$nextComponent";
			result = methodNextComponent(pThreadData, paramCount);
			break;
        case cCompMethodFirstProperty:
			pThreadData->mCurMethodName = "$firstProperty";
			result = methodFirstProperty(pThreadData, paramCount);
			break;
        case cCompMethodNextProperty:
			pThreadData->mCurMethodName = "$nextProperty";
			result = methodNextProperty(pThreadData, paramCount);
			break;
        case cCompMethodCheckRestrictions:
			pThreadData->mCurMethodName = "$checkRestrictions";
			result = methodCheckRestrictions(pThreadData, paramCount);
			break;
        case cCompMethodStripErrors:
			pThreadData->mCurMethodName = "$stripErrors";
			result = methodStripErrors(pThreadData, paramCount);
			break;
        case cCompMethodAddComponent:
			pThreadData->mCurMethodName = "$addComponent";
			result = methodAddComponent(pThreadData, paramCount);
			break;
        case cCompMethodRemoveComponent:
			pThreadData->mCurMethodName = "$removeComponent";
			result = methodRemoveComponent(pThreadData, paramCount);
			break;
        case cCompMethodAddProperty:
			pThreadData->mCurMethodName = "$addProperty";
			result = methodAddProperty(pThreadData, paramCount);
			break;
        case cCompMethodRemoveProperty:
			pThreadData->mCurMethodName = "$removeProperty";
			result = methodRemoveProperty(pThreadData, paramCount);
			break;
        case cCompMethodFirstPropertyValue:
			pThreadData->mCurMethodName = "$firstPropertyValue";
			result = methodFirstPropertyValue(pThreadData, paramCount);
			break;
        case cCompMethodNextPropertyValue:
			pThreadData->mCurMethodName = "$nextPropertyValue";
			result = methodNextPropertyValue(pThreadData, paramCount);
			break;
        case cCompMethodPropertyToList:
			pThreadData->mCurMethodName = "$propertyToList";
			result = methodPropertyToList(pThreadData, paramCount);
			break;
        case cCompMethodListToProperty:
			pThreadData->mCurMethodName = "$listToProperty";
			result = methodListToProperty(pThreadData, paramCount);
			break;
	}
	
	callErrorMethod(pThreadData, result);

	return 0L;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cCompPropertyType             = 2400,
                    cCompPropertyTypeDesc         = 2401,
                    cCompPropertyComponentCount   = 2402,
                    cCompPropertyPropertyCount    = 2403,
                    cCompPropertyErrorCount       = 2404,
                    cCompPropertyICSOutput        = 2405,
                    cCompPropertyCurrentComponent = 2406,
                    cCompPropertyCurrentProperty  = 2407,
                    cCompPropertyDateStart        = 2408,
                    cCompPropertyDateEnd          = 2409,
                    cCompPropertyDue              = 2410,
                    cCompPropertyDuration         = 2411,
                    cCompPropertyMethod           = 2412,
                    cCompPropertyDateStamp        = 2413,
                    cCompPropertyLocation         = 2414,
                    cCompPropertyDescription      = 2415,
                    cCompPropertyComment          = 2416,
                    cCompPropertyUID              = 2417,
                    cCompPropertyRelCalID         = 2418,
                    cCompPropertyRecurrenceID     = 2419,
                    cCompPropertySequence         = 2420,
                    cCompPropertyStatus           = 2421,
                    cCompPropertyDateStartObj     = 2422,
                    cCompPropertyDateEndObj       = 2423,
                    cCompPropertyDueObj           = 2424,
                    cCompPropertyDateStampObj     = 2425;

// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cComponentPropertyTable[] = 
{
	cCompPropertyType,             cCompPropertyType,             fftInteger,   EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, 0, 0,
    cCompPropertyTypeDesc,         cCompPropertyTypeDesc,         fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyComponentCount,   cCompPropertyComponentCount,   fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyPropertyCount,    cCompPropertyPropertyCount,    fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyErrorCount,       cCompPropertyErrorCount,       fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyICSOutput,        cCompPropertyICSOutput,        fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyCurrentComponent, cCompPropertyCurrentComponent, fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyCurrentProperty,  cCompPropertyCurrentProperty,  fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDateStart,        cCompPropertyDateStart,        fftDate,      EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDateEnd,          cCompPropertyDateEnd,          fftDate,      EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDue,              cCompPropertyDue,              fftDate,      EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDuration,         cCompPropertyDuration,         fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyMethod,           cCompPropertyMethod,           fftConstant,  EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, 0, 0,
    cCompPropertyDateStamp,        cCompPropertyDateStamp,        fftDate,      EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyLocation,         cCompPropertyLocation,         fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDescription,      cCompPropertyDescription,      fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyComment,          cCompPropertyComment,          fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyUID,              cCompPropertyUID,              fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyRelCalID,         cCompPropertyRelCalID,         fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyRecurrenceID,     cCompPropertyRecurrenceID,     fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertySequence,         cCompPropertySequence,         fftInteger,   EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyStatus,           cCompPropertyStatus,           fftConstant,  EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, 0, 0,
    cCompPropertyDateStartObj,     cCompPropertyDateStartObj,     fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDateEndObj,       cCompPropertyDateEndObj,       fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDueObj,           cCompPropertyDueObj,           fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0,
    cCompPropertyDateStampObj,     cCompPropertyDateStampObj,     fftObject,    EXTD_FLAG_PROPCUSTOM, 0, 0, 0
};

// List of properties in Simple
qlong NVObjComponent::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cComponentPropertyTable) / sizeof(ECOproperty);
    
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cComponentPropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                                  PROPERTY CALL                                               **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjComponent::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cCompPropertyType:
        case cCompPropertyTypeDesc:
        case cCompPropertyComponentCount:
        case cCompPropertyPropertyCount:
        case cCompPropertyErrorCount:
        case cCompPropertyICSOutput:
        case cCompPropertyCurrentComponent:
        case cCompPropertyCurrentProperty:
        case cCompPropertyDateStart:
        case cCompPropertyDateEnd:
        case cCompPropertyDue:
        case cCompPropertyDuration:
        case cCompPropertyMethod:
        case cCompPropertyDateStamp:
        case cCompPropertyLocation:
        case cCompPropertyDescription:
        case cCompPropertyComment:
        case cCompPropertyUID:
        case cCompPropertyRelCalID:
        case cCompPropertyRecurrenceID:
        case cCompPropertySequence:
        case cCompPropertyStatus:
        case cCompPropertyDateStartObj:
        case cCompPropertyDateEndObj:
        case cCompPropertyDueObj:
        case cCompPropertyDateStampObj:
			return qfalse;
            
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjComponent::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn, typeVal;
    
    if (!comp) {
        ECOaddParam(pThreadData->mEci, &fValReturn);
        return qtrue;
    }
    
    // Setup defaults for count properties and their associated lookups.
    icalcomponent_kind compTypeSearch = ICAL_ANY_COMPONENT;
    ConstantLookup<icalcomponent_kind> compTypeLookup;
    icalproperty_kind propTypeSearch = ICAL_ANY_PROPERTY; 
    ConstantLookup<icalproperty_kind> propTypeLookup;

    NVObjComponent *newComp = 0;
	icalcomponent *compAssign = 0;
    NVObjProperty *newProp = 0;
    icalproperty *propAssign = 0;
    
    NVObjDate* newDate = 0;
    
    qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cCompPropertyType:
            getEXTFldValFromICalType<icalcomponent_kind>(fValReturn, icalcomponent_isa(comp.get()), kConstResourcePrefix);
			break;
            
        case cCompPropertyTypeDesc:
            getEXTFldValFromChar(fValReturn, icalcomponent_kind_to_string(icalcomponent_isa(comp.get())));
			break;
            
        case cCompPropertyComponentCount:
            // Check if a parameter was passed and set the search type
            if ( getParamVar(pThreadData, 1, typeVal) == qtrue ) {
                int typeTest = getIntFromEXTFldVal(typeVal);
                if (compTypeLookup.isValid(typeTest)) {
                    compTypeSearch = compTypeLookup.get(typeTest);
                } 
            }
            getEXTFldValFromInt(fValReturn, icalcomponent_count_components(comp.get(), compTypeSearch));
            break;
            
        case cCompPropertyPropertyCount:
            // Check if a parameter was passed and set the search type
            if ( getParamVar(pThreadData, 1, typeVal) == qtrue ) {
                int typeTest = getIntFromEXTFldVal(typeVal);
                if (propTypeLookup.isValid(typeTest)) {
                    propTypeSearch = propTypeLookup.get(typeTest);
                }
            }
            getEXTFldValFromInt(fValReturn, icalcomponent_count_properties(comp.get(), propTypeSearch));
            break;
            
        case cCompPropertyErrorCount:
            getEXTFldValFromInt(fValReturn, icalcomponent_count_errors(comp.get()));
            break;
            
        case cCompPropertyICSOutput:
            getEXTFldValFromChar(fValReturn, icalcomponent_as_ical_string(comp.get()));
            break;
            
        case cCompPropertyCurrentComponent:
            compAssign = icalcomponent_get_current_component(comp.get());
            if (compAssign) {
                // Create new Omnis object and set pointer for Omnis object
                newComp = createNVObj<NVObjComponent>(pThreadData);
                if (newComp) {
                    newComp->setComponent(compAssign, comp);
                    getEXTFldValForObj<NVObjComponent>(fValReturn, newComp);
                }
            }
            break;
        case cCompPropertyCurrentProperty:
            propAssign = icalcomponent_get_current_property(comp.get());
            if (propAssign) {
                // Create new Omnis object and set pointer for Omnis object
                newProp = createNVObj<NVObjProperty>(pThreadData);
                if (newProp) {
                    newProp->setProp(propAssign, comp);
                    getEXTFldValForObj<NVObjProperty>(fValReturn, newProp);
                }
            }
            break;
        case cCompPropertyDateStart:
            getEXTFldValFromTimeType(fValReturn, icalcomponent_get_dtstart(comp.get()), true, pThreadData);
            break;
        case cCompPropertyDateEnd:
            getEXTFldValFromTimeType(fValReturn, icalcomponent_get_dtend(comp.get()), true, pThreadData);
            break;
        case cCompPropertyDue:
            getEXTFldValFromTimeType(fValReturn, icalcomponent_get_due(comp.get()), true, pThreadData);
            break;
        case cCompPropertyDuration:
            // TODO: Implement duration object
            break;
        case cCompPropertyMethod:
            getEXTFldValFromICalType<icalproperty_method>(fValReturn, icalcomponent_get_method(comp.get()));
            break;
        case cCompPropertyDateStamp:
            getEXTFldValFromTimeType(fValReturn, icalcomponent_get_dtstamp(comp.get()), true, pThreadData);
            break;
        case cCompPropertyLocation:
            getEXTFldValFromiCalChar(fValReturn, icalcomponent_get_location(comp.get()));
            break;
        case cCompPropertyDescription:
            getEXTFldValFromiCalChar(fValReturn, icalcomponent_get_description(comp.get()));
            break;
        case cCompPropertyComment:
            getEXTFldValFromiCalChar(fValReturn, icalcomponent_get_comment(comp.get()));
            break;
        case cCompPropertyUID:
            getEXTFldValFromiCalChar(fValReturn, icalcomponent_get_uid(comp.get()));
            break;
        case cCompPropertyRelCalID:
            getEXTFldValFromiCalChar(fValReturn, icalcomponent_get_relcalid(comp.get()));
            break;
        case cCompPropertyRecurrenceID:
            // Implement recurrence object
            break;
        case cCompPropertySequence:
            getEXTFldValFromInt(fValReturn, icalcomponent_get_sequence(comp.get()));
            break;
        case cCompPropertyStatus:
            getEXTFldValFromICalType<icalproperty_status>(fValReturn, icalcomponent_get_status(comp.get()));
            break;
        case cCompPropertyDateStartObj:
            newDate = createNVObj<NVObjDate>(pThreadData);
            if (newDate) {
                newDate->setDateTime(icalcomponent_get_dtstart(comp.get()));
                getEXTFldValForObj<NVObjDate>(fValReturn, newDate);
            }
            break;
        case cCompPropertyDateEndObj:
            newDate = createNVObj<NVObjDate>(pThreadData);
            if (newDate) {
                newDate->setDateTime(icalcomponent_get_dtend(comp.get()));
                getEXTFldValForObj<NVObjDate>(fValReturn, newDate);
            }
            break;
        case cCompPropertyDueObj:
            newDate = createNVObj<NVObjDate>(pThreadData);
            if (newDate) {
                newDate->setDateTime(icalcomponent_get_due(comp.get()));
                getEXTFldValForObj<NVObjDate>(fValReturn, newDate);
            }
            break;
        case cCompPropertyDateStampObj:
            newDate = createNVObj<NVObjDate>(pThreadData);
            if (newDate) {
                newDate->setDateTime(icalcomponent_get_dtstamp(comp.get()));
                getEXTFldValForObj<NVObjDate>(fValReturn, newDate);
            }
            break;
	}
    
    ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller

	return qtrue;
}

// Method to set a property of the object
qlong NVObjComponent::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;

	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cCompPropertyType:
			break;
	}

	return 1L;
}

/**************************************************************************************************
 **                                 INTERNAL METHODS                                             **
 **************************************************************************************************/

icalcomponent* NVObjComponent::getComponent() {
    return comp.get();
}

void NVObjComponent::setComponent(icalcomponent* c, shared_ptr<icalcomponent> pc) {
    parentComp = pc;
    
    if (pc) {
        comp = shared_ptr<icalcomponent>(c, KeepComponent);
    } else  {
        comp = shared_ptr<icalcomponent>(c, FreeComponent);
    }
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// This method initializes a new component to a specified type
tResult NVObjComponent::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{ 
    EXTfldval typeVal;
    icalcomponent_kind typeAssign = ICAL_NO_COMPONENT;
    ConstantLookup<icalcomponent_kind> compTypeLookup;
    
    // Read parameter
    if ( getParamVar(pThreadData, 1, typeVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected component type constant.";
        return ERR_BAD_PARAMS;
	}
    
    int intTest = getIntFromEXTFldVal(typeVal, compTypeLookup.first(), compTypeLookup.last());
    if (intTest == -1  || !compTypeLookup.isValid(intTest)) {
        pThreadData->mExtraErrorText = "First parameter, type, is unrecognized.  Expected component type constant.";
        return ERR_BAD_PARAMS;
    }
    
    // Convert Omnis constant into libical enum and create new component
    typeAssign = compTypeLookup.get(intTest);
    icalcomponent* c = icalcomponent_new(typeAssign);
    if (!c) {
        pThreadData->mExtraErrorText = str(format("Unable to create new component from libical. Error: %s") % icalerror_strerror(icalerrno));
        return ERR_METHOD_FAILED;
    }
    
    // Assign shared_ptr for current component
    setComponent(c);
    
    // Copy the object and return to the caller (this allows inline calls)
    NVObjComponent* thisCopy = createNVObj<NVObjComponent>(pThreadData);
    if(!thisCopy) {
        return ERR_METHOD_FAILED;
    }
    thisCopy->copy(this);  // Move data into new object
    
    EXTfldval retVal;  retVal.setObjInst(thisCopy->getInstance(), qtrue);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// Helper function to read a single line
char* read_stream(char *s, size_t size, void *d) {
    return fgets(s,size, reinterpret_cast<FILE*>(d));
}

// This method loads the contents of an ICS file into the current component
tResult NVObjComponent::methodLoad( tThreadData* pThreadData, qshort pParamCount )
{ 
    EXTfldval pathVal;
    
    // Parameter 1: Path of ICS file
    if ( getParamVar(pThreadData, 1, pathVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, path, is unrecognized.  Expected file path.";
        return ERR_BAD_PARAMS;
	}
    if( ensurePosixPath(pathVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, path, is unrecognized.  Expected file path.";
        return ERR_BAD_PARAMS;
    }
    std::string pathString = getStringFromEXTFldVal(pathVal);
    
    // Create a new parser object
    shared_ptr<icalparser> parser = shared_ptr<icalparser>(icalparser_new(), icalparser_free);
    
    // Open the file for reading
    shared_ptr<FILE> stream = shared_ptr<FILE>(fopen(pathString.c_str(),"r"), fclose);
    if (!stream) {
        pThreadData->mExtraErrorText = str(format("Unable to load file \"%s\"") % pathString);
        return ERR_METHOD_FAILED;
    }
    
    // Tell the parser what input routine it should use.
    icalparser_set_gen_data(parser.get(),stream.get());
    
    // Read file
    char* line;
    icalcomponent* c;
    do {
        // Get a single content line by making one or more calls to read_stream()
        line = icalparser_get_line(parser.get(),read_stream);
        
        // Assign the pointer
        c = icalparser_add_line(parser.get(),line);
    } while (line != 0 && !c);
    
    // At this point we either have a component or we hit the end of the file
    if (!c) {
        pThreadData->mExtraErrorText = str(format("Unable to load component from file \"%s\"") % pathString);
        return ERR_METHOD_FAILED;
    }
    
    // We have a good component, so assign it to the current Omnis object instance
    setComponent(c);
    
    return METHOD_DONE_RETURN;
}

// This method loads a string of ICS data into the current component
tResult NVObjComponent::methodLoadText( tThreadData* pThreadData, qshort pParamCount )
{ 
    // Read parameter
    EXTfldval icsVar;
    if ( getParamVar(pThreadData, 1, icsVar) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, ICS Contents, is unrecognized. Expected character field containing ICS data.";
        return ERR_BAD_PARAMS;
	}
    
    // Convert to UTF8 string and get new component
    std::string inString = getStringFromEXTFldVal(icsVar);
    icalcomponent* c = icalcomponent_new_from_string(inString.c_str());
    if (!c) {
        pThreadData->mExtraErrorText = str(format("Unable to create new component from libical. Error: %s") % icalerror_strerror(icalerrno));
        return ERR_METHOD_FAILED;
    }
    
    // Assign shared_ptr for current component
    setComponent(c);
    
    return METHOD_DONE_RETURN;
}


// This method returns the first child component in the component.
tResult NVObjComponent::methodFirstComponent( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalcomponent_kind filterType = ICAL_ANY_COMPONENT;
    ConstantLookup<icalcomponent_kind> compTypeLookup;
    
    // Param 1: (Optional) Set the type of component to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, compTypeLookup.first(), compTypeLookup.last());
        if (filterInt > 0) {
            filterType = compTypeLookup.get(filterInt);
        }
	}
    
    // Get first component and set the return value (if a value is found)
    NVObjComponent *newComp = 0;
    icalcomponent* first = icalcomponent_get_first_component(comp.get(), filterType);
    if (first) {
        // Create new Omnis object and set pointer for Omnis object
        newComp = createNVObj<NVObjComponent>(pThreadData);
        if (newComp) {
            newComp->setComponent(first, comp);
            getEXTFldValForObj<NVObjComponent>(retVal, newComp);
        }
    }
    
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// This method returns the next child component.
tResult NVObjComponent::methodNextComponent( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalcomponent_kind filterType = ICAL_ANY_COMPONENT;
    ConstantLookup<icalcomponent_kind> compTypeLookup;
    
    // Param 1: (Optional) Set the type of component to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, compTypeLookup.first(), compTypeLookup.last());
        if (filterInt > 0) {
            filterType = compTypeLookup.get(filterInt);
        }
	}
    
    // Get next component and set the return value (if a value is found)
    NVObjComponent *newComp = 0;
    icalcomponent* next = icalcomponent_get_next_component(comp.get(), filterType);
    if (next) {
        // Create new Omnis object and set pointer for Omnis object
        newComp = createNVObj<NVObjComponent>(pThreadData);
        if (newComp) {
            newComp->setComponent(next, comp);
            getEXTFldValForObj<NVObjComponent>(retVal, newComp);
        }
    }
    
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// This method returns the first property in the component.
tResult NVObjComponent::methodFirstProperty( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalproperty_kind filterType = ICAL_ANY_PROPERTY;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    
    // Param 1: (Optional) Set the type of property to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, propTypeLookup.first(), propTypeLookup.last());
        if (filterInt > 0) {
            filterType = propTypeLookup.get(filterInt);
        }
	}
    
    // Get first property and set the return value (if a value is found)
    NVObjProperty *newProp = 0;
    icalproperty* first = icalcomponent_get_first_property(comp.get(), filterType);
    if (first) {
        // Create new Omnis object and set pointer for Omnis object
        newProp = createNVObj<NVObjProperty>(pThreadData);
        if (newProp) {
            newProp->setProp(first, comp);
            getEXTFldValForObj<NVObjProperty>(retVal, newProp);
        }
    }
    
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// This method returns the next property in the component, after the the component passed in the first parameter.
tResult NVObjComponent::methodNextProperty( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalproperty_kind filterType = ICAL_ANY_PROPERTY;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    
    // Param 1: (Optional) Set the type of property to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, propTypeLookup.first(), propTypeLookup.last());
        if (filterInt > 0) {
            filterType = propTypeLookup.get(filterInt);
        }
	}
    
    // Get next component and set the return value (if a value is found)
    NVObjProperty *newProp = 0;
    icalproperty* next = icalcomponent_get_next_property(comp.get(), filterType);
    if (next) {
        // Create new Omnis object and set pointer for Omnis object
        newProp = createNVObj<NVObjProperty>(pThreadData);
        if (newProp) {
            newProp->setProp(next, comp);
            getEXTFldValForObj<NVObjProperty>(retVal, newProp);
        }
    }
    
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// This method checks the RFC restrictions in the object and inserts X-LIC error properties for any error.
tResult NVObjComponent::methodCheckRestrictions( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    int errors = icalcomponent_check_restrictions(comp.get());
    
    EXTfldval retVal;
    getEXTFldValFromInt(retVal, errors);
    
    ECOaddParam(pThreadData->mEci, &retVal); 

	return METHOD_DONE_RETURN;
}

// This method strips all errors in the component
tResult NVObjComponent::methodStripErrors( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    icalcomponent_strip_errors(comp.get());

	return METHOD_DONE_RETURN;
}

// This method adds a child component to the component
tResult NVObjComponent::methodAddComponent( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: Component object to add as child
    EXTfldval compVal;
    if ( getParamVar(pThreadData, 1, compVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, comp, is unrecognized. Expected component object.";
        return ERR_BAD_PARAMS;
	}
    NVObjComponent *compParam = getObjForEXTfldval<NVObjComponent>(pThreadData, compVal);
    if (!compParam) {
        pThreadData->mExtraErrorText = "First parameter, comp, is unrecognized. Expected component object.";
        return ERR_BAD_PARAMS;
    }
    
    // Make a copy of the component for proper cleanup with shared_ptr
    icalcomponent *newComp = icalcomponent_new_clone(compParam->getComponent());
    
    // Add the component
    icalcomponent_add_component(comp.get(), newComp);
    // Alter the object
    compParam->setComponent(newComp, comp);
    
    // Mark the parameter as changed
    ECOsetParameterChanged( pThreadData->mEci, 1 );
    
	return METHOD_DONE_RETURN;
}

// This method removes a child component from the component
tResult NVObjComponent::methodRemoveComponent( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: Component object to remove
    EXTfldval compVal;
    if ( getParamVar(pThreadData, 1, compVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, comp, is unrecognized. Expected component object.";
        return ERR_BAD_PARAMS;
	}
    NVObjComponent *compParam = getObjForEXTfldval<NVObjComponent>(pThreadData, compVal);
    if (!compParam) {
        pThreadData->mExtraErrorText = "First parameter, comp, is unrecognized. Expected component object.";
        return ERR_BAD_PARAMS;
    }
    
    // Remove the component
    icalcomponent_remove_component(comp.get(), compParam->getComponent());
    
    // Alter the object to be empty  (This will destruct the instance compParam NVObjComponent instance)
    compVal.setEmpty(fftObject, dpDefault);
    
    // Mark the parameter as changed
    ECOsetParameterChanged( pThreadData->mEci, 1 );
    
	return METHOD_DONE_RETURN;
}

// This method adds a property to the component
tResult NVObjComponent::methodAddProperty( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: Property object to add
    EXTfldval propVal;
    if ( getParamVar(pThreadData, 1, propVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, prop, is unrecognized. Expected property object.";
        return ERR_BAD_PARAMS;
	}
    NVObjProperty *propParam = getObjForEXTfldval<NVObjProperty>(pThreadData, propVal);
    if (!propParam) {
        pThreadData->mExtraErrorText = "First parameter, prop, is unrecognized. Expected property object.";
        return ERR_BAD_PARAMS;
    }
    
    // Make a copy of the component for proper cleanup with shared_ptr
    icalproperty *newProp = icalproperty_new_clone(propParam->getProp());
    
    // Add the component
    icalcomponent_add_property(comp.get(), newProp);
    // Alter the object
    propParam->setProp(newProp, comp);
    
    // Mark the parameter as changed
    ECOsetParameterChanged( pThreadData->mEci, 1 );
    
	return METHOD_DONE_RETURN;
}

// This method removes a property from the component
tResult NVObjComponent::methodRemoveProperty( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: Property object to remove
    EXTfldval propVal;
    if ( getParamVar(pThreadData, 1, propVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, prop, is unrecognized. Expected property object.";
        return ERR_BAD_PARAMS;
	}
    NVObjProperty *propParam = getObjForEXTfldval<NVObjProperty>(pThreadData, propVal);
    if (!propParam) {
        pThreadData->mExtraErrorText = "First parameter, prop, is unrecognized. Expected property object.";
        return ERR_BAD_PARAMS;
    }
    
    // Remove the component
    icalcomponent_remove_property(comp.get(), propParam->getProp());
    
    // Alter the object to be empty (This will destruct the instance propParam NVObjProperty instance)
    propVal.setEmpty(fftObject, dpDefault);
    
    // Mark the parameter as changed
    ECOsetParameterChanged( pThreadData->mEci, 1 );
    
	return METHOD_DONE_RETURN;
}

// This method returns the first property value in the component.
tResult NVObjComponent::methodFirstPropertyValue( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval objVal, retVal;
    icalproperty_kind filterType = ICAL_ANY_PROPERTY;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    
    // Param 1: (Optional) Set the type of property to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, propTypeLookup.first(), propTypeLookup.last());
        if (filterInt > 0) {
            filterType = propTypeLookup.get(filterInt);
        }
	}
    
    // Get first property
    icalproperty* first = icalcomponent_get_first_property(comp.get(), filterType);
    if (!first) {
        ECOaddParam(pThreadData->mEci, &retVal);
        return METHOD_DONE_RETURN;
    }
    
    // Return the value to the caller
    getValueForType(pThreadData,retVal,first);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// This method returns the next property value in the component
tResult NVObjComponent::methodNextPropertyValue( tThreadData* pThreadData, qshort pParamCount )
{ 
	if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    EXTfldval retVal;
    icalproperty_kind filterType = ICAL_ANY_PROPERTY;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    
    // Param 1: (Optional) Set the type of property to iterate through
    EXTfldval filterVal;
    int filterInt;
    if ( getParamVar(pThreadData, 1, filterVal) == qtrue ) {
        filterInt = getIntFromEXTFldVal(filterVal, propTypeLookup.first(), propTypeLookup.last());
        if (filterInt > 0) {
            filterType = propTypeLookup.get(filterInt);
        }
	}
    
    // Get next property
    icalproperty* next = icalcomponent_get_next_property(comp.get(), filterType);
    if (!next) {
        ECOaddParam(pThreadData->mEci, &retVal);
        return METHOD_DONE_RETURN;
    }
    
    // Return the value to the caller
    getValueForType(pThreadData,retVal,next);
    ECOaddParam(pThreadData->mEci, &retVal);
    
    return METHOD_DONE_RETURN;
}

// This method creates a list for the specified properties
tResult NVObjComponent::methodPropertyToList( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: List to write properties to
    EXTqlist listVal;
    if ( getParamList(pThreadData, 1, listVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, list, is unrecognized. Expected list.";
        return ERR_BAD_PARAMS;
	}
    
    EXTfldval param2Val, typeVal, colVal, paramTypeVal, paramColVal;
    qshort firstParam;
    icalproperty_kind propType = ICAL_NO_PROPERTY;
    icalparameter_kind paramType = ICAL_NO_PARAMETER;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    std::string errorText;
    
    // Parameter 2: Name of column to use for property value or constant
    if ( getParamVar(pThreadData, 2, param2Val) != qtrue ) {
        pThreadData->mExtraErrorText = "Second parameter is unrecognized. Expected list column name or property type constant.";
        return ERR_BAD_PARAMS;
	}
    
    // Parameter 3: Determined by the type of parameter 2.  Can be (List, kPropertyType, ... -or- (List, 'ColName', kPropertyType, ...
    if (getType(param2Val).valType == fftConstant) {
        // Passed a constant.  Means the property value is discared
        firstParam = 3;
        typeVal = param2Val;
        errorText = "Second";
    } else if (getType(param2Val).valType == fftCharacter) {
        // Passed a column name.  Means the property value is to be assigned to the specified column name.
        firstParam = 4;
        colVal = param2Val;
        errorText = "Third";
        
        if (colVal.isEmpty() == qtrue) {
            pThreadData->mExtraErrorText = "Second parameter is invalid.  List column name is empty.";
            return ERR_BAD_PARAMS;
        }
        
        // Read the 3rd parameter which will be the property type constant
        if ( getParamVar(pThreadData, 3, typeVal) != qtrue ) {
            pThreadData->mExtraErrorText = "Third parameter is unrecognized. Expected property type constant.";
            return ERR_BAD_PARAMS;
        }
    } else {
        pThreadData->mExtraErrorText = "Second parameter is unrecognized. Expected list column name or property type constant.";
        return ERR_BAD_PARAMS; 
    }
    
    // Verify that the constant is valid
    if (getType(typeVal).valType != fftConstant) {
        pThreadData->mExtraErrorText = str(format("%s parameter is unrecognized. Expected property type constant.") % errorText);
        return ERR_BAD_PARAMS;
    }
    propType = getICalTypeFromEXTFldVal<icalproperty_kind>(typeVal);
    if (propType == ICAL_NO_PROPERTY) {
        pThreadData->mExtraErrorText = str(format("%s parameter is unrecognized. Expected property type constant.") % errorText);
        return ERR_BAD_PARAMS; 
    }
    
    // Check the count of parameters to make sure we have an even number of name/value pairs
    if( ((pParamCount - (firstParam-1)) % 2) != 0 ) {
        pThreadData->mExtraErrorText = "Incorrect number of parameters.";
        return ERR_BAD_PARAMS; 
    }
    
    // --------------------------------
    //        MAP NAME-VALUE PAIRS
    // --------------------------------
    
    std::map<icalparameter_kind, std::string> nameMap;
    std::map<icalparameter_kind, std::string>::iterator it;
    
    // Parameter Name/Value Pairs: Loop through all parameter types and read the column names (while validating type)
    for(qshort curParam = firstParam; curParam <= pParamCount; curParam+=2) {
        // Parameter X: List column name
        if ( getParamVar(pThreadData, curParam, paramColVal) != qtrue ) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is unrecognized. Expected list column name.") % curParam);
            return ERR_BAD_PARAMS;
        }
        if (paramColVal.isEmpty() == qtrue) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is empty. Expected list column name.") % curParam);
            return ERR_BAD_PARAMS;
        }
        
        // Parameter X+1: Parameter type constant
        if ( getParamVar(pThreadData, curParam+1, paramTypeVal) != qtrue ) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is unrecognized. Expected parameter type constant") % curParam);
            return ERR_BAD_PARAMS;
        }
        paramType = getICalTypeFromEXTFldVal<icalparameter_kind>(paramTypeVal);
        if (paramType == ICAL_NO_PARAMETER) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is unrecognized. Expected parameter type constant") % curParam);
            return ERR_BAD_PARAMS; 
        }
        
        // Add values to lookup map
        nameMap[paramType] = getStringFromEXTFldVal(paramColVal);
    }
    
    // --------------------------------
    //        CREATE LIST
    // --------------------------------
    
    // Setup Omnis type variables
    ffttype fType;
    qshort fDp;
    qlong fLen;
    
    // Clear list contents
    listVal.clear(listVlen);
    
    // Setup column for property value
    if (colVal.isEmpty() == qfalse) {
        getOmnisTypeForType(propType, fType, fDp, fLen);
        listVal.addCol( fType, fDp, fLen, &colVal.getChar(qtrue) );
    }
    
    // Define parameter columns
    str255 colName;
    std::string colNameString;
    for (it = nameMap.begin(); it != nameMap.end(); ++it) {
        paramType = (*it).first;
        
        colNameString = (*it).second; 
        colName = initStr255(colNameString.c_str());
        
        getOmnisTypeForType(paramType, fType, fDp, fLen);
        listVal.addCol( fType, fDp, fLen, &colName );
    }
    
    // --------------------------------
    //        ITERATE PROPERTIES
    // --------------------------------
    
    // Variables for tracking and assignemnt
    qshort row = 0;
    qshort col = 0;
    EXTfldval assignVal;
    icalparameter* theParam;
    
    // Loop all properties 1 line = 1 property
    icalproperty* theProp = icalcomponent_get_first_property(comp.get(), propType);
    while (theProp) {
        // Insert a new row and reset the Column counter
        listVal.insertRow(); row++;
        col = 0;
        
        // Check if the property value needs to be assigned to the row
        if ((colVal.isEmpty() == qfalse)) {
            col = 1;
            listVal.getColValRef(row, col, assignVal, qtrue);
            getValueForType(pThreadData,assignVal,theProp);
        }
        
        // Add all the parameters to the row
        for (it = nameMap.begin(); it != nameMap.end(); ++it) {
            col++;
            
            paramType = (*it).first;
            theParam = icalproperty_get_first_parameter(theProp, paramType); // Only get the first parameter of the type (only way this kind of flattening works)
            if (theParam) {
                listVal.getColValRef(row, col, assignVal, qtrue);
                getValueForType(pThreadData,assignVal,theParam);
            }
        }
        
        theProp = icalcomponent_get_next_property(comp.get(), propType);
    }
    
    ECOsetParameterChanged(pThreadData->mEci, 1);  // Mark the list as changed
    
    return METHOD_DONE_RETURN;
}

// This method add properties and parameters for a given list
tResult NVObjComponent::methodListToProperty( tThreadData* pThreadData, qshort pParamCount )
{ 
    if (!comp) {
        pThreadData->mExtraErrorText = "Object not initialized";
        return ERR_METHOD_FAILED;
    }
    
    // Parameter 1: List to read properties from
    EXTqlist listVal;
    if ( getParamList(pThreadData, 1, listVal) != qtrue ) {
        pThreadData->mExtraErrorText = "First parameter, list, is unrecognized. Expected list.";
        return ERR_BAD_PARAMS;
	}
    if (listVal.rowCnt() <= 0) {
        // QUICK FAIL: If there are no rows then don't bother doing anything
        return METHOD_DONE_RETURN;
    }
    
    EXTfldval param2Val, typeVal, colVal, paramTypeVal, paramColVal;
    qshort firstParam;
    icalproperty_kind propType = ICAL_NO_PROPERTY;
    icalparameter_kind paramType = ICAL_NO_PARAMETER;
    ConstantLookup<icalproperty_kind> propTypeLookup;
    ConstantLookup<icalparameter_kind> paramTypeLookup;
    std::string errorText;
    
    // Parameter 2: Name of column to use for property value or constant
    if ( getParamVar(pThreadData, 2, param2Val) != qtrue ) {
        pThreadData->mExtraErrorText = "Second parameter is unrecognized. Expected list column name or property type constant.";
        return ERR_BAD_PARAMS;
	}
    
    // Parameter 3: Determined by the type of parameter 2.  Can be (List, kPropertyType, ... -or- (List, 'ColName', kPropertyType, ...
    if (getType(param2Val).valType == fftConstant) {
        // Passed a constant.  Means the property value is discared
        firstParam = 3;
        typeVal = param2Val;
        errorText = "Second";
    } else if (getType(param2Val).valType == fftCharacter) {
        // Passed a column name.  Means the property value is to be assigned to the specified column name.
        firstParam = 4;
        colVal = param2Val;
        errorText = "Third";
        
        if (colVal.isEmpty() == qtrue) {
            pThreadData->mExtraErrorText = "Second parameter is invalid.  List column name is empty.";
            return ERR_BAD_PARAMS;
        }
        
        // Read the 3rd parameter which will be the property type constant
        if ( getParamVar(pThreadData, 3, typeVal) != qtrue ) {
            pThreadData->mExtraErrorText = "Third parameter is unrecognized. Expected property type constant.";
            return ERR_BAD_PARAMS;
        }
    } else {
        pThreadData->mExtraErrorText = "Second parameter is unrecognized. Expected list column name or property type constant.";
        return ERR_BAD_PARAMS; 
    }
    
    // Verify that the constant is valid
    if (getType(typeVal).valType != fftConstant) {
        pThreadData->mExtraErrorText = str(format("%s parameter is unrecognized. Expected property type constant.") % errorText);
        return ERR_BAD_PARAMS;
    }
    propType = getICalTypeFromEXTFldVal<icalproperty_kind>(typeVal);
    if (propType == ICAL_NO_PROPERTY) {
        pThreadData->mExtraErrorText = str(format("%s parameter is unrecognized. Expected property type constant.") % errorText);
        return ERR_BAD_PARAMS; 
    }
    
    // Check the count of parameters to make sure we have an even number of name/value pairs
    if( ((pParamCount - (firstParam-1)) % 2) != 0 ) {
        pThreadData->mExtraErrorText = "Incorrect number of parameters.";
        return ERR_BAD_PARAMS; 
    }
    
    // --------------------------------
    //        MAP NAME-VALUE PAIRS
    // --------------------------------
    
    std::map<std::string, icalparameter_kind> nameMap;
    std::map<std::string, icalparameter_kind>::iterator it;
    
    // Parameter Name/Value Pairs: Loop through all parameter types and read the column names (while validating type)
    for(qshort curParam = firstParam; curParam <= pParamCount; curParam+=2) {
        // Parameter X: List column name
        if ( getParamVar(pThreadData, curParam, paramColVal) != qtrue ) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is unrecognized. Expected list column name.") % curParam);
            return ERR_BAD_PARAMS;
        }
        if (paramColVal.isEmpty() == qtrue) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is empty. Expected list column name.") % curParam);
            return ERR_BAD_PARAMS;
        }
        
        // Parameter X+1: Parameter type constant
        if ( getParamVar(pThreadData, curParam+1, paramTypeVal) != qtrue ) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is unrecognized. Expected parameter type constant") % curParam);
            return ERR_BAD_PARAMS;
        }
        paramType = getICalTypeFromEXTFldVal<icalparameter_kind>(paramTypeVal);
        if (paramType == ICAL_NO_PARAMETER) {
            pThreadData->mExtraErrorText = str(format("Parameter %i is unrecognized. Expected parameter type constant") % curParam);
            return ERR_BAD_PARAMS; 
        }
        
        // Add values to lookup map
        nameMap[getStringFromEXTFldVal(paramColVal)] = paramType;
    }
    
    // --------------------------------
    //        ITERATE LIST
    // --------------------------------
    
    // Setup tracking variables
    EXTfldval assignVal, colNameVal;
    str255 colName;
    std::string colNameString, colCompare;
    icalproperty* theProp;
    icalparameter* theParam;
    
    // Loop the list and fetch all values
    for( qshort row = 1; row <= listVal.rowCnt(); ++row) {
        
        // Add the new property.  1 Line = 1 Property
        theProp = icalproperty_new(propType);
        
        // Loop all columns and add the values as need to the property
        for( qshort col = 1; col <= listVal.colCnt(); ++col) {
            // Get column name
            listVal.getCol(col, qfalse, colName);
            colNameVal.setChar(colName);
            colNameString = getStringFromEXTFldVal(colNameVal);

            it = nameMap.find(colNameString);
            if (it != nameMap.end()) {
                // Found the column -- Add a new parameter and append it to the property
                listVal.getColValRef(row, col, assignVal, qtrue);
                
                theParam = icalparameter_new((*it).second);
                setValueForType(pThreadData,assignVal,theParam);
                icalproperty_add_parameter(theProp, theParam);
            } else if (colVal.compare(colName, qtrue) ==  0) {
                    listVal.getColValRef(row, col, assignVal, qtrue);
                    setValueForType(pThreadData,assignVal,theProp);
            }
        }
    }
    
    icalcomponent_add_property(comp.get(), theProp);
    
    return METHOD_DONE_RETURN;
}


