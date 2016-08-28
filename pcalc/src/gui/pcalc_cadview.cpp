/*****************************************************************
 * $Id: pcalc_cadview.cpp 2241 2016-08-24 10:22:19Z rutger $
 * Created: Aug 24, 2016 - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_cadview.h"

#include "rs_layer.h"
#include "rs_pen.h"


PCALC_CadView::PCALC_CadView(QWidget* parent)
            : QG_GraphicView(parent) {
    initView();
    initLayers();
}

/**
 * @brief PCALC_CadView::initView Setup of view for direct rendering
 * of entities
 */
void PCALC_CadView::initView() {
    // from QCad main.cpp
    RS_SYSTEM->init(RB_SYSTEM->getAppName(), RB_SYSTEM->getAppVersion(),
                    RB_SYSTEM->getAppName(), RB_SYSTEM->getAppDir());
    RS_FILEIO->registerFilter(new RS_FilterCxf());
    RS_FILEIO->registerFilter(new RS_FilterDxf());
    RS_FILEIO->registerFilter(new RS_FilterDxf1());
    RS_FILEIO->registerFilter(new RB_FilterSvg());
    RS_FONTLIST->init();
    RS_PATTERNLIST->init();

    // from QG_MdiWindow::initDoc()
    RS_Graphic* graphic = new RS_Graphic();
    graphic->newDoc();

    // from QG_MdiWindow::initView()
    // QG_GraphicView* cadView = new QG_GraphicView(this);
    this->setContainer(graphic);
    double defaultZoom = 4.0;
    this->setFactor(defaultZoom);
    this->setOffset(50, 50);
    //this->setDefaultAction(new RS_ActionDefault(*document, *this));
}

void PCALC_CadView::initLayers() {
    this->setBackground(RS_Color(Qt::white));
    this->showRulers(false);

    // set default layers
    RS_Graphic* graphic = this->getGraphic();
    graphic->addLayer(new RS_Layer(
                          RS_LayerData("OUTLINE",
                                       RS_Pen(RS_Color(Qt::blue), RS2::Width09,
                                              RS2::SolidLine, 0.0),
                                       false , false)));
    // for example thread lines on stud bolt
    graphic->addLayer(new RS_Layer(
                          RS_LayerData("SECONDARYLINE",
                                       RS_Pen(RS_Color(Qt::black), RS2::Width07,
                                              RS2::SolidLine, 0.0),
                                       false , false)));
    graphic->addLayer(new RS_Layer(
                          RS_LayerData("DIMENSION",
                                       RS_Pen(RS_Color(Qt::black), RS2::Width07,
                                              RS2::SolidLine, 0.0),
                                       false, false)));
    graphic->addLayer(new RS_Layer(
                          RS_LayerData("SYMLINE",
                                       RS_Pen(RS_Color(Qt::cyan), RS2::Width11,
                                              RS2::DashDotLine, 0.0),
                                       false, false)));
    graphic->activateLayer("OUTLINE");
}

/**
 * @brief PCALC_CadView::addLine short
 * for addEntity(RS_LineData(RS_Vector, RS_Vector));
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void PCALC_CadView::addLine(double x1, double y1, double x2, double y2) {
    RS_Graphic* graphic = this->getGraphic();
    graphic->addEntity(new RS_Line(graphic,
                                   RS_LineData(RS_Vector(x1, y1),
                                               RS_Vector(x2, y2))));
}

void PCALC_CadView::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);

    switch (e->type()) {
    // Language change per each Ui widget
    //    case QEvent::LanguageChange:
    //        retranslateUi(this);
    //        break;
    case QEvent::PaletteChange: {
        // from RB_Widget::setPaletteColors() is mBaseColor in RB_Widget
        QColor color= QApplication::palette().color(QPalette::Active,
                                                    QPalette::Base);
        setBackground(RS_Color(color));
        break;
    }
    default:
        break;
    }
}

