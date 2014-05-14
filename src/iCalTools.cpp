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

#include "iCalTools.he"
#include "OmnisTools.he"
#include "Constants.he"
#include "libical.he"

// Object type includes
#include "Recurrence.he"

// Boost includes
#include <boost/algorithm/string.hpp>

using namespace OmnisTools;
using namespace LibiCalConstants;

// Get an EXTfldval with a date based on an icaltimetype
void iCalTools::getEXTFldValFromTimeType(EXTfldval& fVal, icaltimetype tt, bool asObj, tThreadData* pThreadData) {
    if (icaltime_is_null_time(tt) || !icaltime_is_valid_time(tt)) {
        return;
    }
    
    NVObjDate* omnisDateObj;
    datestamptype convDate;
    
    if (asObj) {
        assert(pThreadData != 0);
        
        omnisDateObj = createNVObj<NVObjDate>(pThreadData);
        if (omnisDateObj) {
            omnisDateObj->setDateTime(tt);
            getEXTFldValForObj<NVObjDate>(fVal, omnisDateObj);
        }
    } else {
        convDate.mYear  = static_cast<qshort>(tt.year);
        convDate.mMonth = static_cast<char>(tt.month);
        convDate.mDay = static_cast<char>(tt.day);
        convDate.mDateOk = static_cast<char>(qtrue);
        
        if (icaltime_is_date(tt)) {
            // Is only a date -- mark time as NOT OK
            convDate.mTimeOk = static_cast<char>(qfalse);
            convDate.mSecOk = static_cast<char>(qfalse);
            convDate.mHunOk = static_cast<char>(qfalse);
            
            fVal.setDate(convDate, dpFdate1980);
        } else {
            // Is date/time update time fields
            convDate.mHour = static_cast<char>(tt.hour);
            convDate.mMin = static_cast<char>(tt.minute);
            convDate.mTimeOk = static_cast<char>(qtrue);
            
            convDate.mSec = static_cast<char>(tt.second);
            convDate.mSecOk = static_cast<char>(qtrue);
            
            convDate.mHunOk = static_cast<char>(qfalse);
            
            fVal.setDate(convDate, dpFdtimeC);
        }
    }
}

// Get an EXTfldval with a date based on an icaltimetype
icaltimetype iCalTools::getTimeTypeFromEXTFldVal(tThreadData* pThreadData, EXTfldval& fVal) {
    
    datestamptype convDate;
    FieldValType fValType = getType(fVal);
    qshort def = fValType.valSubType; // Used for subformat of date
	fVal.getDate(convDate, def);
    
    NVObjDate* omnisDateObj;
    icaltimetype tt;
    
    if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
        omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
        if (omnisDateObj) {
            tt = omnisDateObj->getDateTime();
        }
    } else {
        tt.year = static_cast<int>(convDate.mYear);
        tt.month = static_cast<int>(convDate.mMonth);
        tt.day = static_cast<int>(convDate.mDay);
        
        if (convDate.mTimeOk) {
            tt.hour = static_cast<int>(convDate.mHour);
            tt.minute = static_cast<int>(convDate.mMin);
            if (convDate.mSecOk) {
                tt.second = static_cast<int>(convDate.mSec);
            } else {
                tt.second = 0;
            }
            tt.is_date = false;
        } else {
            tt.hour = 0;
            tt.minute = 0;
            tt.second = 0;
            
            tt.is_date = true;
        }
    }
    
    return tt;
}

// Get an EXTfldval for an icalrecurrencetype
void iCalTools::getEXTFldValFromRecurrence(EXTfldval& fVal, icalrecurrencetype rt, OmnisTools::tThreadData* pThreadData) {
    
    // Create new Omnis Recurrence object and set the value
    NVObjRecurrence* recur = createNVObj<NVObjRecurrence>(pThreadData);
    recur->setRecurrence(rt);
    
    getEXTFldValForObj<NVObjRecurrence>(fVal, recur);
}

// Get an icalrecurrencetype from an EXTfldval
icalrecurrencetype iCalTools::getRecurrenceFromEXTFldVal(OmnisTools::tThreadData* pThreadData, EXTfldval& fVal) {
    
    NVObjRecurrence* recur;
    recur = getObjForEXTfldval<NVObjRecurrence>(pThreadData, fVal);
    
    return recur->getRecurrence();
}

