# Binocular-microscopic-three-dimensional-imaging
My project  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/api.png)  
GenICam  
GigE TL camera mako G-192B PoE
Vimba Manual.pdf  
Vimba CPP Manual.pdf  
Vimba ImageTransform Manual.pdf  
# 工作流程
相机采集时，连续的采集处理采集处理采集处理，采用队列保证每一帧都能处理，DMA队列->准备队列->处理队列  
# 数据传输和显示流程
不同相机有不同的流采集器，如果一个结构安装了多个相机，那么就会有多个流采集器，对应多个通道。对于每个通道，实际采集数据时拆分后的数据包RawData形式，在内存中是一维数组形式，每一帧有不同的标识表明一帧的开始和结束，每个数据包也有标识显示当前的通道，想要显示图像，用户程序需要将一维数据拼装回图像形式，可借助OpenCV或者MIL等图像处理包完成该工作。图片来源于网络。  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/shuju.png)
# Buffer management
User:  
1.Allocate memory for the frame buffers on the host PC.  
2.Announce the buffer (this hands the frame buffer over to the API ).  
3.Queue a frame (prepare buffer to be filled).  
Vimba:  
4.Vimba fills the buffer with an image from the camera  
5.Vimba returns the filled buffer (and hands it over to the user).  
User:  
6.Work with the image.  
7.Requeue the frame to hand it over to the API.  
# SynchronousGrab 同步
# asynchronousGrab 异步
# Image Capture and Image AcquisiƟon  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/acquistion.png)   
# Qt .pro
在.pro文件中设置    
INCLUDEPATH += $$(VIMBA_HOME)
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaCPP\include"
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaImageTransform\include"

LIBS += I:/qtcode/VimbaC/Lib/Win64/VimbaC.lib  
LIBS += I:/qtcode/VimbaCPP/Lib/Win64/VimbaCPP.lib  
LIBS += I:/qtcode/VimbaImageTransform/Lib/Win64/VimbaImageTransform.lib  
设置完成后，将下面4个文件放入debug 或者release中  
VimbaC.dll  
VimbaCPP.dll  
VimbaCPPd.dll  
VimbaImageTransform.dll  

# Qt 第三方库配置
QT配置一直失败，无法链接，连接后需要run qmake一下  
