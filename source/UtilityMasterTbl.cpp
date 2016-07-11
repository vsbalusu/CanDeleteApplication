#include "UtilityMasterTbl.h"

#include "CentralSecOSManager.h"
#include "GuiBuilderCommon.h"
#include "CommonClasses/CommonFunctions.h"
#include "CommonClasses/Macros.h"
#include "ComboSelectionrDlg.h"
#include <QComboBox>
#include <QFileDialog>


    if( i == 35501 ){
        pCentralSecOSManager->save_delete_utilities( theImageId , theScenarioId , theUtilityId , false );
        resetAllDetails();
    }
}
