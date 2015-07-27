#ifndef RRULER_H
#define RRULER_H

class RGraphicsView;

/**
 * Abstract base class for widgets that display a ruler, usually shown at 
 * the top and left of a graphics view.
 *
 * \scriptable
 */
class RRuler {

public:
	RRuler() :
		view(NULL), orientation(Qt::Horizontal) {
	}
	virtual ~RRuler() {
	}

	void setGraphicsView(RGraphicsView* view) {
		this->view = view;
	}

	Qt::Orientation getOrientation() const {
		return orientation;
	}

	void setOrientation(Qt::Orientation orientation) {
		this->orientation = orientation;
	}

	virtual void paintTick(int pos, bool major, const QString& label) = 0;

	virtual QFont getFont() const = 0;

protected:
	RGraphicsView* view;
	Qt::Orientation orientation;
};

Q_DECLARE_METATYPE(RRuler*)

#endif
