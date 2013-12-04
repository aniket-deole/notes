#ifndef _NOTEDATA_HH_
#define _NOTEDATA_HH_

class NoteData {
private:
  std::string title;
  std::string remaining;
  std::string summary;
public:

  NoteData () {}

  NoteData (std::string t, std::string r, std::string s) {
    title = t;
    remaining = r;
    summary = s;
  }

  std::string getTitle () { return title; }
  std::string getRemaining () { return remaining; }
  std::string getSummary () { return summary; }
};


#endif
