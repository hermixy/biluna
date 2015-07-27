#include <QDragMoveEvent>
#include <QScrollBar>

#include "RDebug.h"
#include "RDocumentInterface.h"
#include "REntity.h"
#include "RGraphicsSceneQt.h"
#include "RGraphicsView.h"
#include "RLine.h"
#include "RPoint.h"
#include "RSettings.h"



RGraphicsSceneQt::RGraphicsSceneQt(RDocumentInterface& documentInterface)
    : RGraphicsScene(documentInterface) {
    setProjectionRenderingHint(RS::Top);
}

RGraphicsSceneQt::~RGraphicsSceneQt() {
}

void RGraphicsSceneQt::exportLine(const RLine& line, double offset) {
    if (!exportToPreview) {
        if (getEntity() == NULL) {
            RDebug::warning("RGraphicsSceneQt::exportLine: entity is NULL");
            return;
        }
	}

    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

	currentPainterPath = RPainterPath();
    currentPainterPath.setZLevel(0);
    currentPainterPath.setPen(currentPen);

    if (!exportToPreview) {
        if (getEntity()->isSelected()) {
            currentPainterPath.setSelected(true);
        }
	}

	RGraphicsScene::exportLine(line, offset);

    if (!exportToPreview) {
        painterPaths.insert(getBlockRefOrEntity()->getId(), currentPainterPath);
    } else {
		addToPreview(currentPainterPath);
	}
}

void RGraphicsSceneQt::exportArc(const RArc& arc, double offset) {
    if (!exportToPreview) {
        if (getEntity() == NULL) {
            RDebug::warning("RGraphicsSceneQt::exportArc: entity is NULL");
            return;
        }
	}

    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

	currentPainterPath = RPainterPath();
    currentPainterPath.setZLevel(0);
    currentPainterPath.setPen(currentPen);

    if (!exportToPreview) {
        if (getEntity()->isSelected()) {
            currentPainterPath.setSelected(true);
        }
	}

	RGraphicsScene::exportArc(arc, offset);

    if (!exportToPreview) {
        painterPaths.insert(getBlockRefOrEntity()->getId(), currentPainterPath);
	} else {
		addToPreview(currentPainterPath);
	}
}

void RGraphicsSceneQt::exportPoint(const RPoint& point) {
    if (!exportToPreview) {
        if (getEntity() == NULL) {
            RDebug::warning("RGraphicsSceneQt::exportPoint: entity is NULL");
            return;
        }
	}

    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

	currentPainterPath = RPainterPath();
    currentPainterPath.setZLevel(0);
    currentPainterPath.setPen(currentPen);

    if (!exportToPreview) {
        if (getEntity()->isSelected()) {
            currentPainterPath.setSelected(true);
        }
	}

    currentPainterPath.addPoint(point.position);
    //currentPainterPath.addRect(point.position.x, point.position.y, 0, 0);
//    currentPainterPath.moveTo(point.position.x, point.position.y);
//    currentPainterPath.lineTo(point.position.x, point.position.y);

    if (!exportToPreview) {
        painterPaths.insert(getBlockRefOrEntity()->getId(), currentPainterPath);
	} else {
		addToPreview(currentPainterPath);
	}
}

void RGraphicsSceneQt::exportCircle(const RCircle& circle) {
    if (!exportToPreview) {
        if (getEntity() == NULL) {
            RDebug::warning("RGraphicsSceneQt::exportCircle: entity is NULL");
            return;
        }
	}

    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

	currentPainterPath = RPainterPath();
    currentPainterPath.setZLevel(0);
    currentPainterPath.setPen(currentPen);

    if (!exportToPreview) {
        if (getEntity()->isSelected()) {
            currentPainterPath.setSelected(true);
        }
	}

    RArc arc(circle.center, circle.radius, 0.0, 2*M_PI, false);
    RGraphicsScene::exportArc(arc);
    /*
    currentPainterPath.addEllipse(QPointF(circle.center.x, circle.center.y),
			circle.radius, circle.radius);
    */

    if (!exportToPreview) {
        painterPaths.insert(getBlockRefOrEntity()->getId(), currentPainterPath);
	} else {
		addToPreview(currentPainterPath);
	}
}

void RGraphicsSceneQt::exportArcSegment(const RArc& arc) {
    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

    currentPainterPath.moveTo(QPointF(arc.getStartPoint().x,
			arc.getStartPoint().y));
    RBox box(arc.center - RVector(arc.radius, arc.radius), arc.center
			+ RVector(arc.radius, arc.radius));
	currentPainterPath.arcTo(box.toQRectF(), 360 - RMath::rad2deg(
			arc.startAngle), -RMath::rad2deg(arc.getSweep()));
}

void RGraphicsSceneQt::exportLineSegment(const RLine& line) {
    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */
    // add new painter path with current entity ID:
    currentPainterPath.moveTo(line.startPoint.x, line.startPoint.y);
    currentPainterPath.lineTo(line.endPoint.x, line.endPoint.y);
}