// Looks at the current type and attempts to set the appropriate value
void iCalTools::getValueForType(tThreadData *pThreadData, EXTfldval& retVal, icalproperty* prop) {
    
    icalproperty_kind curType = icalproperty_isa(prop);
    switch (curType) {
        case ICAL_ACTION_PROPERTY:
            getEXTFldValFromICalType<icalproperty_action>(retVal, icalproperty_get_action(prop), kConstResourcePrefix);
            break;
        case ICAL_ALLOWCONFLICT_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_allowconflict(prop));
            break;
        case ICAL_ATTACH_PROPERTY:
            // TODO: Add attachment support
            break;
        case ICAL_ATTENDEE_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_attendee(prop));
            break;
        case ICAL_CALID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_calid(prop));
            break;
        case ICAL_CALMASTER_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_calmaster(prop));
            break;
        case ICAL_CALSCALE_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_calscale(prop));
            break;
        case ICAL_CAPVERSION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_capversion(prop));
            break;
        case ICAL_CARLEVEL_PROPERTY:
            getEXTFldValFromICalType<icalproperty_carlevel>(retVal, icalproperty_get_carlevel(prop), kConstResourcePrefix);
            break;
        case ICAL_CARID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_carid(prop));
            break;
        case ICAL_CATEGORIES_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_categories(prop));
            break;
        case ICAL_CLASS_PROPERTY:
            getEXTFldValFromICalType<icalproperty_class>(retVal, icalproperty_get_class(prop), kConstResourcePrefix);
            break;
        case ICAL_CMD_PROPERTY:
            getEXTFldValFromICalType<icalproperty_cmd>(retVal, icalproperty_get_cmd(prop), kConstResourcePrefix);
            break;
        case ICAL_COMMENT_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_comment(prop));
            break;
        case ICAL_COMPLETED_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_completed(prop), true, pThreadData);
            break;
        case ICAL_COMPONENTS_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_components(prop));
            break;
        case ICAL_CONTACT_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_contact(prop));
            break;
        case ICAL_CREATED_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_created(prop), true, pThreadData);
            break;
        case ICAL_CSID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_csid(prop));
            break;
        case ICAL_DATEMAX_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_datemax(prop), true, pThreadData);
            break;
        case ICAL_DATEMIN_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_datemin(prop), true, pThreadData);
            break;
        case ICAL_DECREED_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_decreed(prop));
            break;
        case ICAL_DEFAULTCHARSET_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_defaultcharset(prop));
            break;
        case ICAL_DEFAULTLOCALE_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_defaultlocale(prop));
            break;
        case ICAL_DEFAULTTZID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_defaulttzid(prop));
            break;
        case ICAL_DEFAULTVCARS_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_defaultvcars(prop));
            break;
        case ICAL_DENY_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_deny(prop));
            break;
        case ICAL_DESCRIPTION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_description(prop));
            break;
        case ICAL_DTEND_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_dtend(prop), true, pThreadData);
            break;
        case ICAL_DTSTAMP_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_dtstamp(prop), true, pThreadData);
            break;
        case ICAL_DTSTART_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_dtstart(prop), true, pThreadData);
            break;
        case ICAL_DUE_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_due(prop), true, pThreadData);
            break;
        case ICAL_DURATION_PROPERTY:
            // TODO: Add duration support
            break;
        case ICAL_EXDATE_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_exdate(prop), true, pThreadData);
            break;
        case ICAL_EXPAND_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_expand(prop));
            break;
        case ICAL_EXRULE_PROPERTY:
            getEXTFldValFromRecurrence(retVal, icalproperty_get_exrule(prop), pThreadData);
            break;
        case ICAL_FREEBUSY_PROPERTY:
            // TODO: Add period type support
            break;
        case ICAL_GEO_PROPERTY:
            // TODO: Add Geo support
            break;
        case ICAL_GRANT_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_grant(prop));
            break;
        case ICAL_ITIPVERSION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_itipversion(prop));
            break;
        case ICAL_LASTMODIFIED_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_lastmodified(prop), true, pThreadData);
            break;
        case ICAL_LOCATION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_location(prop));
            break;
        case ICAL_MAXCOMPONENTSIZE_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_maxcomponentsize(prop));
            break;
        case ICAL_MAXDATE_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_maxdate(prop), true, pThreadData);
            break;
        case ICAL_MAXRESULTS_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_maxresults(prop));
            break;
        case ICAL_MAXRESULTSSIZE_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_maxresultssize(prop));
            break;
        case ICAL_METHOD_PROPERTY:
            getEXTFldValFromICalType<icalproperty_method>(retVal, icalproperty_get_method(prop), kConstResourcePrefix);
            break;
        case ICAL_MINDATE_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_mindate(prop), true, pThreadData);
            break;
        case ICAL_MULTIPART_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_multipart(prop));
            break;
        case ICAL_NAME_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_name(prop));
            break;
        case ICAL_ORGANIZER_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_organizer(prop));
            break;
        case ICAL_OWNER_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_owner(prop));
            break;
        case ICAL_PERCENTCOMPLETE_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_percentcomplete(prop));
            break;
        case ICAL_PERMISSION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_permission(prop));
            break;
        case ICAL_PRIORITY_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_priority(prop));
            break;
        case ICAL_PRODID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_prodid(prop));
            break;
        case ICAL_QUERY_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_query(prop));
            break;
        case ICAL_QUERYLEVEL_PROPERTY:
            getEXTFldValFromICalType<icalproperty_querylevel>(retVal, icalproperty_get_querylevel(prop), kConstResourcePrefix);
            break;
        case ICAL_QUERYID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_queryid(prop));
            break;
        case ICAL_QUERYNAME_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_queryname(prop));
            break;
        case ICAL_RDATE_PROPERTY:
            // TODO: Add date/time period support
            break;
        case ICAL_RECURACCEPTED_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_recuraccepted(prop));
            break;
        case ICAL_RECUREXPAND_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_recurexpand(prop));
            break;
        case ICAL_RECURLIMIT_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_recurlimit(prop));
            break;
        case ICAL_RECURRENCEID_PROPERTY:
            getEXTFldValFromTimeType(retVal, icalproperty_get_recurrenceid(prop), true, pThreadData);
            break;
        case ICAL_RELATEDTO_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_relatedto(prop));
            break;
        case ICAL_RELCALID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_relcalid(prop));
            break;
        case ICAL_REPEAT_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_repeat(prop));
            break;
        case ICAL_REQUESTSTATUS_PROPERTY:
            getEXTFldValFromICalType<icalrequeststatus>(retVal, icalproperty_get_requeststatus(prop).code, kConstResourcePrefix);
            break;
        case ICAL_RESOURCES_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_resources(prop));
            break;
        case ICAL_RESTRICTION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_restriction(prop));
            break;
        case ICAL_RRULE_PROPERTY:
            getEXTFldValFromRecurrence(retVal, icalproperty_get_rrule(prop), pThreadData);
            break;
        case ICAL_SCOPE_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_scope(prop));
            break;
        case ICAL_SEQUENCE_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_sequence(prop));
            break;
        case ICAL_STATUS_PROPERTY:
            getEXTFldValFromICalType<icalproperty_status>(retVal, icalproperty_get_status(prop), kConstResourcePrefix);
            break;
        case ICAL_STORESEXPANDED_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_storesexpanded(prop));
            break;
        case ICAL_SUMMARY_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_summary(prop));
            break;
        case ICAL_TARGET_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_target(prop));
            break;
        case ICAL_TRANSP_PROPERTY:
            getEXTFldValFromICalType<icalproperty_transp>(retVal, icalproperty_get_transp(prop), kConstResourcePrefix);
            break;
        case ICAL_TRIGGER_PROPERTY:
            // TODO: Add trigger type support
            break;
        case ICAL_TZID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_tzid(prop));
            break;
        case ICAL_TZNAME_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_tzname(prop));
            break;
        case ICAL_TZOFFSETFROM_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_tzoffsetfrom(prop));
            break;
        case ICAL_TZOFFSETTO_PROPERTY:
            getEXTFldValFromInt(retVal, icalproperty_get_tzoffsetto(prop));
            break;
        case ICAL_TZURL_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_tzurl(prop));
            break;
        case ICAL_UID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_uid(prop));
            break;
        case ICAL_URL_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_url(prop));
            break;
        case ICAL_VERSION_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_version(prop));
            break;
        case ICAL_X_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_x(prop));
            break;
        case ICAL_XLICCLASS_PROPERTY:
            getEXTFldValFromICalType<icalproperty_xlicclass>(retVal, icalproperty_get_xlicclass(prop), kConstResourcePrefix);
            break;
        case ICAL_XLICCLUSTERCOUNT_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicclustercount(prop));
            break;
        case ICAL_XLICERROR_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicerror(prop));
            break;
        case ICAL_XLICMIMECHARSET_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicmimecharset(prop));
            break;
        case ICAL_XLICMIMECID_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicmimecid(prop));
            break;
        case ICAL_XLICMIMECONTENTTYPE_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicmimecontenttype(prop));
            break;
        case ICAL_XLICMIMEENCODING_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicmimeencoding(prop));
            break;
        case ICAL_XLICMIMEFILENAME_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicmimefilename(prop));
            break;
        case ICAL_XLICMIMEOPTINFO_PROPERTY:
            getEXTFldValFromiCalChar(retVal, icalproperty_get_xlicmimeoptinfo(prop));
            break;
            // Empty or Unknown values return nothing
        case ICAL_ANY_PROPERTY:
        case ICAL_NO_PROPERTY:
            break;
        default:
            break;
    }
}

