#include "stdio.h"
#include "math.h"
#include "SFML/Graphics.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "Vector.h"
#include <time.h>

enum bonusType {
	shrink, enlarge, shield, moustache
};

class GameSystem
{
public:

	//переменные для фона
	struct Background;
	int counterOne;
	int counterTwo;
	int backspeedOne;
	int backspeedTwo;
	int velocity;

	struct Object;
	struct Bonus;
	struct Obstacle;
	struct Result;
	int gameOver = 0;
	int k = 0;
	float obstacleV = -300.0f;
	float bonusV = -300.0f;
	float obstacleFrequency = 4.0f;
	float controlForce = 1.0f;
	bool collision = false;
	//bool gameOn = false;
	int objectSize = 300;
	int explosionSize = 100;
	int obstaclesNumber = 6;
	int bonusNumber = 3;
	float bonusActionTime = 10.0f;
	float playerSize = 40;
	sf::Color playerColor = sf::Color(249, 255, 50, 255);
	int l = 0;
	bool shieldOn = false;
	
	std::string name;
	
	//for debug
	//float dt = 0.002f;

	int frameNumber = 0;
	bool explosion = false;
	sf::Vector2f explosionPos;
	float explosionStartTime;
	float timeAfterExplosion;
	float collisionTime;

	sf::Font font;
	std::ostringstream oss;
	sf::Text txt;
	sf::Text txtBig;

	int screenHeight = 800;
	int screenWidth = 1200;
	float stringHeight = 8.0f;
	float t;

	int level;
	sf::Clock clock;
	//float t0, last_t;

