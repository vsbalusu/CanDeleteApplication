#include "UtilityMasterTbl.h"

#include "CentralSecOSManager.h"
#include "GuiBuilderCommon.h"
#include "CommonClasses/CommonFunctions.h"
#include "CommonClasses/Macros.h"
#include "ComboSelectionrDlg.h"
#include <QComboBox>
#include <QFileDialog>

UtilityMasterTbl::UtilityMasterTbl( bool theDisplayOfLowerPanel , bool theSelectionRequired , QDialog * theParent ):
DatabaseTableDlg( theDisplayOfLowerPanel , theSelectionRequired , theParent ),pComboSelectionrDlg( new ComboSelectionrDlg() )
{

    connect( &myComboMapper , SIGNAL( mapped(QString)) , this , SLOT( openSelectionBox( QString ) ) );
    connect( this , SIGNAL( rowChecked( int , bool )) , this , SLOT( rowChecked( int , bool ) ) );
    connect( ui->myDatabaseTable , SIGNAL( itemChanged( QTableWidgetItem* ) ) , this , SLOT( validateEnteredData( QTableWidgetItem* ) ) );
    connect( &myBrowseUtilityLocationMapper , SIGNAL( mapped(QString) )  , this , SLOT( browseUtility(QString) ) );
    connect( &myBrowseUtilityIconLocationMapper , SIGNAL( mapped(QString) )  , this , SLOT( browseUtilityIcon(QString) ) );
//    bool a = connect( ui->myDatabaseTable->verticalHeader() , SIGNAL( sectionClicked(int)) , this , SLOT( rowSelected( int ) ) );

    resetAllDetails();

}

UtilityMasterTbl::~UtilityMasterTbl()
{
}

void UtilityMasterTbl::addRecord()
{
    DatabaseTableDlg::addRecord();
    int rows = rowCount();

    changeColor( rows - 1 );

    {
    // Btn for Selection of the SecOs/Kernel Version
    QPushButton * btn = new QPushButton( GuiBuilderCommon::OPEN_COMBO_DLG );
    ui->myDatabaseTable->setCellWidget( rows - 1 , 1 , btn );
    QTableWidgetItem * item = ui->myDatabaseTable->item( rows -1 , 0 );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    char itemCstr1[20]={0};
    snprintf( itemCstr1 , 20 , "%p", item );
    QString itemQStr1( itemCstr1 );

    myComboMapper.setMapping( btn , itemQStr1 );
    connect( btn , SIGNAL( clicked() ) , &myComboMapper , SLOT( map() ) );

    }

    {
    QPushButton * btn1 = new QPushButton( "..." );
    ui->myDatabaseTable->setCellWidget( rows - 1 , 4 , btn1 );
    //QTableWidgetItem * item1 = new QTableWidgetItem();
    QTableWidgetItem * item1 = ui->myDatabaseTable->item( rows -1 , 3 );
    item1->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
//    ui->myDatabaseTable->setItem( rows -1 , 3 , item1 );
    GuiBuilderCommon::createMapperWithTableItem( btn1 , myBrowseUtilityLocationMapper , item1 );
    connect( btn1 , SIGNAL( clicked() ) , &myBrowseUtilityLocationMapper , SLOT( map() ) );
    }

    {
    QPushButton * btn2 = new QPushButton( "..." );
    ui->myDatabaseTable->setCellWidget( rows - 1 , 6 , btn2 );
//    QTableWidgetItem * item = new QTableWidgetItem();
    QTableWidgetItem * item = ui->myDatabaseTable->item( rows -1 , 5 );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
//    ui->myDatabaseTable->setItem( rows -1 , 5 , item );
    GuiBuilderCommon::createMapperWithTableItem( btn2 , myBrowseUtilityIconLocationMapper , item );
    connect( btn2 , SIGNAL( clicked() ) , &myBrowseUtilityIconLocationMapper , SLOT( map() ) );
    }
}

void UtilityMasterTbl::deleteRecord()
{

    QList<int> rowList = getCheckedRows();
    foreach( int row , rowList ){
        Utilities utility;
        string id = getPrimaryKey( row );
        utility.UtilityId = id;
        bool a = pCentralSecOSManager->updateMaster( CentralSecOSManager::Delete , utility );
        if( !a ){
            CommonFunctions::displayMessage( "Error occoured during deletion!" );
        }
    }
    resetAllDetails();
}

