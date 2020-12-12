Cali: Compiler Assisted Library Isolation
=========================================

Installation
------------
Extract archive into a folder of your choice. Afterwards create two symlinks:
```shell script
sudo ln -s $(pwd)/cali-linker/cali /usr/local/bin/ld.cali
sudo ln -s $(pwd)/precompiled-libraries /usr/cali-lib
```


How to isolate a library
------------------------
Isolating one or multiple libraries from a program is usually a three-step process. 
First, you need to get the program compiled with clang 7 and link-time optimization. 
Second, you need to create a config file that names the libraries to be isolated, and potential compatibility options. 
Third, you can specify the desired permission configuration. 


### 1. Compile a program with Clang 7 / LTO / Cali linker
In most build systems, it is sufficient to set `CFLAGS`, `CXXFLAGS`, `LDFLAGS` and maybe some other environment variables. 
This example code is for the common automake build system (`./configure` scripts).
Default config file location is given in `LDFLAGS`.

```shell script
export CFLAGS="-flto -O2"
export CXXFLAGS="-flto -O2"
export LDFLAGS="-flto -fuse-ld=cali -Wl,--cali-config=`pwd`/config.yaml"  # config file in current directory
export CC=clang-7
export CXX=clang++-7
export RANLIB=llvm-ranlib-7
```

If you want the result in a custom installation directory:
```shell script
INSTALL_DIR=`pwd`/install
mkdir -p "$INSTALL_DIR"
export ACLOCAL_PATH="$INSTALL_DIR/automake"
export CFLAGS="-isystem $INSTALL_DIR/include -I$INSTALL_DIR/include -flto -O2"
export CXXFLAGS="-isystem $INSTALL_DIR/include -I$INSTALL_DIR/include -flto -O2"
export LDFLAGS="-L$INSTALL_DIR/lib -Wl,-rpath=$INSTALL_DIR/lib -flto -fuse-ld=cali -Wl,--cali-config=`pwd`/config.yaml"
export PATH=$INSTALL_DIR/bin:$PATH
export CC=clang-7
export CXX=clang++-7
export RANLIB=llvm-ranlib-7
```

The configuration file does not need to exist in this step, the cali linker is not isolating anything, but creating template configuration files.

- Configure with `CALI_IGNORE_MISSING_CONFIG` (ignore missing config file): `CALI_IGNORE_MISSING_CONFIG=1 ./configure --prefix="$INSTALL_DIR" --disable-shared`
- Do a first build with `CALI_GENERATE_CONFIG_TEMPLATES` (create boilerplate code for missing config files): `CALI_GENERATE_CONFIG_TEMPLATES=1 make -j $(nproc)`
- Test that the built binaries are functional (e.g. clang did not break anything). Usually: `CALI_IGNORE_MISSING_CONFIG=1 make check`

### Hints and tricks: 
- Sometimes libtool has problems with clang/lld, ignoring some parameters. Typical error: `/usr/bin/ld: unrecognized option '--ipc-config=...'`. 
  Solution: apply the script `patch-libtool.sh`
- Configure doesn't work? Try `autoreconf`

### 2. Create the configuration file
In this step, you'll create a config file for isolation and ensure that the split-up application is still functional.

You should find config templates (*.yaml.tmpl.*) in the folder where your config file should be. 
Use one of these files to start your `config.yaml`, and merge the other templates if necessary (they might contain different subsets of selectors, for example).

Uncomment the selectors for each library you want to isolate and select an allow-all permission (mount / read-write, network allowed etc.).
Recompile the programs: remove (`rm`) the binary and rebuild using `make` - no need to rebuild all object files.
Afterwards, check again that Cali did not break anything: `make check`

#### Tweaks to the analysis
- `strongFPAnalysis: true`: use advanced indirect call analysis. 
  Symptoms: Segfaults and potentially other errors. Side effect: Can increase compilation time, should not break functionality.
- `strongFDAnalysis: true`: use advanced file descriptor heuristics (likely not necessary).
  Symptoms: Errors like `Bad file descriptor`.
  Side effect: Strange / wrong program behavior if used incorrectly / error in `sendmsg: Bad file descriptor`. 
