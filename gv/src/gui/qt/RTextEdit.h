#ifndef RTEXTEDIT_H_
#define RTEXTEDIT_H_

#include <QTextEdit>
#include <QtDesigner/QDesignerExportWidget>

/**
 * \scriptable
 */
class /*QDESIGNER_WIDGET_EXPORT*/ RTextEdit: public QTextEdit {

Q_OBJECT

public:
	RTextEdit(QWidget* parent);
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;
};

Q_DECLARE_METATYPE(RTextEdit*)

#endif
