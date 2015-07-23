/*****************************************************************
 * $Id: rb_urlbasedrenderer.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 28, 2011 9:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include <QtWidgets/QApplication>
#include <QWebView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>
#include <QDir>
#include <QUrlQuery>
#include "db_global.h"

/**
 * Copy of Qt-Labs url-rendering
 */
class DB_EXPORT RB_RendererReply : public QNetworkReply {

    Q_OBJECT

public:
    RB_RendererReply(QObject* object, const QNetworkRequest& request)
        : QNetworkReply(object)
        , position(0)
    {
        setRequest(request);
        setOperation(QNetworkAccessManager::GetOperation);
        setHeader(QNetworkRequest::ContentTypeHeader,QVariant("image/png"));
        open(ReadOnly|Unbuffered);
        setUrl(request.url());
        QUrlQuery urlQuery = QUrlQuery(request.url());
        QString pattern = urlQuery.queryItemValue("pattern");
        Qt::BrushStyle brushStyle = Qt::SolidPattern;
        if (pattern == "dense")
            brushStyle = Qt::Dense2Pattern;
        else if (pattern == "cross")
            brushStyle = Qt::CrossPattern;
        else if (pattern == "diagonal")
            brushStyle = Qt::FDiagPattern;
        const QString radiusString = urlQuery.queryItemValue("radius");
        const QString widthString = urlQuery.queryItemValue("width");
        const QString heightString = urlQuery.queryItemValue("height");
        const QString colorString = urlQuery.queryItemValue("color");
        QColor color(colorString);
        bool ok;
        int radius = radiusString.toInt(&ok);
        if (!ok)
            radius = 5;
        QSize size;
        size.setWidth(widthString.toInt(&ok));
        if (!ok)
            size.setWidth(100);
        size.setHeight(heightString.toInt(&ok));
        if (!ok)
            size.setHeight(100);
        connect(&watcher, SIGNAL(finished()), SLOT(generateDone()));
        QFuture<QByteArray> future = QtConcurrent::run<QByteArray>(generate, size, brushStyle, radius, color);
        watcher.setFuture(future);
    }

    qint64 readData(char* data, qint64 maxSize)
    {
        const qint64 readSize = qMin(maxSize, (qint64)(buffer.size() - position));
        memcpy(data, buffer.constData() + position, readSize);
        position += readSize;
        return readSize;
    }

    virtual qint64 bytesAvailable() const
    {
        return buffer.size() - position;
    }

    virtual qint64 pos () const
    {
        return position;
    }

    virtual bool seek( qint64 pos )
    {
        if (pos < 0 || pos >= buffer.size())
            return false;
        position = pos;
        return true;
    }

    virtual qint64 size () const
    {
        return buffer.size();
    }

    static QByteArray generate(const QSize& size, const Qt::BrushStyle style, int radius, const QColor& color)
    {
        QImage image(size, QImage::Format_ARGB32_Premultiplied);
        image.fill(0);
        QPainter painter(&image);
        QPainterPath path;
        path.addRoundedRect(QRectF(QPoint(0, 0), size), radius, radius);
        painter.fillPath(path, QBrush(color, style));
        QByteArray saveData;
        QBuffer b(&saveData);
        image.save(&b, "PNG");
        return saveData;
    }

public slots:
    void generateDone()
    {
        setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
        position = 0;
        buffer = watcher.result();
        emit readyRead();
        emit finished();
    }

    void abort()
    {
    }
public:
    QNetworkReply* rawReply;
private:
    QFutureWatcher<QByteArray> watcher;
    QByteArray buffer;
    int position;
};

class DB_EXPORT RB_UrlBasedRenderer : public QNetworkAccessManager
{
public:
    RB_UrlBasedRenderer(QObject* parent = 0) : QNetworkAccessManager(parent)
    {
    }

    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
    {
        if (op != GetOperation || request.url().scheme() != "qt-render" || request.url().host() != "button")
            return QNetworkAccessManager::createRequest(op, request, outgoingData);
        return new RB_RendererReply(this, request);
    }
};
/*
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWebView webView;
    RB_UrlBasedRenderer urlBasedRenderer;
    webView.page()->setNetworkAccessManager(&urlBasedRenderer);
    webView.setHtml("<HTML><BODY><P>Testing image:</P><IMG src=\"qt-render://button?width=100&height=100&radius=5&color=green&pattern=dense\" /><I>Rutger</I></BODY></HTML>");
    webView.show();
    return app.exec();
}

#include <main.moc>
*/
