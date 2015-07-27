#ifndef RNONCOPYABLE_H
#define RNONCOPYABLE_H



/**
 * Instances of classes derived from this base class cannot be copied.
 *
 * \ingroup core
 */
class RNonCopyable {
protected:
    RNonCopyable() {}
    ~RNonCopyable() {}

private:
    RNonCopyable(const RNonCopyable&);
    const RNonCopyable& operator=(const RNonCopyable&);
};

Q_DECLARE_METATYPE(RNonCopyable*)

#endif
