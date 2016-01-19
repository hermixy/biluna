/*****************************************************************
 * $Id: db_screenshotdialog.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Feb 23, 2015 8:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_screenshotdialog.h"
#include "ui_db_screenshotdialog.h"

#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "rb_settings.h"

DB_ScreenshotDialog::DB_ScreenshotDialog(QWidget* parent) :
    RB_Dialog(parent),
    ui(new Ui::DB_ScreenshotDialog) {
    ui->setupUi(this);
    mRubberBand = NULL;

    readSettings();
}

DB_ScreenshotDialog::~DB_ScreenshotDialog() {
    delete ui;
}

void DB_ScreenshotDialog::init() {
    setWindowTitle(tr("DB Screenshot"));
    slotShootScreen();

    RB_SETTINGS->beginGroup(objectName());
    ui->chbHide->setChecked(
               RB_SETTINGS->value("hidewindow", 1).toInt() == 1 ? true : false);
    ui->chbFullPrimaryScreen->setChecked(
                RB_SETTINGS->value("fullscreen", 1).toInt() == 0
                ? true : false);
    ui->chbSaveScaled->setChecked(
                RB_SETTINGS->value("savescaled", 1).toInt() == 0
                ? true : false);
    ui->sbWidth->setValue(RB_SETTINGS->value("maxwidth", 500).toInt());
    ui->sbHeight->setValue(RB_SETTINGS->value("maxheight", 300).toInt());
    RB_SETTINGS->endGroup();
}

void DB_ScreenshotDialog::keyReleaseEvent(QKeyEvent* e) {
    int key = e->key();

    if (key) { // switch case does not work, do not know why.
        QPixmap selPixmap;
        setPixmap(selPixmap);
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setPixmap(selPixmap);
        e->accept();
        return;
    }

    RB_Dialog::keyReleaseEvent(e);
}

void DB_ScreenshotDialog::mousePressEvent(QMouseEvent* event) {
    mStartPoint = event->pos();

    if (!ui->lblScreenshot->rect().contains(mStartPoint)) {
        if (mRubberBand) {
            mRubberBand->hide();
        }

        RB_Dialog::mousePressEvent(event);
        return;
    }

    if (!mRubberBand) {
        mRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    } else {
        mRubberBand->hide();
    }

    mRubberBand->setGeometry(QRect(mStartPoint, QSize()));
    mRubberBand->show();
}

void DB_ScreenshotDialog::mouseMoveEvent(QMouseEvent* event) {
    if (!mRubberBand) {
        RB_Dialog::mouseMoveEvent(event);
        return;
    } else if (!ui->lblScreenshot->rect().contains(event->pos())) {
        mRubberBand->hide();
    } else {
        mRubberBand->setGeometry(QRect(mStartPoint, event->pos()).normalized());
    }
}

void DB_ScreenshotDialog::mouseReleaseEvent(QMouseEvent* event) {
    // mRubberBand->hide();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().

    mEndPoint = event->pos();
}

void DB_ScreenshotDialog::resizeEvent(QResizeEvent* /* e */) {
    QSize scaledSize = mOriginalPixmap.size();
    scaledSize.scale(ui->lblScreenshot->size(), Qt::KeepAspectRatio);
    if (!ui->lblScreenshot->pixmap()
             || scaledSize != ui->lblScreenshot->pixmap()->size()) {
         updateScreenshotLabel();
    }
    if (mRubberBand && !mRubberBand->isHidden()) {
        mRubberBand->hide();
    }
}

void DB_ScreenshotDialog::on_pbNew_clicked() {
    if (ui->chbHide->isChecked()) {
         hide();
    }
    ui->pbNew->setDisabled(true);

    QTimer::singleShot(ui->sbDelay->value() * 1000,
                       this, SLOT(slotShootScreen()));
}

void DB_ScreenshotDialog::on_pbClipboard_clicked() {
    QPixmap selPixmap;
    setPixmap(selPixmap);
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setPixmap(selPixmap);
}

void DB_ScreenshotDialog::on_pbSave_clicked() {
    QString format = "png";
    QString fileName = objectName() + ".png";
    DB_ActionFileSaveAs action;
    action.getSaveFileName(
                fileName,
                "Portable Network Graphics Files (*.png);;All Files (*)");

    if (fileName.isEmpty()) {
        return;
    }

    QPixmap selPixmap;
    setPixmap(selPixmap);
    selPixmap.save(fileName, format.toLatin1());
}

