#include "RExporter.h"

#include <QSet>

#include "RDocument.h"
#include "REntity.h"
#include "RDebug.h"
#include "RTriangle.h"
#include "RLine.h"
#include "RLinetype.h"
#include "RView.h"
#include "RArc.h"
#include "RBlockReferenceEntity.h"

RExporter::RExporter(RDocument& document)
    : document(document),
      currentLayer(NULL),
      currentBlockRef(NULL),
      layerSource(NULL),
      blockSource(NULL),
      projectionRenderingHint(RS::ThreeD) {

    currentPen.setColor(Qt::white);
    currentPen.setWidth(3);
    currentPen.setCapStyle(Qt::RoundCap);
}



RExporter::~RExporter() {
}

void RExporter::setLayerSource(RDocument* ls) {
    layerSource = ls;
}

void RExporter::setBlockSource(RDocument* bs) {
    blockSource = bs;
}

/**
 * \return The document that is currently being exported.
 */
RDocument& RExporter::getDocument() {
    return document;
}

/**
 * \return Block reference that is currently being exported or
 *      if no block reference is being exported, the current entity
 *      that is being exported.
 */
REntity* RExporter::getBlockRefOrEntity() {
    if (currentBlockRef==NULL) {
        return getEntity();
    }

    return currentBlockRef;
}

/**
 * \return Pointer to the entity that is currently being exported.
 */
const REntity* RExporter::getEntity() const {
    if (entityStack.size()>0) {
        return entityStack.top();
    }
    return NULL;
}

/**
 * Sets a rendering hint for the current projection used for exports.
 *
 * This is a rendering hint for the exporting entity to decide how
 * its geometry should be rendered. For example a wall with a window
 * exports itself differently when viewed from top than when viewed
 * from the front or side. Note that the rendering difference is not
 * only in the projection in a mathematical sense, but also in what
 * edges are exported, how they are visualized and what types of
 * shapes are exported (lines and arcs or triangles).
 *
 * This rendering hint may also be used to determine what grid is
 * appropriate to display for exporters that export to a graphics
 * scene.
 *
 * The mathematical projection is defined separately by \ref setProjection().
 */
void RExporter::setProjectionRenderingHint(RS::ProjectionRenderingHint p) {
    projectionRenderingHint = p;
}

/**
 * \return The current projection for exports.
 */
RS::ProjectionRenderingHint RExporter::getProjectionRenderingHint() {
    return projectionRenderingHint;
}

/**
 * Sets the current pen (currentPen).
 */
void RExporter::setPen(const QPen& pen) {
    currentPen = pen;
    currentPen.setCapStyle(Qt::RoundCap);
    currentPen.setJoinStyle(Qt::RoundJoin);
}

/**
 * \return Current pen.
 */
QPen RExporter::getPen() {
    return currentPen;
}

void RExporter::setBrush(const QBrush& brush) {
	currentBrush = brush;
}

QBrush RExporter::getBrush() {
    return currentBrush;
}

void RExporter::setEntityAttributes() {
    REntity* currentEntity = getEntity();
    if (currentEntity == NULL) {
        return;
    }

    setColor(currentEntity->getColor(true, currentLayer, currentBlockRef));
    setLineweight(currentEntity->getLineweight(true, currentLayer, currentBlockRef));
    setLinetypeId(currentEntity->getLinetypeId(true, currentLayer, currentBlockRef));

    setStyle(Qt::SolidLine);
}

void RExporter::setStyle(Qt::PenStyle penStyle){
	currentPen.setStyle(penStyle);
}

void RExporter::setDashPattern(const QVector<qreal>& dashes){
	currentPen.setDashPattern(dashes);
}

/**
 * Sets the current export color for entities. The default implementation
 * calls \ref setColor. Exporter implementations may choose to re-implement
 * this method for example to display entities in a different color when
 * they are selected.
 */
/*
void RExporter::setEntityColor(float r, float g, float b, float a) {
	QColor clr;
	clr.setRgbF(r, g, b, a);
	setEntityColor(clr);
}

void RExporter::setEntityColor(const RColor& color){
    setColor(color);
}
*/

