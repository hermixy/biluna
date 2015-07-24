/*****************************************************************
 * $Id: peng_modelfactory.cpp 2233 2015-04-29 19:10:59Z rutger $
 * Created: Apr 15, 2010 11:29 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_modelfactory.h"

#include "db_modelfactory.h"
#include "peng_objectfactory.h"
#include "peng_sqlcommonfunctions.h"
#include "rb_database.h"

PENG_ModelFactory* PENG_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
PENG_ModelFactory::PENG_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("PENG_ModelFactory::PENG_ModelFactory()");
    this->setObjectFactory(PENG_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PENG_ModelFactory::~PENG_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PENG_ModelFactory::~PENG_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
PENG_ModelFactory* PENG_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new PENG_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as a shared/single model
 * @return model manager
 */
RB_MmProxy* PENG_ModelFactory::getModel(int type, bool shared) {
    RB_MmProxy* model = NULL;

    if (shared) {
        std::map<int, RB_MmProxy*>::iterator iter;
        iter = mModelList.find(type);
        if (iter != mModelList.end()) {
            // shared model 1- exists or 2- has been removed/deleted and set to NULL
            model = (*iter).second;
        }
        // Model already created
        if (model) return model;
    }

    QSqlDatabase db = RB_DATABASE->database(mDatabaseConnection);

    switch (type) {
    case ModelEngCone:
        model = getTableModel(db, mObjectFactory, type, "PENG_ConeList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEngWeldolet:
        model = getTableModel(db, mObjectFactory, type, "PENG_WeldoletList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEquipment:
        model = getTableModel(db, mObjectFactory, type, "PENG_EquipmentList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelEquipmentType:
        model = getTableModel(db, mObjectFactory, type, "PENG_TypeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "typename");
        break;
    case ModelFluid:
        model = getTableModel(db, mObjectFactory, type, "PENG_FluidList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "code");
        break;
    case ModelFluidStability:
        model = getTableModel(db, mObjectFactory, type, "PENG_FluidStabilityList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "code");
        break;
    case ModelHazardGroup:
        model = getTableModel(db, mObjectFactory, type, "PENG_HazardGroupList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "code");
        break;
    case ModelInsulationCC:
        model = getTableModel(db, mObjectFactory, type, "PENG_InsulationCCList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "linesize");
        break;
    case ModelInsulationHCI:
        model = getTableModel(db, mObjectFactory, type, "PENG_InsulationHCIList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "linesize");
        break;
    case ModelInsulationHCO:
        model = getTableModel(db, mObjectFactory, type, "PENG_InsulationHCOList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "linesize");
        break;
    case ModelInsulationPP:
        model = getTableModel(db, mObjectFactory, type, "PENG_InsulationPPList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "linesize");
        break;
    case ModelLine:
        model = getTableModel(db, mObjectFactory, type, "PENG_LineList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelLineClassif:
        model = getTableModel(db, mObjectFactory, type, "PENG_LineClassifList");
        break;
    case ModelLineCustom:
        model = getTableModel(db, mObjectFactory, type, "PENG_LineCustomList");
        break;
    case ModelLineEnginDes:
        model = getTableModel(db, mObjectFactory, type, "PENG_LineEnginDesList");
        break;
    case ModelLineProcess:
        model = getTableModel(db, mObjectFactory, type, "PENG_LineProcessList");
        break;
    case ModelPaint:
        model = getTableModel(db, mObjectFactory, type, "PENG_PaintList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelPedEqClassif:
        model = getTableModel(db, mObjectFactory, type, "PENG_PedClassifList");
        break;
    case ModelPedLineClassif:
        model = getTableModel(db, mObjectFactory, type, "PENG_PedClassifList");
        break;
    case ModelPipeClass:
        model = getTableModel(db, mObjectFactory, type, "PENG_PipeClassList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "classnumber");
        break;
    case ModelPipeClassPT:
        model = getTableModel(db, mObjectFactory, type, "PENG_PipeClassPTList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "designtemperature");
        break;
    case ModelPipeClassSize:
        model = getTableModel(db, mObjectFactory, type, "PENG_PipeClassSizeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "size");
        break;
    case ModelProject:
        model = getTableModel(db, mObjectFactory, type, "PENG_ProjectList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelProjectEdit:
        model = getTableModel(db, mObjectFactory, type, "PENG_ProjectList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelSetting:
        model = getTableModel(db, mObjectFactory, type, "PENG_SettingList");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PENG_ModelFactory::getModel()"
                        "non-existing model ERROR");
        break;
    }

    return model;
}

/**
 * Get parent model. Used in child models or models depending on selection of
 * a parent model.
 * @param type child model type
 * @return parent model or NULL if not exists
 */
RB_MmProxy* PENG_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelEngCone:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEngWeldolet:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEquipment:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEquipmentType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelFluid:
        iter = mModelList.find(ModelNone);
        break;
    case ModelFluidStability:
        iter = mModelList.find(ModelNone);
        break;
    case ModelHazardGroup:
        iter = mModelList.find(ModelNone);
        break;
    case ModelInsulationCC:
        iter = mModelList.find(ModelNone);
        break;
    case ModelInsulationHCI:
        iter = mModelList.find(ModelNone);
        break;
    case ModelInsulationHCO:
        iter = mModelList.find(ModelNone);
        break;
    case ModelInsulationPP:
        iter = mModelList.find(ModelNone);
        break;
    case ModelLine:
        iter = mModelList.find(ModelNone);
        break;
    case ModelLineClassif:
        iter = mModelList.find(ModelLine);
        break;
    case ModelLineCustom:
        iter = mModelList.find(ModelLine);
        break;
    case ModelLineEnginDes:
        iter = mModelList.find(ModelLine);
        break;
    case ModelLineProcess:
        iter = mModelList.find(ModelLine);
        break;
    case ModelPaint:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPedEqClassif:
        iter = mModelList.find(ModelEquipment);
        break;
    case ModelPedLineClassif:
        iter = mModelList.find(ModelLine);
        break;
    case ModelPipeClass:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPipeClassPT:
        iter = mModelList.find(ModelPipeClass);
        break;
    case ModelPipeClassSize:
        iter = mModelList.find(ModelPipeClass);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProjectEdit:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSetting:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PENG_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

/**
 * Add default rows, for example equipment types
 * \todo Base the default rows on SQL data file, not hardcoded.
 * @returns true on success
 */
bool PENG_ModelFactory::addDefaultRows() {
    PENG_SqlCommonFunctions f;

    if (f.isDefaultRow()) {
        // (default) rows already exist
        return true;
    }

    RB_ObjectContainer* eqTypeList =
            new RB_ObjectContainer("", NULL, "PENG_TypeList", PENG_OBJECTFACTORY);
    RB_String parentId = PENG_MODELFACTORY->getRootId();

    RB_ObjectBase* obj = PENG_OBJECTFACTORY->newObject("", eqTypeList);
    obj->setValue("parent", parentId);
    obj->setValue("typename", "Column");
    obj = PENG_OBJECTFACTORY->newObject("", eqTypeList);
    obj->setValue("parent", parentId);
    obj->setValue("typename", "Filter");
    obj = PENG_OBJECTFACTORY->newObject("", eqTypeList);
    obj->setValue("parent", parentId);
    obj->setValue("typename", "Horizontal Vessel");
    obj = PENG_OBJECTFACTORY->newObject("", eqTypeList);
    obj->setValue("parent", parentId);
    obj->setValue("typename", "Vertical Vessel");
    obj = PENG_OBJECTFACTORY->newObject("", eqTypeList);
    obj->setValue("parent", parentId);
    obj->setValue("typename", "Storage Tank");

    bool success = eqTypeList->dbUpdateList(PENG_MODELFACTORY->getDatabase());
    delete eqTypeList;

    return success;
}

