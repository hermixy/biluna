/****************************************************************************
** $Id: gv_ptrlist.h 2465 2005-07-04 23:54:49Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_EXTPTRLIST_H
#define GV_EXTPTRLIST_H

#include "gv_ptrlist.h"
#include "rb_debug.h"


/**
 * An extended pointer based list that allows subscribers
 * to listen to various events. Further, this list has exactly one 
 * or none active item.
 */
template <typename T>
class GV_ExtPtrList : public GV_PtrList<T> {
public:
    GV_ExtPtrList() {
        activeItem = NULL;
    }
    
    void activate(T* item) {
        activeItem = item;
    }

    //! @return The active item of NULL if no item is activated.
    T* getActive() {
        return activeItem;
    }
    
    virtual void clear() {
        activeItem = NULL;
        GV_PtrList<T>::clear();
    }

    virtual typename GV_ExtPtrList::iterator erase(typename GV_ExtPtrList::iterator pos) {
        if (*pos==activeItem) {
            activeItem = NULL;
        }
        return GV_PtrList<T>::erase(pos);
    }
    
    virtual int removeAll(T* value) {
        RB_DEBUG->print(RB_Debug::D_TESTING, "removeAll");
        for (int i=0; i<this->size(); ++i) {
            if ((*this)[i]==activeItem) {
                activeItem = NULL;
            }
        }
        return GV_PtrList<T>::removeAll(value);
    }

    virtual void removeAt(int i) {
        if ((*this)[i]==activeItem) {
            activeItem = NULL;
        }
        GV_PtrList<T>::removeAt(i);
    }
    virtual void removeFirst() {
        if ((*this)[0]==activeItem) {
            activeItem = NULL;
        }
        GV_PtrList<T>::removeFirst();
    }
    virtual void removeLast() {
        if ((*this)[this->size()-1]==activeItem) {
            activeItem = NULL;
        }
        GV_PtrList<T>::removeLast();
    }
    virtual void replace(int i, T* value) {
        if ((*this)[i]==activeItem) {
            activeItem = NULL;
        }
        GV_PtrList<T>::replace(i, value);
    }

private:
    //! Currently active item
    T* activeItem;
};

#endif
