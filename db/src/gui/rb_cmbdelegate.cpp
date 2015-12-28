/*****************************************************************
 * $Id: rb_cmbdelegate.cpp 1655 2012-05-30 19:11:20Z rutger $
 * Created: Feb 18, 2010 9:25:40 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_cmbdelegate.h"
#include "rb_mmproxy.h"


/**
 * Constructor
 * @param parent
 * @param strL string list for the combobox items
 */
RB_CmbDelegate::RB_CmbDelegate(QObject* parent, const QStringList& strL,
                               RB_MmProxy *proxyModel, bool isSetText)
                : QStyledItemDelegate(parent) {
    mProxyModel = proxyModel;
    mStrL = strL;
    mIsSetText = isSetText;
//    mLabel = new QLabel();
//    mLabel->setTextFormat(Qt::RichText); // accept richtext
//    mLabel->setWordWrap(false);
}

RB_CmbDelegate::~RB_CmbDelegate() {
//    delete mLabel;
}

QWidget* RB_CmbDelegate::createEditor(QWidget* parent,
                                    const QStyleOptionViewItem& /*option*/,
                                    const QModelIndex& /*index*/) const {
    QComboBox* editor = new QComboBox(parent);
    for (int i = 0; i < mStrL.count(); ++i) {
        editor->addItem(mStrL.at(i));
    }
    editor->installEventFilter(const_cast<RB_CmbDelegate*>(this));
    return editor;
}

#if 0
/**
 * Paint can be used for something like the stardelegate example
 */
void RB_CmbDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const {
    QVariant value = index.data(Qt::DisplayRole);

    if (value.isValid() && !value.isNull()) {
        int cmbIndex = value.toInt();

        if (cmbIndex < 0 || cmbIndex >= mStrL.count()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        bool odd = false;
//        if (mProxyModel) { crashes
//            QModelIndex proxyIdx = mProxyModel->mapFromSource(index);
//            odd = proxyIdx.row() % 2;
//        }
        bool selected = option.state & QStyle::State_Selected;
        QPalette palette(option.palette);
        palette.setColor(QPalette::Active, QPalette::Window,
                         selected ? option.palette.highlight().color()
                                  : option.palette.base().color());
        // Does not work when sorting
        palette.setColor(QPalette::Active, QPalette::Window,
                         selected ? option.palette.highlight().color()
                                  : (odd ? option.palette.alternateBase().color()
                                         : option.palette.base().color()));
//        palette.setColor(QPalette::Active, QPalette::WindowText,
//                         selected
//                         ? option.palette.highlightedText().color()
//                         : option.palette.text().color());

        //    int yOffset = checkbox->height() < option.rect.height()
        //            ? (option.rect.height() - checkbox->height()) / 2 : 0;
        //    QRect checkboxRect(option.rect.x(), option.rect.y() + yOffset,
        //            checkbox->width(), checkbox->height());
        //    checkbox->setPalette(palette);
        //    bool checked = index.model()->data(index, Qt::CheckStateRole)
        //                                       .toInt() == Qt::Checked;
        //    checkbox->setChecked(checked);

        QRect labelRect(option.rect.x()/* + checkbox->width() */,
                option.rect.y(), option.rect.width() /* - checkbox->width() */,
                option.rect.height());
        mLabel->setPalette(palette);
        mLabel->setFixedSize(qMax(0, labelRect.width()),
                            labelRect.height());
        QString str = mStrL.at(cmbIndex);
        mLabel->setText("<div style=\"margin-left: 5;\">"
                        + str + "</div>");

        // To cache the pixmap for faster response, refer QPixmapCach:

        //    QString checkboxKey = QString("CHECKBOX:%1.%2").arg(selected)
        //                                                   .arg(checked);
        //    paintWidget(painter, checkboxRect, checkboxKey, checkbox);

        QString labelKey = QString("LABEL:%1.%2.%3x%4").arg(selected)
                .arg(str).arg(labelRect.width()).arg(labelRect.height());
        paintWidget(painter, labelRect, labelKey, mLabel);
    }
}
#endif

void RB_CmbDelegate::setEditorData(QWidget* editor,
                                    const QModelIndex& index) const {
    // Use edit role to get the original data
    QVariant value = index.model()->data(index, RB2::RoleOrigData);

    QComboBox* cb = static_cast<QComboBox*>(editor);
    cb->setCurrentIndex(value.toInt());
}


void RB_CmbDelegate::setModelData(QWidget* editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QComboBox* cb = static_cast<QComboBox*>(editor);
    if (!mIsSetText) {
        model->setData(index, cb->currentIndex());
    } else {
        model->setData(index, cb->currentText());
    }
}


void RB_CmbDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const {
    editor->setGeometry(option.rect);
}

/**
 * @returns text at index
 */
QString RB_CmbDelegate::at(int index) const {
    return mStrL.at(index);
}

void RB_CmbDelegate::paintWidget(QPainter* painter,
                                const QRect &rect, const QString& cacheKey,
                                QWidget* widget) const {
    QPixmap pixmap(widget->size());
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(cacheKey, &pixmap)) {
#else
    if (!QPixmapCache::find(cacheKey, pixmap)) {
#endif
        widget->render(&pixmap);
        QPixmapCache::insert(cacheKey, pixmap);
    }
    painter->drawPixmap(rect, pixmap);
}
