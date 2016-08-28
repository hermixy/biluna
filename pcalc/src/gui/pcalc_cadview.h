/*****************************************************************
 * $Id: pcalc_cadview.h 2241 2016-08-24 10:22:19Z rutger $
 * Created: Aug 24, 2016 - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_CADVIEW_H
#define PCALC_CADVIEW_H

#include "qg_graphicview.h"

#include "rb_filtersvg.h"
#include "rb_system.h"
#include "rs_fileio.h"
#include "rs_filtercxf.h"
#include "rs_filterdxf.h"
#include "rs_filterdxf1.h"
#include "rs_fontlist.h"
#include "rs_graphic.h"
#include "rs_patternlist.h"
#include "rs_system.h"
#include "rs_text.h"

/**
 * @brief The PCALC_CadView class based on QG_GraphicView
 */
class PCALC_CadView : public QG_GraphicView {

    Q_OBJECT

public:
    explicit PCALC_CadView(QWidget* parent = 0);
    void addLine(double x1, double y1, double x2,double y2);


signals:

public slots:

protected:
    virtual void changeEvent(QEvent* e);

private:
    void initView();
    void initLayers();

};

#endif // PCALC_CADVIEW_H
