#include "ctrl_headerthread.h"

Ctrl_HeaderThread::Ctrl_HeaderThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}

Ctrl_HeaderThread *Ctrl_HeaderThread::bulid(QObject *parent)
{
    Ctrl_HeaderThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_HeaderThread(parent);
    return sington;
}
