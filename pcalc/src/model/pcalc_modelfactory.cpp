/*****************************************************************
 * $Id: pcalc_modelfactory.cpp 2232 2015-04-23 16:04:08Z rutger $
 * Created: Apr 15, 2010 11:29 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_modelfactory.h"

#include "db_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_sqlcommonfunctions.h"
#include "rb_database.h"

PCALC_ModelFactory* PCALC_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
PCALC_ModelFactory::PCALC_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("PCALC_ModelFactory::PCALC_ModelFactory()");
    this->setObjectFactory(PCALC_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_ModelFactory::~PCALC_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PCALC_ModelFactory::~PCALC_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
PCALC_ModelFactory* PCALC_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new PCALC_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as a shared/single model
 * @return model manager
 */
RB_MmProxy* PCALC_ModelFactory::getModel(int type, bool shared) {
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

    QSqlDatabase db = getDatabase(); // RB_DATABASE->database(mDatabaseConnection); // same as
    QSqlDatabase stdDb = getStandardDatabase();

    switch (type) {
    case ModelBoltAsme:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_BoltAsmeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "nomsize");
        break;
    case ModelBoltEn:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_BoltEnList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "dsnom");
        break;
    case ModelClassGenerator:
        model = getTableModel(db, mObjectFactory, type, "PCALC_ClassGeneratorList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "codename", "subname");
        break;
    case ModelCompRating:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_CompRatingList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating");
        break;
    case ModelCompSerie:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_CompSerieList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "type");
        break;
    case ModelCompType:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_CompTypeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "type");
        break;
    case ModelDimension:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_DimensionList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "code");
        break;
    case ModelEN13555EGeG:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555EGeGList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temp", "qg");
        break;
    case ModelEN13555Gasket:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555GasketList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelEN13555Manuf:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555ManufacturerList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelEN13555PQRdeltaeGC:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555PqrDeltaeGCList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temp", "qg");
        break;
    case ModelEN13555QminL:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555QminLList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "testpress", "leakrate");
        break;
    case ModelEN13555QsmaxAlphaG:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555QsmaxAlphaGList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temp");
        break;
    case ModelEN13555QsminL:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_EN13555QsminLList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "testpress", "qa", "leakrate");
        break;
    case ModelEN1591Assembly:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_AssemblyList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591BoltNutWasher:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_BoltNutWasherList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591Flange:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_FlangeList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591Gasket:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_GasketList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591LoadCase:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_LoadCaseList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591Shell:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_ShellList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelFlangeAsme:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_FlangeAsmeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating", "nomsize");
        break;
    case ModelFlangeEn:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_FlangeEnList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating", "nomsize");
        break;
    case ModelFlangeFacingDimAsme:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_FlangeFacingDimAsmeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating", "nomsize");
        break;
    case ModelFlangeFacingDimEn:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_FlangeFacingDimEnList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating", "nomsize");
        break;
    case ModelFlangeTypeLimit:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_FlangeTypeLimitList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating");
        break;
    case ModelGasket:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_GasketList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "rating", "nomsize");
        break;
    case ModelGeneratorFormula:
        model = getTableModel(db, mObjectFactory, type, "PCALC_GeneratorFormulaList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelMatCreepTable:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_CreepTableList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelMaterial:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_MaterialList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelMatElasModulTable:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_ElasModulTableList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelMatElasModulValue:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_ElasModulList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelMatThermExpTable:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_ThermExpTableList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelMatThermExpValue:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_ThermExpList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelNutAsme:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_NutAsmeList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "nomsize");
        break;
    case ModelNutEn:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_NutEnList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "damin");
        break;
    case ModelProject:
        model = getTableModel(db, mObjectFactory, type, "PENG_ProjectList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelProjectEdit:
        model = getTableModel(db, mObjectFactory, type, "PENG_ProjectList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelRmMin:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_RmMinList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRp02:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_Rp02List");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRp10:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_Rp10List");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRp10T100kh:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_Rp10T100khList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRp10T10kh:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_Rp10T10khList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRpT100kh:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_RpT100khList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRpT10kh:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_RpT10khList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    case ModelRpT200kh:
        model = getTableModel(stdDb, mObjectFactory, type, "STD_RpT200khList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "temperature");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PCALC_ModelFactory::getModel()"
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
RB_MmProxy* PCALC_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelBoltAsme:
        iter = mModelList.find(ModelCompType);
        break;
    case ModelBoltEn:
        iter = mModelList.find(ModelCompType);
        break;
    case ModelClassGenerator:
        iter = mModelList.find(ModelGeneratorFormula);
        break;
    case ModelCompRating:
        iter = mModelList.find(ModelDimension);
        break;
    case ModelCompSerie:
        iter = mModelList.find(ModelDimension);
        break;
    case ModelCompType:
        iter = mModelList.find(ModelDimension);
        break;
    case ModelDimension:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555EGeG:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555Gasket:
        iter = mModelList.find(ModelEN13555Manuf);
        break;
    case ModelEN13555Manuf:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555PQRdeltaeGC:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555QminL:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555QsmaxAlphaG:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555QsminL:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN1591Assembly:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN1591BoltNutWasher:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591Flange:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591Gasket:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591LoadCase:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591Shell:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelFlangeAsme:
        iter = mModelList.find(ModelCompRating);
        break;
    case ModelFlangeEn:
        iter = mModelList.find(ModelCompRating);
        break;
    case ModelFlangeFacingDimAsme:
        iter = mModelList.find(ModelCompRating);
        break;
    case ModelFlangeFacingDimEn:
        iter = mModelList.find(ModelCompRating);
        break;
    case ModelFlangeTypeLimit:
        iter = mModelList.find(ModelCompType);
        break;
    case ModelGasket:
        iter = mModelList.find(ModelCompRating);
        break;
    case ModelGeneratorFormula:
        iter = mModelList.find(ModelNone);
        break;
    case ModelMatCreepTable:
        iter = mModelList.find(ModelNone);
        break;
    case ModelMatElasModulTable:
        iter = mModelList.find(ModelNone);
        break;
    case ModelMatElasModulValue:
        iter = mModelList.find(ModelMatElasModulTable);
        break;
    case ModelMaterial:
        iter = mModelList.find(ModelNone);
        break;
    case ModelMatThermExpTable:
        iter = mModelList.find(ModelNone);
        break;
    case ModelMatThermExpValue:
        iter = mModelList.find(ModelMatThermExpTable);
        break;
    case ModelNutAsme:
        iter = mModelList.find(ModelCompType);
        break;
    case ModelNutEn:
        iter = mModelList.find(ModelCompType);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProjectEdit:
        iter = mModelList.find(ModelNone);
        break;
    case ModelRmMin:
        iter = mModelList.find(ModelMaterial);
        break;
    case ModelRp02:
        iter = mModelList.find(ModelMaterial);
        break;
    case ModelRp10:
        iter = mModelList.find(ModelMaterial);
        break;
    case ModelRp10T100kh:
        iter = mModelList.find(ModelMatCreepTable);
        break;
    case ModelRp10T10kh:
        iter = mModelList.find(ModelMatCreepTable);
        break;
    case ModelRpT100kh:
        iter = mModelList.find(ModelMatCreepTable);
        break;
    case ModelRpT10kh:
        iter = mModelList.find(ModelMatCreepTable);
        break;
    case ModelRpT200kh:
        iter = mModelList.find(ModelMatCreepTable);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PCALC_ModelFactory::getParentModel() "
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
bool PCALC_ModelFactory::addDefaultRows() {
//    PCALC_SqlCommonFunctions f;

    bool success = true;

    return success;
}

