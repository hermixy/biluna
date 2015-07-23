/*******************************************************************************
 * $Id: db_calendarview.cpp 2198 2014-11-17 21:34:16Z rutger $
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

#include "db_calendarview.h"
#include "db_calendarwidget.h"
#include "db_dayitem.h"
#include "db_weekdayheaderitem.h"
#include "rb_mdiwindow.h"

DB_CalendarView::DB_CalendarView(QWidget *parent)
                : QGraphicsView(new QGraphicsScene(), parent) {
    mStyle = new DB_CalendarStyle();
    mDisplayMode = DisplayFullWeeks;

    mContentPane = new DB_CalendarItem();
    scene()->addItem(mContentPane);

    mHeader = new DB_CalendarItem();
    
    scene()->addItem(mHeader);
    scene()->setBackgroundBrush(mStyle->pastDayGradient);

    // Create headers 7mi days of the week
    for (int i = 1; i <= 7; i++) {
        /*DB_WeekDayHeaderItem* item = */new DB_WeekDayHeaderItem(this, i, mHeader);
    }

    mExpandedDayOfWeek = 0;
    mExpandedWeekItem = 0;
    mExpandedDate = QDate(0,0,0);
    mWeekCount = 0;

    mCalendars = NULL; // new QList<DB_Calendar*>();
    mBottomLine = NULL;
    mLeftScrollLine = NULL;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame); 

    // View will be redrawn in bounding rectangle based on all changes in the viewing area.
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

/**
 * Destructor
 */
DB_CalendarView::~DB_CalendarView() {

}



/**
 *	@param mode
 */
void DB_CalendarView::setDisplayMode(DisplayMode mode) {
    if(mode != mDisplayMode) {
        mDisplayMode = mode;
        setRange(mRangeStart, mRangeEnd);
    }
}

/**
 *	@param start
 *	@param end
 */
void DB_CalendarView::setRange(const QDate& start, const QDate& end) {
    collapseAll();
    //qDeleteAll(mWeeks);

    if (mWeeks.count() > 0) {
      qDeleteAll(mWeeks);
      mWeeks.clear();
    }

    mRangeStart = start;
    mRangeEnd = end;

    if (mRangeEnd < mRangeStart) {
        mRangeEnd = mRangeStart;
    }

    // if the difference between the start and the end of more than 10 weeks
    if (mRangeStart.daysTo(mRangeEnd) > (7 * 10) - 1) {
        mRangeEnd = mRangeStart.addDays((7 * 10) - 1);
    }

    if (mDisplayMode == DisplayFullWeeks) {
        mStartDate = mRangeStart.addDays(-(mRangeStart.dayOfWeek() - 1));
        mEndDate = mRangeEnd.addDays(7-mRangeEnd.dayOfWeek());

        mWeekCount = (mStartDate.daysTo(mEndDate) + 1) / 7;

        // create a week
        for (int i = 0; i < mWeekCount; i++) {
            DB_WeekItem *week;

            if (i == 0) {
                week = new DB_WeekItem(this, mRangeStart, mRangeEnd, mContentPane);
            } else {
                week = new DB_WeekItem(this, mStartDate.addDays(i*7), mRangeEnd, mContentPane);
            }

            mWeeks.append(week);
        }

        dataChanged();

        // if you see one week
        if(mWeekCount == 1) {
            expandWeek(mWeeks.at(0));
        } else {
            layoutChanged();
        }

        // HACK: draw bottom border line and left scrollbar line
        if (!mBottomLine) {
            QRectF rect = sceneRect();
            mBottomLine = scene()->addLine(0, rect.height()-1, rect.width(), rect.height()-1);
            mBottomLine->setPen(style()->shadowColor);
        }
        if (!mLeftScrollLine) {
            QRectF rect = sceneRect();
            mLeftScrollLine = scene()->addLine(rect.width() - 30, 0,
                                               rect.width() - 30, rect.height()-1);
            mLeftScrollLine->setPen(style()->shadowColor);
        }
    } else if (mDisplayMode == DisplayOnlyRange) {
        mStartDate = mRangeStart;

        // if the difference between the start date and the date of the end of display > 13
        if(mRangeStart.daysTo(mRangeEnd) > 13) {
            mRangeEnd = mRangeStart.addDays(13);
        }

        mEndDate = mRangeEnd;

        DB_WeekItem *week = new DB_WeekItem(this, mStartDate, mEndDate, mContentPane);
        mWeeks.append(week);

        mWeekCount = 1;
        dataChanged();
        expandWeek(week);
    }
}

