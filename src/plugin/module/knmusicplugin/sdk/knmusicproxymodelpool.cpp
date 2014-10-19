/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicproxymodelpool.h"

KNMusicProxyModelPool *KNMusicProxyModelPool::m_instance=nullptr;

KNMusicProxyModelPool *KNMusicProxyModelPool::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicProxyModelPool:m_instance;
}

KNMusicProxyModelPool::~KNMusicProxyModelPool()
{
    //Recover all the memory.
    KNMusicProxyModel *element;
    while(!m_available.isEmpty())
    {
        element=m_available.takeLast();
        delete element;
    }
    while(!m_occupation.isEmpty())
    {
        element=m_occupation.takeLast();
        delete element;
    }
}

KNMusicProxyModel *KNMusicProxyModelPool::alloct()
{
    //Check is available list is empty.
    if(m_available.isEmpty())
    {
        //If so, alloct one.
        KNMusicProxyModel *element=new KNMusicProxyModel;
        //Add to occupation list.
        m_occupation.append(element);
        return element;
    }
    //Return the first one of the avaliable.
    return m_available.first();
}

bool KNMusicProxyModelPool::release(KNMusicProxyModel *model)
{
    //Find the model in occupation list.
    int modelIndex=m_occupation.indexOf(model);
    //If not find, return false.
    if(modelIndex==-1)
    {
        return false;
    }
    //Reset the model first.
    resetProxyModel(model);
    //Set it free.
    m_available.append(m_occupation.takeAt(modelIndex));
    return true;
}

bool KNMusicProxyModelPool::isOccupied(KNMusicProxyModel *model)
{
    return m_occupation.contains(model);
}

KNMusicProxyModelPool::KNMusicProxyModelPool(QObject *parent) :
    QObject(parent)
{
}

KNMusicProxyModel *KNMusicProxyModelPool::playing() const
{
    return m_playing;
}

bool KNMusicProxyModelPool::setPlaying(KNMusicProxyModel *playing)
{
    int modelIndex=m_occupation.indexOf(playing);
    //If not find, return false.
    if(modelIndex==-1)
    {
        return false;
    }
    //Get that model.
    m_playing=m_occupation.takeAt(modelIndex);
}

void KNMusicProxyModelPool::releasePlaying()
{
    //If playing model is not null,
    if(m_playing!=nullptr)
    {
        //Reset the proxy model.
        resetProxyModel(m_playing);
        //Move it to available list.
        m_available.append(m_playing);
        //Clear the pointer.
        m_playing=nullptr;
    }
}

void KNMusicProxyModelPool::resetProxyModel(KNMusicProxyModel *model)
{
    model->setSourceModel(nullptr);
}

