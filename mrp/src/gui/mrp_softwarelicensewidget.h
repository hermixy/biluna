/*****************************************************************
 * $Id: sail_mapnavigationwidget.h 1729 2012-08-08 20:48:52Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SOFTWARELICENSEWIDGET_H
#define MRP_SOFTWARELICENSEWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_mrp_softwarelicensewidget.h"


/**
 * Widget to set tracks based on coordinates
 */
class MRP_SoftwareLicenseWidget : public RB_Widget, private Ui::MRP_SoftwareLicenseWidget {

    Q_OBJECT
    
public:
    explicit MRP_SoftwareLicenseWidget(QWidget* parent = 0);
    virtual ~MRP_SoftwareLicenseWidget();
    
    virtual RB_String getName() const { return "MRP Software License"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveMRP; }

    virtual void init();
    virtual bool fileSave(bool withSelect);

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void slotPbAddClicked();
    virtual void on_ileCustomer_clicked();
    virtual void on_ileSalesOrder_clicked();
    virtual void on_ileContact_clicked();

protected:
    virtual void changeEvent(QEvent* e);

private:
    //! License data model
    RB_MmProxy* mModel;
    //! License data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // MRP_SOFTWARELICENSEWIDGET_H
