/****************************************************************************
** $Id: gv_graphic.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "gv_graphic.h"

#include "rb_debug.h"
//#include "gv_fileio.h"
#include "gv_math.h"
#include "gv_units.h"
#include "gv_settings.h"


/**
 * Default constructor.
 */
GV_Graphic::GV_Graphic(GV_EntityContainer* parentContainer)
        : GV_Entity(parentContainer), GV_Document(parentContainer),
        layerList(), blockList(true)
#ifdef GV_CAM
, camData()
#endif
{

    GV_SETTINGS->beginGroup("/Defaults");
    setUnit(GV_Units::stringToUnit(GV_SETTINGS->readEntry("/Unit", "None")));
    GV_SETTINGS->endGroup();

    GV2::Unit unit = getUnit();

    if (unit==GV2::Inch) {
        addVariable("$DIMASZ", 0.1, 40);
        addVariable("$DIMEXE", 0.05, 40);
        addVariable("$DIMEXO", 0.025, 40);
        addVariable("$DIMGAP", 0.025, 40);
        addVariable("$DIMTXT", 0.1, 40);
    } else {
        addVariable("$DIMASZ",
                    GV_Units::convert(2.5, GV2::Millimeter, unit), 40);
        addVariable("$DIMEXE",
                    GV_Units::convert(1.25, GV2::Millimeter, unit), 40);
        addVariable("$DIMEXO",
                    GV_Units::convert(0.625, GV2::Millimeter, unit), 40);
        addVariable("$DIMGAP",
                    GV_Units::convert(0.625, GV2::Millimeter, unit), 40);
        addVariable("$DIMTXT",
                    GV_Units::convert(2.5, GV2::Millimeter, unit), 40);
    }
    setModified(false);
    setModifiedSinceRedraw(true);
}



/**
 * Destructor.
 */
GV_Graphic::~GV_Graphic() {
    RB_DEBUG->print("GV_Graphic::~GV_Graphic");
    RB_DEBUG->print("GV_Graphic::~GV_Graphic: OK");
}



/**
 * Counts the entities on the given layer.
 */
int GV_Graphic::countLayerEntities(GV_Layer* layer) {

    int c=0;

    if (layer!=NULL) {
        for (GV_Entity* t=firstEntity(GV2::ResolveNone);
                t!=NULL;
                t=nextEntity(GV2::ResolveNone)) {

            if (t->getLayer()!=NULL &&
                    t->getLayer()->getName()==layer->getName()) {
                c+=t->countDeep();
            }
        }
    }

    return c;
}



/**
 * Removes the given layer and undoes all entities on it.
 */
void GV_Graphic::removeLayer(GV_Layer* layer) {

    if (layer!=NULL && layer->getName()!="0") {

        // remove all entities on that layer:
        startUndoCycle();
        for (GV_Entity* e=firstEntity(GV2::ResolveNone);
                e!=NULL;
                e=nextEntity(GV2::ResolveNone)) {

            if (e->getLayer()!=NULL &&
                    e->getLayer()->getName()==layer->getName()) {

                e->setUndoState(true);
                e->setLayer("0");
                addUndoable(e);
            }
        }
        endUndoCycle();

        // remove all entities in blocks that are on that layer:
        for (int bi=0; bi<blockList.count(); bi++) {
            GV_Block* blk = blockList.at(bi);

            if (blk!=NULL) {
                for (GV_Entity* e=blk->firstEntity(GV2::ResolveNone);
                        e!=NULL;
                        e=blk->nextEntity(GV2::ResolveNone)) {

                    if (e->getLayer()!=NULL &&
                            e->getLayer()->getName()==layer->getName()) {

                        e->setUndoState(true);
                        e->setLayer("0");
                        //addUndoable(e);
                    }
                }
            }
        }

        layerList.remove(layer);
    }
}


/**
 * Clears all layers, blocks and entities of this graphic.
 * A default layer (0) is created.
 */
void GV_Graphic::newDoc() {

    RB_DEBUG->print("GV_Graphic::newDoc");

    clear();

    clearLayers();
    clearBlocks();

    addLayer(
        new GV_Layer(
            GV_LayerData(
                "0", 
                GV_Pen(GV_Color(0,0,0), GV2::Width07, GV2::SolidLine), 
                false, 
                false
            )
        )
    );
    //addLayer(new GV_Layer("ByBlock"));

    setModified(false);
    setModifiedSinceRedraw(true);
    
    RB_DEBUG->print("GV_Graphic::newDoc: OK");
}



/**
 * Saves this graphic with the current filename and settings.
 */
