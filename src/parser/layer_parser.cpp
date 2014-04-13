#include "parser.hpp"

using namespace parser;

TMXObject parse_layer(const TMXMap& map, const QString& source, int z, const std::vector<int>& data) {
  int width = map.at("width");
  int height = map.at("height");
  int tilewidth = map.at("tilewidth");
  int tileheight = map.at("tileheight");
  for (int i = 0; i < height * width; i++)
    if (data[i] != 0)
      return std::make_tuple((i % height) * tilewidth, (i / height) * tileheight, z, source);
  return TMXObject();
}

std::vector<TMXObject> LayerParser::parse(const TMX& tmx) {
  std::vector<TMXObject> objects;
  TMXTileset tileset = std::get<Tileset>(tmx);
  TMXLayer layer = std::get<Layer>(tmx);
  for (TMXLayer::size_type i = 0; i < layer.size(); i++)
    objects.push_back(parse_layer(std::get<Map>(tmx), tileset[i].second, i, layer[i].second));
  return objects;
}
    
  

