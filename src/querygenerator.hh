#ifndef _QUERY_GENERATOR_HH_
#define _QUERY_GENERATOR_HH_



class QueryGenerator {
  private:
    std::string getNotebookCountString;
    std::string getAllNotebooksString;
    QueryGenerator ();
    static QueryGenerator* queryGenerator;
  public:

    enum QUERYTYPE { getNotebookCount, getAllNotebooks };

    static std::string getQuery (QUERYTYPE type, std::string base);
};


#endif
