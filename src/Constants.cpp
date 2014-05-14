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

#include "Constants.he"

using std::map;

/********************************************************************
 *                           COMPONENT TYPE                         *
 ********************************************************************/

// Build lookup information for component type
template<>
void LibiCalConstants::getLookup<icalcomponent_kind>(map<int, icalcomponent_kind>& lookup, icalcomponent_kind& def, int& start, int& offset) {
    start = 23001;
    def = ICAL_NO_COMPONENT;
    
    lookup[1]   = ICAL_NO_COMPONENT;
    lookup[2]   = ICAL_ANY_COMPONENT;
    lookup[3]   = ICAL_XROOT_COMPONENT;
    lookup[4]   = ICAL_XATTACH_COMPONENT;
    lookup[5]   = ICAL_VEVENT_COMPONENT;
    lookup[6]   = ICAL_VTODO_COMPONENT;
    lookup[7]   = ICAL_VJOURNAL_COMPONENT;
    lookup[8]   = ICAL_VCALENDAR_COMPONENT;
    lookup[9]   = ICAL_VAGENDA_COMPONENT;
    lookup[10]  = ICAL_VFREEBUSY_COMPONENT;
    lookup[11]  = ICAL_VALARM_COMPONENT;
    lookup[12]  = ICAL_XAUDIOALARM_COMPONENT;
    lookup[13]  = ICAL_XDISPLAYALARM_COMPONENT;
    lookup[14]  = ICAL_XEMAILALARM_COMPONENT;
    lookup[15]  = ICAL_XPROCEDUREALARM_COMPONENT;
    lookup[16]  = ICAL_VTIMEZONE_COMPONENT;
    lookup[17]  = ICAL_XSTANDARD_COMPONENT;
    lookup[18]  = ICAL_XDAYLIGHT_COMPONENT;
    lookup[19]  = ICAL_X_COMPONENT;
    lookup[20]  = ICAL_VSCHEDULE_COMPONENT;
    lookup[21]  = ICAL_VQUERY_COMPONENT;
    lookup[22]  = ICAL_VREPLY_COMPONENT;
    lookup[23]  = ICAL_VCAR_COMPONENT;
    lookup[24]  = ICAL_VCOMMAND_COMPONENT;
    lookup[25]  = ICAL_XLICINVALID_COMPONENT;
    lookup[26]  = ICAL_XLICMIMEPART_COMPONENT;
}

/********************************************************************
 *                            PROPERTY TYPE                         *
 ********************************************************************/