// Looks at the current type and attempts to set the appropriate value
void iCalTools::setValueForType(tThreadData *pThreadData, EXTfldval& fVal, icalproperty* prop) {
    
    std::string stringAssign;
    icalreqstattype statTypeAssign;
    
    icaltimetype timeTypeAssign;
    NVObjDate* omnisDateObj;
    
    icalproperty_kind curType = icalproperty_isa(prop);
    switch (curType) {
        case ICAL_ACTION_PROPERTY:
            icalproperty_set_action(prop, getICalTypeFromEXTFldVal<icalproperty_action>(fVal));
            break;
        case ICAL_ALLOWCONFLICT_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_allowconflict(prop, stringAssign.c_str());
            break;
        case ICAL_ATTACH_PROPERTY:
            // TODO: Add attachment support
            break;
        case ICAL_ATTENDEE_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_attendee(prop, stringAssign.c_str());
            break;
        case ICAL_CALID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_calid(prop, stringAssign.c_str());
            break;
        case ICAL_CALMASTER_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_calmaster(prop, stringAssign.c_str());
            break;
        case ICAL_CALSCALE_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_calscale(prop, stringAssign.c_str());
            break;
        case ICAL_CAPVERSION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_capversion(prop, stringAssign.c_str());
            break;
        case ICAL_CARLEVEL_PROPERTY:
            icalproperty_set_carlevel(prop, getICalTypeFromEXTFldVal<icalproperty_carlevel>(fVal));
            break;
        case ICAL_CARID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_carid(prop, stringAssign.c_str());
            break;
        case ICAL_CATEGORIES_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_categories(prop, stringAssign.c_str());
            break;
        case ICAL_CLASS_PROPERTY:
            icalproperty_set_class(prop, getICalTypeFromEXTFldVal<icalproperty_class>(fVal));
            break;
        case ICAL_CMD_PROPERTY:
            icalproperty_set_cmd(prop, getICalTypeFromEXTFldVal<icalproperty_cmd>(fVal));
            break;
        case ICAL_COMMENT_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_comment(prop, stringAssign.c_str());
            break;
        case ICAL_COMPLETED_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_completed(prop, timeTypeAssign);
            break;
        case ICAL_COMPONENTS_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_components(prop, stringAssign.c_str());
            break;
        case ICAL_CONTACT_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_contact(prop, stringAssign.c_str());
            break;
        case ICAL_CREATED_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_created(prop, timeTypeAssign);
            break;
        case ICAL_CSID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_csid(prop, stringAssign.c_str());
            break;
        case ICAL_DATEMAX_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_datemax(prop, timeTypeAssign);
            break;
        case ICAL_DATEMIN_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_datemin(prop, timeTypeAssign);
            break;
        case ICAL_DECREED_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_decreed(prop, stringAssign.c_str());
            break;
        case ICAL_DEFAULTCHARSET_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_defaultcharset(prop, stringAssign.c_str());
            break;
        case ICAL_DEFAULTLOCALE_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_defaultlocale(prop, stringAssign.c_str());
            break;
        case ICAL_DEFAULTTZID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_defaulttzid(prop, stringAssign.c_str());
            break;
        case ICAL_DEFAULTVCARS_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_defaultvcars(prop, stringAssign.c_str());
            break;
        case ICAL_DENY_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_deny(prop, stringAssign.c_str());
            break;
        case ICAL_DESCRIPTION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_description(prop, stringAssign.c_str());
            break;
        case ICAL_DTEND_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_dtend(prop, timeTypeAssign);
            break;
        case ICAL_DTSTAMP_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_dtstamp(prop, timeTypeAssign);
            break;
        case ICAL_DTSTART_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_dtstart(prop, timeTypeAssign);
            break;
        case ICAL_DUE_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_due(prop, timeTypeAssign);
            break;
        case ICAL_DURATION_PROPERTY:
            // TODO: Add duration support
            break;
        case ICAL_EXDATE_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_exdate(prop, timeTypeAssign);
            break;
        case ICAL_EXPAND_PROPERTY:
            icalproperty_set_expand(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_EXRULE_PROPERTY:
            icalproperty_set_exrule(prop, getRecurrenceFromEXTFldVal(pThreadData, fVal));
            break;
        case ICAL_FREEBUSY_PROPERTY:
            // TODO: Add period type support
            break;
        case ICAL_GEO_PROPERTY:
            // TODO: Add Geo support
            break;
        case ICAL_GRANT_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_grant(prop, stringAssign.c_str());
            break;
        case ICAL_ITIPVERSION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_itipversion(prop, stringAssign.c_str());
            break;
        case ICAL_LASTMODIFIED_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_lastmodified(prop, timeTypeAssign);
            break;
        case ICAL_LOCATION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_location(prop, stringAssign.c_str());
            break;
        case ICAL_MAXCOMPONENTSIZE_PROPERTY:
            icalproperty_set_maxcomponentsize(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_MAXDATE_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_maxdate(prop, timeTypeAssign);
            break;
        case ICAL_MAXRESULTS_PROPERTY:
            icalproperty_set_maxresults(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_MAXRESULTSSIZE_PROPERTY:
            icalproperty_set_maxresultssize(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_METHOD_PROPERTY:
            icalproperty_set_method(prop, getICalTypeFromEXTFldVal<icalproperty_method>(fVal));
            break;
        case ICAL_MINDATE_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_mindate(prop, timeTypeAssign);
            break;
        case ICAL_MULTIPART_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_multipart(prop, stringAssign.c_str());
            break;
        case ICAL_NAME_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_name(prop, stringAssign.c_str());
            break;
        case ICAL_ORGANIZER_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_organizer(prop, stringAssign.c_str());
            break;
        case ICAL_OWNER_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_owner(prop, stringAssign.c_str());
            break;
        case ICAL_PERCENTCOMPLETE_PROPERTY:
            icalproperty_set_percentcomplete(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_PERMISSION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_permission(prop, stringAssign.c_str());
            break;
        case ICAL_PRIORITY_PROPERTY:
            icalproperty_set_priority(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_PRODID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_prodid(prop, stringAssign.c_str());
            break;
        case ICAL_QUERY_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_query(prop, stringAssign.c_str());
            break;
        case ICAL_QUERYLEVEL_PROPERTY:
            icalproperty_set_querylevel(prop, getICalTypeFromEXTFldVal<icalproperty_querylevel>(fVal));
            break;
        case ICAL_QUERYID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_queryid(prop, stringAssign.c_str());
            break;
        case ICAL_QUERYNAME_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_queryname(prop, stringAssign.c_str());
            break;
        case ICAL_RDATE_PROPERTY:
            // TODO: Add date/time period support
            break;
        case ICAL_RECURACCEPTED_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_recuraccepted(prop, stringAssign.c_str());
            break;
        case ICAL_RECUREXPAND_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_recurexpand(prop, stringAssign.c_str());
            break;
        case ICAL_RECURLIMIT_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_recurlimit(prop, stringAssign.c_str());
            break;
        case ICAL_RECURRENCEID_PROPERTY:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalproperty_set_recurrenceid(prop, timeTypeAssign);
            break;
        case ICAL_RELATEDTO_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_relatedto(prop, stringAssign.c_str());
            break;
        case ICAL_RELCALID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_relcalid(prop, stringAssign.c_str());
            break;
        case ICAL_REPEAT_PROPERTY:
            icalproperty_set_repeat(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_REQUESTSTATUS_PROPERTY:
            statTypeAssign.code = getICalTypeFromEXTFldVal<icalrequeststatus>(fVal);
            icalproperty_set_requeststatus(prop, statTypeAssign);
            break;
        case ICAL_RESOURCES_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_resources(prop, stringAssign.c_str());
            break;
        case ICAL_RESTRICTION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_restriction(prop, stringAssign.c_str());
            break;
        case ICAL_RRULE_PROPERTY:
            icalproperty_set_rrule(prop, getRecurrenceFromEXTFldVal(pThreadData, fVal));
            break;
        case ICAL_SCOPE_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_scope(prop, stringAssign.c_str());
            break;
        case ICAL_SEQUENCE_PROPERTY:
            icalproperty_set_sequence(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_STATUS_PROPERTY:
            icalproperty_set_status(prop, getICalTypeFromEXTFldVal<icalproperty_status>(fVal));
            break;
        case ICAL_STORESEXPANDED_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_storesexpanded(prop, stringAssign.c_str());
            break;
        case ICAL_SUMMARY_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_summary(prop, stringAssign.c_str());
            break;
        case ICAL_TARGET_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_target(prop, stringAssign.c_str());
            break;
        case ICAL_TRANSP_PROPERTY:
            icalproperty_set_transp(prop, getICalTypeFromEXTFldVal<icalproperty_transp>(fVal));
            break;
        case ICAL_TRIGGER_PROPERTY:
            // TODO: Add trigger type support
            break;
        case ICAL_TZID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_tzid(prop, stringAssign.c_str());
            break;
        case ICAL_TZNAME_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_tzname(prop, stringAssign.c_str());
            break;
        case ICAL_TZOFFSETFROM_PROPERTY:
            icalproperty_set_tzoffsetfrom(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_TZOFFSETTO_PROPERTY:
            icalproperty_set_tzoffsetto(prop, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_TZURL_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_tzurl(prop, stringAssign.c_str());
            break;
        case ICAL_UID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_uid(prop, stringAssign.c_str());
            break;
        case ICAL_URL_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_url(prop, stringAssign.c_str());
            break;
        case ICAL_VERSION_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_version(prop, stringAssign.c_str());
            break;
        case ICAL_X_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_x(prop, stringAssign.c_str());
            break;
        case ICAL_XLICCLASS_PROPERTY:
            icalproperty_set_xlicclass(prop, getICalTypeFromEXTFldVal<icalproperty_xlicclass>(fVal));
            break;
        case ICAL_XLICCLUSTERCOUNT_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicclustercount(prop, stringAssign.c_str());
            break;
        case ICAL_XLICERROR_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicerror(prop, stringAssign.c_str());
            break;
        case ICAL_XLICMIMECHARSET_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicmimecharset(prop, stringAssign.c_str());
            break;
        case ICAL_XLICMIMECID_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicmimecid(prop, stringAssign.c_str());
            break;
        case ICAL_XLICMIMECONTENTTYPE_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicmimecontenttype(prop, stringAssign.c_str());
            break;
        case ICAL_XLICMIMEENCODING_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicmimeencoding(prop, stringAssign.c_str());
            break;
        case ICAL_XLICMIMEFILENAME_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicmimefilename(prop, stringAssign.c_str());
            break;
        case ICAL_XLICMIMEOPTINFO_PROPERTY:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalproperty_set_xlicmimeoptinfo(prop, stringAssign.c_str());
            break;
            // Empty or Unknown values return nothing
        case ICAL_ANY_PROPERTY:
        case ICAL_NO_PROPERTY:
            break;
        default:
            break;
    }
}

// Get the Omnis EXTfldval information for a given Property
void iCalTools::getOmnisTypeForType(icalproperty_kind curType, ffttype &fft, qshort &fdp, qlong &flen) {
    
    switch (curType) {
        case ICAL_ACTION_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_ALLOWCONFLICT_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_ATTACH_PROPERTY:
            // TODO: Add attachment support
            break;
        case ICAL_ATTENDEE_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CALID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CALMASTER_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CALSCALE_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CAPVERSION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CARLEVEL_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_CARID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CATEGORIES_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CLASS_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_CMD_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_COMMENT_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_COMPLETED_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_COMPONENTS_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CONTACT_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CREATED_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_CSID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DATEMAX_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_DATEMIN_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_DECREED_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DEFAULTCHARSET_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DEFAULTLOCALE_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DEFAULTTZID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DEFAULTVCARS_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DENY_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DESCRIPTION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DTEND_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_DTSTAMP_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_DTSTART_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_DUE_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_DURATION_PROPERTY:
            // TODO: Add duration support
            break;
        case ICAL_EXDATE_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_EXPAND_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_EXRULE_PROPERTY:
            fft = fftObject;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_FREEBUSY_PROPERTY:
            // TODO: Add period type support
            break;
        case ICAL_GEO_PROPERTY:
            // TODO: Add Geo support
            break;
        case ICAL_GRANT_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_ITIPVERSION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_LASTMODIFIED_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_LOCATION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_MAXCOMPONENTSIZE_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_MAXDATE_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_MAXRESULTS_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_MAXRESULTSSIZE_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_METHOD_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_MINDATE_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_MULTIPART_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_NAME_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_ORGANIZER_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_OWNER_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_PERCENTCOMPLETE_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_PERMISSION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_PRIORITY_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_PRODID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_QUERY_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_QUERYLEVEL_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_QUERYID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_QUERYNAME_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RDATE_PROPERTY:
            // TODO: Add date/time period support
            break;
        case ICAL_RECURACCEPTED_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RECUREXPAND_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RECURLIMIT_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RECURRENCEID_PROPERTY:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_RELATEDTO_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RELCALID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_REPEAT_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_REQUESTSTATUS_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_RESOURCES_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RESTRICTION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_RRULE_PROPERTY:
            fft = fftObject;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_SCOPE_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_SEQUENCE_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_STATUS_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_STORESEXPANDED_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_SUMMARY_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TARGET_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TRANSP_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_TRIGGER_PROPERTY:
            // TODO: Add trigger type support
            break;
        case ICAL_TZID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TZNAME_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TZOFFSETFROM_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_TZOFFSETTO_PROPERTY:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_TZURL_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_UID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_URL_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_VERSION_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_X_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICCLASS_PROPERTY:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_XLICCLUSTERCOUNT_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICERROR_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICMIMECHARSET_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICMIMECID_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICMIMECONTENTTYPE_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICMIMEENCODING_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICMIMEFILENAME_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICMIMEOPTINFO_PROPERTY:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
            // Empty or Unknown values return nothing
        case ICAL_ANY_PROPERTY:
        case ICAL_NO_PROPERTY:
            break;
        default:
            break;
    }
}

// Looks at the current type and attempts to set the appropriate value
void iCalTools::getValueForType(tThreadData *pThreadData, EXTfldval& retVal, icalparameter* param) {
    
    icalparameter_kind curType = icalparameter_isa(param);
    switch (curType) {
        case ICAL_ACTIONPARAM_PARAMETER:
            getEXTFldValFromICalType<icalparameter_action>(retVal, icalparameter_get_actionparam(param), kConstResourcePrefix);
            break;
        case ICAL_ALTREP_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_altrep(param));
            break;
        case ICAL_CHARSET_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_charset(param));
            break;
        case ICAL_CN_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_cn(param));
            break;
        case ICAL_CUTYPE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_cutype>(retVal, icalparameter_get_cutype(param), kConstResourcePrefix);
            break;
        case ICAL_DELEGATEDFROM_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_delegatedfrom(param));
            break;
        case ICAL_DELEGATEDTO_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_delegatedto(param));
            break;
        case ICAL_DIR_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_dir(param));
            break;
        case ICAL_ENABLE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_enable>(retVal, icalparameter_get_enable(param), kConstResourcePrefix);
            break;
        case ICAL_ENCODING_PARAMETER:
            getEXTFldValFromICalType<icalparameter_encoding>(retVal, icalparameter_get_encoding(param), kConstResourcePrefix);
            break;
        case ICAL_FBTYPE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_fbtype>(retVal, icalparameter_get_fbtype(param), kConstResourcePrefix);
            break;
        case ICAL_FMTTYPE_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_fmttype(param));
            break;
        case ICAL_IANA_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_iana(param));
            break;
        case ICAL_ID_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_id(param));
            break;
        case ICAL_LANGUAGE_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_language(param));
            break;
        case ICAL_LATENCY_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_latency(param));
            break;
        case ICAL_LOCAL_PARAMETER:
            getEXTFldValFromICalType<icalparameter_local>(retVal, icalparameter_get_local(param), kConstResourcePrefix);
            break;
        case ICAL_LOCALIZE_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_localize(param));
            break;
        case ICAL_MEMBER_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_member(param));
            break;
        case ICAL_OPTIONS_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_options(param));
            break;
        case ICAL_PARTSTAT_PARAMETER:
            getEXTFldValFromICalType<icalparameter_partstat>(retVal, icalparameter_get_partstat(param), kConstResourcePrefix);
            break;
        case ICAL_RANGE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_range>(retVal, icalparameter_get_range(param), kConstResourcePrefix);
            break;
        case ICAL_RELATED_PARAMETER:
            getEXTFldValFromICalType<icalparameter_related>(retVal, icalparameter_get_related(param), kConstResourcePrefix);
            break;
        case ICAL_RELTYPE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_reltype>(retVal, icalparameter_get_reltype(param), kConstResourcePrefix);
            break;
        case ICAL_ROLE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_role>(retVal, icalparameter_get_role(param), kConstResourcePrefix);
            break;
        case ICAL_RSVP_PARAMETER:
            getEXTFldValFromICalType<icalparameter_rsvp>(retVal, icalparameter_get_rsvp(param), kConstResourcePrefix);
            break;
        case ICAL_SENTBY_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_sentby(param));
            break;
        case ICAL_TZID_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_tzid(param));
            break;
        case ICAL_VALUE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_value>(retVal, icalparameter_get_value(param), kConstResourcePrefix);
            break;
        case ICAL_X_PARAMETER:
            getEXTFldValFromiCalChar(retVal, icalparameter_get_x(param));
            break;
        case ICAL_XLICCOMPARETYPE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_xliccomparetype>(retVal, icalparameter_get_xliccomparetype(param), kConstResourcePrefix);
            break;
        case ICAL_XLICERRORTYPE_PARAMETER:
            getEXTFldValFromICalType<icalparameter_xlicerrortype>(retVal, icalparameter_get_xlicerrortype(param), kConstResourcePrefix);
            break;
            // Empty or Unknown values return nothing
        case ICAL_ANY_PARAMETER:
        case ICAL_NO_PARAMETER:
            break;
        default:
            break;
    }
}