void DB_ScreenshotDialog::on_pbClose_clicked() {
    RB_SETTINGS->beginGroup(objectName());
    RB_SETTINGS->setValue("hidewindow", ui->chbHide->isChecked() ? 1 : 0);
    RB_SETTINGS->setValue("fullscreen", ui->chbFullPrimaryScreen->isChecked()
                          ? 1 : 0);
    RB_SETTINGS->setValue("savescaled", ui->chbSaveScaled->isChecked()
                          ? 1 : 0);
    RB_SETTINGS->setValue("maxwidth", ui->sbWidth->value());
    RB_SETTINGS->setValue("maxheight", ui->sbHeight->value());
    RB_SETTINGS->endGroup();
    writeSettings();
    close();
    accept();
}

void DB_ScreenshotDialog::slotShootScreen() {
    if (ui->sbDelay->value() != 0) {
         qApp->beep();
    }

    mOriginalPixmap = QPixmap(); // clear image for low memory situations
                                 // on embedded devices.
    QScreen* screen = QGuiApplication::primaryScreen();
    mOriginalPixmap = ui->chbFullPrimaryScreen->isChecked()
            ? screen->grabWindow(0)
            : screen->grabWindow(DB_DIALOGFACTORY->getMainWindow()->winId());
    this->updateScreenshotLabel();
    ui->pbNew->setDisabled(false);

    if (ui->chbHide->isChecked()) {
         show();
    }
}

void DB_ScreenshotDialog::slotUpdateCheckBox() {
    if (ui->sbDelay->value() == 0) {
        ui->chbHide->setDisabled(true);
        ui->chbHide->setChecked(false);
    } else {
        ui->chbHide->setDisabled(false);
    }
}

void DB_ScreenshotDialog::updateScreenshotLabel() {
    ui->lblScreenshot->setPixmap(
                mOriginalPixmap.scaled(ui->lblScreenshot->size(),
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));
}

void DB_ScreenshotDialog::setPixmap(QPixmap& selPixmap) {
    if (!mRubberBand || mRubberBand->isHidden()) {
        selPixmap = mOriginalPixmap;
    } else {
        // TODO mapping not correct

        // only selection
        QRect rubberRect = mRubberBand->rect();
        // HACK: -2 and -1 are strange modifications
        rubberRect.moveTopLeft(QPoint(std::min(mStartPoint.x(), mEndPoint.x()) - 2,
                                     std::min(mStartPoint.y(), mEndPoint.y()) - 2));
        rubberRect.setWidth(rubberRect.width() - 1);
        rubberRect.setHeight(rubberRect.height() - 1);

        // transform label coordinates to original pixmap coordinates
        QRect labelRect = ui->lblScreenshot->rect();
        double widthScale = ((double)labelRect.width())
                / ((double)mOriginalPixmap.size().width());
        double heightScale = ((double)labelRect.height())
                / ((double)mOriginalPixmap.size().height());
        double scale = 0;
        double verticalMove = 0;
        double horizontalMove = 0;

        if (widthScale < heightScale) {
            scale = widthScale;
        } else {
            scale = heightScale;
        }

        if (scale == widthScale) {
            verticalMove = (labelRect.height()
                            - mOriginalPixmap.size().height() * scale) / 2;
            rubberRect.moveTopLeft(QPoint(rubberRect.x(),
                                          rubberRect.y() - verticalMove));
        } else {
            horizontalMove = (labelRect.width()
                              - mOriginalPixmap.size().width() * scale) / 2;
            rubberRect.moveTopLeft(QPoint(rubberRect.x() - horizontalMove,
                                          rubberRect.y()));
        }

        if (scale != 0.0) {
            rubberRect.setCoords(rubberRect.x() / scale,
                                 rubberRect.y() / scale,
                                 (rubberRect.x() + rubberRect.width()) / scale,
                                 (rubberRect.y() + rubberRect.height()) / scale);
        }

        // save intersection
        QRect selRect = mOriginalPixmap.rect().intersected(rubberRect);
        selPixmap = mOriginalPixmap.copy(selRect);
    }

    // scale
    if (ui->chbSaveScaled->isChecked()) {
        QSize maxSize = QSize(ui->sbWidth->value(), ui->sbHeight->value());
        selPixmap = selPixmap.scaled(maxSize,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
    }
}
