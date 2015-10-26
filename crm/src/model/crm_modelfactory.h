/*****************************************************************
 * $Id: crm_modelfactory.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_MODELFACTORY_H
#define CRM_MODELFACTORY_H

#include "rb_modelfactory.h"

#define CRM_MODELFACTORY CRM_ModelFactory::getInstance()

/**
 * Model factory for the Customer Relation Management
 * Accounting CRM plugin
 */
class CRM_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~CRM_ModelFactory();
    static CRM_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, example = 271XXX:
     * - 27 = CRM,
     * - 1 = model,
     * - XXX = sequence number
     * 271100 is starting number leaving 271000 to 271099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 271000
        //        ModelTest = 271000,             /**< Table model */
        //        ModelTestChild,                 /**< Child table model */
        //        ModelTestDialog,                /**< Dialog model */
        //        ModelTestTree,                  /**< Tree model */
        //        ModelTestTreeChild,             /**< Child Tree model */

        // start numbering from 271100
        ModelNone = 271100,             /**< Invalid model */
        ModelActivity,                  /**< Activity model such as call, meeting and task */
        ModelCampaign,                  /**< Sales campaigns, such as mailing */
        ModelCampaignTarget,            /**< Sales campaign target audience */
        ModelCampaignType,              /**< Sales campaign types, such as mailing, seminar */
        ModelContact,                   /**< ACC Contact or lead model from ACC*/
        ModelContactDetail,             /**< Detail contact or lead model */
        ModelCustomer,                  /**< ACC Customer branch model from ACC*/
        ModelCustomerDetail,            /**< Detail customer branch model */
        ModelDocument,                  /**< Document model */
        ModelGroup,                     /**< Sales campaigns target group, such as a mailing list */
        ModelGroupContact,              /**< A contact in a sales campaigns target group */
        ModelLeadSourceType,            /**< Lead source type, such as website, seminar, cold call */
        ModelProject,                   /**< ACC Project or CRM account model */
        ModelProjectEdit,               /**< ACC Project or CRM account model */
        ModelSysSeqNo,                  /**< System sequence number model */
        ModelTemplate,                  /**< Template model */
        ModelDefault                    /**< Default- or all models */
    };

private:
    CRM_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static CRM_ModelFactory* mActiveFactory;

};

#endif // CRM_MODELFACTORY_H
