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

#include "q3dsurface.h"
#include "q3dsurface_p.h"
#include "q3dvalueaxis.h"
#include "qsurfacedataproxy.h"
#include "q3dcamera.h"

#include <QMouseEvent>

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

/*!
 * \class Q3DSurface
 * \inmodule QtDataVisualization
 * \brief The Q3DSurface class provides methods for rendering 3D surface plots.
 * \since 1.0.0
 *
 * This class enables developers to render 3D surface plots and to view them by rotating the scene
 * freely. The class provides configurable gradient texture to illustrate the height on the data. The
 * surface plotting includes also gridline that can be set on or off. The visual appearance of the
 * surface can be changed by controlling the smooth status.
 *
 * The Q3DSurface supports selection by showing a highlighted ball on the data point where the user has clicked
 * with left mouse button (when default input handler is in use). The selection pointer is accompanied with
 * a label which in default case shows the value of the data point and the coordinates of the point.
 *
 * The value range and the label format shown on the axis can be controlled through Q3DValueAxis.
 * The Q3DSurface supports only a grid with fixed steps, so when setting ranges set a value that matches
 * the grid step. To calculate the steps divide the whole data range with the number of segments.
 *
 * To rotate the graph, hold down the right mouse button and move the mouse. Zooming is done using mouse
 * wheel. Both assume the default input handler is in use.
 *
 * If no axes are explicitly set to Q3DSurface, temporary default axes with no labels are created.
 * These default axes can be modified via axis accessors, but as soon any axis is explicitly
 * set for the orientation, the default axis for that orientation is destroyed.
 *
 * Data proxies work similarly: If no data proxy is explicitly set, Q3DSurface creates a default
 * proxy. If any other proxy is set as active data proxy later, the default proxy and all data
 * added to it is destroyed.
 *
 * \section1 How to construct a minimal Q3DSurface graph
 *
 * First, construct Q3DSurface:
 *
 * \snippet doc_src_q3dsurface_construction.cpp 0
 *
 * Now Q3DSurface is ready to receive data to be rendered. Create data elements to receive values:
 *
 * \snippet doc_src_q3dsurface_construction.cpp 1
 *
 * First feed the data to the row elements and then add their pointers to the data element:
 *
 * \snippet doc_src_q3dsurface_construction.cpp 2
 *
 * For the active data proxy set pointer of the data element:
 *
 * \snippet doc_src_q3dsurface_construction.cpp 3
 *
 * Finally you will need to set it visible:
 *
 * \snippet doc_src_q3dsurface_construction.cpp 4
 *
 * The complete code needed to create and display this graph is:
 *
 * \snippet doc_src_q3dsurface_construction.cpp 5
 *
 * And this is what those few lines of code produce:
 *
 * \image q3dsurface-minimal.png
 *
 * The scene can be rotated and zoomed into, but no other interaction is included in this minimal
 * code example.
 *
 *
 * \sa Q3DBars, Q3DScatter, {Qt Data Visualization C++ Classes}
 */

/*!
 * Constructs a new 3D surface window.
 */
Q3DSurface::Q3DSurface()
    : d_ptr(new Q3DSurfacePrivate(this, geometry()))
{
    setVisualController(d_ptr->m_shared);
    d_ptr->m_shared->initializeOpenGL();
    QObject::connect(d_ptr->m_shared, &Abstract3DController::needRender, this,
                     &Q3DWindow::renderLater);
}

/*!
 *  Destroys the 3D surface window.
 */
Q3DSurface::~Q3DSurface()
{
}

/*!
 * \internal
 */
void Q3DSurface::mouseDoubleClickEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseDoubleClickEvent(event);
}

/*!
 * \internal
 */
void Q3DSurface::touchEvent(QTouchEvent *event)
{
    d_ptr->m_shared->touchEvent(event);
}

/*!
 * \internal
 */
void Q3DSurface::mousePressEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mousePressEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DSurface::mouseReleaseEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseReleaseEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DSurface::mouseMoveEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseMoveEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DSurface::wheelEvent(QWheelEvent *event)
{
    d_ptr->m_shared->wheelEvent(event);
}

/*!
 * \internal
 */
void Q3DSurface::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    d_ptr->m_shared->setWidth(width());
    d_ptr->m_shared->setHeight(height());
}

/*!
 * \property Q3DSurface::gridVisible
 *
 * Sets grid visibility to \a visible. It is preset to \c true by default.
 */
void Q3DSurface::setGridVisible(bool visible)
{
    d_ptr->m_shared->setGridEnabled(visible);
}

bool Q3DSurface::isGridVisible() const
{
    return d_ptr->m_shared->gridEnabled();
}

