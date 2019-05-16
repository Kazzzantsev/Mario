#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
float offsetX = 0, offsetY = 0;
int ground = 150;
const int H = 17;
const int W = 150;


String TileMap[H] = {
	"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	"0                                                                                                                                                    0",
	"0                                                                                                                                                    0",
	"0                   w0                                 w                                                                                             0",
	"0                                      w                             w                                                     w                         0",
	"0                                                                                                                                                    0",
	"0                      c                                                                       w                                                     0",
	"0                                                                       r                                     Y0000                   w              0",
	"0                                                                      rr                                      0000                                  0",
	"0                                                                     rrr                                      0                                     0",
	"0               c    kckck                                           rrrr                                     00   00                                0",
	"0                                      t0                           rrrrr                                     000  00                                0",
	"0G                                     00              t0          rrrrrr            G                        000  00                                0",
	"0           d    g       d             00              00         rrrrrrr0       g       d      d        g         00                 d     g        0",
	"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
	"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
	"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
};

class PLAYER { /* создаем класс PLAYER( нам нужно чтобы движения персонажа осуществлялись под действием  гравитации) */

public:
	float dx, dy; // скорость
	FloatRect rect; //  здесь rect(x, y, widht, height)
	bool onGround; /*  onGround- переменная, которая показывает, находится ли персонаж на земле */
	Sprite sprite; // сюда будем загружать картинку
	float currentFrame; //  текущий кадр для анимации

	PLAYER(Texture &image)
	{
		sprite.setTexture(image); // в конструктор класса загружаем картинку
		rect = FloatRect(2 * 16, 9 * 16, 20, 15); /* указываем первоначальные координаты x=0, y=0, ширина-40, высота-50 */

		dx = dy = 0;
		currentFrame = 0;
	}
	void update(float time)
	{

		rect.left += dx * time;
		Collision(0);

		if (!onGround) dy = dy + 0.0005*time;
		rect.top += dy*time;
		onGround = false;
		Collision(1);


		currentFrame += 0.005*time;
		if (currentFrame > 5) currentFrame -= 5;

		if (dx>0) sprite.setTextureRect(IntRect(79 + 31 * int(currentFrame), 144, 16, 16));
		if (dx<0) sprite.setTextureRect(IntRect(79 + 31 * int(currentFrame) + 16, 144, -16, 16));
		
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}



	void Collision(int num)
	{

		for (int i = rect.top / 16; i<(rect.top + rect.height) / 16; i++)
		for (int j = rect.left / 16; j<(rect.left + rect.width) / 16; j++)
		{
			if ((TileMap[i][j] == 'P') || (TileMap[i][j] == 'k') || (TileMap[i][j] == '0') || (TileMap[i][j] == 'r') || (TileMap[i][j] == 't'))
			{
				if (dy>0 && num == 1)
				{
					rect.top = i * 16 - rect.height;  dy = 0;   onGround = true;
				}
				if (dy<0 && num == 1)
				{
					rect.top = i * 16 + 16;   dy = 0;
				}
				if (dx>0 && num == 0)
				{
					rect.left = j * 16 - rect.width;
				}
				if (dx<0 && num == 0)
				{
					rect.left = j * 16 + 16;
				}
			}

			if (TileMap[i][j] == 'c') {
				 TileMap[i][j]='0'; 
			}
		}

	}
};
class ENEMY
{

public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool life;


	void set(Texture &image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x, y, 16, 16);

		dx = 0.05;
		currentFrame = 0;
		life = true;
	}

	void update(float time)
	{
		rect.left += dx * time;

		Collision();


		currentFrame += time * 0.005;
		if (currentFrame > 2) currentFrame -= 2;

		sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 16, 16));
		if (!life) sprite.setTextureRect(IntRect(58, 0, 16, 16));


		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

	}


	void Collision()
	{

		for (int i = rect.top / 16; i<(rect.top + rect.height) / 16; i++)
		for (int j = rect.left / 16; j<(rect.left + rect.width) / 16; j++)
		if ((TileMap[i][j] == 'P') || (TileMap[i][j] == '0'))
		{
			if (dx>0)
			{
				rect.left = j * 16 - rect.width; dx *= -1;
			}
			else if (dx<0)
			{
				rect.left = j * 16 + 16;  dx *= -1;
			}

		}
	}

};

