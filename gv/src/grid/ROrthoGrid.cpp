#include <math.h>

#include <QFontMetrics>

#include "RDebug.h"
#include "RDocumentInterface.h"
#include "RExporter.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RLine.h"
#include "ROrthoGrid.h"
#include "RUcs.h"



ROrthoGrid::ROrthoGrid() 
  : RGrid(),
    spacing(RVector::invalid),
    minSpacing(RVector::invalid),
    metaSpacing(RVector::invalid),
    scaleGrid(true),
    minPixelSpacing(10),
    unit(RS::None)
{
}



ROrthoGrid::~ROrthoGrid() {
}



/**
 * Maps the given model position to the grid.
 */
RVector ROrthoGrid::snapToGrid(RGraphicsView& view, const RVector& positionUcs) {
    RDocumentInterface* documentInterface = view.getDocumentInterface();
    if (documentInterface==NULL) {
        return RVector::invalid;
    }

    RVector gridPositionUcs = RVector(
        RMath::mround(positionUcs.x / spacing.x) * spacing.x,
        RMath::mround(positionUcs.y / spacing.y) * spacing.y,
        RMath::mround(positionUcs.z / spacing.z) * spacing.z
    );

    RUcs ucs = documentInterface->getCurrentUcs();
    return ucs.mapFromUcs(gridPositionUcs);
}



/**
 * Updates the grid information, in particular the grid spacing and
 * grid region to the current view port.
 */
void ROrthoGrid::update(RGraphicsView& view) {
    if (viewBox==view.getBox()) {
        return;
    }

    viewBox = view.getBox();

    RDocument* doc = view.getDocument();
    if (doc == NULL) {
        RDebug::warning(QString("ROrthoGrid::update: document is NULL"));
        return;
    }

    RGraphicsScene* scene = view.getScene();
    if (scene==NULL) {
        RDebug::warning("ROrthoGrid::update: scene is NULL");
        return;
    }

    unit = doc->getUnit();

    if (!minSpacing.isValid()) {
        if (RS::isMetric(unit)) {
            minSpacing = RVector(1.0e-3, 1.0e-3, 1.0e-3);
        } else {
            minSpacing = RVector(1.0 / 128, 1.0 / 128, 1.0 / 128);
        }
    }

    RS::ProjectionRenderingHint projection =
            scene->getProjectionRenderingHint();
    // for 3d views, we have no convenient way to calculate the grid dimensions:
    if (projection == RS::ThreeD) {
        gridBox = RBox(RVector(-1000, -1000), RVector(1000, 1000));
        spacing = RVector(10.0, 10.0);
        return;
    }

    if (scaleGrid) {
        // ideal (minimum) grid spacing for the given view (some odd number):
        double idealSpacing = view.mapDistanceFromView(minPixelSpacing);

        if (RS::isMetric(unit)) {
            // idealSpacing = minSpacing * idealFactor
            RVector idealFactor(idealSpacing / minSpacing.x, idealSpacing
                            / minSpacing.y, idealSpacing / minSpacing.z);

            // idealFactor = minSpacing * 10^n
            RVector n(log(idealFactor.x / minSpacing.x) / log(10.0), log(
                            idealFactor.y / minSpacing.y) / log(10.0), log(
                            idealFactor.z / minSpacing.z) / log(10.0));

            // factor = minSpacing * 10^ceil(n)
            RVector factor(minSpacing.x * pow(10.0, ceil(n.x - 1.0e-6)),
                            minSpacing.y * pow(10.0, ceil(n.y - 1.0e-6)), minSpacing.z
                                            * pow(10.0, ceil(n.z - 1.0e-6)));

            spacing = RVector(minSpacing.x * factor.x, minSpacing.y * factor.y,
                            minSpacing.z * factor.z);
            metaSpacing = spacing * 10;
        } else {
            spacing = RS::convert(minSpacing, unit, RS::Inch);
            spacing.x = inchAutoscale(spacing.x, idealSpacing);
            spacing.y = inchAutoscale(spacing.y, idealSpacing);

            metaSpacing = RVector(1.0 / 64, 1.0 / 64, 1.0 / 64);
            metaSpacing.x = inchAutoscale(metaSpacing.x, idealSpacing * 4);
            metaSpacing.y = inchAutoscale(metaSpacing.y, idealSpacing * 4);

            spacing = RS::convert(spacing, RS::Inch, unit);
            metaSpacing = RS::convert(metaSpacing, RS::Inch, unit);
        }
    } else {
        spacing = minSpacing;
    }
    
    RVector minGridPoint;
    RVector maxGridPoint;

    minGridPoint = viewBox.getCorner1();
    minGridPoint = minGridPoint.getDividedComponents(spacing).getFloor();
    //minGridPoint -= RVector(1,1,1);
    minGridPoint = minGridPoint.getMultipliedComponents(spacing);

    maxGridPoint = viewBox.getCorner2();
    maxGridPoint = maxGridPoint.getDividedComponents(spacing).getCeil();
    //minGridPoint += RVector(1,1,1);
    maxGridPoint = maxGridPoint.getMultipliedComponents(spacing);

    minGridPoint.z = viewBox.getCorner1().z;
    maxGridPoint.z = viewBox.getCorner2().z;
    
    gridBox = RBox(minGridPoint, maxGridPoint);

    minGridPoint = viewBox.getCorner1();
    minGridPoint = minGridPoint.getDividedComponents(metaSpacing).getFloor();
    minGridPoint = minGridPoint.getMultipliedComponents(metaSpacing);

    maxGridPoint = viewBox.getCorner2();
    maxGridPoint = maxGridPoint.getDividedComponents(metaSpacing).getCeil();
    maxGridPoint = maxGridPoint.getMultipliedComponents(metaSpacing);

    minGridPoint.z = viewBox.getCorner1().z;
    maxGridPoint.z = viewBox.getCorner2().z;
    
    metaGridBox = RBox(minGridPoint, maxGridPoint);
}

