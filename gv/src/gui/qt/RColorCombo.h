#ifndef RCOLORCOMBO_H_
#define RCOLORCOMBO_H_

#include <QComboBox>

#include "RColor.h"

/**
 * \scriptable
 */
class RColorCombo: public QComboBox {

Q_OBJECT
Q_PROPERTY(bool onlyFixed READ getOnlyFixed WRITE setOnlyFixed);
Q_PROPERTY(bool blackWhite READ isBlackWhite WRITE setBlackWhite);
Q_PROPERTY(bool showAlphaChannel READ getShowAlphaChannel WRITE setShowAlphaChannel);
Q_PROPERTY(QColor color READ getColor WRITE setColor);

public:
	RColorCombo(QWidget *parent = 0);

	RColor getColor();
	void setColor(const RColor& col);
	void init();
	bool getOnlyFixed();
	void setOnlyFixed(bool onlyFixed);
	bool isBlackWhite();
	void setBlackWhite(bool blackWhite);
	bool getShowAlphaChannel();
	void setShowAlphaChannel(bool showAlphaChannel);

signals:
	void valueChanged(const RColor& color);
	void dialogRunning(bool isRunning);

public slots:
	void colorChanged(int index);

private:
	RColor currentColor;
	bool onlyFixed;
	bool blackWhite;
	bool showAlphaChannel;
};

Q_DECLARE_METATYPE(RColorCombo*)

#endif /* RCOLORCOMBO_H_ */
