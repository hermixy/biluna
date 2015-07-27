#ifndef RVIEw_H
#define RVIEw_H

#include <QString>
#include <QColor>
#include <QDebug>

#include "RGlobal.h"
#include "RObject.h"
#include "RBox.h"

class RDocument;

/**
 * Represents a view in a drawing.
 *
 * \ingroup core
 * \scriptable
 */
class RView: public RObject {
public:
	static RPropertyTypeId PropertyName;
	static RPropertyTypeId PropertyCenterPoint;
	static RPropertyTypeId PropertyWidth;
	static RPropertyTypeId PropertyHeight;

public:
	RView();

    RView(RDocument* document, const QString& name, RVector centerPoint,
        double width, double height);

	virtual ~RView();

    static void init();

    virtual RView* clone();

    QString getName() const {
        return name;
    }

    void setName(const QString& n){
    	name = n;
    }

    RVector getCenterPoint() const {
		return centerPoint;
	}

	void setCenterPoint(const RVector& cPoint) {
		centerPoint = cPoint;
	}

	double getWidth() const {
		return width;
	}

	void setWidth(double w) {
		width = w;
	}

	double getHeight() const {
		return height;
	}

	void setHeight(double h) {
		height = h;
	}

	RBox getBox(){
		return RBox(centerPoint - RVector(width, height) / 2, centerPoint
				+ RVector(width, height) / 2);
	}

	virtual QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);
	virtual bool setProperty(RPropertyTypeId propertyTypeId,
            const QVariant& value);

	virtual bool isSelectedForPropertyEditing();

private:
    RDocument* document;
	QString name;
	RVector centerPoint;
	double width;
	double height;
};

QDebug operator<<(QDebug dbg, const RView& v);

Q_DECLARE_METATYPE(QSharedPointer<RView>)
Q_DECLARE_METATYPE(QSharedPointer<RView>*)
Q_DECLARE_METATYPE(RView)
Q_DECLARE_METATYPE(RView*)

#endif
