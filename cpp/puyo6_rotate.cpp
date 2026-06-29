public:

	//回転
	//PuyoArrayActiveクラスのprivateメンバ変数として int puyorotate を宣言し，これに回転状態を記憶させている．
	//puyorotateにはコンストラクタ及びGeneratePuyo関数で値0を代入する必要あり．
	void Rotate(PuyoArrayActive &puyoactive)
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
			if (puyo2_x <= 0 || puyo2_y >= puyoactive.GetLine() - 1)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
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
			if (puyo2_x <= 0 || puyo2_y <= 0)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
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
			if (puyo1_x >= puyoactive.GetColumn() - 1 || puyo1_y <= 0)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
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
			if (puyo1_x >= puyoactive.GetColumn() - 1 || puyo1_y >= puyoactive.GetLine() - 1)	//もし回転した結果field_arrayの範囲外に出るなら回転しない
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


prog: ex004_array.o ex004.o\
	cc ex004_array.o ex004.o -o prog\
ex004_array.o: ex004_array.c ex004_array.h\
	cc -c ex004_array.c\
ex004.o: ex004.c ex004_array.h\
	cc -c ex004.c