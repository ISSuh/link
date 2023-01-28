/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <stdio.h>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/storage/database/database_factory.h>
#include <link/storage/database/adaptor/sqlite3/sqlite3_adaptor.h>

using namespace nlink;

const char* kSampleDBPath = "./test.db";

void RemoveTestDatabaseIfExist(const std::string& path) {
  const int result = ::remove(path.c_str());
  if (result != 0) {
    std::cout << path << " remove fail. " << strerror(errno) << std::endl;
  }
}

TEST(Storage, Open_storage) {
  storage::DatabaseFactory db_factory;
  auto db = db_factory.CreateDatabase<storage::Sqlite3>();

  std::string empty_path("");
  EXPECT_FALSE(db->Connect(empty_path));

  EXPECT_TRUE(db->Connect(kSampleDBPath));
  db->Disconnect();
}

TEST(Storage, excute_storage) {
  RemoveTestDatabaseIfExist(kSampleDBPath);

  storage::DatabaseFactory db_factory;
  auto db = db_factory.CreateDatabase<storage::Sqlite3>();

  EXPECT_TRUE(db->Connect(kSampleDBPath));

  std::string sql = "CREATE TABLE PEOPLE ("  \
                    "ID INT PRIMARY KEY     NOT NULL," \
                    "NAME           TEXT    NOT NULL);";
  db->Excute(sql);

  sql = "INSERT INTO PEOPLE VALUES(0, 'tom');";
  db->Excute(sql);

  sql = "INSERT INTO PEOPLE VALUES(1, 'jake');";
  db->Excute(sql);

  sql = "SELECT * "  \
        "FROM PEOPLE;";
  db->Excute(sql);


  db->Disconnect();
}
