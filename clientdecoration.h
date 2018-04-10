#ifndef CLIENTDECORATION_H
#define CLIENTDECORATION_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPoint>
#include <QMap>
#include <QTime>

class TrafficLights;
class TrafficLightButton;
class ClientDecorationStyle;
class ClientDecorationProperties;

class ClientDecoration : public QQuickPaintedItem {
    Q_OBJECT
    Q_DISABLE_COPY(ClientDecoration)
public:
    ClientDecoration(QQuickItem *parent = nullptr);
    ~ClientDecoration();

    static ClientDecorationProperties *qmlAttachedProperties(QObject *object);

    ClientDecorationStyle *style();

    void updateDimensions();

    void paint(QPainter *p) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QMargins m_borders { 1, 32, 1, 1 };
    QMargins m_shadows { 10, 10, 10, 10 };

    QRectF m_iconRect { };
    QRectF m_titleRect { };

    ClientDecorationStyle *m_style { nullptr };

    TrafficLights *m_trafficLights { nullptr };
};
QML_DECLARE_TYPEINFO(ClientDecoration, QML_HAS_ATTACHED_PROPERTIES)

class ClientDecorationProperties : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool prefersLeftSide READ prefersLeftSide CONSTANT)
public:
    ClientDecorationProperties(QObject *parent = nullptr);

    bool prefersLeftSide();
};

class TrafficLights : public QQuickPaintedItem {
    Q_OBJECT
    Q_DISABLE_COPY(TrafficLights)
public:
    TrafficLights(QQuickItem *parent = nullptr, ClientDecorationStyle *style = nullptr);
    ~TrafficLights();

    ClientDecorationStyle *style();

    void updateDimensions();

    void paint(QPainter *p) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_closeButtonWidth { 48 };
    int m_buttonWidth { 44 };

    QRectF m_minimizeButtonRect { };
    QRectF m_maximizeButtonRect { };
    QRectF m_closeButtonRect { };

    TrafficLightButton *m_minimizeButton { nullptr };
    TrafficLightButton *m_maximizeButton { nullptr };
    TrafficLightButton *m_closeButton { nullptr };

    ClientDecorationStyle *m_style { nullptr };
};

class TrafficLightButton : public QQuickPaintedItem {
    Q_OBJECT
    Q_DISABLE_COPY(TrafficLightButton)
protected:
    TrafficLightButton(TrafficLights *parent = nullptr);
    TrafficLights *trafficLights();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    virtual void activate() = 0;

private:
    QTime m_lastPress;
};

class TrafficLightMinimizeButton : public TrafficLightButton {
    Q_OBJECT
    Q_DISABLE_COPY(TrafficLightMinimizeButton)
public:
    TrafficLightMinimizeButton(TrafficLights *parent = nullptr);
    void paint(QPainter *p) override;
public slots:
    void activate() override;
};

class TrafficLightMaximizeButton : public TrafficLightButton {
    Q_OBJECT
    Q_DISABLE_COPY(TrafficLightMaximizeButton)
public:
    TrafficLightMaximizeButton(TrafficLights *parent = nullptr);
    void paint(QPainter *p) override;
public slots:
    void activate() override;
};

class TrafficLightCloseButton : public TrafficLightButton {
    Q_OBJECT
    Q_DISABLE_COPY(TrafficLightCloseButton)
public:
    TrafficLightCloseButton(TrafficLights *parent = nullptr);
    void paint(QPainter *p) override;
public slots:
    void activate() override;
};

#endif // CLIENTDECORATION_H
