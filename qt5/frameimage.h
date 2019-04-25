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
    QImage m_Image;// the qimage
    std::vector<std::string> m_cameras;// A list of known camera IDs
    void UpdateCameraListBox();//更新相机列表到QListWidget
private slots:
    void OnCameraListChanged(int reason);//槽，相机改变
};

#endif // FRAMEIMAGE_H
