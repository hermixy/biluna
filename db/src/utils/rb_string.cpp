/*****************************************************************
 * $Id: rb_string.cpp 1680 2012-06-23 15:08:20Z rutger $
 *
 * Copyright (C) 2004 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_string.h"

/**
 * Replace in string each occurance of s1 by s2, example:
 *  RB_String s1 = "abcdefg";
 *  res = RB_StringCompat::replace(s1, 'a', 'A');
 *  assert(res=="Abcdefg");
 * @param str string to be updated
 * @param s1 string to search for and to be replaced
 * @param s2 replacement string
 * @return updated string
 */
RB_String RB_StringCompat::replace(const RB_String& str,
                                   const RB_String& s1,
                                   const RB_String& s2) {

    if (s1.isEmpty()) {
        return str;
    }

    RB_String ret = "";
    
//    Below line replaced because uint gave compiler message about comparison
//    of unsigned integer and signed integer
//    for (uint i=0; i<str.length(); ++i) {
    for (int i=0; i<str.length(); ++i) {
        if (str.mid(i, s1.length())==s1) {
            ret += s2;
            i += s1.length()-1;
        } else {
            ret += str.at(i);
        }
    }

    return ret;
}

/* void RB_StringCompat::test() {
    RB_String res;
    RB_String s1 = "abcdefg";
    res = RB_StringCompat::replace(s1, 'a', 'A');
    assert(res=="Abcdefg");
    res = RB_StringCompat::replace(s1, 'b', 'B');
    assert(res=="aBcdefg");
    res = RB_StringCompat::replace(s1, 'g', 'G');
    assert(res=="abcdefG");

    res = RB_StringCompat::replace(s1, "", "blah");
    assert(res=="abcdefg");
    res = RB_StringCompat::replace(s1, "ab", "AB");
    assert(res=="ABcdefg");
    res = RB_StringCompat::replace(s1, "def", "DEF");
    assert(res=="abcDEFg");
    res = RB_StringCompat::replace(s1, "g", "G");
    assert(res=="abcdefG");
    res = RB_StringCompat::replace(s1, "fg", "FG");
    assert(res=="abcdeFG");

    s1 = "a";
    res = RB_StringCompat::replace(s1, "a", "ABC");
    assert(res=="ABC");
    s1 = "ab";
    res = RB_StringCompat::replace(s1, "ab", "");
    assert(res=="");
} */

