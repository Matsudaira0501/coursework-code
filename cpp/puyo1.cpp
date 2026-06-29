//課題

#include <curses.h>
#include <cstdlib>
#include <stdlib.h>

//ぷよの色を表すの列挙型
//NONEが無し，RED,BLUE,..が色を表す
enum puyocolor { NONE, RED, BLUE, GREEN, YELLOW };

class PuyoArray
{
private:
	puyocolor *data;
	unsigned int data_line;  //盤面状態を保持する配列
	unsigned int data_column;  //盤面の行数
	
	void Release(){
	if (data == NULL) {
		return;
	}

	delete[] data;
	data = NULL;
	}  //dataのメモリ開放
	
public:
	void ChangeSize(unsigned int line, unsigned int column)
	{
		Release();

		//新しいサイズでメモリ確保
		data = new puyocolor[line*column];

		data_line = line;
		data_column = column;
	}  //盤面サイズ変更
	
	unsigned int GetLine()
	{
		return data_line;
	} //盤面の行数を返す
	
	unsigned int GetColumn()
	{
		return data_column;
	}  //盤面の列数を返す
	
	puyocolor GetValue(unsigned int y, unsigned int x)
	{
	if (y >= GetLine() || x >= GetColumn())
	{
		//引数の値が正しくない
		return NONE;
	}

	return data[y*GetColumn() + x];
	}  // 盤面の指定された位置の値を返す
	
	void SetValue(unsigned int y, unsigned int x, puyocolor value)
	{
		if (y >= GetLine() || x >= GetColumn())
		{
			//引数の値が正しくない
			return;
		}

		data[y*GetColumn() + x] = value;
	} //盤面の指定された位置に値を書き込む
	
	PuyoArray(){
		data = NULL;
		data_line = 0;
	    data_column = 0;
	}  //メンバ変数の初期化を行う．具体的には，dataにNULLを代入，data_lineとdata_columnに0を代入．
	
	~PuyoArray(){
		Release();
	}  //Release()を呼びメモリ開放を行う．
};


class PuyoArrayActive : public PuyoArray
{
public:
	int puyorotate; //操作中ぷよの回転状態を0～3の値で格納。

	PuyoArrayActive(){
		puyorotate = 0;
	}
};

class PuyoArrayStack : public PuyoArray
{};


class PuyoControl
{
public:
	void GeneratePuyo(PuyoArrayActive &puyo) 	//盤面に新しいぷよ生成
	{
	int ra = rand () % 4 + 1;
	int rb = rand () % 4 + 1;

	puyocolor newpuyo1;
	//newpuyo1 = RED;
	switch (ra)
	{
	 case 1:
		newpuyo1 = RED;
		break;
	 case 2:	
		newpuyo1 = BLUE;
		break;
	 case 3:
		newpuyo1 = GREEN;
		break;
	 case 4:
		newpuyo1 = YELLOW;
		break;
	}
	

	puyocolor newpuyo2;
	//newpuyo2 = BLUE;

    switch (rb)
	{
	 case 1:
		newpuyo2 = RED;
		break;
	 case 2:	
		newpuyo2 = BLUE;
		break;
	 case 3:
		newpuyo2 = GREEN;
		break;
	 case 4:
		newpuyo2 = YELLOW;
		break;
	}

	puyo.SetValue(0, 5, newpuyo1);
	puyo.SetValue(0, 6, newpuyo2);
	puyo.puyorotate = 0;
	}
	
