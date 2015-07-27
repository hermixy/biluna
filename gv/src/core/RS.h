#ifndef RS_H
#define RS_H

#include <QVariant>
#include <QPair>

class RVector;
class RPropertyAttributes;

/**
 * Some useful macros for OS detection.
 */
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define ROS_WINDOWS
#  define ROS_WIN32
#  define ROS_WIN64
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define ROS_WINDOWS
#  define ROS_WIN32
#else
#  define ROS_POSIX
#endif



/**
 * Class namespace for various global enums.
 *
 * Most enums fit in a class, but some are truly global, others
 * cannot be declared in a class because this would result in
 * problems with include files that depend on each other (note that
 * there is no way to forward-declare nested classes or enums).
 *
 * \ingroup core
 * \scriptable
 */
class RS {

public:
    /**
     * Sets the current rendering hint for exports.
     *
     * \see RExporter::setProjectionRenderingHint
     */
    enum ProjectionRenderingHint {
        Top,
        Side,
        Front,
        ThreeD
    };

    /**
     * Projection type for isometric projections.
     */
    enum IsoProjectionType {
        IsoTop,        //!< Top projection (x/y)
        IsoFront,      //!< Front projection (x/z)
        IsoSide        //!< Side projection (y/z)
    };

    enum Unit {
		None = 0, /**< No unit (unit from parent) */
		Inch = 1, /**< Inch */
		Foot = 2, /**< Foot: 12 Inches */
		Mile = 3, /**< Mile: 1760 Yards = 1609 m */
		Millimeter = 4, /**< Millimeter: 0.001m */
		Centimeter = 5, /**< Centimeter: 0.01m */
		Meter = 6, /**< Meter */
		Kilometer = 7, /**< Kilometer: 1000m */
		Microinch = 8, /**< Microinch: 0.000001 */
		Mil = 9, /**< Mil = 0.001 Inch*/
		Yard = 10, /**< Yard: 3 Feet */
		Angstrom = 11, /**< Angstrom: 10^-10m  */
		Nanometer = 12, /**< Nanometer: 10^-9m  */
		Micron = 13, /**< Micron: 10^-6m  */
		Decimeter = 14, /**< Decimeter: 0.1m */
		Decameter = 15, /**< Decameter: 10m */
		Hectometer = 16, /**< Hectometer: 100m */
		Gigameter = 17, /**< Gigameter: 1000000m */
		Astro = 18, /**< Astro: 149.6 x 10^9m */
		Lightyear = 19, /**< Lightyear: 9460731798 x 10^6m */
		Parsec = 20, /**< Parsec: 30857 x 10^12 */
		Plu = 21 /**< PLU: Plotter units used in HP/GL. 40plu = 1mm */
	};

    /**
     * Angle format for printing angles.
     */
    enum AngleFormat {
       DegreeMinuteSeconds,
       Degrees,
       Grads,
       Radians
    };

public:
	static bool compare(const QVariant& v1, const QVariant& v2);
	static bool compare(const QPair<QVariant, RPropertyAttributes>& p1, const QPair<
			QVariant, RPropertyAttributes>& p2);
	static int getCPUCores();
	static bool isMetric(RS::Unit unit);
	static double convert(double value, RS::Unit source, RS::Unit dest);
	static RVector convert(const RVector& value, RS::Unit source, RS::Unit dest);
	static double getFactorToM(RS::Unit unit);
	static QString getLabel(double v);
	static QString getUniqueSystemId();

    static const double PointTolerance;
    static const double AngleTolerance;
};

Q_DECLARE_METATYPE(RS*)
Q_DECLARE_METATYPE(RS::Unit)
Q_DECLARE_METATYPE(RS::IsoProjectionType)
Q_DECLARE_METATYPE(RS::IsoProjectionType*)
Q_DECLARE_METATYPE(RS::ProjectionRenderingHint)

#endif
