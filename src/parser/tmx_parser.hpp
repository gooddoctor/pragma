#ifndef __TMX_PARSER_H
#define __TMX_PARSER_H

#include <map>
#include <tuple>
#include <vector>

#include <QString>
#include <QFile>

class TMXParser {
public:
typedef std::map<QString, int> TMXMap;
typedef std::vector<std::pair<int, QString> > TMXTileset;
typedef std::vector<std::pair<int, std::vector<int> > > TMXLayer;
typedef std::tuple<TMXMap, TMXTileset, TMXLayer> TMX;
public:
  static TMXParser& comming_in_fast() {
    static TMXParser instance;
    return instance;
  }
  TMX parse(const QString& tmx_file);
};

#endif
