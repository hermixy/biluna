/*****************************************************************
 * $Id: db_palettecolorpickerwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 2, 2013 8:27 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *
 * From https://github.com/ahiddenseeker/qt-palette-color-picker
 * original author HiddenSeeker
 * Describes a widget, which performs color picking
 *****************************************************************/
#ifndef DB_PALETTECOLORPICKERWIDGET_H
#define DB_PALETTECOLORPICKERWIDGET_H

#include <QTableWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QImage>
#include <QHash>
#include <QMouseEvent>
#include <QList>
#include <QColor>

#include "rb_widget.h"

namespace Ui {
class DB_PaletteColorPickerWidget;
}


/**
 * A palette color picker widget
 */
class DB_EXPORT DB_PaletteColorPickerWidget: public RB_Widget {

	Q_OBJECT

public:
    explicit DB_PaletteColorPickerWidget(QWidget * parent = NULL);
    ~DB_PaletteColorPickerWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Palette color"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();

signals:
    void alternativeApplicationStyle(bool isAlternative);

public slots:
    void on_pbDefault_clicked();
    void on_pbBase_clicked();
    void on_pbReverse_clicked();
    void on_pbPreview_clicked();
    void on_pbApply_clicked();

    void slotChangeColor();

private:
    void setItemText();
    void setColorPalette(const QPalette& palette);
    void setWidgetContent();
    void setPaletteBasedItem(int row, int col, const QPalette& palette,
                             QPalette::ColorGroup group,
                             QPalette::ColorRole role);
    void setItem(int row, int col, const QColor& backColor);
    void setWidgetPalette(QWidget* wgt, const QPalette& palette);
    void setPreviewPalette();
    void setApplicationPalette();
    void setPalette(QPalette &palette);

    Ui::DB_PaletteColorPickerWidget* ui;
    bool mReverseColor;
    QPalette mDefaultPalette;
};

#endif // DB_PALETTECOLORPICKERWIDGET_H