// Build lookup information for property type
template<>
void LibiCalConstants::getLookup<icalproperty_kind>(map<int, icalproperty_kind>& lookup, icalproperty_kind& def, int& start, int& offset) {
    start = 23050;
    def = ICAL_NO_PROPERTY;
	
 	lookup[1]  = ICAL_ANY_PROPERTY;
    lookup[2]  = ICAL_ACTION_PROPERTY;
    lookup[3]  = ICAL_ALLOWCONFLICT_PROPERTY;
    lookup[4]  = ICAL_ATTACH_PROPERTY;
    lookup[5]  = ICAL_ATTENDEE_PROPERTY;
    lookup[6]  = ICAL_CALID_PROPERTY;
    lookup[7]  = ICAL_CALMASTER_PROPERTY;
    lookup[8]  = ICAL_CALSCALE_PROPERTY;
    lookup[9]  = ICAL_CAPVERSION_PROPERTY;
    lookup[10]  = ICAL_CARLEVEL_PROPERTY;
    lookup[11]  = ICAL_CARID_PROPERTY;
    lookup[12]  = ICAL_CATEGORIES_PROPERTY;
    lookup[13]  = ICAL_CLASS_PROPERTY;
    lookup[14]  = ICAL_CMD_PROPERTY;
    lookup[15]  = ICAL_COMMENT_PROPERTY;
    lookup[16]  = ICAL_COMPLETED_PROPERTY;
    lookup[17]  = ICAL_COMPONENTS_PROPERTY;
    lookup[18]  = ICAL_CONTACT_PROPERTY;
    lookup[19]  = ICAL_CREATED_PROPERTY;
    lookup[20]  = ICAL_CSID_PROPERTY;
    lookup[21]  = ICAL_DATEMAX_PROPERTY;
    lookup[22]  = ICAL_DATEMIN_PROPERTY;
    lookup[23]  = ICAL_DECREED_PROPERTY;
    lookup[24]  = ICAL_DEFAULTCHARSET_PROPERTY;
    lookup[25]  = ICAL_DEFAULTLOCALE_PROPERTY;
    lookup[26]  = ICAL_DEFAULTTZID_PROPERTY;
    lookup[27]  = ICAL_DEFAULTVCARS_PROPERTY;
    lookup[28]  = ICAL_DENY_PROPERTY;
    lookup[29]  = ICAL_DESCRIPTION_PROPERTY;
    lookup[30]  = ICAL_DTEND_PROPERTY;
    lookup[31]  = ICAL_DTSTAMP_PROPERTY;
    lookup[32]  = ICAL_DTSTART_PROPERTY;
    lookup[33]  = ICAL_DUE_PROPERTY;
    lookup[34]  = ICAL_DURATION_PROPERTY;
    lookup[35]  = ICAL_EXDATE_PROPERTY;
    lookup[36]  = ICAL_EXPAND_PROPERTY;
    lookup[37]  = ICAL_EXRULE_PROPERTY;
    lookup[38]  = ICAL_FREEBUSY_PROPERTY;
    lookup[39]  = ICAL_GEO_PROPERTY;
    lookup[40]  = ICAL_GRANT_PROPERTY;
    lookup[41]  = ICAL_ITIPVERSION_PROPERTY;
    lookup[42]  = ICAL_LASTMODIFIED_PROPERTY;
    lookup[43]  = ICAL_LOCATION_PROPERTY;
    lookup[44]  = ICAL_MAXCOMPONENTSIZE_PROPERTY;
    lookup[45]  = ICAL_MAXDATE_PROPERTY;
    lookup[46]  = ICAL_MAXRESULTS_PROPERTY;
    lookup[47]  = ICAL_MAXRESULTSSIZE_PROPERTY;
    lookup[48]  = ICAL_METHOD_PROPERTY;
    lookup[49]  = ICAL_MINDATE_PROPERTY;
    lookup[50]  = ICAL_MULTIPART_PROPERTY;
    lookup[51]  = ICAL_NAME_PROPERTY;
    lookup[52]  = ICAL_ORGANIZER_PROPERTY;
    lookup[53]  = ICAL_OWNER_PROPERTY;
    lookup[54]  = ICAL_PERCENTCOMPLETE_PROPERTY;
    lookup[55]  = ICAL_PERMISSION_PROPERTY;
    lookup[56]  = ICAL_PRIORITY_PROPERTY;
    lookup[57]  = ICAL_PRODID_PROPERTY;
    lookup[58]  = ICAL_QUERY_PROPERTY;
    lookup[59]  = ICAL_QUERYLEVEL_PROPERTY;
    lookup[60]  = ICAL_QUERYID_PROPERTY;
    lookup[61]  = ICAL_QUERYNAME_PROPERTY;
    lookup[62]  = ICAL_RDATE_PROPERTY;
    lookup[63]  = ICAL_RECURACCEPTED_PROPERTY;
    lookup[64]  = ICAL_RECUREXPAND_PROPERTY;
    lookup[65]  = ICAL_RECURLIMIT_PROPERTY;
    lookup[66]  = ICAL_RECURRENCEID_PROPERTY;
    lookup[67]  = ICAL_RELATEDTO_PROPERTY;
    lookup[68]  = ICAL_RELCALID_PROPERTY;
    lookup[69]  = ICAL_REPEAT_PROPERTY;
    lookup[70]  = ICAL_REQUESTSTATUS_PROPERTY;
    lookup[71]  = ICAL_RESOURCES_PROPERTY;
    lookup[72]  = ICAL_RESTRICTION_PROPERTY;
    lookup[73]  = ICAL_RRULE_PROPERTY;
    lookup[74]  = ICAL_SCOPE_PROPERTY;
    lookup[75]  = ICAL_SEQUENCE_PROPERTY;
    lookup[76]  = ICAL_STATUS_PROPERTY;
    lookup[77]  = ICAL_STORESEXPANDED_PROPERTY;
    lookup[78]  = ICAL_SUMMARY_PROPERTY;
    lookup[79]  = ICAL_TARGET_PROPERTY;
    lookup[80]  = ICAL_TRANSP_PROPERTY;
    lookup[81]  = ICAL_TRIGGER_PROPERTY;
    lookup[82]  = ICAL_TZID_PROPERTY;
    lookup[83]  = ICAL_TZNAME_PROPERTY;
    lookup[84]  = ICAL_TZOFFSETFROM_PROPERTY;
    lookup[85]  = ICAL_TZOFFSETTO_PROPERTY;
    lookup[86]  = ICAL_TZURL_PROPERTY;
    lookup[87]  = ICAL_UID_PROPERTY;
    lookup[88]  = ICAL_URL_PROPERTY;
    lookup[89]  = ICAL_VERSION_PROPERTY;
    lookup[90]  = ICAL_X_PROPERTY;
    lookup[91]  = ICAL_XLICCLASS_PROPERTY;
    lookup[92]  = ICAL_XLICCLUSTERCOUNT_PROPERTY;
    lookup[93]  = ICAL_XLICERROR_PROPERTY;
    lookup[94]  = ICAL_XLICMIMECHARSET_PROPERTY;
    lookup[95]  = ICAL_XLICMIMECID_PROPERTY;
    lookup[96]  = ICAL_XLICMIMECONTENTTYPE_PROPERTY;
    lookup[97]  = ICAL_XLICMIMEENCODING_PROPERTY;
    lookup[98]  = ICAL_XLICMIMEFILENAME_PROPERTY;
    lookup[99]  = ICAL_XLICMIMEOPTINFO_PROPERTY;
    lookup[100]  = ICAL_NO_PROPERTY;
}

/********************************************************************
 *                        PROPERTY TYPE                             *
 ********************************************************************/

