#ifndef RLISTVIEW_H_
#define RLISTVIEW_H_

#include <QWidget>
#include <QListView>
#include <QtDebug>

/**
 * \scriptable
 * \generateScriptShell
 */
class RListView: public QListView {

    Q_OBJECT

public:
    RListView(QWidget* parent = 0) :
		QListView(parent) {
	}
    virtual ~RListView(){}

protected:
    void contextMenuEvent(QContextMenuEvent* e) {
		QListView::contextMenuEvent(e);
	}
	void mousePressEvent(QMouseEvent* e) {
		QListView::mousePressEvent(e);
	}
	void mouseReleaseEvent(QMouseEvent* e) {
		QListView::mouseReleaseEvent(e);
	}
	void mouseMoveEvent(QMouseEvent* e) {
		QListView::mouseMoveEvent(e);
	}
	void dragEnterEvent(QDragEnterEvent* event){
		QListView::dragEnterEvent(event);
	}
	void dropEvent(QDropEvent* event) {
		QListView::dropEvent(event);
	}
};

Q_DECLARE_METATYPE(RListView*)

#endif
