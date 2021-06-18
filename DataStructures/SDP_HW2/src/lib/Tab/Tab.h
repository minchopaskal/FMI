#ifndef __TAB_H
#define __TAB_H

#include <string>
#include <ctime>

class Tab {
private:
  std::string URL;
  time_t timestamp;
public:
  Tab(std::string URL = "");

  bool operator==(const Tab&) const;
  
  std::string getURL() const;
  time_t getTimestamp() const;

  void setURL(std::string);
};

class URLComparator {
public:
  bool operator()(const Tab& t1, const Tab& t2);
};

class TimestampComparator {
public:
  bool operator()(const Tab& t1, const Tab& t2);
};

#endif
