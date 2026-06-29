//課題
//2020/04/17

#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//ぷよの色を表すの列挙型
//NONEが無し，RED,BLUE,..が色を表す
enum puyocolor { NONE, RED, BLUE, GREEN, YELLOW };


class PuyoArray
{
public:
	PuyoArray(){
		data = NULL;
		data_line = 0;
		data_column = 0;
	
	}
	~PuyoArray(){
		Release();
	}

	//盤面サイズ変更
	void ChangeSize(unsigned int line, unsigned int column){
		Release();

		//新しいサイズでメモリ確保
		data = new puyocolor[line*column];

		data_line = line;
		data_column = column;
	}
	
	//盤面の行数を返す
	unsigned int GetLine(){
		return data_line;
	}

	//盤面の列数を返す
	unsigned int GetColumn(){
		return data_column;
	}

	//盤面の指定された位置の値を返す
	puyocolor GetValue(unsigned int y, unsigned int x){
		if (y >= GetLine() || x >= GetColumn()){
			//引数の値が正しくない
			return NONE;
		}

		return data[y*GetColumn() + x];
	}

	//盤面の指定された位置に値を書き込む
	void SetValue(unsigned int y, unsigned int x, puyocolor value){
		if (y >= GetLine() || x >= GetColumn()){
			//引数の値が正しくない
			return;
		}

		data[y*GetColumn() + x] = value;
	}

private:

	puyocolor *data;
	unsigned int data_line;
	unsigned int data_column;

	//メモリ開放
	void Release(){
		if (data == NULL) {
			return;
		}

		delete[] data;
		data = NULL;
	}


};






class PuyoArrayActive : public PuyoArray
{


};






class PuyoArrayStack : public PuyoArray
{
	
};






class PuyoControl
{
public:
	
		

	//盤面に新しいぷよ生成
	void GeneratePuyo(PuyoArrayActive &puyo)
	{
		puyocolor newpuyo1;
		newpuyo1 = RED;

		puyocolor newpuyo2;
		newpuyo2 = BLUE;

		puyocolor newpuyo3;
		newpuyo3 = GREEN;

		puyocolor newpuyo4;
		newpuyo4 = YELLOW;

		puyocolor color[4] = {newpuyo1, newpuyo2, newpuyo3, newpuyo4};
		int i = rand() % 4;
		int j = rand() % 4;


		
		puyocolor puyo1, puyo2;

		puyo1 = color[i];
		puyo2 = color[j];
		puyo.SetValue(0, 5, puyo1);
		puyo.SetValue(0, 6, puyo2);
	}

	//ぷよの着地判定．着地判定があるとtrueを返す
	bool LandingPuyo(PuyoArrayActive &puyo, PuyoArrayStack &landedpuyo, PuyoControl &control)
	{
		bool landed = false;

		for (int y = 0; y < puyo.GetLine(); y++)
		{
			for (int x = 0; x < puyo.GetColumn(); x++)
			{
				if (puyo.GetValue(y, x) != NONE && (y == puyo.GetLine() - 1 || landedpuyo.GetValue(y+1, x) != NONE || landedpuyo.GetValue(y, x+1) != NONE || landedpuyo.GetValue(y, x-1) != NONE))
				{
					landed = true;
					
					//着地判定されたぷよを消す．本処理は必要に応じて変更する．
					landedpuyo.SetValue(y, x, puyo.GetValue(y, x));

					puyo.SetValue(y, x, NONE);

					control.LandingPuyo(puyo, landedpuyo, control);
				}
			}
		}

		return landed;
	}  

