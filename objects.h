//OBJECT IDs

enum IDS {PLAYER,BULLET,ENEMY};

//PLAYER

struct SpaceShip
{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	int boundx;
	int boundy;
	int score;

	int maxframe;
	int curframe;
	int frameDelay;
	int frameCount;
	int frameWidth;
	int frameHeight;
	int AniDir;
	int AniCol;
	int AniRow;

	ALLEGRO_BITMAP *image;


};

struct bullet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;

};

struct comet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;

	int maxframe;
	int curframe;
	int frameDelay;
	int frameCount;
	int frameWidth;
	int frameHeight;
	int AniDir;
	int AniCol;
	int AniRow;

	ALLEGRO_BITMAP *image;

};

struct explosion
{

};