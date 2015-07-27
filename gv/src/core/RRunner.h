#ifndef RRUNNER_H_
#define RRUNNER_H_

/**
 * \scriptable
 * \generateScriptShell
 */
class RRunner {
	friend class RThread;
protected:
	virtual void run() {
		// TODO should be pure virtual
	}
};

Q_DECLARE_METATYPE(RRunner*)

#endif /* RRUNNER_H_ */