void UtilityMasterTbl::saveRecord()
{
    QList<int> rowList = getCheckedRows();
    foreach( int row , rowList ){

        int columns = columnCount();
        if( columns < 8 ){ // 8 = 5 fields + 2 Browse Buttons + 1 SecOs Version No
            CommonFunctions::displayMessage( "Error 3 : Contact Developer " );
            continue;
        }
        Utilities utilities;
        utilities.SecOsVersionNo = getCellData( TableWidgetitem , row , 0 );
        utilities.UtilityName = getCellData( TableWidgetitem , row , 2 );
        utilities.UtilityLocation = getCellData( TableWidgetitem , row , 3 );
        utilities.UtilityIconLocation = getCellData( TableWidgetitem , row , 5 );
        utilities.UtilityTooltip = getCellData( TableWidgetitem , row , 7 );
        utilities.UtilityId = getPrimaryKey( row );

        bool retValue = false;
        if( utilities.UtilityId.size() ){
            retValue = pCentralSecOSManager->updateMaster( CentralSecOSManager::Edit , utilities , utilities.UtilityId );
        }else{
            retValue = pCentralSecOSManager->updateMaster( CentralSecOSManager::Add , utilities );
        }
        if( !retValue ){
            CommonFunctions::displayMessage( "Error during Insertiong / Updation " );
        }
    }
    resetAllDetails();
}

void UtilityMasterTbl::createColumns()
{
    ui->myDatabaseTable->setColumnCount( 8 ); // 7 = 5 fields + 2 Browse Buttons

    QTableWidgetItem * h1 = new QTableWidgetItem( "SecOs Version No" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 0 , h1 );

    QTableWidgetItem * h2 = new QTableWidgetItem( "" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 1 , h2 );

    QTableWidgetItem * h3 = new QTableWidgetItem( "Utility Name" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 2 , h3 );

    QTableWidgetItem * h4 = new QTableWidgetItem( "Utility Location " );
    ui->myDatabaseTable->setHorizontalHeaderItem( 3 , h4 );

    QTableWidgetItem * h5 = new QTableWidgetItem( "" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 4 , h5 );

    QTableWidgetItem * h6 = new QTableWidgetItem( "Utility Icon Location" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 5 , h6 );

    QTableWidgetItem * h7 = new QTableWidgetItem( "" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 6 , h7 );

    QTableWidgetItem * h8 = new QTableWidgetItem( "Tooltip" );
    ui->myDatabaseTable->setHorizontalHeaderItem( 7 , h8 );

    appendColumnAtEnd();
}

void UtilityMasterTbl::fillData( string theSecOsId )
{
    CHECK_PTR( pCentralSecOSManager );
    list<Utilities> utilityList = pCentralSecOSManager->listUtilitiesMaster( theSecOsId );
    int rowCtr = 0;
    CommonFunctions::clearTableWidget( ui->myDatabaseTable );
    getSecOsVersionCombo();

    for( list<Utilities>::iterator itr = utilityList.begin() ; itr != utilityList.end() ; ++itr ){

        Utilities utility = (*itr);
        ui->myDatabaseTable->setRowCount( rowCtr + 1 );

        QTableWidgetItem * item = new QTableWidgetItem( QString::fromStdString( utility.SecOsVersionNo ) );
        ui->myDatabaseTable->setItem( rowCtr , 0 , item );

        {
        QPushButton * btn = new QPushButton( GuiBuilderCommon::OPEN_COMBO_DLG );
        ui->myDatabaseTable->setCellWidget( rowCtr , 1 , btn );
        char itemCstr1[20]={0};
        snprintf( itemCstr1 , 20 , "%p", item );
        QString itemQStr1( itemCstr1 );
        myComboMapper.setMapping( btn , itemQStr1 );
        connect( btn , SIGNAL( clicked() ) , &myComboMapper , SLOT( map() ) );
        }

        QTableWidgetItem * item1 = new QTableWidgetItem( QString::fromStdString( utility.UtilityName ) );
        ui->myDatabaseTable->setItem( rowCtr , 2 , item1 );

        QTableWidgetItem * item2 = new QTableWidgetItem( QString::fromStdString( utility.UtilityLocation ) );
        ui->myDatabaseTable->setItem( rowCtr , 3 , item2 );
        item2->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        {
        QPushButton * btn1 = new QPushButton( "..." );
        ui->myDatabaseTable->setCellWidget(  rowCtr , 4 , btn1 );
        GuiBuilderCommon::createMapperWithTableItem( btn1 , myBrowseUtilityLocationMapper , item2 );
        connect( btn1 , SIGNAL( clicked() ) , &myBrowseUtilityLocationMapper , SLOT( map() ) );
        }

        QTableWidgetItem * item3 = new QTableWidgetItem( QString::fromStdString( utility.UtilityIconLocation ) );
        ui->myDatabaseTable->setItem( rowCtr , 5 , item3 );
        item3->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        {
        QPushButton * btn2 = new QPushButton( "..." );
        ui->myDatabaseTable->setCellWidget(  rowCtr , 6 , btn2 );
        GuiBuilderCommon::createMapperWithTableItem( btn2 , myBrowseUtilityIconLocationMapper , item3 );
        connect( btn2 , SIGNAL( clicked() ) , &myBrowseUtilityIconLocationMapper , SLOT( map() ) );
        }

        QTableWidgetItem * item4 = new QTableWidgetItem( QString::fromStdString( utility.UtilityTooltip ) );
        ui->myDatabaseTable->setItem( rowCtr , 7 , item4 );
        item4->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        setPrimaryKey( utility.UtilityId , rowCtr );
        setRowEditable( rowCtr , false );
        ++rowCtr;
    }
    addCheckBoxedAtEnd();
}

