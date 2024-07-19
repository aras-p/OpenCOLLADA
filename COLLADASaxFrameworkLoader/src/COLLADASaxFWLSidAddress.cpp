/*
Copyright (c) 2008-2009 NetAllied Systems GmbH

This file is part of COLLADASaxFrameworkLoader.

Licensed under the MIT Open Source License, 
for details please see LICENSE file or the website
http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADASaxFWLStableHeaders.h"
#include "COLLADASaxFWLSidAddress.h"

namespace COLLADASaxFWL
{

	const int regExpMatchesVectorLength = 30;    /* should be a multiple of 3 */
	const char* sidSeparator = "/";

	//------------------------------
	SidAddress::SidAddress( )
		: mMemberSelection(MEMBER_SELECTION_NONE)
		, mFirstIndex(0)
		, mSecondIndex(0)
		, mIsValid(false)
	{
	}

	//------------------------------
	SidAddress::SidAddress( const String& sidAddress )
		: mMemberSelection(MEMBER_SELECTION_NONE)
		, mFirstIndex(0)
		, mSecondIndex(0)
		, mIsValid(false)
	{
		parseAddress( sidAddress );
	}

	//------------------------------
	SidAddress::SidAddress( const COLLADABU::URI& id, const String& sid )
		: mId(id.getFragment())
		, mMemberSelection(MEMBER_SELECTION_NONE)
		, mFirstIndex(0)
		, mSecondIndex(0)
		, mIsValid( !id.getFragment().empty() && !sid.empty() )
	{
		mSids.push_back( sid );
	}

	//------------------------------
	SidAddress::SidAddress( const COLLADABU::URI& id )
		: mId(id.getFragment())
		, mMemberSelection(MEMBER_SELECTION_NONE)
		, mFirstIndex(0)
		, mSecondIndex(0)
		, mIsValid( !id.getFragment().empty() )
	{
	}

	//------------------------------
	SidAddress::~SidAddress()
	{
	}

	//------------------------------
	void SidAddress::parseAddress( const String& sidAddress )
	{
		/* An sid path looks as follows
		<id>/sid1/sid2/sid3<accessor>
		where,
		<id> is either a valid id or a "." for relative paths
		<accessor> is either "." followed by a name eg "ANGLE"
		or a one dimensional array access of the form (<number>)
		or a two dimensional array access of the form (<number>)(<number>)
		or empty
		The number of sids is arbitrary an can be zero

		*/

		// Find the last sid separator "/"
		// we use this to split the address in to parts "<id>/sid1/sid2/"  and  "sid3<accessor>"
		size_t lastSidSeparator = sidAddress.find_last_of(sidSeparator);

		bool hasId = false;
		if ( lastSidSeparator != String::npos )
		{
			size_t nextTokenIndex = 0; 
			size_t startPos = 0;
			while (nextTokenIndex != lastSidSeparator)
			{
				nextTokenIndex = sidAddress.find_first_of(sidSeparator, startPos);
				if ( hasId )
				{
					mSids.push_back(String(sidAddress, startPos, nextTokenIndex- startPos));
				}
				else
				{
					if ( sidAddress[startPos] != '.' )
						mId.assign(sidAddress, startPos, nextTokenIndex- startPos);
					hasId = true;
				}
				startPos = nextTokenIndex + 1;
			}
		}

		mIsValid = true;

		// Original code was using regular expression: "(.+)\.(.+)"
		size_t lastSidStart = lastSidSeparator + 1;
		size_t dotPos = sidAddress.find_last_of('.', lastSidStart);
		if (dotPos != String::npos && dotPos > lastSidStart + 1 && dotPos + 1 < sidAddress.length())
		{
			// the name accessor is present
			String idOrSid = sidAddress.substr(lastSidStart, dotPos - lastSidStart);
			if (hasId)
			{
				mSids.push_back(idOrSid);
			}
			else
			{
				if (sidAddress[lastSidStart] != '.')
					mId = idOrSid;
				hasId = true;
			}

			mMemberSelectionName = sidAddress.substr(dotPos + 1);
			mMemberSelection = MEMBER_SELECTION_NAME;
		}
		else
		{
			// Original code was using a regular expression: "([^(]+)(?:\(([0-9]+)\))?(?:\(([0-9]+)\))?"
			size_t openParenPos = sidAddress.find_first_of('(', lastSidStart);
			size_t closeParenPos = sidAddress.find_first_of(')', lastSidStart);
			size_t idOrSidEnd = sidAddress.size();
			mMemberSelection = MEMBER_SELECTION_NONE;
			if (openParenPos != String::npos && closeParenPos != String::npos && openParenPos + 1 < closeParenPos)
			{
				// Have "(number)"
				idOrSidEnd = openParenPos;
				mMemberSelection = MEMBER_SELECTION_ONE_INDEX;
				bool failed = false;
				const char* bufferBegin = sidAddress.c_str() + openParenPos + 1;
				mFirstIndex = (size_t)GeneratedSaxParser::Utils::toUint32(&bufferBegin, sidAddress.c_str() + closeParenPos, failed);
				if (failed)
				{
					mIsValid = false;
					return;
				}

				// Check if we have another "(number)"
				size_t openParen2Pos = sidAddress.find_first_of('(', closeParenPos + 1);
				size_t closeParen2Pos = sidAddress.find_first_of(')', closeParenPos + 1);
				if (openParen2Pos != String::npos && closeParen2Pos != String::npos && openParen2Pos + 1 < closeParen2Pos)
				{
					mMemberSelection = MEMBER_SELECTION_TWO_INDICES;
					failed = false;
					bufferBegin = sidAddress.c_str() + openParen2Pos + 1;
					mSecondIndex = (size_t)GeneratedSaxParser::Utils::toUint32(&bufferBegin, sidAddress.c_str() + closeParen2Pos, failed);
					if (failed)
					{
						mIsValid = false;
						return;
					}
				}
			}

			String idOrSid = sidAddress.substr(lastSidStart, idOrSidEnd - lastSidStart);
			if (idOrSid.empty())
			{
				mIsValid = false;
				return;
			}
			if (hasId)
			{
				mSids.push_back(idOrSid);
			}
			else
			{
				if (sidAddress[lastSidStart] != '.')
					mId = idOrSid;
				hasId = true;
			}
		}
	}

	//------------------------------
	String SidAddress::getSidAddressString() const
	{
		if ( !mIsValid )
			return String();

		std::ostringstream stream;
		stream << mId;
		SidList::const_iterator it = mSids.begin();
		for ( ; it != mSids.end(); ++it )
		{
			stream << "/" << *it;
		}
		switch ( mMemberSelection )
		{
		case MEMBER_SELECTION_NAME:
			stream << "." << mMemberSelectionName;
			break;
		case MEMBER_SELECTION_ONE_INDEX:
			stream << "(" << mFirstIndex << ")";
			break;
		case MEMBER_SELECTION_TWO_INDICES:
			stream << "(" << mFirstIndex << ")";
			stream << "(" << mSecondIndex<< ")";
			break;
		default:
			break;
		}

		return stream.str();
	}
} // namespace COLLADASaxFWL
