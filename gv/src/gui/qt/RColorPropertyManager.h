#ifndef RCOLORPROPERTYMANAGER_H_
#define RCOLORPROPERTYMANAGER_H_

//#include <QtAbstractPropertyManager>
//#include <QtProperty>
#include "qtpropertybrowser.h"
#include <QString>
#include <QIcon>

#include "RColor.h"

class RColorPropertyManager: public QtAbstractPropertyManager {

Q_OBJECT

public:
	RColorPropertyManager(QObject* parent = NULL);
	virtual RColor value(const QtProperty * property) const;

signals:
	void valueChanged(QtProperty* property, const RColor color);

public slots:
	void setValue(QtProperty * property, const RColor& color);
	void setMixedValue(QtProperty * property);

protected:
	virtual void initializeProperty(QtProperty * property);
	virtual void uninitializeProperty(QtProperty * property);
	virtual QIcon valueIcon(const QtProperty * property) const;
	virtual QString valueText(const QtProperty * property) const;

private:
    // TODO color as map (there might be more than one color property in one browser):
	RColor color;
};

#endif /* RCOLORPROPERTYMANAGER_H_ */
