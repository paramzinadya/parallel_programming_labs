Для работы с библиотекой необходимо:

Заголовочные файлы из каталога \Pre-built.2\include
pthread.h; sched.h; semaphore.h Прописать путь в свойствах проекта и подключить так: #include <pthread.h> Или скопировать в каталог с исходными текстами программы и подключить так: #include ”pthread.h”
Файл pthreadVCE2.lib из каталога \Pre-built.2\lib
Можно подключить директивой #pragma comment(lib,"pthreadVCE2.lib")
pthreadVCE2.dll из каталога \Pre-built.2\dll
Поместить в каталог с exe файлом
Open Source POSIX Threads for Win32: http://sourceware.org/pthreads-win32/ ftp://sourceware.org/pub/pthreads-win32
