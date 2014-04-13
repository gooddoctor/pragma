#include <QDebug>

#include "parser/tmx_parser.hpp"
#include "parser/layer_parser.hpp"

int main(int, char**) {
  TMXParser::TMX tmx = TMXParser::comming_in_fast().parse("tile/map.tmx");
  int width = std::get<0>(tmx)["width"];
  int height = std::get<0>(tmx)["height"];
  int tilewidth = std::get<0>(tmx)["tilewidth"];
  int tileheight = std::get<0>(tmx)["tileheight"];
  qDebug() << width << height << tilewidth << tileheight;

  TMXParser::TMXTileset tileset = std::get<1>(tmx);
  TMXParser::TMXLayer tmx_layer = std::get<2>(tmx);
  for (int i = 0; i < tmx_layer.size(); i++) {
    LayerParser::Layer layer = LayerParser::comming_in_fast().
					    parse(width, height, tilewidth, tileheight,
					          tileset[i].second, tmx_layer[i].second);
    qDebug() << std::get<0>(layer) << std::get<1>(layer) << std::get<2>(layer);
  }
}
