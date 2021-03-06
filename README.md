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
 如果一个网卡连接一个相机，StreamBytesPerSecond =124000000  
 如果一个网卡连接两个个相机，需要将带宽调整为一半，GVSPAdjustPacketSize 以及StreamBytesPerSecond = 62000000  
# Qt .pro
window系统在.pro文件中设置    
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
linux系统中配置有限网ip和子网掩码  
两个相机ip分别为169.254.127.127和169.254.171.220 所以配置时ip169.254.0.0和子网掩码255.255.0.0  
linux系统中的por

#opencv include files  
#INCLUDEPATH += /usr/local/include  
#INCLUDEPATH += /usr/include/opencv2/  


#vimba include files  
#INCLUDEPATH += /usr/local/Vimba_2_1/  
#INCLUDEPATH += /usr/local/Vimba_2_1/VimbaCPP/Examples  
#INCLUDEPATH += /usr/local/Vimba_2_1/VimbaImageTransform/Include  

INCLUDEPATH += /home/ganggang/Downloads/Vimba_2_1/  
INCLUDEPATH += /home/ganggang/Downloads/Vimba_2_1/VimbaCPP/Examples  
INCLUDEPATH += /home/ganggang/Downloads/Vimba_2_1/VimbaImageTransform/Include  

LIBS += -L"/home/ganggang/Downloads/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit/"
LIBS += -L"/home/ganggang/Downloads/Vimba_2_1/VimbaImageTransform/DynamicLib/x86_64bit/"
LIBS += -lVimbaC -lVimbaCPP -lVimbaImageTransform


#opencv libs  
#LIBS += -L"/usr/local/lib/"  
#LIBS += -L"/usr/lib/"   
#LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_videoio -lx264 -lopencv_photo -lopencv_cudaarithm  -lopencv_cudafilters -lopencv_cudaimgproc  
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_photo  

#Vimba libs  
#LIBS += -L"/usr/local/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit/"  
#LIBS += -L"/usr/local/Vimba_2_1/VimbaImageTransform/DynamicLib/x86_64bit/"  
LIBS += -L"/home/test-pc/Documents/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit/"  
LIBS += -L"/home/test-pc/Documents/Vimba_2_1/VimbaImageTransform/DynamicLib/x86_64bit/"  

LIBS += -lVimbaC -lVimbaCPP -lVimbaImageTransform  

# Qt 第三方库配置
QT配置一直失败，无法链接，连接后需要run qmake一下   
当多继承含有QObjiect时，需要将QObjiect放在前面，例如class FrameObserver : public QObject,public IFrameObserver，如何反过来将会出现中间编译错误  
# 处理流程
1.相机打开  
2.准备工作  
A.定义帧大小  
B.开始引擎   
C.帧放入队列    
3.开始采集  
  采集过程  这过程中，可以利用类的初始化进行参数传递，重要的是哪一个相机的传递，构造函数能够实现传递，（重要）
5.停止采集  
6.停止采集引擎  
7.释放队列  
8.释放所有帧  
9.释放所有观察器  
10.关闭当前相机  
# 官方例子在vs2017中打开
将qt 的version 调整到自己版本的，qt project option version  
将qt连接库lib调整到qt五的版本  
附加库目录选择本机qt的bin目录，本机新建一个环境变量QTDIR 值就是Qt的目录，因为include 和bin 目录都继承了环境变量，下次配置三方库时观察是否使用变量，变量可以在环境变量中配置（全局变量）  
# SDK特点
整体编程风格（C++)  
充分体现C++语言的优点，类的继承和面向对象，采用相机观察器、功能观察器和帧观察器，三种观察器能够表现AVT相机的大部分主要特点
# 系统搭建中的学习
1.slots 在对象已经创建，但slots只有声明而没有定义可能会出现链接错误，将定义完成即可  
2.QObject 是所有qt类的基类，只有QObject的类和派生类才能使用connect将信号和槽进行连接，connect(Object1,SINGAL(AAA),this,SLOTS(ADD)),第一个参数是信号的发出者，第二个参数是信号，第三个参数是信号的接收者，第四个是槽。平时使用的mainwindow，widget都是QObject的派生类，可以直接使用connect，
3.问题：找不到信号发送者，connect仅仅适用于活动对象。  问题解决，首先发送者是一个OBject指针，因此，需要将m_pCameraObserver =new Camera();观察器然后注册观察器  
.H中
    // Our camera observer  
    CameraObserver *m_pCameraObserver;  
    