void UtilityMasterTbl::getSecOsVersionCombo()
{
    /// Adding SecOs Versions List to Front End
    list<SecOsVersions> listSecOsVersions = pCentralSecOSManager->listSecOsVersionMaster();

    pComboSelectionrDlg->clearData();
    int counter = 0;
    for( list<SecOsVersions>::iterator itr = listSecOsVersions.begin() ; itr != listSecOsVersions.end() ; ++itr ){
        QString data = QString::fromStdString( (*itr).SecOSVersionNo );
        pComboSelectionrDlg->addData( data , data );
        ++counter;
    }
}


bool UtilityMasterTbl::resetRowDetails( int theRow )
{
    if( DatabaseTableDlg::resetRowDetails( theRow ) ){
        return true;
    }
    Utilities utilities = pCentralSecOSManager->listUtilitiesMasterUsingId( getPrimaryKey( theRow ) );
//    QWidget * widget = ui->myDatabaseTable->cellWidget( theRow , 0 );
//    QComboBox * combo = dynamic_cast<QComboBox*>( widget );
//    if( combo ){
//        combo->setCurrentIndex( combo->findText( QString::fromStdString( utilities.SecOsVersionNo ) ) );
//    }

    QTableWidgetItem * item = ui->myDatabaseTable->item( theRow , 0 );
    if( item ){
        item->setText( QString::fromStdString( utilities.SecOsVersionNo ) );
    }

    item = ui->myDatabaseTable->item( theRow , 2 );
    if( item ){
        item->setText( QString::fromStdString( utilities.UtilityName ) );
    }
    item = ui->myDatabaseTable->item( theRow , 3 );
    if( item ){
        item->setText( QString::fromStdString( utilities.UtilityLocation ) );
    }
    item = ui->myDatabaseTable->item( theRow , 5 );
    if( item ){
        item->setText( QString::fromStdString( utilities.UtilityIconLocation ) );
    }
    item = ui->myDatabaseTable->item( theRow , 7 );
    if( item ){
        item->setText( QString::fromStdString( utilities.UtilityTooltip ) );
    }

    setRowEditable( theRow , false );
    return true;

}

void UtilityMasterTbl::changeColor( int theRow )
{
    /**/

    if( theRow >= rowCount() ){
        return;
    }

//    QWidget * widg = ui->myDatabaseTable->cellWidget( theRow , 0 );
//    QComboBox * cmbBox = dynamic_cast<QComboBox*>( widg );
//    if( cmbBox ){
//        cmbBox->setStyleSheet( "color:blue;" );
//    }
//
//    int columncount = columnCount();
//    for( int i = 0 ; i < columncount ; ++i ){
//        if( i == 0 || i == 3 || i == 5 ){
//            continue;
//        }
//        QTableWidgetItem * item = ui->myDatabaseTable->item( theRow , i );
//        if( !item ){
//            item = new QTableWidgetItem();
//            ui->myDatabaseTable->setItem( theRow , i , item );
//        }
//        item->setTextColor( GuiBuilderCommon::myNewAddedColor );
//    }

    for( int i = 0 ; i < columnCount() ; ++i ){
        if( i == 1 || i == 4 || i == 6 ){
            continue;
        }
        QTableWidgetItem * item1 = ui->myDatabaseTable->item( theRow , i );
        if( !item1 ){
            item1 = new QTableWidgetItem();
            ui->myDatabaseTable->setItem( theRow , i , item1 );
        }
        item1->setTextColor( GuiBuilderCommon::myNewAddedColor );
    }
}

void UtilityMasterTbl::browseUtilityIcon( QString theItem )
{
    bool ok = false;
    long int itemInt = theItem.toLong( &ok , 16 );
    if( !ok ){
        return;
    }
    void * itemVoid = ( void * )itemInt;
    QTableWidgetItem * item = (QTableWidgetItem *)itemVoid;

    CHECK_PTR( item );
    QString str = QFileDialog::getOpenFileName( 0 , "select Utility Icon" , "" , "*.png" );
    if( str.size() == 0 ){
        return;
    }
    item->setText( str );
}

