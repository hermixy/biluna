#include <QRegExp>
#include <QScriptEngine>

#include "RDebug.h"
#include "RMath.h"


QString RMath::lastError = "";

int RMath::absmod(int a, int b) {
	if (b == 0) {
		return a;
	}
	int m = a % b;
	if ((b < 0 && m > 0) || (b > 0 && m < 0)) {
		return b + m;
	}
	return m;
}

/**
 * Safe pow function.
 *
 * \param x the base
 *
 * \param y the exponent
 *
 * \return The value of x raised to the power of y.
 */
double RMath::pow(double x, double y) {
	errno = 0;
	double ret = ::pow(x, y);
	if (errno == EDOM) {
		RDebug::error("RMath::pow: EDOM in pow");
		ret = 0.0;
	} else if (errno == ERANGE) {
		RDebug::warning("RMath::pow: ERANGE in pow");
		ret = 0.0;
	}
	return ret;
}

bool RMath::isNormal(double v) {
    if (std::fpclassify(v)==FP_NAN ||
        std::fpclassify(v)==FP_INFINITE) {
        return false;
    }
    return true;
}

double RMath::eval(const QString& expression, bool* ok) {
    lastError = "";

    if (expression.isEmpty()) {
        if (ok!=NULL) {
            *ok = false;
        }
        lastError = "Expression is empty";
        //qDebug() << "RMath::evel: error: " << lastError;
        return 0.0;
    }

    if (ok!=NULL) {
        *ok = true;
    }

    QString expr = expression;
    int idx = -1;

    // convert surveyor type angles (e.g. N10d30'12.5"E) to degrees:
    if (expr.contains(QRegExp("[NESW]", Qt::CaseInsensitive))) {
        // \b(?:(?:([NS])(?:([+-]?)(?:(?:(\d*\.?\d*)[d°])?(?:(\d*\.?\d*)')?(?:(\d*\.?\d*)")?|(\d*))([EW]))?)|([EW]))\b
        QRegExp re(
            "\\b"                               // a word
            "(?:"
              "(?:"
                "([NS])"                        // expression starts with nord or south
                "(?:"
                  "([+-]?)"                     // sign
                  "(?:"
                    "(?:(\\d*\\.?\\d*)[d°])?"   // degrees with d
                    "(?:(\\d*\\.?\\d*)')?"      // minutes with '
                    "(?:(\\d*\\.?\\d*)\")?"     // seconds with "
                    "|"                         // or...
                    "(\\d*)"                    // degrees without d
                  ")"
                  "([EW])"                      // east or west
                ")?"
              ")"
              "|"                               // or...
              "([EW])"                          // only east (0d) or west (180d)
            ")"
            "\\b",
            Qt::CaseInsensitive, QRegExp::RegExp2);
        do {
            idx = re.indexIn(expr);
            if (idx==-1) {
                break;
            }
            double angle = 0.0;
            QString sign;

            // "E" or "W":
            if (!re.cap(8).isEmpty()) {
                if (re.cap(8).toUpper()=="E") {
                    angle = 0.0;
                }
                else if (re.cap(8).toUpper()=="W") {
                    angle = 180.0;
                }
                else {
                    if (ok!=NULL) {
                        *ok = false;
                    }
                    lastError = "Invalid cardinal direction found";
                    return 0.0;
                }
            }
            // "[NS]...[EW]":
            else {
                bool north = re.cap(1).toUpper()=="N";
                bool south = re.cap(1).toUpper()=="S";
                sign = re.cap(2);
                double degrees = 0.0;
                double minutes = 0.0;
                double seconds = 0.0;
                if (!re.cap(6).isEmpty()) {
                    degrees = re.cap(6).toDouble(ok);
                }
                else {
                    degrees = re.cap(3).toDouble(ok);
                    minutes = re.cap(4).toDouble(ok);
                    seconds = re.cap(5).toDouble(ok);
                }
                bool east = re.cap(7).toUpper()=="E";
                bool west = re.cap(7).toUpper()=="W";

                double base = (north ? 90.0 : 270.0);
                int dir = ((north && west) || (south && east) ? 1 : -1);
                angle = base + dir * (degrees + minutes/60.0 + seconds/3600.0);
            }

            expr.replace(
                re.cap(),
                QString("%1%2").arg(sign).arg(angle, 0, 'g', 16)
            );
        } while(idx!=-1);
    }

    //qDebug() << "RMath::eval: expression 001 is: " << expr;

    // convert radiant angles (e.g. "1.2r") to degrees:
    {
        QRegExp re("((?:\\.\\d+)|(?:\\d+\\.\\d*)|(?:\\d+))r\\b", Qt::CaseInsensitive, QRegExp::RegExp2);
        do {
            idx = re.indexIn(expr);
            if (idx==-1) {
                break;
            }
            QString match = re.cap(1);
            //qDebug() << "RMath::eval: match 001a is: " << match;
            expr.replace(
                re,
                QString("%1").arg(rad2deg(match.toDouble(ok)), 0, 'g', 16)
            );
            //qDebug() << "RMath::eval: expression 001a is: " << expr;
        } while(idx!=-1);
    }

    //qDebug() << "RMath::eval: expression 002 is: " << expr;

    // convert grad angles (e.g. "100g") to degrees:
    {
        QRegExp re("((?:\\.\\d+)|(?:\\d+\\.\\d*)|(?:\\d+))g\\b", Qt::CaseInsensitive, QRegExp::RegExp2);
        do {
            idx = re.indexIn(expr);
            if (idx==-1) {
                break;
            }
            QString match = re.cap(1);
            expr.replace(
                re,
                QString("%1").arg(gra2deg(match.toDouble(ok)), 0, 'g', 16)
            );
        } while(idx!=-1);
    }

    //qDebug() << "RMath::eval: expression 003 is: " << expr;

    // convert explicitely indicated degree angles (e.g. "90d") to degrees:
    {
        QRegExp re("((?:\\.\\d+)|(?:\\d+\\.\\d*)|(?:\\d+))d\\b", Qt::CaseInsensitive, QRegExp::RegExp2);
        do {
            idx = re.indexIn(expr);
            if (idx==-1) {
                break;
            }
            QString match = re.cap(1);
            expr.replace(
                re,
                QString("%1").arg(match.toDouble(ok), 0, 'g', 16)
            );
        } while(idx!=-1);
    }

    //qDebug() << "RMath::eval: expression is: " << expr;

    QScriptEngine e;
    QScriptValue res = e.evaluate(expr);

    if (res.isError()) {
        if (ok!=NULL) {
            *ok = false;
        }
        lastError = res.toString();
        //qDebug() << "RMath::evel: error: " << lastError;
        return RNANDOUBLE;
    }

    if (!res.isNumber()) {
		if (ok != NULL) {
			*ok = false;
		}
		lastError = expr + " is not a number";
        //qDebug() << "RMath::evel: error: " << lastError;
        return RNANDOUBLE;
	}

    //qDebug() << "res.toNumber(): " << res.toNumber();
    //qDebug() << "fpclassify: " << std::fpclassify(res.toNumber());

    if (!isNormal(res.toNumber())) {
        if (ok != NULL) {
            *ok = false;
        }
        lastError = expr + " is not a normal number";
        //qDebug() << "RMath::evel: error: " << lastError;
        return RNANDOUBLE;
    }

    return res.toNumber();
}