bool GV_Graphic::save() {
/*
    bool ret = false;
    
    RB_DEBUG->print("GV_Graphic::save");
    RB_DEBUG->print("  file: %s", (const char*)filename.toLatin1());
    RB_DEBUG->print("  format: %d", (int)formatType);

    RB_DEBUG->print("  export...");
    ret = GV_FILEIO->fileExport(*this, filename, formatType);

    if (ret) {
        setModified(false);
        setModifiedSinceRedraw(true);
        layerList.setModified(false);
        blockList.setModified(false);
    }

    RB_DEBUG->print("GV_Graphic::save ok");

    return ret;
*/
    return false;
}



/**
 * Saves this graphic with the given filename and current settings.
 */
bool GV_Graphic::saveAs(const QString &filename, GV2::FormatType type) {

    RB_DEBUG->print("GV_Graphic::saveAs");

    this->filename = filename;
    this->formatType = type;

    return save();
}



/**
 * Loads the given fils into this graphic.
 */
bool GV_Graphic::open(const QString &filename, GV2::FormatType type, bool clean) {
//    RB_DEBUG->print("GV_Graphic::open(%s)", (const char*)filename.toLatin1());

    bool ret = false;

    this->filename = filename;
    this->originalFilename = filename;

    // clean all:
    if (clean) {
        newDoc();
    }

    // import file:
//    ret = GV_FILEIO->fileImport(*this, filename, type);
        
    if (clean) {
        setModified(false);
        setModifiedSinceRedraw(true);
        layerList.setModified(false);
        blockList.setModified(false);
    }

//    RB_DEBUG->print("GV_Graphic::open(%s): OK", (const char*)filename.toLatin1());

    return ret;
}



/**
 * @return true if the grid is switched on (visible).
 */
bool GV_Graphic::isGridOn() {
    int on = getVariableInt("$GRIDMODE", 1);
    bool ret = (on!=0);
//    RB_DEBUG->print("GV_Graphic::isGridOn: id: %d, ret: %d", getId(), (int)ret);
    return ret;
}



/**
 * Enables / disables the grid.
 */
void GV_Graphic::setGridOn(bool on) {
//    RB_DEBUG->print("GV_Graphic::setGridOn: id: %d, on: %d", getId(), (bool)on);
    addVariable("$GRIDMODE", (int)on, 70);
}

/**
 * Toggles the grid.
 */
/*void GV_Graphic::toggleGrid() {
    RB_DEBUG->print("GV_Graphic::toggleGrid: id: %d", getId());
    setGridOn(!isGridOn());
}*/



/**
 * Sets the unit of this graphic to 'u'
 */
void GV_Graphic::setUnit(GV2::Unit u) {

    setPaperSize(GV_Units::convert(getPaperSize(), getUnit(), u));

    addVariable("$INSUNITS", (int)u, 70);

    //unit = u;
}



/**
 * Gets the unit of this graphic 
 */
GV2::Unit GV_Graphic::getUnit() {
    return (GV2::Unit)getVariableInt("$INSUNITS", 0);
    //return unit;
}



/**
 * @return The linear format type for this document.
 * This is determined by the variable "$LUNITS".
 */
GV2::LinearFormat GV_Graphic::getLinearFormat() {
    int lunits = getVariableInt("$LUNITS", 2);

    switch (lunits) {
    default:
    case 2:
        return GV2::Decimal;
        break;

    case 1:
        return GV2::Scientific;
        break;

    case 3:
        return GV2::Engineering;
        break;

    case 4:
        return GV2::Architectural;
        break;

    case 5:
        return GV2::Fractional;
        break;
    }

    return GV2::Decimal;
}



/**
 * @return The linear precision for this document.
 * This is determined by the variable "$LUPREC".
 */
int GV_Graphic::getLinearPrecision() {
    return getVariableInt("$LUPREC", 4);
}

    
    
bool GV_Graphic::showLeadingZeroes() {
    return !(getVariableInt("$DIMZIN", 8) & 4);
}


bool GV_Graphic::showTrailingZeroes() {
    return !(getVariableInt("$DIMZIN", 8) & 8);
}

bool GV_Graphic::showLeadingZeroesAngle() {
    return !(getVariableInt("$DIMAZIN", 8) & 4);
}


bool GV_Graphic::showTrailingZeroesAngle() {
    return !(getVariableInt("$DIMAZIN", 8) & 8);
}


/**
 * @return The angle format type for this document.
 * This is determined by the variable "$AUNITS".
 */
GV2::AngleFormat GV_Graphic::getAngleFormat() {
    int aunits = getVariableInt("$AUNITS", 0);

    switch (aunits) {
    default:
    case 0:
        return GV2::DegreesDecimal;
        break;

    case 1:
        return GV2::DegreesMinutesSeconds;
        break;

    case 2:
        return GV2::Gradians;
        break;

    case 3:
        return GV2::Radians;
        break;

    case 4:
        return GV2::Surveyors;
        break;
    }

    return GV2::DegreesDecimal;
}



