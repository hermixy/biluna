/*******************************************************************************
 * $Id: db_calendarwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
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

#include <QtCore>
#include <QtWidgets>

#include "db_actionsystemdatabaseconnect.h"
#include "db_calendarwidget.h"
#include "db_calendarview.h"
#include "db_modelfactory.h"
#include "rb_cmbdelegate.h"
#include "rb_datawidgetmapper.h"


/**
 * Constructor
 */
DB_CalendarWidget::DB_CalendarWidget(QWidget *parent) : RB_Widget(parent) {

    setupUi(this);
    setMinimumSize(600, 480);

    mColors << QColor(56, 128, 189)
    << QColor(249, 162, 57)
    << QColor(0, 139, 70)
    << QColor(237, 19, 93)
    << QColor(165, 93, 38)
    << QColor(239, 71, 63)
    << QColor(132, 199, 112)
    << QColor(0, 90, 157);

    QListIterator<QColor> i(mColors);

    while (i.hasNext()) {
        QColor color = i.next();
        QPixmap pixmap(24, 24);
        pixmap.fill(color);
        QIcon icon;
        icon.addPixmap(pixmap);
        mIcons.append(icon);
    }

    mActivityModel = NULL;
    mActivityMapper = NULL;

    mIsOutOfSync = false;
}

/**
 * Destructor
 */
DB_CalendarWidget::~DB_CalendarWidget() {
    delete mActivityModel;

    int count = mCalendars.count();

    for (int i = 0; i < count; ++i) {
        DB_Calendar* cal = mCalendars.at(i);
        delete cal;
    }

    RB_DEBUG->print("DB_CalendarWidget::~DB_CalendarWidget() OK");
}

void DB_CalendarWidget::generateData() {
    // Holiday calendar
/*
    DB_Calendar *cal = new DB_Calendar(0);
    cal->setName("Holiday DB_Calendar");
    cal->setColor(mColors.at(0));
    cal->setSelected(false);
    mCalendars.append(cal);

    DB_Activity *app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,23),QTime(19,0,0)),
                     QDateTime(QDate(2008,12,23),QTime(23,0,0)));

    app->setSubject("Holiday party!");
    app->setPlace("Secret place");
    app->setDescription("Secret holiday party at secret place. Everybody is welcome!");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,24),QTime(12,0,0)),
                     QDateTime(QDate(2008,12,24),QTime(23,59,0)));

    app->setSubject("Christmas party!");
    app->setPlace("Everywhere");
    app->setDescription("Everybody is welcome!");
    cal->insertActivity(app);

    // Work DB_Calendar

    cal = new DB_Calendar(1);
    cal->setName("Work DB_Calendar");
    cal->setColor(mColors.at(1));
    cal->setSelected(false);
    mCalendars.append(cal);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,22),QTime(9,0,0)),
                     QDateTime(QDate(2008,12,22),QTime(11,00,0)));
    app->setSubject("Meeting");
    app->setPlace("Somewhere");
    app->setDescription("Remember this meeting!");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,22),QTime(11,00,0)),
                     QDateTime(QDate(2008,12,22),QTime(12,30,0)));
    app->setSubject("Lunch with Boss");
    app->setPlace("Hilton");
    app->setDescription("Good food.");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,23),QTime(10,00,0)),
                     QDateTime(QDate(2008,12,23),QTime(11,00,0)));
    app->setSubject("Call to collegue");
    cal->insertActivity(app);

    // QuickCalendar

    cal = new DB_Calendar(2);
    cal->setName("QuickCalendar");
    cal->setColor(mColors.at(2));
    cal->setSelected(false);
    mCalendars.append(cal);

    //

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,22),QTime(8,0,0)),
                     QDateTime(QDate(2008,12,22),QTime(23,59,0)));
    app->setSubject("Rewrite QuickCalendar");
    app->setPlace("Home");
    app->setDescription("Rewrite QuickCalendar to better meet contest requirements.");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,23),QTime(8,0,0)),
                     QDateTime(QDate(2008,12,23),QTime(23,59,0)));
    app->setSubject("Finish QuickCalendar");
    app->setPlace("Home");
    app->setDescription("Make QuickCalendar as ready as possible!");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,27),QTime(10,0,0)),
                     QDateTime(QDate(2008,12,27),QTime(18,30,0)));
    app->setSubject("Design and implement new features");
    app->setPlace("Home");
    app->setDescription("Design and implement new features!");
    cal->insertActivity(app);

    // Personal DB_Calendar

    cal = new DB_Calendar(3);
    cal->setName("Personal calendar");
    cal->setColor(mColors.at(3));
    cal->setSelected(false);
    mCalendars.append(cal);

    //
    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,23),QTime(16,0,0)),
                     QDateTime(QDate(2008,12,23),QTime(19,0,0)));
    app->setSubject("Shopping");
    app->setPlace("Everywhere");
    app->setDescription("Buy some Christmas presents!");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,26),QTime(10,0,0)),
                     QDateTime(QDate(2008,12,26),QTime(23,59,0)));
    app->setSubject("Family meeting");
    app->setPlace("Pamis");
    app->setDescription("Do not miss this!");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,27),QTime(19,0,0)),
                     QDateTime(QDate(2008,12,27),QTime(23,59,0)));
    app->setSubject("Dining with friends");
    app->setPlace("Downtown");
    app->setDescription("");
    cal->insertActivity(app);

    app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,28),QTime(8,0,0)),
                     QDateTime(QDate(2008,12,28),QTime(23,59,0)));
    app->setSubject("Try to relax...");
    app->setPlace("Home");
    app->setDescription("Relax after \"hard\" week.");
    cal->insertActivity(app); */
}

