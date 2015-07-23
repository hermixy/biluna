/*****************************************************************
 * $Id: db_commandwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 11, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_COMMANDWIDGET_H
#define DB_COMMANDWIDGET_H

#include "db_dialogfactory.h"
#include "rb_widget.h"
#include "ui_db_commandwidget.h"


/**
 * Command widget
 */
class DB_EXPORT DB_CommandWidget: public RB_Widget, private Ui::DB_CommandWidget {

    Q_OBJECT

public:
    DB_CommandWidget(QWidget* parent = 0);
    virtual ~DB_CommandWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return tr("Command Line"); }
    virtual int getWidgetType() const { return (int)DB_DialogFactory::WidgetCommand; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init() { }

    bool checkFocus();
    virtual QSize sizeHint() const {
        return QSize(1024, 30);
    }

    virtual bool closeWidget() { return true; }

signals:
    void escape();

public slots:
    void setFocus();
    void setCommand(const QString& cmd);
    void appendHistory(const QString& msg);
    void trigger();
    void tabPressed();
    void slotEscape();
    void setCommandMode();
    void setNormalMode();
    void redirectStderr();
    void processStderr();
    QString getCommonStart(const QStringList& sl);

private:

};

#endif

