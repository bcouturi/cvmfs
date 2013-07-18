
#include "io_dispatcher.h"
#include "file.h"
#include "util.h"
#include "../cvmfs/platform.h"
#include "../cvmfs/fs_traversal.h"


class TraversalDelegate {
 public:
  TraversalDelegate() {}

  void FileCb(const std::string      &relative_path,
              const std::string      &file_name,
              const platform_stat64  &info) {
    const std::string path = relative_path + "/" + file_name;
    File *file = new File(path, info);
    IoDispatcher::Instance()->ScheduleRead(file);
  }
};






int main() {
  tbb::tick_count start, end;
  tbb::tick_count all_start, all_end;

  //RaiseFileDescriptorLimit(100000);

  all_start = tbb::tick_count::now();

  TraversalDelegate delegate;

  start = tbb::tick_count::now();
  FileSystemTraversal<TraversalDelegate> t(&delegate, "", true);
  t.fn_new_file = &TraversalDelegate::FileCb;
  t.Recurse(input_path);
  end = tbb::tick_count::now();
  std::cout << "recursion took:   " << (end - start).seconds() << " seconds" << std::endl;

  Print("going to wait now...");
  IoDispatcher::Instance()->Wait();
  Print("waited...");

#ifdef MEASURE_ALLOCATION_TIME
  std::cout << "allocations took: " << ((double)CharBuffer::allocation_time_   / (double)kTimeResolution) << " seconds" << std::endl;
  std::cout << "deallocs took:    " << ((double)CharBuffer::deallocation_time_ / (double)kTimeResolution) << " seconds" << std::endl;
  std::cout << "leaked buffers:   " << CharBuffer::active_instances_ << std::endl;
#endif

  IoDispatcher::Destroy();

  all_end = tbb::tick_count::now();
  std::cout << "overall time:     " << (all_end - all_start).seconds() << " seconds" << std::endl;

  return 0;
}