/**
 * \todo remove or refactor
 *
 * Overrides any calls to \ref setColor or \ref setEntityColor. All color
 * changes that are made between a call to this function and a call
 * to \ref unsetOverrideColor have no effect.
 */
/*
void RExporter::setFixedColor(float r, float g, float b, float a) {
	QColor clr;
	clr.setRgbF(r, g, b, a);
	setFixedColor(clr);
}

void RExporter::setFixedColor(const QColor& clr){
    setColor(clr);
    fixedColor = true;
}
*/

/**
 * \todo remove or refactor
 *
 * Allows setting of colors after a section of exports with a fixed color.
void RExporter::unsetFixedColor() {
    fixedColor = false;
}
 */

/**
 * \return True if this exporter is currently using a fixed export color
 * (\ref setFixedColor has been called), false otherwise.
bool RExporter::isColorFixed() {
    return fixedColor;
}
 */

/**
 * \return Pointer to the entity that is currently being exported.
 */
REntity* RExporter::getEntity() {
    if (entityStack.size()>0) {
        return entityStack.top();
    }
    return NULL;
}

/**
 * Sets the current export color in RGB and Alpha.
 * All components are in the range of 0.0 <= component <= 1.0.
 * If \ref isColorFixed is true, this function does nothing.
 */
void RExporter::setColor(float r, float g, float b, float a) {
	QColor clr;
	clr.setRgbF(r, g, b, a);
	setColor(clr);
}

void RExporter::setColor(const RColor& color) {
    /*
    if (getEntity() != NULL) {
		if (color.isByLayer()) {
            RDocument* doc = getEntity()->getDocument();
            if (doc==NULL) {
                doc = &document;
            }
            QSharedPointer<RLayer> layer =
                doc->queryLayer(getEntity()->getLayerId());
			if (layer.isNull()) {
				RDebug::warning(QString("RExporter::setColor: layer is NULL"));
				return;
			}
			RColor clr = layer->getColor();
			currentPen.setColor(clr);
			return;
		}
        if (color.isByBlock()) {
            // find block reference we're in:
            for (int i=entityStack.size()-1; i>=0; i--) {
                RBlockReferenceEntity* blockRef =
                    dynamic_cast<RBlockReferenceEntity*>(entityStack.at(i));
                if (blockRef!=NULL) {
                    RColor clr = blockRef->getColor();
                    currentPen.setColor(clr);
                    return;
                }
            }
        }
    }
    */
	currentPen.setColor(color);
}

void RExporter::setLinetypeId(RLinetype::Id ltId) {
    RDocument* doc = NULL;
    if (getEntity()!=NULL) {
        doc = getEntity()->getDocument();
    }
    if (doc == NULL) {
        doc = &document;
    }

    currentLinetypePattern =
        RLinetypePatternMap::getPattern(doc->getLinetypeName(ltId));
}

void RExporter::setLinetypePattern(const RLinetypePattern& ltPattern) {
    Q_ASSERT(ltPattern.isValid());
    currentLinetypePattern = ltPattern;
}

void RExporter::setLineweight(RLineweight::Lineweight weight){
    Q_ASSERT(weight>=0);
    /*
    if (weight < 0 && getEntity()!=NULL) {
        currentPen.setWidthF(getEntity()->getLineweightInUnits());
        return;
    }
    */

    /*
    if (getEntity() != NULL) {
        if (weight==RLineweight::WeightByLayer) {
            RDocument* doc = getEntity()->getDocument();
            if (doc==NULL) {
                doc = &document;
            }
            QSharedPointer<RLayer> layer =
                doc->queryLayer(getEntity()->getLayerId());
            if (layer.isNull()) {
                RDebug::warning(QString("RExporter::setLineweight: layer is NULL"));
                return;
            }
            RLineweight::Lineweight lw = layer->getLineweight();
            //currentPen.setLin(clr);
            // TODO: unit conversion:
            currentPen.setWidthF(lw / 100.0);
            return;
        }
        if (weight==RLineweight::WeightByBlock) {
            // find block reference we're in:
            for (int i=entityStack.size()-1; i>=0; i--) {
                RBlockReferenceEntity* blockRef =
                    dynamic_cast<RBlockReferenceEntity*>(entityStack.at(i));
                if (blockRef!=NULL) {
                    RLineweight::Lineweight lw = blockRef->getLineweight();
                    // TODO: unit conversion:
                    currentPen.setWidthF(lw / 100.0);
                    return;
                }
            }
        }
    }
    */
    // TODO: unit conversion:
    currentPen.setWidthF(weight / 100.0);
}

