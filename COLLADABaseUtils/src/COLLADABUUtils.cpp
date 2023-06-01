/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of COLLADAStreamWriter.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADABUStableHeaders.h"
#include "COLLADABUUtils.h"
#include "COLLADABUPlatform.h"

#include <string.h>
#include <list>

namespace COLLADABU
{
	const String Utils::EMPTY_STRING = String();

    //---------------------------------
    String Utils::checkNCName ( const String &ncName )
    {
        String result;
        result.reserve ( ncName.length() );

        // check if first character is an alpha character
        char firstCharacter = ncName[0];

        if ( isAsciiAlphaChar ( firstCharacter )  )
            result.append ( 1, firstCharacter );
        else
            result.append ( 1, '_' );

        //replace all spaces and colons by underlines
        for ( size_t i = 1; i<ncName.length(); ++i )
        {
            char character = ncName[i];

            if ( isIDChar ( character ) )
                result.append ( 1, character );
            else
                result.append ( 1, '_' );
        }

        return result;
    }

    //---------------------------------
    String Utils::checkID ( const String &id )
    {
        return checkNCName ( id );
    }

    //---------------------------------
    String Utils::translateToXML ( const String &srcString )
    {
        String returnString = "";

        for ( unsigned int i=0; i<srcString.length(); ++i )
        {
            switch ( srcString[i])
            {
//             case '\r':
//                 returnString += "&#13";
//                 break;
            case '<':
                returnString += "&lt;";
                break;
            case '>':
                returnString += "&gt;";
                break;
            case '&':
                returnString += "&amp;";
                break;
            case '"':
                returnString += "&quot;";
                break;
            case '\'':
                returnString += "&apos;";
                break;
            default :
                returnString += srcString[i];
            }
        }

        return returnString;
    }



    //---------------------------------
    String Utils::replaceDot ( const String &text )
    {
        std::stringstream stream;

        for ( size_t i = 0; i < text.length(); ++i )
        {
            if ( text[i] == '.' )
                stream << '_';
            else
                stream << text[i];
        }

        return stream.str();
    }

    //---------------------------------
    void Utils::stringFindAndReplace ( String &source, const String searchString, const String replaceString )
    {
        size_t found = source.find ( searchString );
        if ( found != String::npos )
        {
            size_t searchStrLength = searchString.length();
            size_t replaceStrLength = replaceString.length();
            do
            {
                source.replace ( found, searchStrLength, replaceString );
                found = source.find (searchString, found + replaceStrLength );
            } while ( found != String::npos );
        }

    }

    //---------------------------------
    bool Utils::equals ( const String &str1, const String &str2 )
    {
        return ( strcmp ( str1.c_str(), str2.c_str() ) == 0 );
    }

    //--------------------------------
    bool Utils::equalsIgnoreCase ( const String& s1, const String& s2 )
    {
        String::const_iterator it1=s1.begin();
        String::const_iterator it2=s2.begin();

        // has the end of at least one of the strings been reached?
        while ( (it1!=s1.end()) && (it2!=s2.end()) )
        {
            if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
                // return -1 to indicate 'smaller than', 1 otherwise
                return false;
            // proceed to the next character in each string
            ++it1;
            ++it2;
        }
        size_t size1=s1.size(), size2=s2.size();// cache lengths
        //return -1,0 or 1 according to strings' lengths
        if (size1==size2)
            return true;
        return false;
    }

    //--------------------------------
	Utils::SystemType Utils::getSystemType()
	{
#ifdef COLLADABU_OS_WIN
		return WINDOWS;
#else
		return POSIX;
#endif
	}
}
