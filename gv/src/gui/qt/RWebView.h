#ifndef RWEBVIEW_H_
#define RWEBVIEW_H_

#include <QWebView>
#include <QtDesigner/QDesignerExportWidget>

/**
 * \scriptable
 */
class /*QDESIGNER_WIDGET_EXPORT*/ RWebView: public QWebView {

Q_OBJECT

public:
    RWebView(QWidget* parent);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
};

Q_DECLARE_METATYPE(RWebView*)

#endif