//  Action
template<>
void LibiCalConstants::getLookup<icalproperty_action>(map<int, icalproperty_action>& lookup, icalproperty_action& def, int& start, int& offset) {
    start = 23400;
    def = ICAL_ACTION_NONE;
    
    lookup[1]  = ICAL_ACTION_X;
    lookup[2]  = ICAL_ACTION_AUDIO;
    lookup[3]  = ICAL_ACTION_DISPLAY;
    lookup[4]  = ICAL_ACTION_EMAIL;
    lookup[5]  = ICAL_ACTION_PROCEDURE;
    lookup[6]  = ICAL_ACTION_NONE;
}


//  Car Level
template<>
void LibiCalConstants::getLookup<icalproperty_carlevel>(map<int, icalproperty_carlevel>& lookup, icalproperty_carlevel& def, int& start, int& offset) {
    start = 23406;
    offset = 7;
    def = ICAL_CARLEVEL_NONE;
    
    lookup[7]  = ICAL_CARLEVEL_X;
    lookup[8]  = ICAL_CARLEVEL_CARNONE;
    lookup[9]  = ICAL_CARLEVEL_CARMIN;
    lookup[10]  = ICAL_CARLEVEL_CARFULL1;
    lookup[11]  = ICAL_CARLEVEL_NONE;
}


//  Class
template<>
void LibiCalConstants::getLookup<icalproperty_class>(map<int, icalproperty_class>& lookup, icalproperty_class& def, int& start, int& offset) {
    start = 23411;
    offset = 12;
    def = ICAL_CLASS_NONE;
    
    lookup[12]  = ICAL_CLASS_X;
    lookup[13]  = ICAL_CLASS_PUBLIC;
    lookup[14]  = ICAL_CLASS_PRIVATE;
    lookup[15]  = ICAL_CLASS_CONFIDENTIAL;
    lookup[16]  = ICAL_CLASS_NONE;
}


//  Cmd
template<>
void LibiCalConstants::getLookup<icalproperty_cmd>(map<int, icalproperty_cmd>& lookup, icalproperty_cmd& def, int& start, int& offset) {
    start = 23416;
    offset = 17;
    def = ICAL_CMD_NONE;
    
    lookup[17]  = ICAL_CMD_X;
    lookup[18]  = ICAL_CMD_ABORT;
    lookup[19]  = ICAL_CMD_CONTINUE;
    lookup[20]  = ICAL_CMD_CREATE;
    lookup[21]  = ICAL_CMD_DELETE;
    lookup[22]  = ICAL_CMD_GENERATEUID;
    lookup[23]  = ICAL_CMD_GETCAPABILITY;
    lookup[24]  = ICAL_CMD_IDENTIFY;
    lookup[25]  = ICAL_CMD_MODIFY;
    lookup[26]  = ICAL_CMD_MOVE;
    lookup[27]  = ICAL_CMD_REPLY;
    lookup[28]  = ICAL_CMD_SEARCH;
    lookup[29]  = ICAL_CMD_SETLOCALE;
    lookup[30]  = ICAL_CMD_NONE;
}


//  Method
template<>
void LibiCalConstants::getLookup<icalproperty_method>(map<int, icalproperty_method>& lookup, icalproperty_method& def, int& start, int& offset) {
    start = 23430;
    offset = 31;
    def = ICAL_METHOD_NONE;
    
    lookup[31]  = ICAL_METHOD_X;
    lookup[32]  = ICAL_METHOD_PUBLISH;
    lookup[33]  = ICAL_METHOD_REQUEST;
    lookup[34]  = ICAL_METHOD_REPLY;
    lookup[35]  = ICAL_METHOD_ADD;
    lookup[36]  = ICAL_METHOD_CANCEL;
    lookup[37]  = ICAL_METHOD_REFRESH;
    lookup[38]  = ICAL_METHOD_COUNTER;
    lookup[39]  = ICAL_METHOD_DECLINECOUNTER;
    lookup[40]  = ICAL_METHOD_CREATE;
    lookup[41]  = ICAL_METHOD_READ;
    lookup[42]  = ICAL_METHOD_RESPONSE;
    lookup[43]  = ICAL_METHOD_MOVE;
    lookup[44]  = ICAL_METHOD_MODIFY;
    lookup[45]  = ICAL_METHOD_GENERATEUID;
    lookup[46]  = ICAL_METHOD_DELETE;
    lookup[47]  = ICAL_METHOD_NONE;
}


//  Query Level
template<>
void LibiCalConstants::getLookup<icalproperty_querylevel>(map<int, icalproperty_querylevel>& lookup, icalproperty_querylevel& def, int& start, int& offset) {
    start = 23447;
    offset = 48;
    def = ICAL_QUERYLEVEL_NONE;
    
    lookup[48]  = ICAL_QUERYLEVEL_X;
    lookup[49]  = ICAL_QUERYLEVEL_CALQL1;
    lookup[50]  = ICAL_QUERYLEVEL_CALQLNONE;
    lookup[51]  = ICAL_QUERYLEVEL_NONE;
}


