#include "BoardRepository.hpp"
#include "Core/Exception/NotImplementedException.hpp"
#include <ctime>
#include <filesystem>
#include <iterator>
#include <sstream>
#include <string.h>
#include <string>
#include <tr1/unordered_map>
#include <vector>

using namespace Prog3::Repository::SQLite;
using namespace Prog3::Core::Model;
using namespace Prog3::Core::Exception;
using namespace std;

#ifdef RELEASE_SERVICE
string const BoardRepository::databaseFile = "./data/kanban-board.db";
#else
string const BoardRepository::databaseFile = "../data/kanban-board.db";
#endif

BoardRepository::BoardRepository() : database(nullptr) {

    string databaseDirectory = filesystem::path(databaseFile).parent_path().string();

    if (filesystem::is_directory(databaseDirectory) == false) {
        filesystem::create_directory(databaseDirectory);
    }

    int result = sqlite3_open(databaseFile.c_str(), &database);

    if (SQLITE_OK != result) {
        cout << "Cannot open database: " << sqlite3_errmsg(database) << endl;
    }

    initialize();
}

BoardRepository::~BoardRepository() {
    sqlite3_close(database);
}

void BoardRepository::initialize() {
    int result = 0;
    char *errorMessage = nullptr;

    string sqlCreateTableColumn =
        "create table if not exists column("
        "id integer not null primary key autoincrement,"
        "name text not null,"
        "position integer not null UNIQUE);";

    string sqlCreateTableItem =
        "create table if not exists item("
        "id integer not null primary key autoincrement,"
        "title text not null,"
        "date text not null,"
        "position integer not null,"
        "column_id integer not null,"
        "unique (position, column_id),"
        "foreign key (column_id) references column (id));";

    result = sqlite3_exec(database, sqlCreateTableColumn.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
    result = sqlite3_exec(database, sqlCreateTableItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    // only if dummy data is needed ;)
    createDummyData();
}

Board BoardRepository::getBoard() {
    throw NotImplementedException();
}

std::vector<Column> BoardRepository::getColumns() {

    string sqlGetIds =
        "SELECT id FROM column;";

    int result = 0;
    char *errorMessage = nullptr;

    vector<int> columnIds;

    vector<Column> columns;

    result = sqlite3_exec(database, sqlGetIds.c_str(), BoardRepository::getIdsCallback, &columnIds, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK != result) {
        return vector<Column>();
    }
    for (int id : columnIds) {
        optional<Column> tmp = getColumn(id);
        if (tmp.has_value()) {
            columns.push_back(tmp.value());
        }
    }
    return columns;
}

std::optional<Column> BoardRepository::getColumn(int id) {

    /*
        string sqlGetColumn = "SELECT * FROM column WHERE id=" + to_string(id) + ";";

        int result = 0;
        char *errorMessage = nullptr;

        Column column(-1, "", 0);
        Column *pColumn = &column;

        result = sqlite3_exec(database, sqlGetColumn.c_str(), BoardRepository::getColumnCallback, pColumn, &errorMessage);
        handleSQLError(result, errorMessage);

        if (SQLITE_OK == result && column.getId() != -1) {
            return column;
        } else {
            return nullopt;
        }*/

    string sqlSelect = "SELECT * FROM column WHERE id=" + to_string(id) + ";";

    char *errorMessage = nullptr;
    // das erstellen von "column" wird zum reservieren vom Speicherplatz benötigt
    Column column(-1, "", 0);
    Column *pcolumn = &column;

    vector<Item> items = getItems(id);
    vector<Item> *pitems = &items;

    int result = sqlite3_exec(database, sqlSelect.c_str(), BoardRepository::getColumnCallback, pcolumn, &errorMessage);
    handleSQLError(result, errorMessage);

    if (result != SQLITE_OK || column.getId() == -1) {
        return nullopt;
    }
    // statt auto -> Item
    for (auto item : items) {
        column.addItem(item);
    }

    return column;
}

std::optional<Column> BoardRepository::postColumn(std::string name, int position) {

    // Eingefügt
    string sqlPostItem =
        "INSERT INTO column('name', 'position') "
        "VALUES('" +
        name + "', '" + to_string(position) + "')";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {
        int columnId = sqlite3_last_insert_rowid(database);
        return Column(columnId, name, position);
    }

    return std::nullopt;
}

std::optional<Prog3::Core::Model::Column> BoardRepository::putColumn(int id, std::string name, int position) {
    throw NotImplementedException();
}

void BoardRepository::deleteColumn(int id) {

    // Eingefügt
    string sqlPostItem =
        "DELETE FROM column WHERE id = " + to_string(id) + ";";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {

        std::cout << "Erfolgreich gelöscht!" << endl;
    }
}

std::vector<Item> BoardRepository::getItems(int columnId) {

    string sqlGetItems =
        "SELECT * FROM item WHERE column_id=" + to_string(columnId) + ";";

    int result = 0;
    char *errorMessage = nullptr;

    vector<Item> items;

    result = sqlite3_exec(database, sqlGetItems.c_str(), BoardRepository::getItemsCallback, &items, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result && items.size() != 0) {
        return items;
    } else {
        return vector<Item>();
    }
}

std::optional<Item> BoardRepository::getItem(int columnId, int itemId) {

    string sqlGetItem =
        "SELECT * FROM item WHERE column_id=" + to_string(columnId) + " AND id=" + to_string(itemId) + ";";

    int result = 0;
    char *errorMessage = nullptr;

    vector<Item> items;

    result = sqlite3_exec(database, sqlGetItem.c_str(), BoardRepository::getItemsCallback, &items, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result && items.size() != 0) {
        return items[0];
    } else {
        return nullopt;
    }
}

std::optional<Item> BoardRepository::postItem(int columnId, std::string title, int position) {

    // Eingefügt
    time_t now = time(0);
    char *datetime = ctime(&now); // * = definieren das eine variable als Zeiger deklariert werden soll

    string sqlPostItem =
        "INSERT INTO item ('title', 'date', 'position', 'column_id') "
        "VALUES ('" +
        title + "', '" + datetime + "', " + to_string(position) + ", " + to_string(columnId) + ");";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    int itemId = INVALID_ID;
    if (SQLITE_OK == result) {
        itemId = sqlite3_last_insert_rowid(database);
        return Item(itemId, title, position, datetime);
    }
    return std::nullopt;
}

std::optional<Prog3::Core::Model::Item> BoardRepository::putItem(int columnId, int itemId, std::string title, int position) {
    throw NotImplementedException();
}

void BoardRepository::deleteItem(int columnId, int itemId) {

    // Eingefügt
    /*
    time_t now = time(0);
    char *datetime = ctime(&now); // * = definieren das eine variable als Zeiger deklariert werden soll
    */
    string sqlPostItem =
        "DELETE FROM item WHERE column_id = " + to_string(columnId) + " AND id = " + to_string(itemId);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {
        std::cout << "Erfolgreich gelöscht!" << endl;
    }
}

void BoardRepository::handleSQLError(int statementResult, char *errorMessage) {

    if (statementResult != SQLITE_OK) {
        cout << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

void BoardRepository::createDummyData() {

    cout << "creatingDummyData ..." << endl;

    int result = 0;
    char *errorMessage;
    string sqlInsertDummyColumns =
        "insert into column (name, position)"
        "VALUES"
        "(\"prepare\", 1),"
        "(\"running\", 2),"
        "(\"finished\", 3);";

    result = sqlite3_exec(database, sqlInsertDummyColumns.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    string sqlInserDummyItems =
        "insert into item (title, date, position, column_id)"
        "VALUES"
        "(\"in plan\", date('now'), 1, 1),"
        "(\"some running task\", date('now'), 1, 2),"
        "(\"finished task 1\", date('now'), 1, 3),"
        "(\"finished task 2\", date('now'), 2, 3);";

    result = sqlite3_exec(database, sqlInserDummyItems.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

/*
  I know source code comments are bad, but this one is to guide you through the use of sqlite3_exec() in case you want to use it.
  sqlite3_exec takes a "Callback function" as one of its arguments, and since there are many crazy approaches in the wild internet,
  I want to show you how the signature of this "callback function" may look like in order to work with sqlite3_exec()
*/
int BoardRepository::queryCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames) {
    return 0;
}

int BoardRepository::getColumnCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames) {

    Column *pColumn = static_cast<Column *>(data);

    vector<std::string> values;

    for (int i = 0; i < numberOfColumns; i++) {
        values.push_back(*(fieldValues++));
    }

    int id = std::stoi(values[0]);
    string name = values[1];
    int position = std::stoi(values[2]);

    pColumn->setID(id);
    pColumn->setName(name);
    pColumn->setPos(position);

    return 0;
}

int BoardRepository::getItemsCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames) {

    vector<Item> *pItems = static_cast<vector<Item> *>(data);

    vector<string> values;

    for (int i = 0; i < numberOfColumns; i++) {
        values.push_back(*(fieldValues++));
    }

    int id = stoi(values[0]);
    string title = values[1];
    string date = values[2];
    int position = stoi(values[3]);

    Item item(id, title, position, date);

    pItems->push_back(item);

    return 0;
}

int BoardRepository::getIdsCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames) {

    vector<int> *pIds = static_cast<vector<int> *>(data);
    /*
        vector<int> values;

        if (numberOfColumns == 0) {
            return 0;
        }


          for(int i = 0; i < numberOfColumns; i++){
            values.push_back(*(fieldValues++));
          }
      */
    pIds->push_back(atoi(fieldValues[0]));

    return 0;
}
