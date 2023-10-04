#include <Novice.h>
#include <corecrt_math_defines.h>
#include <corecrt_math.h>
#include "Easing.h"


const char kWindowTitle[] = "3week";



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

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


	Player player = {
		{300,300},
		{50,50},
		{100}
	};
	PlayerPoint point = {
		{player.pos.x,player.pos.y},
		{30,30}
	};

	PlayerChild anchor = {
	{player.pos.x,player.pos.y},
	{10,10}
	};

	PlayerChild direction = {
	{0},
	{10,10}
	};

	float easeChange = 1;

	Vector2 centorPoint = { player.pos.x,player.pos.y };

	float playerTopoint = 300;//playerからpointまで　pointは壁を貫通

	float shotPower = 300;

	float easePlayerToPoint = 0;


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
			if (player.shot&&easePlayerToPoint == 100)
			{
				player.move = true;
			}
			if(easePlayerToPoint == 100)player.shot = false;

		}

		if (player.shot)//プレイヤーからポイントまでアンカーを動かす
		{
			point.pos.x = player.pos.x + easeOutExpo(easePlayerToPoint/100) * playerTopoint;

			if (easePlayerToPoint<100)
			{
				easePlayerToPoint += 10;
			}
			else
			{
				easePlayerToPoint = 100;
			}

				


			centorPoint.x = player.pos.x + playerTopoint / 2;
			centorPoint.y = player.pos.y;

			anchor.pos.x = player.pos.x + constant(anchor.easeT / 100) * playerTopoint;

			if (direction.easeT < 50)
			{
				direction.pos.x = constant(1 - direction.easeT / 50) * -shotPower;
				direction.pos.y = constant(direction.easeT / 50) * -shotPower;

			}
			else
			{
				direction.pos.x = constant(direction.easeT / 50 - 1) * shotPower;
				direction.pos.y = constant(direction.easeT / 50 - 1) * shotPower - shotPower;

			}
			
			if (timer % 1 == 0 && easePlayerToPoint == 100)
			{
				direction.easeT += 1 * easeChange;
				anchor.easeT += 1 * easeChange;

			}
			if (anchor.easeT > 100)
			{
				anchor.easeT = 100;
				direction.easeT = 100;

				easeChange *= -1;
			}
			else if (anchor.easeT < 0)
			{
				direction.easeT = 0;
				anchor.easeT = 0;
				easeChange *= -1;
			}


			player.length.x = anchor.pos.x - player.pos.x;
			point.length.x = anchor.pos.x - point.pos.x;
			player.movePos.x = player.pos.x;
			point.movePos.x = point.pos.x;

		}
		if (player.move)
		{
			player.pos.x = player.movePos.x + easeInQuint(player.easeT / 100) * player.length.x;
			point.pos.x = point.movePos.x + easeInQuint(point.easeT / 100) * point.length.x;

			if (player.easeT < 100)
			{
				player.easeT += 2;
				point.easeT += 2;
			}
			else if (player.easeT >= 100)
			{
				player.move = false;
				player.easeT = 0;
				point.easeT = 0;
				anchor.easeT = 0;
				direction.easeT = 0;
				easeChange = 1;
				easePlayerToPoint = 0;

			}


		}
		timer++;
		///                                                            ///
		/// --------------------↑更新処理ここまで-------------------- ///
		///                                                            ///       

		///                                                            ///
		/// --------------------↓描画処理ここから-------------------- ///
		///                                                            ///

		Novice::ScreenPrintf(100, 100, "anchor.easeT=%f", anchor.easeT);
		Novice::ScreenPrintf(100, 120, "point.pos.x=%0.1f,point.pos.y=%0.1f,point.easeT=%0.1f", point.pos.x, point.pos.y, point.easeT);
		Novice::ScreenPrintf(100, 140, "direction.easeT=%f", direction.easeT);


		Novice::DrawEllipse(int(player.pos.x), int(player.pos.y), int(player.radius.x), int(player.radius.y), 0, BLUE, kFillModeSolid);
		Novice::DrawEllipse(int(point.pos.x), int(point.pos.y), int(point.radius.x), int(point.radius.y), 0, RED, kFillModeSolid);
		if(!player.shot&&!player.move)		Novice::DrawEllipse(int(player.pos.x), int(player.pos.y), int(player.radius.x), int(player.radius.y), 0, 0xff00ffff, kFillModeSolid);


		Novice::DrawBox(int(player.pos.x), int(player.pos.y - 5), int(point.pos.x - player.pos.x), 10, 0, GREEN, kFillModeSolid);
		if (player.shot)
		{
			Novice::DrawBox(int(player.pos.x + playerTopoint / 2 - 5), int(player.pos.y - 5), 10, 10, 0, RED, kFillModeSolid);

			Novice::DrawEllipse(int(anchor.pos.x), int(anchor.pos.y), (int)shotPower, (int)shotPower, 0, GREEN, kFillModeWireFrame);
			Novice::DrawLine(int(anchor.pos.x - shotPower), int(anchor.pos.y), int(anchor.pos.x), int(anchor.pos.y - shotPower), GREEN);
			Novice::DrawLine(int(anchor.pos.x), int(anchor.pos.y - shotPower), int(anchor.pos.x + shotPower), int(anchor.pos.y), GREEN);
			Novice::DrawLine(int(anchor.pos.x - shotPower), int(anchor.pos.y), int(anchor.pos.x + shotPower), int(anchor.pos.y), GREEN);

			//Novice::DrawEllipse(int(centorPoint.x + direction.pos.x), int(centorPoint.y + direction.pos.y), 10, 10, 0, RED, kFillModeSolid);
			//Novice::DrawLine(int(centorPoint.x), int(centorPoint.y), int(centorPoint.x + direction.pos.x), int(centorPoint.y + direction.pos.y), RED);
		}
		Novice::DrawEllipse(int(anchor.pos.x), int(anchor.pos.y), int(anchor.radius.x), int(anchor.radius.y), 0, 0xff6666ff, kFillModeSolid);
		Novice::DrawEllipse(int(anchor.pos.x + direction.pos.x), int(anchor.pos.y + direction.pos.y), 10, 10, 0, 0xff6666ff, kFillModeSolid);
		Novice::DrawLine(int(anchor.pos.x), int(anchor.pos.y), int(anchor.pos.x + direction.pos.x), int(anchor.pos.y + direction.pos.y), 0xff6666ff);

		///                                                            ///
		/// --------------------↑描画処理ここまで-------------------- ///
		///                                                            ///       



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
