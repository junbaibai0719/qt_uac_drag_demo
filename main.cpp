#include "mainwindow.h"
#include <QWidget>
#include <QtGui>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <Windows.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "uac_drop_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    void* user32 = LoadLibraryA("user32");
    FARPROC func = GetProcAddress((HMODULE)user32, "ChangeWindowMessageFilter");
    qDebug() << (*func)();
    user32 = LoadLibraryA("user32");
    func = GetProcAddress((HMODULE)user32, "ChangeWindowMessageFilter");
    qDebug() << (*func)();
    ChangeWindowMessageFilter(WM_DROPFILES, 1);
//    ChangeWindowMessageFilter(WM_COPYDATA, 1);
//    ChangeWindowMessageFilter(0x0049, 1);
    qDebug() << w.winId() << w.effectiveWinId();
    qDebug() << ChangeWindowMessageFilterEx((HWND)w.effectiveWinId(), WM_DROPFILES, MSGFLT_ALLOW, NULL);
    qDebug() << ChangeWindowMessageFilterEx((HWND)w.effectiveWinId(), WM_COPYDATA, MSGFLT_ALLOW, NULL);
    qDebug() << ChangeWindowMessageFilterEx((HWND)w.effectiveWinId(), 0x0049, MSGFLT_ALLOW, NULL);
    DragAcceptFiles((HWND)w.effectiveWinId(), true);
    qDebug() << GetLastError();
    HRESULT res = RevokeDragDrop((HWND)w.winId());
    qDebug() << "res:" <<res;
//    DragAcceptFiles((HWND)w.effectiveWinId(), true);
//    DragAcceptFiles((HWND)w.winId(), true);
//    w.setAcceptDrops(true);
//    qDebug() << w.acceptDrops();
    w.show();
    return a.exec();
}
