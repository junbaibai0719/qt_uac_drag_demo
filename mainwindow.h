#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <shellapi.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dragEnterEvent(QDragEnterEvent *e);
    bool nativeEvent(const QByteArray &eventType, void *message, long long *result){
        if (eventType == "windows_generic_MSG"){
            PMSG msg = (PMSG) message;
            if(msg->message == 563){
                qDebug() << msg->message << msg->hwnd << msg->wParam << msg->lParam << msg->time << msg->pt.x << msg->pt.y;
                UINT file_num = DragQueryFile((HDROP) msg->wParam, 0xFFFFFFFF, NULL, 0);
                qDebug() << "文件数量:" << file_num;
                for(int i=0;i<file_num;i++){
                    UINT file_name_size = DragQueryFile((HDROP) msg->wParam, i, NULL, 0);
                    qDebug() << file_name_size;
                    LPWSTR  fn = (LPWSTR)malloc(sizeof(WCHAR)*file_name_size);
                    UINT code = DragQueryFile((HDROP) msg->wParam, i, fn, file_name_size);
                    QString filename = QString::fromStdWString(fn);
                    free(fn);
                    qDebug() << "第" << i << "个文件:" << filename;
                    qDebug() << "get name error:" << code;
                }
                qDebug() << eventType << message << *result;
            }
        }
        return QMainWindow::nativeEvent(eventType, message, result);
    };
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
