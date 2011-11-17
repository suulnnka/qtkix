#include <QtGui>
#include "qtkix.h"
#include "ui_qtkix.h"

qtkix::qtkix(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::qtkix),
	pix(40*8+1,40*8+1)
{
	ui->setupUi(this);
	board = new Gui_Board;
	new_game();
	ui->label->installEventFilter(this);

	can_move = true;

	ui->spinBox->setValue(10);
	ui->spinBox_2->setValue(14);
	ui->state->setText("");

	ui->label->setAttribute(Qt::WA_PaintOnScreen);
	ui->state->setAttribute(Qt::WA_PaintOnScreen);

}

qtkix::~qtkix()
{
    delete ui;
}
void qtkix::undo(){
    if(! can_move)
        return ;
    can_move = false;

	board->gui_board_undo();

    draw();
    can_move = true;
    return ;
}
void qtkix::auto_undo(){
    if(! can_move)
        return ;
    can_move = false;

	board->gui_board_auto_undo();
	check_whos_turn();

    draw();
    can_move = true;
    return ;
}
void qtkix::new_game(){
    if(! can_move)
        return ;
    can_move = false;
	shape = 0;

	board->gui_board_new_game();

    draw();
	ui->state->setText("");
    can_move = true;
    return ;
}
void qtkix::flip(int move){
    if(! can_move)
        return ;
    can_move = false;

	board->gui_board_flip(move);

    draw();
    check_whos_turn();
    can_move = true;
    return ;
}
void qtkix::search(){
	draw();
    can_move = false;

	ui->state->setText("thinking...");
	this->repaint();

	int deep;
	int space = 64;
	space -= board->gui_board_cnt(QT_BOARD_WHITE);
	space -= board->gui_board_cnt(QT_BOARD_BLACK);

	if(ui->spinBox->value() > 16)
		ui->spinBox->setValue(16);
	if(ui->spinBox_2->value() > 20)
		ui->spinBox_2->setValue(20);
	if(ui->spinBox->value() < 1)
		ui->spinBox->setValue(1);
	if(ui->spinBox_2->value() < 1)
		ui->spinBox_2->setValue(1);

	if(space <= ui->spinBox_2->value())
		deep = space;
	else
		deep = ui->spinBox->value();

	Ai_Data *data = new Ai_Data;
	data->node = 0;
	data->value = 0;
	QTime time;
	time.start();
	int move = board->gui_board_search(deep,data);
	int t = time.elapsed();

	QString s;
	s.append("value : ");
	if(data->value>=0)
		s.append("+");
	s.append(QString::number(data->value));

	s.append("\t\t");
	if(t != 0)
		s.append(QString::number(data->node/t));
	s.append("Kn/s\t\t");
	if((data->node)>=1000){
		double n = (data->node) / 1000;
		char c = 'K';
		if( n >= 1000.0 ){
			n = n / 1000;
			c = 'M';
			if( n >= 1000.0 ){
				n = n / 1000;
				c = 'B';
			}
		}
		int prec = 2;
		if(n>=10.0)
			prec = 1;
		if(n>=100.0)
			prec = 0;
		s.append(QString::number(n,'f',prec));
		s.append(QChar::fromAscii(c));
	}else
		s.append(QString::number(data->node));
	s.append("n");

	ui->state->setText(s);

	board->gui_board_flip(move);

	free(data);
    draw();
    check_whos_turn();
    can_move = true;
    return ;
}
void qtkix::draw(){

	pix.fill(QColor(192,192,192,255));
	QPainter painter(&pix);

	painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

	for(int row = 0; row < 8; row++)
		for(int col = 0; col < 8; col++)
			painter.drawRect(row * 40, col * 40, 40, 40);

	for(int row = 1; row <= 8; row++)
	for(int col = 1; col <= 8; col++){

		int c = board->gui_board_get_color(row*10+col);
		int x =board->gui_board_shape_x(row,col,shape);
		int y =board->gui_board_shape_y(row,col,shape);

		if(c == QT_BOARD_NONE){
			if(board->gui_board_can_move(row*10+col)){
				painter.setPen(QPen(QBrush(Qt::SolidLine), 1));
				painter.setBrush(
					QBrush(QColor(192,192,192,255),Qt::SolidPattern));
				painter.drawRect(x*40-37, y*40-37, 34, 34);
			}
			continue;
		}
		if(c == QT_BOARD_BLACK)
			painter.setBrush(
				QBrush(QColor(14,14,14,255),Qt::SolidPattern));
		if(c == QT_BOARD_WHITE)
			painter.setBrush(
				QBrush(QColor(255,255,255,255),Qt::SolidPattern));
		painter.setPen(QPen(QBrush(Qt::SolidLine), 1,Qt::NoPen));
		painter.drawEllipse(x*40-37,y*40-37,34,34);
	}

	ui->label->setPixmap(pix);

	ui->cntb->setText(QString::number(board->gui_board_cnt(QT_BOARD_BLACK)));
	ui->cntw->setText(QString::number(board->gui_board_cnt(QT_BOARD_WHITE)));



	return ;
}
void qtkix::check_whos_turn(){
	can_move = false;
	if(ui->checkBox->isChecked()){
		can_move = true;
		return ;
	}
	while(board->gui_board_double_hit()){
		search();
		can_move = false;
	}
	draw();
	can_move = true;
    return ;
}
void qtkix::turn90(){
	if(! can_move)
		return ;
	can_move = false;

	if(shape&4)
		shape = (shape&4)|((shape-1)&3);
	else
		shape = (shape&4)|((shape+1)&3);

	draw();
	can_move = true;
	return ;
}
void qtkix::mirrorx(){
	if(! can_move)
		return ;
	can_move = false;

	shape = shape ^ 4;

	draw();
	can_move = true;
	return ;
}
bool qtkix::eventFilter(QObject *someOb, QEvent *ev){
	if(someOb == ui->label && ev->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *me = static_cast<QMouseEvent *>(ev);
		int x = me->x();
		int y = me->y();

		int x_r = x%40;
		int y_r = y%40;

		x = x / 40 + 1 ;
		y = y / 40 + 1 ;

		if(x>=1&&x<=8&&y>=1&&y<=8)
			if(abs(x_r-20)<=16 && abs(y_r-20)<=16){
				flip(board->gui_board_shape_pan(x,y,shape));
			}
		return true;
	}
	return false;
}
void qtkix::on_pushButton_clicked()
{
	new_game();
}

void qtkix::on_pushButton_2_clicked()
{
	mirrorx();
}

void qtkix::on_pushButton_3_clicked()
{
	turn90();
}

void qtkix::on_pushButton_4_clicked()
{
	undo();
}

void qtkix::on_pushButton_5_clicked()
{
	auto_undo();
}

void qtkix::on_pushButton_6_clicked()
{
	board->gui_board_computer_swap_color();
	check_whos_turn();
}

void qtkix::on_checkBox_clicked()
{
	check_whos_turn();
}
