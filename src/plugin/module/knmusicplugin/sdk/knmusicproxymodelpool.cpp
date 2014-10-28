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
    //Check is available list is empty. If so, return the first one of the
    //avaliable, or else generate one.
    KNMusicProxyModel *element=
            m_available.isEmpty()?new KNMusicProxyModel:m_available.takeFirst();
    //Add to occupation list.
    m_occupation.append(element);
    //Add a counter for that element.
    m_referenceCounter.append(1);
    return element;
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
    //Check model's reference.
    if(m_referenceCounter.at(modelIndex)==1)
    {
        //Reset the model first.
        resetProxyModel(model);
        //Set it free.
        m_available.append(m_occupation.takeAt(modelIndex));
        //Remove the reference counter.
        m_referenceCounter.removeAt(modelIndex);
    }
    else
    {
        //Modify the counter.
        m_referenceCounter.replace(modelIndex,
                                   m_referenceCounter.at(modelIndex)-1);
    }
    return true;
}

bool KNMusicProxyModelPool::isModelPlaying(KNMusicProxyModel *model)
{
    //Find the model in occupation list.
    int modelIndex=m_occupation.indexOf(model);
    //If not find, of course it can be playing.
    if(modelIndex==-1)
    {
        return false;
    }
    return (m_referenceCounter.at(modelIndex)>1);
}

void KNMusicProxyModelPool::reference(KNMusicProxyModel *model)
{
    //Find the model in occupation list.
    int modelIndex=m_occupation.indexOf(model);
    //If not find, return.
    if(modelIndex==-1)
    {
        return;
    }
    //Add counting to the reference counter.
    m_referenceCounter.replace(modelIndex,
                               m_referenceCounter.at(modelIndex)+1);
}

KNMusicProxyModelPool::KNMusicProxyModelPool(QObject *parent) :
    QObject(parent)
{
}

void KNMusicProxyModelPool::resetProxyModel(KNMusicProxyModel *model)
{
    //Clear the source model.
    model->setSourceModel(nullptr);
}