	bool LandingPuyo(PuyoArrayActive &puyo, PuyoArrayStack &puyostack) 	//操作中ぷよの着地判定
	{
	enum puyocolor col, col1, col2, col3;
	bool landed = false;

	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = 0; x < puyo.GetColumn(); x++)
		{
			if (puyo.GetValue(y, x) != NONE && (y == puyo.GetLine() - 1 || puyostack.GetValue(y+1, x) != NONE))
			{
				landed = true; 
                col = puyo.GetValue(y, x);
				col1 = puyo.GetValue(y, x+1);
				col2 = puyo.GetValue(y, x-1);
				col3 = puyo.GetValue(y-1, x);
				//着地判定されたぷよを消す．本処理は必要に応じて変更する．
				puyo.SetValue(y, x, NONE);
				puyostack.SetValue(y, x, col);
                if (puyo.GetValue(y, x+1) != NONE)
				{
					puyo.SetValue(y, x+1, NONE);
					puyostack.SetValue(y, x+1, col1);
				}   
				if (puyo.GetValue(y, x-1) != NONE)
				{
					puyo.SetValue(y, x-1, NONE);
					puyostack.SetValue(y, x-1, col2);
				} 
				if (puyo.GetValue(y-1, x) != NONE)
				{
					puyo.SetValue(y-1, x, NONE);
					puyostack.SetValue(y-1, x, col3);
				}
			}
		}
	}

	return landed;
	}


	bool REMove(PuyoArrayActive &puyo, PuyoArrayStack &puyostack)
	{
	enum puyocolor col, col1, col2;
	bool landed = false;

	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = 0; x < puyo.GetColumn(); x++)
		{
			if (puyostack.GetValue(y, x) != NONE && puyostack.GetValue(y+1, x) == NONE && y != puyo.GetLine() - 1)
			{
				landed = true; 
                col = puyostack.GetValue(y, x);

				puyostack.SetValue(y, x, NONE);
				puyostack.SetValue(y+1, x, col);
			}
		}
	}

	return landed;
	}
	void MoveLeft(PuyoArrayActive &puyo, PuyoArrayStack &puyostack) 	//左移動
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

			if (0 < x && puyo.GetValue(y, x - 1) == NONE && puyostack.GetValue(y, x - 1) == NONE && (puyo.GetValue(y + 1, x) == NONE || (puyo.GetValue(y + 1, x) != NONE && puyostack.GetValue(y + 1, x - 1) == NONE)))
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
	
	void MoveRight(PuyoArrayActive &puyo, PuyoArrayStack &puyostack)  //右移動
	{
	//一時的格納場所メモリ確保
	puyocolor *puyo_temp = new puyocolor[puyo.GetLine()*puyo.GetColumn()];

	for (int i = 0; i < puyo.GetLine()*puyo.GetColumn(); i++)
	{
		puyo_temp[i] = NONE;
	}

	//1つ右の位置にpuyoactiveからpuyo_tempへとコピーg++ -o puyo1 puyo1.c -lncurses
	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = puyo.GetColumn() - 1; x >= 0; x--)
		{
			if (puyo.GetValue(y, x) == NONE) {
				continue;
			}

			if (x < puyo.GetColumn() - 1 && puyo.GetValue(y, x + 1) == NONE && puyostack.GetValue(y, x + 1) == NONE && (puyo.GetValue(y + 1, x) == NONE || (puyo.GetValue(y + 1, x) != NONE && puyostack.GetValue(y + 1, x + 1) == NONE)))
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
	
	void MoveDown(PuyoArrayActive &puyo)  //下移動
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

	public:

	//ぷよ消滅処理を全座標で行う
	//消滅したぷよの数を返す
	int VanishPuyo(PuyoArrayStack &puyostack)
	{
		int vanishednumber = 0;
		for (int y = 0; y < puyostack.GetLine(); y++)
		{
			for (int x = 0; x < puyostack.GetColumn(); x++)
			{
				vanishednumber += VanishPuyo(puyostack, y, x);
			}
		}

		return vanishednumber;
	}

	//ぷよ消滅処理を座標(x,y)で行う
	//消滅したぷよの数を返す
	int VanishPuyo(PuyoArrayStack &puyostack, unsigned int y, unsigned int x)
	{
		//判定個所にぷよがなければ処理終了
		if (puyostack.GetValue(y, x) == NONE)
		{
			return 0;
		}


		//判定状態を表す列挙型
		//NOCHECK判定未実施，CHECKINGが判定対象，CHECKEDが判定済み
		enum checkstate{ NOCHECK, CHECKING, CHECKED };

		//判定結果格納用の配列
		enum checkstate *field_array_check;
		field_array_check = new enum checkstate[puyostack.GetLine()*puyostack.GetColumn()];

		//配列初期化
		for (int i = 0; i < puyostack.GetLine()*puyostack.GetColumn(); i++)
		{
			field_array_check[i] = NOCHECK;
		}

		//座標(x,y)を判定対象にする
		field_array_check[y*puyostack.GetColumn() + x] = CHECKING;

		//判定対象が1つもなくなるまで，判定対象の上下左右に同じ色のぷよがあるか確認し，あれば新たな判定対象にする
		bool checkagain = true;
		while (checkagain)
		{
			checkagain = false;

			for (int yy = 0; yy < puyostack.GetLine(); yy++)
			{
				for (int xx = 0; xx < puyostack.GetColumn(); xx++)
				{
					//(xx,yy)に判定対象がある場合
					if (field_array_check[yy*puyostack.GetColumn() + xx] == CHECKING)
					{
						//(xx+1,yy)の判定
						if (xx < puyostack.GetColumn() - 1)
						{
							//(xx+1,yy)と(xx,yy)のぷよの色が同じで，(xx+1,yy)のぷよが判定未実施か確認
							if (puyostack.GetValue(yy, xx + 1) == puyostack.GetValue(yy, xx) && field_array_check[yy*puyostack.GetColumn() + (xx + 1)] == NOCHECK)
							{
								//(xx+1,yy)を判定対象にする
								field_array_check[yy*puyostack.GetColumn() + (xx + 1)] = CHECKING;
								checkagain = true;
							}
						}

						//(xx-1,yy)の判定
						if (xx > 0)
						{
							if (puyostack.GetValue(yy, xx - 1) == puyostack.GetValue(yy, xx) && field_array_check[yy*puyostack.GetColumn() + (xx - 1)] == NOCHECK)
							{
								field_array_check[yy*puyostack.GetColumn() + (xx - 1)] = CHECKING;
								checkagain = true;
							}
						}

						//(xx,yy+1)の判定
						if (yy < puyostack.GetLine() - 1)
						{
							if (puyostack.GetValue(yy + 1, xx) == puyostack.GetValue(yy, xx) && field_array_check[(yy + 1)*puyostack.GetColumn() + xx] == NOCHECK)
							{
								field_array_check[(yy + 1)*puyostack.GetColumn() + xx] = CHECKING;
								checkagain = true;
							}
						}

						//(xx,yy-1)の判定
						if (yy > 0)
						{
							if (puyostack.GetValue(yy - 1, xx) == puyostack.GetValue(yy, xx) && field_array_check[(yy - 1)*puyostack.GetColumn() + xx] == NOCHECK)
							{
								field_array_check[(yy - 1)*puyostack.GetColumn() + xx] = CHECKING;
								checkagain = true;
							}
						}

						//(xx,yy)を判定済みにする
						field_array_check[yy*puyostack.GetColumn() + xx] = CHECKED;
					}
				}
			}
		}

		//判定済みの数をカウント
		int puyocount = 0;
		for (int i = 0; i < puyostack.GetLine()*puyostack.GetColumn(); i++)
		{
			if (field_array_check[i] == CHECKED)
			{
				puyocount++;
			}
		}

		//4個以上あれば，判定済み座標のぷよを消す
		int vanishednumber = 0;
		if (4 <= puyocount)
		{
			for (int yy = 0; yy < puyostack.GetLine(); yy++)
			{
				for (int xx = 0; xx < puyostack.GetColumn(); xx++)
				{
					if (field_array_check[yy*puyostack.GetColumn() + xx] == CHECKED)
					{
						puyostack.SetValue(yy, xx, NONE);

						vanishednumber++;
					}
				}
			}
		}

		//メモリ解放
		delete[] field_array_check;

		return vanishednumber;
	}

	//回転
	//PuyoArrayActiveクラスのprivateメンバ変数として int puyorotate を宣言し，これに回転状態を記憶させている．
	//puyorotateにはコンストラクタ及びGeneratePuyo関数で値0を代入する必要あり．
	void Rotate(PuyoArrayActive &puyoactive, PuyoArrayStack &puyostack)
	{
		//フィールドをラスタ順に探索（最も上の行を左から右方向へチェックして，次に一つ下の行を左から右方向へチェックして，次にその下の行・・と繰り返す）し，先に発見される方をpuyo1, 次に発見される方をpuyo2に格納
		puyocolor puyo1, puyo2;
		int puyo1_x = 0;
		int puyo1_y = 0;
		int puyo2_x = 0;
		int puyo2_y = 0;

		bool findingpuyo1 = true;
		for (int y = 0; y < puyoactive.GetLine(); y++)
		{
			for (int x = 0; x < puyoactive.GetColumn(); x++)
			{
				if (puyoactive.GetValue(y, x) != NONE)
				{
					if (findingpuyo1)
					{
						puyo1 = puyoactive.GetValue(y, x);
						puyo1_x = x;
						puyo1_y = y;
						findingpuyo1 = false;
					}
					else
					{
						puyo2 = puyoactive.GetValue(y, x);
						puyo2_x = x;
						puyo2_y = y;
					}
				}
			}
		}


		//回転前のぷよを消す
		puyoactive.SetValue(puyo1_y, puyo1_x, NONE);
		puyoactive.SetValue(puyo2_y, puyo2_x, NONE);


		//操作中ぷよの回転
		switch (puyoactive.puyorotate)
		{
		case 0:
			//回転パターン
			//RB -> R
			//      B
			//Rがpuyo1, Bがpuyo2
			if (puyo2_x <= 0 || puyo2_y >= puyoactive.GetLine() - 1 || puyostack.GetValue(puyo2_y + 1, puyo2_x - 1) != NONE)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
			{
				puyoactive.SetValue(puyo1_y, puyo1_x, puyo1);
				puyoactive.SetValue(puyo2_y, puyo2_x, puyo2);
				break;
			}

			//回転後の位置にぷよを置く
			puyoactive.SetValue(puyo1_y, puyo1_x, puyo1);
			puyoactive.SetValue(puyo2_y + 1, puyo2_x - 1, puyo2);
			//次の回転パターンの設定
			puyoactive.puyorotate = 1;
			break;

		case 1:
			//回転パターン
			//R -> BR
			//B
			//Rがpuyo1, Bがpuyo2
			if (puyo2_x <= 0 || puyo2_y <= 0 || puyostack.GetValue(puyo2_y - 1, puyo2_x - 1) != NONE)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
			{
				puyoactive.SetValue(puyo1_y, puyo1_x, puyo1);
				puyoactive.SetValue(puyo2_y, puyo2_x, puyo2);
				break;
			}

			//回転後の位置にぷよを置く
			puyoactive.SetValue(puyo1_y, puyo1_x, puyo1);
			puyoactive.SetValue(puyo2_y - 1, puyo2_x - 1, puyo2);

			//次の回転パターンの設定
			puyoactive.puyorotate = 2;
			break;

		case 2:
			//回転パターン
			//      B
			//BR -> R
			//Bがpuyo1, Rがpuyo2
			if (puyo1_x >= puyoactive.GetColumn() - 1 || puyo1_y <= 0 || puyostack.GetValue(puyo1_y - 1, puyo1_x + 1) != NONE)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
			{
				puyoactive.SetValue(puyo1_y, puyo1_x, puyo1);
				puyoactive.SetValue(puyo2_y, puyo2_x, puyo2);
				break;
			}

			//回転後の位置にぷよを置く
			puyoactive.SetValue(puyo1_y - 1, puyo1_x + 1, puyo1);
			puyoactive.SetValue(puyo2_y, puyo2_x, puyo2);

			//次の回転パターンの設定
			puyoactive.puyorotate = 3;
			break;

		case 3:
			//回転パターン
			//B
			//R -> RB
			//Bがpuyo1, Rがpuyo2
			if (puyo1_x >= puyoactive.GetColumn() - 1 || puyo1_y >= puyoactive.GetLine() - 1 || puyostack.GetValue(puyo1_y + 1, puyo1_x + 1) != NONE)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
			{
				puyoactive.SetValue(puyo1_y, puyo1_x, puyo1);
				puyoactive.SetValue(puyo2_y, puyo2_x, puyo2);
				break;
			}

			//回転後の位置にぷよを置く
			puyoactive.SetValue(puyo1_y + 1, puyo1_x + 1, puyo1);
			puyoactive.SetValue(puyo2_y, puyo2_x, puyo2);

			//次の回転パターンの設定
			puyoactive.puyorotate = 0;
			break;

		default:
			break;
		}
	}

};

