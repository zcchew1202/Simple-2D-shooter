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

};