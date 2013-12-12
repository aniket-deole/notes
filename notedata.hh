#ifndef _NOTEDATA_HH_
#define _NOTEDATA_HH_

class NoteData {
private:
  int primary_key;
   
  std::string title;
  std::string remaining;
  std::string summary;
public:

  NoteData () {}

  NoteData (int p_key, std::string t, std::string r, std::string s) {
    primary_key = p_key;
    title = t;
    remaining = r;
    summary = s;
  }

  int getPrimaryKey () { return primary_key; }

  std::string getTitle () { return title; }
  std::string getRemaining () { return remaining; }
  std::string getSummary () { return summary; }
};


#endif