//  Status
template<>
void LibiCalConstants::getLookup<icalproperty_status>(map<int, icalproperty_status>& lookup, icalproperty_status& def, int& start, int& offset) {
    start = 23451;
    offset = 52;
    def = ICAL_STATUS_NONE;
    
    lookup[52]  = ICAL_STATUS_X;
    lookup[53]  = ICAL_STATUS_TENTATIVE;
    lookup[54]  = ICAL_STATUS_CONFIRMED;
    lookup[55]  = ICAL_STATUS_COMPLETED;
    lookup[56]  = ICAL_STATUS_NEEDSACTION;
    lookup[57]  = ICAL_STATUS_CANCELLED;
    lookup[58]  = ICAL_STATUS_INPROCESS;
    lookup[59]  = ICAL_STATUS_DRAFT;
    lookup[60]  = ICAL_STATUS_FINAL;
    lookup[61]  = ICAL_STATUS_NONE;
}


//  Transp
template<>
void LibiCalConstants::getLookup<icalproperty_transp>(map<int, icalproperty_transp>& lookup, icalproperty_transp& def, int& start, int& offset) {
    start = 23461;
    offset = 62;
    def = ICAL_TRANSP_NONE;
    
    lookup[62]  = ICAL_TRANSP_X;
    lookup[63]  = ICAL_TRANSP_OPAQUE;
    lookup[64]  = ICAL_TRANSP_OPAQUENOCONFLICT;
    lookup[65]  = ICAL_TRANSP_TRANSPARENT;
    lookup[66]  = ICAL_TRANSP_TRANSPARENTNOCONFLICT;
    lookup[67]  = ICAL_TRANSP_NONE;
}


//  Xlic Class
template<>
void LibiCalConstants::getLookup<icalproperty_xlicclass>(map<int, icalproperty_xlicclass>& lookup, icalproperty_xlicclass& def, int& start, int& offset) {
    start = 23467;
    offset = 67;
    def = ICAL_XLICCLASS_NONE;
    
    lookup[68]  = ICAL_XLICCLASS_X;
    lookup[69]  = ICAL_XLICCLASS_PUBLISHNEW;
    lookup[70]  = ICAL_XLICCLASS_PUBLISHUPDATE;
    lookup[71]  = ICAL_XLICCLASS_PUBLISHFREEBUSY;
    lookup[72]  = ICAL_XLICCLASS_REQUESTNEW;
    lookup[73]  = ICAL_XLICCLASS_REQUESTUPDATE;
    lookup[74]  = ICAL_XLICCLASS_REQUESTRESCHEDULE;
    lookup[75]  = ICAL_XLICCLASS_REQUESTDELEGATE;
    lookup[76]  = ICAL_XLICCLASS_REQUESTNEWORGANIZER;
    lookup[77]  = ICAL_XLICCLASS_REQUESTFORWARD;
    lookup[78]  = ICAL_XLICCLASS_REQUESTSTATUS;
    lookup[79]  = ICAL_XLICCLASS_REQUESTFREEBUSY;
    lookup[80]  = ICAL_XLICCLASS_REPLYACCEPT;
    lookup[81]  = ICAL_XLICCLASS_REPLYDECLINE;
    lookup[82]  = ICAL_XLICCLASS_REPLYDELEGATE;
    lookup[83]  = ICAL_XLICCLASS_REPLYCRASHERACCEPT;
    lookup[84]  = ICAL_XLICCLASS_REPLYCRASHERDECLINE;
    lookup[85]  = ICAL_XLICCLASS_ADDINSTANCE;
    lookup[86]  = ICAL_XLICCLASS_CANCELEVENT;
    lookup[87]  = ICAL_XLICCLASS_CANCELINSTANCE;
    lookup[88]  = ICAL_XLICCLASS_CANCELALL;
    lookup[89]  = ICAL_XLICCLASS_REFRESH;
    lookup[90]  = ICAL_XLICCLASS_COUNTER;
    lookup[91]  = ICAL_XLICCLASS_DECLINECOUNTER;
    lookup[92]  = ICAL_XLICCLASS_MALFORMED;
    lookup[93]  = ICAL_XLICCLASS_OBSOLETE;
    lookup[94]  = ICAL_XLICCLASS_MISSEQUENCED;
    lookup[95]  = ICAL_XLICCLASS_UNKNOWN;
    lookup[96]  = ICAL_XLICCLASS_NONE;
}

/********************************************************************
 *                      PARAMETER TYPE                              *
 ********************************************************************/

