#ifndef REXPORTER_H
#define REXPORTER_H

#include <QColor>
#include <QPen>
#include <QBrush>
#include <QStack>

#include "RMatrix.h"
#include "REntity.h"
#include "RView.h"

class RVector;
class RDocument;
class RTriangle;
class RLine;
class RPoint;
class RCircle;
class RArc;
class RLinetype;

/**
 * Abstract base class for exporters.
 *
 * Exporters export documents into files, views, etc.
 *
 * \ingroup core
 * \scriptable
 */
class RExporter {

public:
    RExporter(RDocument& document);
    virtual ~RExporter();

    void setLayerSource(RDocument* ls);
    void setBlockSource(RDocument* bs);

    RDocument& getDocument();
    void setProjectionRenderingHint(RS::ProjectionRenderingHint p);
    RS::ProjectionRenderingHint getProjectionRenderingHint();
    virtual void setPen(const QPen& pen);
    virtual QPen getPen();
    virtual void setBrush(const QBrush& brush);
    virtual QBrush getBrush();

    virtual void setEntityAttributes();

    virtual void setStyle(Qt::PenStyle penStyle);
    virtual void setDashPattern(const QVector<qreal>& dashes);

    virtual void setColor(float r, float g, float b, float a = 1.0f);
    virtual void setColor(const RColor& color);
    /*
    virtual void setEntityColor(float r, float g, float b, float a = 1.0f);
    virtual void setEntityColor(const RColor& color);
    virtual void setFixedColor(float r, float g, float b, float a = 1.0f);
    virtual void setFixedColor(const QColor& clr);
    virtual void unsetFixedColor();
    virtual bool isColorFixed();
    */

    virtual void setLineweight(RLineweight::Lineweight weight);

    virtual void setLinetypeId(RLinetype::Id ltId);
    virtual void setLinetypePattern(const RLinetypePattern& ltPattern);
    RLinetypePattern getLinetypePattern();

    virtual REntity* getBlockRefOrEntity();
    virtual REntity* getEntity();
    virtual const REntity* getEntity() const;

    virtual bool isEntitySelected();

    virtual void startExport();
    virtual void endExport();
    virtual void exportDocument();
    virtual void exportLayers();
    virtual void exportBlocks();
    virtual void exportViews();
    virtual void exportEntities();
    virtual void exportEntities(const RBox& box);
    virtual void exportLinetypes();
    virtual void exportLayer(RLayer& /*layer*/) {}
    virtual void exportLayer(RLayer::Id layerId);
    virtual void exportBlock(RBlock& /*block*/) {}
    virtual void exportBlock(RBlock::Id blockId);
    virtual void exportView(RView& /*view*/) {}
    virtual void exportView(RView::Id viewId);
    virtual void exportEntity(REntity& e);
    virtual void exportEntity(REntity::Id entityId);
    virtual void exportLinetype(RLinetype& /*linetype*/) {}
    virtual void startEntity(bool /*topLevelEntity*/) {}
    virtual void endEntity() {}
    virtual void exportCurrentEntity();
    virtual void unexportEntity(REntity::Id entityId);
    virtual void exportEntities(QSet<REntity::Id>& entityIds);

    /**
     * Exports a line with the current attributes.
     */
    virtual void exportLine(const RLine& line, double offset = RNANDOUBLE);

    virtual void exportPoint(const RPoint& point) = 0;

    virtual void exportCircle(const RCircle& circle) = 0;

    virtual void exportArc(const RArc& arc, double offset = RNANDOUBLE);

	virtual void exportArcSegment(const RArc& arc) = 0;

    /**
     * Exports a line segment (a line without pattern). This is called 
     * from the standard implementation of exportLine for every dash in a 
     * dashed line.
     */
    virtual void exportLineSegment(const RLine& line) = 0;

    virtual void exportQuad(
        const RVector& p1,
        const RVector& p2,
        const RVector& p3,
        const RVector& p4
    );

    virtual void exportVerticalQuad(
        const RVector& p1,
        const RVector& p2,
        double height
    );

    virtual void exportBox(const RBox& box);

    /**
     * Exports a triangle with the current attributes.
     */
    virtual void exportTriangle(const RTriangle& triangle) = 0;

    virtual void exportRectangle(const RVector& p1, const RVector& p2);

    virtual double getPatternFactor();

    QStack<REntity*> getEntityStack();

    /**
     * \nonscriptable
     */
    double getPatternOffset(double length, const RLinetypePattern& pattern,
			int index, double* gap = NULL);
	double getPatternOffset(double length, const RLinetypePattern& pattern);


protected:
    RDocument& document;
    QPen currentPen;
    RLinetypePattern currentLinetypePattern;
    QBrush currentBrush;
    QStack<REntity*> entityStack;
    RLayer* currentLayer;
    RBlockReferenceEntity* currentBlockRef;
    //QSharedPointer<REntity> blockRefBS;
    RDocument* layerSource;
    RDocument* blockSource;

private:
    RS::ProjectionRenderingHint projectionRenderingHint;
    //bool fixedColor;
};

Q_DECLARE_METATYPE(RExporter*)

#endif
