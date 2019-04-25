#include "frameimage.h"
#include "ui_frameimage.h"

frameimage::frameimage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frameimage)
{
    ui->setupUi(this);
    m_api.ApiStartUp();
    QObject::connect(m_api.GetCameraObserver(), SIGNAL( CameraListChangedSignal(int)), this, SLOT( OnCameraListChanged(int) ));
    UpdateCameraListBox();
}

frameimage::~frameimage()
{
    delete ui;
    m_api.ApiShutDown();

}
void frameimage::OnCameraListChanged( int reason )
{
    bool bUpdateList = false;
    // We only react on new cameras being found and known cameras being unplugged
    if( AVT::VmbAPI::UpdateTriggerPluggedIn == reason )
    {
        bUpdateList = true;
    }
    else if( AVT::VmbAPI::UpdateTriggerPluggedOut == reason )
    {
        bUpdateList = true;
    }

    if( true == bUpdateList )
    {
        UpdateCameraListBox();
    }
}
void frameimage::UpdateCameraListBox()
{
    // Get all cameras currently connected to Vimba
    CameraPtrVector cameras = m_api.GetCameraList();

    // Simply forget about all cameras known so far
    ui->m_ListBoxCameras->clear();
    m_cameras.clear();

    // And query the camera details again
    for(    CameraPtrVector::const_iterator iter = cameras.begin();
            cameras.end() != iter;
            ++iter )
    {
        std::string strCameraName;
        std::string strCameraID;
        if( VmbErrorSuccess != (*iter)->GetName( strCameraName ) )
        {
            strCameraName = "[NoName]";
        }
        // If for any reason we cannot get the ID of a camera we skip it
        if( VmbErrorSuccess == (*iter)->GetID( strCameraID ) )
        {
            ui->m_ListBoxCameras->addItem( QString::fromStdString( strCameraName + " " +strCameraID ) );
            m_cameras.push_back( strCameraID );
        }
    }
}
