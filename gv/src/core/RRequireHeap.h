#ifndef RREQUIREHEAP_H_
#define RREQUIREHEAP_H_

#include "RDebug.h"

/**
 * \ingroup core
 */
class RRequireHeap {
public:
	void doDelete() {
		delete this;
	}

protected:
	virtual ~RRequireHeap(){
	}
};

Q_DECLARE_METATYPE(RRequireHeap*)

#endif /* RREQUIREHEAP_H_ */
