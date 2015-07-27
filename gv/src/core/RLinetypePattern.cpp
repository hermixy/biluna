#include "RLinetypePattern.h"

#include "RMath.h"
#include "RDebug.h"

/*
RLinetypePattern RLinetypePattern::patternDashed(2, 0.125, -0.0625);
RLinetypePattern RLinetypePattern::patternCenter(2, 0.125, -0.0625);
RLinetypePattern RLinetypePattern::patternDashDot(2, 0.125, -0.0625);
*/

RLinetypePatternMap RLinetypePatternMap::patternMap;

RLinetypePattern::RLinetypePattern(int num...)
	:num(num), pattern(NULL), symmetrical(NULL) {

    va_list vl;
    va_start(vl, num);
    pattern = new double[num];
    for (int i=0; i<num; ++i) {
		pattern[i] = va_arg(vl, double);
	}
    va_end(vl);

    symmetrical = new bool[num];
    for (int i = 0; i < num; ++i) {
    	symmetrical[i] = true;
		for (int a = 1; a < num; ++a) {
			if(fabs( pattern[ RMath::absmod((i-a), num) ]
			                - pattern[ RMath::absmod((i+a), num) ] ) > 0.1){
				symmetrical[i] = false;
				break;
			}
		}
	}
}

RLinetypePattern::RLinetypePattern() :
	num(-1), pattern(NULL), symmetrical(NULL) {
}

RLinetypePattern::RLinetypePattern(const RLinetypePattern& other) :
	num(-1), pattern(NULL), symmetrical(NULL) {
	operator =(other);
}

RLinetypePattern::~RLinetypePattern() {
	if (pattern != NULL) {
		delete pattern;
	}
	if (symmetrical != NULL) {
		delete symmetrical;
	}
}

RLinetypePattern& RLinetypePattern::operator=(const RLinetypePattern& other) {
	if (this == &other) {
		return *this;
	}

	if (pattern != NULL) {
		delete pattern;
	}
	if (symmetrical != NULL) {
		delete symmetrical;
	}

	num = other.num;
	if (num > 0) {
		pattern = new double[num];
		symmetrical = new bool[num];
		for (int i = 0; i < num; ++i) {
			pattern[i] = other.pattern[i];
			symmetrical[i] = other.symmetrical[i];
		}
	} else {
		pattern = NULL;
		symmetrical = NULL;
	}
	return *this;
}

void RLinetypePattern::scale(double factor) {
	for (int i = 0; i < num; ++i) {
		pattern[i] *= factor;
	}
}

double RLinetypePattern::getDelta(double pos) const {
	if (pos < 0) {
		pos += (trunc(pos / getPatternLength()) + 1) * getPatternLength();
	} else if (pos > getPatternLength()) {
		pos -= trunc(pos / getPatternLength()) * getPatternLength();
	}
	double total = 0.0;
	for (int i = 0; i < num; ++i) {
		total += fabs(pattern[i]);
		if (total >= pos) {
			if (pattern[i] < 0) {
				return -(total - pos);
			}
			return total - pos;
		}
	}
	RDebug::warning(QString("RLinetypePattern::getDelta: invalid pos argument"));
	return 0.0;
}

bool RLinetypePattern::hasDashAt(double pos) const {
	if (pos < 0) {
		pos += (trunc(pos / getPatternLength()) + 1) * getPatternLength();
	} else if (pos > getPatternLength()) {
		pos -= trunc(pos / getPatternLength()) * getPatternLength();
	}
	double total = 0.0;
	for (int i = 0; i < num; ++i) {
		total += fabs(pattern[i]);
		if (total > pos) {
			return pattern[i] > 0;
		}
	}
	RDebug::warning(QString("RLinetypePattern::hasDashAt: invalid pos argument"));
	return false;
}

bool RLinetypePattern::isSymmetrical(int i) const {
	if (i >= num) {
		return false;
	}
	return symmetrical[i];
	//	if (num == 2) {
	//		return true;
	//	}
	//	if (num == 4) {
	//		return (i % 2) == 0;
	//	}
	//	return true;
}

bool RLinetypePattern::isValid() const {
    return num>0;
}

int RLinetypePattern::getNumDashes() const {
    return num;
}

double RLinetypePattern::getPatternLength() const {
    double ret=0;
    for(int i=0;i<num;++i) {
        ret += fabs(pattern[i]);
    }
    return ret;
}

double RLinetypePattern::getDashLengthAt(int i) const {
    if(i>=0 && i<num) {
        return pattern[i];
    }
    return 0.0;
}

RLinetypePatternMap::RLinetypePatternMap() {
    insert("BYLAYER", RLinetypePattern());
    insert("BYBLOCK", RLinetypePattern());

    insert("CONTINUOUS", RLinetypePattern(1, 10.0));

    insert("BORDER", RLinetypePattern(6, 12.0, -6.0, 12.0, -5.95, 0.1, -5.95));
    insert("BORDER2", RLinetypePattern(6, 6.0, -3.0, 6.0, -2.95, 0.1, -2.95));
    insert("BORDERX2", RLinetypePattern(6, 24.0, -12.0, 24.0, -11.95, 0.1, -11.95));

    insert("CENTER", RLinetypePattern(4, 32.0, -6.0, 6.0, -6.0));
    insert("CENTER2", RLinetypePattern(4, 16.0, -3.0, 3.0, -3.0));
    insert("CENTERX2", RLinetypePattern(4, 64.0, -12.0, 12.0, -12.0));

    insert("DASHDOT", RLinetypePattern(4, 12.0, -5.95, 0.1, -5.95));
    insert("DASHDOT2", RLinetypePattern(4, 6.0, -2.95, 0.1, -2.95));
    insert("DASHDOTX2", RLinetypePattern(4, 24.0, -11.95, 0.1, -11.95));

    insert("DASHED", RLinetypePattern(2, 12.0, -6.0));
    insert("DASHED2", RLinetypePattern(2, 6.0, -3.0));
    insert("DASHEDX2", RLinetypePattern(2, 24.0, -12.0));

    insert("DIVIDE", RLinetypePattern(6, 12.0, -5.9, 0.15, -5.9, 0.15, -5.9));
    insert("DIVIDE2", RLinetypePattern(6, 6.0, -2.9, 0.15, -2.9, 0.15, -2.9));
    insert("DIVIDEX2", RLinetypePattern(6, 24.0, -11.9, 0.15, -11.9, 0.15, -11.9));

    insert("DOT", RLinetypePattern(2, 0.1, -6.2));
    insert("DOT2", RLinetypePattern(2, 0.1, -3.1));
    insert("DOTX2", RLinetypePattern(2, 0.1, -12.4));
}

RLinetypePattern RLinetypePatternMap::getPattern(const QString& name) {
    if (patternMap.contains(name.toUpper())) {
        return patternMap[name.toUpper()];
    }
    return RLinetypePattern();
}
