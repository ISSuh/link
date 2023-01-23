/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_STORAGE_DATABASE_STORAGE_H_
#define LINK_STORAGE_DATABASE_STORAGE_H_

namespace nlink {
namespace storage {

class Database {
 public:
  bool Connect();
  bool Disconnect();
 private:
};

}  // namespace storage
}  // namespace nlink

#endif  // LINK_STORAGE_DATABASE_STORAGE_H_
