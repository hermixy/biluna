#ifndef ROPERATION_H
#define ROPERATION_H

class RDocument;
class RExporter;
class RTransaction;



/**
 * This abstract class is an interface for all operation implementations.
 * An operation encapsulates a modification that can be applied to a 
 * document. An implementing class might for example implement a move
 * operation that moves all selected entities by a given vector.
 * 
 * \ingroup core
 * \scriptable
 */
class ROperation {
public:
    ROperation(bool undoable=true) : undoable(undoable) {}
    virtual ~ROperation() {}
    
    /**
     * Applies this operation to selection in the given document.
     */
    virtual RTransaction apply(RDocument& document) const = 0;
    
    /**
     * Previews this operation (as it would be applied to the selection in the
     * given document) be exporting the result to the given exporter. The
     * exporter typically exports into the preview of a graphics scene.
     */
    virtual void preview(RDocument& document, RExporter& exporter) const = 0;

protected:
    bool undoable;
};

Q_DECLARE_METATYPE(ROperation*)

#endif
