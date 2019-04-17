# Binocular-microscopic-three-dimensional-imaging
My project  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/api.png)  
GenICam  
GigE TL camera mako G-192B PoE
Vimba Manual.pdf  
Vimba CPP Manual.pdf  
Vimba ImageTransform Manual.pdf  
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

INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaCPP\include"  
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaImageTransform\include"  

LIBS += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaCPP\Lib\Win64\VimbaCPP.lib"  
LIBS += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaC\Lib\Win64\VimbaC.lib"  
LIBS += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaImageTransform\Lib\Win64\VimbaImageTransform.lib"  
# Qt 第三方库配置
QT配置一直失败，无法链接