	//左移動
	void MoveLeft(PuyoArrayActive &puyo)
	{
		//一時的格納場所メモリ確保
		puyocolor *puyo_temp = new puyocolor[puyo.GetLine()*puyo.GetColumn()];

		for (int i = 0; i < puyo.GetLine()*puyo.GetColumn(); i++)
		{
			puyo_temp[i] = NONE;
		}

		//1つ左の位置にpuyoactiveからpuyo_tempへとコピー
		for (int y = 0; y < puyo.GetLine(); y++)
		{
			for (int x = 0; x < puyo.GetColumn(); x++)
			{
				if (puyo.GetValue(y, x) == NONE) {
					continue;
				}

				if (0 < x && puyo.GetValue(y, x - 1) == NONE)
				{
					puyo_temp[y*puyo.GetColumn() + (x - 1)] = puyo.GetValue(y, x);
					//コピー後に元位置のpuyoactiveのデータは消す
					puyo.SetValue(y, x, NONE);
				}
				else
				{
					puyo_temp[y*puyo.GetColumn() + x] = puyo.GetValue(y, x);
				}
			}
		}

		//puyo_tempからpuyoactiveへコピー
		for (int y = 0; y < puyo.GetLine(); y++)
		{
			for (int x = 0; x < puyo.GetColumn(); x++)
			{
				puyo.SetValue(y, x, puyo_temp[y*puyo.GetColumn() + x]);
			}
		}

		//一時的格納場所メモリ解放
		delete[] puyo_temp;
	}

	//右移動
	void MoveRight(PuyoArrayActive &puyo)
	{
		//一時的格納場所メモリ確保
		puyocolor *puyo_temp = new puyocolor[puyo.GetLine()*puyo.GetColumn()];

		for (int i = 0; i < puyo.GetLine()*puyo.GetColumn(); i++)
		{
			puyo_temp[i] = NONE;
		}

		//1つ右の位置にpuyoactiveからpuyo_tempへとコピー
		for (int y = 0; y < puyo.GetLine(); y++)
		{
			for (int x = puyo.GetColumn() - 1; x >= 0; x--)
			{
				if (puyo.GetValue(y, x) == NONE) {
					continue;
				}

				if (x < puyo.GetColumn() - 1 && puyo.GetValue(y, x + 1) == NONE)
				{
					puyo_temp[y*puyo.GetColumn() + (x + 1)] = puyo.GetValue(y, x);
					//コピー後に元位置のpuyoactiveのデータは消す
					puyo.SetValue(y, x, NONE);
				}
				else
				{
					puyo_temp[y*puyo.GetColumn() + x] = puyo.GetValue(y, x);
				}
			}
		}

		//puyo_tempからpuyoactiveへコピー
		for (int y = 0; y <puyo.GetLine(); y++)
		{
			for (int x = 0; x <puyo.GetColumn(); x++)
			{
				puyo.SetValue(y, x, puyo_temp[y*puyo.GetColumn() + x]);
			}
		}

		//一時的格納場所メモリ解放
		delete[] puyo_temp;
	}

	//下移動
	void MoveDown(PuyoArrayActive &puyo)
	{
		//一時的格納場所メモリ確保
		puyocolor *puyo_temp = new puyocolor[puyo.GetLine()*puyo.GetColumn()];

		for (int i = 0; i < puyo.GetLine()*puyo.GetColumn(); i++)
		{
			puyo_temp[i] = NONE;
		}

		//1つ下の位置にpuyoactiveからpuyo_tempへとコピー
		for (int y = puyo.GetLine() - 1; y >= 0; y--)
		{
			for (int x = 0; x < puyo.GetColumn(); x++)
			{
				if (puyo.GetValue(y, x) == NONE) {
					continue;
				}

				if (y < puyo.GetLine() - 1 && puyo.GetValue(y + 1, x) == NONE)
				{
					puyo_temp[(y + 1)*puyo.GetColumn() + x] = puyo.GetValue(y, x);
					//コピー後に元位置のpuyoactiveのデータは消す
					puyo.SetValue(y, x, NONE);
				}
				else
				{
					puyo_temp[y*puyo.GetColumn() + x] = puyo.GetValue(y, x);
				}
			}
		}

		//puyo_tempからpuyoactiveへコピー
		for (int y = 0; y < puyo.GetLine(); y++)
		{
			for (int x = 0; x < puyo.GetColumn(); x++)
			{
				puyo.SetValue(y, x, puyo_temp[y*puyo.GetColumn() + x]);
			}
		}

		//一時的格納場所メモリ解放
		delete[] puyo_temp;
	}





	




};



