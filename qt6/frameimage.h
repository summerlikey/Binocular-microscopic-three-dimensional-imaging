#ifndef FRAMEIMAGE_H
#define FRAMEIMAGE_H

#include <QMainWindow>
#include "api.h"
namespace Ui {
class frameimage;
}

class frameimage : public QMainWindow
{
    Q_OBJECT

public:
    explicit frameimage(QWidget *parent = nullptr);
    ~frameimage();

private:
    Ui::frameimage *ui;
    Api m_api;//创建Api m_Api类后面都基于此类
    QImage m_Image;// the qimage,最终采用qlabel显示
    std::vector<std::string> m_cameras;// A list of known camera IDs
    void UpdateCameraListBox();//更新相机列表到QListWidget
    // Are we streaming?
    bool m_bIsStreaming;
    void Log( std::string strMsg, VmbErrorType eErr );//输出错误信息
    void Log(std::string strMsg);//输出错误信息
    VmbErrorType CopyToImage(VmbUchar_t *pInBuffer,VmbPixelFormat_t ePixelFormat,QImage &pOutImage, const float *Matrix = NULL);//将缓冲区图像显示到qImage，并返回错误类型VmbErrorType err
private slots:
    // The event handler for starting / stopping acquisition
    void OnBnClickedButtonStartstop();//开始和停止采集按钮
    void OnCameraListChanged(int reason);//槽，相机改变
    void OnFrameReady( int status );//帧处理
    //void OnBnClickedButtonStartstop();//开始抓取，停止抓取按钮，如果还没有用connect连接将会出现连接错误

};

#endif // FRAMEIMAGE_H