/**
 * Initialize the widget
 */
void DB_CalendarWidget::init() {
    initCalendarMode();

    for (int month = 1; month <= 12; ++month) {
        cbMonth->addItem(QDate::longMonthName(month));
    }

    cbMonth->setMaxVisibleItems(12);
    initDate(QDate::currentDate());

    connect(cbMonth, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotMonthChanged(int)));
    connect(sbYear, SIGNAL(valueChanged(int)),
            this, SLOT(slotYearChanged(int)));

    connect(pbPreviousMonth, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(pbNextMonth, SIGNAL(clicked()), this, SLOT(slotNext()));

    connect(sbWeeks, SIGNAL(valueChanged(int)), this, SLOT(slotWeeksChanged(int)));

    connect(pbPreviousWeek, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(pbNextWeek, SIGNAL(clicked()), this, SLOT(slotNext()));

    connect(deFrom, SIGNAL(dateChanged(QDate)), this, SLOT(slotRangeDateChanged(QDate)));
    connect(deTo, SIGNAL(dateChanged(QDate)), this, SLOT(slotRangeDateChanged(QDate)));

    connect(pbPreviousRange, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(pbNextRange, SIGNAL(clicked()), this, SLOT(slotNext()));

    mActivitiesTab = tabWidget->widget(1);
    tabWidget->removeTab(1);

    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            this, SLOT(slotRootIsSet(int))); // only for RB2::ValidTrue
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            this, SLOT(slotDatabaseConnection(int))); // only for RB2::ValidFalse
    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotTabChanged(int)));

    DB_MODELFACTORY->emitState();
    readSettings();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool DB_CalendarWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mActivityModel) {
        if (mActivityModel->submitAllAndSelect()) {
            setWindowModified(false);
            mIsOutOfSync = true;
            return true;
        }
    } else if (mActivityModel) {
        if (mActivityModel->submitAll()) {
            setWindowModified(false);
            mIsOutOfSync = true;
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button connect to database clicked
 */
void DB_CalendarWidget::on_pbDatabase_clicked() {
    DB_ActionSystemDatabaseConnect::factory();
}

/**
 * Button add clicked
 */
void DB_CalendarWidget::on_pbAdd_clicked() {
    if (!mActivityModel) return;
    mActivityModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mActivityModel->rowCount();
    mActivityModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitytype_id"));
    mActivityModel->setData(idx, 4, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitycode"));
    mActivityModel->setData(idx, "<NEW>", Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("priority_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("status_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("parent"));
    mActivityModel->setData(idx, DB_MODELFACTORY->getRootId(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mstart"));
    mActivityModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mend"));
    mActivityModel->setData(idx, QDateTime::currentDateTime().addSecs(3600), Qt::EditRole);
    // end NOTE

    tvActivity->setCurrentIndex(mActivityModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvActivity->scrollTo(tvActivity->currentIndex());
    cbActivityType->setFocus();
}

/**
 * Button delete clicked
 */
void DB_CalendarWidget::on_pbDelete_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete activity ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this activity?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvActivity->currentIndex();
    if (!index.isValid()) return;

    mActivityModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
    setWindowModified(false);
}

/**
 * Push button save clicked
 */
void DB_CalendarWidget::on_pbSave_clicked() {
    fileSave(true);
}

void DB_CalendarWidget::slotSelectStatusChanged(int index) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (index == 0) {
        mActivityModel->setWhere("", false); // not select
    } else {
        mActivityModel->setWhere("status_id > 0 AND status_id < 4", false); // not select
    }

    mActivityModel->select();

    if (!tvActivity->currentIndex().isValid()) {
        mActivityMapper->slotClearWidgets();
    }

    QApplication::restoreOverrideCursor();
}

/**
 * Start date time changed, update end date time when before start.
 */
void DB_CalendarWidget::slotUpdateEndDateTime() {
    QDateTime startDt = dteStart->dateTime();

    if (startDt > dteEnd->dateTime()) {
        dteEnd->setDateTime(startDt);
    }
}

/**
 * Add new activity based on start end date, triggered from calendar view
 * @param start start date time
 * @param end end date time
 */
void DB_CalendarWidget::addNewActivity(const QDateTime& start,
                                       const QDateTime& end) {
    if (tabWidget->count() < 2) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
        return;
    }

    tabWidget->setCurrentIndex(1);

    // Almost same as on_pbAdd_clicked()
    if (!mActivityModel) return;
    mActivityModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mActivityModel->rowCount();
    mActivityModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitytype_id"));
    mActivityModel->setData(idx, 4, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitycode"));
    mActivityModel->setData(idx, "<NEW>", Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("priority_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("status_id"));
    mActivityModel->setData(idx, 0, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("parent"));
    mActivityModel->setData(idx, DB_MODELFACTORY->getRootId(), Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mstart"));
    mActivityModel->setData(idx, start, Qt::EditRole);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mend"));
    mActivityModel->setData(idx, end, Qt::EditRole);
    // end NOTE

    tvActivity->setCurrentIndex(mActivityModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvActivity->scrollTo(tvActivity->currentIndex());
    leCode->setFocus();
    leCode->selectAll();
}

/**
 * Activate activity in table view based on ID
 * @param id ID
 */
void DB_CalendarWidget::selectActivity(const RB_String& id, bool isEdit) {
    int rowCount = mActivityModel->rowCount();
    QModelIndex idx;

    for (int row = 0; row < rowCount; ++row) {
        idx = mActivityModel->index(row, 0, QModelIndex());

        if (idx.data().toString() == id) {
            if (isEdit) tabWidget->setCurrentIndex(1);
            idx = mActivityModel->index(row, mActivityModel->fieldIndex("subject"));
            tvActivity->setCurrentIndex(idx);
            row = rowCount;
        }
    }
}

/**
 * Initialize the combobox with calendar view mode
 */
void DB_CalendarWidget::initCalendarMode() {
    cbCalendarMode->addItem(tr("By Month"));
    cbCalendarMode->addItem(tr("By Week"));
    cbCalendarMode->addItem(tr("By Range"));
    cbCalendarMode->setCurrentIndex(0);

    connect(cbCalendarMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCalendarMode(int)));
}

void DB_CalendarWidget::initDate(const QDate &date) {
    mDate = date;

    mCalendarView->setCalendars(&mCalendars);
    mCalendarView->setMonth(mDate.year(), mDate.month());

    cbMonth->setCurrentIndex(mDate.month() - 1);
    sbYear->setValue(mDate.year());
    sbWeeks->setValue(3);
    deFrom->setDate(mDate.addDays(-1));
    deTo->setDate(mDate.addDays(1));

}

void DB_CalendarWidget::setDate(const QDate &date) {
    mDate = date;

    if(cbCalendarMode->currentIndex() == 0) { // month
        cbMonth->setCurrentIndex(mDate.month() - 1);
        sbYear->setValue(mDate.year());
        mCalendarView->setMonth(mDate.year(), mDate.month());
    } else if(cbCalendarMode->currentIndex() == 1) { // weeks
        mCalendarView->setRange(mDate, mDate.addDays((7 * sbWeeks->value()) - 1));
    } else { // range
        mCalendarView->setRange(deFrom->date(), deTo->date());
    }
}

/**
 * Set the model and view
 */
void DB_CalendarWidget::setModelView() {
    RB_StringList items;
    items << tr("All") << tr("Active only");
    cbSelectStatus->addItems(items);
    connect(cbSelectStatus, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelectStatusChanged(int)));

    //
    // 1. Set model for customer mapper
    //
    mActivityModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelActivity); // shared
    mActivityModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mActivityMapper = mActivityModel->getMapper();

    RB_StringList typeItems;
    typeItems << tr("Call") << tr("Case") << tr("Email") << tr("Log")
             << tr("Meeting") << tr("Note") << tr("Opportunity")
             << tr("Reminder") << tr("Solution") << tr("Task");
    mActivityModel->setTextList(mActivityModel->fieldIndex("activitytype_id"), typeItems);
    cbActivityType->setModel(new QStringListModel(typeItems, this));
    mActivityMapper->addMapping(cbActivityType, mActivityModel->fieldIndex("activitytype_id"),
                                "currentIndex");
    mActivityMapper->addMapping(leCode, mActivityModel->fieldIndex("activitycode"));
    mActivityMapper->addMapping(leSubject, mActivityModel->fieldIndex("subject"));
    mActivityMapper->addMapping(dteStart, mActivityModel->fieldIndex("mstart"));
    mActivityMapper->addMapping(dteEnd, mActivityModel->fieldIndex("mend"));
    mActivityMapper->addMapping(chbReminder, mActivityModel->fieldIndex("isreminder"));
    mActivityMapper->addMapping(leReminder, mActivityModel->fieldIndex("remindertime"));
    RB_StringList prioItems;
    prioItems << tr("None") << tr("Low") << tr("Medium") << tr("High");
    mActivityModel->setTextList(mActivityModel->fieldIndex("priority_id"), prioItems);
    cbPriority->setModel(new QStringListModel(prioItems, this));
    mActivityMapper->addMapping(cbPriority, mActivityModel->fieldIndex("priority_id"),
                                "currentIndex");
    mActivityMapper->addMapping(pteDescription, mActivityModel->fieldIndex("description"));
    RB_StringList statusItems;
    statusItems << tr("None") << tr("Not started") << tr("In progress") << tr("Pending input")
              << tr("Completed") << tr("Deferred");
    mActivityModel->setTextList(mActivityModel->fieldIndex("status_id"), statusItems);
    cbStatus->setModel(new QStringListModel(statusItems, this));
    mActivityMapper->addMapping(cbStatus, mActivityModel->fieldIndex("status_id"),
                                "currentIndex");
    mActivityMapper->addMapping(leLocation, mActivityModel->fieldIndex("location"));
    mActivityMapper->addMapping(leValue, mActivityModel->fieldIndex("mvalue"));

    // Align and validator
    leReminder->setAlignment(Qt::AlignRight);
    leReminder->setValidator(new QIntValidator(0, 999999, this));
    leValue->setAlignment(Qt::AlignRight);
    leValue->setValidator(new QDoubleValidator(0.0, 9999999999.99, 2, this));

    //
    // 3. Select after relations have been set, only for database models
    //
    mActivityModel->select();
    //    mActivityModel->setHeaderData(0, Qt::Horizontal, tr("Code"));
    //    mActivityModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvActivity, mActivityModel);
    tvActivity->setItemDelegateForColumn(mActivityModel->fieldIndex("activitytype_id"),
                                         new RB_CmbDelegate(this, typeItems, mActivityModel));
    tvActivity->setItemDelegateForColumn(mActivityModel->fieldIndex("priority_id"),
                                         new RB_CmbDelegate(this, prioItems, mActivityModel));
    tvActivity->setItemDelegateForColumn(mActivityModel->fieldIndex("status_id"),
                                         new RB_CmbDelegate(this, statusItems, mActivityModel));

    readSettings();
    pteDescription->setTabChangesFocus(true);
    connect(dteStart, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateEndDateTime()));
    connect (mActivityModel, SIGNAL(modelBeforeSubmitted()),
             this, SLOT(slotDataChanged()));

    int count = mActivityModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mActivityModel->fieldIndex("activitytype_id")
                && i != mActivityModel->fieldIndex("activitycode")
                && i != mActivityModel->fieldIndex("subject")
                && i != mActivityModel->fieldIndex("mstart")
                && i != mActivityModel->fieldIndex("mend")
                && i != mActivityModel->fieldIndex("priority_id")
                && i != mActivityModel->fieldIndex("status_id")
                && i != mActivityModel->fieldIndex("mvalue")) {
            tvActivity->hideColumn(i);
        } else {
            tvActivity->showColumn(i);

            if (tvActivity->columnWidth(i) < 5) {
                tvActivity->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Synchronize the activities between the table view (model)
 * and the calendar view
 */
void DB_CalendarWidget::syncActivities() {
    if (!mActivityModel) {
        RB_DEBUG->error("DB_CalendarWidget::syncActivities() model NULL ERROR");
    }

    DB_Calendar* cal = NULL;

    // create DB calendar
    if (mCalendars.count() == 0) {
        cal = new DB_Calendar("DB");
        RB_ObjectContainer* actList = new RB_ObjectContainer("0", NULL, "DB_ActivityList");
        cal->addObject(actList);
        cal->setName("General DB Calendar");
        cal->setColor(mColors.at(0));
        cal->setSelected(true); // if false: will not show activities
        mCalendars.append(cal);
    } else {
        cal = mCalendars.at(0);
        RB_ObjectContainer* actList = cal->getContainer("DB_ActivityList");
        actList->erase();
    }


    DB_Activity* act = NULL;

    QModelIndex idx;
    int count = mActivityModel->rowCount();

    for (int row = 0; row < count; ++row) {
        act = new DB_Activity(); // Graphics item

        idx = mActivityModel->index(row, mActivityModel->fieldIndex("id"));
        act->setId(idx.data().toString());

        idx = mActivityModel->index(row, mActivityModel->fieldIndex("mstart"));
        QDateTime start = idx.data().toDateTime();
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("mend"));
        QDateTime end = idx.data().toDateTime();
        act->setDateTimes(start, end);

        idx = mActivityModel->index(row, mActivityModel->fieldIndex("subject"));
        act->setSubject(idx.data().toString());
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("location"));
        act->setPlace(idx.data().toString());
        idx = mActivityModel->index(row, mActivityModel->fieldIndex("description"));
        act->setDescription(idx.data().toString());

        cal->insertActivity(act);
    }

    mCalendarView->setCalendars(&mCalendars);
//    slotDataChanged(); done with tabChanged
}

/**
 * Activity data changed, refresh calendar view
 */
void DB_CalendarWidget::slotDataChanged() {
    mCalendarView->dataChanged();
    mCalendarView->layoutChanged();
}

/**
* @param mode full week or range only
*/
void DB_CalendarWidget::modeChanged(int mode) {
    if(mode == 0) {
        mCalendarView->setDisplayMode(DB_CalendarView::DisplayFullWeeks);
    } else {
        mCalendarView->setDisplayMode(DB_CalendarView::DisplayOnlyRange);
    }
}

void DB_CalendarWidget::slotCalendarMode(int index) {
    switch (index) {
    case 0: {
        stkdWidget->setCurrentIndex(index);
        cbMonth->setCurrentIndex(mDate.month() - 1);
        sbYear->setValue(mDate.year());

        modeChanged(0);
        mCalendarView->setMonth(mDate.year(), mDate.month());
        break;
    }
    case 1: {
        stkdWidget->setCurrentIndex(index);
        QDate date = mDate.addDays(-(mDate.dayOfWeek()-1));

        modeChanged(0);
        mCalendarView->setRange(date, date.addDays((7*sbWeeks->value())-1));
        break;
    }
    case 2: {
        stkdWidget->setCurrentIndex(index);
        int delta = deFrom->date().daysTo(deTo->date());
        deFrom->setDate(mDate);
        deTo->setDate(mDate.addDays(delta));

        modeChanged(1);
        mCalendarView->setRange(deFrom->date(), deTo->date());
        break;
    }
    default:
        break;
    }
}

void DB_CalendarWidget::slotMonthChanged(int index) {
    if(index + 1 != mDate.month()) {
        mDate.setDate(mDate.year(), index + 1, 1);
        mCalendarView->setMonth(mDate.year(), mDate.month());
    }
}

void DB_CalendarWidget::slotYearChanged(int year) {
    if(year != mDate.year()) {
        mDate.setDate(year, mDate.month(), 1);
        mCalendarView->setMonth(mDate.year(), mDate.month());
    }
}

void DB_CalendarWidget::slotPrevious() {
    if(cbCalendarMode->currentIndex() == 0) {
        if(mDate.month() == 1) {
            mDate.setDate(mDate.year()-1, 12, 1);
        } else {
            mDate.setDate(mDate.year(), mDate.month()-1, 1);
        }

        cbMonth->setCurrentIndex(mDate.month() - 1);
        sbYear->setValue(mDate.year());
        mCalendarView->setMonth(mDate.year(), mDate.month());
    } else if(cbCalendarMode->currentIndex() == 1) {
        mDate = mDate.addDays(-7);
        mCalendarView->setRange(mDate, mDate.addDays(7 * (sbWeeks->value() - 1)));
    } else if(cbCalendarMode->currentIndex() == 2) {
        mDate = mDate.addDays(-1);
        int delta = deFrom->date().daysTo(deTo->date());
        deFrom->setDate(mDate);
        deTo->setDate(mDate.addDays(delta));
        mCalendarView->setRange(deFrom->date(), deTo->date());
    }
}

void DB_CalendarWidget::slotNext() {
    if(cbCalendarMode->currentIndex() == 0) {
        if(mDate.month() == 12) {
            mDate.setDate(mDate.year()+1, 1, 1);
        } else {
            mDate.setDate(mDate.year(), mDate.month()+1, 1);
        }
        cbMonth->setCurrentIndex(mDate.month() - 1);
        sbYear->setValue(mDate.year());
        mCalendarView->setMonth(mDate.year(), mDate.month());
    } else if(cbCalendarMode->currentIndex() == 1) {
        mDate = mDate.addDays(7);
        mCalendarView->setRange(mDate, mDate.addDays(7 * (sbWeeks->value() - 1)));
    } else if(cbCalendarMode->currentIndex() == 2) {
        mDate = mDate.addDays(1);
        int delta = deFrom->date().daysTo(deTo->date());
        deFrom->setDate(mDate);
        deTo->setDate(mDate.addDays(delta));
        mCalendarView->setRange(deFrom->date(), deTo->date());
    }
}

void DB_CalendarWidget::slotWeeksChanged(int weeks) {
    if (cbCalendarMode->currentIndex() == 1) {
        QDate date = mDate.addDays(-(mDate.dayOfWeek()-1));
        mCalendarView->setRange(date, date.addDays((7*weeks)-1));
    }
}

void DB_CalendarWidget::slotRangeDateChanged(const QDate& /*date*/) {
    if(deTo->date() < deFrom->date()) {
        deTo->setDate(deFrom->date());
    } else if (deFrom->date().daysTo(deTo->date()) > 14) {
        if (sender() == deFrom) {
            deTo->setDate(deFrom->date().addDays(14));
        } else {
            deFrom->setDate(deTo->date().addDays(-14));
        }
    }

    mDate = deFrom->date();
    mCalendarView->setRange(mDate, deTo->date());
}

/**
 * Add activity tab and based on valid root id
 * @param value RB2::ValidTrue root is valid and add activities tab
 */
void DB_CalendarWidget::slotRootIsSet(int value) {
    if (value == (int)RB2::ValidTrue) {
        if (tabWidget->count() < 2) {
            tabWidget->addTab(mActivitiesTab, tr("Activities"));
            setModelView();
        }

        syncActivities();
        slotDataChanged();
        mIsOutOfSync = false;

    }
}

/**
 * Remove activity tab based on valid database connection
 * @param value RB2::ValidFalse database connection false,
  *  remove activities and tab
 */
void DB_CalendarWidget::slotDatabaseConnection(int value) {
    if (value == (int)RB2::ValidFalse) {
        if (mActivityModel) {

            // Delete root first because database is already closed
            // otherwise memory leak
            if (mActivityModel->getRoot()) {
                delete mActivityModel->getRoot();
                mActivityModel->setRoot(NULL);
            }

            delete mActivityModel;
            mActivityModel = NULL;

            // delete activities from calendar
            qDeleteAll(mCalendars);
            mCalendars.clear();
            mCalendarView->dataChanged();
            mIsOutOfSync = false;
        }

        tabWidget->setCurrentIndex(0);
        tabWidget->removeTab(1);
    }
}

/**
 * Tab selection changed. Will set model and view for activities
 * @param index index of tab
 */
void DB_CalendarWidget::slotTabChanged(int index) {
    if (index == 0 && mIsOutOfSync) {
        syncActivities();
        mIsOutOfSync = false;
        // mCalendarView->dataChanged();
        slotDataChanged();
    }
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String DB_CalendarWidget::getHelpSubject() const {
    return objectName();
}

bool DB_CalendarWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void DB_CalendarWidget::changeEvent(QEvent *e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
