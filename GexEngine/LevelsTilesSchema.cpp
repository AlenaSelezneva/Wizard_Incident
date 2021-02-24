#include "LevelsTilesSchema.h"

std::vector<std::vector<Tile::Type>> LevelsTilesSchema::getLevelLayout()
{
    int n = 40;
    int m = 35;

    std::vector<std::vector<Tile::Type>> level(n, std::vector<Tile::Type>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i == 0 || j == 0 || i == n - 1 || j == m - 1)
                level[i][j] = Tile::Type::Wall;
            else
                level[i][j] = Tile::Type::Floor;
        }
    }

    level[10][10] = Tile::Type::Wall;
    level[37][33] = Tile::Type::Wall;
    level[20][25] = Tile::Type::Wall;
    level[35][6] = Tile::Type::Wall;

    return level;
}