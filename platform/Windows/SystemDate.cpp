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

#include "SystemDate.h"
#include "OmnisTools.he"

#include <windows.h>
#include <map>

// Build mapping from Windows Timezone to Olson Timezone (Used in libical)
std::map<std::wstring, std::string> olsonMap;
void buildOlsonMap() {
	if( olsonMap.size() > 0 )
		return;

	olsonMap[L"Afghanistan"] = "Asia/Kabul";
	olsonMap[L"Afghanistan Standard Time"] = "Asia/Kabul";
	olsonMap[L"Alaskan"] = "America/Anchorage";
	olsonMap[L"Alaskan Standard Time"] = "America/Anchorage";
	olsonMap[L"Arab"] = "Asia/Riyadh";
	olsonMap[L"Arab Standard Time"] = "Asia/Riyadh";
	olsonMap[L"Arabian"] = "Asia/Muscat";
	olsonMap[L"Arabian Standard Time"] = "Asia/Muscat";
	olsonMap[L"Arabic Standard Time"] = "Asia/Baghdad";
	olsonMap[L"Argentina Standard Time"] = "America/Argentina/Buenos_Aires";
	olsonMap[L"Armenian Standard Time"] = "Asia/Yerevan";
	olsonMap[L"Atlantic"] = "America/Halifax";
	olsonMap[L"Atlantic Standard Time"] = "America/Halifax";
	olsonMap[L"AUS Central"] = "Australia/Darwin";
	olsonMap[L"AUS Central Standard Time"] = "Australia/Darwin";
	olsonMap[L"AUS Eastern"] = "Australia/Sydney";
	olsonMap[L"AUS Eastern Standard Time"] = "Australia/Sydney";
	olsonMap[L"Azerbaijan Standard Time"] = "Asia/Baku";
	olsonMap[L"Azores"] = "Atlantic/Azores";
	olsonMap[L"Azores Standard Time"] = "Atlantic/Azores";
	olsonMap[L"Bangkok"] = "Asia/Bangkok";
	olsonMap[L"Bangkok Standard Time"] = "Asia/Bangkok";
	olsonMap[L"Bangladesh Standard Time"] = "Asia/Dhaka";
	olsonMap[L"Beijing"] = "Asia/Shanghai";
	olsonMap[L"Canada Central"] = "America/Regina";
	olsonMap[L"Canada Central Standard Time"] = "America/Regina";
	olsonMap[L"Cape Verde Standard Time"] = "Atlantic/Cape_Verde";
	olsonMap[L"Caucasus"] = "Asia/Yerevan";
	olsonMap[L"Caucasus Standard Time"] = "Asia/Yerevan";
	olsonMap[L"Cen. Australia"] = "Australia/Adelaide";
	olsonMap[L"Cen. Australia Standard Time"] = "Australia/Adelaide";
	olsonMap[L"Central"] = "America/Chicago";
	olsonMap[L"Central America Standard Time"] = "America/Regina";
	olsonMap[L"Central Asia"] = "Asia/Almaty";
	olsonMap[L"Central Asia Standard Time"] = "Asia/Almaty";
	olsonMap[L"Central Brazilian Standard Time"] = "America/Cuiaba";
	olsonMap[L"Central Europe"] = "Europe/Prague";
	olsonMap[L"Central Europe Standard Time"] = "Europe/Prague";
	olsonMap[L"Central European"] = "Europe/Belgrade";
	olsonMap[L"Central European Standard Time"] = "Europe/Belgrade";
	olsonMap[L"Central Pacific"] = "Pacific/Guadalcanal";
	olsonMap[L"Central Pacific Standard Time"] = "Pacific/Guadalcanal";
	olsonMap[L"Central Standard Time"] = "America/Chicago";
	olsonMap[L"Central Standard Time (Mexico)"] = "America/Mexico_City";
	olsonMap[L"China"] = "Asia/Shanghai";
	olsonMap[L"China Standard Time"] = "Asia/Shanghai";
	olsonMap[L"Dateline"] = "-1200";
	olsonMap[L"Dateline Standard Time"] = "-1200";
	olsonMap[L"E. Africa"] = "Africa/Nairobi";
	olsonMap[L"E. Africa Standard Time"] = "Africa/Nairobi";
	olsonMap[L"E. Australia"] = "Australia/Brisbane";
	olsonMap[L"E. Australia Standard Time"] = "Australia/Brisbane";
	olsonMap[L"E. Europe"] = "Europe/Minsk";
	olsonMap[L"E. Europe Standard Time"] = "Europe/Minsk";
	olsonMap[L"E. South America"] = "America/Sao_Paulo";
	olsonMap[L"E. South America Standard Time"] = "America/Sao_Paulo";
	olsonMap[L"Eastern"] = "America/New_York";
	olsonMap[L"Eastern Standard Time"] = "America/New_York";
	olsonMap[L"Egypt"] = "Africa/Cairo";
	olsonMap[L"Egypt Standard Time"] = "Africa/Cairo";
	olsonMap[L"Ekaterinburg"] = "Asia/Yekaterinburg";
	olsonMap[L"Ekaterinburg Standard Time"] = "Asia/Yekaterinburg";
	olsonMap[L"Fiji"] = "Pacific/Fiji";
	olsonMap[L"Fiji Standard Time"] = "Pacific/Fiji";
	olsonMap[L"FLE"] = "Europe/Helsinki";
	olsonMap[L"FLE Standard Time"] = "Europe/Helsinki";
	olsonMap[L"Georgian Standard Time"] = "Asia/Tbilisi";
	olsonMap[L"GFT"] = "Europe/Athens";
	olsonMap[L"GFT Standard Time"] = "Europe/Athens";
	olsonMap[L"GMT"] = "Europe/London";
	olsonMap[L"GMT Standard Time"] = "Europe/London";
	olsonMap[L"Greenland Standard Time"] = "America/Godthab";
	olsonMap[L"Greenwich"] = "GMT";
	olsonMap[L"Greenwich Standard Time"] = "GMT";
	olsonMap[L"GTB"] = "Europe/Athens";
	olsonMap[L"GTB Standard Time"] = "Europe/Athens";
	olsonMap[L"Hawaiian"] = "Pacific/Honolulu";
	olsonMap[L"Hawaiian Standard Time"] = "Pacific/Honolulu";
	olsonMap[L"India"] = "Asia/Calcutta";
	olsonMap[L"India Standard Time"] = "Asia/Calcutta";
	olsonMap[L"Iran"] = "Asia/Tehran";
	olsonMap[L"Iran Standard Time"] = "Asia/Tehran";
	olsonMap[L"Israel"] = "Asia/Jerusalem";
	olsonMap[L"Israel Standard Time"] = "Asia/Jerusalem";
	olsonMap[L"Jordan Standard Time"] = "Asia/Amman";
	olsonMap[L"Kamchatka Standard Time"] = "Asia/Kamchatka";
	olsonMap[L"Korea"] = "Asia/Seoul";
	olsonMap[L"Korea Standard Time"] = "Asia/Seoul";
	olsonMap[L"Magadan Standard Time"] = "Asia/Magadan";
	olsonMap[L"Mauritius Standard Time"] = "Indian/Mauritius";
	olsonMap[L"Mexico"] = "America/Mexico_City";
	olsonMap[L"Mexico Standard Time"] = "America/Mexico_City";
	olsonMap[L"Mexico Standard Time 2"] = "America/Chihuahua";
	olsonMap[L"Mid-Atlantic"] = "Atlantic/South_Georgia";
	olsonMap[L"Mid-Atlantic Standard Time"] = "Atlantic/South_Georgia";
	olsonMap[L"Middle East Standard Time"] = "Asia/Beirut";
	olsonMap[L"Montevideo Standard Time"] = "America/Montevideo";
	olsonMap[L"Morocco Standard Time"] = "Africa/Casablanca";
	olsonMap[L"Mountain"] = "America/Denver";
	olsonMap[L"Mountain Standard Time"] = "America/Denver";
	olsonMap[L"Mountain Standard Time (Mexico)"] = "America/Chihuahua";
	olsonMap[L"Myanmar Standard Time"] = "Asia/Rangoon";
	olsonMap[L"N. Central Asia Standard Time"] = "Asia/Novosibirsk";
	olsonMap[L"Namibia Standard Time"] = "Africa/Windhoek";
	olsonMap[L"Nepal Standard Time"] = "Asia/Katmandu";
	olsonMap[L"New Zealand"] = "Pacific/Auckland";
	olsonMap[L"New Zealand Standard Time"] = "Pacific/Auckland";
	olsonMap[L"Newfoundland"] = "America/St_Johns";
	olsonMap[L"Newfoundland Standard Time"] = "America/St_Johns";
	olsonMap[L"North Asia East Standard Time"] = "Asia/Irkutsk";
	olsonMap[L"North Asia Standard Time"] = "Asia/Krasnoyarsk";
	olsonMap[L"Pacific"] = "America/Los_Angeles";
	olsonMap[L"Pacific SA"] = "America/Santiago";
	olsonMap[L"Pacific SA Standard Time"] = "America/Santiago";
	olsonMap[L"Pacific Standard Time"] = "America/Los_Angeles";
	olsonMap[L"Pacific Standard Time (Mexico)"] = "America/Tijuana";
	olsonMap[L"Pakistan Standard Time"] = "Asia/Karachi";
	olsonMap[L"Paraguay Standard Time"] = "America/Asuncion";
	olsonMap[L"Prague Bratislava"] = "Europe/Prague";
	olsonMap[L"Romance"] = "Europe/Paris";
	olsonMap[L"Romance Standard Time"] = "Europe/Paris";
	olsonMap[L"Russian"] = "Europe/Moscow";
	olsonMap[L"Russian Standard Time"] = "Europe/Moscow";
	olsonMap[L"SA Eastern"] = "America/Cayenne";
	olsonMap[L"SA Eastern Standard Time"] = "America/Cayenne";
	olsonMap[L"SA Pacific"] = "America/Bogota";
	olsonMap[L"SA Pacific Standard Time"] = "America/Bogota";
	olsonMap[L"SA Western"] = "America/Guyana";
	olsonMap[L"SA Western Standard Time"] = "America/Guyana";
	olsonMap[L"Samoa"] = "Pacific/Apia";
	olsonMap[L"Samoa Standard Time"] = "Pacific/Apia";
	olsonMap[L"Saudi Arabia"] = "Asia/Riyadh";
	olsonMap[L"Saudi Arabia Standard Time"] = "Asia/Riyadh";
	olsonMap[L"SE Asia"] = "Asia/Bangkok";
	olsonMap[L"SE Asia Standard Time"] = "Asia/Bangkok";
	olsonMap[L"Singapore"] = "Asia/Singapore";
	olsonMap[L"Singapore Standard Time"] = "Asia/Singapore";
	olsonMap[L"South Africa"] = "Africa/Harare";
	olsonMap[L"South Africa Standard Time"] = "Africa/Harare";
	olsonMap[L"Sri Lanka"] = "Asia/Colombo";
	olsonMap[L"Sri Lanka Standard Time"] = "Asia/Colombo";
	olsonMap[L"Syria Standard Time"] = "Asia/Damascus";
	olsonMap[L"Sydney Standard Time"] = "Australia/Sydney";
	olsonMap[L"Taipei"] = "Asia/Taipei";
	olsonMap[L"Taipei Standard Time"] = "Asia/Taipei";
	olsonMap[L"Tasmania"] = "Australia/Hobart";
	olsonMap[L"Tasmania Standard Time"] = "Australia/Hobart";
	olsonMap[L"Tokyo"] = "Asia/Tokyo";
	olsonMap[L"Tokyo Standard Time"] = "Asia/Tokyo";
	olsonMap[L"Tonga Standard Time"] = "Pacific/Tongatapu";
	olsonMap[L"Ulaanbaatar Standard Time"] = "Asia/Ulaanbaatar";
	olsonMap[L"US Eastern"] = "America/Indianapolis";
	olsonMap[L"US Eastern Standard Time"] = "America/Indianapolis";
	olsonMap[L"US Mountain"] = "America/Phoenix";
	olsonMap[L"US Mountain Standard Time"] = "America/Phoenix";
	olsonMap[L"UTC"] = "UTC";
	olsonMap[L"UTC+12"] = "+1200";
	olsonMap[L"UTC-02"] = "-0200";
	olsonMap[L"UTC-11"] = "-1100";
	olsonMap[L"Venezuela Standard Time"] = "America/Caracas";
	olsonMap[L"Vladivostok"] = "Asia/Vladivostok";
	olsonMap[L"Vladivostok Standard Time"] = "Asia/Vladivostok";
	olsonMap[L"W. Australia"] = "Australia/Perth";
	olsonMap[L"W. Australia Standard Time"] = "Australia/Perth";
	olsonMap[L"W. Central Africa Standard Time"] = "Africa/Luanda";
	olsonMap[L"W. Europe"] = "Europe/Berlin";
	olsonMap[L"W. Europe Standard Time"] = "Europe/Berlin";
	olsonMap[L"Warsaw"] = "Europe/Warsaw";
	olsonMap[L"West Asia"] = "Asia/Karachi";
	olsonMap[L"West Asia Standard Time"] = "Asia/Karachi";
	olsonMap[L"West Pacific"] = "Pacific/Guam";
	olsonMap[L"West Pacific Standard Time"] = "Pacific/Guam";
	olsonMap[L"Western Brazilian Standard Time"] = "America/Rio_Branco";
	olsonMap[L"Yakutsk"] = "Asia/Yakutsk";
	olsonMap[L"Yakutsk Standard Time"] = "Asia/Yakutsk";
}

SystemTimeZone::SystemTimeZone() {    
    buildOlsonMap();
	
	// Use Windows API to determine daylight savings time status
	TIME_ZONE_INFORMATION TimeZoneInfo;
	DWORD returnCode;
	returnCode = GetTimeZoneInformation( &TimeZoneInfo );
	
	std::wstring windowsName;
	wchar_t* wideName = (wchar_t*) TimeZoneInfo.StandardName;
	if (wideName) {
		windowsName = wideName;
	}

	// Get daylight savings status
	switch (returnCode) {
		case TIME_ZONE_ID_UNKNOWN:
			_isDaylight = false;
			break;
		case TIME_ZONE_ID_STANDARD:
			_isDaylight = false;
			break;
		case TIME_ZONE_ID_DAYLIGHT:
			_isDaylight = true;
			break;
		default:
			_isDaylight = false;
			break;
	}

	// Find and convert string
	std::map<std::wstring,std::string>::iterator it;
	it = olsonMap.find(windowsName);
	if (it == olsonMap.end()) {
		_name = "UTC";  // Default to UTC
	} else {
		_name = (*it).second;
	}
}

SystemTimeZone::~SystemTimeZone() { }