// Build lookup information for parameter type
template<>
void LibiCalConstants::getLookup<icalparameter_kind>(map<int, icalparameter_kind>& lookup, icalparameter_kind& def, int& start, int& offset) {
    start = 23200;
    def = ICAL_NO_PARAMETER;
	
 	lookup[1]  = ICAL_ANY_PARAMETER;
    lookup[2]  = ICAL_ACTIONPARAM_PARAMETER;
    lookup[3]  = ICAL_ALTREP_PARAMETER;
    lookup[4]  = ICAL_CHARSET_PARAMETER;
    lookup[5]  = ICAL_CN_PARAMETER;
    lookup[6]  = ICAL_CUTYPE_PARAMETER;
    lookup[7]  = ICAL_DELEGATEDFROM_PARAMETER;
    lookup[8]  = ICAL_DELEGATEDTO_PARAMETER;
    lookup[9]  = ICAL_DIR_PARAMETER;
    lookup[10]  = ICAL_ENABLE_PARAMETER;
    lookup[11]  = ICAL_ENCODING_PARAMETER;
    lookup[12]  = ICAL_FBTYPE_PARAMETER;
    lookup[13]  = ICAL_FMTTYPE_PARAMETER;
    lookup[14]  = ICAL_IANA_PARAMETER;
    lookup[15]  = ICAL_ID_PARAMETER;
    lookup[16]  = ICAL_LANGUAGE_PARAMETER;
    lookup[17]  = ICAL_LATENCY_PARAMETER;
    lookup[18]  = ICAL_LOCAL_PARAMETER;
    lookup[19]  = ICAL_LOCALIZE_PARAMETER;
    lookup[20]  = ICAL_MEMBER_PARAMETER;
    lookup[21]  = ICAL_OPTIONS_PARAMETER;
    lookup[22]  = ICAL_PARTSTAT_PARAMETER;
    lookup[23]  = ICAL_RANGE_PARAMETER;
    lookup[24]  = ICAL_RELATED_PARAMETER;
    lookup[25]  = ICAL_RELTYPE_PARAMETER;
    lookup[26]  = ICAL_ROLE_PARAMETER;
    lookup[27]  = ICAL_RSVP_PARAMETER;
    lookup[28]  = ICAL_SENTBY_PARAMETER;
    lookup[29]  = ICAL_TZID_PARAMETER;
    lookup[30]  = ICAL_VALUE_PARAMETER;
    lookup[31]  = ICAL_X_PARAMETER;
    lookup[32]  = ICAL_XLICCOMPARETYPE_PARAMETER;
    lookup[33]  = ICAL_XLICERRORTYPE_PARAMETER;
    lookup[34]  = ICAL_NO_PARAMETER;
}

/********************************************************************
 *                      PARAMETER VALUE                              *
 ********************************************************************/

//  Action
template<>
void LibiCalConstants::getLookup<icalparameter_action>(map<int, icalparameter_action>& lookup, icalparameter_action& def, int& start, int& offset) {
    start = 23300;
    def = ICAL_ACTIONPARAM_NONE;
    
    lookup[1]  = ICAL_ACTIONPARAM_X;
    lookup[2]  = ICAL_ACTIONPARAM_ASK;
    lookup[3]  = ICAL_ACTIONPARAM_ABORT;
    lookup[4]  = ICAL_ACTIONPARAM_NONE;
}

//  CU Type
template<>
void LibiCalConstants::getLookup<icalparameter_cutype>(std::map<int, icalparameter_cutype>& lookup, icalparameter_cutype& def, int& start, int& offset) {
    start = 23304;
    offset = 5;
    def = ICAL_CUTYPE_NONE;
    
    lookup[5]  = ICAL_CUTYPE_X;
    lookup[6]  = ICAL_CUTYPE_INDIVIDUAL;
    lookup[7]  = ICAL_CUTYPE_GROUP;
    lookup[8]  = ICAL_CUTYPE_RESOURCE;
    lookup[9]  = ICAL_CUTYPE_ROOM;
    lookup[10]  = ICAL_CUTYPE_UNKNOWN;
    lookup[11]  = ICAL_CUTYPE_NONE;
}

//  Enable
template<>
void LibiCalConstants::getLookup<icalparameter_enable>(std::map<int, icalparameter_enable>& lookup, icalparameter_enable& def, int& start, int& offset) {
    start = 23311;
    offset = 12;
    def = ICAL_ENABLE_NONE;
    
    lookup[12]  = ICAL_ENABLE_X;
    lookup[13]  = ICAL_ENABLE_TRUE;
    lookup[14]  = ICAL_ENABLE_FALSE;
    lookup[15]  = ICAL_ENABLE_NONE;
}

//  Encoding
template<>
void LibiCalConstants::getLookup<icalparameter_encoding>(std::map<int, icalparameter_encoding>& lookup, icalparameter_encoding& def, int& start, int& offset) {
    start = 23315;
    offset = 16;
    def = ICAL_ENCODING_NONE;
    
    lookup[16]  = ICAL_ENCODING_X;
    lookup[17]  = ICAL_ENCODING_8BIT;
    lookup[18]  = ICAL_ENCODING_BASE64;
    lookup[19]  = ICAL_ENCODING_NONE;
}

//  FB Type
template<>
void LibiCalConstants::getLookup<icalparameter_fbtype>(std::map<int, icalparameter_fbtype>& lookup, icalparameter_fbtype& def, int& start, int& offset) {
    start = 23319;
    offset = 20;
    def = ICAL_FBTYPE_NONE;
    
    lookup[20]  = ICAL_FBTYPE_X;
    lookup[21]  = ICAL_FBTYPE_FREE;
    lookup[22]  = ICAL_FBTYPE_BUSY;
    lookup[23]  = ICAL_FBTYPE_BUSYUNAVAILABLE;
    lookup[24]  = ICAL_FBTYPE_BUSYTENTATIVE;
    lookup[25]  = ICAL_FBTYPE_NONE;
}

//  Local
template<>
void LibiCalConstants::getLookup<icalparameter_local>(std::map<int, icalparameter_local>& lookup, icalparameter_local& def, int& start, int& offset) {
    start = 23325;
    offset = 26;
    def = ICAL_LOCAL_NONE;
    
    lookup[26]  = ICAL_LOCAL_X;
    lookup[27]  = ICAL_LOCAL_TRUE;
    lookup[28]  = ICAL_LOCAL_FALSE;
    lookup[29]  = ICAL_LOCAL_NONE;
}