// //盤面状態
// puyocolor *data = NULL;
// unsigned int data_line = 0;
// unsigned int data_column = 0;

// //メモリ開放
// void Release()
// {
// 	if (data == NULL) {
// 		return;
// 	}

// 	delete[] data;
// 	data = NULL;
// }

// //盤面サイズ変更
// void ChangeSize(unsigned int line, unsigned int column)
// {
// 	Release();

// 	//新しいサイズでメモリ確保
// 	data = new puyocolor[line*column];

// 	data_line = line;
// 	data_column = column;
// }

// //盤面の行数を返す
// unsigned int GetLine()
// {
// 	return data_line;
// }

// //盤面の列数を返す
// unsigned int GetColumn()
// {
// 	return data_column;
// }

// //盤面の指定された位置の値を返す
// puyocolor GetValue(unsigned int y, unsigned int x)
// {
// 	if (y >= GetLine() || x >= GetColumn())
// 	{
// 		//引数の値が正しくない
// 		return NONE;
// 	}

// 	return data[y*GetColumn() + x];
// }

// //盤面の指定された位置に値を書き込む
// void SetValue(unsigned int y, unsigned int x, puyocolor value)
// {
// 	if (y >= GetLine() || x >= GetColumn())
// 	{
// 		//引数の値が正しくない
// 		return;
// 	}

// 	data[y*GetColumn() + x] = value;
// }


// //盤面に新しいぷよ生成
// void GeneratePuyo()
// {
// 	puyocolor newpuyo1;
// 	newpuyo1 = RED;

// 	puyocolor newpuyo2;
// 	newpuyo2 = BLUE;

// 	puyocolor newpuyo3;
// 	newpuyo3 = GREEN;

// 	puyocolor newpuyo4;
// 	newpuyo4 = YELLOW;

// 	puyocolor color[4] = {newpuyo1, newpuyo2, newpuyo3, newpuyo4};
// 	int i = rand() % 4;
// 	int j = rand() % 4;


	
// 	puyocolor puyo1, puyo2;

// 	puyo1 = color[i];
// 	puyo2 = color[j];
// 	SetValue(0, 5, puyo1);
// 	SetValue(0, 6, puyo2);
// }

// //ぷよの着地判定．着地判定があるとtrueを返す
// bool LandingPuyo()
// {
// 	bool landed = false;

// 	for (int y = 0; y < GetLine(); y++)
// 	{
// 		for (int x = 0; x < GetColumn(); x++)
// 		{
// 			if (GetValue(y, x) != NONE && y == GetLine() - 1)
// 			{
// 				landed = true;

// 				//着地判定されたぷよを消す．本処理は必要に応じて変更する．
// 				SetValue(y, x, NONE);
// 			}
// 		}
// 	}

// 	return landed;
// }

// //左移動
// void MoveLeft()
// {
// 	//一時的格納場所メモリ確保
// 	puyocolor *puyo_temp = new puyocolor[GetLine()*GetColumn()];

// 	for (int i = 0; i < GetLine()*GetColumn(); i++)
// 	{
// 		puyo_temp[i] = NONE;
// 	}

// 	//1つ左の位置にpuyoactiveからpuyo_tempへとコピー
// 	for (int y = 0; y < GetLine(); y++)
// 	{
// 		for (int x = 0; x < GetColumn(); x++)
// 		{
// 			if (GetValue(y, x) == NONE) {
// 				continue;
// 			}

// 			if (0 < x && GetValue(y, x - 1) == NONE)
// 			{
// 				puyo_temp[y*GetColumn() + (x - 1)] = GetValue(y, x);
// 				//コピー後に元位置のpuyoactiveのデータは消す
// 				SetValue(y, x, NONE);
// 			}
// 			else
// 			{
// 				puyo_temp[y*GetColumn() + x] = GetValue(y, x);
// 			}
// 		}
// 	}

// 	//puyo_tempからpuyoactiveへコピー
// 	for (int y = 0; y < GetLine(); y++)
// 	{
// 		for (int x = 0; x < GetColumn(); x++)
// 		{
// 			SetValue(y, x, puyo_temp[y*GetColumn() + x]);
// 		}
// 	}