/**
 * @return The linear precision for this document.
 * This is determined by the variable "$LUPREC".
 */
int GV_Graphic::getAnglePrecision() {
    return getVariableInt("$AUPREC", 2);
}



/**
 * @return The insertion point of the drawing into the paper space.
 * This is the distance from the lower left paper edge to the zero
 * point of the drawing. DXF: $PINSBASE.
 */
GV_Vector GV_Graphic::getPaperInsertionBase() {
    return getVariableVector("$PINSBASE", GV_Vector(0.0,0.0));
}


/**
 * Sets the PINSBASE variable.
 */
void GV_Graphic::setPaperInsertionBase(const GV_Vector& p) {
    addVariable("$PINSBASE", p, 10);
}


/**
 * @return Paper size in graphic units.
 */
GV_Vector GV_Graphic::getPaperSize() {
    GV_Vector def = GV_Units::convert(GV_Vector(210.0,297.0),
                                      GV2::Millimeter, getUnit());

    GV_Vector v1 = getVariableVector("$PLIMMIN", GV_Vector(0.0,0.0));
    GV_Vector v2 = getVariableVector("$PLIMMAX", def);

    return v2-v1;
}


/**
 * Sets a new paper size.
 */
void GV_Graphic::setPaperSize(const GV_Vector& s) {
    addVariable("$PLIMMIN", GV_Vector(0.0,0.0), 10);
    addVariable("$PLIMMAX", s, 10);
}



/**
 * @return Paper format.
 * This is determined by the variables "$PLIMMIN" and "$PLIMMAX".
 *
 * @param landscape will be set to true for landscape and false for portrait if not NULL.
 */
GV2::PaperFormat GV_Graphic::getPaperFormat(bool* landscape) {
    GV_Vector size = GV_Units::convert(getPaperSize(),
                                       getUnit(), GV2::Millimeter);

    if (landscape!=NULL) {
        *landscape = (size.x>size.y);
    }

    return GV_Units::paperSizeToFormat(size);
}



/**
 * Sets the paper format to the given format.
 */
void GV_Graphic::setPaperFormat(GV2::PaperFormat f, bool landscape) {
    GV_Vector size = GV_Units::paperFormatToSize(f);

    if (landscape) {
        double tmp = size.x;
        size.x = size.y;
        size.y = tmp;
    }

    if (f!=GV2::Custom) {
        setPaperSize(GV_Units::convert(size, GV2::Millimeter, getUnit()));
    }
}



/**
 * @return Paper space scaling (DXF: $PSVPSCALE).
 */
double GV_Graphic::getPaperScale() {
    double ret;

    ret = getVariableDouble("$PSVPSCALE", 1.0);
    if (ret<1.0e-6) {
        ret = 1.0;
    }

    return ret;
}



/**
 * Sets a new scale factor for the paper space.
 */
void GV_Graphic::setPaperScale(double s) {
    addVariable("$PSVPSCALE", s, 40);
}



/**
 * Centers drawing on page. Affects DXF variable $PINSBASE.
 */
void GV_Graphic::centerToPage() {
    GV_Vector size = getPaperSize();

    double scale = getPaperScale();

    GV_Vector pinsbase = (size-getSize()*scale)/2.0 - getMin()*scale;

    setPaperInsertionBase(pinsbase);
}



/**
 * Fits drawing on page. Affects DXF variable $PINSBASE.
 */
void GV_Graphic::fitToPage() {
    double border = GV_Units::convert(25.0, GV2::Millimeter, getUnit());
    GV_Vector ps = getPaperSize() - GV_Vector(border, border);
    GV_Vector s = getSize();
    double fx = GV_MAXDOUBLE;
    double fy = GV_MAXDOUBLE;
    //double factor = 1.0;

    //ps = GV_Units::convert(ps, getUnit(), GV2::Millimeter);

    if (fabs(s.x)>1.0e-6) {
        fx = ps.x / s.x;
    }
    if (fabs(s.y)>1.0e-6) {
        fy = ps.y / s.y;
    }

    setPaperScale(std::min(fx, fy));
    centerToPage();
}


/**
 * @retval true The document has been modified since it was last saved.
 * @retval false The document has not been modified since it was last saved.
 */
bool GV_Graphic::isModified() const {
    return GV_Document::isModified() ||
        layerList.isModified() || blockList.isModified();
}

/**
 * Sets the documents modified status to 'm'.
 */
void GV_Graphic::setModified(bool m) {
    GV_Document::setModified(m);
    layerList.setModified(m);
    blockList.setModified(m);
}
