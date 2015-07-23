/*******************************************************************************
 * $Id: db_calendarwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Apr 3, 2013 10:00:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *
 * Originally by Jukka-Pekka Makela. 2008
 * Contact: jpm@mehteenit.net
 * The original of this file was part of QuickCalendar.
 ******************************************************************************/

#ifndef DB_CALENDARWIDGET_H
#define DB_CALENDARWIDGET_H

#include <QColor>
#include <QIcon>
#include <QList>
#include <QMainWindow>
#include <QTableView>
#include "db_calendarview.h"
#include "db_calendar.h"
#include "db_dialogfactory.h"
#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_calendarwidget.h"

class RB_MmProxy;


/**
@class DB_CalendarWidget
@brief Class implementing MDI window widget
*/
class DB_EXPORT DB_CalendarWidget : public RB_Widget, private Ui::DB_CalendarWidget {

    Q_OBJECT

public:
    explicit DB_CalendarWidget(QWidget* parent = 0);
    virtual ~DB_CalendarWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Calendar"; } // document name
    virtual int getWidgetType() const { return (int)DB_DialogFactory::WidgetCalendar; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    void init();
    virtual bool fileSave(bool withSelect);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

    virtual void addNewActivity(const QDateTime& start, const QDateTime& end);
    virtual void selectActivity(const RB_String& id, bool isEdit);

public slots:
    virtual void on_pbDatabase_clicked();
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void slotSelectStatusChanged(int index);

    virtual void slotUpdateEndDateTime();

protected:
    void changeEvent(QEvent* e);

private slots: 
    void slotDataChanged();
    void modeChanged(int mode);
    void slotCalendarMode(int index);

    void slotMonthChanged(int index);
    void slotYearChanged(int year);
    void slotPrevious();
    void slotNext();
    void slotWeeksChanged(int weeks);
    void slotRangeDateChanged(const QDate& date);

    void slotRootIsSet(int value);
    void slotDatabaseConnection(int value);
    void slotTabChanged(int index);

private:
    void generateData(); // not used

    void initCalendarMode();
    void initDate(const QDate& date);
    void setDate(const QDate& date);

    void setModelView();
    void syncActivities();

    QList<QColor> mColors;
    QList<QIcon> mIcons;

    // DB_CalendarView *mCalendarView; part of .ui file
    QList<DB_Calendar*> mCalendars;
    QDate mDate;

    // if no connection to database, remove tab
    QWidget* mActivitiesTab;

    //! Activity data model
    RB_MmProxy* mActivityModel;
    //! Activity data widget mapper
    RB_DataWidgetMapper* mActivityMapper;

    //! Flag indicating that model and calendar view are out of sync
    bool mIsOutOfSync;
};

#endif