//  PartStat
template<>
void LibiCalConstants::getLookup<icalparameter_partstat>(std::map<int, icalparameter_partstat>& lookup, icalparameter_partstat& def, int& start, int& offset) {
    start = 23329;
    offset = 30;
    def = ICAL_PARTSTAT_NONE;
    
    lookup[30]  = ICAL_PARTSTAT_X;
    lookup[31]  = ICAL_PARTSTAT_NEEDSACTION;
    lookup[32]  = ICAL_PARTSTAT_ACCEPTED;
    lookup[33]  = ICAL_PARTSTAT_DECLINED;
    lookup[34]  = ICAL_PARTSTAT_TENTATIVE;
    lookup[35]  = ICAL_PARTSTAT_DELEGATED;
    lookup[36]  = ICAL_PARTSTAT_COMPLETED;
    lookup[37]  = ICAL_PARTSTAT_INPROCESS;
    lookup[38]  = ICAL_PARTSTAT_NONE;
}

//  Range
template<>
void LibiCalConstants::getLookup<icalparameter_range>(std::map<int, icalparameter_range>& lookup, icalparameter_range& def, int& start, int& offset) {
    start = 23338;
    offset = 39;
    def = ICAL_RANGE_NONE;
    
    lookup[39]  = ICAL_RANGE_X;
    lookup[40]  = ICAL_RANGE_THISANDPRIOR;
    lookup[41]  = ICAL_RANGE_THISANDFUTURE;
    lookup[42]  = ICAL_RANGE_NONE;
}

//  Related
template<>
void LibiCalConstants::getLookup<icalparameter_related>(std::map<int, icalparameter_related>& lookup, icalparameter_related& def, int& start, int& offset) {
    start = 23342;
    offset = 43;
    def = ICAL_RELATED_NONE;
    
    lookup[43]  = ICAL_RELATED_X;
    lookup[44]  = ICAL_RELATED_START;
    lookup[45]  = ICAL_RELATED_END;
    lookup[46]  = ICAL_RELATED_NONE;
}

//  RelType
template<>
void LibiCalConstants::getLookup<icalparameter_reltype>(std::map<int, icalparameter_reltype>& lookup, icalparameter_reltype& def, int& start, int& offset) {
    start = 23346;
    offset = 47;
    def = ICAL_RELTYPE_NONE;
    
    lookup[47]  = ICAL_RELTYPE_X;
    lookup[48]  = ICAL_RELTYPE_PARENT;
    lookup[49]  = ICAL_RELTYPE_CHILD;
    lookup[50]  = ICAL_RELTYPE_SIBLING;
    lookup[51]  = ICAL_RELTYPE_NONE;
}

//  Role
template<>
void LibiCalConstants::getLookup<icalparameter_role>(std::map<int, icalparameter_role>& lookup, icalparameter_role& def, int& start, int& offset) {
    start = 23351;
    offset = 52;
    def = ICAL_ROLE_NONE;
    
    lookup[52]  = ICAL_ROLE_X;
    lookup[53]  = ICAL_ROLE_CHAIR;
    lookup[54]  = ICAL_ROLE_REQPARTICIPANT;
    lookup[55]  = ICAL_ROLE_OPTPARTICIPANT;
    lookup[56]  = ICAL_ROLE_NONPARTICIPANT;
    lookup[57]  = ICAL_ROLE_NONE;
}

//  RSVP
template<>
void LibiCalConstants::getLookup<icalparameter_rsvp>(std::map<int, icalparameter_rsvp>& lookup, icalparameter_rsvp& def, int& start, int& offset) {
    start = 23357;
    offset = 58;
    def = ICAL_RSVP_NONE;
    
    lookup[58]  = ICAL_RSVP_X;
    lookup[59]  = ICAL_RSVP_TRUE;
    lookup[60]  = ICAL_RSVP_FALSE;
    lookup[61]  = ICAL_RSVP_NONE;
}

//  Value
template<>
void LibiCalConstants::getLookup<icalparameter_value>(std::map<int, icalparameter_value>& lookup, icalparameter_value& def, int& start, int& offset) {
    start = 23361;
    offset = 62;
    def = ICAL_VALUE_NONE;
    
    lookup[62]  = ICAL_VALUE_X;
    lookup[63]  = ICAL_VALUE_BINARY;
    lookup[64]  = ICAL_VALUE_BOOLEAN;
    lookup[65]  = ICAL_VALUE_DATE;
    lookup[66]  = ICAL_VALUE_DURATION;
    lookup[67]  = ICAL_VALUE_FLOAT;
    lookup[68]  = ICAL_VALUE_INTEGER;
    lookup[69]  = ICAL_VALUE_PERIOD;
    lookup[70]  = ICAL_VALUE_RECUR;
    lookup[71]  = ICAL_VALUE_TEXT;
    lookup[72]  = ICAL_VALUE_URI;
    lookup[73]  = ICAL_VALUE_ERROR;
    lookup[74]  = ICAL_VALUE_DATETIME;
    lookup[75]  = ICAL_VALUE_UTCOFFSET;
    lookup[76]  = ICAL_VALUE_CALADDRESS;
    lookup[77]  = ICAL_VALUE_NONE;
}

