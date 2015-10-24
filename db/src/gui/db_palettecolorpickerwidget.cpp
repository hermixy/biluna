/*****************************************************************
 * $Id: db_palettecolorpickerwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Oct 29, 2013 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_palettecolorpickerwidget.h"
#include "ui_db_palettecolorpickerwidget.h"

#include "db_dialogfactory.h"

/**
 * @brief DB_PaletteColorPickerWidget::DB_PaletteColorPickerWidget
 * @param parent
 */
DB_PaletteColorPickerWidget::DB_PaletteColorPickerWidget(QWidget * parent)
                : RB_Widget(parent), ui(new Ui::DB_PaletteColorPickerWidget) {
    ui->setupUi(this);

    mReverseColor = false;
    mDefaultPalette = DB_DIALOGFACTORY->getOriginalPalette();
}

DB_PaletteColorPickerWidget::~DB_PaletteColorPickerWidget() {
    delete ui;
}


void DB_PaletteColorPickerWidget::init() {
    ui->twColor->setColumnCount(4);
    ui->twColor->setRowCount(17);
    ui->twColor->setHorizontalHeaderItem(0, new QTableWidgetItem("Color Group"));
    ui->twColor->setHorizontalHeaderItem(1, new QTableWidgetItem("Active"));
    ui->twColor->setHorizontalHeaderItem(2, new QTableWidgetItem("Inactive"));
    ui->twColor->setHorizontalHeaderItem(3, new QTableWidgetItem("Disabled"));

    setItemText();
    setColorPalette(mDefaultPalette);
    readSettings();

    connect(this, SIGNAL(alternativeApplicationStyle(bool)),
            DB_DIALOGFACTORY, SLOT(slotAlternativeApplicationStyle(bool)));

    // Context menu
    // ui->twColor->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction* changeColorAction = new QAction("Change color", ui->twColor);
    ui->twColor->addAction(changeColorAction);
    connect(changeColorAction, SIGNAL(triggered()),
            this, SLOT(slotChangeColor()));

    setWidgetContent();
}

void DB_PaletteColorPickerWidget::on_pbDefault_clicked() {
    setColorPalette(mDefaultPalette);
    QApplication::setStyle(QStyleFactory::create(DB_DIALOGFACTORY->getOriginalStyle()));
    setWidgetPalette(ui->tabWidgetPreview, mDefaultPalette);
    QApplication::setPalette(mDefaultPalette);
    emit alternativeApplicationStyle(false);
}

void DB_PaletteColorPickerWidget::on_pbBase_clicked() {
    QPalette palette(QColor(ui->sbRed->value(),
                            ui->sbGreen->value(),
                            ui->sbBlue->value()));
    setColorPalette(palette);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    setWidgetPalette(ui->tabWidgetPreview, palette);
    emit alternativeApplicationStyle(true);
}

void DB_PaletteColorPickerWidget::on_pbReverse_clicked() {
    for (int row = 0; row < 17; ++row) {
        for (int col = 1; col < 4; ++col) {
            QColor backColor = ui->twColor->item(row, col)->backgroundColor();

            int red = 255 - backColor.red();
            int green = 255 - backColor.green();
            int blue = 255 - backColor.blue();
            backColor.setRgb(red, green, blue); // new background color

            setItem(row, col, backColor);
        }
    }

    on_pbPreview_clicked();
    emit alternativeApplicationStyle(true);
}

void DB_PaletteColorPickerWidget::on_pbPreview_clicked() {
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    setPreviewPalette();
}

void DB_PaletteColorPickerWidget::on_pbApply_clicked() {
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    setApplicationPalette();
    emit alternativeApplicationStyle(true);
}

void DB_PaletteColorPickerWidget::slotChangeColor() {
    QList<QTableWidgetItem*> itemList = ui->twColor->selectedItems();
    QTableWidgetItem* item = itemList.at(0);

    if (!item) {
        DB_DIALOGFACTORY->requestInformationDialog("Not a valid color selected");
        DB_DIALOGFACTORY->commandMessage("Not a valid color selected");
        return;
    }

    QColorDialog* dlg = new QColorDialog();
    dlg->setCurrentColor(item->backgroundColor());
    int result = dlg->exec();

    if (result != QDialog::Accepted) {
        DB_DIALOGFACTORY->commandMessage("Color selection canceled");
        dlg->deleteLater();
        return;
    }

    QColor selectedColor = dlg->selectedColor();

    foreach (item, itemList) {
        setItem(item->row(), item->column(), selectedColor);
    }

    dlg->deleteLater();
}

