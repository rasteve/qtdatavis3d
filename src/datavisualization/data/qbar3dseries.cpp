/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
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

#include "qbar3dseries_p.h"
#include "bars3dcontroller_p.h"
#include <QtCore/qmath.h>

QT_BEGIN_NAMESPACE_DATAVISUALIZATION

/*!
 * \class QBar3DSeries
 * \inmodule QtDataVisualization
 * \brief Base series class for Q3DBars.
 * \since Qt Data Visualization 1.0
 *
 * QBar3DSeries manages the series specific visual elements, as well as series data
 * (via data proxy).
 *
 * If no data proxy is set explicitly for the series, the series creates a default
 * proxy. Setting another proxy will destroy the existing proxy and all data added to it.
 *
 * QBar3DSeries supports the following format tags for QAbstract3DSeries::setItemLabelFormat():
 * \table
 *   \row
 *     \li @rowTitle      \li Title from row axis
 *   \row
 *     \li @colTitle      \li Title from column axis
 *   \row
 *     \li @valueTitle    \li Title from value axis
 *   \row
 *     \li @rowIdx        \li Visible row index
 *   \row
 *     \li @colIdx        \li Visible Column index
 *   \row
 *     \li @rowLabel      \li Label from row axis
 *   \row
 *     \li @colLabel      \li Label from column axis
 *   \row
 *     \li @valueLabel    \li Item value formatted using the same format the value axis attached to the graph uses,
 *                            see \l{QValue3DAxis::setLabelFormat()} for more information.
 *   \row
 *     \li @seriesName    \li Name of the series
 *   \row
 *     \li %<format spec> \li Item value in specified format.
 * \endtable
 *
 * For example:
 * \snippet doc_src_qtdatavisualization.cpp 1
 *
 * \sa {Qt Data Visualization Data Handling}
 */

/*!
 * \qmltype Bar3DSeries
 * \inqmlmodule QtDataVisualization
 * \since QtDataVisualization 1.0
 * \ingroup datavisualization_qml
 * \instantiates QBar3DSeries
 * \inherits Abstract3DSeries
 * \brief Base series type for Bars3D.
 *
 * This type  manages the series specific visual elements, as well as series data
 * (via data proxy).
 *
 * For more complete description, see QBar3DSeries.
 *
 * \sa {Qt Data Visualization Data Handling}
 */

/*!
 * \qmlproperty BarDataProxy Bar3DSeries::dataProxy
 *
 * This property holds the active data \a proxy. The series assumes ownership of any proxy set to
 * it and deletes any previously set proxy when a new one is added. The \a proxy cannot be null or
 * set to another series.
 */

/*!
 * \qmlproperty point Bar3DSeries::selectedBar
 *
 * Selects a bar at the \a position. The \a position is the (row, column) position in
 * the data array of the series.
 * Only one bar can be selected at a time.
 * To clear selection from this series, set invalidSelectionPosition as the \a position.
 * If this series is added to a graph, the graph can adjust the selection according to user
 * interaction or if it becomes invalid. Selecting a bar on another added series will also
 * clear the selection.
 * Removing rows from or inserting rows to the series before the row of the selected bar
 * will adjust the selection so that the same bar will stay selected.
 *
 * \sa AbstractGraph3D::clearSelection()
 */

/*!
 * \qmlproperty point Bar3DSeries::invalidSelectionPosition
 * A constant property providing an invalid position for selection. Set this position to
 * selectedBar property if you want to clear the selection from this series.
 *
 * \sa AbstractGraph3D::clearSelection()
 */

/*!
 * Constructs QBar3DSeries with the given \a parent.
 */
QBar3DSeries::QBar3DSeries(QObject *parent) :
    QAbstract3DSeries(new QBar3DSeriesPrivate(this), parent)
{
    // Default proxy
    dptr()->setDataProxy(new QBarDataProxy);
}

/*!
 * Constructs QBar3DSeries with the given \a dataProxy and the \a parent.
 */
QBar3DSeries::QBar3DSeries(QBarDataProxy *dataProxy, QObject *parent) :
    QAbstract3DSeries(new QBar3DSeriesPrivate(this), parent)
{
    dptr()->setDataProxy(dataProxy);
}

/*!
 * \internal
 */
QBar3DSeries::QBar3DSeries(QBar3DSeriesPrivate *d, QObject *parent) :
    QAbstract3DSeries(d, parent)
{
}

/*!
 * Destroys QBar3DSeries.
 */
QBar3DSeries::~QBar3DSeries()
{
}

/*!
 * \property QBar3DSeries::dataProxy
 *
 * This property holds the active data \a proxy. The series assumes ownership of any proxy set to
 * it and deletes any previously set proxy when a new one is added. The \a proxy cannot be null or
 * set to another series.
 */
void QBar3DSeries::setDataProxy(QBarDataProxy *proxy)
{
    d_ptr->setDataProxy(proxy);
}