QString RMath::getError() {
    return lastError;
}

bool RMath::hasError() {
    return !lastError.isEmpty();
}

/**
 * Converts the given string to an angle. Accepted
 * formats are:
 * #.## for an angle in degrees,
 * #.##r for an angle in rad,
 * #.##g for an angle in gon,
 * ##d##.#'##.##" for an angle in degrees, minutes, seconds,
 * N##d##'##"E, S##D##'##"E for suveyors units.
 */
/*double RMath::stringToAngle(const QString& str) {
    if (str.isEmpty()) {
        return 0.0;
    }

    if (str.endsWith('r', Qt::CaseInsensitive)) {

    }

}*/

/**
 * Converts the given angle in rad to the given angle format.
 * \todo Support different formats.
 */
QString RMath::angleToString(double a) {
    return QString("%1").arg(rad2deg(a));
}


/**
 * Converts radians to degrees.
 *
 * \param a angle in radians
 */
double RMath::rad2deg(double a) {
	return (a / (2.0 * M_PI) * 360.0);
}

/**
 * Converts grads to degrees.
 *
 * \param a angle in grad (gon)
 */
double RMath::gra2deg(double a) {
    return a / 400.0 * 360.0;
}

/**
 * Converts degrees to radians.
 *
 * \param a angle in degrees
 */
double RMath::deg2rad(double a) {
	return ((a / 360.0) * (2.0 * M_PI));
}

/**
 * Converts radians to gradians.
 *
 * \param a angle in radians
 */
double RMath::rad2gra(double a) {
	return (a / (2.0 * M_PI) * 400.0);
}

/**
 * Finds greatest common divider using Euclid's algorithm.
 * \sa http://en.wikipedia.org/wiki/Greatest_common_divisor
 *
 * \param a the first number
 *
 * \param b the second number
 *
 * \return The greatest common divisor of \c a and \c b.
 */
int RMath::getGcd(int a, int b) {
	int rem;

	while (b != 0) {
		rem = a % b;
		a = b;
		b = rem;
	}

	return a;
}

/**
 * Tests if angle a is between a1 and a2. a, a1 and a2 must be in the
 * range between 0 and 2*PI.
 * All angles in rad.
 *
 * \param a the test angle
 *
 * \param a1 the lower limiting angle
 *
 * \param a2 the upper limiting angle
 *
 * \param reversed True for clockwise testing. False for ccw testing.
 *
 * \return true if the angle a is between a1 and a2.
 */
