/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef SRC_LINK_STORAGE_DATABASE_DATABASE_FACTORY_H_
#define SRC_LINK_STORAGE_DATABASE_DATABASE_FACTORY_H_

#include <memory>
#include <utility>

#include "link/storage/database/database.h"
#include "link/storage/database/adaptor/adaptor.h"

namespace nlink {
namespace storage {

class DatabaseFactory {
 public:
  template<typename AdaptorType>
  std::unique_ptr<Database> CreateDatabase() {
    std::unique_ptr<Adaptor> adaptor =
      std::make_unique<AdaptorType>();
    return std::make_unique<Database>(std::move(adaptor));
  }
};

}  // namespace storage
}  // namespace nlink

#endif  // SRC_LINK_STORAGE_DATABASE_DATABASE_FACTORY_H_
