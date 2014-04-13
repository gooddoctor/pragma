#ifndef __LAYER_PARSER_H
#define __LAYER_PARSER_H

#include <vector>

#include <QString>

class LayerParser {
public:
typedef std::tuple<int, int, QString> Layer;
public:
  static LayerParser& comming_in_fast() {
    static LayerParser instance;
    return instance;
  }
  Layer parse(int width, int height, int tilewidth, int tileheight,
	      const QString& source, const std::vector<int>& data);
};

#endif
