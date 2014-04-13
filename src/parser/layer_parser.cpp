#include "layer_parser.hpp"
#include <tuple>

LayerParser::Layer LayerParser::parse(int width, int height, 
				      int tilewidth, int tileheight,
				      const QString& source, const std::vector<int>& data) {
  for (int i = 0; i < height * width; i++) {
    if (data[i] != 0) {
      return std::make_tuple((i % height) * tilewidth, (i / height) * tileheight, source);
    }
  }
  return Layer();
}
