/****************************************************************************
** $Id: rs_ptrlist.h 4861 2007-03-07 15:29:43Z andrew $
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
** not clear to you.
**
**********************************************************************/


#ifndef RS_PTRLIST_H
#define RS_PTRLIST_H

#include <QList>
#include "rs_debug.h"

#define RS_MutableListIterator QMutableListIterator

/**
 * A QList that operates on pointers and offers autodeletion for
 * its items when it is cleared.
 * 
 */
template <typename T>
class /*CAD_EXPORT*/ RS_PtrList : public QList<T*> {
public:
    RS_PtrList() {
        autoDel = false;
        isCurrentValid = false;
        index = -1;
    }

    RS_PtrList(const RS_PtrList& other) : QList<T*>(other) {
        autoDel = false;
        isCurrentValid = false;
        index = -1;
    }
    
    virtual ~RS_PtrList() {
        clear();
        index = -1;
    }
    
    virtual bool isEmpty() {
        return QList<T*>::isEmpty();
    }
    virtual T* takeFirst() {
        return QList<T*>::takeFirst();
    }
    virtual int size() {
        return QList<T*>::size();
    }
    virtual typename RS_PtrList::iterator end() {
        return QList<T*>::end();
    }

    virtual T* operator[] (int i) {
        if (i>=0 && i<size()) {
            return QList<T*>::operator[](i);
        }
        else {
            return NULL;
        }
    }

    virtual void clear() {
        if (autoDel) {
            while (!isEmpty()) {
                delete takeFirst();
            }
        }
        isCurrentValid = false;
        QList<T*>::clear();
    }

    virtual typename RS_PtrList::iterator erase(typename RS_PtrList::iterator pos) {
        if (pos!=end()) {
            if (autoDel) {
                delete *pos;
                *pos = NULL;
            }
            return QList<T*>::erase(pos);
        }
        else {
            return end();
        }
    }

    virtual typename RS_PtrList::iterator erase(typename RS_PtrList::iterator , 
                                        typename RS_PtrList::iterator ) {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "RS_PtrList::erase(begin, end): unsupported function");
        assert(false);
        return end();
    }

    virtual void pop_back() {
        RS_DEBUG->print(RS_Debug::D_ERROR,
            "RS_PtrList::pop_back(): unsupported function");
        assert(false);
    }

    virtual void pop_front() {
        RS_DEBUG->print(RS_Debug::D_ERROR,
            "RS_PtrList::pop_front(): unsupported function");
        assert(false);
    }
    
    
    virtual int remove(T* value) {
        return removeAll(value);
    }

    virtual int removeAll(T* value) {
        if (autoDel) {
            for (int i=0; i<size(); ++i) {
                if ((*this)[i]==value) {
                    delete (*this)[i];
                }
            }
        }
        return QList<T*>::removeAll(value);
    }
    virtual void removeAt(int i) {
        if (i>=0 && i<size()) {
            if (autoDel) {
                delete (*this)[i];
            }
            QList<T*>::removeAt(i);
        }
    }
    virtual void removeFirst() {
        if (size()>0) {
            if (autoDel) {
                delete (*this)[0];
            }
            QList<T*>::removeFirst();
        }
    }
    virtual void removeLast() {
        if (size()>0) {
            if (autoDel) {
                delete (*this)[size()-1];
            }
            QList<T*>::removeLast();
        }
    }
    virtual void replace(int i, T* value) {
        T* replaced = (*this)[i];
        if (replaced!=NULL) {
            QList<T*>::replace(i, value);
            if (autoDel) {
                delete replaced;
            }
        }
        else {
            if (autoDel) {
                delete value;
            }
        }
    }

    virtual void setAutoDelete(bool on) {
        autoDel = on;
    }

    virtual bool autoDelete() {
        return autoDel;
    }
    
    virtual T* first() {
        if (!QList<T*>::isEmpty()) {
            isCurrentValid = true;
            index = 0;
            return QList<T*>::at(index);
        }
        else {
            isCurrentValid = false;
            index = -1;
            return NULL;
        }
    }

    virtual T* next() {
        if (index!=-1 && !QList<T*>::isEmpty() && index < QList<T*>::count()-1) {
            isCurrentValid = true;
            ++index;
            return QList<T*>::at(index);
        }
        else {
            isCurrentValid = false;
            index = -1;
            return NULL;
        }
    }

    virtual T* last() {
        if (!QList<T*>::isEmpty()) {
            isCurrentValid = true;
            index = QList<T*>::count()-1;
            return QList<T*>::at(index);
        }
        else {
            isCurrentValid = false;
            index = -1;
            return NULL;
        }
    }

    virtual T* prev() {
        if (index>0 && !QList<T*>::isEmpty()) {
            isCurrentValid = true;
            --index;
            return QList<T*>::at(index);
        }
        else {
            isCurrentValid = false;
            index = -1;
            return NULL;
        }
    }
    
    virtual void setCurrent(T* value) {
        index = QList<T*>::indexOf(value);
        if (index!=-1) {
            isCurrentValid = true;
        }
        else {
            isCurrentValid = false;
        }
    }
    
    virtual T* current() {
        if (isCurrentValid) {
            return QList<T*>::at(index);
        }
        else {
            return NULL;
        }
    }

    virtual int currentIndex() {
        return index;
    }

    virtual void setCurrentIndex(int i) {
        index = i;
    }

private:
    bool autoDel;
    bool isCurrentValid;
    int index;
};

#endif