//  XLIC Compare Type
template<>
void LibiCalConstants::getLookup<icalparameter_xliccomparetype>(std::map<int, icalparameter_xliccomparetype>& lookup, icalparameter_xliccomparetype& def, int& start, int& offset) {
    start = 23377;
    offset = 78;
    def = ICAL_XLICCOMPARETYPE_NONE;
    
    lookup[78]  = ICAL_XLICCOMPARETYPE_X;
    lookup[79]  = ICAL_XLICCOMPARETYPE_EQUAL;
    lookup[80]  = ICAL_XLICCOMPARETYPE_NOTEQUAL;
    lookup[81]  = ICAL_XLICCOMPARETYPE_LESS;
    lookup[82]  = ICAL_XLICCOMPARETYPE_GREATER;
    lookup[83]  = ICAL_XLICCOMPARETYPE_LESSEQUAL;
    lookup[84]  = ICAL_XLICCOMPARETYPE_GREATEREQUAL;
    lookup[85]  = ICAL_XLICCOMPARETYPE_REGEX;
    lookup[86]  = ICAL_XLICCOMPARETYPE_ISNULL;
    lookup[87]  = ICAL_XLICCOMPARETYPE_ISNOTNULL;
    lookup[88]  = ICAL_XLICCOMPARETYPE_NONE;
}

//  XLIC Error Type
template<>
void LibiCalConstants::getLookup<icalparameter_xlicerrortype>(std::map<int, icalparameter_xlicerrortype>& lookup, icalparameter_xlicerrortype& def, int& start, int& offset) {
    start = 23388;
    offset = 89;
    def = ICAL_XLICERRORTYPE_NONE;
    
    lookup[89]  = ICAL_XLICERRORTYPE_X;
    lookup[90]  = ICAL_XLICERRORTYPE_COMPONENTPARSEERROR;
    lookup[91]  = ICAL_XLICERRORTYPE_PROPERTYPARSEERROR;
    lookup[92]  = ICAL_XLICERRORTYPE_PARAMETERNAMEPARSEERROR;
    lookup[93]  = ICAL_XLICERRORTYPE_PARAMETERVALUEPARSEERROR;
    lookup[94]  = ICAL_XLICERRORTYPE_VALUEPARSEERROR;
    lookup[95]  = ICAL_XLICERRORTYPE_INVALIDITIP;
    lookup[96]  = ICAL_XLICERRORTYPE_UNKNOWNVCALPROPERROR;
    lookup[97]  = ICAL_XLICERRORTYPE_MIMEPARSEERROR;
    lookup[98]  = ICAL_XLICERRORTYPE_VCALPROPPARSEERROR;
    lookup[99]  = ICAL_XLICERRORTYPE_NONE;
}

/********************************************************************
 *                            VALUE TYPE                            *
 ********************************************************************/

// Build the lookups for value type
template<>
void LibiCalConstants::getLookup<icalvalue_kind>(map<int, icalvalue_kind>& lookup, icalvalue_kind& def, int& start, int& offset) {
    start = 23250;
    def = ICAL_NO_VALUE;
    
    lookup[1]  = ICAL_ANY_VALUE;
    lookup[2]  = ICAL_QUERY_VALUE;
    lookup[3]  = ICAL_DATE_VALUE;
    lookup[4]  = ICAL_ATTACH_VALUE;
    lookup[5]  = ICAL_GEO_VALUE;
    lookup[6]  = ICAL_STATUS_VALUE;
    lookup[7]  = ICAL_TRANSP_VALUE;
    lookup[8]  = ICAL_STRING_VALUE;
    lookup[9]  = ICAL_TEXT_VALUE;
    lookup[10]  = ICAL_REQUESTSTATUS_VALUE;
    lookup[11]  = ICAL_CMD_VALUE;
    lookup[12]  = ICAL_BINARY_VALUE;
    lookup[13]  = ICAL_QUERYLEVEL_VALUE;
    lookup[14]  = ICAL_PERIOD_VALUE;
    lookup[15]  = ICAL_FLOAT_VALUE;
    lookup[16]  = ICAL_DATETIMEPERIOD_VALUE;
    lookup[17]  = ICAL_CARLEVEL_VALUE;
    lookup[18]  = ICAL_INTEGER_VALUE;
    lookup[19]  = ICAL_CLASS_VALUE;
    lookup[20]  = ICAL_URI_VALUE;
    lookup[21]  = ICAL_DURATION_VALUE;
    lookup[22]  = ICAL_BOOLEAN_VALUE;
    lookup[23]  = ICAL_X_VALUE;
    lookup[24]  = ICAL_CALADDRESS_VALUE;
    lookup[25]  = ICAL_TRIGGER_VALUE;
    lookup[26]  = ICAL_XLICCLASS_VALUE;
    lookup[27]  = ICAL_RECUR_VALUE;
    lookup[28]  = ICAL_ACTION_VALUE;
    lookup[29]  = ICAL_DATETIME_VALUE;
    lookup[30]  = ICAL_UTCOFFSET_VALUE;
    lookup[31]  = ICAL_METHOD_VALUE;
    lookup[32]  = ICAL_NO_VALUE;
}
/********************************************************************
 *                         REQUEST STATUS                           *
 ********************************************************************/