int main()
{
	RenderWindow window(VideoMode(512, 384), "It`s me! Mario!");
	
		Texture t;
	t.loadFromFile("g.png"); // загружаем картинку
	float currentFrame = 0;

	PLAYER p(t); // загружаем текстуру
	Sprite tile(t);
	Clock clock; /* засекаем время с последнего тика( чтобы привязка персонажа была ко времени, а не к мощности процессора) */
	RectangleShape rectangle(Vector2f(16, 16));
	ENEMY  enemy;
	ENEMY  enemy2;
	ENEMY  enemy3;
	enemy.set(t, 48 * 16, 13 * 16);
	enemy2.set(t, 96 * 16, 13 * 16);
	enemy3.set(t, 48 * 16 * 3, 13 * 16);
	SoundBuffer buffer;
	buffer.loadFromFile("Jump.ogg");
	Sound sound(buffer);

	Music music;
	music.openFromFile("Mario_theme.ogg");
	music.play();

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); /* создаем переменную время, getElapsedTime() - дать прошедшее время( возьмем  его в микросекундах) */

		clock.restart(); // перезагружает часы
		time = time / 800; //здесь происходит регулировка скорости движения персонажа
		
		Event event;
		while (window.pollEvent(event))
		{

			if (event.type == Event::Closed)
				window.close(); // обрабатываем событие закрытия окна
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) // если клавиша нажата и клавиша налево
		{
		
			p.dx = -0.1; // при нажатии налево- ускоряемся на -0.1
			
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) // если клавиша нажата и клавиша направо
		{
			
			p.dx = 0.1; // при нажатии направо- ускоряемся на 0.1
			

		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) // вверх
		{
			if (p.onGround) { p.dy = -0.3; p.onGround = false; sound.play(); } // если мы на земле, то только тогда можем осуществить прыжок
		}
	
		p.update(time); // загружаем время
		enemy.update(time);
		enemy2.update(time);
		enemy3.update(time);
		window.clear(sf::Color(0, 80, 150)); // очищаем экран
		if (p.rect.left>300) offsetX = p.rect.left - 300;
		offsetY = p.rect.top - 300;

		
		if (p.rect.intersects(enemy.rect))
		{
			if (enemy.life) {
				if (p.dy>0) { enemy.dx = 0; p.dy = -0.2; enemy.life = false; }
				else p.sprite.setColor(Color::Black);
			}
		}
		if (p.rect.intersects(enemy2.rect))
		{
			if (enemy2.life) {
				if (p.dy>0) { enemy2.dx = 0; p.dy = -0.2; enemy2.life = false; }
				else p.sprite.setColor(Color::Black);
			}
		}
		if (p.rect.intersects(enemy3.rect))
		{
			if (enemy3.life) {
				if (p.dy>0) { enemy3.dx = 0; p.dy = -0.2; enemy3.life = false; }
				else p.sprite.setColor(Color::Black);
			} 
		}

		for (int i = 0; i<H; i++)
		for (int j = 0; j<W; j++)
		{
			if (TileMap[i][j] == 'P')  tile.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));

			if (TileMap[i][j] == 'k')  tile.setTextureRect(IntRect(143, 112, 16, 16));

			if (TileMap[i][j] == 'c')  tile.setTextureRect(IntRect(143 - 16, 112, 16, 16));

			if (TileMap[i][j] == 't')  tile.setTextureRect(IntRect(0, 47, 32, 95 - 47));

			if (TileMap[i][j] == 'g')  tile.setTextureRect(IntRect(0, 16 * 9 - 5, 3 * 16, 16 * 2 + 5));

			if (TileMap[i][j] == 'G')  tile.setTextureRect(IntRect(145, 222, 222 - 145, 255 - 222));

			if (TileMap[i][j] == 'd')  tile.setTextureRect(IntRect(0, 106, 74, 127 - 106));

			if (TileMap[i][j] == 'w')  tile.setTextureRect(IntRect(99, 224, 140 - 99, 255 - 224));

			if (TileMap[i][j] == 'r')  tile.setTextureRect(IntRect(143 - 32, 112, 16, 16));
			if (TileMap[i][j] == 'Y')  tile.setTextureRect(IntRect(96, 0, 112,112 ));

			if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0')) continue;

			tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
			window.draw(tile);
		}
		window.draw(p.sprite);
		window.draw(enemy.sprite);
		window.draw(enemy2.sprite);
		window.draw(enemy3.sprite);
		window.display();
	}

	return 0;
}