/**
 *	@param year
 *	@param month
 */
void DB_CalendarView::setMonth(int year, int month) {
    QDate start(year, month, 1);
    QDate end = start.addDays(start.daysInMonth() - 1);
    setRange(start, end);
}

/**
 *	@param calendars
 */
void DB_CalendarView::setCalendars(QList<DB_Calendar*>* calendars) {
    mCalendars = calendars;
}

/**
 *	@param date
 */
void DB_CalendarView::expandDate(const QDate& date) {
    bool weekChanged = false;

    if (date != mExpandedDate) {
        QListIterator<DB_WeekItem*> i(mWeeks);

        while (i.hasNext()) {
            DB_WeekItem* week = i.next();
            int dayOfWeek = week->dayOfWeek(date);

            if (dayOfWeek >= 0) {
                if(mExpandedWeekItem != week) {
                    weekChanged = true;
                }

                mExpandedWeekItem = week;
                mExpandedDayOfWeek = dayOfWeek;
                mExpandedDate = date;
                i.toBack();

            }
        }
    } else {
        mExpandedWeekItem = 0;
        mExpandedDayOfWeek = 0;
        mExpandedDate = QDate();
    }

    layoutChanged();

    // scroll area
    if (mExpandedWeekItem != 0 && weekChanged) {
        mExpandedWeekItem->mScrollArea->scrollTo(mStyle->quarterHeight * 4 * 7);
    }
}

void DB_CalendarView::expandWeekDay(int weekNumber, int dayOfWeek) {
    if (!mExpandedDate.isValid()) {
        QListIterator<DB_WeekItem*> weekIter(mWeeks);

        while (weekIter.hasNext()) {
            DB_WeekItem* week = weekIter.next();

            if (week->getStartDate().weekNumber() ==  weekNumber) {
                QListIterator<DB_DayItem*> dayIter(week->getDayItems());

                while (dayIter.hasNext()) {
                    DB_DayItem* day = dayIter.next();

                    if (day->getDate().dayOfWeek() == dayOfWeek) {
                        expandDate(day->getDate());
                        return;
                    }
                }
            }
        }
    }
}

/**
 * @param number
 */
void DB_CalendarView::expandWeek(int number) {
    QListIterator <DB_WeekItem*> i(mWeeks);

    while(i.hasNext()) {
        DB_WeekItem *week = i.next();

        if (week->mDate.weekNumber() == number) {
            expandWeek(week);
            i.toBack();
        }
    }
}

/**
 *	@param week
 */
void DB_CalendarView::expandWeek(DB_WeekItem *week) {
    if (week != mExpandedWeekItem) {
        mExpandedWeekItem = week;
    } else {
        if(mWeeks.count() > 1) {
            mExpandedWeekItem = 0;
            mExpandedDayOfWeek = 0;
            mExpandedDate = QDate(0,0,0);
        }
    }

    layoutChanged();

    if (mExpandedWeekItem != 0) {
        mExpandedWeekItem->mScrollArea->scrollTo(mStyle->quarterHeight * 4 * 7);
    }
}

/**
 * @return Number of the week unfolded. If no week is not deployed, return 0.
 */
int DB_CalendarView::expandedWeekNumber() const {
    if (mExpandedWeekItem != 0) {
        return mExpandedWeekItem->mDate.weekNumber();
    } else {
        return 0;
    }
}

void DB_CalendarView::collapseAll() {
    if (mExpandedWeekItem != 0) {
        if(mWeeks.count() > 1) {
            mExpandedWeekItem = 0;
            mExpandedDayOfWeek = 0;
            mExpandedDate = QDate(0,0,0);
            layoutChanged();
        }
    }
}

/**
 * @param event
 */