// Looks at the current type and attempts to set the appropriate value
void iCalTools::setValueForType(tThreadData *pThreadData, EXTfldval& fVal, icalparameter* param) {
    
    std::string stringAssign;
    
    icalparameter_kind curType = icalparameter_isa(param);
    switch (curType) {
        case ICAL_ACTIONPARAM_PARAMETER:
            icalparameter_set_actionparam(param, getICalTypeFromEXTFldVal<icalparameter_action>(fVal));
            break;
        case ICAL_ALTREP_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_altrep(param, stringAssign.c_str());
            break;
        case ICAL_CHARSET_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_charset(param, stringAssign.c_str());
            break;
        case ICAL_CN_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_cn(param, stringAssign.c_str());
            break;
        case ICAL_CUTYPE_PARAMETER:
            icalparameter_set_cutype(param, getICalTypeFromEXTFldVal<icalparameter_cutype>(fVal));
            break;
        case ICAL_DELEGATEDFROM_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_delegatedfrom(param, stringAssign.c_str());
            break;
        case ICAL_DELEGATEDTO_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_delegatedto(param, stringAssign.c_str());
            break;
        case ICAL_DIR_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_dir(param, stringAssign.c_str());
            break;
        case ICAL_ENABLE_PARAMETER:
            icalparameter_set_enable(param, getICalTypeFromEXTFldVal<icalparameter_enable>(fVal));
            break;
        case ICAL_ENCODING_PARAMETER:
            icalparameter_set_encoding(param, getICalTypeFromEXTFldVal<icalparameter_encoding>(fVal));
            break;
        case ICAL_FBTYPE_PARAMETER:
            icalparameter_set_fbtype(param, getICalTypeFromEXTFldVal<icalparameter_fbtype>(fVal));
            break;
        case ICAL_FMTTYPE_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_fmttype(param, stringAssign.c_str());
            break;
        case ICAL_IANA_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_iana(param, stringAssign.c_str());
            break;
        case ICAL_ID_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_id(param, stringAssign.c_str());
            break;
        case ICAL_LANGUAGE_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_language(param, stringAssign.c_str());
            break;
        case ICAL_LATENCY_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_latency(param, stringAssign.c_str());
            break;
        case ICAL_LOCAL_PARAMETER:
            icalparameter_set_local(param, getICalTypeFromEXTFldVal<icalparameter_local>(fVal));
            break;
        case ICAL_LOCALIZE_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_localize(param, stringAssign.c_str());
            break;
        case ICAL_MEMBER_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_member(param, stringAssign.c_str());
            break;
        case ICAL_OPTIONS_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_options(param, stringAssign.c_str());
            break;
        case ICAL_PARTSTAT_PARAMETER:
            icalparameter_set_partstat(param, getICalTypeFromEXTFldVal<icalparameter_partstat>(fVal));
            break;
        case ICAL_RANGE_PARAMETER:
            icalparameter_set_range(param, getICalTypeFromEXTFldVal<icalparameter_range>(fVal));
            break;
        case ICAL_RELATED_PARAMETER:
            icalparameter_set_related(param, getICalTypeFromEXTFldVal<icalparameter_related>(fVal));
            break;
        case ICAL_RELTYPE_PARAMETER:
            icalparameter_set_reltype(param, getICalTypeFromEXTFldVal<icalparameter_reltype>(fVal));
            break;
        case ICAL_ROLE_PARAMETER:
            icalparameter_set_role(param, getICalTypeFromEXTFldVal<icalparameter_role>(fVal));
            break;
        case ICAL_RSVP_PARAMETER:
            icalparameter_set_rsvp(param, getICalTypeFromEXTFldVal<icalparameter_rsvp>(fVal));
            break;
        case ICAL_SENTBY_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_sentby(param, stringAssign.c_str());
            break;
        case ICAL_TZID_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_tzid(param, stringAssign.c_str());
            break;
        case ICAL_VALUE_PARAMETER:
            icalparameter_set_value(param, getICalTypeFromEXTFldVal<icalparameter_value>(fVal));
            break;
        case ICAL_X_PARAMETER:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalparameter_set_x(param, stringAssign.c_str());
            break;
        case ICAL_XLICCOMPARETYPE_PARAMETER:
            icalparameter_set_xliccomparetype(param, getICalTypeFromEXTFldVal<icalparameter_xliccomparetype>(fVal));
            break;
        case ICAL_XLICERRORTYPE_PARAMETER:
            icalparameter_set_xlicerrortype(param, getICalTypeFromEXTFldVal<icalparameter_xlicerrortype>(fVal));
            break;
            // Empty or Unknown values return nothing
        case ICAL_ANY_PARAMETER:
        case ICAL_NO_PARAMETER:
            break;
        default:
            break;
    }
}

