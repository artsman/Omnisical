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

#include "OmnisTools.he"
#include <sstream>
#include <iostream>
#include <map>

#ifdef USE_BOOST
  #include <boost/lexical_cast.hpp>

  using boost::lexical_cast;
  using boost::bad_lexical_cast;
#endif

#if ! defined(MARKUP_SIZEOFWCHAR)
#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
#define MARKUP_SIZEOFWCHAR 4
#else
#define MARKUP_SIZEOFWCHAR 2
#endif
#endif

// Get a parameter from the thread data
qbool OmnisTools::getParamVar( tThreadData* pThreadData, qshort pParamNum, EXTfldval& pOutVar )
{
	return getParamVar(pThreadData->mEci, pParamNum, pOutVar);
}

// Get a parameter from the EXTCompInfo
qbool OmnisTools::getParamVar( EXTCompInfo* pEci, qshort pParamNum, EXTfldval& pOutVar )
{
	EXTParamInfo* param = ECOfindParamNum( pEci, pParamNum );
	if( !param )
		return qfalse;
	
	pOutVar.setFldVal(reinterpret_cast<qfldval>(param->mData));
	pOutVar.setReadOnly( qfalse );
	
	return qtrue;
}

qbool OmnisTools::getParamList( tThreadData* pThreadData, qshort pParamNum, EXTqlist& pOutList, qbool pCanBeRow) {
	
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param ) 
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	if ( isList(fval,pCanBeRow) != qtrue)
		return qfalse;
	
	fval.getList(&pOutList,qfalse);
	
	return qtrue;
}

qbool OmnisTools::isList( EXTfldval& pFVal, qbool pCanBeRow ) {
	ffttype fft; pFVal.getType(fft);
	return ( (fft == fftList || (pCanBeRow && fft == fftRow)) ? qtrue : qfalse );
}

qbool OmnisTools::getParamBool( tThreadData* pThreadData, qshort pParamNum, qbool& pOutBool ) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param )
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	
	ffttype fft; fval.getType(fft);
	if ( fft != fftInteger && fft != fftBoolean && fft != fftConstant )
		return qfalse;
	
	qlong x = fval.getLong();
	if ( x != 0 && x != 1 )
		return qfalse;
	
	pOutBool = qbool(x != 0);
	return qtrue;
}

qbool OmnisTools::getParamShort( tThreadData* pThreadData, qshort pParamNum, qshort& pOutShort ) {
	
	qlong longVal;
	if( getParamLong(pThreadData, pParamNum, longVal) != qtrue )
		return qfalse;
	
	if (longVal < 0 || longVal > 255)
		return qfalse;
	
	pOutShort = static_cast<qshort>(longVal);
	return qtrue;	
}

qbool OmnisTools::getParamLong( tThreadData* pThreadData, qshort pParamNum, qlong& pOutInteger ) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param )
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	
	ffttype fft; fval.getType(fft);
	if ( fft == fftCharacter )
	{
		strxxx& s_num = fval.getChar();
		if ( stringToQlong( s_num, pOutInteger ) )
			return qtrue;
	}
	else if ( fft == fftInteger || fft == fftBoolean || fft == fftConstant || fft == fftNumber )
	{
		pOutInteger = fval.getLong();
		return qtrue;
	}
	
	return qfalse;
}

// Convenience method to return the type of an EXTfldval
OmnisTools::FieldValType OmnisTools::getType( EXTfldval& fVal ) {
	
	OmnisTools::FieldValType retType;
	fVal.getType(retType.valType, &retType.valSubType);
	
	return retType;
}

// Get a qbool from a C++ boolean
qbool OmnisTools::getQBoolFromBool(bool b) {
	if (b == true) {
		return qtrue;
	} else if (b == false) {
		return qfalse;
	} else {
		return 0;
	}
}

// Get a C++ boolean from a qbool
bool OmnisTools::getBoolFromQBool(qbool qb) {
	if (qb == qtrue) {
		return true;
	} else if (qb == qfalse) {
		return false;
	} else {
		return NULL;
	}
}