// 	//一時的格納場所メモリ解放
// 	delete[] puyo_temp;
// }

// //右移動
// void MoveRight()
// {
// 	//一時的格納場所メモリ確保
// 	puyocolor *puyo_temp = new puyocolor[GetLine()*GetColumn()];

// 	for (int i = 0; i < GetLine()*GetColumn(); i++)
// 	{
// 		puyo_temp[i] = NONE;
// 	}

// 	//1つ右の位置にpuyoactiveからpuyo_tempへとコピー
// 	for (int y = 0; y < GetLine(); y++)
// 	{
// 		for (int x = GetColumn() - 1; x >= 0; x--)
// 		{
// 			if (GetValue(y, x) == NONE) {
// 				continue;
// 			}

// 			if (x < GetColumn() - 1 && GetValue(y, x + 1) == NONE)
// 			{
// 				puyo_temp[y*GetColumn() + (x + 1)] = GetValue(y, x);
// 				//コピー後に元位置のpuyoactiveのデータは消す
// 				SetValue(y, x, NONE);
// 			}
// 			else
// 			{
// 				puyo_temp[y*GetColumn() + x] = GetValue(y, x);
// 			}
// 		}
// 	}

// 	//puyo_tempからpuyoactiveへコピー
// 	for (int y = 0; y <GetLine(); y++)
// 	{
// 		for (int x = 0; x <GetColumn(); x++)
// 		{
// 			SetValue(y, x, puyo_temp[y*GetColumn() + x]);
// 		}
// 	}

// 	//一時的格納場所メモリ解放
// 	delete[] puyo_temp;
// }

// //下移動
// void MoveDown()
// {
// 	//一時的格納場所メモリ確保
// 	puyocolor *puyo_temp = new puyocolor[GetLine()*GetColumn()];

// 	for (int i = 0; i < GetLine()*GetColumn(); i++)
// 	{
// 		puyo_temp[i] = NONE;
// 	}

// 	//1つ下の位置にpuyoactiveからpuyo_tempへとコピー
// 	for (int y = GetLine() - 1; y >= 0; y--)
// 	{
// 		for (int x = 0; x < GetColumn(); x++)
// 		{
// 			if (GetValue(y, x) == NONE) {
// 				continue;
// 			}

// 			if (y < GetLine() - 1 && GetValue(y + 1, x) == NONE)
// 			{
// 				puyo_temp[(y + 1)*GetColumn() + x] = GetValue(y, x);
// 				//コピー後に元位置のpuyoactiveのデータは消す
// 				SetValue(y, x, NONE);
// 			}
// 			else
// 			{
// 				puyo_temp[y*GetColumn() + x] = GetValue(y, x);
// 			}
// 		}
// 	}

// 	//puyo_tempからpuyoactiveへコピー
// 	for (int y = 0; y < GetLine(); y++)
// 	{
// 		for (int x = 0; x < GetColumn(); x++)
// 		{
// 			SetValue(y, x, puyo_temp[y*GetColumn() + x]);
// 		}
// 	}

// 	//一時的格納場所メモリ解放
// 	delete[] puyo_temp;
// }