// Get the Omnis EXTfldval information for a given Parameter
void iCalTools::getOmnisTypeForType(icalparameter_kind curType, ffttype &fft, qshort &fdp, qlong &flen) {
    
    switch (curType) {
        case ICAL_ACTIONPARAM_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_ALTREP_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CHARSET_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CN_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CUTYPE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_DELEGATEDFROM_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DELEGATEDTO_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DIR_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_ENABLE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_ENCODING_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_FBTYPE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_FMTTYPE_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_IANA_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_ID_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_LANGUAGE_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_LATENCY_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_LOCAL_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_LOCALIZE_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_MEMBER_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_OPTIONS_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_PARTSTAT_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_RANGE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_RELATED_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_RELTYPE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_ROLE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_RSVP_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_SENTBY_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TZID_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_VALUE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_X_PARAMETER:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_XLICCOMPARETYPE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_XLICERRORTYPE_PARAMETER:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
            // Empty or Unknown values return nothing
        case ICAL_ANY_PARAMETER:
        case ICAL_NO_PARAMETER:
            break;
        default:
            break;
    }
}

// Looks at the current type and returns the appropriate value
void iCalTools::getValueForType(tThreadData *pThreadData, EXTfldval& retVal, icalvalue* val) {
    icalvalue_kind curType = icalvalue_isa(val);
    
    // Based on the type of data place the appropriately converted data into an Omnis EXTfldval
    switch (curType) {
        case ICAL_QUERY_VALUE:
            getEXTFldValFromiCalChar(retVal,icalvalue_get_query(val));
            break;
        case ICAL_DATE_VALUE:
            getEXTFldValFromTimeType(retVal,icalvalue_get_date(val), true, pThreadData);
            break;
        case ICAL_ATTACH_VALUE:
            // TODO: Add attachment support
            break;
        case ICAL_GEO_VALUE:
            // TODO: Add Geocode support -- icalgeotype.lat and icalgeotype.lon
            break;
        case ICAL_STATUS_VALUE:
            getEXTFldValFromICalType<icalproperty_status>(retVal,icalvalue_get_status(val), kConstResourcePrefix);
            break;
        case ICAL_TRANSP_VALUE:
            getEXTFldValFromICalType<icalproperty_transp>(retVal,icalvalue_get_transp(val), kConstResourcePrefix);
            break;
        case ICAL_STRING_VALUE:
            getEXTFldValFromiCalChar(retVal, icalvalue_get_string(val));
            break;
        case ICAL_TEXT_VALUE:
            getEXTFldValFromiCalChar(retVal, icalvalue_get_text(val));
            break;
        case ICAL_REQUESTSTATUS_VALUE:
            getEXTFldValFromICalType<icalrequeststatus>(retVal, icalvalue_get_requeststatus(val).code, kConstResourcePrefix);
            break;
        case ICAL_CMD_VALUE:
            getEXTFldValFromICalType<icalproperty_cmd>(retVal,icalvalue_get_cmd(val), kConstResourcePrefix);
            break;
        case ICAL_BINARY_VALUE:
            getEXTfldvalFromBinary<const char>(retVal, icalvalue_get_binary(val));
            break;
        case ICAL_QUERYLEVEL_VALUE:
            getEXTFldValFromICalType<icalproperty_querylevel>(retVal, icalvalue_get_querylevel(val), kConstResourcePrefix);
            break;
        case ICAL_PERIOD_VALUE:
            // TODO: Implement Period Type -- icalperiodtype
            break;
        case ICAL_FLOAT_VALUE:
            getEXTFldValFromFloat(retVal, icalvalue_get_float(val));
            break;
        case ICAL_DATETIMEPERIOD_VALUE:
            // TODO: Implement Date/Time Period -- icaldatetimeperiodtype
            break;
        case ICAL_CARLEVEL_VALUE:
            getEXTFldValFromICalType<icalproperty_carlevel>(retVal, icalvalue_get_carlevel(val), kConstResourcePrefix);
            break;
        case ICAL_INTEGER_VALUE:
            getEXTFldValFromInt(retVal, icalvalue_get_integer(val));
            break;
        case ICAL_CLASS_VALUE:
            getEXTFldValFromICalType<icalproperty_class>(retVal, icalvalue_get_class(val), kConstResourcePrefix);
            break;
        case ICAL_URI_VALUE:
            getEXTFldValFromiCalChar(retVal, icalvalue_get_uri(val));
            break;
        case ICAL_DURATION_VALUE:
            // TODO: Implement duration -- icaldurationtype
            break;
        case ICAL_BOOLEAN_VALUE:
            getEXTFldValFromBool(retVal, icalvalue_get_boolean(val) == 1 );
            break;
        case ICAL_X_VALUE:
            getEXTFldValFromiCalChar(retVal, icalvalue_get_x(val));
            break;
        case ICAL_CALADDRESS_VALUE:
            getEXTFldValFromiCalChar(retVal, icalvalue_get_caladdress(val));
            break;
        case ICAL_TRIGGER_VALUE:
            // TODO: Implement trigger value -- icaltriggertype
            break;
        case ICAL_XLICCLASS_VALUE:
            getEXTFldValFromICalType<icalproperty_xlicclass>(retVal, icalvalue_get_xlicclass(val), kConstResourcePrefix);
            break;
        case ICAL_RECUR_VALUE:
            getEXTFldValFromRecurrence(retVal, icalvalue_get_recur(val), pThreadData);
            break;
        case ICAL_ACTION_VALUE:
            getEXTFldValFromICalType<icalproperty_action>(retVal, icalvalue_get_action(val), kConstResourcePrefix);
            break;
        case ICAL_DATETIME_VALUE:
            getEXTFldValFromTimeType(retVal,icalvalue_get_datetime(val), true, pThreadData);
            break;
        case ICAL_UTCOFFSET_VALUE:
            getEXTFldValFromInt(retVal, icalvalue_get_utcoffset(val));
            break;
        case ICAL_METHOD_VALUE:
            getEXTFldValFromICalType<icalproperty_method>(retVal, icalvalue_get_method(val), kConstResourcePrefix);
            break;
            // Empty or Unknown values return nothing
        case ICAL_NO_VALUE:
        case ICAL_ANY_VALUE:
            break;
        default:
            break;
    }
}