double ROrthoGrid::inchAutoscale(double x, double idealSpacing) {
    while (x < idealSpacing) {
        if (RMath::mround(x) >= 36) {
            x *= 2;
        } else if (RMath::mround(x) >= 12) {
            x *= 3;
        } else if (RMath::mround(x) >= 4) {
            x *= 3;
        } else if (RMath::mround(x) >= 1) {
            x *= 2;
        } else {
            x *= 2;
        }
    }
    return x;
}


void ROrthoGrid::paint(RGraphicsView& view) {
    if (!spacing.isValid()) {
            return;
    }
    RVector min = gridBox.getCorner1();
    RVector max = gridBox.getCorner2();

    RVector gridPointUcs;
    for (gridPointUcs.x = min.x; gridPointUcs.x < max.x;
            gridPointUcs.x += spacing.x) {
        for (gridPointUcs.y = min.y; gridPointUcs.y < max.y; gridPointUcs.y
                        += spacing.y) {
                view.paintGridPoint(gridPointUcs);
        }
    }
}

void ROrthoGrid::paintMetaGrid(RGraphicsView& view) {
    if (!metaSpacing.isValid()) {
        return;
    }

    RVector min = metaGridBox.getCorner1();
    RVector max = metaGridBox.getCorner2();

    for (double x=min.x; x<max.x; x+=metaSpacing.x) {
        view.paintMetaGridLine(RLine(RVector(x, min.y), RVector(x, max.y)));
    }
    for (double y=min.y; y<max.y; y+=metaSpacing.y) {
        view.paintMetaGridLine(RLine(RVector(min.x, y), RVector(max.x, y)));
    }
}

void ROrthoGrid::paintRuler(RGraphicsView& view, RRuler& ruler) {
    if (!metaSpacing.isValid()) {
        return;
    }

    RDocument* doc = view.getDocument();
    if (doc == NULL) {
        return;
    }

    RVector min = gridBox.getCorner1();
    RVector max = gridBox.getCorner2();
    bool isHorizontal = ruler.getOrientation() == Qt::Horizontal;

    double tickSpacing;
    if (RS::isMetric(doc->getUnit())) {
        if (isHorizontal) {
            tickSpacing = metaSpacing.x;
        } else {
            tickSpacing = metaSpacing.y;
        }
        if (view.mapDistanceToView(tickSpacing) >= 80) {
            tickSpacing /= 10;
        } else if (view.mapDistanceToView(tickSpacing) >= 30) {
            tickSpacing /= 5;
        } else if (view.mapDistanceToView(tickSpacing) >= 20) {
            tickSpacing /= 2;
        }
    } else {
        if (isHorizontal) {
            tickSpacing = spacing.x;
        } else {
            tickSpacing = spacing.y;
        }
    }
    int pSpacing = (int) ceil(view.mapDistanceToView(tickSpacing));

    QString l1 = RS::getLabel(isHorizontal ? min.x : min.y);
    QString l2 = RS::getLabel(isHorizontal ? max.x : max.y);
    int labelWidth = std::max(
                QFontMetrics(ruler.getFont()).boundingRect(l1).width(),
                QFontMetrics(ruler.getFont()).boundingRect(l2).width()) + 15;
    int minLabelStep = labelWidth / pSpacing + 1;

    int labelStep = minLabelStep;
    if (RS::isMetric(doc->getUnit())) {
        // metric
        if (labelStep >= 3 && labelStep <= 4) {
            labelStep = 5;
        } else if (labelStep >= 6 && labelStep <= 9) {
            labelStep = 10;
        } else if (labelStep >= 11 && labelStep <= 19) {
            labelStep = 20;
        } else if (labelStep >= 21 && labelStep <= 99) {
            labelStep = 100;
        }
    } else {
        // non metric
        int f = 1;
        do {
            if (isHorizontal) {
                labelStep = RMath::mround(metaSpacing.x / spacing.x) * f;
            } else {
                labelStep = RMath::mround(metaSpacing.y / spacing.y) * f;
            }
            f = f * 2;
        } while (labelStep < minLabelStep);
    }

    double minPos;
    double maxPos;
    if (isHorizontal) {
        minPos = (floor(view.mapFromView(RVector(0, 0)).x
                        / (labelStep * tickSpacing))-1) * (labelStep * tickSpacing);
        maxPos = (ceil(view.mapFromView(RVector(view.getWidth(), 0)).x
                        / (labelStep * tickSpacing))+1) * (labelStep * tickSpacing);
    } else {
        minPos = (floor(view.mapFromView(RVector(0, view.getHeight())).y
                        / (labelStep * tickSpacing))-1) * (labelStep * tickSpacing);
        maxPos = (ceil(view.mapFromView(RVector(0, 0)).y
                        / (labelStep * tickSpacing))+1) * (labelStep * tickSpacing);
    }

    int c;
    double p;
    for (c = 0, p = minPos; p < maxPos; p += tickSpacing, ++c) {
        bool hasLabel = c % labelStep == 0;
        double v;
        if (isHorizontal) {
            v = view.mapToView(RVector(p, 0)).x;
        } else {
            v = view.mapToView(RVector(0, p)).y;
        }
        ruler.paintTick(v, hasLabel, hasLabel ? RS::getLabel(p) : QString());
    }
}

QString ROrthoGrid::getInfoText() {
    return QString("%1 / %2").arg(spacing.x).arg(metaSpacing.x);
}
