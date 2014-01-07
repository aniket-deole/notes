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


NoteData::NoteData (int p_key, std::string t, std::string r, std::string b, int n_id, int create_time, int modified_time, std::string g, std::string n_guid) {
    primary_key = p_key;
    title = t;
    remaining = r;
    body = b;
    notebook_id = n_id;
    create_unix_time = create_time;
    modified_unix_time = modified_time;
    guid = g;
    notebook_guid = n_guid;

    /* Create summary to display in the note list pane view. */

	
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
            tags.push_back(b.substr(startpos, endpos - startpos));
            b = b.substr(endpos, b.size() - endpos);
        }
    }


    // auto, iterators or range based for loop would probably be better but
    // this makes it a bit easier to read.    
    for(size_t i = 0; i < tags.size(); i++)
    {
 //       std::cout << tags[i] << std::endl;
    }

    for(size_t i = 0; i < text.size(); i++)
    {
 //       std::cout << text[i] << std::endl;
        strippedSummary.append (text[i]);
    }

    /* Removing HTML Tags Over */

    /* Change special Characters. */
    strippedSummary = ReplaceString (strippedSummary, "&nbsp;", " ");
    strippedSummary = ReplaceString (strippedSummary, "\n", " ");
    strippedSummary = ReplaceString (strippedSummary, "&", "&amp;");

    summary = strippedSummary;
}