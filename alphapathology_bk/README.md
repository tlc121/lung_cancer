# AlphaPathology
## 编译
将alpha拉到本地后，需要在根目录下打开command窗口，然后输入:

`conan install ./conanfile.txt -s arch=x86_64 -s os=Windows -s compiler.version=15 -r cloud --update`

然后就可以通过cmake gui正常编译
