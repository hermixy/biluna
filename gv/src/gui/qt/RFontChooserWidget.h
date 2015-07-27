#ifndef _RFONTCHOOSERWIDGET_H_
#define _RFONTCHOOSERWIDGET_H_

#include <QComboBox>
#include <QFont>
#include <QFontComboBox>

class QLabel;

/**
 * \scriptable
 */
class RFontChooserWidget: public QWidget {

Q_OBJECT
//Q_PROPERTY(bool onlyFixed READ getOnlyFixed WRITE setOnlyFixed);
Q_PROPERTY(QString label READ getLabel WRITE setLabel);

public:
	RFontChooserWidget(QWidget *parent = 0);

	QFont getFont() const;
	void setFont(const QFont& font);
	int getSize() const;
	void setSize(int size);
    QString getLabel() const;
    void setLabel(const QString& l);

signals:
	void valueChanged(const QFont& font);

public slots:
	void fontChanged(const QFont& font);
	void sizeChanged(int index);

protected:
    void resizeEvent(QResizeEvent* event);

private:
	void updateSizeCombo();

private:
	QFont font;
	QComboBox* cbSize;
	QFontComboBox* cbFont;
	QLabel* lbSampleText;
	QLabel* lbLabel;
};

Q_DECLARE_METATYPE(RFontChooserWidget*)

#endif