void DB_PaletteColorPickerWidget::setItemText() {
    ui->twColor->setItem(0, 0, new QTableWidgetItem("Window"));
    ui->twColor->setItem(1, 0, new QTableWidgetItem("WindowText"));
    ui->twColor->setItem(2, 0, new QTableWidgetItem("Base"));
    ui->twColor->setItem(3, 0, new QTableWidgetItem("AlternateBase"));
    ui->twColor->setItem(4, 0, new QTableWidgetItem("ToolTipBase"));
    ui->twColor->setItem(5, 0, new QTableWidgetItem("ToolTipText"));
    ui->twColor->setItem(6, 0, new QTableWidgetItem("Text"));
    ui->twColor->setItem(7, 0, new QTableWidgetItem("Button"));
    ui->twColor->setItem(8, 0, new QTableWidgetItem("ButtonText"));
    ui->twColor->setItem(9, 0, new QTableWidgetItem("BrightText"));

    ui->twColor->setItem(10, 0, new QTableWidgetItem("Light"));
    ui->twColor->setItem(11, 0, new QTableWidgetItem("Midlight"));
    ui->twColor->setItem(12, 0, new QTableWidgetItem("Mid"));
    ui->twColor->setItem(13, 0, new QTableWidgetItem("Dark"));
    ui->twColor->setItem(14, 0, new QTableWidgetItem("Shadow"));

    ui->twColor->setItem(15, 0, new QTableWidgetItem("Highlight"));
    ui->twColor->setItem(16, 0, new QTableWidgetItem("HighlightedText"));
}

void DB_PaletteColorPickerWidget::setColorPalette(const QPalette& palette) {
    // Active
    setPaletteBasedItem(0, 1, palette, QPalette::Active, QPalette::Window);
    setPaletteBasedItem(1, 1, palette, QPalette::Active, QPalette::WindowText);
    setPaletteBasedItem(2, 1, palette, QPalette::Active, QPalette::Base);
    setPaletteBasedItem(3, 1, palette, QPalette::Active, QPalette::AlternateBase);
    setPaletteBasedItem(4, 1, palette, QPalette::Active, QPalette::ToolTipBase);
    setPaletteBasedItem(5, 1, palette, QPalette::Active, QPalette::ToolTipText);
    setPaletteBasedItem(6, 1, palette, QPalette::Active, QPalette::Text);
    setPaletteBasedItem(7, 1, palette, QPalette::Active, QPalette::Button);
    setPaletteBasedItem(8, 1, palette, QPalette::Active, QPalette::ButtonText);
    setPaletteBasedItem(9, 1, palette, QPalette::Active, QPalette::BrightText);

    setPaletteBasedItem(10, 1, palette, QPalette::Active, QPalette::Light);
    setPaletteBasedItem(11, 1, palette, QPalette::Active, QPalette::Midlight);
    setPaletteBasedItem(12, 1, palette, QPalette::Active, QPalette::Mid);
    setPaletteBasedItem(13, 1, palette, QPalette::Active, QPalette::Dark);
    setPaletteBasedItem(14, 1, palette, QPalette::Active, QPalette::Shadow);

    setPaletteBasedItem(15, 1, palette, QPalette::Active, QPalette::Highlight); // 250,170,0 orange
    setPaletteBasedItem(16, 1, palette, QPalette::Active, QPalette::HighlightedText);

    //Inactive
    setPaletteBasedItem(0, 2, palette, QPalette::Inactive, QPalette::Window);
    setPaletteBasedItem(1, 2, palette, QPalette::Inactive, QPalette::WindowText);
    setPaletteBasedItem(2, 2, palette, QPalette::Inactive, QPalette::Base);
    setPaletteBasedItem(3, 2, palette, QPalette::Inactive, QPalette::AlternateBase);
    setPaletteBasedItem(4, 2, palette, QPalette::Inactive, QPalette::ToolTipBase);
    setPaletteBasedItem(5, 2, palette, QPalette::Inactive, QPalette::ToolTipText);
    setPaletteBasedItem(6, 2, palette, QPalette::Inactive, QPalette::Text);
    setPaletteBasedItem(7, 2, palette, QPalette::Inactive, QPalette::Button);
    setPaletteBasedItem(8, 2, palette, QPalette::Inactive, QPalette::ButtonText);
    setPaletteBasedItem(9, 2, palette, QPalette::Inactive, QPalette::BrightText);

    setPaletteBasedItem(10, 2, palette, QPalette::Inactive, QPalette::Light);
    setPaletteBasedItem(11, 2, palette, QPalette::Inactive, QPalette::Midlight);
    setPaletteBasedItem(12, 2, palette, QPalette::Inactive, QPalette::Mid);
    setPaletteBasedItem(13, 2, palette, QPalette::Inactive, QPalette::Dark);
    setPaletteBasedItem(14, 2, palette, QPalette::Inactive, QPalette::Shadow);

    setPaletteBasedItem(15, 2, palette, QPalette::Inactive, QPalette::Highlight); // 250,170,0 orange
    setPaletteBasedItem(16, 2, palette, QPalette::Inactive, QPalette::HighlightedText);

    //Disabled
    setPaletteBasedItem(0, 3, palette, QPalette::Disabled, QPalette::Window);
    setPaletteBasedItem(1, 3, palette, QPalette::Disabled, QPalette::WindowText);
    setPaletteBasedItem(2, 3, palette, QPalette::Disabled, QPalette::Base);
    setPaletteBasedItem(3, 3, palette, QPalette::Disabled, QPalette::AlternateBase);
    setPaletteBasedItem(4, 3, palette, QPalette::Disabled, QPalette::ToolTipBase);
    setPaletteBasedItem(5, 3, palette, QPalette::Disabled, QPalette::ToolTipText);
    setPaletteBasedItem(6, 3, palette, QPalette::Disabled, QPalette::Text);
    setPaletteBasedItem(7, 3, palette, QPalette::Disabled, QPalette::Button);
    setPaletteBasedItem(8, 3, palette, QPalette::Disabled, QPalette::ButtonText);
    setPaletteBasedItem(9, 3, palette, QPalette::Disabled, QPalette::BrightText);

    setPaletteBasedItem(10, 3, palette, QPalette::Disabled, QPalette::Light);
    setPaletteBasedItem(11, 3, palette, QPalette::Disabled, QPalette::Midlight);
    setPaletteBasedItem(12, 3, palette, QPalette::Disabled, QPalette::Mid);
    setPaletteBasedItem(13, 3, palette, QPalette::Disabled, QPalette::Dark);
    setPaletteBasedItem(14, 3, palette, QPalette::Disabled, QPalette::Shadow);

    setPaletteBasedItem(15, 3, palette, QPalette::Disabled, QPalette::Highlight); // 250,170,0 orange
    setPaletteBasedItem(16, 3, palette, QPalette::Disabled, QPalette::HighlightedText);
}

