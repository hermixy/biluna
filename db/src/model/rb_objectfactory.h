/*****************************************************************
 * $Id: rb_objectfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 17, 2006 12:34:25 AM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_OBJECTFACTORY_H
#define RB_OBJECTFACTORY_H

#include <iostream>
#include <string>
#include <vector>

#include "rb.h"
#include "rb_string.h"
#include "rb_stringlist.h"
#include "rb_variant.h"

// classes referenced with circular reference to this class
class RB_ObjectBase;
class RB_ObjectContainer;


/**
 * Interface class with Rb_ObjectAtomic, RB_ObjectBase, RB_ObjectContainer,
 * RB_ObjectIterator, RB_ObjectMember model base objects. Inherit this class
 * when building new models
 */
class DB_EXPORT RB_ObjectFactory {

public:
    RB_ObjectFactory();
    virtual ~RB_ObjectFactory();

    /**
     * Create new object, does nothing.
     * Function parameter originates from reading XML files
     * Note: Function to be overridden by implementation class.
     * @param id identification of object
     * @param parent parent container of object
     * @param type type of object (in case of EQL)
     * @param addToParent flag indicating whether object should be added
     * to parent list, default is true
     * @param withContainers create object with containers for lists,
     * in case of a selection dialog with a treeview this is set to false.
     * Default is true.
     * @return object
     */
    virtual RB_ObjectBase* newObject(const RB_String& id,
                                     RB_ObjectBase* parent,
                                     const RB_String& typeName = "",
                                     bool addToParent = true) = 0;
    virtual RB_ObjectBase* newSingleObject(const RB_String& objectName);
    virtual RB_ObjectBase* newParentObject(const RB_String& objectName);
    virtual RB_ObjectBase* shallowCopy(RB_ObjectBase* object);
    virtual RB_ObjectBase* deepCopy(RB_ObjectBase* object,
                                    RB2::ResolveLevel level);
    virtual bool createNewId();
    virtual void setCreateNewId(bool createId);
    virtual bool isObject(const RB_String& objectName);
    virtual void tableNameToObjectName(RB_String& name);

    /**
     * Get main list name of this model, does nothing.
     * Note: Function to be overridden by implementation class.
     * @return main list name
     */
    virtual RB_String mainListName() const = 0; // { return RB_String(); }

    RB_ObjectBase* getModelStructure(const RB_String& objName);
    virtual RB_String getName() const;
    virtual RB_String getPerspective() const;
    virtual RB_String getVersion() const;

    bool isSystemSetting(const RB_String& name);
    bool isProjectSetting(const RB_String& name);


    // do not use below function
//    virtual RB_ObjectBase* createObject(const RB_String& id,
//                                RB_ObjectContainer* parent,
//                                const RB_String& objectName);

protected:
    RB_ObjectBase* createModelStructure(const RB_String& objName);
    void buildModelStructure(RB_ObjectBase* obj);
    RB_ObjectBase* resolveCopy(RB_ObjectBase* object, RB2::ResolveLevel level);
    bool isFactoryInitiated();
    RB2::ResolveLevel resolveNext(RB2::ResolveLevel level);
    void setName(const RB_String& factoryName);

private:
    //! Container only used to pass container name to create object
    RB_ObjectContainer* mObjC;
    //! Container of objects part of this factory
    // RB_ObjectContainer* mObjList;
    std::vector<RB_String> mObjectNameList;
    //! Object list root
    RB_ObjectContainer* mRoot;
    //! Model is initiated flag
    bool mFactoryIsInitiated;
    //! Create new id is default false for clone(), but true for copy()
    bool mCreateNewId;
    //! Name of this object factory, same as class name e.g. DB_ObjectFactory
    RB_String mName;
};

#endif /*RB_OBJECTFACTORY_H*/
