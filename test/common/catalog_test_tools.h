/**
 * This file is part of the CernVM File System.
 */

#ifndef CVMFS_CATALOG_TEST_TOOLS_H_
#define CVMFS_CATALOG_TEST_TOOLS_H_

#include <string>
#include <vector>

#include "catalog_mgr_rw.h"
#include "directory_entry.h"
#include "manifest.h"
#include "server_tool.h"
#include "statistics.h"
#include "upload.h"

struct DirSpecItem {
  catalog::DirectoryEntry entry_;
  XattrList xattrs_;
  std::string parent_;

  DirSpecItem(const catalog::DirectoryEntry& entry, const XattrList& xattrs,
              const std::string& parent)
      : entry_(entry), xattrs_(xattrs), parent_(parent) {}


  const catalog::DirectoryEntryBase& entry_base() const {
    return static_cast<const catalog::DirectoryEntryBase&>(entry_);
  }
  const XattrList& xattrs() const { return xattrs_; }
  const std::string& parent() const { return parent_; }
};

struct DirSpec {
  void AddFile(const std::string& name,
               const std::string& parent,
               const std::string& digest,
               const size_t size,
               const XattrList& xattrs = XattrList(),
               shash::Suffix suffix = shash::kSha1);
  void AddDirectory(const std::string& name,
                    const std::string& parent,
                    const size_t size);

  void AddDirectoryEntry(const catalog::DirectoryEntry& entry,
                         const XattrList& xattrs,
                         const std::string& parent);

  void ToString(const DirSpec& spec, std::string* out);

  size_t NumItems() const { return items_.size(); }

  const DirSpecItem& Item(const size_t idx) const { return items_[idx]; }

  std::vector<DirSpecItem> items_;
};


class CatalogTestTool : public ServerTool {
 public:
  typedef std::vector<std::pair<std::string, shash::Any> > History;

  CatalogTestTool(const std::string& name);
  ~CatalogTestTool();

  bool Init();
  bool Apply(const std::string& id, const DirSpec& spec);

  bool DirSpecAtRootHash(const shash::Any& root_hash, DirSpec* spec);

  manifest::Manifest* manifest() { return manifest_.weak_ref(); }

  History history() { return history_; }

 private:
  static upload::Spooler* CreateSpooler(const std::string& config);

  static manifest::Manifest* CreateRepository(const std::string& dir,
                                              upload::Spooler* spooler);

  static catalog::WritableCatalogManager* CreateCatalogMgr(
      const shash::Any& root_hash, const std::string stratum0,
      const std::string& temp_dir, upload::Spooler* spooler,
      download::DownloadManager* dl_mgr, perf::Statistics* stats);

  const std::string name_;

  std::string stratum0_;
  std::string temp_dir_;

  UniquePtr<manifest::Manifest> manifest_;
  UniquePtr<upload::Spooler> spooler_;
  History history_;
};

#endif  //  CVMFS_CATALOG_TEST_TOOLS_H_