// Get a std::wstring from an EXTfldval object
std::wstring OmnisTools::getWStringFromEXTFldVal(EXTfldval& fVal) {
	std::wstring retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	wchar_t* cString;
#if MARKUP_SIZEOFWCHAR == 2
	// For 2-Byte UTF16 wchar_t* (Typically Windows)
	// Convert from UTF8 to UTF16 and set new stringLength
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 to UTF16
	CHRconvToUtf16 utf16conv(utf8data, stringLength);
	UChar* utf16data = utf16conv.dataPtr();
	stringLength = utf16conv.len();
	
	// Translate UTF16 binary into char* string
	cString = reinterpret_cast<wchar_t*>(utf16data);
#else
	// For 4-Byte UTF32 wchar_t* (Typically Mac and Linux)
	// Convert from UTF8 to UTF32 and set new stringLength
	stringLength = length;
	CHRconvToUtf32FromChar utf32conv(omnisString, stringLength, qfalse);
	U32Char* utf32data = utf32conv.dataPtr();
	stringLength = utf32conv.len();
	
	// Translate UTF16 binary into char* string
	cString = reinterpret_cast<wchar_t*>(utf32data);
#endif
	
	// Create standard string
	retString = std::wstring(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromWString(EXTfldval& fVal, const std::wstring readString) {
	qlong length;
	qchar* omnisString = getQCharFromWString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromWChar(EXTfldval& fVal, const wchar_t* readChar) {
    std::wstring readString;
    if (readChar == 0)
        readString = L"";
    else
        readString = readChar;
    
    getEXTFldValFromWString(fVal, readString);
}

// Get a dynamically allocated qchar* array from a std::wstring
qchar* OmnisTools::getQCharFromWString(const std::wstring readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	wchar_t* cString = const_cast<wchar_t*>(readString.c_str());
	
    // Declare pointer to new data
    qchar* omnisString;
    
#if MARKUP_SIZEOFWCHAR == 2
	// For 2-Byte UTF16 wchar_t* (Typically Windows)
	// Feed into raw byte data
	UChar* utf16data = reinterpret_cast<UChar*> (cString);
	
	// Convert to UTF-8
	CHRconvFromUtf16 utf16conv(utf16data, length);
	length = utf16conv.len();
	qbyte* utf8data = utf16conv.dataPtr();
	
	// Allocate new qchar* string
	omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
#else
	// For 4-Byte UTF32 wchar_t* (Typically Mac and Linux)
	U32Char* utf32data = reinterpret_cast<U32Char*> (cString);
	
	// Convert to UTF-8
	CHRconvFromUtf32ToChar utf32conv(utf32data, length, qfalse);
	retLength = length = utf32conv.len();
	
	omnisString = new qchar[length];
	OMstrcpy(omnisString, utf32conv.dataPtr()); // Copy string so it lives past the end of this function
#endif
	
	return omnisString;
}

// Get a std::string from an EXTfldval object
std::string OmnisTools::getStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 binary into char* string
	char* cString = reinterpret_cast<char*> (utf8data);
	
	// Create standard string
	retString = std::string(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::string
void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, const std::string readString) {
	qlong length;
	qchar* omnisString = getQCharFromString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromChar(EXTfldval& fVal, const char* readChar) {
    std::string readString;
    if (readChar)
        readString = readChar;
    else
        readString = "";
    
    getEXTFldValFromString(fVal, readString);
}

// Get a dynamically allocated qchar* array from a std::string
qchar* OmnisTools::getQCharFromString(const std::string readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	char* cString = const_cast<char*>(readString.c_str());
	
	// Feed into raw byte data
	qbyte* utf8data = reinterpret_cast<qbyte*> (cString);
	
	// Allocate new qchar* string
	qchar* omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
	
	return omnisString;
}

// Get a str15 object for a character constant (No string if it doesn't correspond to the conditions)
str15 OmnisTools::initStr15(const char* in){
    str15 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 15) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Get a str31 object for a character constant (No string if it doesn't correspond to the conditions)
str31 OmnisTools::initStr31(const char* in){
    str31 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 31) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Get a str80 object for a character constant (No string if it doesn't correspond to the conditions)
str80 OmnisTools::initStr80(const char* in){
    str80 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 80) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Get a str255 object for a character constant (No string if it doesn't correspond to the conditions)
str255 OmnisTools::initStr255(const char* in) {
    str255 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 255) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Return a C++ bool from an EXTfldval
bool OmnisTools::getBoolFromEXTFldVal(EXTfldval& fVal) {
	qshort omnBool;
	omnBool = fVal.getBool();
	switch (omnBool) {
		case 2:
			return true;
		case 1:
			return false;
		default:
			return false;
	}
}

// Get an EXTfldval for a C++ bool
void OmnisTools::getEXTFldValFromBool(EXTfldval& fVal, bool b) {
	qshort omBool;
	if (b==true) 
        omBool = 2;
	else if (b==false)
        omBool = 1;
    
	fVal.setBool(omBool);
}

// Return a C++ int from an EXTfldval
int OmnisTools::getIntFromEXTFldVal(EXTfldval& fVal) {
	qlong omnInt = fVal.getLong();
	
	if (omnInt < INT_MIN || omnInt > INT_MAX) {
        omnInt = 0; // zero out any numbers that exceed
	}
	
	return static_cast<int>( omnInt );
}

// Get an EXTfldval for a C++ int
void OmnisTools::getEXTFldValFromInt(EXTfldval& fVal, int i) {
	fVal.setLong(static_cast<qlong>(i));
}

// Return a C++ long from an EXTfldval
long OmnisTools::getLongFromEXTFldVal(EXTfldval& fVal) {
	long retLong = static_cast<long>(fVal.getLong());
	return retLong;
}

// Get an EXTfldval for a C++ long
void OmnisTools::getEXTFldValFromLong(EXTfldval& fVal, long l) {
	fVal.setLong(static_cast<qlong>(l));
}

// Return a C++ long from an EXTfldval
float OmnisTools::getFloatFromEXTFldVal(EXTfldval& fVal) {
	qreal omnReal; qshort dp;
	fVal.getNum(omnReal, dp);
	
	float retFloat = static_cast<float>(omnReal);
	
	return retFloat;
}

// Get an EXTfldval for a C++ double
void OmnisTools::getEXTFldValFromFloat(EXTfldval& fVal, float d) {
	qreal omnReal = static_cast<qreal>(d);
	qshort dp = dpFmask;
	fVal.setNum(omnReal, dp);
}

// Return a C++ long from an EXTfldval
double OmnisTools::getDoubleFromEXTFldVal(EXTfldval& fVal) {
	qreal omnReal; qshort dp;
	fVal.getNum(omnReal, dp);
	
	double retDbl = static_cast<double>(omnReal);
	
	return retDbl;
}

// Get an EXTfldval for a C++ double
void OmnisTools::getEXTFldValFromDouble(EXTfldval& fVal, double d) {
	qreal omnReal = static_cast<qreal>(d);
	qshort dp = dpFmask;
	fVal.setNum(omnReal, dp);
}

// Get an EXTfldval for a defined constant
void OmnisTools::getEXTFldValFromConstant(EXTfldval& fVal, qlong constID, qlong prefixID) {
    
	// Check for prefix
	str80 prefixRead;
	str255 constantValue;
	if (prefixID > 0) {
		// Read string from resource, and assign it to return parameter
		RESloadString(gInstLib,prefixID,prefixRead);
		constantValue.concat(prefixRead);
	}
    
	// Read complete resource string
	str255 resourceValue;
	RESloadString(gInstLib,constID,resourceValue);
	
	// Translate into std::wstring for easy substring
	EXTfldval convVar;
	convVar.setChar(resourceValue, dpDefault);
	std::wstring resourceString = getWStringFromEXTFldVal(convVar);
	
	// Get substring between tilde (sometimes used for categories) and first colon.
	int tildePos = resourceString.find(L"~") + 1;
	int colonPos = resourceString.find(L":");
	
	std::wstring constantString = resourceString.substr(tildePos,colonPos-tildePos);
	getEXTFldValFromWString(convVar, constantString);
	
	// Add constant to EXTfldval
	constantValue.concat(convVar.getChar());
	fVal.setConstant(constantValue);
}

// Get an integer for an EXTfldval where the EXTfldval contains a constant
#ifdef USE_BOOST
static std::map<std::wstring,int> constCache;
int OmnisTools::getIntFromEXTFldVal(EXTfldval& fVal, qlong firstID, qlong lastID) {
	
	if (getType(fVal).valType == fftInteger) {
		return getIntFromEXTFldVal(fVal);
	} else if (getType(fVal).valType == fftConstant) {
		return getIntFromEXTFldVal(fVal);
	}
	
	int retNum = -1;
	// Get string that needs to be matched
	std::wstring matchString = getWStringFromEXTFldVal(fVal);
	
	// Get map iterator for searching
	std::map<std::wstring,int>::iterator it;
	it = constCache.find(matchString);
	if (it != constCache.end()) {
		retNum = it->second;
	} else {
		// Unable to find match, must loop constants in range and look for it
		int tildePos, colonPos, numPos, constNum;
		EXTfldval convVar;
		str255 resourceValue;
		std::wstring resourceMatch, resourceString;
		std::wstring numString = L"";
		
		for( int i = firstID; i <= lastID; ++i) {
			// Load resource and put into std::wstring for easy substr
			RESloadString(gInstLib,i,resourceValue);
			convVar.setChar(resourceValue, dpDefault);
			resourceString = getWStringFromEXTFldVal(convVar);
			tildePos = resourceString.find(L"~") + 1;
			colonPos = resourceString.find(L":");
			if (colonPos != -1) { // All constants should have colons.  If it doesn't then don't interpret the line
				resourceMatch = resourceString.substr(tildePos, colonPos-tildePos);
				
				// While looping add items to the const cache
				numPos = colonPos + 1;
				numString.clear();
				while (resourceString[numPos] != L':' && numPos < static_cast<int>(resourceString.length())) {
					numString += resourceString[numPos++];
				}
				try {
					constNum = lexical_cast<int>(numString);
				}
				catch(bad_lexical_cast &) {
					constNum = -1;
				}
				
				constCache[resourceMatch] = constNum;  // Add constant to cache
			}
		}
		
		// Locate constant now that all the constants have been cached
		it = constCache.find(matchString);
		if (it != constCache.end()) {
			retNum = it->second;
		}
	}
	return retNum;
}
#endif // USE_BOOST

// Get an ISO 8601 Formatted Date String from EXTFldVal
std::string OmnisTools::getISO8601DateStringFromEXTFldVal(EXTfldval& fVal) {
	datestamptype theDate;
	std::string retString;
	std::stringstream sin;
	
	fVal.getDate(theDate, dpFdtimeC);
	FieldValType theType = getType(fVal);
    
	if (theType.valType != fftDate)
		return "";
    
	// Set date part of string
    if (theDate.mDateOk == qtrue
        && !(theType.valSubType == dpFdtime1900
             || theType.valSubType == dpFdtime1980
             || theType.valSubType == dpFdtime2000
             || theType.valSubType == dpFdtimeC
             || theType.valSubType == dpFtime))
    {	
        sin << int(theDate.mYear);
        if (theDate.mMonth < 10) {
            sin << "-0" << int(theDate.mMonth);
        } else {
            sin << "-" << int(theDate.mMonth);
        } 
        if (theDate.mDay < 10) {
            sin << "-0" << int(theDate.mDay);
        } else {
            sin << "-" << int(theDate.mDay);
        } 
	}
	
	// Set time part of string
    if (theDate.mTimeOk == qtrue
        && !(theType.valSubType == dpFdate1900
             || theType.valSubType == dpFdate1900
             || theType.valSubType == dpFdate1980
             || theType.valSubType == dpFdate2000
             || theType.valSubType == dpFdtime1900
             || theType.valSubType == dpFdtime1980
             || theType.valSubType == dpFdtime2000
             || theType.valSubType == dpFdtimeC))
    {
        sin << "T";
        if (theDate.mHour < 10) {
            sin << "0" << int(theDate.mHour);
        } else {
            sin << int(theDate.mHour);
        }
        if (theDate.mMin < 10) {
            sin << ":0" << int(theDate.mMin);
        } else {
            sin << ":" << int(theDate.mMin);
        }
        if (theDate.mSecOk) {
            if (theDate.mSec < 10) {
                sin << ":0" << int(theDate.mSec);
            } else {
                sin << ":" << int(theDate.mSec);
            }
        }
        // NOTE: Even though Omnis contains hundredths data, it is not part of
        //       the ISO8601 format
    }
    
    // Place string into return value
    retString = sin.str();
	
	return retString;
}

qbool OmnisTools::ensurePosixPath(EXTfldval& pathVal) {
#ifdef ismac
	str255 posixCheck, posixPath;
	qshort def = dpFcharacter;
	posixCheck = pathVal.getChar().cString();
	qlong err;
	std::wstring path = getWStringFromEXTFldVal(pathVal);
	
	if (path[0] != L'/') {
		err = ECOconvertHFSToPosix(posixCheck, posixPath);
		if (err != 0) {
			return qfalse;
		}
		pathVal.setChar(posixPath, def);
	}
#endif
	
	return qtrue;
}

static std::map<LPARAM,const char*> messageLookup;

void buildTranslations() {
    if( messageLookup.size() > 0) {
        return;
    }
    
    messageLookup[WM_NULL] = "WM_NULL";
    messageLookup[WM_CREATE] = "WM_CREATE";
    messageLookup[WM_DESTROY] = "WM_DESTROY";
    messageLookup[WM_SHOWWINDOW] = "WM_SHOWWINDOW";
    messageLookup[WM_MOVE] = "WM_MOVE";
    messageLookup[WM_SIZE] = "WM_SIZE";
    messageLookup[WM_NCPAINT] = "WM_NCPAINT";
    messageLookup[WM_PAINT] = "WM_PAINT";
    messageLookup[WM_ERASEBKGND] = "WM_ERASEBKGND";
    messageLookup[WM_TIMER] = "WM_TIMER";
    messageLookup[WM_NCACTIVATE] = "WM_NCACTIVATE";
    messageLookup[WM_WINDOWPOSCHANGING] = "WM_WINDOWPOSCHANGING";
    messageLookup[WM_WINDOWPOSCHANGED] = "WM_WINDOWPOSCHANGED";
    messageLookup[WM_GETMINMAXINFO] = "WM_GETMINMAXINFO";
    messageLookup[WM_PAUSEPLAY] = "WM_PAUSEPLAY";
    messageLookup[WM_RESUMEPLAY] = "WM_RESUMEPLAY";
    messageLookup[WM_MOUSEMOVE] = "WM_MOUSEMOVE";
    messageLookup[WM_LBUTTONDOWN] = "WM_LBUTTONDOWN";
    messageLookup[WM_LBUTTONUP] = "WM_LBUTTONUP";
    messageLookup[WM_LBUTTONDBLCLK] = "WM_LBUTTONDBLCLK";
    messageLookup[WM_RBUTTONDOWN] = "WM_RBUTTONDOWN";
    messageLookup[WM_RBUTTONUP] = "WM_RBUTTONUP";
    messageLookup[WM_RBUTTONDBLCLK] = "WM_RBUTTONDBLCLK";
    messageLookup[WM_NCLBUTTONDOWN] = "WM_NCLBUTTONDOWN";
    messageLookup[WM_KEYDOWN] = "WM_KEYDOWN";
    messageLookup[WM_KEYUP] = "WM_KEYUP";
    messageLookup[WM_HSCROLL] = "WM_HSCROLL";
    messageLookup[WM_VSCROLL] = "WM_VSCROLL";
    messageLookup[WM_SETCURSOR] = "WM_SETCURSOR";
    messageLookup[WM_DROPFILES] = "WM_DROPFILES";
    messageLookup[WM_MOUSEWHEEL] = "WM_MOUSEWHEEL";
    messageLookup[WM_USER] = "WM_USER";
    messageLookup[WM_EXUSER] = "WM_EXUSER";
    messageLookup[WM_GETERASEINFO] = "WM_GETERASEINFO";
    messageLookup[WM_CHILDPAINT] = "WM_CHILDPAINT";
    messageLookup[WM_KEYDOWNPREVIEW] = "WM_KEYDOWNPREVIEW";
    messageLookup[WM_KEYUPPREVIEW] = "WM_KEYUPPREVIEW";
    messageLookup[WM_BORDCALCRECT] = "WM_BORDCALCRECT";
    messageLookup[WM_BORDPAINT] = "WM_BORDPAINT";
    messageLookup[WM_SHOWSIZEGRIP] = "WM_SHOWSIZEGRIP";
    messageLookup[WM_DRAGBORDER] = "WM_DRAGBORDER";
    messageLookup[WM_FOCUSCHANGED] = "WM_FOCUSCHANGED";
    messageLookup[WM_ERASEBKGNDRECT] = "WM_ERASEBKGNDRECT";
    messageLookup[WM_GETSHADOWRECT] = "WM_GETSHADOWRECT";
    messageLookup[WM_BORDERCHANGING] = "WM_BORDERCHANGING";
    messageLookup[WM_BORDERCHANGED] = "WM_BORDERCHANGED";
    messageLookup[WM_BORDERASEBACKGROUND] = "WM_BORDERASEBACKGROUND";
    messageLookup[WM_MULTIKEYDOWNPREVIEW] = "WM_MULTIKEYDOWNPREVIEW";
    messageLookup[WM_CONTROL] = "WM_CONTROL";
    messageLookup[WM_FLD_ENABLE] = "WM_FLD_ENABLE";
    messageLookup[WM_FLD_DISABLE] = "WM_FLD_DISABLE";
    messageLookup[WM_FLD_FILLCHANGED] = "WM_FLD_FILLCHANGED";
    messageLookup[WM_FLD_FONTCHANGED] = "WM_FLD_FONTCHANGED";
    messageLookup[WM_FLD_WILLPOPUP] = "WM_FLD_WILLPOPUP";
    messageLookup[WM_FLD_LOCMENU] = "WM_FLD_LOCMENU";
    messageLookup[WM_FLD_DROPLISTEND] = "WM_FLD_DROPLISTEND";
    messageLookup[WM_DRAGDROP] = "WM_DRAGDROP";
    messageLookup[WM_FLD_TABCONTROL] = "WM_FLD_TABCONTROL";
    messageLookup[WM_TOOLTIP] = "WM_TOOLTIP";
    messageLookup[WM_FLD_RBUTTONDOWN] = "WM_FLD_RBUTTONDOWN";
    messageLookup[WM_FLD_DESIGN] = "WM_FLD_DESIGN";
    messageLookup[WM_FLD_SETMENU] = "WM_FLD_SETMENU";
    messageLookup[WM_FLD_EXECMENU] = "WM_FLD_EXECMENU";
    messageLookup[WM_FLD_CHILDPOSCHANGED] = "WM_FLD_CHILDPOSCHANGED";
    messageLookup[WM_EVALMOUSEOVER] = "WM_EVALMOUSEOVER";
    messageLookup[WM_GETBACKOBJECTS] = "WM_GETBACKOBJECTS";
    messageLookup[WM_CONTAINER] = "WM_CONTAINER";
    messageLookup[WM_GETITEMREF] = "WM_GETITEMREF";
    messageLookup[WM_FLD_HELP] = "WM_FLD_HELP";
    messageLookup[WM_FLD_SETDATA] = "WM_FLD_SETDATA";
    messageLookup[WM_FLD_GETDATA] = "WM_FLD_GETDATA";
    messageLookup[WM_FLD_CMPDATA] = "WM_FLD_CMPDATA";
    messageLookup[WM_FLD_GETDATALEN] = "WM_FLD_GETDATALEN";
    messageLookup[WM_FLD_DISABLE_VIEW] = "WM_FLD_DISABLE_VIEW";
    messageLookup[WM_FLD_ACTIVATE] = "WM_FLD_ACTIVATE";
    messageLookup[WM_FLD_SCROLLVIEW] = "WM_FLD_SCROLLVIEW";
    messageLookup[WM_PRI_INSTALL] = "WM_PRI_INSTALL";
    messageLookup[WM_PRI_REMOVE] = "WM_PRI_REMOVE";
    messageLookup[WM_THUMB_TIP] = "WM_THUMB_TIP";
    messageLookup[WM_GETTOOLTIPRECT] = "WM_GETTOOLTIPRECT";
    messageLookup[WM_FLD_NEEDCONTROLTAB] = "WM_FLD_NEEDCONTROLTAB";
    messageLookup[WM_CPLX_KILLROW] = "WM_CPLX_KILLROW";
    messageLookup[WM_CPLX_SETROW] = "WM_CPLX_SETROW";
    messageLookup[WM_CPLX_STARTDRAW] = "WM_CPLX_STARTDRAW";
    messageLookup[WM_CPLX_ENDDRAW] = "WM_CPLX_ENDDRAW";
    messageLookup[WM_CPLX_PAINT] = "WM_CPLX_PAINT";
    messageLookup[WM_FLD_GETTEXTSPEC] = "WM_FLD_GETTEXTSPEC";
    messageLookup[WM_FLD_DISABLEFLOAT] = "WM_FLD_DISABLEFLOAT";
    messageLookup[WM_FLD_STYLES] = "WM_FLD_STYLES";
    messageLookup[WM_FLD_REPLACESELFROMCATALOG] = "WM_FLD_REPLACESELFROMCATALOG";
    messageLookup[WM_FORMWNDSTATUS] = "WM_FORMWNDSTATUS";
    messageLookup[WM_STARTJVM] = "WM_STARTJVM";
    messageLookup[WM_FLD_CLOSENOTAHELP] = "WM_FLD_CLOSENOTAHELP";
    messageLookup[WM_MULTIKEYDOWN] = "WM_MULTIKEYDOWN";
    messageLookup[WM_DROPFILEDATA] = "WM_DROPFILEDATA";
    messageLookup[WM_GENERALFLAGS] = "WM_GENERALFLAGS";
    messageLookup[WM_GETANSICODEPAGE] = "WM_GETANSICODEPAGE";
    messageLookup[WM_TESTFORCANCEL] = "WM_TESTFORCANCEL";
    messageLookup[WM_SETMAINSTACK] = "WM_SETMAINSTACK";
    messageLookup[WM_RESERVED1] = "WM_RESERVED1";
    messageLookup[WM_FLD_PARENTCHANGED] = "WM_FLD_PARENTCHANGED";
    messageLookup[WM_ORFC_GETSCROLLAMT] = "WM_ORFC_GETSCROLLAMT";
    messageLookup[WM_ORFC_CURFIELDCHANGING] = "WM_ORFC_CURFIELDCHANGING";
    messageLookup[WM_GENERALFLAGS_DISABLE_ELLIPSIS] = "WM_GENERALFLAGS_DISABLE_ELLIPSIS";
    messageLookup[ECM_FIRST_MSG] = "ECM_FIRST_MSG";
    messageLookup[ECM_GETCOMPLIBINFO] = "ECM_GETCOMPLIBINFO";
    messageLookup[ECM_GETCOMPID] = "ECM_GETCOMPID";
    messageLookup[ECM_CONSTPREFIX] = "ECM_CONSTPREFIX";
    messageLookup[ECM_GETPROPNAME] = "ECM_GETPROPNAME";
    messageLookup[ECM_GETEVENTNAME] = "ECM_GETEVENTNAME";
    messageLookup[ECM_GETCONSTNAME] = "ECM_GETCONSTNAME";
    messageLookup[ECM_GETMETHODNAME] = "ECM_GETMETHODNAME";
    messageLookup[ECM_SETPROPERTY] = "ECM_SETPROPERTY";
    messageLookup[ECM_CONNECT] = "ECM_CONNECT";
    messageLookup[ECM_DISCONNECT] = "ECM_DISCONNECT";
    messageLookup[ECM_GETCOMPICON] = "ECM_GETCOMPICON";
    messageLookup[ECM_METHODCALL] = "ECM_METHODCALL";
    messageLookup[ECM_MEMORYDELETION] = "ECM_MEMORYDELETION";
    messageLookup[ECM_OBJCONSTRUCT] = "ECM_OBJCONSTRUCT";
    messageLookup[ECM_OBJDESTRUCT] = "ECM_OBJDESTRUCT";
    messageLookup[ECM_GETPICTUREDIM] = "ECM_GETPICTUREDIM";
    messageLookup[ECM_PROPERTYCANASSIGN] = "ECM_PROPERTYCANASSIGN";
    messageLookup[ECM_GETPROPERTY] = "ECM_GETPROPERTY";
    messageLookup[ECM_DEBUGGING] = "ECM_DEBUGGING";
    messageLookup[ECM_INSTALLLIBRARY] = "ECM_INSTALLLIBRARY";
    messageLookup[ECM_GETPROPERTYENUMS] = "ECM_GETPROPERTYENUMS";
    messageLookup[ECM_OBJECTDATABLOCK] = "ECM_OBJECTDATABLOCK";
    messageLookup[ECM_XFCEVENT] = "ECM_XFCEVENT";
    messageLookup[ECM_GETHANDLERICON] = "ECM_GETHANDLERICON";
    messageLookup[ECM_NEWPROPERTYFLAGS] = "ECM_NEWPROPERTYFLAGS";
    messageLookup[ECM_SETPRIMARYDATA] = "ECM_SETPRIMARYDATA";
    messageLookup[ECM_GETPRIMARYDATA] = "ECM_GETPRIMARYDATA";
    messageLookup[ECM_CMPPRIMARYDATA] = "ECM_CMPPRIMARYDATA";
    messageLookup[ECM_GETPRIMARYDATALEN] = "ECM_GETPRIMARYDATALEN";
    messageLookup[ECM_PRIMARYDATACHANGE] = "ECM_PRIMARYDATACHANGE";
    messageLookup[ECM_HELP] = "ECM_HELP";
    messageLookup[ECM_CUSTOMTABNAME] = "ECM_CUSTOMTABNAME";
    messageLookup[ECM_LISTDRAWLINE] = "ECM_LISTDRAWLINE";
    messageLookup[ECM_PRINT] = "ECM_PRINT";
    messageLookup[ECM_PRINTMAPPING] = "ECM_PRINTMAPPING";
    messageLookup[ECM_GETPROCEDURE] = "ECM_GETPROCEDURE";
    messageLookup[ECM_GETEVENTMETHOD] = "ECM_GETEVENTMETHOD";
    messageLookup[ECM_OBJINITIALIZE] = "ECM_OBJINITIALIZE";
    messageLookup[ECM_GETOBJECTRECT] = "ECM_GETOBJECTRECT";
    messageLookup[ECM_PAINTCONTENTS] = "ECM_PAINTCONTENTS";
    messageLookup[ECM_NEWMETHODFLAGS] = "ECM_NEWMETHODFLAGS";
    messageLookup[ECM_BOBJ_EXERASE] = "ECM_BOBJ_EXERASE";
    messageLookup[ECM_FMT_CANASSIGN] = "ECM_FMT_CANASSIGN";
    messageLookup[ECM_FMT_SETPROPERTY] = "ECM_FMT_SETPROPERTY";
    messageLookup[ECM_FMT_GETPROPERTY] = "ECM_FMT_GETPROPERTY";
    messageLookup[ECM_GETVERSION] = "ECM_GETVERSION";
    messageLookup[ECM_ICONDRAWENTRY] = "ECM_ICONDRAWENTRY";
    messageLookup[ECM_TEXTDRAWENTRY] = "ECM_TEXTDRAWENTRY";
    messageLookup[ECM_ISUNICODE] = "ECM_ISUNICODE";
    messageLookup[ECM_ADDTOPRINTJOB] = "ECM_ADDTOPRINTJOB";
    messageLookup[ECM_COMPONENTCMD] = "ECM_COMPONENTCMD";
    messageLookup[ECM_CANADDTOPRINTJOB] = "ECM_CANADDTOPRINTJOB";
    messageLookup[ECM_GETOBJECT] = "ECM_GETOBJECT";
    messageLookup[ECM_GETSTATICOBJECT] = "ECM_GETSTATICOBJECT";
    messageLookup[ECM_PROPERTYCALCTYPE] = "ECM_PROPERTYCALCTYPE";
    messageLookup[ECM_OBJECT_REBUILD] = "ECM_OBJECT_REBUILD";
    messageLookup[ECM_OBJECT_COPY] = "ECM_OBJECT_COPY";
    messageLookup[ECM_GETPICTFORMAT] = "ECM_GETPICTFORMAT";
    messageLookup[ECM_CONVTOHPIXMAP] = "ECM_CONVTOHPIXMAP";
    messageLookup[ECM_CONVFROMHPIXMAP] = "ECM_CONVFROMHPIXMAP";
    messageLookup[ECM_ISCONVFORMAT] = "ECM_ISCONVFORMAT";
    messageLookup[ECM_CONVHEADER] = "ECM_CONVHEADER";
    messageLookup[ECM_GETPICTFILEDESC] = "ECM_GETPICTFILEDESC";
    messageLookup[ECM_EVENTRESULT] = "ECM_EVENTRESULT";
    messageLookup[ECM_GETCOMPSTOREGROUP] = "ECM_GETCOMPSTOREGROUP";
    messageLookup[ECM_GETCOMPSTOREICON] = "ECM_GETCOMPSTOREICON";
    messageLookup[ECM_INBUILT_OVERRIDE] = "ECM_INBUILT_OVERRIDE";
    messageLookup[ECM_HASPRIMARYDATACHANGED] = "ECM_HASPRIMARYDATACHANGED";
    messageLookup[ECM_CANFOCUS] = "ECM_CANFOCUS";
    messageLookup[ECM_CANCLICK] = "ECM_CANCLICK";
    messageLookup[ECM_DAM_GETCOLTEXT] = "ECM_DAM_GETCOLTEXT";
    messageLookup[ECM_DAM_NULLTEXT] = "ECM_DAM_NULLTEXT";
    messageLookup[ECM_DAM_NOTNULLTEXT] = "ECM_DAM_NOTNULLTEXT";
    messageLookup[ECM_SQLOBJECT_COPY] = "ECM_SQLOBJECT_COPY";
    messageLookup[ECM_CANSHOWSYSTEMFOCUS] = "ECM_CANSHOWSYSTEMFOCUS";
    messageLookup[ECM_AUTOMATIONMSG] = "ECM_AUTOMATIONMSG";
    messageLookup[ECM_START_EVAFTER] = "ECM_START_EVAFTER";
    messageLookup[ECM_END_EVAFTER] = "ECM_END_EVAFTER";
    messageLookup[ECM_DOCGEN_ALLPROPS] = "ECM_DOCGEN_ALLPROPS";
}

str255 OmnisTools::translateMessage(LPARAM msg) {
    
    buildTranslations();
    
    return initStr255(messageLookup[msg]);
}




