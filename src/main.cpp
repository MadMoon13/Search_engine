#include "converterJSON.h"
#include "invertedIndex.h"
#include "searchServer.h"

int main() {
    // start program by creating object responsible for working with .json files (config, requests. source files).
    ConverterJSON conv;

    // check if crucial files are missing, it can be either "config.json" or "requests.json"
    conv.checkConfigIntegrity();
    conv.checkRequestsIntegrity();

    // create objects of specified types, by means provided by ConverterJSON class, to be passed to other classes.
    auto docs = conv.getTextDocuments();
    auto requests = conv.getRequests();
    auto max_responses = conv.getResponsesLimit();

    // now create an InvertedIndex class object to form dictionary from source files.
    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    // after dictionary is formed, create object of SearchServer class, by passing existing InvertedIndex object to it.
    SearchServer srv(idx);
    srv.setMaxAnswers(max_responses);
    auto result = srv.search(requests);

    // now all there is left to do is to convert results of a search and pass them to ConverterJSON class for it to
    // form an "answers.json" document.
    auto converted_result = srv.convertToPair(result);
    conv.putAnswers(converted_result);
}