- `concurrentLibraryCommunication`: enable for multithreaded programs.
  Side effect: Can lead to segfaults in rare cases. 
- `programIsForking`: enable for programs that do non-trivial forks (e.g. forks other than fork/exec).
  Side effect: Can lead to segfaults in rare cases. 
- `silent`: Set to `1` if debug output from the isolation is breaking tests.
  Side effect: less output
- `linkerOverride`: Set to `false` if symbol errors occur (like: `undefined symbol: non-virtual thunk to ...`)

### 3. Restrict library permissions
Now you can refine the permission configuration. 
For options see the configuration reference below. 
After setting permissions, remove the generated binaries and re-run `make`.
Check that the program still works afterwards.

Some useful tricks:
- You can use tools like `strace` or `ltrace` to see which files/directories an application uses. 
  You can also use these tools to debug errors induced by wrong permission config (`strace -f`).
- You can use variables (`$HOME`, `$ARGV`, `$ARGV_FOLDERS`, `$CWD`) to chose accessible folders at launch time.
- The files/folders you name should exist when the program is launched. This is in particular problematic for output files:
  In this case, you have to allow access to the folder where the output file should be created.
  If the name of the output file is given as command line parameter, you can use `$ARGV_FOLDERS` for this job.
- Usually a good idea to share: `/dev/urandom` and `/dev/random` (both read-only).
- A logfile `<binaryname>.log` is created for each library, containing a list of used library functions



Configuration Format
--------------------
A config file is always in YAML format.
This config file specifies two contexts: `main` and `library`. 

There are global options that specify global analysis details. 
The context configuration specifies per-context details, including which input files (object files / libraries) belong into which context.
The `library` context has an additional permission configuration.  

Suggested minimal configuration with most important options:
```yaml
---

linkerOverride: false
addLibstdc++: false
strongFPAnalysis: false
strongFDAnalysis: false

contexts:
  main:
    programIsForking: false
    concurrentLibraryCommunication: false
    selectors:
      - "*.o"
  library:
    selectors:
      - "libabc.so"
    permissions:
      readonly:
        - /
      readwrite:
        - "$ARGV"
      network: none  # all/host, local, none
```

All options:
```yaml
---

addLibstdc++: false
linkerOverride: false
limit_subnode_depth: 3
limit_struct_parts: 32
strongFPAnalysis: false
strongFDAnalysis: false
filedescriptors: {}  # Map ["function name" => argument number] to support FD detection
replaceArguments: {}
ignoreOutputs: []

contexts:
  main:
    mprotect_mode: false
    sequential_mode: false
    programIsForking: false
    silent: 0
    concurrentLibraryCommunication: false
    function_behavior: {}
    selectors:
      - "*.o"
      - "libstdc++.a"
  library:
    selectors:
      - "libabc.so"
    ignored_functions: []
    permissions:
      readonly:
        - /bin
        - /lib64
        - /dev/urandom
      readwrite:
        - "$HOME/.config/abc"
        - "$ARGV"
      tmpfs:
        - /dev/random_tmpfs
      user: 65534  # = nobody
      group: 65534  # = nogroup
      network: local  # all/host, local, none
      seccomp: |
        DEFAULT ALLOW
      max_cpus: 0  # 0 = all
      capabilities: []  # list of capabilities that are retained / not dropped
      silent: false
      rlimit:
        as: ""  # address space in MB
        core: ""  # core file size in MB
        cpu: ""  # CPU time in sec
        fsize: ""  # File size in MB
        nofile: ""  # number of open FDs
        nproc: ""  # number of PIDs
        stack: ""  # stack size in MB
      cgroup_mem_max: 0
      cgroup_pids_max: 0
      cgroup_cpu_ms_per_sec: 0
```

