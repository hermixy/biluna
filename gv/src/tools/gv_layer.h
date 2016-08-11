/****************************************************************************
** $Id: gv_layer.h 6589 2007-10-02 20:54:09Z andrew $
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


#ifndef GV_LAYER_H
#define GV_LAYER_H

#include <stddef.h>

#ifdef __hpux
#include <sys/_size_t.h>
#endif

#include "gv_flags.h"
#include "gv_pen.h"
#include "gv_propertyowner.h"
#include <QString>



/**
 * Holds the data that defines a layer.
 */
class GV_LayerData {
public:
    GV_LayerData() {}

    GV_LayerData(const QString& name,
                 const GV_Pen& pen,
                 bool frozen,
                 bool locked) {
        this->name = name;
        this->pen = pen;
        this->frozen = frozen;
        this->locked = locked;
    }

    //! Layer name
    QString name;

    //! default pen for this layer
    GV_Pen pen;

    //! Frozen flag
    bool frozen;
    
    //! Locked flag
    bool locked;

    //! Converted flag (cam)
    bool converted;
};



/**
 * Class for representing a layer
 *
 * @author Andrew Mustun
 */
class GV_Layer : public GV_PropertyOwner {
public:
    explicit GV_Layer(const QString& name);
    explicit GV_Layer(const GV_LayerData& data);
    
    GV_Layer* clone() {
        return new GV_Layer(*this);
    }

    /** sets a new name for this layer. */
    void setName(const QString& name) {
        data.name = name;
    }

    /** @return the name of this layer. */
    QString getName() const {
        return data.name;
    }

    /** sets the default pen for this layer. */
    void setPen(const GV_Pen& pen) {
        data.pen = pen;
    }

    /** @return default pen for this layer. */
    GV_Pen getPen() const {
        return data.pen;
    }

    /**
     * @retval true if this layer is frozen (invisible)
     * @retval false if this layer isn't frozen (visible)
     */
    bool isFrozen() const {
        return data.frozen;
    }

    /**
     * @retval true the layer has been converted already
     * @retval false the layer still needs to be converted
     */
    bool isConverted() const {
        return data.converted;
    }

    /**
     * Sets the converted flag
     */
    void setConverted(bool c) {
        data.converted = c;
    }

    /**
     * Toggles the visibility of this layer.
     * Freezes the layer if it's not frozen, thaws the layer otherwise
     */
    void toggle() {
        //toggleFlag(GV2::FlagFrozen);
        data.frozen = !data.frozen;
    }

    /**
     * (De-)freezes this layer.
     *
     * @param freeze true: freeze, false: defreeze
     */
    void freeze(bool freeze) {
        data.frozen = freeze;
    }

    /**
     * Toggles the lock of this layer.
     */
    void toggleLock() {
        data.locked = !data.locked;
    }
    
    /**
     * Locks/Unlocks this layer.
     *
     * @param l true: lock, false: unlock
     */
    void lock(bool l) {
        data.locked = l;
    }

    /**
     * return the LOCK state of the Layer
     */
    bool isLocked() {
        return data.locked;
    }

private:
    //! Layer data
    GV_LayerData data;

};

#endif
