#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "objects.h"

//GLOBAL variables
enum KEYS{UP,DOWN,LEFT,RIGHT,SPACE};
bool keys[5] = { false, false, false, false ,false};

const int WIDTH = 800;
const int NUM_BULLETS = 5;
const int HEIGHT = 400;
const int NUM_COMETS = 10;

//FUNCTION prototypes

void InitShip(SpaceShip &ship);
void DrawShip(SpaceShip &ship);

void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

void InitBullets(bullet Bullets[], int size);
void DrawBullets(bullet Bullets[], int size);
void FireBullets(bullet Bullets[], int size, SpaceShip &ship);
void UpdateBullets(bullet Bullets[], int size);
void CollideBullets(bullet Bullets[], int bsize, comet Comet[], int csize, SpaceShip &ship);

void InitComets(comet Comet[], int size);
void DrawComets(comet Comet[], int size);
void StartComets(comet Comet[], int size);
void UpdateComets(comet Comet[], int size);
void CollideComets(comet Comet[], int csize, SpaceShip &ship);

int main(void)
{
	//PRIMITIVE variables
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool isGameOver = false;

	//OBJECT variables
	SpaceShip ship;
	bullet Bullets[NUM_BULLETS];
	comet Comet[NUM_COMETS];

	//ALLEGRO variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"failed to initialize allegro!", NULL, NULL);
		return -1;
	}

	display = al_create_display(WIDTH, HEIGHT);

	//initializing functions
	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"failed to initialize display!", NULL, NULL);
		return -1;
	}

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/FPS);

	srand(time(NULL));

	InitShip(ship);
	InitBullets(Bullets, NUM_BULLETS);
	InitComets(Comet, NUM_COMETS);

	font18 = al_load_font("arial.ttf", 18, 0);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (keys[UP] == true)
				MoveShipUp(ship);

			if (keys[DOWN] == true)
				MoveShipDown(ship);

			if (keys[LEFT] == true)
				MoveShipLeft(ship);

			if (keys[RIGHT] == true)
				MoveShipRight(ship);

			if (!isGameOver)
			{
				UpdateBullets(Bullets, NUM_BULLETS);

				StartComets(Comet, NUM_COMETS);
				UpdateComets(Comet, NUM_COMETS);

				CollideBullets(Bullets, NUM_BULLETS, Comet, NUM_COMETS, ship);
				CollideComets(Comet, NUM_COMETS, ship);

				if (ship.lives <= 0)
				{
					isGameOver = true;
				}

			}

			

		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done=true;
				break;

			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;

			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				FireBullets(Bullets, NUM_BULLETS, ship);
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;

			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;

			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		
		//ensures objects are only drawn at the beginning, hence screen won't flicker every 1/FPS sec
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if (!isGameOver)
			{
				DrawShip(ship);
				DrawBullets(Bullets, NUM_BULLETS);
				DrawComets(Comet, NUM_COMETS);

				al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "Player has destroyed %i enemies, Player has %i lives left", ship.score, ship.lives);
			}

			else
			{
				al_draw_textf(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Game Over! Your final score is : %i", ship.score);
			}
		
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	
		

	}


	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

	return 0;
}

void InitShip(SpaceShip &ship)
{
	ship.x = 20;
	ship.y = HEIGHT / 2;
	ship.ID = PLAYER;
	ship.lives = 3;
	ship.speed = 7;
	ship.boundx = 6;
	ship.boundy = 7;
	ship.score = 0;

}