//表示&
void Display(PuyoArrayActive &puyo, PuyoArrayStack &puyostack, int score)
{
	//落下中ぷよ表示
	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = 0; x < puyo.GetColumn(); x++)
		{ 
			int puyo_color;
			puyo_color = puyo.GetValue(y, x);
			if (puyo_color == NONE){
				puyo_color =  puyostack.GetValue(y, x);
			}
			switch (puyo_color)
			{
			case NONE:
				attrset(COLOR_PAIR(5));
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
			
		}
	}

	

	//情報表示
	int count = 0;
	for (int y = 0; y < puyo.GetLine(); y++)
	{
		for (int x = 0; x < puyo.GetColumn(); x++)
		{
			if ((puyo.GetValue(y, x) || puyostack.GetValue(y, x)) != NONE)
			{
				count++;
			}
		}
	}

	char msg[256], msg2[256];
	sprintf(msg, "Field: %d x %d, Puyo number: %03d", puyo.GetLine(), puyo.GetColumn(), count);
	sprintf(msg2, "Score: %d", score);
	mvaddstr(2, COLS - 35, msg);
	mvaddstr(3, COLS - 35, msg2);

	refresh();
}


//ここから実行される
int main(int argc, char **argv){
	//インスタンス作成

	PuyoArrayActive puyo;
	PuyoArrayStack puyostack;
	PuyoControl control;
	
	//画面の初期化
	initscr();
	
	//カラー属性を扱うための初期化
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);

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
	puyo.ChangeSize(LINES/2, COLS/2);
	puyostack.ChangeSize(LINES/2, COLS/2);	//フィールドは画面サイズの縦横1/2にする
	control.GeneratePuyo(puyo);	//最初のぷよ生成

	int delay = 10;
	int waitCount = 5000;

	int puyostate = 0;
	int score = 0;
	int scorebuff = 1;

	//メイン処理ループ
	while (1)
	{
		//キー入力受付
		int ch;
		if(control.REMove(puyo, puyostack) == false)
		{
			ch = getch();
	  	}
		

		//Qの入力で終了
		if (ch == 'Q')
		{
			break;
		}

		//入力キーごとの処理
		switch (ch)
		{
		case KEY_LEFT:
			control.MoveLeft(puyo, puyostack);
			break;
		case KEY_RIGHT:
			control.MoveRight(puyo, puyostack);
			break;
		case 'z':
			control.Rotate(puyo, puyostack);
			break;
		default:
			break;
		}


		//処理速度調整のためのif文
		if (delay%waitCount == 0){
			
			//ぷよ着地判定
			if (control.LandingPuyo(puyo, puyostack))
			{
				
				//着地していたら新しいぷよ生成
				control.GeneratePuyo(puyo);

				scorebuff = 1;
			}
			else
			{
				//ぷよ下に移動
				control.MoveDown(puyo);
			}	
			
			if (control.VanishPuyo(puyostack) != 0)
			{
				score += 1000 * scorebuff;
				scorebuff = 2 * scorebuff;
			}
		}
		
		delay++;

		//表示
		Display(puyo, puyostack, score);
	}


	//画面をリセット
	endwin();

	return 0;
}
