#ifndef RLISTWIDGET_H_
#define RLISTWIDGET_H_

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QtDebug>

/**
 * TODO generateScriptShell
 *
 * \scriptable
 * \generateScriptShell
 */
class RListWidget: public QListWidget {

    Q_OBJECT

public:
    RListWidget(QWidget* parent=0);
    virtual ~RListWidget();

protected:
    void contextMenuEvent(QContextMenuEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

signals:
    void iconClicked(int x, QListWidgetItem* item);

private:
    QListWidgetItem* itemPressed;
    int iconOffset;
};

Q_DECLARE_METATYPE(RListWidget*)

#endif
