#include "RGlobal.h"
#include "RPropertyAttributes.h"
#include "RColor.h"
#include "RLinetype.h"
#include "RLineweight.h"
#include "RVector.h"
#ifdef Q_OS_WIN32
#include "windows.h"
#endif

const double RS::PointTolerance = 1.0e-9;
const double RS::AngleTolerance = 1.0e-9;

QString RS::getUniqueSystemId() {
#ifdef Q_OS_LINUX
	return QString("linux_%1").arg(getenv("HOSTNAME"));
#endif
#ifdef Q_OS_MAC
	return QString("osx_%1").arg(getenv("HOSTNAME"));
#endif
#ifdef Q_OS_WIN32
	return QString("win32_%1").arg(getenv("COMPUTERNAME"));
#endif
}

QString RS::getLabel(double v){
	if (fabs(v) < 1e-6) {
		v = 0.0;
	}
	QString s;
	s.sprintf("%.6g", v);
	return s;
}

double RS::convert(double value, RS::Unit source, RS::Unit dest) {
	return value * getFactorToM(source) / getFactorToM(dest);
}

RVector RS::convert(const RVector& value, RS::Unit source, RS::Unit dest) {
	return value * convert(1.0, source, dest);
}

double RS::getFactorToM(RS::Unit unit) {
	switch (unit) {
	case RS::None:
		return 1.0;
		break;
	case RS::Inch:
		return 25.4 / 1000;
		break;
	case RS::Foot:
		return 304.8 / 1000;
		break;
	case RS::Mile:
		return 1609344 / 1000;
		break;
	case RS::Millimeter:
		return 1.0 / 1000;
		break;
	case RS::Centimeter:
		return 10 / 1000;
		break;
	case RS::Meter:
		return 1000 / 1000;
		break;
	case RS::Kilometer:
		return 1000000 / 1000;
		break;
	case RS::Microinch:
		return 0.0000254 / 1000;
		break;
	case RS::Mil:
		return 0.0254 / 1000;
		break;
	case RS::Yard:
		return 914.4 / 1000;
		break;
	case RS::Angstrom:
		return 0.0000001 / 1000;
		break;
	case RS::Nanometer:
		return 0.000001 / 1000;
		break;
	case RS::Micron:
		return 0.001 / 1000;
		break;
	case RS::Decimeter:
		return 100.0 / 1000;
		break;
	case RS::Decameter:
		return 10000.0 / 1000;
		break;
	case RS::Hectometer:
		return 100000.0 / 1000;
		break;
	case RS::Gigameter:
		return 1000000000.0 / 1000;
		break;
	case RS::Astro:
		return 149600000000000.0 / 1000;
		break;
	case RS::Lightyear:
		return 9460731798000000000.0 / 1000;
		break;
	case RS::Parsec:
		return 30857000000000000000.0 / 1000;
		break;
	case RS::Plu:
		return 0.025 / 1000;
		break;
	default:
		return 1.0;
		break;
	}
}

bool RS::isMetric(RS::Unit unit) {
	if (unit == RS::Inch || unit == RS::Foot || unit == RS::Mile || unit
			== RS::Mil || unit == RS::Yard) {
		return false;
	}
	return true;
}

bool RS::compare(const QVariant& v1, const QVariant& v2) {
	if (v1.canConvert<RColor> () && v2.canConvert<RColor> ()) {
		return v1.value<RColor> () == v2.value<RColor> ();
	}
	if (v1.canConvert<RLinetype> () && v2.canConvert<RLinetype> ()) {
		return v1.value<RLinetype> () == v2.value<RLinetype> ();
	}
	if (v1.canConvert<RLineweight::Lineweight> () && v2.canConvert<RLineweight::Lineweight> ()) {
		return v1.value<RLineweight::Lineweight> () == v2.value<RLineweight::Lineweight> ();
	}
	return v1 == v2;
}

bool RS::compare(const QPair<QVariant, RPropertyAttributes>& p1, const QPair<
		QVariant, RPropertyAttributes>& p2) {
	return compare(p1.first, p2.first);
}

int RS::getCPUCores(){
	static int cores = -1;

	if(cores!=-1){
		return cores;
	}

#ifdef Q_OS_WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	cores = sysinfo.dwNumberOfProcessors;
#elif defined(Q_OS_UNIX)
	cores = sysconf( _SC_NPROCESSORS_ONLN );
#elif defined (Q_OS_MAC) || defined (Q_OS_BSD4) || defined (Q_OS_FREEBSD) || defined (Q_OS_NETBSD) || defined (Q_OS_OPENBSD)
	nt mib[4];
	size_t len;

	/* set the mib for hw.ncpu */
	mib[0] = CTL_HW;
	mib[1] = HW_AVAILCPU; // alternatively, try HW_NCPU;

	/* get the number of CPUs from the system */
	sysctl(mib, 2, &cores, &len, NULL, 0);

	if( cores < 1 ){
		mib[1] = HW_NCPU;
		sysctl( mib, 2, &cores, &len, NULL, 0 );
	}
#elif defined(Q_OS_HPUX)
	cores = mpctl(MPC_GETNUMSPUS, NULL, NULL);
#elif defined(Q_OS_IRIX)
	cores = sysconf( _SC_NPROC_ONLN );
#endif

    if (cores < 1) {
		cores = 1;
	}
	RDebug::debug(QString("RS::getCPUCores: detected CPU cores: %1").arg(cores));
	return cores;
}
