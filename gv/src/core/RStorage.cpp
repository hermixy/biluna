#include "RStorage.h"

RStorage::RStorage() :
    currentColor(RColor::ByLayer),
    currentLineweight(RLineweight::WeightByLayer),
    currentLinetype(RLinetype(NULL, "BYLAYER")),
    currentViewId(RView::INVALID_ID),
    currentBlockId(RBlock::INVALID_ID),
    lastTransactionId(-1) {
}

void RStorage::setCurrentColor(const RColor& color) {
    //setVariable("CurrentColor", color);
    currentColor = color;
}

RColor RStorage::getCurrentColor() {
    return currentColor;
}

void RStorage::setCurrentLineweight(RLineweight::Lineweight lw) {
    currentLineweight = lw;
}

RLineweight::Lineweight RStorage::getCurrentLineweight() {
    return currentLineweight;
}

void RStorage::setCurrentLinetype(RLinetype lt) {
    currentLinetype = lt;
}

RLinetype RStorage::getCurrentLinetype() {
    return currentLinetype;
}

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, RStorage& s) {
    //dbg.nospace() << "RStorage(" << QString("%1").arg((int)&s, 0, 16) << ", ";
    dbg.nospace() << "\n";

	QSet<RLayer::Id> layers = s.queryAllLayers(true);
	QSetIterator<RLayer::Id> i(layers);
	while (i.hasNext()) {
        RLayer::Id id = i.next();
        QSharedPointer<RLayer> l = s.queryObjectDirect(id).dynamicCast<RLayer>();
        if (l.isNull()) {
            dbg.nospace() << "layer not found: " << id;
            continue;
        }
        dbg.nospace() << *l.data() << "\n";
	}

	QSet<RLayer::Id> views = s.queryAllViews(true);
	QSetIterator<RView::Id> i4(views);
	while (i4.hasNext()) {
        RView::Id id = i4.next();
        QSharedPointer<RView> v = s.queryObjectDirect(id).dynamicCast<RView>();
        if (v.isNull()) {
            dbg.nospace() << "view not found: " << id;
            continue;
        }
        dbg.nospace() << *v.data() << "\n";
    }

	QSet<RLayer::Id> blocks = s.queryAllBlocks(true);
	QSetIterator<RBlock::Id> i3(blocks);
	while (i3.hasNext()) {
        RBlock::Id id = i3.next();
        QSharedPointer<RBlock> b = s.queryObjectDirect(id).dynamicCast<RBlock>();
        if (b.isNull()) {
            dbg.nospace() << "block not found: " << id;
            continue;
        }
        dbg.nospace() << *b.data() << "\n";
    }

    QSet<REntity::Id> entities = s.queryAllEntities(true, true);
	QSetIterator<REntity::Id> i2(entities);
	while (i2.hasNext()) {
        REntity::Id id = i2.next();
        QSharedPointer<REntity> e = s.queryObjectDirect(id).dynamicCast<REntity>();
        if (e.isNull()) {
            dbg.nospace() << "entity not found: " << id;
            continue;
        }
        dbg.nospace() << *e.data() << "\n";
    }

	dbg.nospace() << "lastTransactionId: " << s.getLastTransactionId() << "\n";
	for (int a = 0; a <= s.getMaxTransactionId(); ++a) {
		RTransaction t = s.getTransaction(a);
		dbg.nospace() << t << "\n";
	}

	dbg.nospace() << ")";
	return dbg.space();
}

