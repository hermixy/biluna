/*****************************************************************
 * $Id: peng_actioneqtype.cpp 1370 2011-02-17 22:26:05Z rutger $
 * Created: Feb 28, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_webwidget.h"

#include <QtWidgets>
#include <QtNetwork>

RB_WebWidget::RB_WebWidget(const QString& mimeType, QWidget* parent)
                                                    : QSvgWidget(parent) {
    this->mMimeType = mimeType;
}

/*
void RB_WebWidget::updateModel()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

    if (reply->error() != QNetworkReply::NoError)
        return;

    bool hasHeader = false;
    QString charset = "latin1";

    foreach (QString piece, mimeType.split(";")) {
        piece = piece.trimmed();
        if (piece.contains("=")) {
            int index = piece.indexOf("=");
            QString left = piece.left(index).trimmed();
            QString right = piece.mid(index + 1).trimmed();
            if (left == "header")
                hasHeader = (right == "present");
            else if (left == "charset")
                charset = right;
        }
    }
    
    QTextStream stream(reply);
    stream.setCodec(QTextCodec::codecForName(charset.toLatin1()));

    QStandardItemModel *model = new QStandardItemModel(this);
    QList<QStandardItem *> items;
    bool firstLine = hasHeader;
    bool wasQuote = false;
    bool wasCR = false;
    bool quoted = false;
    QString text;

    while (!stream.atEnd()) {

        QString ch = stream.read(1);

        if (wasQuote) {
            if (ch == "\"") {
                if (quoted) {
                    text += ch;         // quoted "" are inserted as "
                    wasQuote = false;   // no quotes are pending
                } else {
                    quoted = true;      // unquoted "" starts quoting
                    wasQuote = true;    // with a pending quote
                }
                continue;               // process the next character

            } else {
                quoted = !quoted;       // process the pending quote
                wasQuote = false;       // no quotes are pending
            }                           // process the current character

        } else if (wasCR) {
            wasCR = false;

            if (ch == "\n") {           // CR LF represents the end of a row
                if (!text.isEmpty())
                    items.append(new QStandardItem(QString(text)));

                addRow(firstLine, model, items);
                items.clear();
                text = "";
                firstLine = false;
                continue;               // process the next character
            } else
                text += "\r";           // CR on its own is inserted
        }                               // process the current character

        // wasQuote is never true here.
        // wasCR is never true here.

        if (ch == "\"")
            wasQuote = true;            // handle the pending quote later

        else if (ch == ",") {
            if (quoted)
                text += ch;
            else {
                items.append(new QStandardItem(QString(text)));
                text = "";
            }
        }

        else if (ch == "\r") {
            if (!quoted)
                wasCR = true;
            else
                text += ch;
        }

        else if (ch == "\n")
            text += ch;
        else
            text += ch;

    }

    if (items.count() > 0)
        addRow(firstLine, model, items);

    reply->close();

    setModel(model);
    resizeColumnsToContents();
    horizontalHeader()->setStretchLastSection(true);
}

void RB_WebWidget::addRow(bool firstLine, QStandardItemModel *model,
                     const QList<QStandardItem *> &items)
{
    if (firstLine) {
        for (int j = 0; j < items.count(); ++j)
            model->setHorizontalHeaderItem(j, items[j]);
    } else
        model->appendRow(items);
}
*/
