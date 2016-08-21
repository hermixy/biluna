/****************************************************************************
** $Id: rs_layerlist.h 6340 2007-09-12 18:24:22Z andrew $
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


#ifndef RS_LAYERLIST_H
#define RS_LAYERLIST_H


#include "rs_layer.h"
#include "rs_entity.h"
#include "rs_ptrlist.h"
#include "rs_settings.h"

/**
 * A list of layers.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_LayerList : public QObject {
    Q_OBJECT
public:
    RS_LayerList();
    virtual ~RS_LayerList() {
        RS_DEBUG->print("RS_LayerList::~RS_LayerList");
        clear();
    }

    void clear();

    /**
     * @return Number of layers in the list.
     */
    int count() const {
        return layers.count();
    }

    /**
     * @return Layer at given position or NULL if i is out of range.
     */
    RS_Layer* at(int i) {
        if (i>=0 && i<layers.count()) {
            return layers[i];
        }
        else {
            return NULL;
        }
    }

    void update() {
        emit signalUpdate();
    }
    void activate(const RS_String& name) {
        activate(find(name));
    }
    void activate(RS_Layer* layer);
    //! @return The active layer of NULL if no layer is activated.
    RS_Layer* getActive() {
        return activeLayer;
    }
    virtual void add(RS_Layer* layer);
    virtual void remove(RS_Layer* layer);
    virtual void edit(RS_Layer* layer, const RS_Layer& source);
    RS_Layer* find(const RS_String& name);
    int getIndex(const RS_String& name);
    int getIndex(RS_Layer* layer);
    void toggle(const RS_String& name) {
        toggle(find(name));
    }
    void toggle(RS_Layer* layer);
    void toggleLock(const RS_String& name) {
        toggleLock(find(name));
    }
    void toggleLock(RS_Layer* layer);
    void freeze(const RS_String& name, bool fr) {
        freeze(find(name), fr);
    }
    void freeze(RS_Layer* layer, bool freeze);
    void freezeAll(bool freeze);
    void freezeAllPrefixedExcept(RS_Layer* layer);

    static bool testPrefix(const RS_String& name, 
        const RS_String& prefix, const RS_String& notPrefix = RS_String()) {
        
        return name.startsWith(prefix) && (notPrefix.isEmpty() || !name.startsWith(notPrefix));
    }
    
    static bool hasPrefix(const RS_String& name) {
        return name.startsWith("__prefix__");
    }

    /**
     * Sets the layer lists modified status to 'm'.
     */
    void setModified(bool m) {
        modified = m;
    }
    
    /**
     * @retval true The layer list has been modified.
     * @retval false The layer list has not been modified.
     */
    virtual bool isModified() const {
        return modified;
    }

    /**
     * If set to on, only the layer the user clicks on will be 
     * visible by default. Default is false.
     */
    void setDefaultToOneLayer(bool on) {
        RS_SETTINGS->beginGroup("/LayerList");
        RS_SETTINGS->writeEntry("/DefaultToOneLayer", (int)on);
        RS_SETTINGS->endGroup();
    }

    bool getDefaultToOneLayer() {
        RS_SETTINGS->beginGroup("/LayerList");
        bool ret = (bool)RS_SETTINGS->readNumEntry("/DefaultToOneLayer", 0);
        RS_SETTINGS->endGroup();
        return ret;
    }
    
    RS_String getDefaultToOneLayerPrefix() {
        RS_SETTINGS->beginGroup("/LayerList");
        RS_String ret = RS_SETTINGS->readEntry("/DefaultToOneLayerPrefix", "__floor__");
        RS_SETTINGS->endGroup();
        return ret;
    }

    RS_String generateNewLayerName(const RS_String& prefix);

signals:
    void signalUpdate();
    void signalLayerActivated(RS_Layer*);
    void signalLayerAdded(RS_Layer*);
    void signalLayerRemoved(RS_Layer*);
    void signalLayerEdited(RS_Layer*);
    void signalLayerToggled(RS_Layer*);

private:
    //! layers in the graphic
    RS_PtrList<RS_Layer> layers;
    //! Currently active layer
    RS_Layer* activeLayer;
    /** Flag set if the layer list was modified and not yet saved. */
    bool modified;
};

#endif
