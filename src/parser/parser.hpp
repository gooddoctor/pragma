#ifndef __PARSER_H
#define __PARSER_H

#include <map>
#include <tuple>
#include <vector>

#include <QString>

namespace parser {
  typedef std::map<QString, int> TMXMap;
  typedef std::vector<std::pair<int, QString> > TMXTileset;
  typedef std::vector<std::pair<int, std::vector<int> > > TMXLayer;
  typedef std::tuple<int, int, int, QString> TMXObject;
  typedef std::tuple<TMXMap, TMXTileset, TMXLayer> TMX;
  enum {X, Y, Z, Sprite};
  enum {Map, Tileset, Layer};

  class TMXParser {
  public:
    static TMXParser& comming_in_fast() {
      static TMXParser instance;
      return instance;
    }
    TMX parse(const QString& tmx_file);
  };

  class LayerParser {
  public:
    static LayerParser& comming_in_fast() {
      static LayerParser instance;
      return instance;
    }
    std::vector<TMXObject> parse(const TMX& tmx);
  };
}

#endif
