# UNIT 01: Introduction and overall goal of the project.
## Introduction
Greetings! This program was developed as a final project of online course "C++ developer from scratch(or from zero)" of educational platform "Skillbox". As a part of a task we were provided with a legend, which justified creation and implementation of variety of classes and corresponding methods to achieve a result,  given by a task.

According to a legend, we are new employee of company, which recently stopped using existing search engines and are striving to design and implement one of their own making. We are tasked with creating such engine.

## Overall goal of the project
Main goal of a project is to provide it's user with a neccesary means to handle multiple text documents and give them ability to form request to a database, consisting of those documents, to get a most relevant results to given request. Hence the name "Search engine".

# UNIT 02: Methodology, implemented classes and .json files.
## Methodology
Since there is no direct interaction between user and a program, all crucial data exists within a file "config.json" which provides path's to a source files, manages number of answers and contains information about engine itself. Without "config.json" in a project root directory program will not run, and no futher interaction is possible.

After "config.json" have been found or placed in a root directory, object of a class "ConverterJSON" will be created to handle data, provided by a file. First of all, content of source files and "requests.json" shall be written to containers of a <vector> type, to be passed to the objects of different classes futher down the line.
 
One of such objects is "InvertedIndex" class, which will be responsible for creation of "frequency dictionary" to form a collection of individual words in every source file and matching those words with <vectors> consisting of "Entry" structures. "Entry" defines id of a document, from which word has benn taken and provides count to how many times it was implemented in a document.

After "InvertedIndex" has finished updating it's dictionary, it is time to create another object, this time of a class "SearchServer", which will be used to form indexes of relativity for each document. This task will be performed by creating, sorting and orginizing another structure - "RelativeIndex". Each "RelativeIndex" contains id of document it decribes and a rank. Basically, the higher the rank - the more relevant document with certain id in relation to given request.

There is only one thing left to do and it's to create file "answers.json", which will contain our results. It is done by converting results, given by "SearchServer", in to <vector> of pairs and passing it back to "ConverterJSON".

## Implemented classes
**ConverterJSON** - as mentioned above, this class is responsible for handling our .json files, i.e. "config", "requests" and "answers". To do so it implement's a few methods: 

* checkConfigIntegrity() - this method attempts to open file "config.json" and will terminate execution of a programm if file is not found or corrupted.

* checkRequestsIntegrity() - similar to the one described above, but it will check "requests.json" instead.   

* getTextDocuments() - method will attempt to form <vector> of <string> type from content of the source files. In case of missing files a warning will be issued, but execution will continue. 
 
* getRequests() - this method is pretty much similar to one described above.

* getResponsesLimit() - get number of possible responses to a single request, to determine how many relevant documents will be included in "answers.json".

* putAnswers() - after all our classes has done their job, this method will create new file(or clear and rewrite existing one) "answers.json", which will contain search results.

**InvertedIndex** - this class sole responsibility is to form a collection of a word and <vector> of "Entry" type, to determine how often every single word was found in the source files. It's methods are:

* breakDownString() - receive a string and integer from a <vector> to form a part of dictionary for this document. Create "Entry" for every unique individual word and add it to a dictionary, or increase count, in case current word already been included.

* updateDocumentBase() - this method relies on a previous one to do it's job, since it recives <vector> formed from source files and launches different threads to speed up a process of filling dictionary.

* getWordsCount() - attempts to iterate through dictionary and return <vector> filled with "Entry" for a given word.

* print() - supplementary method, not currently used, just in case.

* sortDictionaryIds() - this method is present to make sure what documents are structured by their id's in the correct order, i.e. from lowest id to highest id.

**SearchServer** - After .json files have been handled and dictionary have been filled it is finally time to get results for our requests. Here is how:

* getEntriesForString() - we take one element of <vector> of requests and break it down to unique words. After we have done so, implement "getWordsCount()" for every word to get <vector> of "Entry".

* getIndexForString() - now we have entries for a certain request. So the next step is to form "RelativeIndex" for each document, contained in entries. We do so by creating index for every document id, finding every "Entry" with the same id and increasing rank of document, based on count, contained in "Entry".

* getMaxRank() - now it is time to find a document with a highest rank to continue form a result.

* sortByMaxRank() - we want documents with a highest to rank to be first in our resulting <vector>. If two documents have equal ranks - the preference goes to the one with the lowest id.

* formRelativeRanks() - after finding document with a maximum rank we will define relevancy of each document in accordance with a request. So if a document a 100% relevant - it will receive a releveant rank of 1, if it only 50% relevant - it's rank will be 0.5 etc.

* search() - this method will receive full <vector> of requests, formed as a result of "getRequests()" method, and implement all of the methods above to create a result, containing relevant to every request documents, limited by maximum possible responses.

* convertToPair() - as a final stroke, we need to convert our results from "RelativeIndex" to pairs, since it was specifically defined by the task.

  ## .json files
  
It is very important to note, what successfull execution of a programm heavily depends on correct handling of .json files. Their existance in the root directory is crucial to performing first steps of our program, and they should be treated with care. Let's break every one of them down and see what is so important about them:

"config.json" - without this file you can not run program. It is as simple as that. So, to avoid unnecessary troubles, please make sure what it is placed in the root directory of the project. Now let's take a closer look at a structure of this file:

it contains two fields - "config" and "files". In the "config" field there is an information about our program  - name, version and, most importantly, number of maximum possible responses.
"files" is the field used to determine path to each individual source file. Please take note what it contains relative pathes, like "..\\..\\file001.txt" and it is crucial for a corresponding files to have a matching names and be placed at the project root directory.

"requests.json" - second very important file, i.e. without it program will terminate its execution. It was not specified in the task, but what is the point of search engine without any requests, right? On to the content of file:

There is not really much to describe here, it is just "requests" and lines of text, main organizing principle is "one line - one request". As always, you should make sure what this file is placed in the project root directory.

"answers.json" - now this is a file created as a result of program completing its task. It consists of field "answers", which contains fields "request00X", "result", "relevance". "request00X" is a number of request, according to it's position in "requests.json", "result" is a bool value, which will be "true" if any results were found or "false" if all of the source files do not contain a single word corresponding to request. "relevance" is formed by two values: "doc_id" as a number of document and "rank" as a rank of relevancy, formed by "searchServer". 

# UNIT 03: Technical information
## Clion configuration
Current project was build with IDE Clion by "JetBrains", version 2023.1.1 and CMake, version 3.25. As of a third-party software - nlohmann_json was implemented for handling .json files. Current project also includes series of test, which were made using GoogleTest FrameWork.

You should have no problems running it on your local machine, provided you have corresponding vesions of CLion and CMake available, since nlohmann_json is included in the source code, and GoogleTests will be build through FetchContent command in their respective CMake list.