bool RMath::isAngleBetween(double a, double a1, double a2, bool reversed) {

	bool ret = false;

	if (reversed) {
		double tmp = a1;
		a1 = a2;
		a2 = tmp;
	}

	if (a1 >= a2 - 1.0e-12) {
		if (a >= a1 - 1.0e-12 || a <= a2 + 1.0e-12) {
			ret = true;
		}
	} else {
		if (a >= a1 - 1.0e-12 && a <= a2 + 1.0e-12) {
			ret = true;
		}
	}
	return ret;
}

/**
 * Gets the normalized angle from \c a.
 * Used to make sure that an angle is in the range between 0 and 2 pi.
 *
 * \param a the unnormalized angle, e.g. 8
 *
 * \return The angle \c a normalized to the range of \f$ 0 \ldots 2\pi \f$,
 * e.g. normalized angle from 8 is 1.716.
 */
double RMath::getNormalizedAngle(double a) {

	if (a >= 0.0) {
		int n = (int) floor(a / (2*M_PI));
		a -= 2*M_PI * n;
	} else {
		int n = (int) ceil(a / (-2*M_PI));
		a += 2*M_PI * n;
	}

	return a;
}

/**
 *
 * \param a1 first angle in rad
 *
 * \param a2 s second angle in rad
 *
 * \return The angle that needs to be added to a1 to reach a2.
 *         Always positive and less than 2*pi.
 */
double RMath::getAngleDifference(double a1, double a2) {
	double ret;

	if (a1 >= a2) {
		a2 += 2*M_PI;
	}
	ret = a2 - a1;

	if (ret >= 2*M_PI) {
		ret = 0.0;
	}

	return ret;
}

/**
 * \param a1 first angle in rad
 *
 * \param a2 s second angle in rad
 *
 * \return The angle that needs to be added to a1 to reach a2.
 *         Always between -pi and pi.
 */
double RMath::getAngleDifference180(double a1, double a2) {
	double ret;

	ret = a2 - a1;
	if (ret > M_PI) {
		ret = -(2*M_PI - ret);
	}
	if (ret < -M_PI) {
		ret = 2*M_PI + ret;
	}

	return ret;
}

/**
 * Adds 180° to the given angle if a text constructed with that angle
 * otherwise wouldn't be readable.
 * Used for dimension texts and for mirroring texts.
 *
 * \param angle the original angle
 *
 * \param readable true: make angle readable, false: unreadable
 *
 * \param corrected Pointer to boolean that will point to true if the given angle was
 *   corrected, false otherwise, or null.
 *
 * \return The given angle or the given \c angle + pi, depending which one
 * is readable from the bottom or right.
 */
double RMath::makeAngleReadable(double angle, bool readable, bool* corrected) {

	double ret;

	bool cor = isAngleReadable(angle) ^ readable;

	// quadrant 1 & 4
	if (!cor) {
		ret = angle;
	}
	// quadrant 2 & 3
	else {
		ret = angle + M_PI;
	}

	if (corrected != NULL) {
		*corrected = cor;
	}

	return ret;
}

/**
 * \param angle the angle in rad
 *
 * \param tolerance The tolerance by which the angle still maybe
 * in the unreadable range.
 *
 * \return true: If the given angle is in a range that is readable
 * for texts created with that angle.
 */
bool RMath::isAngleReadable(double angle, double tolerance) {
	double angleCorrected = getNormalizedAngle(angle);
	if (angleCorrected > M_PI / 2.0 * 3.0 + tolerance || angleCorrected < M_PI
			/ 2.0 + tolerance) {
		return true;
	} else {
		return false;
	}
}

/**
 * Tests if two angels point approximately in the same direction.
 *
 * \param dir1 first direction
 *
 * \param dir2 second direction
 *
 * \param tolerance Tolerance in rad.
 *
 * \retval true The two angles point in the same direction.
 *
 * \retval false The difference between the two angles is at
 * least \c tolerance radians.
 */
bool RMath::isSameDirection(double dir1, double dir2, double tolerance) {
	double diff = fabs(dir1 - dir2);
	if (diff < tolerance || diff > 2*M_PI - tolerance) {
		return true;
	} else {
		return false;
	}
}


void RMath::toFraction(double v, int maxDenominator, int& number, int& numerator, int& denominator) {
    int in = (int)v;
    number = in;

    if (in==v) {
        number = in;
        numerator = 0;
        denominator = 1;
        return;
    }

    simplify(abs(mround((v-in)*maxDenominator)), maxDenominator, numerator, denominator);
}

void RMath::simplify(int numerator, int denominator, int& numeratorRes, int& denominatorRes) {
    int g = getGcd(numerator, denominator);
    numeratorRes = numerator/g;
    denominatorRes = denominator/g;
}


bool RMath::fuzzyCompare(double v1, double v2, double tolerance) {
    return fabs(v1-v2) < tolerance;
}