void DB_PaletteColorPickerWidget::setWidgetContent() {
    // Active
    ui->radioButton->setChecked(true);
    ui->comboBox->addItem("Item 1");
    ui->comboBox->addItem("Item 2");
    ui->textEdit->setText("This is some basic text");
    ui->listWidget->addItem("List item 1");
    ui->listWidget->addItem("List item 2");
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 5; ++i) {
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    }
    ui->treeWidget->insertTopLevelItems(0, items);
    QTreeWidgetItem* childItem = new QTreeWidgetItem(QStringList(QString("child item")));
    QTreeWidgetItem* parentItem = items.at(0);
    parentItem->addChild(childItem);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setAlternatingRowColors(true);
    int row = 0;
    int column = 0;
    QTableWidgetItem* newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
    ui->tableWidget->setItem(row, column, newItem);
    newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
    ui->tableWidget->setItem(row, column, newItem);
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->addLine(0,0,50,50,QPen(QColor(Qt::red)));
    ui->graphicsView->scene()->addLine(0,10,50,10,QPen(QColor(Qt::blue)));

    //Disabled
    ui->radioButton_3->setChecked(true);
    ui->comboBox_2->addItem("Item 1");
    ui->comboBox_2->addItem("Item 2");
    ui->textEdit_2->setText("This is some basic text");
    ui->listWidget_2->addItem("List item 1");
    ui->listWidget_2->addItem("List item 2");
    ui->treeWidget_2->setColumnCount(1);
    for (int i = 0; i < 5; ++i) {
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    }
    ui->treeWidget_2->insertTopLevelItems(0, items);
    childItem = new QTreeWidgetItem(QStringList(QString("child item")));
    parentItem = items.at(0);
    parentItem->addChild(childItem);
    ui->tableWidget_2->setRowCount(10);
    ui->tableWidget_2->setColumnCount(5);
    ui->tableWidget_2->setAlternatingRowColors(true);
    row = 0;
    column = 0;
    newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
    ui->tableWidget_2->setItem(row, column, newItem);
    newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
    ui->tableWidget_2->setItem(row, column, newItem);
    scene = new QGraphicsScene();
    ui->graphicsView_2->setScene(scene);
    ui->graphicsView_2->scene()->addLine(0,0,50,50,QPen(QColor(Qt::red)));
    ui->graphicsView_2->scene()->addLine(0,10,50,10,QPen(QColor(Qt::blue)));
}

void DB_PaletteColorPickerWidget::setPaletteBasedItem(int row, int col,
                                                      const QPalette& palette,
                                                      QPalette::ColorGroup group,
                                                      QPalette::ColorRole role) {
    setItem(row, col, palette.color(group, role));
}

