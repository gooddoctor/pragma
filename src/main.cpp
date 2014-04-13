#include <QDebug>

#include "parser/parser.hpp"

using namespace parser;

int main(int argc, char** argv) {
  std::vector<TMXObject> objects = LayerParser::comming_in_fast().
				   parse(TMXParser::comming_in_fast().parse("map.tmx"));
  for (auto it = objects.begin(); it != objects.end(); it++) {
    qDebug() << std::get<X>(*it) << std::get<Y>(*it)
	     << std::get<Z>(*it) << std::get<Image>(*it);
  }
}
