#ifndef RTRANSACTIONSTACK_H
#define RTRANSACTIONSTACK_H

#include <QMetaType>

#include "RNonCopyable.h"
#include "RTransaction.h"

class RDocument;

/**
 * A transaction stack handles transactions on a document. Transactions
 * are stored in a \ref RStorage "storage" implementation. 
 * Transactions can be undone and redone.
 *
 * \ingroup core
 */
class RTransactionStack : RNonCopyable {
public:
    RTransactionStack(RDocument& document);
    ~RTransactionStack();

    void reset();

    RTransaction undo();
    RTransaction redo();
    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

private:
    RDocument& document;
};

Q_DECLARE_METATYPE(RTransactionStack*)

#endif