### Global options
| Option | default | Explanation |
| ------ | ------- | ----------- |
| addLibstdc++ | `false` | Add an analyzable (LLVM-IR) version of `libstdc++` |
| filedescriptors | `{}` | Mark additional function parameters as file descriptor (format: `{"function_name": argument_number}`) |
| ignoreOutputs | `[]` | A list of output files (`-o ...`) that should not be processed |
| limit_subnode_depth | `3` | Subnode generation limit |
| limit_struct_parts | `32` | Struct subnode generation limit |
| linkerOverride | `false` | Use LLVM's flag `OverrideFromSrc` to link LLVM modules |
| replaceArguments | `{}` | Replace link-time arguments (for example: `{"-llibname": "/usr/lib/mylib.so"}`) |
| strongFPAnalysis | `false` | Use advanced function pointer analysis (try to follow indirect calls) |
| strongFDAnalysis | `false` | Use heuristic file descriptor detection |

### Context options

| Option | default | Explanation |
| ------ | ------- | ----------- |
| concurrentLibraryCommunication | `false` | Must be `true` if multiple program threads might try to use the library interface at the same time |
| function_behavior | `{}` | Mark functions from this context as heap functions, by naming their libc counterpart. Example: `{"AcquireMagickMemory": "malloc"}` |
| ignored_functions | `[]` | Functions to ignore in analysis and isolation |
| mprotect_mode | `false` | Use `mprotect` before each library call to protect shared memory against concurrency attacks |
| programIsForking | `false` | Must be `true` if the program uses non-trivial forks (and these forks use the library), to prevent deadlocking. |
| sequential_mode | `false` | Enforce that only one thread in the library context is running code, to protect shared memory against concurrency attacks |
| silent | `0` | Silence some runtime warnings. Possible values (can be combined): `0` = none, `1` = silence shared memory warnings, `2` = redirect stderr to `/tmp/stderr` |
| selectors | `[]` | A list of glob patterns naming the linker input files that belong into this context. Examples: `*.o` (all object files), `libsqlite3.so` (sqlite) etc. |

### Permission options

| Option | default | Explanation |
| ------ | ------- | ----------- |
| readonly | `[]` | A list of folders that are accessible readonly. Examples: <ul><li>`/usr`</li><li>`/dev/urandom`</li><li>`$ARGV` (all files/folders named as program arguments)</li><li>`$ARGV_FOLDERS` (the parent folders of all files named in program arguments - useful if output files do not exist at program start)</li><li>`$HOME/.config/application` (access to config folder in home directory)</li><li>`$CWD` (current working directory)</li></ul> |
| readwrite | `[]` | A list of folder available read-write, same options as `readonly` |
| tmpfs | `[]` | A list of folders to mount a private temporary filesystem, same options as `readonly` |
| user / group | `65534` | The used ID to use inside the user namespace. Does not apply to file permissions etc. |
| network | `local` | Allow or deny access to network interfaces. Options: `all`, `local` (only access to localhost), `none` (no network) |
| seccomp | `"DEFAULT ALLOW"` | A seccomp policy as kafel input (can deny certain syscalls) |
| max_cpus | `0` | Number of CPU cores the library has access to. `0` = all |
| capabilities | `[]` | A list of capabilities that should be retained (by default: all dropped) |
| silent | `false` | Silence permission warnings |
| rlimit | `{}` | Limit physical resources. Options: `as`, `core`, `cpu`, `fsize`, `nofile`, `nproc`, `stack` |
| cgroup_mem_max | `0` | CGroup memory limits (`0` = no limit) |
| cgroup_pids_max | `0` | CGroup pid limit (`0` = no limit) |
| cgroup_cpu_ms_per_sec | `0` | CGroup option to limit CPU time (`0` = no limit) |

### Example configuration
Configuration for ImageMagick, taken from the paper:
```yaml
---

contexts:
  main:
    selectors: 
      - "*.o"
      - "libMagick*.a"
    function_behavior:
      AcquireMagickMemory: malloc
      RelinquishMagickMemory: free
  library:
    selectors: 
      - libgs.so  # Ghostscript
    permissions:
      readonly:
        - /var/lib/ghostscript
        - /dev/urandom
      readwrite:
          # folder containing input/output files in argv
        - "$ARGV_FOLDERS"
        - /tmp
      network: none
```

