#include <memory>

#include <QDomDocument>

#include <base64.hpp>
#include <zlib.h>

#include "tmx_parser.hpp"

QString get_content(const QString& tmx_file) {
  QFile file(tmx_file);
  file.open(QIODevice::ReadWrite);
  return file.readAll();
}

TMXParser::TMXMap get_map(const QDomDocument& xml) {
  QDomElement entry = xml.documentElement();
  return TMXParser::TMXMap({{"width", entry.attribute("width").toInt()},
			    {"height", entry.attribute("height").toInt()},
			    {"tilewidth", entry.attribute("tilewidth").toInt()},
			    {"tileheight", entry.attribute("tileheight").toInt()}});
}

TMXParser::TMXTileset get_tileset(const QDomDocument& xml) {
  TMXParser::TMXTileset tmx_tileset;
  for (QDomElement entry = xml.documentElement().firstChildElement(); !entry.isNull();
       entry = entry.nextSiblingElement())
    if (entry.tagName() == "tileset")
      tmx_tileset.push_back({entry.attribute("firstgid").toInt(),
			     entry.firstChildElement().attribute("source")});
  return tmx_tileset;
}

//################################################################################
std::vector<int> get_layer_data(const QDomElement& entry) {
  int width = entry.attribute("width").toInt();
  int height = entry.attribute("height").toInt();
  unsigned long len = width * height * sizeof(int);

  std::unique_ptr<int> data(new int[width * height]);
  auto compress_data = base64_decode(entry.firstChildElement().text().trimmed().toStdString());
  uncompress((unsigned char*)data.get(), &len, &compress_data[0], compress_data.size());

  return std::vector<int>(data.get(), data.get() + width * height);
}
TMXParser::TMXLayer get_layer(const QDomDocument& xml) {
  TMXParser::TMXLayer tmx_layer;
  int i = 1;
  for (QDomElement entry = xml.documentElement().firstChildElement(); !entry.isNull();
       entry = entry.nextSiblingElement())
    if (entry.tagName() == "layer")
      tmx_layer.push_back({i++, get_layer_data(entry)});
  return tmx_layer;
}
//################################################################################      

TMXParser::TMX TMXParser::parse(const QString& tmx_file) {
  QDomDocument xml;
  xml.setContent(get_content(tmx_file));
  return make_tuple(get_map(xml), get_tileset(xml), get_layer(xml));
}
