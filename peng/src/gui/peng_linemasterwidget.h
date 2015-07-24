/*****************************************************************
 * $Id: peng_linemasterwidget.h 1811 2012-12-03 20:33:28Z rutger $
 * Created: Apr 09, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_LINEMASTERWIDGET_H
#define PENG_LINEMASTERWIDGET_H

#include "rb_widget.h"
#include "ui_peng_linemasterwidget.h"

/**
 * Line master widget, part of the navigation dockwidget
 */
class PENG_LineMasterWidget : public RB_Widget,
                                    private Ui::PENG_LineMasterWidget {
    Q_OBJECT

public:
    PENG_LineMasterWidget(QWidget *parent = 0);
    virtual ~PENG_LineMasterWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Line Master[*]"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Line master model
    RB_MmProxy* mModel;

};

#endif // PENG_LINEMASTERWIDGET_H
