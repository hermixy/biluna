/****************************************************************************
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


#ifndef RS_FILTERDXF_H
#define RS_FILTERDXF_H

#include "rs_filterinterface.h"

#include "rs_block.h"
#include "rs_color.h"
#include "rs_hash.h"
#include "rs_dimension.h"
#include "rs_hatch.h"
#include "rs_insert.h"
#include "rs_layer.h"
#include "rs_layer.h"
#include "rs_leader.h"
#include "rs_polyline.h"
#include "rs_spline.h"
#include "rs_solid.h"
#include "rs_text.h"
#include "rs_image.h"

#include "dl_creationinterface.h"
#include "dl_dxf.h"


class DL_WriterA;

/**
 * This format filter class can import and export DXF files.
 * It depends on the dxflib library.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_FilterDxf : public RS_FilterInterface, public DL_CreationInterface {
public:
    RS_FilterDxf();
    ~RS_FilterDxf();

    virtual RS_String getDescription(RS2::FormatType t) {
        switch (t) {
        case RS2::FormatDxf:
            return "DXF 2000";
        case RS2::FormatDxf12:
            return "DXF R12";
        default:
            return "";
        }
    }
    
    virtual RS_String getDefaultExtension(RS2::FormatType t) {
        switch (t) {
        case RS2::FormatDxf:
            return "dxf";
        case RS2::FormatDxf12:
            return "dxf";
        default:
            return "";
        }
    }

    virtual RS_String getFilterString(RS2::FormatType t, bool openMode=true) {
        switch (t) {
        case RS2::FormatDxf:
            if (openMode) {
                return QObject::tr("Drawing Exchange %1").arg("(*.dxf *.DXF)");
            }
            else {
                return QObject::tr("Drawing Exchange DXF 2000 %1").arg("(*.dxf *.DXF)");
            }
        case RS2::FormatDxf12:
            return QObject::tr("Drawing Exchange DXF R12 %1").arg("(*.dxf *.DXF)");
        default:
            return "";
        }
    }
    
    // Import:
    virtual bool fileImport(RS_Graphic& g, const RS_String& file, RS2::FormatType /*formatType*/);

    // Methods from DL_CreationInterface:
    virtual void addLayer(const DL_LayerData& data);
    virtual void addBlock(const DL_BlockData& data);
    virtual void endBlock();
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addEllipse(const DL_EllipseData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void addSpline(const DL_SplineData& data);
    virtual void addKnot(const DL_KnotData&) {}
    virtual void addControlPoint(const DL_ControlPointData& data);
    virtual void addInsert(const DL_InsertData& data);
    virtual void addTrace(const DL_TraceData& ) {}
    virtual void addSolid(const DL_SolidData& data);
    virtual void add3dFace(const DL_3dFaceData& ) {}
    virtual void addMTextChunk(const char* text);
    virtual void addMText(const DL_MTextData& data);
    virtual void addText(const DL_TextData& data);
    RS_DimensionData convDimensionData(const DL_DimensionData& data);
    virtual void addDimAlign(const DL_DimensionData& data,
                             const DL_DimAlignedData& edata);
    virtual void addDimLinear(const DL_DimensionData& data,
                              const DL_DimLinearData& edata);
    virtual void addDimRadial(const DL_DimensionData& data,
                              const DL_DimRadialData& edata);
    virtual void addDimDiametric(const DL_DimensionData& data,
                                 const DL_DimDiametricData& edata);
    virtual void addDimAngular(const DL_DimensionData& data,
                               const DL_DimAngularData& edata);
    virtual void addDimAngular3P(const DL_DimensionData& data,
                                 const DL_DimAngular3PData& edata);
    virtual void addDimOrdinate(const DL_DimensionData& data,
                             const DL_DimOrdinateData& edata);
    virtual void addLeader(const DL_LeaderData& data);
    virtual void addLeaderVertex(const DL_LeaderVertexData& data);
    virtual void addHatch(const DL_HatchData& data);
    virtual void addHatchLoop(const DL_HatchLoopData& data);
    virtual void addHatchEdge(const DL_HatchEdgeData& data);
    virtual void addImage(const DL_ImageData& data);
    virtual void linkImage(const DL_ImageDefData& data);
    virtual void endEntity();
    virtual void endSequence() {}

    virtual void addComment(const char* comment);
    
    virtual void setVariableVector(const char* key,
                                   double v1, double v2, double v3, int code);
    virtual void setVariableString(const char* key, const char* value, int code);
    virtual void setVariableInt(const char* key, int value, int code);
    virtual void setVariableDouble(const char* key, double value, int code);

    // Export:
    virtual bool fileExport(RS_Graphic& g, const RS_String& file, RS2::FormatType formatType);

    void writeComment(const char* comment);
    virtual void writeVariables();
    virtual void writeLayer(RS_Layer* l);
    void writeLineType(RS2::LineType t);
    void writeAppid(const char* appid);
    void writeBlock(RS_Block* blk);
    virtual void writeEntity(RS_Entity* e);
    void writePoint(RS_Point* p);
    void writeLine(RS_Line* l);
    void writePolyline(RS_Polyline* l);
    void writeSpline(RS_Spline* s);
    void writeCircle(RS_Circle* c);
    void writeArc(RS_Arc* a);
    void writeEllipse(RS_Ellipse* s);
    void writeInsert(RS_Insert* i);
    void writeText(RS_Text* t);
    void writeDimension(RS_Dimension* d);
    void writeLeader(RS_Leader* l);
    void writeHatch(RS_Hatch* h);
    void writeHatchLoop(RS_EntityContainer* loop);
    void writeSolid(RS_Solid* s);
    void write3dFace(RS_3dFace* f);
    void writeImage(RS_Image* i);
    void writeEntityContainer(RS_EntityContainer* con);
    void writeAtomicEntities(RS_EntityContainer* c, RS2::ResolveLevel level);
    
    void writeImageDef(RS_Image* i);

    void setEntityAttributes(RS_Entity* entity);
    DL_Attributes getEntityAttributes(RS_Entity* entity);

    static RS_String toDxfString(const RS_String& string, RS2::FormatType formatType=RS2::FormatDxf);
    static RS_String toNativeString(const RS_String& string);

public:
    RS_Pen attributesToPen(bool forLayer=false) const;

    static RS_Color numberToColor(int num, bool comp=false, bool forLayer=false);
    static RS_Color numberToColor24(int num);
    static int colorToNumber(const RS_Color& col);
    static int colorToNumber24(const RS_Color& col);

    static RS2::LineType nameToLineType(const RS_String& name);
    static RS_String lineTypeToName(RS2::LineType lineType);

    static RS2::LineWidth numberToWidth(int num);
    static int widthToNumber(RS2::LineWidth width);

    static RS2::AngleFormat numberToAngleFormat(int num);
    static int angleFormatToNumber(RS2::AngleFormat af);

    static RS2::Unit numberToUnit(int num);
    static int unitToNumber(RS2::Unit unit);
    
    static bool isVariableTwoDimensional(const RS_String& var);

protected:
    /** Format type */
    RS2::FormatType formatType;
    /** Pointer to the graphic we currently operate on. */
    RS_Graphic* graphic;
    /** File name. Used to find out the full path of images. */
    RS_String file;
    /** string for concatinating text parts of MTEXT entities. */
    RS_String mtext;
    /** Pointer to current polyline entity we're adding vertices to. */
    RS_Polyline* polyline;
    /** Pointer to current spline entity we're adding control points to. */
    RS_Spline* spline;
    /** Pointer to current leader entity we're adding vertices to. */
    RS_Leader* leader;
    /** Pointer to current entity container (either block or graphic) */
    RS_EntityContainer* currentContainer;

    /** Pointer to current hatch or NULL. */
    RS_Hatch* hatch;
    /** Pointer to current hatch loop or NULL. */
    RS_EntityContainer* hatchLoop;

    DL_Dxf dxf;
    DL_WriterA* dw;
    RS_VariableDict variables;
    bool exportAllLayersVisible;

    RS_String currentLayer;
}
;

#endif
