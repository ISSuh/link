/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_STORAGE_DATABASE_ADAPTOR_ODBC_ODBC_ADAPTOR_H_
#define LINK_STORAGE_DATABASE_ADAPTOR_ODBC_ODBC_ADAPTOR_H_

#include <sql.h>
#include <sqlext.h>

#include <string>

#include "link/storage/database/adaptor/adaptor.h"

namespace nlink {
namespace storage {

class ODBCAdaptor : public Adaptor {
 public:
  bool Connect(const std::string& path) override;
  bool Excute(const std::string& sql) override;
  bool Close() override;

 private:
};

}  // namespace storage
}  // namespace nlink

#endif  // #define LINK_STORAGE_DATABASE_ADAPTOR_ODBC_ODBC_ADAPTOR_H_
