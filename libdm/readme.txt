修改记录： 
core 目录下的文件，修改原先使用的系统接口（POSIX） 为 平台自定义接口（platform目录下）

sdksrc/sdk/lwm2msdk.c : 主要修改原先机制：
 --  linux 使用 selelct 等待退出信号（PIPE）和网络可读 216B使用全局变量表示退出标志 使用轮询模式运行程序 
 --  linux 使用信号， C216B 废弃 


 control : 


-DC216B_PLAT
-DLINUX_PLAT -- HAVE_PIPE    HAVE_SELECT  HAVE_SIGNAL   
 