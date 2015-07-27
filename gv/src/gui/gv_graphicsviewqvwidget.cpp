#include "gv_graphicsviewqvwidget.h"

#include "gv_eventhandlerqv.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsSceneQV.h"
#include "RMemoryStorage.h"
#include "ROrthoGrid.h"
//#include "RSpatialIndexNavel.h"
#include "RSpatialIndexSimple.h"

#include "RAddObjectOperation.h"
#include "RArcEntity.h"
#include "RCircleEntity.h"
#include "RLineEntity.h"
#include "RPointEntity.h"
#include <QSharedPointer>



GV_GraphicsViewQVWidget::GV_GraphicsViewQVWidget(QWidget *parent)
        : RB_Widget(parent) {
    setupUi(this);

    mEventHandler = NULL;
}

GV_GraphicsViewQVWidget::~GV_GraphicsViewQVWidget() {
    delete mEventHandler;
}

void GV_GraphicsViewQVWidget::init() {

    // From New.js
    RStorage* ms = new RMemoryStorage();
    //RSpatialIndexNavel* spatialIndex = new RSpatialIndexNavel();
    RSpatialIndexSimple* si = new RSpatialIndexSimple();

    RDocument* document = new RDocument(*ms, *si);
    RDocumentInterface* di = new RDocumentInterface(*document);
    RGraphicsSceneQV* scene = new RGraphicsSceneQV(*di);

    view->setScene(scene);
    view->setGrid(new ROrthoGrid());

    int lbHeight = view->horizontalScrollBar()->sizeHint().height();
    int lbWidth = verticalRuler->sizeHint().width();
    int rtHeight = horizontalRuler->sizeHint().height();
    int rtWidth = view->verticalScrollBar()->sizeHint().width();
    leftBottomFrame->setMinimumHeight(lbHeight);
    leftTopFrame->setMinimumSize(rtHeight, lbWidth);
    rightTopFrame->setMinimumWidth(rtWidth);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->horizontalScrollBar()->setSingleStep(50);
    view->verticalScrollBar()->setSingleStep(50);

    horizontalRuler->setGraphicsView(view);
    verticalRuler->setGraphicsView(view);

    di->addCoordinateListener(horizontalRuler);
    di->addCoordinateListener(verticalRuler);

    //
    // di->regenerateScenes();
    //

    mEventHandler = new GV_EventHandlerQV();
    mEventHandler->setGraphicsView(view);
    mEventHandler->setMdiChild(NULL);
    mEventHandler->setDocumentInterface(di);
    mEventHandler->setScrollBar(view->horizontalScrollBar());
    mEventHandler->setScrollBar(view->verticalScrollBar());
    mEventHandler->setRuler(horizontalRuler);
    mEventHandler->setRuler(verticalRuler);

//    this.graphicsView.drop.connect(eventHandler, "drop");
//    this.graphicsView.dragEnter.connect(eventHandler, "dragEnter");
    connect(view, SIGNAL(viewportChanged()),
            mEventHandler, SLOT(viewportChanged()));
    connect(view->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            mEventHandler, SLOT(horizontalScrolled(int)));
    connect(view->verticalScrollBar(), SIGNAL(valueChanged(int)),
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
//    pe->init();
//    RAddObjectOperation* oper = new RAddObjectOperation(QSharedPointer<RObject>(pe));
//    di->applyOperation(*oper);

    RLineEntity* le = new RLineEntity(document, RLineData(RVector(0,0),
                                                          RVector(100,100)));
//    oper->addObject(QSharedPointer<RObject>(le));
    RAddObjectOperation* oper = new RAddObjectOperation(QSharedPointer<RObject>(le));
    di->applyOperation(*oper);

//    RCircleEntity* ce = new RCircleEntity(document, RCircleData(RVector(50,50),
//                                                                50.0), 648213795);
//    oper->addObject(QSharedPointer<RObject>(ce));
//    RAddObjectOperation* oper = new RAddObjectOperation(QSharedPointer<RObject>(ce));
//    di->applyOperation(*oper);

//    RArcEntity* ae = new RArcEntity(document, RArcData(RVector(50,50),
//                                                       25.0, 0.0, 90.0));
//    oper->addObject(QSharedPointer<RObject>(ae));
//    oper = new RAddObjectOperation(QSharedPointer<RObject>(ae));
//    di->applyOperation(*oper);


}