// Looks at the current type and attempts to set the appropriate value
void iCalTools::setValueForType(tThreadData *pThreadData, EXTfldval& fVal, icalvalue* val) {
    
    icalvalue_kind curType = icalvalue_isa(val);
    
    std::string stringAssign;
    icalreqstattype statTypeAssign;
    
    NVObjDate* omnisDateObj;
    icaltimetype timeTypeAssign;
    
    // Based on the type of data place the appropriately converted data into the libical Value
    switch (curType) {
        case ICAL_QUERY_VALUE:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalvalue_set_query(val,stringAssign.c_str());
            break;
        case ICAL_DATE_VALUE:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalvalue_set_date(val, timeTypeAssign);
            break;
        case ICAL_ATTACH_VALUE:
            // TODO: Add attachment support
            break;
        case ICAL_GEO_VALUE:
            // TODO: Add Geocode support -- icalgeotype.lat and icalgeotype.lon
            break;
        case ICAL_STATUS_VALUE:
            icalvalue_set_status(val, getICalTypeFromEXTFldVal<icalproperty_status>(fVal));
            break;
        case ICAL_TRANSP_VALUE:
            icalvalue_set_transp(val, getICalTypeFromEXTFldVal<icalproperty_transp>(fVal));
            break;
        case ICAL_STRING_VALUE:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalvalue_set_string(val, stringAssign.c_str());
            break;
        case ICAL_TEXT_VALUE:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalvalue_set_text(val, stringAssign.c_str());
            break;
        case ICAL_REQUESTSTATUS_VALUE:
            statTypeAssign.code = getICalTypeFromEXTFldVal<icalrequeststatus>(fVal);
            icalvalue_set_requeststatus(val, statTypeAssign);
            break;
        case ICAL_CMD_VALUE:
            icalvalue_set_cmd(val, getICalTypeFromEXTFldVal<icalproperty_cmd>(fVal));
            break;
        case ICAL_BINARY_VALUE:
            icalvalue_set_binary(val, getBinaryFromEXTfldval<const char>(fVal));
            break;
        case ICAL_QUERYLEVEL_VALUE:
            icalvalue_set_querylevel(val, getICalTypeFromEXTFldVal<icalproperty_querylevel>(fVal));
            break;
        case ICAL_PERIOD_VALUE:
            // TODO: Implement Period Type -- icalperiodtype
            break;
        case ICAL_FLOAT_VALUE:
            icalvalue_set_float(val, getFloatFromEXTFldVal(fVal));
            break;
        case ICAL_DATETIMEPERIOD_VALUE:
            // TODO: Implement Date/Time Period -- icaldatetimeperiodtype
            break;
        case ICAL_CARLEVEL_VALUE:
            icalvalue_set_carlevel(val, getICalTypeFromEXTFldVal<icalproperty_carlevel>(fVal));
            break;
        case ICAL_INTEGER_VALUE:
            icalvalue_set_integer(val, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_CLASS_VALUE:
            icalvalue_set_class(val, getICalTypeFromEXTFldVal<icalproperty_class>(fVal));
            break;
        case ICAL_URI_VALUE:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalvalue_set_uri(val, stringAssign.c_str());
            break;
        case ICAL_DURATION_VALUE:
            // TODO: Implement duration -- icaldurationtype
            break;
        case ICAL_BOOLEAN_VALUE:
            icalvalue_set_boolean(val, getBoolFromEXTFldVal(fVal));
            break;
        case ICAL_X_VALUE:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalvalue_set_x(val, stringAssign.c_str());
            break;
        case ICAL_CALADDRESS_VALUE:
            stringAssign = getiCalStringFromEXTFldVal(fVal);
            icalvalue_set_caladdress(val, stringAssign.c_str());
            break;
        case ICAL_TRIGGER_VALUE:
            // TODO: Implement trigger value -- icaltriggertype
            break;
        case ICAL_XLICCLASS_VALUE:
            icalvalue_set_xlicclass(val, getICalTypeFromEXTFldVal<icalproperty_xlicclass>(fVal));
            break;
        case ICAL_RECUR_VALUE:
            icalvalue_set_recur(val, getRecurrenceFromEXTFldVal(pThreadData,fVal));
            break;
        case ICAL_ACTION_VALUE:
            icalvalue_set_action(val, getICalTypeFromEXTFldVal<icalproperty_action>(fVal));
            break;
        case ICAL_DATETIME_VALUE:
            if (getType(fVal).valType == fftObject || getType(fVal).valType == fftObjref) {
                omnisDateObj = getObjForEXTfldval<NVObjDate>(pThreadData, fVal);
                if (omnisDateObj) {
                    timeTypeAssign = omnisDateObj->getDateTime();
                }
            } else {
                timeTypeAssign = getTimeTypeFromEXTFldVal(pThreadData, fVal);
            }
            
            icalvalue_set_datetime(val, timeTypeAssign);
            break;
        case ICAL_UTCOFFSET_VALUE:
            icalvalue_set_utcoffset(val, getIntFromEXTFldVal(fVal));
            break;
        case ICAL_METHOD_VALUE:
            icalvalue_set_method(val, getICalTypeFromEXTFldVal<icalproperty_method>(fVal));
            break;
            // Empty or Unknown values return nothing
        case ICAL_NO_VALUE:
        case ICAL_ANY_VALUE:
            break;
        default:
            break;
    } 
}

// Get the Omnis EXTfldval information for a given Value
void iCalTools::getOmnisTypeForType(icalvalue_kind curType, ffttype &fft, qshort &fdp, qlong &flen) {
    
    switch (curType) {
        case ICAL_QUERY_VALUE:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DATE_VALUE:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_ATTACH_VALUE:
            // TODO: Add attachment support
            break;
        case ICAL_GEO_VALUE:
            // TODO: Add Geocode support -- icalgeotype.lat and icalgeotype.lon
            break;
        case ICAL_STATUS_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_TRANSP_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_STRING_VALUE:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TEXT_VALUE:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_REQUESTSTATUS_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_CMD_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_BINARY_VALUE:
            fft = fftBinary;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_QUERYLEVEL_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_PERIOD_VALUE:
            // TODO: Implement Period Type -- icalperiodtype
            break;
        case ICAL_FLOAT_VALUE:
            fft = fftNumber;
            fdp = dpFloat;
            flen = 0;
            break;
        case ICAL_DATETIMEPERIOD_VALUE:
            // TODO: Implement Date/Time Period -- icaldatetimeperiodtype
            break;
        case ICAL_CARLEVEL_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_INTEGER_VALUE:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_CLASS_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_URI_VALUE:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_DURATION_VALUE:
            // TODO: Implement duration -- icaldurationtype
            break;
        case ICAL_BOOLEAN_VALUE:
            fft = fftBoolean;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_X_VALUE:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_CALADDRESS_VALUE:
            fft = fftCharacter;
            fdp = dpFcharacter;
            flen = 10000000;
            break;
        case ICAL_TRIGGER_VALUE:
            // TODO: Implement trigger value -- icaltriggertype
            break;
        case ICAL_XLICCLASS_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_RECUR_VALUE:
            fft = fftObject;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_ACTION_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_DATETIME_VALUE:
            fft = fftDate;
            fdp = dpFdtimeC;
            flen = 0;
            break;
        case ICAL_UTCOFFSET_VALUE:
            fft = fftInteger;
            fdp = dpDefault;
            flen = 0;
            break;
        case ICAL_METHOD_VALUE:
            fft = fftConstant;
            fdp = dpDefault;
            flen = 0;
            break;
            // Empty or Unknown values return nothing
        case ICAL_NO_VALUE:
        case ICAL_ANY_VALUE:
            break;
        default:
            break;
    }
}

// Get iCal compatible string from EXTFldVal
std::string iCalTools::getiCalStringFromEXTFldVal(EXTfldval& fVal) {
    std::string returnString = getStringFromEXTFldVal(fVal);
    
    // Convert line endings into the iCal standard \n (Line Feed)
    boost::replace_all(returnString, "\r\n", "\n");
    boost::replace_all(returnString, "\r", "\n");
    
    return returnString;
}

// Get EXTfldval from iCal compatible char*
void iCalTools::getEXTFldValFromiCalChar(EXTfldval& retVal, const char * icsChar) {
    std::string icsString;
    if (icsChar)
        icsString = icsChar;
    else
        icsString = "";
    
    // Convert line endings from the iCal standard into what Omnis expects \r (Carriage Return)
    boost::replace_all(icsString, "\r\n", "\r");
    boost::replace_all(icsString, "\n", "\r");
    
    getEXTFldValFromString(retVal, icsString);
}