	GameSystem()
	{
		level = 1;
		t = 0;

		font.loadFromFile("cyfral.ttf");
		oss << std::fixed << std::setprecision(0);
		txt.setFont(font);
		txt.setCharacterSize(24);
		txt.setColor(sf::Color(227, 229, 207, 255));
		txt.setPosition(50, 50);
		txt.setStyle(sf::Text::Bold | sf::Text::Underlined);

		txtBig.setFont(font);
		txtBig.setCharacterSize(24);
		txtBig.setColor(sf::Color(227, 229, 207, 255));
		txtBig.setStyle(sf::Text::Regular);

		//загрузка текстур в массив
		sf::Texture tex0;
		if (!tex0.loadFromFile("asteroid1.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex0);
		images.push_back(tex0.copyToImage());

		sf::Texture tex1;
		if (!tex1.loadFromFile("asteroid2.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex1);
		images.push_back(tex1.copyToImage());

		sf::Texture tex2;
		if (!tex2.loadFromFile("asteroid3.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex2);
		images.push_back(tex2.copyToImage());

		sf::Texture tex3;
		if (!tex3.loadFromFile("asteroid4.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex3);
		images.push_back(tex3.copyToImage());

		sf::Texture tex4;
		if (!tex4.loadFromFile("asteroid5.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex4);
		images.push_back(tex4.copyToImage());

		sf::Texture tex5;
		if (!tex5.loadFromFile("asteroid6.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex5);
		images.push_back(tex5.copyToImage());

		sf::Texture tex6;
		if (!tex6.loadFromFile("bonusShrink.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex6);
		images.push_back(tex6.copyToImage());

		sf::Texture tex7;
		if (!tex7.loadFromFile("bonusEnlarge.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex7);
		images.push_back(tex7.copyToImage());

		sf::Texture tex8;
		if (!tex8.loadFromFile("bonusShield.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex8);
		images.push_back(tex8.copyToImage());

		sf::Texture tex9;
		if (!tex9.loadFromFile("empty.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex9);

		sf::Texture tex10;
		if (!tex10.loadFromFile("activeShield.png"))
		{
			printf("texture error");
		}
		objectTextures.push_back(tex10);

		if (!explosionTexture.loadFromFile("explosion.png"))
		{
			printf("texture error");
		}

		if (!stringTexture.loadFromFile("string_new.png"))
		{
			printf("texture error");
		}

		///////////////////////////////////////////////////////////////////////////////////////
		//текстуры фона

		b1.tex = new sf::Texture();
		b1_dup.tex = new sf::Texture();
		if (!(*b1.tex).loadFromFile("back.jpg"))
		{
			printf("texture error");
		}
		if (!(*b1_dup.tex).loadFromFile("back.jpg"))
		{
			printf("texture error");
		}

		b2.tex = new sf::Texture();
		b2_dup.tex = new sf::Texture();
		if (!(*b2.tex).loadFromFile("cosmos.png"))
		{
			printf("texture error");
		}
		if (!(*b2_dup.tex).loadFromFile("cosmos.png"))
		{
			printf("texture error");
		}

		////////////////////////////////////////////////////////////////////////////////////////////////

		float vx = 100.f;
		b1.Set(sf::Vector2f(vx, 0.f), screenWidth, screenHeight);
		b1_dup.Set(sf::Vector2f(vx, 0.f), screenWidth, screenHeight);
		b1.Init();
		vx *= 1.6;
		b2.Set(sf::Vector2f(vx, 0.f), screenWidth, screenHeight);
		b2_dup.Set(sf::Vector2f(vx, 0.f), screenWidth, screenHeight);
		b2.Init();

		this->player = new Player(Vector2f((float)screenWidth / 2, (float)screenHeight / 2));
		this->activeShield = new Player(Vector2f((float)screenWidth / 2, (float)screenHeight / 2));

		SetStandardPlayerSettings();
		player->SetSpritePoints();
		name = "Anonymus";
	}

	void NewGame()
	{
		t = 0;

		this->player = new Player(Vector2f((float)screenWidth / 2, (float)screenHeight / 2));
		SetStandardPlayerSettings();
		player->SetSpritePoints();
		obstacles.clear();
		bonuses.clear();
		activeBonuses.clear();

		obstacleV = -300.0f;
		bonusV = -300.0f;
		obstacleFrequency = 4;

		explosion = false;
		collision = false;
		k = 0; l = 0;
		level = 0;
	};

	void DrawObject(sf::RenderWindow *window, Vector2f pos, int width, int height, int textureNumber)
	{
		sf::Vertex vertices[4];
		vertices[0].position = sf::Vector2f(pos.x, pos.y);
		vertices[1].position = sf::Vector2f(pos.x + width, pos.y);
		vertices[2].position = sf::Vector2f(pos.x + width, pos.y + height);
		vertices[3].position = sf::Vector2f(pos.x, pos.y + height);

		vertices[0].texCoords = sf::Vector2f(0, 0);
		vertices[1].texCoords = sf::Vector2f(objectTextures[textureNumber].getSize().x, 0);
		vertices[2].texCoords = sf::Vector2f(objectTextures[textureNumber].getSize().x, 
			objectTextures[textureNumber].getSize().y);
		vertices[3].texCoords = sf::Vector2f(0, objectTextures[textureNumber].getSize().y);

		window->draw(vertices, 4, sf::Quads, &objectTextures[textureNumber]);
	}

	void DrawShield(sf::RenderWindow *window)
	{
		sf::Vertex vertices[4];
		vertices[0].position = sf::Vector2f(activeShield->pos.x - objectSize / 2, activeShield->pos.y - objectSize / 2);
		vertices[1].position = sf::Vector2f(activeShield->pos.x + objectSize / 2, activeShield->pos.y - objectSize / 2);
		vertices[2].position = sf::Vector2f(activeShield->pos.x + objectSize / 2, activeShield->pos.y + objectSize / 2);
		vertices[3].position = sf::Vector2f(activeShield->pos.x - objectSize / 2, activeShield->pos.y + objectSize / 2);

		vertices[0].texCoords = sf::Vector2f(0, 0);
		vertices[1].texCoords = sf::Vector2f(objectTextures[10].getSize().x, 0);
		vertices[2].texCoords = sf::Vector2f(objectTextures[10].getSize().x, objectTextures[10].getSize().y);
		vertices[3].texCoords = sf::Vector2f(0, objectTextures[10].getSize().y);

		window->draw(vertices, 4, sf::Quads, &objectTextures[10]);
	};

	void DrawString(sf::RenderWindow *window)
	{
		sf::Vertex vertices1[4];
		sf::Vertex vertices2[4];

		Vector2f leftString = Vector2f(player->pos.x, player->pos.y - screenHeight/2);
		Vector2f rightString = Vector2f(screenWidth - player->pos.x, screenHeight/2 - player->pos.y);
		sf::Vector2f A = sf::Vector2f(0, screenHeight/2);
		sf::Vector2f B = sf::Vector2f(player->pos.x, player->pos.y);

		float coeff = stringHeight / 2/ leftString.GetLength();

		vertices1[0].position = sf::Vector2f((-player->pos.y + screenHeight / 2)*coeff,
			player->pos.x*coeff) + A;
		vertices1[1].position = sf::Vector2f((player->pos.y - screenHeight/2)*coeff, -player->pos.x*coeff) + A;
		vertices1[2].position = sf::Vector2f(leftString.x, leftString.y) + vertices1[1].position;
		vertices1[3].position = sf::Vector2f(leftString.x, leftString.y) + vertices1[0]. position;

		vertices1[0].texCoords = sf::Vector2f(0, 0);
		vertices1[1].texCoords = sf::Vector2f(0, stringHeight);
		vertices1[2].texCoords = sf::Vector2f(leftString.GetLength(), stringHeight);
		vertices1[3].texCoords = sf::Vector2f(leftString.GetLength(), 0);
		////////////////////////////////////////////////////////////////////////////////////////
		vertices2[0].position = sf::Vector2f((player->pos.y - screenHeight/2)*coeff, 
			(screenWidth - player->pos.x)*coeff) + B;
		vertices2[1].position = sf::Vector2f(( - player->pos.y + screenHeight / 2)*coeff,
			( -screenWidth + player->pos.x)*coeff) + B;
		vertices2[2].position = sf::Vector2f(rightString.x, rightString.y) + vertices2[1].position;
		vertices2[3].position = sf::Vector2f(rightString.x, rightString.y) + vertices2[0].position;

		vertices2[0].texCoords = sf::Vector2f(0, 0);
		vertices2[1].texCoords = sf::Vector2f(0, stringHeight);
		vertices2[2].texCoords = sf::Vector2f(leftString.GetLength(), stringHeight);
		vertices2[3].texCoords = sf::Vector2f(leftString.GetLength(), 0);
		////////////////////////////////////////////////////////////////////////////////////////
		window->draw(vertices1, 4, sf::Quads, &stringTexture);
		window->draw(vertices2, 4, sf::Quads, &stringTexture);
	};

	//void Draw(sf::RenderWindow *window)
	void Draw(sf::RenderWindow *window, float dt)
	{
		window->clear(sf::Color(28, 29, 25, 255));
		//window->clear(sf::Color(0, 0, 0, 255));

		int x = b1.vert[0].texCoords.x;
		int z = b1.tex->getSize().x;
		if (z - x <= 0) //первая текстура отрисовывается заново, когда происходит ее выход за экран
		{
			b1.Init();
		}

		window->draw(b1.vert, 4, sf::Quads, b1.tex);
		//чтобы избежать серого фона, если текстура уехала за экран, рисуем вторую текстуру
		if (screenWidth + x > z)
		{
			b1_dup.Init2(x);
			window->draw(b1_dup.vert, 4, sf::Quads, b1_dup.tex);
		}

		int x2 = b2.vert[0].texCoords.x;
		int z2 = b2.tex->getSize().x;
		if (z2 - x2 <= 0) //первая текстура отрисовывается заново, когда происходит ее выход за экран
		{
			b2.Init();
		}

		window->draw(b2.vert, 4, sf::Quads, b2.tex);
		//чтобы избежать серого фона, если текстура уехала за экран, рисуем вторую текстуру
		if (screenWidth + x2 > z2)
		{
			b2_dup.Init2(x2);
			window->draw(b2_dup.vert, 4, sf::Quads, b2_dup.tex);
		}

		DrawString(window);

		player->Draw(window);

   		if (collision == false)
		{ 
     			this->Update(dt);
		} 

		for (int i = 0; i < obstacles.size(); i++)
		{
			DrawObject(window, obstacles[i].pos, obstacles[i].width, obstacles[i].height, obstacles[i].textureNumber);
		}

		for (int i = 0; i < bonuses.size(); i++)
		{
			DrawObject(window, bonuses[i].pos, bonuses[i].width, bonuses[i].height, bonuses[i].textureNumber);
		}

		if (shieldOn == true)
		{
			DrawShield(window);
		}

		if (explosion == true)
		{
			if (collision)
				player->color = sf::Color(249 + 20 * sin(0.05*frameNumber), 255 + 20 * sin(0.1*frameNumber), 50+20 * sin(0.15*frameNumber), 255);
			DrawExplosionFrame(window, explosionPos, frameNumber);
			if (timeAfterExplosion - explosionStartTime > 0.05*frameNumber)
			{
				frameNumber++;
			}
			timeAfterExplosion += 0.01f;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(130, 80));
		rectangle.setFillColor(sf::Color(28, 29, 25, 200));
		rectangle.setOutlineColor(sf::Color(180, 238, 239, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setPosition(40, 40);
		window->draw(rectangle);
		oss.str("");
		oss << "SCORE: " << t;
		txt.setString(oss.str());
		txt.setPosition(55, 65);
		txt.setColor(sf::Color(180, 238, 239, 255));
		txt.setStyle(sf::Text::Bold);
		window->draw(txt);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		rectangle.setSize(sf::Vector2f(130, 80));
		rectangle.setFillColor(sf::Color(28, 29, 25, 200));
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color(227, 229, 207, 255));
		rectangle.setPosition(1030, 40);
		window->draw(rectangle);
		oss.str("");
		oss << "LEVEL: " << level;
		txt.setStyle(sf::Text::Regular);
		txt.setString(oss.str());
		txt.setColor(sf::Color(227, 229, 207, 255));
		txt.setPosition(1050, 65);
		window->draw(txt);

		window->display(); 
	};

	void DrawExplosionFrame(sf::RenderWindow *window, sf::Vector2f pos, int frameNumber)
	{
		sf::Vertex vertices[4];
		vertices[0].position = sf::Vector2f(pos.x - explosionSize / 2, pos.y - explosionSize / 2);
		vertices[1].position = sf::Vector2f(pos.x + explosionSize / 2, pos.y - explosionSize / 2);
		vertices[2].position = sf::Vector2f(pos.x + explosionSize / 2, pos.y + explosionSize / 2);
		vertices[3].position = sf::Vector2f(pos.x - explosionSize / 2, pos.y + explosionSize / 2);

		int verticalIndex = frameNumber / 9;
		int horizontalIndex = frameNumber % 9;

		vertices[0].texCoords = sf::Vector2f(horizontalIndex*explosionSize, verticalIndex*explosionSize);
		vertices[1].texCoords = sf::Vector2f((horizontalIndex + 1)*explosionSize, verticalIndex*explosionSize);
		vertices[2].texCoords = sf::Vector2f((horizontalIndex + 1)*explosionSize, (verticalIndex + 1)*explosionSize);
		vertices[3].texCoords = sf::Vector2f(horizontalIndex*explosionSize, (verticalIndex + 1)*explosionSize);

		window->draw(vertices, 4, sf::Quads, &explosionTexture);
	}

	void StartScreen(sf::RenderWindow *window)
	{
		window->clear(sf::Color(28, 29, 25, 255));

		//////////////////////////////////////////////////////////////////////////
		int text_y = 80;
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(700, 120));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color(180, 238, 239, 255));
		rectangle.setPosition(250, text_y);
		window->draw(rectangle);
		oss.str("");
		oss << "STAR FLIGHT";
		txt.setCharacterSize(100);
		txt.setStyle(sf::Text::Regular);
		txt.setColor(sf::Color(180, 238, 239, 255));
		txt.setString(oss.str());
		txt.setPosition(370, text_y);
		window->draw(txt);
		//////////////////////////////////////////////////////////////////////////
		text_y = 270;
		rectangle.setSize(sf::Vector2f(700, 120));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color(227, 229, 207, 255));
		rectangle.setPosition(250, text_y);
		window->draw(rectangle);
		oss.str("");
		oss << " To conrol the star press UP and DOWN arrows.\nYou need to avoid collisions with the asteroids. :-)\n\n     Every 10 seconds your speed increases.";
		txt.setCharacterSize(24);
		txt.setStyle(sf::Text::Regular);
		txt.setColor(sf::Color(227, 229, 207, 255));
		txt.setString(oss.str());
		txt.setPosition(330, text_y+5);
		window->draw(txt);

		//////////////////////////////////////////////////////////////////////////
		text_y = 460;
		rectangle.setSize(sf::Vector2f(700, 30));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color(227, 229, 207, 255));
		rectangle.setPosition(250, text_y);
		window->draw(rectangle);
		oss.str("");
		txt.setColor(sf::Color(227, 229, 207, 255));
		oss << "Enter your name: ";
		window->draw(txt);
		oss << name;
		txt.setStyle(sf::Text::Regular);
		txt.setString(oss.str());
		txt.setPosition(450, text_y);
		window->draw(txt);

		//////////////////////////////////////////////////////////////////////////
		text_y = 560;
		rectangle.setSize(sf::Vector2f(700, 30));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color(180, 238, 239, 255));
		rectangle.setPosition(250, text_y);
		window->draw(rectangle);
		oss.str("");
		oss << "Press SPACE to enter outer space.";
		txt.setStyle(sf::Text::Regular);
		txt.setColor(sf::Color(180, 238, 239, 255));
		txt.setString(oss.str());
		txt.setPosition(410, text_y);
		window->draw(txt);
		/////////////////////////////////////////////////////////////////////////

		window->display();
	}

	void FinalScreen(sf::RenderWindow *window)
	{
		window->clear(sf::Color(28, 29, 25, 255));

		int text_y = 100;
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(700, 120));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineColor(sf::Color(227, 229, 207, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setPosition(250, text_y-15);
		window->draw(rectangle);
		oss.str("");
		oss << "Game OVER!!!!!!";
		txtBig.setCharacterSize(100);
		txtBig.setStyle(sf::Text::Regular);
		txtBig.setString(oss.str());
		txtBig.setPosition(350, text_y-25);
		window->draw(txtBig);
		/////////////////////////////////////////////////////////////////////////////////
		text_y = 200;
		rectangle.setSize(sf::Vector2f(700, 60));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineColor(sf::Color(227, 229, 207, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setPosition(250, text_y);
		window->draw(rectangle);
		oss.str("");
		oss << "SCORE: "<<t;
		txt.setCharacterSize(54);
		txt.setStyle(sf::Text::Regular);
		txt.setString(oss.str());
		txt.setPosition(520, text_y-5);
		window->draw(txt);
		////////////////////////////////////////////////////////////////////////////////////
		text_y = 300;
		rectangle.setSize(sf::Vector2f(700, 30));
		rectangle.setFillColor(sf::Color(28, 29, 25, 255));
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color(180, 238, 239, 255));
		rectangle.setPosition(250, text_y);
		window->draw(rectangle);
		oss.str("");
		oss << "Press Space to travel through stars again!";
		txt.setCharacterSize(24);
		txt.setStyle(sf::Text::Regular);
		txt.setColor(sf::Color(180, 238, 239, 255));
		txt.setString(oss.str());
		txt.setPosition(360, text_y);
		window->draw(txt);
		////////////////////////////////////////////////////////////////////////////////////////////
		std::ifstream best_f("best.txt", std::ios::in);
		Result cur;
		best.clear();
		for (int i = 0; i < 10; i++)
		{
			best_f >> cur.Name >> cur.score;
			best.push_back(cur);
		}
		if (name.length()>0)
			cur.Name = name;
		else
			cur.Name = "Anonymus";
		cur.score = t;
		best.push_back(cur);
		sort(best.begin(), best.end());
		best_f.close();
		std::ofstream best_f_o("best.txt", std::ios::out);
		for (int i = 0; i < 10; i++)
		{
			best_f_o << best[i].Name << " " << best[i].score << "\n";
		}
		best_f_o.close();
			///////////////////////
		text_y = 380;
		rectangle.setSize(sf::Vector2f(500, 33));
		rectangle.setPosition(350, text_y-20);
		rectangle.setOutlineColor(sf::Color(227, 229, 207, 255));
		window->draw(rectangle);
		oss.str("");
		oss << "Best Results";
		txt.setCharacterSize(34);
		txt.setString(oss.str());
		txt.setColor(sf::Color(227, 229, 207, 255));
		txt.setPosition(530, text_y-20);
		txt.setCharacterSize(24);
		window->draw(txt);
		for (int i = 0; i < 10; i++)
		{
			text_y += 33;
			rectangle.setPosition(350, text_y);
			window->draw(rectangle);
			oss.str("");
			oss << best[i].Name<<" "<<best[i].score;
			txt.setString(oss.str());
			txt.setPosition(460, text_y);
			window->draw(txt);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////

		window->display();
	}

	void Update(float dt)
    {
		//усложнение
		obstacleV *= 1.00002;
		bonusV *= 1.00002;
		obstacleFrequency *= 0.99998;
		if (t > 10 * level)
		{
			level++;
		}

		b1.Update(dt);
		b2.Update(dt);

		player->Update(dt);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			player->AddForce(controlForce);
		};

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			player->AddForce(-controlForce);
		};

		player->AddForce(-(player->pos.y - screenHeight / 2) * player->stiffness);
		player->AddForce(-(player->velocity) * player->viscosity);

		UpdateObjects(dt);
		UpdateActiveBonuses(dt);
		CheckCollisions();

		t += dt;

		//создание препятствий и бонусов
		if (t > obstacleFrequency*k)
		{
 			obstacles.push_back(CreateObstacle());
			DeleteGoneObjects();

			srand(time(0));
			int bonusPosY = rand() % (screenHeight - 300);
			if ((obstacles[obstacles.size() - 1].IsPointInside(sf::Vector2f(screenWidth + objectSize / 2, bonusPosY + 110)) == false) &&
				(obstacles[obstacles.size() - 1].IsPointInside(sf::Vector2f(screenWidth + objectSize / 2, bonusPosY + 150)) == false) &&
				(obstacles[obstacles.size() - 1].IsPointInside(sf::Vector2f(screenWidth + objectSize / 2, bonusPosY + 190)) == false))
			{
				bonuses.push_back(CreateBonus(Vector2f(screenWidth, bonusPosY)));
			}
			k++;
		}
		
		if (t > obstacleFrequency*l+obstacleFrequency/2)
		{
			srand(time(0));
			int randResult = rand() % (screenHeight - 300);
			if ((rand() % 4) == 0)
			bonuses.push_back(CreateBonus(Vector2f(screenWidth, rand() % (screenHeight - 300))));
			l++;
		}

		if (explosion)
			explosionPos.x += obstacleV*dt;

		//увеличение сложности
		//obstacleV -= 0.03f;
		//bonusV -= 0.03f;
	};

	Obstacle CreateObstacle()
	{
		Obstacle obstacle;
		srand(time(0));
		obstacle.textureNumber = rand() % 6;
		obstacle.texture = objectTextures[obstacle.textureNumber];
		obstacle.velocity = obstacleV;
		obstacle.width = objectSize;
		obstacle.height = objectSize;
		obstacle.pos = Vector2f(screenWidth, screenHeight / 2 - rand()%objectSize);
		obstacle.image = images[obstacle.textureNumber];

		return obstacle;
	};

	Bonus CreateBonus(Vector2f pos)
	{
		Bonus bonus;
		bonus.textureNumber = obstaclesNumber + rand() % (bonusNumber);
		bonus.texture = objectTextures[bonus.textureNumber];
		bonus.velocity = bonusV;
		bonus.width = objectSize;
		bonus.height = objectSize;
		bonus.pos = pos;
		bonus.image = images[bonus.textureNumber];
		switch (bonus.textureNumber)
		{
		case 6:
		{
			bonus.type = shrink;
			break;
		}
		case 7:
		{
			bonus.type = enlarge;
			break;
		}
		case 8:
		{
			bonus.type = shield;
			break;
		}
		};

		return bonus;
	};

	void DeleteGoneObjects()
	{
		int i;
		if (obstacles.size() > 1)
		{
			for (i = 0; i < obstacles.size(); i++)
			{
				if (IsActive(obstacles[i]) == false)
				{
					obstacles.erase(obstacles.begin() + i);
					i--;
				}
			};
		}

		if (bonuses.size()>1)
		{
			for (i = 0; i < bonuses.size(); i++)
			{
				if (IsActive(bonuses[i]) == false)
				{
					bonuses.erase(bonuses.begin() + i);
					i--;
				}
			}
		}
	};

	void UpdateObjects(float dt)
	{
		int i;
		for (i = 0; i < obstacles.size(); i++)
		{
			obstacles[i].velocity = obstacleV;
			obstacles[i].pos.x += obstacles[i].velocity*dt;
		};

		for (i = 0; i < bonuses.size(); i++)
		{
			bonuses[i].velocity = bonusV;
			bonuses[i].pos.x += bonuses[i].velocity*dt;
		};

		UndoOverdueBonuses();
	};

	void UpdateActiveBonuses(float dt)
	{
		if (shieldOn == true)
		{
			activeShield->pos = Vector2f(player->pos.x, player->pos.y);
			activeShield->velocity = player->velocity;
			activeShield->acceleration = player->acceleration;
		}
	};

	std::vector <sf::Texture> objectTextures;

	bool IsActive(Object object)
	{
		if (object.pos.x + object.width < 0)
		return false;
		else return true;
	}

	struct Object
	{
		sf::Texture texture;
		int textureNumber;
		float velocity;
		int width;
		int height;
		Vector2f pos;
		int size = 300;
		sf::Image image;

		bool IsPointInside(sf::Vector2f pointToCheck)
		{
			if ((pointToCheck.x >= this->pos.x) &&
				(pointToCheck.x <= this->pos.x + this->size) &&
				(pointToCheck.y >= this->pos.y) &&
				(pointToCheck.y <= this->pos.y + this->size))
			{
				int a = this->image.getPixel((pointToCheck.x - this->pos.x),
					(pointToCheck.y - this->pos.y)).a;

				if (a > 200)
					return true;
			};
			return false;
		}
	};

	void CheckCollisions()
	{
		for (size_t obstacleIndex = 0; obstacleIndex < obstacles.size(); obstacleIndex++)
		{
			for (size_t pointIndex = 0; pointIndex < player->GetPointsCount(); pointIndex++)
			{
				if (obstacles[obstacleIndex].IsPointInside(player->GetPoint(pointIndex)))
				{
					explosion = true;
					explosionPos = player->GetPoint(pointIndex);
					explosionStartTime = t;
					timeAfterExplosion = t;
					frameNumber = 0;

					if (shieldOn)
					{
						shieldOn = false;
						obstacles.erase(obstacles.begin() + obstacleIndex);
						if (obstacleIndex == 0)
							break;
						else obstacleIndex--;
					}
					else
					{
						collision = true;
						collisionTime = clock.getElapsedTime().asSeconds();
						break;
					}
				}
			}
		}
		for (size_t bonusIndex = 0; bonusIndex < bonuses.size(); bonusIndex++)
			for (size_t pointIndex = 0; pointIndex < player->GetPointsCount(); pointIndex++)
			{
				if (bonuses[bonusIndex].IsPointInside(player->GetPoint(pointIndex)))
				{
					bonuses[bonusIndex].activationTime = t;

					switch (bonuses[bonusIndex].type)
					{
					case shrink:
					{
						player->color = sf::Color(62, 209, 62, 255);
						player->r1 = 30;
						break;
					}
					case enlarge:
					{
						player->color = sf::Color(204, 77, 59, 255);
						player->r1 = 50;
						break;
					}
					case shield:
					{
						shieldOn = true;
						this->activeShield = new Player(Vector2f(player->pos.x, player->pos.y));
						break;
					}
					};

					activeBonuses.push_back(bonuses[bonusIndex]);
					bonuses.erase(bonuses.begin() + bonusIndex);
					break;
				}
			}
	};

	void UndoOverdueBonuses()
	{
		for (int i = 0; i < activeBonuses.size(); i++)
		{
			if (t - activeBonuses[i].activationTime > bonusActionTime)
			{
				activeBonuses.erase(activeBonuses.begin() + i);
				i--;
			}
		}

		bool shrinkFlag = false; bool enlargeFlag = false; bool shieldFlag = false;

		for (int i = 0; i < activeBonuses.size(); i++)
		{
			if (activeBonuses[i].type == shrink)
				shrinkFlag = true;
			if (activeBonuses[i].type == enlarge)
				enlargeFlag = true;
			if (activeBonuses[i].type == shield)
				shieldFlag = true;
		}

		if ((shrinkFlag == false) && (enlargeFlag == false)) UndoBonusEffect(shrink);
		if (shieldFlag == false) UndoBonusEffect(shield);
	};

	void UndoBonusEffect(bonusType type)
	{
		switch (type)
		{
		case shrink:
		{
			SetStandardPlayerSettings();
			break;
		}
		case enlarge:
		{
			SetStandardPlayerSettings();
			break;
		}
		case shield:
		{
			shieldOn = false;
			break;
		}
		case moustache:
		{
			//усы
			break;
		}
		}
	};

	std::vector <Bonus> bonuses;

	struct Obstacle : public Object
	{

	};

	struct Bonus : public Object
	{
		bonusType type;
		float activationTime;
	};

	void SetStandardPlayerSettings()
	{
		player->color = playerColor;
		player->r1 = playerSize;
	};

	struct Player
	{
		
		Player(Vector2f pos)
		{
			this->pos = pos;
			this->velocity = 0.0f;
			this->acceleration = 0.0f;
		}

		void SetSpritePoints()
		{
			float Pi = 3.1415f;
			float angle = 2 * Pi / 10;
			float r2 = r1 / 3;

			spritePoints.clear();

			spritePoints.push_back(sf::Vector2f(this->pos.x, this->pos.y - r1));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r2*cos(Pi / 2 + 1 * angle),
				this->pos.y - r2*sin(Pi / 2 + 1 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r1*cos(Pi / 2 + 2 * angle),
				this->pos.y - r1*sin(Pi / 2 + 2 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r2*cos(Pi / 2 + 3 * angle),
				this->pos.y - r2*sin(Pi / 2 + 3 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r1*cos(Pi / 2 + 4 * angle),
				this->pos.y - r1*sin(Pi / 2 + 4 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r2*cos(Pi / 2 + 5 * angle),
				this->pos.y - r2*sin(Pi / 2 + 5 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r1*cos(Pi / 2 + 6 * angle),
				this->pos.y - r1*sin(Pi / 2 + 6 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r2*cos(Pi / 2 + 7 * angle),
				this->pos.y - r2*sin(Pi / 2 + 7 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r1*cos(Pi / 2 + 8 * angle),
				this->pos.y - r1*sin(Pi / 2 + 8 * angle)));
			spritePoints.push_back(sf::Vector2f(this->pos.x + r2*cos(Pi / 2 + 9 * angle),
				this->pos.y - r2*sin(Pi / 2 + 9 * angle)));
		};

		sf::ConvexShape convex;

		Vector2f pos;
		float velocity;
		float acceleration;

		int GetPointsCount()
		{
			return spritePoints.size();
		};

		sf::Vector2f GetPoint(int pointIndex)
		{
			return spritePoints[pointIndex];
		};

		void Draw(sf::RenderWindow *window)
		{
			convex.setPointCount(spritePoints.size());
			
			for (int i = 0; i < 10; i++)
			convex.setPoint(i, spritePoints[i]);
			convex.setFillColor(this->color);

			window->draw(convex);
		};

		void Update(float dt)
		{
			this->velocity += this->acceleration * dt;
			this->pos.y += this->velocity * dt;

			this->acceleration = 0.0f;

			SetSpritePoints();
		};
		void SetColor(sf::Color col)
		{
			this->color = col;
		}

		void AddForce(float force)
		{
			this->acceleration += force / m;
		};
		
		std::vector<sf::Vector2f> spritePoints;

		float stiffness = 0.008f;
		float viscosity = 0.0009f;
		float m = 0.0005f;
		float r1;
		sf::Color color;
	};

	struct Background
	{
		int W, H;
		void Set(sf::Vector2f Vel, int w, int h)
		{
			velocity = Vel;
			W = w;
			H = h;
		}
		void Init()
		{
			vert[0].position = sf::Vector2f(0, 0);
			vert[1].position = sf::Vector2f(W, 0);
			vert[2].position = sf::Vector2f(W, H);
			vert[3].position = sf::Vector2f(0, H);

			vert[0].texCoords = sf::Vector2f(0, 0);
			vert[1].texCoords = sf::Vector2f(W, 0);
			vert[2].texCoords = sf::Vector2f(W, 800);
			vert[3].texCoords = sf::Vector2f(0, 800);
		}
		void Init2(int x)
		{
			vert[0].position = sf::Vector2f((*tex).getSize().x - 2 - x, 0);
			vert[1].position = sf::Vector2f(W, 0);
			vert[2].position = sf::Vector2f(W, H);
			vert[3].position = sf::Vector2f((*tex).getSize().x - 2 - x, H);

			vert[0].texCoords = sf::Vector2f(0, 0);
			vert[1].texCoords = sf::Vector2f(W - (*tex).getSize().x + x, 0);
			vert[2].texCoords = sf::Vector2f(W - (*tex).getSize().x + x, H);
			vert[3].texCoords = sf::Vector2f(0, H);
		}

		sf::Texture *tex;
		sf::Vector2f velocity;

		void Update(float dt)
		{
			for (int i = 0; i < 4; i++)
				vert[i].texCoords += velocity * dt;

		}
		/*void Update2(float dt)
		{
			vert[0].position -= velocity*dt;
			vert[3].position -= velocity*dt;

			vert[1].texCoords += velocity*dt;
			vert[2].texCoords += velocity*dt;
		}*/
		sf::Vertex vert[4];
	};

	struct Result
	{
		std::string Name;
		int score;
		bool operator<(Result r2)
		{
			if (score > r2.score)
				return true;
			else
				return false;
		}
	};

	Background b1, b1_dup, b2, b2_dup;

	Player *player;
	Player *activeShield;
	std::vector<Result> best;
	std::vector <sf::Image> images;
	std::vector <Bonus> activeBonuses;
	std::vector <Obstacle> obstacles;

	sf::ConvexShape convex;

	sf::Texture explosionTexture;
	sf::Texture stringTexture;
};