/**
 * Called to start a new, complete export.
 * Implementors can for example clear any stored exported entities or caches.
 * The default implementation does nothing.
 */
void RExporter::startExport() {
}



/**
 * Called at the end of a completed export.
 * The default implementation does nothing.
 */
void RExporter::endExport() {
}



/**
 * Exports the visual representation of all objects (including entities) of the document.
 */
void RExporter::exportDocument() {
    startExport();
    exportLinetypes();
    exportLayers();
    exportBlocks();
    exportViews();
    exportEntities();
    endExport();
}

void RExporter::exportEntities(){
    //QSet<REntity::Id> ids = document.queryAllEntities(false, true);
    QSet<REntity::Id> ids = document.queryAllEntities(false, false);
    QSet<REntity::Id>::iterator it;
    for (it=ids.begin(); it!=ids.end(); it++) {
    	QSharedPointer<REntity> e = document.queryEntity(*it);
        if (!e.isNull()) {
            exportEntity(*e);
        }
    }
}

void RExporter::exportLayers() {
	QSet<RLayer::Id> ids = document.queryAllLayers();
	QSet<RLayer::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<RLayer> e = document.queryLayer(*it);
		if (!e.isNull()) {
			exportLayer(*e);
		}
	}
}

void RExporter::exportBlocks() {
	QSet<RBlock::Id> ids = document.queryAllBlocks();
	QSet<RBlock::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<RBlock> e = document.queryBlock(*it);
		if (!e.isNull()) {
			exportBlock(*e);
		}
	}
}

void RExporter::exportViews() {
	QSet<RView::Id> ids = document.queryAllViews();
	QSet<RView::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<RView> e = document.queryView(*it);
		if (!e.isNull()) {
			exportView(*e);
		}
	}
}

void RExporter::exportLinetypes() {
	QSet<RLinetype::Id> ids = document.queryAllLinetypes();
	QSet<RLinetype::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<RLinetype> e = document.queryLinetype(*it);
		if (!e.isNull()) {
			exportLinetype(*e);
		}
	}
}


/**
 * Exports the visual representation of all entities that are completely
 * or partly inside the given range.
 */
void RExporter::exportEntities(const RBox& box) {
    QSet<REntity::Id> ids = document.queryIntersectedEntitiesXY(box);
	QSet<REntity::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		exportEntity(*it);
	}
}


void RExporter::exportLayer(RLayer::Id layerId){
	QSharedPointer<RLayer> layer = getDocument().queryLayer(layerId);
	if (layer.isNull() || !layer->isFrozen()) {
		exportLayer(*layer);
	}
}

void RExporter::exportBlock(RBlock::Id blockId){
	QSharedPointer<RBlock> block = getDocument().queryBlock(blockId);
	if (block.isNull() || !block->isFrozen()) {
		exportBlock(*block);
	}
}

void RExporter::exportView(RView::Id viewId){
	QSharedPointer<RView> view = getDocument().queryView(viewId);
	if (view.isNull()) {
		exportView(*view);
	}
}

/**
 * Sets the current entity to the given entity and calls \ref exportEntity().
 * Note that entity is a temporary clone.
 */
