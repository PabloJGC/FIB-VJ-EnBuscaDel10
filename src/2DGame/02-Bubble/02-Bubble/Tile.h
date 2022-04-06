#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

class Tile {

public:
	Tile(int id);
	bool isTransparent() const;
	int getId() const;

private:
	int id;
	bool transparent;

};

#endif // _TILE_INCLUDE