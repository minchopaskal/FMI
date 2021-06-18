#include "Tab.h"

Tab::Tab(std::string URL) :
  URL(URL), timestamp(time(nullptr)) {}

bool Tab::operator==(const Tab& other) const {
  return this->getURL().compare(other.getURL()) == 0
    && this->getTimestamp() == other.getTimestamp();
}

std::string Tab::getURL() const {
  return this->URL;
}

time_t Tab::getTimestamp() const {
  return this->timestamp;
}

void Tab::setURL(std::string URL) {
  this->URL = URL;
  this->timestamp = time(nullptr);
}

bool URLComparator::operator()(const Tab& t1, const Tab& t2) {
  int comparison = t1.getURL().compare(t2.getURL());
  if (comparison < 0) {
    return true;
  }
  if (comparison > 0) {
    return false;
  }

  if(t1.getTimestamp() - t2.getTimestamp() <= 0) {
    return true;
  }

  return false;
}

bool TimestampComparator::operator()(const Tab& t1, const Tab& t2)  {
  int comparison = t1.getTimestamp() - t2.getTimestamp();
  if(comparison < 0) {
    return true;
  }
  if(comparison > 0) {
    return false;
  }

  int strComp = t1.getURL().compare(t2.getURL());
  if(strComp > 0) {
    return false;
  }

  return true;
}