void RExporter::exportEntity(REntity& entity) {
    entityStack.push(&entity);

    RDocument* doc = entity.getDocument();
    if (doc==NULL) {
        doc = &document;
    }

    // find layer of the current entity
    QSharedPointer<RLayer> layer;
    if (layerSource!=NULL) {
        RLayer::Id layerId = entity.getLayerId();
        //QString layerName = layerSource->getLayerName(layerId);
        layer = layerSource->queryLayer(layerId);
        Q_ASSERT(!layer.isNull());
    }
    else {
        layer = doc->queryLayer(entity.getLayerId());
        Q_ASSERT(!layer.isNull());
    }
    if (!layer.isNull()) {
        currentLayer = layer.data();
    }

    // find block reference of the current entity, ignore this entity:
    bool blockRefSet = false;
    if (currentBlockRef==NULL) {
        RBlockReferenceEntity* blockRef =
            dynamic_cast<RBlockReferenceEntity*>(&entity);
        if (blockRef!=NULL) {
            if (blockSource!=NULL) {
                RBlock::Id blockId1 = blockRef->getReferencedBlockId();
                QString blockName = doc->getBlockName(blockId1);
                RBlock::Id blockId2 = blockSource->getBlockId(blockName);
                blockRef->setReferencedBlockId(blockId2);
                blockRef->setDocument(blockSource);
                /*
                RBlock::Id blockId1 = blockRef->getReferencedBlockId();
                QString blockName = doc->getBlockName(blockId1);
                blockRefBS = blockSource->queryBlock(blockName);
                currentBlockRef = blockRefBS.dynamicCast<RBlockReferenceEntity>().data();
                */
            }

            currentBlockRef = blockRef;
            blockRefSet = true;
        }
    }

    startEntity(blockRefSet || currentBlockRef==NULL);
	exportCurrentEntity();
	endEntity();

    if (blockRefSet) {
        currentBlockRef = NULL;
        //blockRefBS.clear();
    }
    currentLayer = NULL;

    entityStack.pop();
}

/**
 * Calls \ref exportEntity(REntity*) with a temporary entity object.
 */
void RExporter::exportEntity(REntity::Id entityId) {
	QSharedPointer<REntity> e = document.queryEntity(entityId);
    if (!e.isNull()) {
        exportEntity(*e);
    }
    else {
        unexportEntity(entityId);
    }
}



/**
 * The default implementation calls the \ref REntity::exportEntity() function
 * of the entity.
 * This method may use the \c currentEntity stack to access the
 * entity that is currently being exported.
 * Exporters can choose to reimplement this function to export an entity
 * in a target platform specific manner (e.g. to optimize things for
 * a specific platform).
 */
void RExporter::exportCurrentEntity() {
    if (getEntity()==NULL) {
        return;
    }
    /*
    RDocument* doc = getEntity()->getDocument();
    if (doc==NULL) {
        doc = &document;
    }
    QSharedPointer<RLayer> layer = doc->queryLayer(getEntity()->getLayerId());
    if (layer.isNull() || !layer->isFrozen()) {
    */
    if (currentLayer==NULL || !currentLayer->isFrozen()) {
        getEntity()->exportEntity(*this);
    }
}



/**
 * \return True if there is a current entity and it is selected, false
 *      otherwise.
 */
bool RExporter::isEntitySelected() {
    const REntity* entity = getEntity();
    if (entity!=NULL) {
        return entity->isSelected();
    }
    else {
        return false;
    }
}



/**
 * This is called for entities for which an export was requested
 * but which cannot be instantiated because they either do not exist
 * (anymore) in the DB or they are currently undone.
 */
void RExporter::unexportEntity(REntity::Id /*entityId*/) {}



/**
 * Calls \ref exportEntity() for all given entities.
 */
void RExporter::exportEntities(QSet<REntity::Id>& entityIds) {
    QSet<REntity::Id>::iterator it;
    for (it=entityIds.begin(); it!=entityIds.end(); ++it) {
        exportEntity(*it);
    }
}



/**
 * Exports a quadrilateral with the current attributes.
 * This is a convenience function that exports two triangles but may
 * also be re-implemented to do something else.
 */
void RExporter::exportQuad(
    const RVector& p1, const RVector& p2,
    const RVector& p3, const RVector& p4) {

    exportTriangle(RTriangle(p1, p2, p3));
    exportTriangle(RTriangle(p3, p4, p1));
}

/**
 * Exports a rectangle with the current attributes.
 * This is a convenience function that exports two triangles but may
 * also be re-implemented to do something else.
 */
void RExporter::exportRectangle(const RVector& p1, const RVector& p2) {
	exportQuad(p1, RVector(p2.x, p1.y), p2, RVector(p1.x, p2.y));
}

/**
 * Exports a vertical quadrilateral with the two given
 * base points and the given height.
 */
