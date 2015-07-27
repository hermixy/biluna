#include "gv_graphicsviewqtwidget.h"

#include "gv_eventhandlerqt.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsSceneQt.h"
#include "ROrthoGrid.h"
//#include "RSpatialIndexNavel.h"
#include "RSpatialIndexSimple.h"
#include "RMemoryStorage.h"

#include "RAddObjectOperation.h"
#include "RArcEntity.h"
#include "RCircleEntity.h"
#include "RLineEntity.h"
#include "RPointEntity.h"
#include <QSharedPointer>




GV_GraphicsViewQtWidget::GV_GraphicsViewQtWidget(QWidget *parent)
        : RB_Widget(parent) {
    setupUi(this);

    mEventHandler = NULL;
}

GV_GraphicsViewQtWidget::~GV_GraphicsViewQtWidget() {
    delete mEventHandler;
}

void GV_GraphicsViewQtWidget::init() {

    // From New.js
    RStorage* ms = new RMemoryStorage();
    //RSpatialIndexNavel* spatialIndex = new RSpatialIndexNavel();
    RSpatialIndexSimple* si = new RSpatialIndexSimple();

    RDocument* document = new RDocument(*ms, *si);
    RDocumentInterface* di = new RDocumentInterface(*document);
    RGraphicsSceneQt* scene = new RGraphicsSceneQt(*di);

    GraphicsView->setScene(scene);
    GraphicsView->setGrid(new ROrthoGrid());

    HorizontalScrollBar_2->setSingleStep(50);
    VerticalScrollBar->setSingleStep(50);

    HorizontalRuler->setGraphicsView(GraphicsView);
    VerticalRuler->setGraphicsView(GraphicsView);

    di->addCoordinateListener(HorizontalRuler);
    di->addCoordinateListener(VerticalRuler);




    mEventHandler = new GV_EventHandlerQt();
    mEventHandler->setGraphicsView(GraphicsView);
    mEventHandler->setMdiChild(NULL);
    mEventHandler->setDocumentInterface(di);
    mEventHandler->setScrollBar(HorizontalScrollBar_2);
    mEventHandler->setScrollBar(VerticalScrollBar);
    mEventHandler->setRuler(HorizontalRuler);
    mEventHandler->setRuler(VerticalRuler);

//    this.graphicsView.drop.connect(eventHandler, "drop");
//    this.graphicsView.dragEnter.connect(eventHandler, "dragEnter");
    connect(GraphicsView, SIGNAL(viewportChanged()),
            mEventHandler, SLOT(viewportChanged()));
    connect(HorizontalScrollBar_2, SIGNAL(valueChanged(int)),
            mEventHandler, SLOT(horizontalScrolled(int)));
    connect(VerticalScrollBar, SIGNAL(valueChanged(int)),
            mEventHandler, SLOT(verticalScrolled(int)));

    mEventHandler->viewportChanged();


    // Test adding point or line
//    var point = new RPointEntity(
//        this.getDocument(),
//        new RPointData(event.getModelPosition())
//    );
//    var op = new RAddObjectOperation(point);
//    this.getDocumentInterface().applyOperation(op);

//    RPointEntity* pe = new RPointEntity(document, RPointData(RVector(35,45)));
//    RAddObjectOperation* oper = new RAddObjectOperation(QSharedPointer<RObject>(pe));
//    di->applyOperation(*oper);

    RLineEntity* le = new RLineEntity(document, RLineData(RVector(0,0),
                                                          RVector(100,100)));
//    oper->addObject(QSharedPointer<RObject>(le));
    RAddObjectOperation* oper = new RAddObjectOperation(QSharedPointer<RObject>(le));
    di->applyOperation(*oper);

//    RCircleEntity* ce = new RCircleEntity(document, RCircleData(RVector(50,50),
//                                                                50.0));
//    oper->addObject(QSharedPointer<RObject>(ce));
//    oper = new RAddObjectOperation(QSharedPointer<RObject>(ce));
//    di->applyOperation(*oper);

//    RArcEntity* ae = new RArcEntity(document, RArcData(RVector(50,50),
//                                                       25.0, 0.0, 90.0));
//    oper->addObject(QSharedPointer<RObject>(ae));
//    oper = new RAddObjectOperation(QSharedPointer<RObject>(ae));
//    di->applyOperation(*oper);

}
