#ifndef __BROWSER_H
#define __BROWSER_H

#include <cstring>
#include "../lib/List/List.h"
#include "../lib/Tab/Tab.h"

class Browser {
private:
  using tabs = List<Tab>;
  using tit = List<Tab>::iterator;
  
  std::string homepage;
  tabs browser;
  tit current;
  
public:
  Browser();

  void go(std::string);
  void insert(std::string);
  void back();
  void forward();
  void remove();
  void print() const;
  void sort(bool);

  std::string getHomepage() const;
  const Tab& getCurrent() const;
};

#endif
