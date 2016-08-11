/****************************************************************************
** $Id: gv_layerlist.cpp 8375 2008-01-23 18:57:12Z andrew $
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


#include "gv_layerlist.h"



/**
 * Default constructor.
 */
GV_LayerList::GV_LayerList() {
    // layers are deleted manually by remove() / clear()
    layers.setAutoDelete(false);
    activeLayer = NULL;
    setModified(false);
}



/**
 * Removes all layers in the layerlist.
 */
void GV_LayerList::clear() {
    layers.clear();
    setModified(true);
}

    
/**
 * Activates the given layer.
 * 
 * @param notify Notify listeners.
 */
void GV_LayerList::activate(GV_Layer* layer) {
    RB_DEBUG->print("GV_LayerList::activate");

    activeLayer = layer;
    
    if (layer==NULL) {
        return;
    }
    
    if (getDefaultToOneLayer()) {
        if (layer->isFrozen()) {
            freezeAllPrefixedExcept(layer);
        }
        else {
            layer->lock(false);
        }
    }
    
    emit signalLayerActivated(activeLayer);

    RB_DEBUG->print("GV_LayerList::activate: OK");
}



/**
 * Adds a layer to the layer list.
 * If there is already a layer with the same name, no layer is 
 * added. In that case the layer passed to the methode will be deleted!
 * If no layer was active so far, the new layer becomes the active one.
 *
 * Listeners are notified.
 */
void GV_LayerList::add(GV_Layer* layer) {
    RB_DEBUG->print("GV_LayerList::add");

    if (layer==NULL) {
        return;
    }
    
    // check if layer already exists:
    GV_Layer* l = find(layer->getName());
    if (l==NULL) {
        layers.append(layer);

        RB_DEBUG->print("GV_LayerList::add: emit signalLayerAdded");
        emit signalLayerAdded(layer);

        setModified(true);

        // if there was no active layer so far, activate this one.
        if (activeLayer==NULL) {
            activate(layer);
        }
        
    } else {
        // if there was no active layer so far, activate this one.
        if (activeLayer==NULL) {
            activate(l);
        }

        l->setPen(layer->getPen());

        delete layer;
        layer = NULL;
    }
    
    RB_DEBUG->print("GV_LayerList::add: OK");
}



/**
 * Removes a layer from the list.
 * Listeners are notified after the layer was removed from 
 * the list but before it gets deleted.
 *
 * @todo: if active layer is removed: 
 *        activate the one at the same position, not first.
 */
void GV_LayerList::remove(GV_Layer* layer) {
    RB_DEBUG->print("GV_LayerList::removeLayer");

    if (layer==NULL) {
        return;
    }
    
    // here the layer is removed from the list but not deleted
    int layerIndex = layers.indexOf(layer);
    layers.removeAt(layerIndex);
    
    emit signalLayerRemoved(layer);
    
    setModified(true);

    // activate another layer if necessary:
    activate(layers.at(layerIndex>0 ? layerIndex-1 : 0));
    
    // now it's save to delete the layer
    delete layer;
}



/**
 * Changes a layer's attributes. The attributes of layer 'layer'
 * are copied from layer 'source'.
 * Listeners are notified.
 */
void GV_LayerList::edit(GV_Layer* layer, const GV_Layer& source) {
    if (layer==NULL) {
        return;
    }

    GV_Layer sourceCopy = source;

    GV_Layer* l = find(sourceCopy.getName());
    if (l!=NULL && l!=layer) {
        sourceCopy.setName(generateNewLayerName(QString("%1_").arg(sourceCopy.getName())));
    }

    *layer = sourceCopy;

    emit signalLayerEdited(layer);
    
    setModified(true);
}



/**
 * @return Pointer to the layer with the given name or
 * \p NULL if no such layer was found.
 */
GV_Layer* GV_LayerList::find(const QString& name) {
    GV_Layer* ret = NULL;

    for (int i=0; i<layers.size(); ++i) {
        if (layers[i]->getName()==name) {
            ret = layers[i];
        }
    }

    return ret;
}



/**
 * @return Index of the given layer in the layer list or -1 if the layer
 * was not found.
 */
int GV_LayerList::getIndex(const QString& name) {
    int ret = -1;

    for (int i=0; i<layers.size(); ++i) {
        if (layers[i]->getName()==name) {
            ret = i;
            break;
        }
    }

    return ret;
}


/**
 * @return Index of the given layer in the layer list or -1 if the layer
 * was not found.
 */
int GV_LayerList::getIndex(GV_Layer* layer) {
    int ret = -1;

    for (int i=0; i<layers.size(); ++i) {
        if (layers[i]==layer) {
            ret = i;
            break;
        }
    }

    return ret;
}


/**
 * Switches on / off the given layer. 
 * Listeners are notified.
 */
void GV_LayerList::toggle(GV_Layer* layer) {
    if (layer==NULL) {
        return;
    }
    
    RB_DEBUG->print("GV_LayerList::toggle");

    layer->toggle();

    // Notify listeners:
    emit signalLayerToggled(layer);
    
    RB_DEBUG->print("GV_LayerList::toggle: OK");
}



/**
 * Locks or unlocks the given layer. 
 * Listeners are notified.
 */
void GV_LayerList::toggleLock(GV_Layer* layer) {
    if (layer==NULL) {
        return;
    }

    layer->toggleLock();

    emit signalLayerToggled(layer);
}



void GV_LayerList::freeze(GV_Layer* layer, bool freeze) {
    if (layer==NULL) {
        return;
    }

    layer->freeze(freeze);

    emit signalLayerToggled(layer);
}



/**
 * Freezes or defreezes all layers without a prefix (example prefix: __prefix__floor__).
 *
 * @param freeze true: freeze, false: defreeze
 */
void GV_LayerList::freezeAll(bool freeze) {
    
    RB_DEBUG->print("GV_LayerList::freezeAll");

    for (int l=0; l<count(); l++) {
        if (!hasPrefix(at(l)->getName())) {
            at(l)->freeze(freeze);
        }
    }

    emit signalLayerToggled(NULL);
    
    RB_DEBUG->print("GV_LayerList::freezeAll: OK");
}



/**
 * Freezes all layers except the given one.
 */
void GV_LayerList::freezeAllPrefixedExcept(GV_Layer* layer) {

    RB_DEBUG->print("GV_LayerList::freezeAllPrefixedExcept");

    if (layer==NULL || !hasPrefix(layer->getName())) {
        return;
    }

    for (int l=0; l<count(); l++) {
        if (hasPrefix(at(l)->getName())) {
            at(l)->freeze(at(l)!=layer);
            at(l)->lock(at(l)!=layer);
        }
    }

    emit signalLayerToggled(NULL);
    
    RB_DEBUG->print("GV_LayerList::freezeAllPrefixedExcept: OK");
}



/**
 * Generates a name for a new layer. The name is not yet existent in this
 * layer list.
 *
 * @param prefix The new name will start with the given prefix.
 */   
QString GV_LayerList::generateNewLayerName(const QString& prefix) {
    bool done;
    int counter = 0;
    QString testName;
    
    do {
        done = true;

        testName = QString("%1%2").arg(prefix).arg(counter);
        if (find(testName)!=NULL) {
            done = false;
            counter++;
        }
        
    } while(!done);

    return testName;
}

