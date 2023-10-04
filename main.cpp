#include <Novice.h>
#include <corecrt_math_defines.h>
#include <corecrt_math.h>
#include "Easing.h"


const char kWindowTitle[] = "3week";



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	struct Vector2
	{
		float x;
		float y;
	};

	struct Player
	{
		Vector2 pos;
		Vector2 radius;
		Vector2 length;
		Vector2 movePos;
		Vector2 velocity;
		Vector2 acceleration;
		float easeT;
		bool move;
		bool shot;
		bool jump;

	};

	struct PlayerChild
	{
		Vector2 pos;
		Vector2 radius;
		Vector2 length;
		Vector2 movePos;
		Vector2 prepos;

		float easeT;
	};
	struct PlayerPoint
	{
		Vector2 pos;
		Vector2 radius;
		Vector2 length;
		Vector2 movePos;
		float easeT;
	};


	Player player = {//プレイヤー　紫、青
		{300,300},
		{50,50},
		{100}
	};
	PlayerPoint point = {//ポイント　赤
		{player.pos.x,player.pos.y},
		{30,30}
	};

	PlayerChild anchor = {//アンカー　薄い赤
	{player.pos.x,player.pos.y},
	{10,10}
	};

	PlayerChild direction = {//　方向　薄い赤
	{0},
	{10,10}
	};

	float easeChange = 1;//アンカーと方向がmaxとminを行き来する

	Vector2 centorPoint = { player.pos.x,player.pos.y };//ゲージ真ん中の場所

	float playerTopoint = 300;//playerからpointまで　pointは壁を貫通

	float shotPower = 300;//ショットパワー　見た目だけこのサイズで、計算するときは/10してる

	float easePlayerToPoint = 0;//最初に赤玉を発射する

	player.acceleration.y = 0.5f;//重力
	float e = 0.7f;//反発係数


	int timer = 0;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///                                                            ///
		/// --------------------↓更新処理ここから-------------------- ///
		///                                                            ///       


		if (keys[DIK_SPACE])
		{
			if (!player.move)player.shot = true;
		}
		else
		{
			if (player.shot && easePlayerToPoint == 100)
			{
				//赤玉が完全に動いたら、ポイントに向かう動きが始まる。
				player.move = true;
			}
			if (easePlayerToPoint == 100)
			{
				player.shot = false;
			}

			if (!player.move&&!player.shot)
			{
				point.pos.x = player.pos.x;
			}
		}


		point.pos.y = player.pos.y;//赤玉のy軸をプレイヤーに合わせる
		centorPoint.y = player.pos.y;//ゲージ真ん中のy軸をプレイヤーに合わせる
		anchor.pos.y = player.pos.y;//ゲージ動く玉のy軸をプレイヤーに合わせる

		if (direction.easeT < 50)
		{
			//　左下から上の動き
			//constantは指定の場所まで一定の速度で移動するイージング関数。速度は()内　場所は*の後
			direction.pos.x = anchor.pos.x + constant(1 - direction.easeT / 50) * -shotPower;
			direction.pos.y = player.pos.y + constant(direction.easeT / 50) * -shotPower;

		}
		else
		{	
			//　上から右下の動き
			direction.pos.x = anchor.pos.x + constant(direction.easeT / 50 - 1) * shotPower;
			direction.pos.y = player.pos.y + constant(direction.easeT / 50 - 1) * shotPower - shotPower;

		}

		if (player.shot)//プレイヤーからポイントまでアンカーを動かす
		{
			//constantは指定の場所まで一定の速度で移動するイージング関数。速度は()内　場所は*の後
			point.pos.x = player.pos.x + easeOutExpo(easePlayerToPoint / 100) * playerTopoint;

			anchor.pos.x = player.pos.x + constant(anchor.easeT / 100) * playerTopoint;
			centorPoint.x = player.pos.x + playerTopoint / 2;//ゲージ真ん中の位置　

			//最初の赤玉の動きのeaseT
			if (easePlayerToPoint < 100)
			{
				easePlayerToPoint += 10;
			}
			else
			{
				easePlayerToPoint = 100;
			}





			//アンカーと方向のeaseT　maxとminで折り返す
			if (timer % 1 == 0 && easePlayerToPoint == 100)
			{
				direction.easeT += 1 * easeChange;
				anchor.easeT += 1 * easeChange;

			}
			if (anchor.easeT > 100)
			{
				anchor.easeT = 100;
				direction.easeT = 100;

				//折り返し変数
				easeChange *= -1;
			}
			else if (anchor.easeT < 0)
			{
				direction.easeT = 0;
				anchor.easeT = 0;
				easeChange *= -1;
			}


			player.length.x = anchor.pos.x - player.pos.x;//spaceを離した時の動きに使う。今の場所からアンカーまでの距離
			point.length.x = anchor.pos.x - point.pos.x;
			player.movePos.x = player.pos.x;//spaceを離した時の動きに使う。今の場所を保存しておく。prePosみたいなもの
			point.movePos.x = point.pos.x;

		}
		else
		{
			anchor.prepos.x = anchor.pos.x;//スペースを離した時になんかする。多分速度の計算
			anchor.prepos.y = anchor.pos.y;
			direction.prepos.x = direction.pos.x;
			direction.prepos.y = direction.pos.y;
		}
		if (player.move)
		{
			//スペースを離した時にアンカーに向かってくる動き
			player.pos.x = player.movePos.x + easeInQuint(player.easeT / 100) * player.length.x;
			point.pos.x = point.movePos.x + easeInQuint(point.easeT / 100) * point.length.x;

			//アンカーに向かってくる動きのeaseT
			if (player.easeT < 100)
			{
				player.easeT += 2;
				point.easeT += 2;
			}
			else if (player.easeT >= 100)
			{
				//easeTが100になったここで速度を計算
				player.velocity.x = (direction.prepos.x - anchor.prepos.x) / 10;
				player.velocity.y = (direction.prepos.y - anchor.prepos.y) / 10;


				//色々初期化　
				player.move = false;

				//この辺はスペースを押した瞬間でも良いかも
				player.easeT = 0;
				point.easeT = 0;
				anchor.easeT = 0;
				direction.easeT = 0;
				easeChange = 1;
				easePlayerToPoint = 0;

			}


		}

		player.velocity.x += player.acceleration.x;//プレイヤーの速度を計算
		player.pos.x += player.velocity.x;//プレイヤーの位置を変更
		player.velocity.y += player.acceleration.y;
		player.pos.y += player.velocity.y;


		//画面外に出ない用と跳ねる度に速度が落ちる。
		if (player.pos.y > 900 - player.radius.y)
		{
			player.pos.y = 900 - player.radius.y;
			player.velocity.y *= -e;
		}
		if (player.pos.y < 0 + player.radius.y)
		{
			player.pos.y = 0 + player.radius.y;
			player.velocity.y *= -e;

		}
		if (player.pos.x < 0 + player.radius.x)
		{
			player.pos.x = 0 + player.radius.x;
			player.velocity.x *= -e;

		}
		if (player.pos.x > 1920 - player.radius.x)
		{
			player.pos.x = 1920 - player.radius.x;
			player.velocity.x *= -e;
		}


		timer++;
		///                                                            ///
		/// --------------------↑更新処理ここまで-------------------- ///
		///                                                            ///       

		///                                                            ///
		/// --------------------↓描画処理ここから-------------------- ///
		///                                                            ///


		Novice::ScreenPrintf(100, 80, "player.shot=%d,player.move=%d", player.shot, player.move);
		Novice::ScreenPrintf(100, 100, "player.velocity.x=%0.2f,player.velocity.y=%0.2f,player.acceleration.x=%0.2f,player.acceleration.y=%0.2f,", player.velocity.x, player.velocity.y, player.acceleration.x, player.acceleration.y);
		Novice::ScreenPrintf(100, 120, "point.pos.x=%0.1f,point.pos.y=%0.1f,point.easeT=%0.1f", point.pos.x, point.pos.y, point.easeT);
		Novice::ScreenPrintf(100, 140, "direction.easeT=%f", direction.easeT);

		//地形
		Novice::DrawBox(0, 900, 1920, 1080, 0, 0x111111ff, kFillModeSolid);
		//プレイヤー
		Novice::DrawEllipse(int(player.pos.x), int(player.pos.y), int(player.radius.x), int(player.radius.y), 0, BLUE, kFillModeSolid);
		//ポイント
		Novice::DrawEllipse(int(point.pos.x), int(point.pos.y), int(point.radius.x), int(point.radius.y), 0, RED, kFillModeSolid);
		//ポイントとプレイヤーが合体してる時
		if (!player.shot && !player.move)Novice::DrawEllipse(int(player.pos.x), int(player.pos.y), int(player.radius.x), int(player.radius.y), 0, 0xff00ffff, kFillModeSolid);

		//プレイヤーとポイントまでのゲージ
		Novice::DrawBox(int(player.pos.x), int(player.pos.y - 5), int(point.pos.x - player.pos.x), 10, 0, GREEN, kFillModeSolid);
		if (player.shot)
		{
			//ゲージ真ん中の点
			Novice::DrawBox(int(player.pos.x + playerTopoint / 2 - 5), int(player.pos.y - 5), 10, 10, 0, RED, kFillModeSolid);

			//円と三角形の線
			Novice::DrawEllipse(int(anchor.pos.x), int(anchor.pos.y), (int)shotPower, (int)shotPower, 0, GREEN, kFillModeWireFrame);
			Novice::DrawLine(int(anchor.pos.x - shotPower), int(anchor.pos.y), int(anchor.pos.x), int(anchor.pos.y - shotPower), GREEN);
			Novice::DrawLine(int(anchor.pos.x), int(anchor.pos.y - shotPower), int(anchor.pos.x + shotPower), int(anchor.pos.y), GREEN);
			Novice::DrawLine(int(anchor.pos.x - shotPower), int(anchor.pos.y), int(anchor.pos.x + shotPower), int(anchor.pos.y), GREEN);

			//Novice::DrawEllipse(int(centorPoint.x + direction.pos.x), int(centorPoint.y + direction.pos.y), 10, 10, 0, RED, kFillModeSolid);
			//Novice::DrawLine(int(centorPoint.x), int(centorPoint.y), int(centorPoint.x + direction.pos.x), int(centorPoint.y + direction.pos.y), RED);
		}
		if (player.shot || player.move)
		{
			//アンカーとポイントとそれを繋ぐ線
			Novice::DrawEllipse(int(anchor.pos.x), int(anchor.pos.y), int(anchor.radius.x), int(anchor.radius.y), 0, 0xff6666ff, kFillModeSolid);
			Novice::DrawEllipse(int(direction.pos.x), int(direction.pos.y), 10, 10, 0, 0xff6666ff, kFillModeSolid);
			Novice::DrawLine(int(anchor.pos.x), int(anchor.pos.y), int(direction.pos.x), int(direction.pos.y), 0xff6666ff);

		}

		///                                                            ///
		/// --------------------↑描画処理ここまで-------------------- ///
		///                                                            ///       


		//今の仕様は、ショットをするとポイントが射出→ゲージ上のアンカーの動き→スペース離す→アンカーから方向までのベクトルを計算、代入
		//赤玉が位置についたらまたショット出来る。連打は赤玉の動きがあるから後ろに下がる。単押しだと真後ろに下がるので何かに使えるかも
		//動いている時にショットすると、離した時にベクトルがリセットする。アンカーに向かう動きで止まっているように見える。
		//y軸

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
