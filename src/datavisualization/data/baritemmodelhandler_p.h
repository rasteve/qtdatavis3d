/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVisualization module.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVisualization API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef BARITEMMODELHANDLER_P_H
#define BARITEMMODELHANDLER_P_H

#include "abstractitemmodelhandler_p.h"
#include "qitemmodelbardataproxy.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

class BarItemModelHandler : public AbstractItemModelHandler
{
    Q_OBJECT
public:
    BarItemModelHandler(QItemModelBarDataProxy *proxy, QObject *parent = 0);
    virtual ~BarItemModelHandler();

protected:
    void virtual resolveModel();

    QItemModelBarDataProxy *m_proxy; // Not owned
};

QT_DATAVISUALIZATION_END_NAMESPACE

#endif