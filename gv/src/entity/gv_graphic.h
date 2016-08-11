/****************************************************************************
** $Id: gv_graphic.h 8803 2008-02-01 18:18:52Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_GRAPHIC_H
#define GV_GRAPHIC_H

#include "gv_blocklist.h"
#include "gv_layerlist.h"
#include "gv_variabledict.h"
#include "gv_variable.h"
#include "gv_document.h"
#include "gv_units.h"
#ifdef GV_CAM
#include "gv_camdata.h"
#endif



/**
 * A graphic document which can contain entities layers and blocks.
 *
 * @author Andrew Mustun
 */
class GV_Graphic : public GV_Document {
public:
    GV_Graphic(GV_EntityContainer* parentContainer=NULL);
    virtual ~GV_Graphic();

    //virtual GV_Entity* clone() {
    //    return new GV_Graphic(*this);
    //}

    /** @return GV2::EntityGraphic */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityGraphic;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityGraphic || GV_Document::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Graphic");
    }

    virtual int countLayerEntities(GV_Layer* layer);

    virtual GV_LayerList* getLayerList() {
        return &layerList;
    }
    virtual GV_BlockList* getBlockList() {
        return &blockList;
    }

    virtual void newDoc();
    virtual bool save();
    virtual bool saveAs(const QString& filename, GV2::FormatType type);
    virtual bool open(const QString& filename, GV2::FormatType type, bool clean=true);
    
    // Wrappers for Layer functions:
    void clearLayers() {
        layerList.clear();
    }
    int countLayers() const {
        return layerList.count();
    }
    GV_Layer* layerAt(int i) {
        return layerList.at(i);
    }
    void activateLayer(const QString& name) {
        layerList.activate(name);
    }
    void activateLayer(GV_Layer* layer) {
        layerList.activate(layer);
    }
    GV_Layer* getActiveLayer() {
        return layerList.getActive();
    }
    virtual void addLayer(GV_Layer* layer) {
        layerList.add(layer);
    }
    virtual void removeLayer(GV_Layer* layer);
    virtual void editLayer(GV_Layer* layer, const GV_Layer& source) {
        layerList.edit(layer, source);
    }
    GV_Layer* findLayer(const QString& name) {
        return layerList.find(name);
    }
    void toggleLayer(const QString& name) {
        layerList.toggle(name);
    }
    void toggleLayer(GV_Layer* layer) {
        layerList.toggle(layer);
    }
    void toggleLayerLock(GV_Layer* layer) {
        layerList.toggleLock(layer);
    }
    void freezeAllLayers(bool freeze) {
        layerList.freezeAll(freeze);
    }

    /*
    void addLayerListListener(GV_LayerListListener* listener) {
        layerList.addListener(listener);
    }
    void addLayerListListeners(GV_List<GV_LayerListListener*>& listeners) {
        layerList.addListeners(listeners);
    }
    void removeLayerListListener(GV_LayerListListener* listener) {
        layerList.removeListener(listener);
    }
    */


    // Wrapper for block functions:
    void clearBlocks() {
        blockList.clear();
    }
    int countBlocks() {
        return blockList.count();
    }
    GV_Block* blockAt(int i) {
        return blockList.at(i);
    }
    void activateBlock(const QString& name) {
        blockList.activate(name);
    }
    void activateBlock(GV_Block* block) {
        blockList.activate(block);
    }
    GV_Block* getActiveBlock() {
        return blockList.getActive();
    }
    virtual bool addBlock(GV_Block* block, bool notify=true) {
        return blockList.add(block, notify);
    }
    virtual void addBlockNotification() {
        blockList.addNotification(NULL);
    }
    virtual void removeBlock(GV_Block* block) {
        blockList.remove(block);
    }
    GV_Block* findBlock(const QString& name) {
        return blockList.find(name);
    }
    QString newBlockName() {
        return blockList.newName();
    }
    void toggleBlock(const QString& name) {
        blockList.toggle(name);
    }
    void toggleBlock(GV_Block* block) {
        blockList.toggle(block);
    }
    void freezeAllBlocks(bool freeze) {
        blockList.freezeAll(freeze);
    }
    void addBlockListListener(GV_BlockListListener* listener) {
        blockList.addListener(listener);
    }
    void removeBlockListListener(GV_BlockListListener* listener) {
        blockList.removeListener(listener);
    }

    // Wrappers for variable functions:
    void clearVariables() {
        variableDict.clear();
    }
    int countVariables() {
        return variableDict.count();
    }

    void addVariable(const QString& key, const GV_Vector& value, int code) {
        variableDict.add(key, value, code);
    }
    void addVariable(const QString& key, const QString& value, int code) {
        variableDict.add(key, value, code);
    }
    void addVariable(const QString& key, int value, int code) {
        variableDict.add(key, value, code);
    }
    void addVariable(const QString& key, double value, int code) {
        variableDict.add(key, value, code);
    }

    GV_Vector getVariableVector(const QString& key, const GV_Vector& def) {
        return variableDict.getVector(key, def);
    }
    QString getVariableString(const QString& key, const QString& def) {
        return variableDict.getString(key, def);
    }
    int getVariableInt(const QString& key, int def) {
        return variableDict.getInt(key, def);
    }
    double getVariableDouble(const QString& key, double def) {
        return variableDict.getDouble(key, def);
    }

    void removeVariable(const QString& key) {
        variableDict.remove(key);
    }

    GV_Hash<QString, GV_Variable>& getVariableDict() {
        return variableDict.getVariableDict();
    }

    GV2::LinearFormat getLinearFormat();
    int getLinearPrecision();
    bool showLeadingZeroes();
    bool showTrailingZeroes();
    bool showLeadingZeroesAngle();
    bool showTrailingZeroesAngle();
    GV2::AngleFormat getAngleFormat();
    int getAnglePrecision();

    GV_Vector getPaperSize();
    void setPaperSize(const GV_Vector& s);
    
    GV_Vector getPaperInsertionBase();
    void setPaperInsertionBase(const GV_Vector& p);
    
    GV2::PaperFormat getPaperFormat(bool* landscape);
    void setPaperFormat(GV2::PaperFormat f, bool landscape);

    double getPaperScale();
    void setPaperScale(double s);

    virtual void setUnit(GV2::Unit u);
    virtual GV2::Unit getUnit();

    bool isGridOn();
    void setGridOn(bool on);
    //void toggleGrid();
    
    bool isDraftOn();
    void setDraftOn(bool on);

    /** Sets the unit of this graphic's dimensions to 'u' */
    /*virtual void setDimensionUnit(GV2::Unit u) {
        addVariable("$INSUNITS", (int)u, 70);
        dimensionUnit = u;
    }*/

    /** Gets the unit of this graphic's dimension */
    /*virtual GV2::Unit getDimensionUnit() {
        return dimensionUnit;
    }*/

    void centerToPage();
    void fitToPage();
    
    /**
     * @retval true The document has been modified since it was last saved.
     * @retval false The document has not been modified since it was last saved.
     */
    virtual bool isModified() const;
    
    /**
     * Sets the documents modified status to 'm'.
     */
    virtual void setModified(bool m);

#ifdef GV_CAM
    GV_CamData& getCamData() {
        return camData;
    }
    void setCamData(const GV_CamData& d) {
        camData = d;
    }
#endif

private:
    GV_LayerList layerList;
    GV_BlockList blockList;
    GV_VariableDict variableDict;
#ifdef GV_CAM
    GV_CamData camData;
#endif
};


#endif