void UtilityMasterTbl::browseUtility( QString theItem )
{
    bool ok = false;
    long int itemInt = theItem.toLong( &ok , 16 );
    if( !ok ){
        return;
    }
    void * itemVoid = ( void * )itemInt;
    QTableWidgetItem * item = (QTableWidgetItem *)itemVoid;

    CHECK_PTR( item );
    QString str = QFileDialog::getOpenFileName( 0 , "select Utility " , "" , "*" );
    if( str.size() == 0 ){
        return;
    }
    item->setText( str );
}

void UtilityMasterTbl::resizeColumns()
{
    int checkBoxWidth = 0;
    int btnSize = 25;

    int columns = columnCount();
    if( !ui->myDatabaseTable->isColumnHidden( columnCount() ) ){
        checkBoxWidth = 50;
    }

    int widthOfColumn = ( ui->myDatabaseTable->width() - checkBoxWidth - 20 - 3*btnSize )/( columns - 3 );

    for( int columnNo = 0 ; columnNo < columns ; ++columnNo ){
        if( columnNo == 1 || columnNo == 4 || columnNo == 6 ){
            ui->myDatabaseTable->setColumnWidth( columnNo , btnSize );
            continue;
        }
        ui->myDatabaseTable->setColumnWidth( columnNo , widthOfColumn );
    }
    ui->myDatabaseTable->setColumnWidth( columns , checkBoxWidth );
}

void UtilityMasterTbl::updateRecord( string theImageId , string theScenarioId )
{
    CHECK_PTR( pCentralSecOSManager );

    QList<int> selectedRows = getSelectedRows();
    list<Utilities> utilityList;
    foreach( int row , selectedRows ){
        Utilities util;
        util.UtilityId = getPrimaryKey( row );
        utilityList.push_back( util );
    }
    pCentralSecOSManager->save_utilities( theImageId , theScenarioId , utilityList );
}

void UtilityMasterTbl::setSelection( const list<Utilities>& theUtilityList )
{
    // Deselect All rows and then select again
    ui->myDatabaseTable->clearSelection();

    // Setting up the selection once again
    for( int i = 0 ; i < rowCount(); ++i ){

        string id = getPrimaryKey( i );
        for( list<Utilities>::const_iterator itr = theUtilityList.begin() ; itr != theUtilityList.end() ; ++itr ){
            Utilities util = (*itr );
            if( id.compare( util.UtilityId ) == 0 ){
                selectRow( i );
            }
        }
    }
}


void UtilityMasterTbl::rowChecked( int theRow , bool theStatus )
{
    QTableWidgetItem * item = ui->myDatabaseTable->item( theRow , 0 );
    CHECK_PTR( item );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    item = ui->myDatabaseTable->item( theRow , 3 );
    CHECK_PTR( item );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    item = ui->myDatabaseTable->item( theRow , 5 );
    CHECK_PTR( item );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
}

void UtilityMasterTbl::openSelectionBox( QString theItem )
{
    bool ok = false;
    long int itemInt = theItem.toLong( &ok , 16 );
    if( !ok ){
        return;
    }
    void * itemVoid = ( void * )itemInt;
    QTableWidgetItem * item = (QTableWidgetItem *)itemVoid;

    CHECK_PTR( item );

    QString secosVersion;
    if( !pComboSelectionrDlg->exec() ){
        return;
    }
    pComboSelectionrDlg->getData( secosVersion );
    CHECK_PTR( item );
    item->setText( secosVersion );
}

void UtilityMasterTbl::validateEnteredData( QTableWidgetItem * theItem )
{
    CHECK_PTR( theItem );
    int col = ui->myDatabaseTable->column( theItem );
    if( col == 0  ){
        validateItem( theItem , InvalidText , 10 );
    }else if( col == 2 ){
        validateItem( theItem , InvalidText , 100 );
    }else if( col == 3 ){
        validateItem( theItem , InvalidText , 200 );
    }else if( col == 5 ){
        validateItem( theItem , InvalidText , 200 );
    }else if( col == 7 ){
        validateItem( theItem , InvalidText , 100 );
    }
}

void UtilityMasterTbl::rowSelected( int theRowSelected )
{
    string pk = getPrimaryKey( theRowSelected );
    emit utilityIdSelected( pk );
}

void UtilityMasterTbl::deselectUtility( string theUtilityId , string theScenarioId , string theImageId )
{
    CHECK_PTR( pCentralSecOSManager )
    int i = 0;
    for( i = 0 ; i < rowCount() ; ++i ){

        if( getPrimaryKey( i ).compare( theUtilityId ) == 0 ){
            i = 35500; // Some aRbitary fixed number.
        }
    }

    if( i == 35501 ){
        pCentralSecOSManager->save_delete_utilities( theImageId , theScenarioId , theUtilityId , false );
        resetAllDetails();
    }
}