void DB_PaletteColorPickerWidget::setItem(int row, int col,
                                          const QColor& backColor) {
    QTableWidgetItem* item = ui->twColor->item(row, col);
    RB_String text = RB_String::number(backColor.red()) + ","
            + RB_String::number(backColor.green()) + ","
            + RB_String::number(backColor.blue());

    if (!item) {
        item = new QTableWidgetItem(text);
        ui->twColor->setItem(row, col, item);
    } else {
        item->setText(text);
    }

    int red = backColor.red();
    int green = backColor.green();
    int blue = backColor.blue();
    QColor foreColor;

    if (red + green + blue < 255) {
        // dark background
        foreColor = QColor(Qt::white);
    } else {
        foreColor = QColor(Qt::black);
    }

    item->setForeground(QBrush(foreColor));
    item->setBackgroundColor(backColor);
}

/**
 * Set the widget palette
 * @brief DB_PaletteColorPickerWidget::setWidgetPalette
 * @param wgt
 * @param palette
 */
void DB_PaletteColorPickerWidget::setWidgetPalette(QWidget* wgt,
                                                   const QPalette& palette) {
    wgt->setPalette(palette);
}

void DB_PaletteColorPickerWidget::setPreviewPalette() {
    QPalette palette;
    setPalette(palette);
    setWidgetPalette(ui->tabWidgetPreview, palette);
}

void DB_PaletteColorPickerWidget::setApplicationPalette() {
    QPalette palette;
    setPalette(palette);
    QApplication::setPalette(palette);
}

void DB_PaletteColorPickerWidget::setPalette(QPalette& palette) {
    palette.setColor(QPalette::Active, QPalette::Window, ui->twColor->item(0,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::WindowText, ui->twColor->item(1,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Base, ui->twColor->item(2,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::AlternateBase, ui->twColor->item(3,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::ToolTipBase, ui->twColor->item(4,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::ToolTipText, ui->twColor->item(5,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Text, ui->twColor->item(6,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Button, ui->twColor->item(7,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::ButtonText, ui->twColor->item(8,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::BrightText, ui->twColor->item(9,1)->backgroundColor());

    palette.setColor(QPalette::Active, QPalette::Light, ui->twColor->item(10,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Midlight, ui->twColor->item(11,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Mid, ui->twColor->item(12,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Dark, ui->twColor->item(13,1)->backgroundColor());
    palette.setColor(QPalette::Active, QPalette::Shadow, ui->twColor->item(14,1)->backgroundColor());

    palette.setColor(QPalette::Active, QPalette::Highlight, ui->twColor->item(15,1)->backgroundColor()); // 250,170,0 orange
    palette.setColor(QPalette::Active, QPalette::HighlightedText, ui->twColor->item(16,1)->backgroundColor());

    //Inactive
    palette.setColor(QPalette::Inactive, QPalette::Window, ui->twColor->item(0,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::WindowText, ui->twColor->item(1,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Base, ui->twColor->item(2,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, ui->twColor->item(3,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, ui->twColor->item(4,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText, ui->twColor->item(5,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Text, ui->twColor->item(6,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Button, ui->twColor->item(7,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, ui->twColor->item(8,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::BrightText, ui->twColor->item(9,2)->backgroundColor());

    palette.setColor(QPalette::Inactive, QPalette::Light, ui->twColor->item(10,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Midlight, ui->twColor->item(11,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Mid, ui->twColor->item(12,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Dark, ui->twColor->item(13,2)->backgroundColor());
    palette.setColor(QPalette::Inactive, QPalette::Shadow, ui->twColor->item(14,2)->backgroundColor());

    palette.setColor(QPalette::Inactive, QPalette::Highlight, ui->twColor->item(15,2)->backgroundColor()); // 250,170,0 orange
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, ui->twColor->item(16,2)->backgroundColor());

    //Disabled
    palette.setColor(QPalette::Disabled, QPalette::Window, ui->twColor->item(0,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::WindowText, ui->twColor->item(1,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Base, ui->twColor->item(2,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, ui->twColor->item(3,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, ui->twColor->item(4,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::ToolTipText, ui->twColor->item(5,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Text, ui->twColor->item(6,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Button, ui->twColor->item(7,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, ui->twColor->item(8,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::BrightText, ui->twColor->item(9,3)->backgroundColor());

    palette.setColor(QPalette::Disabled, QPalette::Light, ui->twColor->item(10,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Midlight, ui->twColor->item(11,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Mid, ui->twColor->item(12,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Dark, ui->twColor->item(13,3)->backgroundColor());
    palette.setColor(QPalette::Disabled, QPalette::Shadow, ui->twColor->item(14,3)->backgroundColor());

    palette.setColor(QPalette::Disabled, QPalette::Highlight, ui->twColor->item(15,3)->backgroundColor()); // 250,170,0 orange
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, ui->twColor->item(16,3)->backgroundColor());
}
