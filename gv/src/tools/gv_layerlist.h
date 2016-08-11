/****************************************************************************
** $Id: gv_layerlist.h 6340 2007-09-12 18:24:22Z andrew $
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


#ifndef GV_LAYERLIST_H
#define GV_LAYERLIST_H


#include "gv_layer.h"
#include "gv_entity.h"
#include "gv_ptrlist.h"
#include "gv_settings.h"

/**
 * A list of layers.
 *
 * @author Andrew Mustun
 */
class GV_LayerList : public QObject {
    Q_OBJECT
public:
    GV_LayerList();
    virtual ~GV_LayerList() {
        RB_DEBUG->print("GV_LayerList::~GV_LayerList");
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
    GV_Layer* at(int i) {
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
    void activate(const QString& name) {
        activate(find(name));
    }
    void activate(GV_Layer* layer);
    //! @return The active layer of NULL if no layer is activated.
    GV_Layer* getActive() {
        return activeLayer;
    }
    virtual void add(GV_Layer* layer);
    virtual void remove(GV_Layer* layer);
    virtual void edit(GV_Layer* layer, const GV_Layer& source);
    GV_Layer* find(const QString& name);
    int getIndex(const QString& name);
    int getIndex(GV_Layer* layer);
    void toggle(const QString& name) {
        toggle(find(name));
    }
    void toggle(GV_Layer* layer);
    void toggleLock(const QString& name) {
        toggleLock(find(name));
    }
    void toggleLock(GV_Layer* layer);
    void freeze(const QString& name, bool fr) {
        freeze(find(name), fr);
    }
    void freeze(GV_Layer* layer, bool freeze);
    void freezeAll(bool freeze);
    void freezeAllPrefixedExcept(GV_Layer* layer);

    static bool testPrefix(const QString& name, 
        const QString& prefix, const QString& notPrefix = QString()) {
        
        return name.startsWith(prefix) && (notPrefix.isEmpty() || !name.startsWith(notPrefix));
    }
    
    static bool hasPrefix(const QString& name) {
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
        GV_SETTINGS->beginGroup("/LayerList");
        GV_SETTINGS->writeEntry("/DefaultToOneLayer", (int)on);
        GV_SETTINGS->endGroup();
    }

    bool getDefaultToOneLayer() {
        GV_SETTINGS->beginGroup("/LayerList");
        bool ret = (bool)GV_SETTINGS->readNumEntry("/DefaultToOneLayer", 0);
        GV_SETTINGS->endGroup();
        return ret;
    }
    
    QString getDefaultToOneLayerPrefix() {
        GV_SETTINGS->beginGroup("/LayerList");
        QString ret = GV_SETTINGS->readEntry("/DefaultToOneLayerPrefix", "__floor__");
        GV_SETTINGS->endGroup();
        return ret;
    }

    QString generateNewLayerName(const QString& prefix);

signals:
    void signalUpdate();
    void signalLayerActivated(GV_Layer*);
    void signalLayerAdded(GV_Layer*);
    void signalLayerRemoved(GV_Layer*);
    void signalLayerEdited(GV_Layer*);
    void signalLayerToggled(GV_Layer*);

private:
    //! layers in the graphic
    GV_PtrList<GV_Layer> layers;
    //! Currently active layer
    GV_Layer* activeLayer;
    /** Flag set if the layer list was modified and not yet saved. */
    bool modified;
};

#endif
