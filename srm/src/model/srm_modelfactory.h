/*****************************************************************
 * $Id: srm_modelfactory.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_MODELFACTORY_H
#define SRM_MODELFACTORY_H

#include "rb_modelfactory.h"

#define SRM_MODELFACTORY SRM_ModelFactory::getInstance()

/**
 * Model factory for the Customer Relation Management
 * Accounting CRM plugin
 */
class SRM_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~SRM_ModelFactory();
    static SRM_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, example = 291XXX:
     * - 29 = CRM,
     * - 1 = model,
     * - XXX = sequence number
     * 291100 is starting number leaving 291000 to 291099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 291000
        //        ModelTest = 291000,             /**< Table model */
        //        ModelTestChild,                 /**< Child table model */
        //        ModelTestDialog,                /**< Dialog model */
        //        ModelTestTree,                  /**< Tree model */
        //        ModelTestTreeChild,             /**< Child Tree model */

        // start numbering from 291100
        ModelNone = 291100,             /**< Invalid model */
        ModelActivity,                  /**< Activity model such as call, meeting and task */
        ModelCampaign,                  /**< Sales campaigns, such as mailing */
        ModelCampaignTarget,            /**< Sales campaign target audience */
        ModelCampaignType,              /**< Sales campaign types, such as mailing, seminar */
        ModelContact,                   /**< Contact or lead model from ACC*/
        ModelContactDetail,             /**< Detail contact or lead model */
        ModelDocument,                  /**< Document model */
        ModelGroup,                     /**< Sales campaigns target group, such as a mailing list */
        ModelGroupContact,              /**< A contact in a sales campaigns target group */
        ModelLeadSourceType,            /**< Lead source type, such as website, seminar, cold call */
        ModelProject,                   /**< Project or CRM account model */
        ModelProjectEdit,               /**< Edit project model */
        ModelPurchData,                 /**< Supplier purchase data model, about items supplier can supply */
        ModelSupplier,                  /**< Supplier model from ACC */
        ModelSupplierDetail,            /**< Supplier detail model */
        ModelSysSeqNo,                  /**< System sequence number model */
        ModelTemplate,                  /**< Template model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    SRM_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static SRM_ModelFactory* mActiveFactory;

};

#endif // SRM_MODELFACTORY_H
