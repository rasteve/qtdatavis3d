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

import QtQuick 2.1
import QtQuick.Controls 1.0
import com.digia.QtDataVisualization 1.0
import "."

Item {
    id: mainview
    width: 1280
    height: 1024
    visible: true

    Data {
        id: chartData
    }

    Axes {
        id: chartAxes
    }

    Item {
        id: dataView
        width: parent.width - camControlArea.width
        height: parent.height
        anchors.right: parent.right;

        Bars3D {
            id: testChart
            width: dataView.width
            height: dataView.height
            shadowQuality: Bars3D.ShadowQualityMedium
            selectionMode: Bars3D.SelectionModeItem
            font.pointSize: 35
            theme: Bars3D.ThemeRetro
            labelStyle: Bars3D.LabelStyleFromTheme
            dataProxy: chartData.proxy
            barThickness: 0.5
            barSpacing: Qt.size(0.5, 0.5)
            barSpacingRelative: false

            columnAxis: chartAxes.column
            valueAxis: chartAxes.expenses
            itemLabelFormat: "@valueTitle for @colLabel, @rowLabel: @valueLabel"

            onSelectedBarPosChanged: {
                // Set camControlArea current row to selected bar
                var rowRole = chartData.proxy.rowLabels[position.x];
                var colRole = chartData.proxy.columnLabels[position.y];
            }

            // Bind UI controls to the camera
            scene.activeCamera.wrapXRotation: false
            scene.activeCamera.xRotation: camControlArea.xValue
            scene.activeCamera.yRotation: camControlArea.yValue
            scene.activeCamera.zoomLevel: zoomSlider.value
            inputHandler: null
        }
    }

    ControlSurface {
        id: camControlArea
        x: 0
        y: 0
        width:  298
        height: 298
        minXValue: testChart.scene.activeCamera.minXRotation
        minYValue: testChart.scene.activeCamera.minYRotation
        maxXValue: testChart.scene.activeCamera.maxYRotation
        maxYValue: testChart.scene.activeCamera.maxYRotation
    }

    Slider {
        id: zoomSlider
        width: camControlArea.width
        anchors.top: camControlArea.bottom
        value: 100
        minimumValue: 10
        maximumValue: 300
    }

    Button {
        id: mappingToggle
        anchors.bottom: parent.bottom
        width: camControlArea.width
        text: "Show Income"
        onClicked: {
            if (chartData.mapping.valueRole === "expenses") {
                chartData.mapping.valueRole = "income"
                text = "Show Expenses"
                testChart.valueAxis = chartAxes.income
            } else {
                chartData.mapping.valueRole = "expenses"
                text = "Show Income"
                testChart.valueAxis = chartAxes.expenses
            }
        }
    }

    Button {
        id: shadowToggle
        anchors.bottom: mappingToggle.top
        width: camControlArea.width
        text: "Hide Shadows"
        onClicked: {
            if (testChart.shadowQuality == Bars3D.ShadowQualityNone) {
                testChart.shadowQuality = Bars3D.ShadowQualityMedium;
                text = "Hide Shadows"
            } else {
                testChart.shadowQuality = Bars3D.ShadowQualityNone;
                text = "Show Shadows"
            }
        }
    }

    Button {
        id: dataToggle
        anchors.bottom: shadowToggle.top
        width: camControlArea.width
        text: "Show 2010 - 2012"
        onClicked: {
            if (testChart.rowAxis.max !== 6) {
                text = "Show 2010 - 2012"
                chartData.mapping.autoRowCategories = true
            } else {
                text = "Show all years"
                // Explicitly defining row categories, since we do not want to show data for
                // all years in the model, just for the selected ones.
                chartData.mapping.autoRowCategories = false
                chartData.mapping.rowCategories = ["2010", "2011", "2012"]
            }
        }
    }
}