void RGraphicsSceneQt::exportTriangle(const RTriangle& triangle) {
    if (getEntity() == NULL && !exportToPreview) {
		RDebug::warning("RGraphicsSceneQt::exportTriangle: entity is NULL");
		return;
	}

    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

    // add new painter path with current entity ID:
    RPainterPath p;
    p.setZLevel(0);
    p.setPen(Qt::NoPen);
    p.setBrush(currentBrush);
    p.moveTo(triangle.corner[0].x, triangle.corner[0].y);
    p.lineTo(triangle.corner[1].x, triangle.corner[1].y);
    p.lineTo(triangle.corner[2].x, triangle.corner[2].y);

    if (!exportToPreview) {
        painterPaths.insert(getBlockRefOrEntity()->getId(), p);
	} else {
		addToPreview(p);
	}
}

void RGraphicsSceneQt::exportRectangle(const RVector& p1,
		const RVector& p2) {
    if (getEntity() == NULL && !exportToPreview) {
		RDebug::warning("RGraphicsSceneQt::exportRectangle: entity is NULL");
		return;
	}

    /*
    if (!exportToPreview) {
       if (getEntity()->getBlockId()!=document.getCurrentBlockId()) {
           return;
       }
    }
    */

    RPainterPath p;
    p.setZLevel(0);
    p.setPen(currentPen);
    p.setBrush(currentBrush);
    RVector v = RVector::getMinimum(p1, p2);
	p.addRect(v.x, v.y, fabs(p2.x - p1.x), fabs(p2.y - p1.y));

    if (!exportToPreview) {
        painterPaths.insert(getBlockRefOrEntity()->getId(), p);
	} else {
		addToPreview(p);
	}
}

void RGraphicsSceneQt::unexportEntity(REntity::Id entityId) {
	if (!exportToPreview) {
		painterPaths.remove(entityId);
    }
}

void RGraphicsSceneQt::deletePainterPaths() {
	painterPaths.clear();
}

/**
 * \return A list of all painter paths that represent the entity with the
 * given ID.
 */
QList<RPainterPath> RGraphicsSceneQt::getPainterPaths(REntity::Id entityId) {
    if (painterPaths.contains(entityId)) {
        return painterPaths.values(entityId);
    }
    else {
        RDebug::warning("RGraphicsSceneQt::getPainterPath: "
            "no painter paths found for entity %d", entityId);
        return QList<RPainterPath>();
    }
}

QList<RPainterPath> RGraphicsSceneQt::getPreviewPainterPaths() {
    return previewPainterPaths;
}

void RGraphicsSceneQt::clearPreview() {
	RGraphicsScene::clearPreview();
    previewPainterPaths.clear();
}
    
void RGraphicsSceneQt::addToPreview(const RPainterPath& painterPath) {
    previewPainterPaths << painterPath;
}

void RGraphicsSceneQt::addToPreview(const QList<RPainterPath>& painterPaths) {
    previewPainterPaths << painterPaths;
}

void RGraphicsSceneQt::highlightEntity(REntity& entity) {
	beginPreview();
	// get painter paths for closest entity:
	QList<RPainterPath> painterPaths = getPainterPaths(entity.getId());
	for (int i = 0; i < painterPaths.size(); ++i) {
        painterPaths[i].setSelected(entity.isSelected());
		painterPaths[i].setHighlighted(true);
	}
	addToPreview(painterPaths);
	endPreview();
}

void RGraphicsSceneQt::startEntity(bool topLevelEntity) {
	if (getEntity() == NULL) {
		return;
	}

    RGraphicsScene::startEntity(topLevelEntity);

	if (!exportToPreview) {
        if (topLevelEntity) {
            painterPaths.remove(getEntity()->getId());
        }
	}
}

/*
void RGraphicsSceneQt::addCursorToPreview(const RVector& pos) {
    //RVector sp = mapFromModel(pos);
    RVector sp = pos;
    RPainterPath p;
    p.setZLevel(0);
    p.setPen(QPen(QColor(255,194,0), 0, Qt::DashLine));
    p.moveTo(sp.x-1.0e6, sp.y);
    p.lineTo(sp.x+1.0e6, sp.y);
    p.moveTo(sp.x, sp.y-1.0e6);
    p.lineTo(sp.x, sp.y+1.0e6);
    addToPreview(p);
}
*/

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, RGraphicsSceneQt& gs) {
    //dbg.nospace() << "RGraphicsSceneQt(" << QString("%1").arg((int)&gs, 0, 16) << ")";
    QMultiMap<REntity::Id, RPainterPath>::iterator it;
    for (it=gs.painterPaths.begin(); it!=gs.painterPaths.end(); it++) {
        dbg.nospace() << "\n" << it.key() << "\n  " << it.value() << "\n";
    }
    return dbg.space();
}