/*!
 * \property Q3DSurface::backgroundVisible
 *
 * Sets background visibility to \a visible. It is preset to \c true by default.
 */
void Q3DSurface::setBackgroundVisible(bool visible)
{
    d_ptr->m_shared->setBackgroundEnabled(visible);
}

bool Q3DSurface::isBackgroundVisible() const
{
    return d_ptr->m_shared->backgroundEnabled();
}

/*!
 * \property Q3DSurface::theme
 *
 * A predefined \a theme from \c QDataVis::Theme. It is preset to \c QDataVis::ThemeQt by
 * default. Theme affects label colors, text color, background color, window color and
 * grid color. Lighting is also adjusted by themes.
 *
 * \preliminary
 */
void Q3DSurface::setTheme(QDataVis::Theme theme)
{
    d_ptr->m_shared->setTheme(theme);
}

QDataVis::Theme Q3DSurface::theme() const
{
    return d_ptr->m_shared->theme().theme();
}

/*!
 * \property Q3DSurface::shadowQuality
 *
 * Sets shadow \a quality to one of \c QDataVis::ShadowQuality. It is preset to
 * \c QDataVis::ShadowQualityMedium by default.
 *
 * \note If setting QDataVis::ShadowQuality of a certain level fails, a level is lowered
 * until it is successful and shadowQualityChanged signal is emitted for each time the change is
 * done.
 */
void Q3DSurface::setShadowQuality(QDataVis::ShadowQuality quality)
{
    return d_ptr->m_shared->setShadowQuality(quality);
}

QDataVis::ShadowQuality Q3DSurface::shadowQuality() const
{
    return d_ptr->m_shared->shadowQuality();
}

/*!
 * \property Q3DSurface::smoothSurfaceEnabled
 *
 * Sets surface smoothing to \a enabled. It is preset to \c false by default.
 * When enabled the normals on the surface are interpolated making edges looking round. If turned
 * off the normals are kept same on a triangle making the color of the triangle solid. This makes
 * the data more readable from the model.
 */
void Q3DSurface::setSmoothSurfaceEnabled(bool enabled)
{
    d_ptr->m_shared->setSmoothSurface(enabled);
}

bool Q3DSurface::isSmoothSurfaceEnabled() const
{
    return d_ptr->m_shared->smoothSurface();
}

/*!
 * \property Q3DSurface::selectionMode
 *
 * Sets point selection \a mode to one of \c QDataVis::SelectionMode. Surface supports SelectionModeItem,
 * SelectionModeSliceRow and SelectionModeSliceColumn. It is preset to \c QDataVis::SelectionModeItem by default.
 */
void Q3DSurface::setSelectionMode(QDataVis::SelectionMode mode)
{
    d_ptr->m_shared->setSelectionMode(mode);
}

QDataVis::SelectionMode Q3DSurface::selectionMode() const
{
    return d_ptr->m_shared->selectionMode();
}


/*!
 * \property Q3DSurface::surfaceGridEnabled
 *
 * Sets surface grid to \a enabled. It is preset to \c true by default.
 */
void Q3DSurface::setSurfaceGridEnabled(bool enabled)
{
    d_ptr->m_shared->setSurfaceGrid(enabled);
}

bool Q3DSurface::isSurfaceGridEnabled() const
{
    return d_ptr->m_shared->surfaceGrid();
}

/*!
 * \property Q3DSurface::gradient
 *
 * The current surface gradient. Setting this property replaces the previous gradient with
 * the given \a gradient.
 */
void Q3DSurface::setGradient(const QLinearGradient &gradient)
{
    d_ptr->m_shared->setGradient(gradient);
}

QLinearGradient Q3DSurface::gradient() const
{
    return d_ptr->m_shared->gradient();
}

/*!
 * \property Q3DSurface::font
 *
 * Sets the \a font for labels. It is preset to \c Arial by default.
 */
void Q3DSurface::setFont(const QFont &font)
{
    d_ptr->m_shared->setFont(font);
}

QFont Q3DSurface::font() const
{
    return d_ptr->m_shared->font();
}

/*!
 * \property Q3DSurface::scene
 *
 * This property contains the read only Q3DScene that can be used to access e.g. camera object.
 */
Q3DScene *Q3DSurface::scene() const
{
    return d_ptr->m_shared->scene();
}

/*!
 * \property Q3DSurface::labelStyle
 *
 * Sets label \a style to one of \c QDataVis::LabelStyle. It is preset to
 * \c QDataVis::LabelStyleFromTheme by default.
 */
void Q3DSurface::setLabelStyle(QDataVis::LabelStyle style)
{
    d_ptr->m_shared->setLabelStyle(style);
}