QBarDataProxy *QBar3DSeries::dataProxy() const
{
    return static_cast<QBarDataProxy *>(d_ptr->dataProxy());
}

/*!
 * \property QBar3DSeries::selectedBar
 *
 * Selects a bar at the \a position. The \a position is the (row, column) position in
 * the data array of the series.
 * Only one bar can be selected at a time.
 * To clear selection from this series, set invalidSelectionPosition() as the \a position.
 * If this series is added to a graph, the graph can adjust the selection according to user
 * interaction or if it becomes invalid. Selecting a bar on another added series will also
 * clear the selection.
 * Removing rows from or inserting rows to the series before the row of the selected bar
 * will adjust the selection so that the same bar will stay selected.
 *
 * \sa QAbstract3DGraph::clearSelection()
 */
void QBar3DSeries::setSelectedBar(const QPoint &position)
{
    // Don't do this in private to avoid loops, as that is used for callback from controller.
    if (d_ptr->m_controller)
        static_cast<Bars3DController *>(d_ptr->m_controller)->setSelectedBar(position, this, true);
    else
        dptr()->setSelectedBar(position);
}

QPoint QBar3DSeries::selectedBar() const
{
    return dptrc()->m_selectedBar;
}

/*!
 * \return an invalid position for selection. Set this position to selectedBar property if you
 * want to clear the selection from this series.
 *
 * \sa QAbstract3DGraph::clearSelection()
 */
QPoint QBar3DSeries::invalidSelectionPosition()
{
    return Bars3DController::invalidSelectionPosition();
}

/*!
 * \internal
 */
QBar3DSeriesPrivate *QBar3DSeries::dptr()
{
    return static_cast<QBar3DSeriesPrivate *>(d_ptr.data());
}

/*!
 * \internal
 */
const QBar3DSeriesPrivate *QBar3DSeries::dptrc() const
{
    return static_cast<const QBar3DSeriesPrivate *>(d_ptr.data());
}

// QBar3DSeriesPrivate

QBar3DSeriesPrivate::QBar3DSeriesPrivate(QBar3DSeries *q)
    : QAbstract3DSeriesPrivate(q, QAbstract3DSeries::SeriesTypeBar),
      m_selectedBar(Bars3DController::invalidSelectionPosition())
{
    m_itemLabelFormat = QStringLiteral("@valueLabel");
    m_mesh = QAbstract3DSeries::MeshBevelBar;
}

QBar3DSeriesPrivate::~QBar3DSeriesPrivate()
{
}

QBar3DSeries *QBar3DSeriesPrivate::qptr()
{
    return static_cast<QBar3DSeries *>(q_ptr);
}

void QBar3DSeriesPrivate::setDataProxy(QAbstractDataProxy *proxy)
{
    Q_ASSERT(proxy->type() == QAbstractDataProxy::DataTypeBar);

    QAbstract3DSeriesPrivate::setDataProxy(proxy);

    emit qptr()->dataProxyChanged(static_cast<QBarDataProxy *>(proxy));
}

void QBar3DSeriesPrivate::connectControllerAndProxy(Abstract3DController *newController)
{
    QBarDataProxy *barDataProxy = static_cast<QBarDataProxy *>(m_dataProxy);

    if (m_controller && barDataProxy) {
        // Disconnect old controller/old proxy
        QObject::disconnect(barDataProxy, 0, m_controller, 0);
        QObject::disconnect(q_ptr, 0, m_controller, 0);
    }

    if (newController && barDataProxy) {
        Bars3DController *controller = static_cast<Bars3DController *>(newController);
        QObject::connect(barDataProxy, &QBarDataProxy::arrayReset, controller,
                         &Bars3DController::handleArrayReset);
        QObject::connect(barDataProxy, &QBarDataProxy::rowsAdded, controller,
                         &Bars3DController::handleRowsAdded);
        QObject::connect(barDataProxy, &QBarDataProxy::rowsChanged, controller,
                         &Bars3DController::handleRowsChanged);
        QObject::connect(barDataProxy, &QBarDataProxy::rowsRemoved, controller,
                         &Bars3DController::handleRowsRemoved);
        QObject::connect(barDataProxy, &QBarDataProxy::rowsInserted, controller,
                         &Bars3DController::handleRowsInserted);
        QObject::connect(barDataProxy, &QBarDataProxy::itemChanged, controller,
                         &Bars3DController::handleItemChanged);
        QObject::connect(barDataProxy, &QBarDataProxy::rowLabelsChanged, controller,
                         &Bars3DController::handleDataRowLabelsChanged);
        QObject::connect(barDataProxy, &QBarDataProxy::columnLabelsChanged, controller,
                         &Bars3DController::handleDataColumnLabelsChanged);
    }
}

void QBar3DSeriesPrivate::setSelectedBar(const QPoint &position)
{
    if (position != m_selectedBar) {
        m_selectedBar = position;
        emit qptr()->selectedBarChanged(m_selectedBar);
    }
}

QT_END_NAMESPACE_DATAVISUALIZATION