.cpp中  
    m_pCameraObserver=new CameraObserver();  
    if(VmbErrorSuccess==m_system.RegisterCameraListObserver( ICameraListObserverPtr (m_pCameraObserver)))  
    {  
        cout<<"m_system.RegisterCameraListObserver"<<endl;  
        return m_pCameraObserver;  
    }  
    
4.由观察器的实现原理产生问题3，观察器的使用，如何将状态更改通知其他类，但又不依赖于对象的类？需要复习，解决了观察器，暂时没有解决此问题

5.设置相机大小采用内联函数，内联函数有什么好处？  

6.const 和 引用&，有什么作用  
7.该QMutex类提供线程之间的访问串行化,QMutex的目的是保护对象，数据结构或代码段，以便一次只能有一个线程访问它.有lock和unlock成员函数  
8.共享指针
9.SP_SET(m_pFrameObserver ,new FrameObserver(m_pCamera))注册帧观察器  
10.强制转换类型static_cast < int > (width),强制转换未int类型，可以用于其他强制转换  
11.qt采用事件驱动程序，需要不断的循环，return a.exec()，一直返回主函数，不让主函数结束。qt在编译中的实现过程，首先，qmake -project  生成 .pro文件
其次再执行qmake 生成makefile文件，最后 make 根据makefile生成可执行文件，qt跨平台，就是使用qmake生成各个平台的makefile，因为每个平台的makefile不同，因此跨平台在编译过程中就开始实现了 。  
12.static修饰的函数，称为静态函数，有两种：1）在类中，称为静态成员函数，类的静态成员属于类本身，在类加载就会分配内存;非静态成员函数属于类的对象，在类实例化时才会分配内存，然后通过类的对向实例去访问；2）不是在类中，称为全局静态函数，限定在本源码文件中使用，普通函数是默认extern的，可以被其他代码文件调用，好处：1）不能被其他文件使用，其他可以定义相同名字的函数。  
static int age;在类中不能初始化，只能在类外初始化，但是static const int age=0;可以在类中初始化，只有int类型可以，char string double 都不行，因为static数据成员的周期大于类的对象，还没有类的对象，静态数据成员就有内存了。    
13.QByteArray 里面有很多字节操作函数，比如const char* 等指针；  
14.目前双线程失败，发现，单独打开一个相机时没问题，但是当打开一个相机后，再打开另一个相机，第二个相机的线程和第一个相机的线程一样，按理不一样，而且帧观察器线程和主线程不一样和run线程也不一样。  未解决，尝试办法，去掉connec，直接再run中队列，调用 帧，利用while（1）进行循环。  
15.virtual 虚函数，可以被重写  
16.神奇的程序，弄了一周，终于知道原因了，网卡打架，导致每一帧没有写满就被传输到程序里面，每次帧类型错误都是-1.  
17.了解一下，电脑如何同时链接多个ip，电脑如何同时和外部电脑通信，它们的线程和我现在的相机线程有什么区别？  
# 开发日志
1906219.将投影仪和相机合为一个应用程序，由于在window系统中、投影仪器源代码只能采用mingw32位编译，而Vimba SDK只能采用MVSV编译，无法同时，还未转换。在linux中gcc64编译可通过，但是投影仪连接无反应，估计hidusb接口问题。