void RExporter::exportVerticalQuad(
        const RVector& p1,
        const RVector& p2,
        double height) {

    exportQuad(p1, p2, p2+RVector(0,0,height), p1+RVector(0,0,height));
}



/**
 * Exports the surfaces of the given box.
 * This is a convenience function that exports 12 triangles but may
 * also be re-implemented to do something else.
 */
void RExporter::exportBox(const RBox& box) {
    QList<RTriangle> triangles = box.getTriangles();
    QList<RTriangle>::iterator it;
    for (it=triangles.begin(); it!=triangles.end(); ++it) {
        exportTriangle(*it);
    }
}

RLinetypePattern RExporter::getLinetypePattern() {
    return currentLinetypePattern;
    /*
    RLinetypePattern p;
    if (getEntity() != NULL) {
        RLinetype::Id id = getEntity()->getLinetypeId();
        RDocument* doc = getEntity()->getDocument();
        if (doc == NULL) {
            doc = &document;
        }
        QString ltName = doc->getLinetypeName(id);
        if (!ltName.compare("BYLAYER", Qt::CaseInsensitive)) {
            QSharedPointer<RLayer> layer = doc->queryLayer(
                    getEntity()->getLayerId());
            if (layer.isNull()) {
                RDebug::error(QString("RExporter::getLinetypePattern: layer is NULL"));
                return p;
            }
            id = layer->getLinetypeId();
        }
        p = RLinetypePatternMap::getPattern(doc->getLinetypeName(id));
    }
    return p;
    */
}

void RExporter::exportLine(const RLine& line, double offset){
    RLinetypePattern p = getLinetypePattern();

    if (getEntity() == NULL || !p.isValid() || p.getNumDashes() == 1) {
		exportLineSegment(line);
		return;
	}

	p.scale(getPatternFactor());
	double length = line.getLength();
	double angle = line.getAngle();
	RVector* vp = NULL;
	vp = new RVector[p.getNumDashes()];
	for (int i = 0; i < p.getNumDashes(); ++i) {
		vp[i] = RVector(cos(angle) * fabs(p.getDashLengthAt(i)),
						sin(angle) * fabs(p.getDashLengthAt(i)));
	}

	if (std::isnan(offset)) {
		offset = getPatternOffset(length, p);
	}

	bool done = false;
	int i = 0;
	RVector cursor(line.getStartPoint() + RVector::createPolar(offset, angle));
	double total = offset;
	bool dashFound = false;
	bool gapFound = false;
	RVector p1 = line.getStartPoint();
	RVector p2;
	do {
		if (dashFound && !gapFound) {
			if (total + fabs(p.getDashLengthAt(i)) >= length - 1.0e-6) {
				exportLineSegment(RLine(p1, line.endPoint));
				break;
			}
			exportLineSegment(RLine(p1, p2));
		}
		if (p.getDashLengthAt(i) > 0) {
			// dash, no gap
			if (total + p.getDashLengthAt(i) > 0) {
				p1 = cursor;
				if (total < 0 || !dashFound) {
					p1 = line.startPoint;
				}
				p2 = cursor + vp[i];
				if (p2.getDistanceTo(line.startPoint) > 1.0e-6) {
					dashFound = true;
				}
			}
			gapFound = false;
		} else {
			gapFound = true;
		}
		cursor += vp[i];
		total += fabs(p.getDashLengthAt(i));
		done = total > length;
		++i;
		if (i >= p.getNumDashes()) {
			i = 0;
		}
	} while (!done);

	if (!gapFound || !dashFound) {
		if (total + fabs(p.getDashLengthAt(i)) >= length - 1.0e-6) {
			exportLineSegment(RLine(p1, line.endPoint));
		} else {
			exportLineSegment(RLine(p1, p2));
		}
	}

	delete[] vp;
}

