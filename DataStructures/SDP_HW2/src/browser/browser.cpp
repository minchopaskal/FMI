#include "browser.h"

Browser::Browser() : homepage("about:config") {
  browser.push_back(Tab(homepage));
  current = browser.begin();
}

void Browser::go(std::string URL) {
  (*current).setURL(URL);
}

void Browser::insert(std::string URL) {
  browser.insertAt(Tab(URL), current);
  forward();
}

void Browser::back() {
  if(browser.getSize() > 1 && current != browser.begin()) {
    --current;
  }
}

void Browser::forward() {
  if(browser.getSize() > 1 && current != browser.last()) {
    ++current;
  }
}

void Browser::remove() {
  if(browser.getSize() == 1) {
    (*current).setURL(homepage);
  } else {
    tit nxt;
    // We are sure prev is valid since size > 1
    if(current == browser.last()) nxt = current.prev();
    else nxt = current.next();
    
    browser.removeAt(current);
    if(browser.getSize() == 1){
      current = browser.begin();
    }
    else current = nxt;
  }
}

void Browser::print() const {
  for(tit it = browser.begin(); it != browser.end(); ++it) {
    std::cout << (*it).getURL() << " " << (*it).getTimestamp() << std::endl;
  }
}

std::string Browser::getHomepage() const {
  return homepage;
}

void Browser::sort(bool byURL) {
  Tab curr = *current;
  if(byURL) {
    browser.sort<URLComparator>();
  } else {
    browser.sort<TimestampComparator>();
  }
  
  bool success= false;
  for(tit it = browser.begin(); it != browser.end(); ++it) {
    if(*it == curr) {
      current = it;
      success = true;
    }
  }
  if(!success) {
    current = browser.last();
  }
}

const Tab& Browser::getCurrent() const {
  return current.cget();
}
