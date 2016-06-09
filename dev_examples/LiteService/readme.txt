
Python 编写windows后台服务的完整示例.
已经在win7和win8上测试通过.

需要安装以下环境，注意OS是32位还是64位的，需要安装对应的版本。
python2.7
pywin32-219  http://sourceforge.net/projects/pywin32/
py2exe-0.6.9  http://sourceforge.net/projects/py2exe/

测试结果:
以下结果均是在win7 64bit环境下编写和打包.

pythonX64下，py2exe缺点是不支持打包成一个单独的exe，运行之后进程中看到的名称没有*32.  换成pythonWIN32可以支持生成一个独立的exe文件,运行后进程中看到的是xx*32.exe . 
把pythonWIN32下打包后的单独一个exe拷到win8X64下可正常运行.

pythonX64下，PyInstaller-2.1打包出来的exe可以install成功，但是无法start成功，提示 "1053 服务没有及时响应启动或控制请求"。  直接在windows的服务中也无法启动。 没有查到是什么原因，按照下面的方法也无法启动。
http://stackoverflow.com/questions/25770873/python-windows-service-pyinstaller-executables-error-1053

cx_Freeze没有在这个项目中尝试过。

可行方法：
用py2exe 在pythonWIN32下打包成一个exe(或者在pythonWIN64下打包成一个exe 附带一些库)，在其它平台下运行。