QDataVis::LabelStyle Q3DSurface::labelStyle() const
{
    return d_ptr->m_shared->labelStyle();
}

/*!
 * Sets a user-defined X-axis. Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DSurface::setAxisX(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisX(axis);
}

/*!
 * \return used X-axis.
 */
Q3DValueAxis *Q3DSurface::axisX() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisX());
}

/*!
 * Sets a user-defined Y-axis. Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DSurface::setAxisY(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisY(axis);
}

/*!
 * \return used Y-axis.
 */
Q3DValueAxis *Q3DSurface::axisY() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisY());
}

/*!
 * Sets a user-defined Z-axis. Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DSurface::setAxisZ(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisZ(axis);
}

/*!
 * \return used Z-axis.
 */
Q3DValueAxis *Q3DSurface::axisZ() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisZ());
}

/*!
 * Adds \a axis to the graph. The axes added via addAxis are not yet taken to use,
 * addAxis is simply used to give the ownership of the \a axis to the graph.
 * The \a axis must not be null or added to another graph.
 *
 * \sa releaseAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DSurface::addAxis(Q3DValueAxis *axis)
{
    d_ptr->m_shared->addAxis(axis);
}

/*!
 * Releases the ownership of the \a axis back to the caller, if it is added to this graph.
 * If the released \a axis is in use, a new default axis will be created and set active.
 *
 * If the default axis is released and added back later, it behaves as any other axis would.
 *
 * \sa addAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DSurface::releaseAxis(Q3DValueAxis *axis)
{
    d_ptr->m_shared->releaseAxis(axis);
}

/*!
 * \return list of all added axes.
 *
 * \sa addAxis()
 */
QList<Q3DValueAxis *> Q3DSurface::axes() const
{
    QList<Q3DAbstractAxis *> abstractAxes = d_ptr->m_shared->axes();
    QList<Q3DValueAxis *> retList;
    foreach (Q3DAbstractAxis *axis, abstractAxes)
        retList.append(static_cast<Q3DValueAxis *>(axis));

    return retList;
}

/*!
 * Sets the active data \a proxy. Implicitly calls addDataProxy() to transfer ownership of
 * the \a proxy to this graph.
 *
 * If the \a proxy is null, a temporary default proxy is created and activated.
 * This temporary proxy is destroyed if another \a proxy is explicitly set active via this method.
 *
 * \sa addDataProxy(), releaseDataProxy()
 */
void Q3DSurface::setActiveDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->m_shared->setActiveDataProxy(proxy);
}

/*!
 * \return active data proxy.
 */
QSurfaceDataProxy *Q3DSurface::activeDataProxy() const
{
    return static_cast<QSurfaceDataProxy *>(d_ptr->m_shared->activeDataProxy());
}

/*!
 * Adds data \a proxy to the graph. The proxies added via addDataProxy are not yet taken to use,
 * addDataProxy is simply used to give the ownership of the data \a proxy to the graph.
 * The \a proxy must not be null or added to another graph.
 *
 * \sa releaseDataProxy(), setActiveDataProxy()
 */
void Q3DSurface::addDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->m_shared->addDataProxy(proxy);
}

/*!
 * Releases the ownership of the data \a proxy back to the caller, if it is added to this graph.
 * If the released \a proxy is in use, a new empty default proxy is created and taken to use.
 *
 * If the default \a proxy is released and added back later, it behaves as any other proxy would.
 *
 * \sa addDataProxy(), setActiveDataProxy()
 */
void Q3DSurface::releaseDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->m_shared->releaseDataProxy(proxy);
}

/*!
 * \return list of all added data proxies.
 *
 * \sa addDataProxy()
 */
QList<QSurfaceDataProxy *> Q3DSurface::dataProxies() const
{
    QList<QSurfaceDataProxy *> retList;
    QList<QAbstractDataProxy *> abstractList = d_ptr->m_shared->dataProxies();
    foreach (QAbstractDataProxy *proxy, abstractList)
        retList.append(static_cast<QSurfaceDataProxy *>(proxy));

    return retList;
}


/*!
 * Modifies the current surface gradient. Sets gradient color to \a color at \a pos.
 */
void Q3DSurface::setGradientColorAt(qreal pos, const QColor &color)
{
    d_ptr->m_shared->setGradientColorAt(pos, color);
}

/////////////////// PRIVATE ///////////////////////////////////

Q3DSurfacePrivate::Q3DSurfacePrivate(Q3DSurface *q, QRect rect)
    : q_ptr(q),
      m_shared(new Surface3DController(rect))
{
}

Q3DSurfacePrivate::~Q3DSurfacePrivate()
{
    delete m_shared;
}

QT_DATAVISUALIZATION_END_NAMESPACE
