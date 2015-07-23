/*****************************************************************
 * $Id: rb_ledelegate.cpp 782 2008-08-24 17:27:42Z rutger $
 * Created: 2011-05-10 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the DB Test project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_tedelegate.h"

#include <QtWidgets>


/**
 * Constructor
 */
RB_TeDelegate::RB_TeDelegate(QObject* parent) : QStyledItemDelegate(parent) {
    mLabel = new QLabel();
    mLabel->setTextFormat(Qt::RichText); // accept richtext
    mLabel->setWordWrap(false);
}

RB_TeDelegate::~RB_TeDelegate() {
	// nothing
}

QWidget* RB_TeDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& /*index*/) const {
    QTextEdit* wgt = new QTextEdit(parent);
    wgt->installEventFilter(const_cast<RB_TeDelegate*>(this));
    wgt->viewport()->setFixedHeight(option.rect.height());
    wgt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    wgt->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    wgt->setTabChangesFocus(true);
    return wgt;
}


void RB_TeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const {
    QVariant value = index.data(Qt::DisplayRole);

    if (value.isValid() && !value.isNull()) {
        bool selected = option.state & QStyle::State_Selected;
        QPalette palette(option.palette);
        palette.setColor(QPalette::Active, QPalette::Window,
                         selected ? option.palette.highlight().color()
                                  : option.palette.base().color());
        palette.setColor(QPalette::Active, QPalette::WindowText,
                         selected
                         ? option.palette.highlightedText().color()
                         : option.palette.text().color());

        //    int yOffset = checkbox->height() < option.rect.height()
        //            ? (option.rect.height() - checkbox->height()) / 2 : 0;
        //    QRect checkboxRect(option.rect.x(), option.rect.y() + yOffset,
        //            checkbox->width(), checkbox->height());
        //    checkbox->setPalette(palette);
        //    bool checked = index.model()->data(index, Qt::CheckStateRole)
        //                                       .toInt() == Qt::Checked;
        //    checkbox->setChecked(checked);

        QRect labelRect(option.rect.x() /* + checkbox->width() */,
                option.rect.y(), option.rect.width() /* - checkbox->width() */,
                option.rect.height());
        mLabel->setPalette(palette);
        mLabel->setFixedSize(qMax(0, labelRect.width()),
                            labelRect.height());
        QString html = index.model()->data(index, Qt::DisplayRole)
                                           .toString();
        mLabel->setText(html);

        //    QString checkboxKey = QString("CHECKBOX:%1.%2").arg(selected)
        //                                                   .arg(checked);
        //    paintWidget(painter, checkboxRect, checkboxKey, checkbox);

        QString labelKey = QString("LABEL:%1.%2.%3x%4").arg(selected)
                .arg(html).arg(labelRect.width()).arg(labelRect.height());
        paintWidget(painter, labelRect, labelKey, mLabel);


    }
}

void RB_TeDelegate::setEditorData(QWidget* editor,
                                  const QModelIndex& index) const {
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QTextEdit* wgt = static_cast<QTextEdit*>(editor);
    wgt->setHtml(value.toString());
}


void RB_TeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                 const QModelIndex& index) const {
    QTextEdit* wgt = static_cast<QTextEdit*>(editor);
    model->setData(index, wgt->toHtml());
}

QSize RB_TeDelegate::sizeHint(const QStyleOptionViewItem& option,
                              const QModelIndex& index) const {
    QString html = index.model()->data(index, Qt::DisplayRole)
                                       .toString();
    mDocument.setDefaultFont(option.font);
    mDocument.setHtml(html);
    int height = option.rect.height() > option.fontMetrics.height()
            ? option.rect.height() : option.fontMetrics.height();
    return QSize(mDocument.idealWidth(), height);

}

void RB_TeDelegate::updateEditorGeometry(QWidget* editor,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& /* index */) const {
    QTextEdit* wgt = static_cast<QTextEdit*>(editor);
    wgt->setGeometry(option.rect); // QRect(0.0, 0.0, option.rect.width(), mHeight));
}

void RB_TeDelegate::paintWidget(QPainter* painter,
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
