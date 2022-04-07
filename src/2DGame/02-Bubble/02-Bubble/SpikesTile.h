#include "Tile.h"

class  SpikesTile : public Tile {
	public:
		enum Orientation {
			UP,
			RIGHT,
			DOWN,
			LEFT
		};
		SpikesTile(int id, glm::ivec2 pos, int size, Orientation orientation);

		bool isDeathZone(glm::ivec2 playerPos, glm::ivec2 playerSize) const;
	private:
		Orientation orientation;
};