// Build the lookups for request status
template<>
void LibiCalConstants::getLookup<icalrequeststatus>(map<int, icalrequeststatus>& lookup, icalrequeststatus& def, int& start, int& offset) {
    start = 23500;
    def = ICAL_UNKNOWN_STATUS;
    
    lookup[1] = ICAL_UNKNOWN_STATUS;
    lookup[2] = ICAL_2_0_SUCCESS_STATUS;
    lookup[3] = ICAL_2_1_FALLBACK_STATUS;
    lookup[4] = ICAL_2_2_IGPROP_STATUS;
    lookup[5] = ICAL_2_3_IGPARAM_STATUS;
    lookup[6] = ICAL_2_4_IGXPROP_STATUS;
    lookup[7] = ICAL_2_5_IGXPARAM_STATUS;
    lookup[8] = ICAL_2_6_IGCOMP_STATUS;
    lookup[9] = ICAL_2_7_FORWARD_STATUS;
    lookup[10] = ICAL_2_8_ONEEVENT_STATUS;
    lookup[11] = ICAL_2_9_TRUNC_STATUS;
    lookup[12] = ICAL_2_10_ONETODO_STATUS;
    lookup[13] = ICAL_2_11_TRUNCRRULE_STATUS;
    lookup[14] = ICAL_3_0_INVPROPNAME_STATUS;
    lookup[15] = ICAL_3_1_INVPROPVAL_STATUS;
    lookup[16] = ICAL_3_2_INVPARAM_STATUS;
    lookup[17] = ICAL_3_3_INVPARAMVAL_STATUS;
    lookup[18] = ICAL_3_4_INVCOMP_STATUS;
    lookup[19] = ICAL_3_5_INVTIME_STATUS;
    lookup[20] = ICAL_3_6_INVRULE_STATUS;
    lookup[21] = ICAL_3_7_INVCU_STATUS;
    lookup[22] = ICAL_3_8_NOAUTH_STATUS;
    lookup[23] = ICAL_3_9_BADVERSION_STATUS;
    lookup[24] = ICAL_3_10_TOOBIG_STATUS;
    lookup[25] = ICAL_3_11_MISSREQCOMP_STATUS;
    lookup[26] = ICAL_3_12_UNKCOMP_STATUS;
    lookup[27] = ICAL_3_13_BADCOMP_STATUS;
    lookup[28] = ICAL_3_14_NOCAP_STATUS;
    lookup[29] = ICAL_3_15_INVCOMMAND;
    lookup[30] = ICAL_4_0_BUSY_STATUS;
    lookup[31] = ICAL_4_1_STORE_ACCESS_DENIED;
    lookup[32] = ICAL_4_2_STORE_FAILED;
    lookup[33] = ICAL_4_3_STORE_NOT_FOUND;
    lookup[34] = ICAL_5_0_MAYBE_STATUS;
    lookup[35] = ICAL_5_1_UNAVAIL_STATUS;
    lookup[36] = ICAL_5_2_NOSERVICE_STATUS;
    lookup[37] = ICAL_5_3_NOSCHED_STATUS;
    lookup[38] = ICAL_6_1_CONTAINER_NOT_FOUND;
    lookup[39] = ICAL_9_0_UNRECOGNIZED_COMMAND;
}

/********************************************************************
 *                         RECURRENCE TYPE                          *
 ********************************************************************/

// Build the lookups for frequency
template<>
void LibiCalConstants::getLookup<icalrecurrencetype_frequency>(map<int, icalrecurrencetype_frequency>& lookup, icalrecurrencetype_frequency& def, int& start, int& offset) {
    start = 23600;
    def = ICAL_NO_RECURRENCE;
    
    lookup[1] = ICAL_SECONDLY_RECURRENCE;
    lookup[2] = ICAL_MINUTELY_RECURRENCE;
    lookup[3] = ICAL_HOURLY_RECURRENCE;
    lookup[4] = ICAL_DAILY_RECURRENCE;
    lookup[5] = ICAL_WEEKLY_RECURRENCE;
    lookup[6] = ICAL_MONTHLY_RECURRENCE;
    lookup[7] = ICAL_YEARLY_RECURRENCE;
    lookup[8] = ICAL_NO_RECURRENCE;
}

// Build the lookups for weekday
template<>
void LibiCalConstants::getLookup<icalrecurrencetype_weekday>(map<int, icalrecurrencetype_weekday>& lookup, icalrecurrencetype_weekday& def, int& start, int& offset) {
    start = 23650;
    def = ICAL_NO_WEEKDAY;
    
	lookup[1] = ICAL_NO_WEEKDAY;
    lookup[2] = ICAL_SUNDAY_WEEKDAY;
    lookup[3] = ICAL_MONDAY_WEEKDAY;
    lookup[4] = ICAL_TUESDAY_WEEKDAY;
    lookup[5] = ICAL_WEDNESDAY_WEEKDAY;
    lookup[6] = ICAL_THURSDAY_WEEKDAY;
    lookup[7] = ICAL_FRIDAY_WEEKDAY;
    lookup[8] = ICAL_SATURDAY_WEEKDAY;
}