void DB_CalendarView::resizeEvent(QResizeEvent* /*event*/) {
    layoutChanged();
}

void DB_CalendarView::layoutChanged() {
    if(mWeeks.count() == 0) {
        return;
    }

    int width = this->width();
    int height = this->height();


    setSceneRect(0,0,width,height);

    // Calculate the width of the work area
    int workAreaWidth = width - mStyle->weekLeftMargin - mStyle->weekRightMargin;

    for (int i = 0; i < 21; i++) {
        mDayWidths[i] = 0;
    }

    int dayCount;
    int firstDay = 0;

    if (mDisplayMode == DisplayFullWeeks) {
        dayCount = 7;
    } else if(mDisplayMode == DisplayOnlyRange) {
        firstDay = mStartDate.dayOfWeek() - 1;
        dayCount = mStartDate.daysTo(mEndDate) + 1;
    }

    if (dayCount > 1) {
        if (mExpandedDayOfWeek != 0) {
            int expandedDayWidth = (workAreaWidth / 3) * 2;
            workAreaWidth -= expandedDayWidth;
            int dayWidth = workAreaWidth / (dayCount - 1);
            int mod = workAreaWidth % (dayCount - 1);

            for (int i = firstDay; i < firstDay + dayCount; i++) {
                if(i == firstDay + mExpandedDayOfWeek - 1) {
                    mDayWidths[i] = expandedDayWidth + mod;
                } else {
                    mDayWidths[i] = dayWidth;
                }
            }
        } else {
            int dayWidth = workAreaWidth / dayCount;
            int mod = workAreaWidth % dayCount;

            for (int i = firstDay; i < firstDay + dayCount; i++) {
                mDayWidths[i] = dayWidth;
            }

            mDayWidths[firstDay] += mod;
        }
    } else {
        // Width of the first day is equal to the entire working area
        mDayWidths[firstDay] = workAreaWidth;
    }

//    for(int i=0;i<21;i++)
//    {
//        qDebug(qPrintable("W: [" + QString::number(i) + "] = " + QString::number(mDayWidths[i])));
//    }

    int weekTop = 0;
    int weekHeight = 0;
    int expandedWeekHeight = 0;

    if (mWeekCount > 1) {
        int headerLeft = 0;
        int headerCount = 0;
        int headerHeight = 40 + (height - 40)%mWeeks.count();

        mHeader->setPos(mStyle->weekLeftMargin, 0);
        mHeader->setSize(width, headerHeight);
        mHeader->setVisible(true);

        QListIterator<QGraphicsItem*> i(mHeader->childItems());

        while (i.hasNext()) {
            DB_WeekDayHeaderItem *item = (DB_WeekDayHeaderItem *)i.next();
            item->setPos(headerLeft, 0);
            item->setSize(mDayWidths[headerCount], headerHeight);
            item->layoutChanged();

            headerLeft += mDayWidths[headerCount];
            headerCount++;
        }

        weekTop = headerHeight;
    } else {
        mHeader->setVisible(false);
    }

    if (mExpandedWeekItem != 0) {
        expandedWeekHeight = (height - weekTop)
                - ((mWeekCount - 1) * mStyle->collapsedWeekHeight());
        weekHeight = mStyle->collapsedWeekHeight();
    } else {
        // We calculate the height of all weeks
        weekHeight = (height - weekTop) / mWeeks.count();
    }

    for (int i = 0; i < mWeeks.count(); i++) {
        DB_WeekItem *week = mWeeks.at(i);

        week->setPos(0, weekTop);

        if (week == mExpandedWeekItem) {
            week->setSize(width, expandedWeekHeight);
            weekTop += expandedWeekHeight;
        } else {
            week->setSize(width, weekHeight);
            weekTop += weekHeight;
        }

        week->layoutChanged();
    }

    // HACK: to draw bottom black line
    if (mBottomLine) {
        mBottomLine->setLine(0, height-1, width, height-1);
        mBottomLine->setPen(style()->shadowColor);
    }
    if (mLeftScrollLine) {
        mLeftScrollLine->setLine(width - mStyle->weekRightMargin, 0,
                                 width - mStyle->weekRightMargin, height-1);
        mLeftScrollLine->setPen(style()->shadowColor);
    }
}

