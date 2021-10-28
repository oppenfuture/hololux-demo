from pathlib import Path
import re, shutil, sys

class Library:
  def __init__(self):
    self.include_folder = Path()
    self.lib_names: List[str] = []
    self.lib_folders: List[Path] = []

class LibFinder:
  def __init__(self, build_folder: Path):
    self.build_folder = build_folder

  def find(self, library: str) -> Library:
    result = Library()
    cmake_file = self.build_folder / f'Find{library}.cmake'
    cmake_content = cmake_file.read_text()

    # search for set(library_INCLUDE_DIR "...")
    pattern = r'set\({}_INCLUDE_DIR \"(.+)\"\)'.format(library)
    match = re.search(pattern, cmake_content, re.MULTILINE)
    if match is None or not match.group(1):
      raise RuntimeError(f'Failed to find include directory of {library}')
    result.include_folder = Path(match.group(1))

    # search for set(library_LIBRARY_LIST ...)
    pattern = r'set\({}_LIBRARY_LIST (.+)\)'.format(library)
    match = re.search(pattern, cmake_content, re.MULTILINE)
    if match is None or not match.group(1):
      print(f'{library} looks like header only')
    else:
      result.lib_names = match.group(1).split()
      # search for set(library_LIB_DIRS "...")
      pattern = r'set\({}_LIB_DIRS \"(.+)\"\)'.format(library)
      match = re.search(pattern, cmake_content, re.MULTILINE)
      if match is None or not match.group(1):
        raise RuntimeError(f'Failed to find lib directories of {library}')
      result.lib_folders = [Path(p) for p in match.group(1).split()]

    return result

class Collector:
  def __init__(self, dst_folder: Path):
    self.include_folder = dst_folder / 'include'
    self.include_folder.mkdir(exist_ok=True, parents=True)
    for item in self.include_folder.iterdir():
      if item.is_dir():
        shutil.rmtree(item)
      else:
        item.unlink()

    self.lib_folder = dst_folder / 'lib'
    self.lib_folder.mkdir(exist_ok=True)

  def collect_library(self, library: Library):
    for item in library.include_folder.iterdir():
      if item.is_dir():
        shutil.copytree(item, self.include_folder / item.name)
      else:
        shutil.copy(item, self.include_folder / item.name)

    for name in library.lib_names:
      found = False
      for folder in library.lib_folders:
        src = folder / f'lib{name}.a'
        if src.exists():
          shutil.copy(src, self.lib_folder)
          found = True
          break
      if not found:
        raise RuntimeError(f'Library {src.name} not found in {library.lib_folders}')

  def collect_hololux(self, build_folder: Path):
    wolf_header_folder = build_folder / '..' / '..' / 'wolf' / 'include'
    shutil.copy(wolf_header_folder / 'HololuxRenderer.h', self.include_folder)
    shutil.copy(wolf_header_folder / 'NetworkInterface.h', self.include_folder)
    shutil.copy(build_folder / 'libwolf.a', self.lib_folder)
    shutil.copy(build_folder / 'libhololux.so', self.lib_folder)

if __name__ == '__main__':
  if len(sys.argv) < 3:
    print(f'Usage: {sys.argv[0]} build_folder output_folder')
    exit(1)

  finder = LibFinder(Path(sys.argv[1]))
  collector = Collector(Path(sys.argv[2]))
  collector.collect_hololux(finder.build_folder)
  for library in ['mineola', 'glm', 'imgpp', 'libjpeg-turbo', 'PNG', 'ZLIB']:
    collector.collect_library(finder.find(library))
