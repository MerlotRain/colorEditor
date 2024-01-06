#include "colorwidget.h"


ColorWidget::ColorWidget(QWidget *parent, ColorComponent component) {}

QColor ColorWidget::color() const { return QColor(); }

ColorWidget::ColorComponent ColorWidget::component() const { return ColorComponent(); }

int ColorWidget::componentValue() const { return 0; }

QPixmap ColorWidget::createDragIcon(const QColor &color) { return QPixmap(); }

void ColorWidget::setColor(const QColor &color, bool emitSignals) {}

void ColorWidget::setComponent(ColorComponent component) {}

void ColorWidget::setComponentValue(int value) {}

void ColorWidget::dragEnterEvent(QDragEnterEvent *event) {}

void ColorWidget::dropEvent(QDropEvent *event) {}

void ColorWidget::mouseMoveEvent(QMouseEvent *event) {}

void ColorWidget::mousePressEvent(QMouseEvent *event) {}

void ColorWidget::mouseReleaseEvent(QMouseEvent *event) {}

int ColorWidget::componentRange() const { return 0; }

int ColorWidget::componentRange(ColorComponent component) const { return 0; }

int ColorWidget::componentValue(ColorComponent component) const { return 0; }

int ColorWidget::hue() const { return 0; }

void ColorWidget::alterColor(QColor &color, int value, ColorComponent component) const {}

const QPixmap &ColorWidget::transparentBackground() { return QPixmap(); }