void DB_CalendarView::createNewActivity(const QDateTime &start, const QDateTime &end) {
    // HACK: to be refactored
    RB_MdiWindow* mdiW = DB_DIALOGFACTORY->getMdiWindow(DB_DialogFactory::WidgetCalendar);
    DB_CalendarWidget* calWdgt = dynamic_cast<DB_CalendarWidget*>(mdiW->getWidget());
    if(!calWdgt)  return;

    calWdgt->addNewActivity(start, end);
}

/**
 * Edit activity in table view, after double clicking of activity
 * in calendar view
 * @param activity activity clicked
 */
void DB_CalendarView::editActivity(DB_Activity* activity) {
    if (!activity) return;

    // HACK: to be refactored
    RB_MdiWindow* mdiW = DB_DIALOGFACTORY->getMdiWindow(DB_DialogFactory::WidgetCalendar);
    DB_CalendarWidget* calWdgt = dynamic_cast<DB_CalendarWidget*>(mdiW->getWidget());
    if(!calWdgt)  return;

    calWdgt->selectActivity(activity->getId(), true);
}

/**
 * Activate activity in table view, after double clicking of activity
 * in calendar view
 * @param activity activity clicked
 */
void DB_CalendarView::selectActivity(DB_Activity* activity) {
    if (!activity) return;

    // HACK: to be refactored
    RB_MdiWindow* mdiW = DB_DIALOGFACTORY->getMdiWindow(DB_DialogFactory::WidgetCalendar);
    DB_CalendarWidget* calWdgt = dynamic_cast<DB_CalendarWidget*>(mdiW->getWidget());
    if(!calWdgt)  return;

    calWdgt->selectActivity(activity->getId(), false);
}

/**
 *	@param appointment
 */
//void DB_CalendarView::showActivityForm(DB_Activity* appointment) {
//    bool someSelected = false;

//    for (int i = 0; i < mCalendars->count(); i++) {
//        if (mCalendars->at(i)->isSelected()) {
//            someSelected = true;
//        }
//    }

//    if (!someSelected) {
//        QMessageBox::warning(0, QObject::tr("DB_Calendar error"),
//                             QObject::tr("Select at least one calendar first!"));
//        return;
//    }

//    if (mCalendars->count() > 0) {
//        DB_ActivityDialog *form =
//                new DB_ActivityDialog(appointment, mCalendars);

//        // Connect a signal to a slot, its closure
//        connect(form, SIGNAL(onClose(DB_Activity*)),
//                this, SLOT(onFormClosed(DB_Activity*)));

//        // TODO:
//        if (appointment->getValue("parent").toString() == "0") {
//            form->deleteActivityButton->hide();
//        }

//        form->setGeometry(300, 300, 400, 250);
//        form->show();
//    }
//}

void DB_CalendarView::dataChanged() {
    QListIterator<DB_WeekItem*> i(mWeeks);

    while (i.hasNext()) {
        DB_WeekItem *week = i.next();
        week->dataChanged();
    }    
}

/**
 *	@param appointment
 */
void DB_CalendarView::onFormClosed(DB_Activity *appointment) {
    if(appointment != 0) {
        dataChanged();
        layoutChanged();
    }
}

void DB_CalendarView::changeGlobalPalette() {
    mStyle->setGlobalPalette();
    layoutChanged();
}

/**
*	@param dayOfWeek
*/
void DB_CalendarView::expandDayOfWeek(int dayOfWeek) {
    if(mExpandedWeekItem != 0) {
        if(dayOfWeek != mExpandedDayOfWeek) {
            mExpandedDayOfWeek = dayOfWeek;
        } else {
            mExpandedDayOfWeek = 0;
        }

        layoutChanged();
    }
}

/**
 * Change event, for example translation
 */
void DB_CalendarView::changeEvent(QEvent* e) {
    QGraphicsView::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        // retranslateUi(this);
        break;
    case QEvent::PaletteChange:
        changeGlobalPalette();
        break;
    default:
        break;
    }
}
