#ifndef QTKIX_H
#define QTKIX_H

#include <QMainWindow>

#include "gui_board.h"

namespace Ui {
    class qtkix;
}

class qtkix : public QMainWindow
{
    Q_OBJECT

public:
	Gui_Board *board;
    int can_move ;
	int shape;
	QPixmap pix;
    explicit qtkix(QWidget *parent = 0);
    void undo();
    void auto_undo();
    void new_game();
	void flip(int move);
    void search();
    void draw();
    void check_whos_turn();
	void turn90();
	void mirrorx();
    ~qtkix();

protected:
	bool eventFilter(QObject *someOb, QEvent *ev);


private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_pushButton_5_clicked();

	void on_pushButton_6_clicked();

	void on_checkBox_clicked();

private:
    Ui::qtkix *ui;
};

#endif // QTKIX_H
