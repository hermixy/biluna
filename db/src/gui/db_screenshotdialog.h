/*****************************************************************
 * $Id: db_screenshotdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 23, 2015 8:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SCREENSHOTDIALOG_H
#define DB_SCREENSHOTDIALOG_H

#include "rb_dialog.h"

namespace Ui {
class DB_ScreenshotDialog;
}

class DB_EXPORT DB_ScreenshotDialog : public RB_Dialog
{
    Q_OBJECT

public:
    explicit DB_ScreenshotDialog(QWidget *parent = 0);
    ~DB_ScreenshotDialog();

    void init();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);


public slots:
    void on_pbNew_clicked();
    void on_pbSave_clicked();
    void on_pbClose_clicked();
    void slotShootScreen();
    void slotUpdateCheckBox();

private:
    void updateScreenshotLabel();

    Ui::DB_ScreenshotDialog* ui;
    QPixmap mOriginalPixmap;
    QRubberBand* mRubberBand;
    QPoint mStartPoint;
    QPoint mEndPoint;
};

#endif // DB_SCREENSHOTDIALOG_H
