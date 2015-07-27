#ifndef RLINETYPEPATTERN_H
#define RLINETYPEPATTERN_H

#include <stdarg.h>
#include <math.h>

#include <QMap>
#include <QMetaType>
#include <QString>

class RLinetypePatternMap;

/**
 * Represents a linetype pattern which can be used to draw
 * dashed graphical elements.
 */
class RLinetypePattern {
public:
    RLinetypePattern(int num...);
    RLinetypePattern();
    RLinetypePattern(const RLinetypePattern& other);
    ~RLinetypePattern();
    bool isValid() const;
    int getNumDashes() const;
	double getPatternLength() const;
	double getDashLengthAt(int i) const;
	bool hasDashAt(double pos) const;
	double getDelta(double pos) const;
	bool isSymmetrical(int i) const;
	void scale(double factor);

	RLinetypePattern& operator=(const RLinetypePattern& other);

public:
    /*
	static RLinetypePattern patternDashed;
	static RLinetypePattern patternDashDot;
	static RLinetypePattern patternCenter;
    */

private:
	int num;
	double* pattern;
	bool* symmetrical;
};

/**
 * Provides access to the global, static map of available linetypes.
 * \todo Refactor to use linetypes that are stored in drawings and
 * allow for custom linetypes.
 */
class RLinetypePatternMap: public QMap<QString, RLinetypePattern> {
public:
    RLinetypePatternMap();
    static RLinetypePattern getPattern(const QString& name);

private:
	static RLinetypePatternMap patternMap;
};

Q_DECLARE_METATYPE(RLinetypePattern*)
Q_DECLARE_METATYPE(RLinetypePattern)

#endif
