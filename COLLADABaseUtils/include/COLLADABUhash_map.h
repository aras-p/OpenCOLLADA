/*
     Copyright (c) 2008-2009 NetAllied Systems GmbH
 
     This file is part of COLLADABaseUtils.
 
     Licensed under the MIT Open Source License, 
     for details please see LICENSE file or the website
     http://www.opensource.org/licenses/mit-license.php
 */


#ifndef __COLLADABU_HASH_MAP_H__
#define __COLLADABU_HASH_MAP_H__

#include "COLLADABUPrerequisites.h"
#include "COLLADABUPlatform.h"
// file to include the hash map platform independently

#include <unordered_map>
#include <unordered_set>

#define COLLADABU_HASH_MAP std::unordered_map
#define COLLADABU_HASH_MULTIMAP std::unordered_multimap
#define COLLADABU_HASH_SET std::unordered_set
#define COLLADABU_HASH_NAMESPACE_OPEN std
#define COLLADABU_HASH_NAMESPACE_CLOSE
#define COLLADABU_HASH_FUN hash

namespace COLLADABU
{
    // typedef COLLADABU_HASH_MAP<X, Y> hash_map<X, Y>;   // Unfortunately, this is not possible...
	template<class X, class Y>
	class hash_map : public COLLADABU_HASH_MAP<X,Y>
	{
	public:

		hash_map() {}
		hash_map(const hash_map& a) : COLLADABU_HASH_MAP<X,Y>(a) {}
		hash_map& operator = (const hash_map& a) { COLLADABU_HASH_MAP<X,Y>::operator = (*this, a); return this; }
	};

	template<class X, class Y>
	class hash_multimap : public COLLADABU_HASH_MULTIMAP<X,Y>
	{
	public:

		hash_multimap() {}
		hash_multimap(const hash_multimap& a) : COLLADABU_HASH_MULTIMAP<X,Y>(a) {}
		hash_multimap& operator = (const hash_multimap& a) { COLLADABU_HASH_MULTIMAP<X,Y>::operator = (*this, a); return this; }
	};

    template<class X>
    class hash_set : public COLLADABU_HASH_SET<X>
    {
        public:

        hash_set() {}
        hash_set(const hash_set& a) : COLLADABU_HASH_SET<X>(a) {}
        hash_set& operator = (const hash_set& a) { COLLADABU_HASH_SET<X>::operator = (*this, a); return this; }
    };
}

#endif // __COLLADABU_HASH_MAP_H__