//表示
void Display(PuyoArrayActive &puyo, PuyoArrayStack &landedpuyo)
{


	
	//落下中ぷよ表示+着地済みぷよ表示
	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = 0; x < puyo.GetColumn(); x++)
		{
			switch (puyo.GetValue(y, x))
			{
			case NONE:
				attrset(COLOR_PAIR(0));
				mvaddch(y, x, '.');
				break;
			case RED:
				attrset(COLOR_PAIR(1));
				mvaddch(y, x, 'R');
				break;
			case BLUE:
				attrset(COLOR_PAIR(2));
				mvaddch(y, x, 'B');
				break;
			case GREEN:
				attrset(COLOR_PAIR(3));
				mvaddch(y, x, 'G');
				break;
			case YELLOW:
				attrset(COLOR_PAIR(4));
				mvaddch(y, x, 'Y');
				break;
			default:
				mvaddch(y, x, '?');
				break;
			}

			switch (landedpuyo.GetValue(y, x))
			{
			case NONE:
				// attrset(COLOR_PAIR(0));
				// mvaddch(y, x, '.');
				break;
			case RED:
				attrset(COLOR_PAIR(1));
				mvaddch(y, x, 'R');
				break;
			case BLUE:
				attrset(COLOR_PAIR(2));
				mvaddch(y, x, 'B');
				break;
			case GREEN:
				attrset(COLOR_PAIR(3));
				mvaddch(y, x, 'G');
				break;
			case YELLOW:
				attrset(COLOR_PAIR(4));
				mvaddch(y, x, 'Y');
				break;
			default:
				mvaddch(y, x, '?');
				break;
			}
			
		}
	}


	// //着地済みぷよ表示
	// for (int y = 0; y < landedpuyo.GetLine(); y++)
	// {
	// 	for (int x = 0; x < landedpuyo.GetColumn(); x++)
	// 	{
	// 		switch (landedpuyo.GetValue(y, x))
	// 		{
	// 		case NONE:
	// 			attrset(COLOR_PAIR(0));
	// 			mvaddch(y, x, '.');
	// 			break;
	// 		case RED:
	// 			attrset(COLOR_PAIR(1));
	// 			mvaddch(y, x, 'R');
	// 			break;
	// 		case BLUE:
	// 			attrset(COLOR_PAIR(2));
	// 			mvaddch(y, x, 'B');
	// 			break;
	// 		case GREEN:
	// 			attrset(COLOR_PAIR(3));
	// 			mvaddch(y, x, 'G');
	// 			break;
	// 		case YELLOW:
	// 			attrset(COLOR_PAIR(4));
	// 			mvaddch(y, x, 'Y');
	// 			break;
	// 		default:
	// 			mvaddch(y, x, '?');
	// 			break;
	// 		}
			
	// 	}
	// }



	//情報表示
	int count = 0;
	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = 0; x < puyo.GetColumn(); x++)
		{
			if (puyo.GetValue(y, x) != NONE)
			{
				count++;
			}
		}
	}

	char msg[256];
	sprintf(msg, "Field: %d x %d, Puyo number: %03d", puyo.GetLine(), puyo.GetColumn(), count);
	mvaddstr(2, COLS - 35, msg);

	refresh();
}


//ここから実行される
int main(int argc, char **argv){



	PuyoArrayActive puyo;
	PuyoControl control;
	PuyoArrayStack landedpuyo;
	//画面の初期化
	initscr();
	//カラー属性を扱うための初期化
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);


	//キーを押しても画面に表示しない
	noecho();
	//キー入力を即座に受け付ける
	cbreak();

	curs_set(0);
	//キー入力受付方法指定
	keypad(stdscr, TRUE);

	//キー入力非ブロッキングモード
	timeout(0);


	//初期化処理
	puyo.ChangeSize(LINES/2, COLS/2);	//フィールドは画面サイズの縦横1/2にする
	landedpuyo.ChangeSize(LINES/2, COLS/2);
	control.GeneratePuyo(puyo);	//最初のぷよ生成

	int delay = 0;
	int waitCount = 20000;

	int puyostate = 0;


	//メイン処理ループ
	while (1)
	{
		//キー入力受付
		int ch;
		ch = getch();

		//Qの入力で終了
		if (ch == 'Q')
		{
			break;
		}

		//入力キーごとの処理
		switch (ch)
		{
		case KEY_LEFT:
			control.MoveLeft(puyo);
			break;
		case KEY_RIGHT:
			control.MoveRight(puyo);
			break;
		case 'z':
			//ぷよ回転処理
			break;
		default:
			break;
		}


		//処理速度調整のためのif文
		if (delay%waitCount == 0){
			//ぷよ下に移動
			control.MoveDown(puyo);
			
			//ぷよ着地判定
			if (control.LandingPuyo(puyo, landedpuyo, control))
			{
				//着地していたら新しいぷよ生成
				control.GeneratePuyo(puyo);
			}
		}
		delay++;

		//表示
		Display(puyo, landedpuyo);
	}


	//画面をリセット
	endwin();
	

	return 0;
}
