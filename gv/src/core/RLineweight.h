#ifndef RLINEWEIGHT_H_
#define RLINEWEIGHT_H_

#include <QCoreApplication>
#include <QMetaType>
#include <QString>
#include <QIcon>
#include <QPair>
#include <QMap>
#include <QList>

/**
 * \scriptable
 * \copyable
 */
class RLineweight {

	Q_DECLARE_TR_FUNCTIONS(RLineweight);

public:
    enum Lineweight {
		Weight000 = 0,
		Weight005 = 5,
		Weight009 = 9,
		Weight013 = 13,
		Weight015 = 15,
		Weight018 = 18,
		Weight020 = 20,
		Weight025 = 25,
		Weight030 = 30,
		Weight035 = 35,
		Weight040 = 40,
		Weight050 = 50,
		Weight053 = 53,
		Weight060 = 60,
		Weight070 = 70,
		Weight080 = 80,
		Weight090 = 90,
		Weight100 = 100,
		Weight106 = 106,
		Weight120 = 120,
		Weight140 = 140,
		Weight158 = 158,
		Weight200 = 200,
		Weight211 = 211,
		WeightByLayer = -1,
		WeightByBlock = -2,
		WeightByLwDefault = -3,
		WeightInvalid = -4
	};

public:
	RLineweight();
	static QList<QPair<QString, RLineweight::Lineweight> > getList(bool onlyFixed);
	static QIcon getIcon(RLineweight::Lineweight color);
	static QString getName(RLineweight::Lineweight lineweight);

private:
	static void init();
	static void init(const QString& cn, RLineweight::Lineweight lineweight);

private:
	static bool isInitialized;
	static QList<QPair<QString, RLineweight::Lineweight> > list;
	static QMap<RLineweight::Lineweight, QIcon> iconMap;

};

Q_DECLARE_METATYPE(RLineweight)
Q_DECLARE_METATYPE(RLineweight*)
Q_DECLARE_METATYPE(RLineweight::Lineweight)
typedef QPair<QString, RLineweight::Lineweight> _RPairStringRLineweight;
Q_DECLARE_METATYPE(QList< _RPairStringRLineweight >)

#endif /* RLINEWEIGHT_H_ */