void DrawShip(SpaceShip &ship)
{
	al_draw_filled_rectangle(ship.x, ship.y - 9, ship.x + 10, ship.y - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(ship.x, ship.y + 9, ship.x + 10, ship.y + 7, al_map_rgb(255, 0, 0));

	al_draw_filled_triangle(ship.x - 12, ship.y - 17, ship.x + 12, ship.y, ship.x - 12, ship.y + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(ship.x-12,ship.y-2,ship.x+15,ship.y+2,al_map_rgb(0,0,255));
}

void MoveShipUp(SpaceShip &ship)
{
	ship.y -= ship.speed;
	if (ship.y < 0)
	{
		ship.y = 0;
	}
}

void MoveShipDown(SpaceShip &ship)
{
	ship.y += ship.speed;
	if (ship.y > HEIGHT)
	{
		ship.y = HEIGHT;
	}
}

void MoveShipLeft(SpaceShip &ship)
{
	ship.x -= ship.speed;
	if (ship.x < 0)
	{
		ship.x = 0;
	}
}

//changing how far ship goes to right is up to me, currently max...
void MoveShipRight(SpaceShip &ship)
{
	ship.x += ship.speed;
	if (ship.x > WIDTH)
	{
		ship.x = WIDTH;
	}
}

void InitBullets(bullet Bullets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		Bullets[i].ID = BULLET;
		Bullets[i].speed = 10;
		Bullets[i].live = false;
	}
}

void DrawBullets(bullet Bullets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		al_draw_filled_circle(Bullets[i].x, Bullets[i].y, 2.0, al_map_rgb(255, 255, 255));
	}
}

void FireBullets(bullet Bullets[], int size, SpaceShip &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (!Bullets[i].live)
		{
			Bullets[i].x = ship.x + 17;
			Bullets[i].y = ship.y;
			Bullets[i].live = true;
			break;
		}
	}
}

void UpdateBullets(bullet Bullets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (Bullets[i].live)
		{
			Bullets[i].x += Bullets[i].speed  ;
			if (Bullets[i].x > WIDTH)
				Bullets[i].live = false;
		}
	}
}

void InitComets(comet Comet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		Comet[i].ID = ENEMY;
		Comet[i].live = false;
		Comet[i].speed = 5;
		Comet[i].boundx = 18;
		Comet[i].boundy = 18;
	}

}

void DrawComets(comet Comet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (Comet[i].live)
			al_draw_filled_circle(Comet[i].x, Comet[i].y, 20, al_map_rgb(255, 0, 0));
	}

}
void StartComets(comet Comet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!Comet[i].live)
		{
			if (rand() % 500 == 0)
			{
				Comet[i].live = true;
				Comet[i].x = WIDTH;
				Comet[i].y = 30 + (rand() % (HEIGHT-60));

				break;
			}

		}
	}
}
void UpdateComets(comet Comet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (Comet[i].live)
		{
			Comet[i].x -= Comet[i].speed;

			if (Comet[i].x < 0)
				Comet[i].live = false;
		}
	}
}

void CollideBullets(bullet Bullets[], int bsize, comet Comet[], int csize, SpaceShip &ship)
{
	for (int i = 0; i < bsize; i++)
	{
		if (Bullets[i].live)
		{
			for (int j = 0; j < csize; j++)
			{
				if (Comet[j].live)
				{
					if (
						Bullets[i].x >= (Comet[j].x - Comet[j].boundx) &&
						Bullets[i].x <= (Comet[j].x + Comet[j].boundx) &&
						Bullets[i].y >= (Comet[j].y - Comet[j].boundy) &&
						Bullets[i].y <= (Comet[j].y + Comet[j].boundy)
						)

					{
						Bullets[i].live = false;
						Comet[j].live = false;

						ship.score++;
					}
				}
				

			}
		}
	}
}

void CollideComets(comet Comet[], int csize, SpaceShip &ship)
{
	for (int i = 0; i < csize; i++)
	{
		if (Comet[i].live)
		{
			if (Comet[i].x - Comet[i].boundx < ship.x + ship.boundx &&
				Comet[i].x + Comet[i].boundx > ship.x - ship.boundx &&
				Comet[i].y - Comet[i].boundy < ship.y + ship.boundy &&
				Comet[i].y + Comet[i].boundy > ship.y - ship.boundy
				)

			{
				ship.lives--;
				Comet[i].live = false;
			}

			else if (Comet[i].x < 0)

			{
				Comet[i].live = false;
				ship.lives--;

			}


		}
	}
}
