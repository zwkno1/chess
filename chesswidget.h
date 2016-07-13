#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPixmap>

class Chess;

class ChessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    bool buildPixmap();

signals:

public slots:
    void restart();
    void undo();
    void search();
private:
    QPixmap & getPixmap(char type);
    void coordToBoard(int x, int y, uint8_t & idx);
    void boardToCoord(uint8_t idx, int & x, int & y);

private:
    uint8_t last_pos_;
    bool selected_;

private:
    Chess * chess_;
    QVector<QPixmap> pixmaps_;
};

#endif // CHESSWIDGET_H
