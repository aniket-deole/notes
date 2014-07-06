/***
Copyright (C) 2013 Aniket Deole <aniket.deole@gmail.com>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 2.1, as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranties of
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <iostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>
#include "notedata.hh"

static std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

NoteData::NoteData (int p_key, std::string t, std::string b,int create_time, int modified_time, std::string g, std::string n_guid,
        std::string n_name) {
    primary_key = p_key;
    title = t;
    body = b;
    create_unix_time = create_time;
    modified_unix_time = modified_time;
    guid = g;
    notebook_guid = n_guid;
    notebookName = n_name;

    /* Create summary to display in the note list pane view. */
//    std::cout << b << std::endl;
	
	    /* Removing HTML Tags */
    std::string strippedSummary = "";
    std::vector<std::string>    tags;
    std::vector<std::string>    text;

    for(;;)
    {
        std::string::size_type  startpos;

        startpos = b.find('<');
        if(startpos == std::string::npos)
        {
            // no tags left only text!
            text.push_back(b);
            break;
        }

        // handle the text before the tag    
        if(0 != startpos)
        {
            text.push_back(b.substr(0, startpos));
            if ((b.size () - startpos) < b.size ())
               b = b.substr(startpos, b.size() - startpos);
            startpos = 0;
        }

        //  skip all the text in the html tag
        std::string::size_type endpos;
        for(endpos = startpos;
            endpos < b.size() && b[endpos] != '>';
            ++endpos)
        {
            // since '>' can appear inside of an attribute string we need
            // to make sure we process it properly.
            if(b[endpos] == '"')
            {
                endpos++;
                while(endpos < b.size() && b[endpos] != '"')
                {
                    endpos++;
                }
            }
        }

        //  Handle text and end of html that has beginning of tag but not the end
        if(endpos == b.size())
        {
           b = b.substr(endpos, b.size() - endpos);
            break;
        }
        else
        {
            //  handle the entire tag
            endpos++;
            if ((endpos - startpos) > b.size ())
                break;
            tags.push_back(b.substr(startpos, endpos - startpos));
            if ((b.size () - endpos) > b.size ()) 
                break;
            b = b.substr(endpos, b.size() - endpos);
        }
    }


    // auto, iterators or range based for loop would probably be better but
    // this makes it a bit easier to read.    
    for(size_t i = 0; i < tags.size(); i++)
    {
 //       
    }

    for(size_t i = 0; i < text.size(); i++)
    {
 //       
        strippedSummary.append (text[i]);
    }

    /* Removing HTML Tags Over */

    /* Change special Characters. */
    strippedSummary = ReplaceString (strippedSummary, "&nbsp;", " ");
    strippedSummary = ReplaceString (strippedSummary, "\n", " ");
    strippedSummary = ReplaceString (strippedSummary, "&", "&amp;");

    summary = trim (strippedSummary);
//    std::cout << strippedSummary << std::endl;
}