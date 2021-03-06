#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}

//Crear tilemap vol dir crear de disk i construir el vao (prepareArrays) o el k es el mateix, emmagatzemar a la grafica
TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}

//pintem triangulitos -> MAI HA D'ACTUALITZAR RES, NOMES REPRESENTAR L'ESTAT ACTUAL DEL JOC- > va al update 
void TileMap::render() const
{
	//fas directament el drawArrays ja que abans del mapa no hi ha res
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

//llegir de disc a partir d'un protocol k ens ve donat ( busca un tilemap, espera dos valors per ser el tamany del tilemap.......)
bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			/*fin.get(tile);
			//si es un espai posem un 0
			if(tile == 0)
				map[j*mapSize.x+i] = 0;
			else
			//implementa la funcio ASCII to integer (que un string numero passi a ser un numero de veritat)
				map[j*mapSize.x+i] = tile - int('0');*/

			//Per poder posar m?s de 10 blocs al mapa (0-9) convertim el tile en int i aixi tenim suficients numeros pels blocs que tenim
			fin >> tile;
			map[j * mapSize.x + i] = tile;
				
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

//moltes dades i al final es pasen a la nostra grafica amb el vao, vbo...
void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	//el mateix k OpenGl pero en ves de un quadradet, nhi han molts
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

glm::ivec2 TileMap::getMapSize() {
	return mapSize;
}
bool TileMap::isMapLimitRight(const glm::ivec2& pos) {
	int x = pos.x / tileSize;
	if (x + 2 >= mapSize.x)return true;
	return false;
}

bool TileMap::isMapLimitTop(const glm::ivec2& pos) {
	int y = pos.y / tileSize;
	if (y - 1 <= 0)return true;
	return false;
}
bool TileMap::isMapLimitDown(const glm::ivec2& pos) {
	int y = pos.y / tileSize;
	if (y + 1 >= mapSize.y)return true;
	return false;
}

bool TileMap::isMapLimitLeft(const glm::ivec2& pos) {
	int x = pos.x / tileSize;
	if (x - 1 <= 0) return true;
	return false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	//es una cerca, mirem que desde una posi fins una altra, veiem si no hi ha llocs per colisionar
	for(int y=y0; y<=y1; y++)
	{
		//Si no hi ha res o hi ha una liana
		if(map[y*mapSize.x+x] != 0 && map[y * mapSize.x + x] != 20 && map[y * mapSize.x + x] != 21)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	//Si no hi ha res o hi ha una liana
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0 && map[y * mapSize.x + x] != 20 && map[y * mapSize.x + x] != 21)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	//una cerca desde un punt a un altre, si trobem un bloc, no caiguis mes
	for(int x=x0; x<=x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{	
			//Si no es una liana
			if (map[y * mapSize.x + x] != 20 && map[y * mapSize.x + x] != 21) {
				if (*posY - tileSize * y + size.y <= 4)
				{
					*posY = tileSize * y - size.y - 3;
					return true;
				}
			}
		}
	}
	
	return false;
}

bool TileMap::collisionGota(const glm::ivec2& pos,int* posY) 
{
	int x0, x1, y;
	glm::ivec2 size = glm::ivec2(18, 18);
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;

	//una cerca desde un punt a un altre, si trobem un bloc, no caiguis mes
	for (int x = x0; x <= x1; x++)
	{
		if (map[pos.y * mapSize.x + x] != 0){
			if (*posY - tileSize * y + size.y <= 6)
			{
				return true;
			}
		}
	}

	return false;
}

bool TileMap::hayLianaUp(const glm::ivec2& pos,  const glm::ivec2& size) {
	int x0, x1, y;
	x0= pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;

	for (int x = x0; x <= x1; x++) {
		if (map[y * mapSize.x + x] == 20 || map[y * mapSize.x + x] == 21) {
			return true;
		}
	}
	return false;
}
bool TileMap::hayLianaDown(const glm::ivec2& pos, const glm::ivec2& size) {
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	//una cerca desde un punt a un altre, si trobem un bloc, no caiguis mes
	for (int x = x0; x <= x1; x++)
	{		
		//Si es una liana
		if (map[y * mapSize.x + x] == 20 || map[y * mapSize.x + x] == 21) {
			
			return true;
			
		}
		
	}

	return false;
}
bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1 -16) / tileSize;
	//una cerca desde un punt a un altre, si trobem un bloc, no caiguis mes
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 20 && map[y * mapSize.x + x] != 21)
		{
			/*if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;*/
				return true;
			//}
		}
	}

	return false;
}





