void RExporter::exportArc(const RArc& arc, double offset){
	RArc normalArc = arc;
	if (arc.isReversed()) {
		normalArc.reverse();
	}
	RLinetypePattern p = getLinetypePattern();

    if (getEntity() == NULL || !p.isValid() || p.getNumDashes() == 1) {
		exportArcSegment(normalArc);
		return;
	}

	if (normalArc.radius < 1.0e-12) {
		return;
	}

	p.scale(getPatternFactor());
	double length = normalArc.getLength();
	double* vp = NULL;
	vp = new double[p.getNumDashes()];
	for (int i = 0; i < p.getNumDashes(); ++i) {
		vp[i] = fabs(p.getDashLengthAt(i)) / normalArc.radius;
	}

	if (std::isnan(offset)) {
		offset = getPatternOffset(length, p);
	}

	bool done = false;
	int i = 0;
	double cursor = normalArc.getStartAngle() + offset / normalArc.radius;
	double total = offset;
	bool dashFound = false;
	bool gapFound = false;
	double p1 = normalArc.getStartAngle();
	double p2;
	do {
		if (dashFound && !gapFound) {
			if (total + fabs(p.getDashLengthAt(i)) >= length - 1.0e-6) {
				exportArcSegment(RArc(normalArc.getCenter(), normalArc.getRadius(), p1,
						normalArc.getEndAngle()));
				break;
			}
			exportArcSegment(RArc(normalArc.getCenter(), normalArc.getRadius(), p1, p2));
		}
		if (p.getDashLengthAt(i) > 0) {
			// dash, no gap
			if (total + p.getDashLengthAt(i) > 0) {
				p1 = cursor;
				if (total < 0 || !dashFound) {
					p1 = normalArc.startAngle;
				}
				p2 = cursor + vp[i];
				if (fabs(p2 - normalArc.getStartAngle()) > 1.0e-6) {
					dashFound = true;
				}
			}
			gapFound = false;
		} else {
			gapFound = true;
		}
		cursor += vp[i];
		total += fabs(p.getDashLengthAt(i));
		done = total > length;
		++i;
		if (i >= p.getNumDashes()) {
			i = 0;
		}
	} while (!done);

	if (!gapFound || !dashFound) {
		if (total + fabs(p.getDashLengthAt(i)) >= length - 1.0e-6) {
			exportArcSegment(RArc(normalArc.getCenter(), normalArc.getRadius(), p1,
					normalArc.getEndAngle()));
		} else {
			exportArcSegment(RArc(normalArc.getCenter(), normalArc.getRadius(), p1, p2));
		}
	}

	delete[] vp;
}

double RExporter::getPatternOffset(double length,
		const RLinetypePattern& pattern) {
	double optOffset = 0.0;
	double gap = 0.0;
	double maxGap = RMINDOUBLE;
	for (int i = 0; i < pattern.getNumDashes(); ++i) {
		if (!pattern.isSymmetrical(i)) {
			continue;
		}
		double offset = getPatternOffset(length, pattern, i, &gap);
		if (gap > maxGap) {
			maxGap = gap;
			optOffset = offset;
//			RDebug::debug(QString("RExporter::getPatternOffset: i=%1").arg(i));
		}
	}
	return optOffset;
}

double RExporter::getPatternOffset(double length,
		const RLinetypePattern& pattern, int index, double* gap) {
	double po = fabs(pattern.getDashLengthAt(index)) / 2;
	for (int i = index - 1; i >= 0; --i) {
		po += fabs(pattern.getDashLengthAt(i));
	}
	double offset = length / 2 - po;
	int m = (int) trunc(offset / (pattern.getPatternLength()));
	offset -= (m + 1) * pattern.getPatternLength();
	if (gap != NULL) {
		*gap = pattern.getDelta(-offset);
	}

	/*
	if (pattern.hasDashAt(offset)) {
		if (gap != NULL) {
			*gap = pattern.getDelta(offset);
		}
		return offset;
	}
	if (gap != NULL) {
		*gap = pattern.getDelta(offset);
	}
	*/
//	RDebug::debug(QString("RExporter::getPatternOffset: pattern.hasDashAt(offset)"));
//	if (index + 1 < pattern.getNumDashes()) {
//		RDebug::debug(QString("RExporter::getPatternOffset: index + 1 < pattern.getNumDashes()"));
//		return getPatternOffset(length, pattern, index + 1);
//	}
	return offset;
}

double RExporter::getPatternFactor() {
	double factor = currentPen.widthF();
	if (factor < 1e-6) {
		return 1.0;
	}
	return factor;
}

QStack<REntity*> RExporter::getEntityStack() {
    return entityStack;
}
