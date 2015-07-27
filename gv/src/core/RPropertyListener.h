#ifndef RPROPERTYLISTENER_H
#define RPROPERTYLISTENER_H

class RDocument;
class REntity;
class RDocument;
class RObject;



/**
 * \brief Abstract base class for classes that are interested in properties 
 * of the current selection or the current action. For example a 
 * property browser or editor.
 *
 * \ingroup core
 * \scriptable
 */
class RPropertyListener {
public:
    virtual ~RPropertyListener() {}

    /**
     * This method is called whenever the relevant combined properties of 
     * the objects in \c container have changed. 
     * 
     * For example this can mean that the selection in a document has 
     * changed and the combined properties of the selected entities 
     * should be displayed in a property browser or editor.
     */
    virtual void updateFromContainer(RDocument* container) = 0;
    
    /**
     * This method is called if only the properties of the given property owner
     * are currently relevant.
     *
     * For example this can be the case when an entity is being drawn and
     * only the properties of that entity should be displayed, so that the user
     * can adjust properties before or while drawing the entity.
     */
    virtual void updateFromObject(RObject* object, RDocument* container =
			NULL) = 0;

    /**
     * This method is called to signal that no properties are currently 
     * relevant. For example that no entities are selected in a document
     * or that nothing is being drawn at the moment.
     */
    virtual void clearEditor() = 0;
};

Q_DECLARE_METATYPE(RPropertyListener*)

#endif
