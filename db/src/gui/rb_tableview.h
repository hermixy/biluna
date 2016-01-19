/*****************************************************************
 * $Id: rb_tableview.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 24, 2011 10:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TABLEVIEW_H
#define RB_TABLEVIEW_H

#include <QTableView>
#include "db_global.h"

class RB_MmProxy;
class RB_ToolButtonBar;

/**
 * Class for tableviews connected to RB_ToolButtonBar
 */
class DB_EXPORT RB_TableView : public QTableView {

    Q_OBJECT

public:
    explicit RB_TableView(QWidget* parent = 0);

    virtual void setModel(RB_MmProxy* model);
    virtual void setToolButtonBar(RB_ToolButtonBar* tbb);
    virtual void removeToolButtonBar();

signals:

public slots:
    virtual void slotAdd();
    virtual void slotAddChild();
    virtual void slotDelete();
    virtual void slotEdit();
    virtual void slotCopy();
    virtual void slotSave();
    virtual void slotRevert();

    virtual void slotPrevious();
    virtual void slotNext();

    virtual void slotFind();
    virtual void slotFilterOn();
    virtual void slotFilterOff();

    virtual void slotDemote();
    virtual void slotPromote();
    virtual void slotUp();
    virtual void slotDown();

private:
    bool maybeSave();

    RB_ToolButtonBar* mToolButtonBar;
    RB_MmProxy* mModel;
    RB_MmProxy* mParentModel;

    bool mRestartedAtBegin;
};

#endif // RB_TABLEVIEW_H
